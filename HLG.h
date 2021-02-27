#ifndef HLG_ARDUINO_H
#define HLG_ARDUINO_H

#include "Arduino.h"

#define HLG_ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

class HLG
{
public:
	//
	// Types
	//
	#define HLG_DEF static const

	typedef uint16_t coord_t;  // Only the 11 LSBs are used, the rest ignored
	typedef uint8_t  opcode_t; // Opcode type TODO(Peacock): Start w/ 0xA
	typedef uint8_t  pin_t;    // An Arduino digital i/o pin

	enum RESOLUTION_MODE
	{
		RESOLUTION_MODE_1920x1080 = 0,
		RESOLUTION_MODE_720x480   = 1,
		RESOLUTION_MODE_640x480   = 2,
	};
	typedef uint8_t ResolutionMode;

	typedef struct _v2_t
	{
		int x;
		int y;
	} v2;

	typedef union _color_t
	{
		struct
		{
			uint8_t r;
			uint8_t g;
			uint8_t b;
		};

		struct
		{
			uint8_t red;
			uint8_t green;
			uint8_t blue;
		};
	} color_t;

	typedef union _v3
	{
		struct
		{
			int x;
			int y;
			int z;
		};
	} v3;

	//
	// Opcodes
	//

	// TODO(Peacock): Start all of these with 0xA or something for some error checking
	// Since there are always the first eight bits of a command sequence and we can't be
	// 100% of how intact the SPI connection is
    HLG_DEF opcode_t HLG_OPCODE_NOP          = 0;
    HLG_DEF opcode_t HLG_OPCODE_SET_COLOR    = 1;
    HLG_DEF opcode_t HLG_OPCODE_CLEAR_SCREEN = 2;
    HLG_DEF opcode_t HLG_OPCODE_DRAW_RECT    = 3;
    HLG_DEF opcode_t HLG_OPCODE_DRAW_LINE    = 4;
    HLG_DEF opcode_t HLG_OPCODE_DRAW_CIRCLE  = 5;
    HLG_DEF opcode_t HLG_OPCODE_DRAW_TEXT    = 6;

    //
    // Colors
    //
    HLG_DEF color_t COLOR_BLACK = { 0x00, 0x00, 0x00 };
    HLG_DEF color_t COLOR_WHITE = { 0xff, 0xff, 0xff };
    HLG_DEF color_t COLOR_RED   = { 0xff, 0x00, 0x00 };
    HLG_DEF color_t COLOR_BLUE  = { 0x00, 0x00, 0xff };

    //
    // Pins
    //
    HLG_DEF pin_t SS_PIN = 10;

    //
    //
    //
    void init(ResolutionMode mode);

    void setColor(uint8_t r, uint8_t g, uint8_t b);
    inline void setColor(color_t color) { this->setColor(color.r, color.g, color.b); }

    void clearScreen();

    void drawPixel(coord_t x, coord_t y);
    inline void drawPixel(v2 coords) { this->drawPixel((coord_t)coords.x, (coord_t)coords.y); }

    void drawRect(coord_t topLeftX, coord_t topLeftY, coord_t botRightX, coord_t botRightY);
    inline void drawRect(v2 topLeft, v2 botRight) { this->drawRect(topLeft.x, topLeft.y, botRight.x, botRight.y); }

    void drawLine(coord_t ax, coord_t ay, coord_t bx, coord_t by);
    inline void drawLine(v2 a, v2 b) { this->drawLine(a.x, a.y, b.x, b.y); }

    void drawCircle(coord_t centerX, coord_t centerY, coord_t radius);
    inline void drawCircle(v2 center, int radius) { this->drawCircle(center.x, center.y, radius); }

    void drawText(coord_t x, coord_t y, const char *text);
    inline void drawText(v2 lhs, const char *text) { this->drawText(lhs.x, lhs.y, text); }
};

#undef HLG_DEF
#endif // ~HLG_ARDUINO_H
