
#pragma once

#include <ESP32Encoder.h>
#include <Menu.h>
#include <button.h>
#include <hardware/pinmapping.h>
#include <icons/menuIcons.h>

enum MENUINPUT {
    BUTTONS,
    ROTARY,
};

Menu* menu;

// TODO Menu: add setting to web config
#define MENU_INPUT MENUINPUT::ROTARY

GPIOPin* menuEnterPin;
GPIOPin* menuUpPin;
GPIOPin* menuDownPin;
ESP32Encoder encoder;
QueueHandle_t button_events;
button_event_t ev;
bool invertMenuInput;
bool invertScrollInput;

int last = 0;

template <typename T>
inline auto makeSaveCallback(const char* param, T& value) {
    return [param, &value]() {
        if (!ParameterRegistry::getInstance().setParameterValue(param, value)) {
            LOG(ERROR, "Failed to save config to filesystem!");
        }
    };
}

template <typename T>
inline T& getMenuVar(const std::string& paramId) {
    static std::map<std::string, T> menuVars;

    if (menuVars.find(paramId) == menuVars.end()) {
        menuVars[paramId] = config.get<T>(paramId.c_str());
    }

    return menuVars[paramId];
}

// Spezieller Helper für MenuInfoEntry (gibt const char** zurück)
inline const char** getMenuInfoVar(const std::string& paramId) {
    // 1. Map hält das eigentliche Arduino String-Objekt dauerhaft am Leben
    static std::map<std::string, String> infoStrings;
    // 2. Map hält den dazugehörigen const char* Pointer, dessen Adresse das Menü braucht
    static std::map<std::string, const char*> infoPointers;

    // Nur beim ersten Laden abrufen
    if (infoStrings.find(paramId) == infoStrings.end()) {
        // String aus der Config laden
        infoStrings[paramId] = config.get<String>(paramId.c_str());

        // Den rohen C-String Pointer in der zweiten Map speichern
        infoPointers[paramId] = infoStrings[paramId].c_str();
    }

    // Wir geben die SPEICHERADRESSE des Pointers zurück (const char**)
    return &infoPointers[paramId];
}

// Gibt das Array der Enum-Optionen zurück
inline const char* const* getMenuEnumOptions(const std::string& paramId) {
    auto param = ParameterRegistry::getInstance().getParameterById(paramId.c_str());
    if (param && param->isEnum()) {
        return param->getEnumOptions();
    }
    return nullptr;
}
// Gibt die Anzahl der Enum-Optionen zurück
inline size_t getMenuEnumCount(const std::string& paramId) {
    auto param = ParameterRegistry::getInstance().getParameterById(paramId.c_str());
    if (param && param->isEnum()) {
        return param->getEnumCount();
    }
    return 0;
}

bool hasScale() {
    return config.get<bool>("hardware.sensors.scale.enabled");
}

void saveInputInvert() {
    menu->InvertMenuInput(reinterpret_cast<bool&>(menuInputInvert));
    //sysParaDisplayMenuInvert.setStorage(true);
}

void saveScrollInvert() {
    menu->InvertScrollInput(reinterpret_cast<bool&>(menuScrollInvert));
    //sysParaDisplayMenuScrollInvert.setStorage(true);
}


void menuInputInit() {
    switch (MENU_INPUT) {
        case MENUINPUT::BUTTONS:
            menuEnterPin = new GPIOPin(PIN_MENU_ENTER, GPIOPin::IN_PULLUP);
            menuUpPin = new GPIOPin(PIN_MENU_OUT_A, GPIOPin::IN_PULLUP);
            menuDownPin = new GPIOPin(PIN_MENU_OUT_B, GPIOPin::IN_PULLUP);

            button_events = pulled_button_init(PIN_BIT(menuEnterPin->getPinNumber()) | PIN_BIT(menuUpPin->getPinNumber()) | PIN_BIT(menuDownPin->getPinNumber()), GPIO_PULLUP_ONLY);


            break;
        case MENUINPUT::ROTARY:
            menuEnterPin = new GPIOPin(PIN_MENU_ENTER, GPIOPin::IN_PULLUP);
            menuUpPin = new GPIOPin(PIN_MENU_OUT_A, GPIOPin::IN_PULLUP);
            menuDownPin = new GPIOPin(PIN_MENU_OUT_B, GPIOPin::IN_PULLUP);

            button_events = pulled_button_init(PIN_BIT(menuEnterPin->getPinNumber()), GPIO_PULLUP_ONLY);

            encoder.useInternalWeakPullResistors = puType::up;
            encoder.attachFullQuad(PIN_MENU_OUT_A, PIN_MENU_OUT_B);
            encoder.setCount(0);


            break;
        default:
            break;
    }
}

void initMenu(U8G2& display) {
    menu = new Menu(display);

    auto& params = ParameterRegistry::getInstance();

    menuInputInit();

    menu->InvertScrollInput(true);
    menu->InvertMenuInput(true);

    /* Main Menu */
    menu->AddInputItem("Brew Temp.", "Brew Temperature", "", "°C", BREW_SETPOINT_MIN, BREW_SETPOINT_MAX, makeSaveCallback("brew.setpoint", getMenuVar<double>("brew.setpoint")), getMenuVar<double>("brew.setpoint"), bitmap_icon_temp, 0.1, 0.5);
    menu->AddInputItem("Steam Temp.", "Steam Temperature", "", "°C", STEAM_SETPOINT_MIN, STEAM_SETPOINT_MAX, makeSaveCallback("steam.setpoint", getMenuVar<double>("steam.setpoint")), getMenuVar<double>("steam.setpoint"), bitmap_icon_steam, 0.1, 0.5);

    menu->AddToggleItem("PID", makeSaveCallback("pid.enabled", getMenuVar<bool>("pid.enabled")), getMenuVar<bool>("pid.enabled"), bitmap_icon_pid);

    menu->SetEventHandler([&]() {
        if (xQueueReceive(button_events, &ev, 1 / portTICK_PERIOD_MS)) {
            if (ev.pin == menuEnterPin->getPinNumber()) {
                if (standbyModeRemainingTimeMillis == 0) {
                    resetStandbyTimer();
                    display.setPowerSave(0);
                    pidON = true;
                    if (steamON) {
                        machineState = kSteam;
                    }
                    else if (checkBrewActive()) {
                        machineState = kBrew;
                    }
                    else {
                        machineState = kPidDisabled;
                    }
                    return;
                }
                if (ev.event == EventState::STATE_DOWN) {
                    resetStandbyTimer();
                }
                menu->Event(EVENT_ENTER, EventState(ev.event));
            }
            else {
                if (MENU_INPUT == MENUINPUT::BUTTONS) {
                    if (ev.pin == menuUpPin->getPinNumber()) {
                        resetStandbyTimer();
                        menu->Event(EVENT_UP, EventState(ev.event));
                    }
                    else if (ev.pin == menuDownPin->getPinNumber()) {
                        resetStandbyTimer();
                        menu->Event(EVENT_DOWN, EventState(ev.event));
                    }
                }
            }
        }
        if (MENU_INPUT == MENUINPUT::ROTARY) {
            int32_t pos = encoder.getCount() / ENCODER_CLICKS_PER_NOTCH;
            if (pos < last) {
                menu->Event(EVENT_UP, EventState(EventState::STATE_DOWN));
                LOG(DEBUG, "Menu: Up");
                menu->Event(EVENT_UP, EventState(EventState::STATE_UP));
            }
            else if (pos > last) {
                menu->Event(EVENT_DOWN, EventState(EventState::STATE_DOWN));
                LOG(DEBUG, "Menu: Down");
                menu->Event(EVENT_DOWN, EventState(EventState::STATE_UP));
            }

            last = pos;
        }
    });

    Menu* m_pidGeneral = new Menu(display);
    m_pidGeneral->AddToggleItem("PID Enabled", makeSaveCallback("pid.enabled", getMenuVar<bool>("pid.enabled")), getMenuVar<bool>("pid.enabled"), bitmap_icon_pid);
    m_pidGeneral->AddToggleItem("Use PonM", makeSaveCallback("pid.use_ponm", getMenuVar<bool>("pid.use_ponm")), getMenuVar<bool>("pid.use_ponm"));
    m_pidGeneral->AddInputItem("EMA Factor", "EMA Factor", "", "", PID_EMA_FACTOR_MIN, PID_EMA_FACTOR_MAX, makeSaveCallback("pid.ema_factor", getMenuVar<double>("pid.ema_factor")), getMenuVar<double>("pid.ema_factor"));
    m_pidGeneral->AddBackItem("Back", bitmap_icon_back);

    Menu *m_pidRegular = new Menu(display);
    m_pidRegular->AddInputItem("Kp", "Kp", "", "", PID_KP_REGULAR_MIN, PID_KP_REGULAR_MAX, makeSaveCallback("pid.regular.kp", getMenuVar<double>("pid.regular.kp")), getMenuVar<double>("pid.regular.kp"));
    m_pidRegular->AddInputItem("Tn", "Tn", "", "", PID_TN_REGULAR_MIN, PID_TN_REGULAR_MAX, makeSaveCallback("pid.regular.tn", getMenuVar<double>("pid.regular.tn")), getMenuVar<double>("pid.regular.tn"));
    m_pidRegular->AddInputItem("Tv", "Tv", "", "", PID_TV_REGULAR_MIN, PID_TV_REGULAR_MAX, makeSaveCallback("pid.regular.tv", getMenuVar<double>("pid.regular.tv")), getMenuVar<double>("pid.regular.tv"));
    m_pidRegular->AddInputItem("I Max", "I Max", "", "", PID_I_MAX_REGULAR_MIN, PID_I_MAX_REGULAR_MAX, makeSaveCallback("pid.regular.i_max", getMenuVar<double>("pid.regular.i_max")), getMenuVar<double>("pid.regular.i_max"));
    m_pidRegular->AddBackItem("Back", bitmap_icon_back);

    Menu *m_pidBrewDetection = new Menu(display);
    m_pidBrewDetection->AddToggleItem("Brew Detection", makeSaveCallback("pid.bd.enabled", getMenuVar<bool>("pid.bd.enabled")), getMenuVar<bool>("pid.bd.enabled"), bitmap_icon_pid);
    m_pidBrewDetection->AddInputItem("Kp", "Kp", "", "", PID_KP_BD_MIN, PID_KP_BD_MAX, makeSaveCallback("pid.bd.kp", getMenuVar<double>("pid.bd.kp")), getMenuVar<double>("pid.bd.kp"));
    m_pidBrewDetection->AddInputItem("Tn", "Tn", "", "", PID_TN_BD_MIN, PID_TN_BD_MAX, makeSaveCallback("pid.bd.tn", getMenuVar<double>("pid.bd.tn")), getMenuVar<double>("pid.bd.tn"));
    m_pidBrewDetection->AddInputItem("Tv", "Tv", "", "", PID_TV_BD_MIN, PID_TV_BD_MAX, makeSaveCallback("pid.bd.tv", getMenuVar<double>("pid.bd.tv")), getMenuVar<double>("pid.bd.tv"));
    m_pidBrewDetection->AddBackItem("Back", bitmap_icon_back);

    Menu *m_pidSteam = new Menu(display);
    m_pidSteam->AddInputItem("Kp", "Kp", "", "", PID_KP_STEAM_MIN, PID_KP_STEAM_MAX, makeSaveCallback("pid.steam.kp", getMenuVar<double>("pid.steam.kp")), getMenuVar<double>("pid.steam.kp"));
    m_pidSteam->AddInputItem("Setpoint", "Setpoint", "", "s", STEAM_SETPOINT_MIN, STEAM_SETPOINT_MAX, makeSaveCallback("steam.setpoint", getMenuVar<double>("steam.setpoint")), getMenuVar<double>("steam.setpoint"), bitmap_icon_temp);
    m_pidSteam->AddBackItem("Back", bitmap_icon_back);

    Menu *m_brewSettings = new Menu(display);
    m_brewSettings->AddInputItem("Setpoint", "Setpoint", "", "s", BREW_SETPOINT_MIN, BREW_SETPOINT_MAX, makeSaveCallback("brew.setpoint", getMenuVar<double>("brew.setpoint")), getMenuVar<double>("brew.setpoint"));
    m_brewSettings->AddInputItem("Temp Offset", "Temp Offset", "", "°C", BREW_TEMP_OFFSET_MIN, BREW_TEMP_OFFSET_MAX, makeSaveCallback("brew.temp_offset", getMenuVar<double>("brew.temp_offset")), getMenuVar<double>("brew.temp_offset"), bitmap_icon_temp);
    m_brewSettings->AddInputItem("PID Delay", "PID Delay", "", "s", BREW_PID_DELAY_MIN, BREW_PID_DELAY_MAX, makeSaveCallback("brew.pid_delay", getMenuVar<double>("brew.pid_delay")), getMenuVar<double>("brew.pid_delay"), bitmap_icon_pid);
    m_brewSettings->AddToggleItem("By Time", makeSaveCallback("brew.by_time.enabled", getMenuVar<bool>("brew.by_time.enabled")), getMenuVar<bool>("brew.by_time.enabled"), bitmap_icon_clock;
    m_brewSettings->AddInputItem("Target Time", "Target Time", "", "s", TARGET_BREW_TIME_MIN, TARGET_BREW_TIME_MAX, makeSaveCallback("brew.by_time.target_time", getMenuVar<double>("brew.by_time.target_time")), getMenuVar<double>("brew.by_time.target_time"), bitmap_icon_clock);
    m_brewSettings->AddBackItem("Back", bitmap_icon_back);

    Menu *m_preInfusion = new Menu(display);
    m_preInfusion->AddToggleItem("Pre Infusion", makeSaveCallback("brew.pre_infusion.enabled", getMenuVar<bool>("brew.pre_infusion.enabled")), getMenuVar<bool>("brew.pre_infusion.enabled"));
    m_preInfusion->AddInputItem("Time", "Time", "", "s", PRE_INFUSION_TIME_MIN, PRE_INFUSION_TIME_MAX, makeSaveCallback("brew.pre_infusion.time", getMenuVar<double>("brew.pre_infusion.time")), getMenuVar<double>("brew.pre_infusion.time"), bitmap_icon_clock);
    m_preInfusion->AddInputItem("Pause", "Pause", "", "s", PRE_INFUSION_PAUSE_MIN, PRE_INFUSION_PAUSE_MAX, makeSaveCallback("brew.pre_infusion.pause", getMenuVar<double>("brew.pre_infusion.pause")), getMenuVar<double>("brew.pre_infusion.pause"), bitmap_icon_clock);
    m_preInfusion->AddBackItem("Back", bitmap_icon_back);

    Menu *m_backflushing = new Menu(display);
    m_backflushing->AddInputItem("Cycles", "Cycles", "", "", BACKFLUSH_CYCLES_MIN, BACKFLUSH_CYCLES_MAX, makeSaveCallback("brew.backflushing.cycles", getMenuVar<double>("brew.backflushing.cycles")), getMenuVar<double>("brew.backflushing.cycles"));
    m_backflushing->AddInputItem("Fill Time", "Fill Time", "", "s", BACKFLUSH_FILL_TIME_MIN, BACKFLUSH_FILL_TIME_MAX, makeSaveCallback("brew.backflushing.fill_time", getMenuVar<double>("brew.backflushing.fill_time")), getMenuVar<double>("brew.backflushing.fill_time"), bitmap_icon_clock);
    m_backflushing->AddInputItem("Flush Time", "Flush Time", "", "s", BACKFLUSH_FLUSH_TIME_MIN, BACKFLUSH_FLUSH_TIME_MAX, makeSaveCallback("brew.backflushing.flush_time", getMenuVar<double>("brew.backflushing.flush_time")), getMenuVar<double>("brew.backflushing.flush_time"), bitmap_icon_clock);
    m_backflushing->AddBackItem("Back", bitmap_icon_back);

    Menu *m_standby = new Menu(display);
    m_standby->AddToggleItem("Standby", makeSaveCallback("standby.enabled", getMenuVar<bool>("standby.enabled")), getMenuVar<bool>("standby.enabled"), bitmap_icon_power);
    m_standby->AddInputItem("Time", "Time", "", "s", STANDBY_MODE_TIME_MIN, STANDBY_MODE_TIME_MAX, makeSaveCallback("standby.time", getMenuVar<double>("standby.time")), getMenuVar<double>("standby.time"), bitmap_icon_clock);
    m_standby->AddBackItem("Back", bitmap_icon_back);

    Menu *m_display = new Menu(display);
    m_display->AddToggleItem("Fullscreen Brew Timer", makeSaveCallback("display.fullscreen_brew_timer", getMenuVar<bool>("display.fullscreen_brew_timer")), getMenuVar<bool>("display.fullscreen_brew_timer"), bitmap_icon_clock);
    m_display->AddToggleItem("BLE Scale Brew Timer", makeSaveCallback("display.blescale_brew_timer", getMenuVar<bool>("display.blescale_brew_timer")), getMenuVar<bool>("display.blescale_brew_timer"), bitmap_icon_clock);
    m_display->AddToggleItem("Fullscreen Manual Flush Timer", makeSaveCallback("display.fullscreen_manual_flush_timer", getMenuVar<bool>("display.fullscreen_manual_flush_timer")), getMenuVar<bool>("display.fullscreen_manual_flush_timer"), bitmap_icon_clock);
    m_display->AddToggleItem("Fullscreen Hot Water Timer", makeSaveCallback("display.fullscreen_hot_water_timer", getMenuVar<bool>("display.fullscreen_hot_water_timer")), getMenuVar<bool>("display.fullscreen_hot_water_timer"), bitmap_icon_clock);
    m_display->AddInputItem("Post Brew Timer Duration", "Post Brew Timer Duration", "", "", POST_BREW_TIMER_DURATION_MIN, POST_BREW_TIMER_DURATION_MAX, makeSaveCallback("display.post_brew_timer_duration", getMenuVar<double>("display.post_brew_timer_duration")), getMenuVar<double>("display.post_brew_timer_duration"), bitmap_icon_clock);
    m_display->AddToggleItem("Heating Logo", makeSaveCallback("display.heating_logo", getMenuVar<bool>("display.heating_logo")), getMenuVar<bool>("display.heating_logo"));
    m_display->AddBackItem("Back", bitmap_icon_back);

    Menu *m_hw_relay = new Menu(display);
    m_hw_relay->AddToggleItem("Heater Trigger High", makeSaveCallback("hardware.relays.heater.trigger_type", getMenuVar<bool>("hw.relay.heater_trigger_high")), getMenuVar<bool>("hw.relay.heater_trigger_high"));
    m_hw_relay->AddToggleItem("Valve Trigger High", makeSaveCallback("hardware.relays.valve.trigger_type", getMenuVar<bool>("hw.relay.valve_trigger_high")), getMenuVar<bool>("hw.relay.valve_trigger_high"));
    m_hw_relay->AddToggleItem("Pump Trigger High", makeSaveCallback("hardware.relays.pump.trigger_type", getMenuVar<bool>("hw.relay.pump_trigger_high")), getMenuVar<bool>("hw.relay.pump_trigger_high"));
    m_hw_relay->AddBackItem("Back", bitmap_icon_back);

    Menu *m_hw_switch = new Menu(display);
    m_hw_switch->AddToggleItem("Brew Switch", makeSaveCallback("hardware.switches.brew.enabled", getMenuVar<bool>("hw.switches.brew.enabled")), getMenuVar<bool>("hw.switches.brew.enabled"));

    Menu *m_mqtt = new Menu(display);
    m_mqtt->AddToggleItem("MQTT", makeSaveCallback("mqtt.enabled", getMenuVar<bool>("mqtt.enabled")), getMenuVar<bool>("mqtt.enabled"));
    m_mqtt->AddToggleItem("Hass.io", makeSaveCallback("mqtt.hassio.enabled", getMenuVar<bool>("mqtt.hassio.enabled")), getMenuVar<bool>("mqtt.hassio.enabled"));

    std::vector<MenuInfoEntry> mqttInfo = {
        {"Broker", getMenuInfoVar("mqtt.broker")},
        {"Port", getMenuInfoVar("mqtt.port")},
        {"User", getMenuInfoVar("mqtt.username")},
        {"Pass", getMenuInfoVar("mqtt.password")},
        {"Topic", getMenuInfoVar("mqtt.topic")},
        {"Hass.io Prefix", getMenuInfoVar("mqtt.hassio.prefix")}
    };
    m_mqtt->AddInfoItem("Info", mqttInfo);
    m_mqtt->AddBackItem("Back", bitmap_icon_back);


    Menu *m_system_debug = new Menu(display);
    m_system_debug->AddToggleItem("Loop Timing", makeSaveCallback("system.timing_debug.enabled", getMenuVar<bool>("system.timing_debug.enabled")), getMenuVar<bool>("system.timing_debug.enabled"));
    m_system_debug->AddToggleItem("Display Log", makeSaveCallback("system.showdisplay.enabled", getMenuVar<bool>("system.showdisplay.enabled")), getMenuVar<bool>("system.showdisplay.enabled"));
    m_system_debug->AddBackItem("Back", bitmap_icon_back);

    std::vector<MenuInfoEntry> systemInfo = {
        {"Hostname", getMenuInfoVar("system.hostname")},
        {"OTA Pass", getMenuInfoVar("system.ota_password")},
        {"Auth User", getMenuInfoVar("system.auth.username")},
        {"Auth Pass", getMenuInfoVar("system.auth.password")},


    };

    Menu *m_system = new Menu(display);
    m_system->AddToggleItem("Offline Mode", makeSaveCallback("system.offline_mode", getMenuVar<bool>("system.offline_mode")), getMenuVar<bool>("system.offline_mode"));
    m_system->AddToggleItem("Auth", makeSaveCallback("system.auth.enabled", getMenuVar<bool>("system.auth.enabled")), getMenuVar<bool>("system.auth.enabled"));
    m_system->AddEnumItem("Log Level", "LogLevel", getMenuEnumOptions("system.log_level"), getMenuEnumCount("system.log_level"), getMenuVar<uint8_t>("system.log_level"), makeSaveCallback("system.log_level", getMenuVar<int>("system.log_level")), true);
    m_system->AddSubMenu("Debug", *m_system_debug, { config.get<int>("system.log_level") == static_cast<int>(Logger::Level::DEBUG)});
    m_system->AddInfoItem("Info", systemInfo);
    m_system->AddBackItem("Back", bitmap_icon_back);


    /*


            // Display
            // _configDefs.emplace("display.template", ConfigDef::forInt(0, 0, 4));
            // _configDefs.emplace("display.inverted", ConfigDef::forBool(false));
            // _configDefs.emplace("display.language", ConfigDef::forInt(1, 0, 2));


            // _configDefs.emplace("display.blinking.mode", ConfigDef::forInt(1, 0, 2));
            // _configDefs.emplace("display.blinking.delta", ConfigDef::forDouble(BLINKING_DELTA, BLINKING_DELTA_MIN, BLINKING_DELTA_MAX));

            // Hardware - OLED
            // _configDefs.emplace("hardware.oled.enabled", ConfigDef::forBool(true));
            // _configDefs.emplace("hardware.oled.type", ConfigDef::forInt(0, 0, 1));
            // _configDefs.emplace("hardware.oled.address", ConfigDef::forInt(0, 0, 1));

            // Hardware - Relays
            _configDefs.emplace("hardware.relays.heater.trigger_type", ConfigDef::forInt(Relay::HIGH_TRIGGER, 0, 1));
            _configDefs.emplace("hardware.relays.valve.trigger_type", ConfigDef::forInt(Relay::HIGH_TRIGGER, 0, 1));
            _configDefs.emplace("hardware.relays.pump.trigger_type", ConfigDef::forInt(Relay::HIGH_TRIGGER, 0, 1));

            // Hardware - Switches
            _configDefs.emplace("hardware.switches.brew.enabled", ConfigDef::forBool(false));
            _configDefs.emplace("hardware.switches.brew.type", ConfigDef::forInt(Switch::TOGGLE, 0, 2));
            _configDefs.emplace("hardware.switches.brew.mode", ConfigDef::forInt(Switch::NORMALLY_OPEN, 0, 1));
            _configDefs.emplace("hardware.switches.steam.enabled", ConfigDef::forBool(false));
            _configDefs.emplace("hardware.switches.steam.type", ConfigDef::forInt(Switch::TOGGLE, 0, 2));
            _configDefs.emplace("hardware.switches.steam.mode", ConfigDef::forInt(Switch::NORMALLY_OPEN, 0, 1));
            _configDefs.emplace("hardware.switches.power.enabled", ConfigDef::forBool(false));
            _configDefs.emplace("hardware.switches.power.type", ConfigDef::forInt(Switch::TOGGLE, 0, 2));
            _configDefs.emplace("hardware.switches.power.mode", ConfigDef::forInt(Switch::NORMALLY_OPEN, 0, 1));
            _configDefs.emplace("hardware.switches.hot_water.enabled", ConfigDef::forBool(false));
            _configDefs.emplace("hardware.switches.hot_water.type", ConfigDef::forInt(Switch::TOGGLE, 0, 2));
            _configDefs.emplace("hardware.switches.hot_water.mode", ConfigDef::forInt(Switch::NORMALLY_OPEN, 0, 1));

            // Hardware - LEDs
            _configDefs.emplace("hardware.leds.status.enabled", ConfigDef::forBool(false));
            _configDefs.emplace("hardware.leds.status.inverted", ConfigDef::forBool(false));
            _configDefs.emplace("hardware.leds.brew.enabled", ConfigDef::forBool(false));
            _configDefs.emplace("hardware.leds.brew.inverted", ConfigDef::forBool(false));
            _configDefs.emplace("hardware.leds.steam.enabled", ConfigDef::forBool(false));
            _configDefs.emplace("hardware.leds.steam.inverted", ConfigDef::forBool(false));

            // Hardware - Sensors
            _configDefs.emplace("hardware.sensors.temperature.type", ConfigDef::forInt(0, 0, 1));
            _configDefs.emplace("hardware.sensors.pressure.enabled", ConfigDef::forBool(false));
            _configDefs.emplace("hardware.sensors.watertank.enabled", ConfigDef::forBool(false));
            _configDefs.emplace("hardware.sensors.watertank.mode", ConfigDef::forInt(Switch::NORMALLY_CLOSED, 0, 1));

            // Scale
            _configDefs.emplace("hardware.sensors.scale.enabled", ConfigDef::forBool(false));
            _configDefs.emplace("hardware.sensors.scale.samples", ConfigDef::forInt(SCALE_SAMPLES, 1, 20));
            _configDefs.emplace("hardware.sensors.scale.type", ConfigDef::forInt(0, 0, 5));
            _configDefs.emplace("hardware.sensors.scale.calibration", ConfigDef::forDouble(SCALE_CALIBRATION_FACTOR, SCALE_CALIBRATION_MIN, SCALE_CALIBRATION_MAX));
            _configDefs.emplace("hardware.sensors.scale.calibration2", ConfigDef::forDouble(SCALE_CALIBRATION_FACTOR, SCALE_CALIBRATION_MIN, SCALE_CALIBRATION_MAX));
            _configDefs.emplace("hardware.sensors.scale.known_weight", ConfigDef::forDouble(SCALE_KNOWN_WEIGHT, SCALE_KNOWN_WEIGHT_MIN, SCALE_KNOWN_WEIGHT_MAX));


    // /* Brew Weight & Time */
    // Menu* weightNTime = new Menu(display);
    // weightNTime->AddInputItem("Brew by Time", "Brew Time", "", " s", TARGET_BREW_TIME_MIN, TARGET_BREW_TIME_MAX, makeSaveCallback("brew.by_time.target_time", getMenuVar<double>("brew.by_time.target_time")), getMenuVar<double>("brew.by_time.target_time"), bitmap_icon_clock);
    //
    // weightNTime->AddInputItem("Brew by Weight", "Brew Weight", "", "g", TARGET_BREW_WEIGHT_MIN, TARGET_BREW_WEIGHT_MAX, makeSaveCallback("brew.by_weight.target_weight",  getMenuVar<double>("brew.by_weight.target_weight")), getMenuVar<double>("brew.by_weight.target_weight"), bitmap_icon_scale, hasScale());
    // weightNTime->AddBackItem("Back", bitmap_icon_back);
    // menu->AddSubMenu(hasScale() ? "Brew Time & Weight" : "Brew Time", *weightNTime);
    //
    // /* Preinfusion */
    // Menu* preInfusion = new Menu(display);
    // preInfusion->AddInputItem("Preinfusion Pause", "Pause", "", "s", PRE_INFUSION_PAUSE_MIN, PRE_INFUSION_PAUSE_MAX, makeSaveCallback("brew.pre_infusion.pause", getMenuVar<double>("brew.pre_infusion.pause")), getMenuVar<double>("brew.pre_infusion.pause"), 1.0, 2.0, true);
    // preInfusion->AddInputItem("Preinfusion", "Time", "", "s", PRE_INFUSION_TIME_MIN, PRE_INFUSION_TIME_MAX, makeSaveCallback("brew.pre_infusion.time", getMenuVar<double>("brew.pre_infusion.time")), getMenuVar<double>("brew.pre_infusion.time"), 1.0, 2.0, true);
    // preInfusion->AddBackItem("Back", bitmap_icon_back);
    // menu->AddSubMenu("Preinfusion", *preInfusion, config.get<bool>("brew.pre_infusion.enabled"));
    // /*
    //  * Maintenance Menu
    //  * */
    // Menu* maintenanceMenu = new Menu(display);
    // maintenanceMenu->AddToggleItem("Backflush", getMenuVar<bool>("maintenance.backflush"), bitmap_icon_refresh);
    // maintenanceMenu->AddBackItem("Back", bitmap_icon_back);
    //
    // menu->AddSubMenu("Maintenance", *maintenanceMenu, bitmap_icon_tools);
    //
    // /*
    //  * Menu Menu
    //  */
    // Menu* menuMenu = new Menu(display);
    // menuMenu->AddToggleItem("Invert Input", saveInputInvert, menuInputInvert, true);
    // menuMenu->AddToggleItem("Invert Scroll", saveScrollInvert, menuScrollInvert, true);
    // menuMenu->AddBackItem("Back", bitmap_icon_back);
    // /*
    //  * Advanced Menu
    //  */
    //
    // Menu* advancedMenu = new Menu(display);
    // advancedMenu->AddInputItem("Brew Temp. Offset", "Brew temp. offset", "", "°C", BREW_TEMP_OFFSET_MIN, BREW_TEMP_OFFSET_MAX, makeSaveCallback("brew.temp_offset", brewTempOffset), brewTempOffset, bitmap_icon_temp);
    // advancedMenu->AddSubMenu("Menu", *menuMenu, true);
    //
    // /*
    //  * Standby Menu
    //  */
    // Menu* standbyMenu = new Menu(display);
    // standbyMenu->AddToggleItem("Standby", makeSaveCallback("standby.enabled", standbyModeOn), standbyModeOn, true);
    // standbyMenu->AddInputItem("Standby Time", "Standby Time", "", " m", STANDBY_MODE_TIME_MIN, STANDBY_MODE_TIME_MAX, makeSaveCallback("standby.time", standbyModeTime), standbyModeTime, bitmap_icon_clock, 1.0, 2.0, true);
    //
    // standbyMenu->AddBackItem("Back", bitmap_icon_back);
    // advancedMenu->AddSubMenu("Standby", *standbyMenu, bitmap_icon_sleep_mode);
    //
    // /* PID Settings */
    // Menu* pidSettings = new Menu(display);
    // pidSettings->AddToggleItem("Enable PonM", makeSaveCallback("pid.use_ponm", getMenuVar<bool>("pid.use_ponm")), getMenuVar<bool>("pid.use_ponm"));
    // pidSettings->AddInputItem("Start Kp", "Start Kp", "", "", PID_KP_START_MIN, PID_KP_START_MAX, []() { (sysParaPidKpStart.setStorage(true)); }, startKp);
    // pidSettings->AddInputItem("Start Tn", "Start Tn", "", "", PID_TN_START_MIN, PID_TN_START_MAX, []() { sysParaPidTnStart.setStorage(true); }, startTn);
    // pidSettings->AddInputItem("Kp", "Kp", "", "", PID_KP_REGULAR_MIN, PID_KP_REGULAR_MAX, []() { sysParaPidKpReg.setStorage(true); }, aggKp);
    // pidSettings->AddInputItem("Tn", "Tn (=Kp/Ki)", "", "", PID_TN_REGULAR_MIN, PID_TN_REGULAR_MAX, []() { sysParaPidTnReg.setStorage(true); }, aggTn);
    // pidSettings->AddInputItem("Tv", "Tv (=Kd/Kp)", "", "", PID_TV_REGULAR_MIN, PID_TV_REGULAR_MAX, []() { sysParaPidTvReg.setStorage(true); }, aggTv);
    // pidSettings->AddInputItem("Integrator Max", "Integrator Max", "", "", PID_I_MAX_REGULAR_MIN, PID_I_MAX_REGULAR_MAX, []() { sysParaPidIMaxReg.setStorage(true); }, aggIMax);
    // pidSettings->AddInputItem("Steam Kp", "Steam Kp", "", "", PID_KP_STEAM_MIN, PID_KP_STEAM_MAX, []() { sysParaPidKpSteam.setStorage(true); }, steamKp);
    //
    // /* Brew PID Settings */
    // Menu* brewPidSettings = new Menu(display);
    // brewPidSettings->AddToggleItem("Enable Brew PID", []() { sysParaUsePonM.setStorage(true); }, reinterpret_cast<bool&>(useBDPID));
    // brewPidSettings->AddInputItem("BD Kp", "BD Kp", "", "", PID_KP_BD_MIN, PID_KP_BD_MAX, []() { sysParaPidKpBd.setStorage(true); }, aggbKp);
    // brewPidSettings->AddInputItem("BD Tn", "BD Tn (=Kp/Ki)", "", "", PID_TN_BD_MIN, PID_TN_BD_MAX, []() { sysParaPidTnBd.setStorage(true); }, aggbTn);
    // brewPidSettings->AddInputItem("BD Tv", "BD Tv (=Kd/Kp)", "", "", PID_TV_BD_MIN, PID_TV_BD_MAX, []() { sysParaPidTvBd.setStorage(true); }, aggbTv);
    // brewPidSettings->AddInputItem("PID BD Time", "PID BD Time", "", "s", BREW_SW_TIME_MIN, BREW_SW_TIME_MAX, []() { sysParaBrewSwTime.setStorage(true); }, brewtimesoftware, hasSoftwareDetection());
    // brewPidSettings->AddInputItem("PID BD Sensitivity", "Sensitivity", "", "", BD_THRESHOLD_MIN, BD_THRESHOLD_MAX, []() { sysParaBrewThresh.setStorage(true); }, brewSensitivity, hasSoftwareDetection());
    // brewPidSettings->AddBackItem("Back", bitmap_icon_back);
    //
    // pidSettings->AddSubMenu("Brew PID", *brewPidSettings);
    // pidSettings->AddBackItem("Back", bitmap_icon_back);
    //
    // advancedMenu->AddSubMenu("PID Settings", *pidSettings, bitmap_icon_pid);
    // advancedMenu->AddBackItem("Back", bitmap_icon_back);
    // menu->AddSubMenu("Advanced", *advancedMenu, bitmap_icon_settings);
    //
    // menu->AddBackItem("Close Menu", bitmap_icon_back);
    menu->Init();
}

void menuLoop() {
    menu->Loop();
}
