/********************************************************
  Version 1.2 (01.03.2020)
******************************************************/

#define logo_width 23
#define logo_height 58
#define startLogo_width 45
#define startLogo_height 45
static const unsigned char PROGMEM logo_bits[] = {
0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x01, 0xff, 0x00, 0x03, 0x83, 0x80, 0x03, 0x01, 0x80, 0x06, 
  0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06, 0x00, 
  0xc0, 0x06, 0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06, 0x00, 0xc0, 
  0x06, 0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06, 
  0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06, 0x00, 
  0xc0, 0x06, 0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06, 0x00, 0xc0, 
  0x06, 0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06, 
  0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06, 0x00, 0xc0, 0x0e, 0x00, 0xe0, 0x18, 0x00, 0x70, 0x38, 0x00, 
  0x30, 0x30, 0x7c, 0x18, 0x30, 0xfe, 0x18, 0x61, 0xff, 0x18, 0x61, 0xff, 0x08, 0x61, 0xff, 0x08, 
  0x61, 0xff, 0x08, 0x21, 0xfe, 0x18, 0x30, 0xfc, 0x18, 0x30, 0x38, 0x30, 0x18, 0x00, 0x30, 0x0c, 
  0x00, 0xe0, 0x07, 0x01, 0xc0, 0x03, 0xff, 0x80, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00 };
// Rancilio Logo
 static const unsigned char PROGMEM startLogoRancilio_bits [] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x0f, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x1f, 0x80, 0x00, 0x7f, 0xf0, 0x00, 0x1f, 0xc0, 
	0x01, 0xff, 0xfc, 0x00, 0x1f, 0xe0, 0x03, 0xff, 0xfe, 0x00, 0x0f, 0xf0, 0x07, 0xff, 0xfe, 0x00, 
	0x07, 0xf8, 0x07, 0xf0, 0xff, 0x00, 0x03, 0xfc, 0x0f, 0xe0, 0x3f, 0x00, 0x01, 0xfe, 0x0f, 0xc0, 
	0x1f, 0x80, 0x00, 0xff, 0x0f, 0x80, 0x0f, 0x80, 0x00, 0x7f, 0x8f, 0x80, 0x0f, 0x80, 0x00, 0x3f, 
	0x8f, 0x80, 0x0f, 0x80, 0x00, 0x1f, 0xcf, 0x80, 0x0f, 0x80, 0x00, 0x1f, 0xcf, 0x80, 0x0f, 0x80, 
	0x00, 0x1f, 0x8f, 0x80, 0x1f, 0x80, 0x00, 0x3f, 0x8f, 0x80, 0x3f, 0x80, 0x00, 0x7f, 0x0f, 0x80, 
	0x3f, 0x00, 0x00, 0x7e, 0x0f, 0x80, 0x7f, 0x00, 0x00, 0xfc, 0x0f, 0x80, 0xfe, 0x00, 0x01, 0xfc, 
	0x0f, 0x81, 0xfc, 0x00, 0x03, 0xf8, 0x0f, 0x81, 0xf8, 0x00, 0x07, 0xf0, 0x0f, 0x83, 0xf8, 0x00, 
	0x07, 0xe0, 0x0f, 0x87, 0xf0, 0x00, 0x0f, 0xe0, 0x0f, 0x8f, 0xe0, 0x00, 0x0f, 0xc0, 0x0f, 0x9f, 
	0xc0, 0x00, 0x1f, 0x80, 0x0f, 0x9f, 0x80, 0x00, 0x1f, 0x80, 0x0f, 0xbf, 0x80, 0x00, 0x1f, 0x00, 
	0x0f, 0x9f, 0xc0, 0x00, 0x1f, 0x00, 0x0f, 0x9f, 0xe0, 0x00, 0x1f, 0x80, 0x1f, 0x8f, 0xf0, 0x00, 
	0x1f, 0x80, 0x1f, 0x87, 0xf8, 0x00, 0x0f, 0xc0, 0x3f, 0x03, 0xfc, 0x00, 0x0f, 0xe0, 0x7f, 0x01, 
	0xfe, 0x00, 0x07, 0xf9, 0xfe, 0x00, 0xff, 0x00, 0x07, 0xff, 0xfe, 0x00, 0x7f, 0x80, 0x03, 0xff, 
	0xfc, 0x00, 0x3f, 0x80, 0x01, 0xff, 0xf8, 0x00, 0x1f, 0x80, 0x00, 0x7f, 0xe0, 0x00, 0x0f, 0x80, 
	0x00, 0x1f, 0x80, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// Gaggia Logo
static const unsigned char PROGMEM startLogoGaggia_bits [] = { 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x0f, 0x00, 0x00, 0x78, 0x00, 0x00, 0x3c, 0x00, 0x00, 0xf0, 0x07, 0xc0, 0x00, 0xf0, 0x00, 
  0x00, 0x3f, 0x00, 0x00, 0x78, 0x00, 0x00, 0xfc, 0x00, 0x03, 0xf0, 0x07, 0xc0, 0x00, 0xf0, 0x00, 
  0x00, 0x7f, 0x00, 0x00, 0x78, 0x00, 0x01, 0xfc, 0x00, 0x07, 0xf0, 0x07, 0xc0, 0x00, 0xf0, 0x00, 
  0x00, 0xfe, 0x00, 0x00, 0x78, 0x00, 0x03, 0xf8, 0x00, 0x0f, 0xe0, 0x07, 0xc0, 0x00, 0xf0, 0x00, 
  0x01, 0xf8, 0x00, 0x00, 0xfc, 0x00, 0x07, 0xe0, 0x00, 0x1f, 0x80, 0x07, 0xc0, 0x01, 0xf8, 0x00, 
  0x03, 0xf0, 0x00, 0x00, 0xfc, 0x00, 0x0f, 0xc0, 0x00, 0x3f, 0x00, 0x07, 0xc0, 0x01, 0xf8, 0x00, 
  0x07, 0xe0, 0x00, 0x00, 0xfc, 0x00, 0x1f, 0x80, 0x00, 0x7e, 0x00, 0x07, 0xc0, 0x01, 0xf8, 0x00, 
  0x07, 0xc0, 0x00, 0x01, 0xfe, 0x00, 0x1f, 0x00, 0x00, 0x7c, 0x00, 0x07, 0xc0, 0x03, 0xfc, 0x00, 
  0x0f, 0x80, 0x00, 0x01, 0xfe, 0x00, 0x3e, 0x00, 0x00, 0xf8, 0x00, 0x07, 0xc0, 0x03, 0xfc, 0x00, 
  0x0f, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x3c, 0x00, 0x00, 0xf0, 0x00, 0x07, 0xc0, 0x03, 0xfc, 0x00, 
  0x1f, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x7c, 0x00, 0x01, 0xf0, 0x00, 0x07, 0xc0, 0x03, 0xfc, 0x00, 
  0x1e, 0x00, 0x00, 0x03, 0xcf, 0x00, 0x78, 0x00, 0x01, 0xe0, 0x00, 0x07, 0xc0, 0x07, 0x9e, 0x00, 
  0x1e, 0x00, 0x00, 0x03, 0xcf, 0x00, 0x78, 0x00, 0x01, 0xe0, 0x00, 0x07, 0xc0, 0x07, 0x9e, 0x00, 
  0x1e, 0x00, 0x00, 0x03, 0xcf, 0x00, 0x78, 0x00, 0x01, 0xe0, 0x00, 0x07, 0xc0, 0x07, 0x9e, 0x00, 
  0x1e, 0x00, 0x00, 0x03, 0xcf, 0x00, 0x78, 0x00, 0x01, 0xe0, 0x00, 0x07, 0xc0, 0x07, 0x9e, 0x00, 
  0x1e, 0x03, 0xc0, 0x07, 0xcf, 0x80, 0x78, 0x0f, 0x01, 0xe0, 0x3c, 0x07, 0xc0, 0x0f, 0x9f, 0x00, 
  0x1e, 0x03, 0xc0, 0x07, 0x87, 0x80, 0x78, 0x0f, 0x01, 0xe0, 0x3c, 0x07, 0xc0, 0x0f, 0x0f, 0x00, 
  0x1e, 0x03, 0xc0, 0x07, 0x87, 0x80, 0x78, 0x0f, 0x01, 0xe0, 0x3c, 0x07, 0xc0, 0x0f, 0x0f, 0x00, 
  0x1e, 0x03, 0xc0, 0x0f, 0x87, 0xc0, 0x78, 0x0f, 0x01, 0xe0, 0x3c, 0x07, 0xc0, 0x1f, 0x0f, 0x80, 
  0x1f, 0x03, 0xc0, 0x0f, 0xff, 0xc0, 0x7c, 0x0f, 0x01, 0xf0, 0x3c, 0x07, 0xc0, 0x1f, 0xff, 0x80, 
  0x1f, 0x03, 0xc0, 0x0f, 0xff, 0xc0, 0x7c, 0x0f, 0x01, 0xf0, 0x3c, 0x07, 0xc0, 0x1f, 0xff, 0x80, 
  0x0f, 0x83, 0xc0, 0x0f, 0xff, 0xc0, 0x3e, 0x0f, 0x00, 0xf8, 0x3c, 0x07, 0xc0, 0x1f, 0xff, 0x80, 
  0x0f, 0x83, 0xc0, 0x1f, 0xff, 0xe0, 0x3e, 0x0f, 0x00, 0xf8, 0x3c, 0x07, 0xc0, 0x3f, 0xff, 0xc0, 
  0x07, 0xc3, 0xc0, 0x1f, 0xff, 0xe0, 0x1f, 0x0f, 0x00, 0x7c, 0x3c, 0x07, 0xc0, 0x3f, 0xff, 0xc0, 
  0x07, 0xe3, 0xc0, 0x1e, 0x01, 0xe0, 0x1f, 0x8f, 0x00, 0x7e, 0x3c, 0x07, 0xc0, 0x3c, 0x03, 0xc0, 
  0x03, 0xf3, 0xc0, 0x3e, 0x01, 0xf0, 0x0f, 0xcf, 0x00, 0x3f, 0x3c, 0x07, 0xc0, 0x7c, 0x03, 0xe0, 
  0x01, 0xff, 0xc0, 0x3e, 0x01, 0xf0, 0x07, 0xff, 0x00, 0x1f, 0xfc, 0x07, 0xc0, 0x7c, 0x03, 0xe0, 
  0x00, 0xff, 0xc0, 0x3c, 0x00, 0xf0, 0x03, 0xff, 0x00, 0x0f, 0xfc, 0x07, 0xc0, 0x78, 0x01, 0xe0, 
  0x00, 0x7f, 0xc0, 0x3c, 0x00, 0xf0, 0x01, 0xff, 0x00, 0x07, 0xfc, 0x07, 0xc0, 0x78, 0x01, 0xe0, 
  0x00, 0x1f, 0xc0, 0x7c, 0x00, 0xf8, 0x00, 0x7f, 0x00, 0x01, 0xfc, 0x07, 0xc0, 0xf8, 0x01, 0xf0, 
  0x00, 0x07, 0xc0, 0x78, 0x00, 0x78, 0x00, 0x1f, 0x00, 0x00, 0x7c, 0x07, 0xc0, 0xf0, 0x00, 0xf0, 
  0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const unsigned char PROGMEM antenna_OK[] = {
  B11111000,
  B10101000,
  B10101000,
  B01110000,
  B00100000,
  B00100000,
  B00100000,
  B00100000
};
static const unsigned char PROGMEM antenna_NOK[] = {
  B10111100,
  B01000100,
  B00100100,
  B00010000,
  B00001000,
  B00010100,
  B00010010,
  B00010001
};
static const unsigned char PROGMEM blynk_OK[] = {
  B00000000, B00000000,
  B10011110, B00100000,
  B01010001, B01000000,
  B00010001, B00000000,
  B11011110, B01100000,
  B00010001, B00000000,
  B01010001, B01000000,
  B10011110, B00100000
};
static const unsigned char PROGMEM blynk_NOK[] = {
  B10000000,
  B01011100,
  B00100010,
  B00010010,
  B00101000,
  B00100100,
  B00100010,
  B00111101
};
