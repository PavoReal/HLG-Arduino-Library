#include "HLG.h"

#include "Arduino.h"
#include <SPI.h>

inline void
WriteBufferToSPI(uint32_t *_buffer, uint16_t bufferSize)
{
	SPI.beginTransaction(SPISettings(115200, MSBFIRST, SPI_MODE2));

#if defined(DEBUG)
	Serial.print("Sending ");
	Serial.print(bufferSize);
	Serial.print(" u32s");
	Serial.println("");

	for (uint16_t i = 0; i < bufferSize; ++i)
	{
		Serial.print(_buffer[i], HEX);
		Serial.print(", ");
	}
#endif

	uint8_t *buffer = (uint8_t*) _buffer;
	uint16_t stop   = bufferSize * 4;

	digitalWrite(HLG::SS_PIN, 0);
	for (uint16_t i = 0; i < stop; ++i)
	{
		SPI.transfer(buffer[i]);
	}
	digitalWrite(HLG::SS_PIN, 1);

#if defined(DEBUG)
	Serial.println("");
#endif

	SPI.endTransaction();
}

void
HLG::init(ResolutionMode mode)
{
	// TODO: Send set resolution command
	
	// Set SPI pins direction
	pinMode(13, OUTPUT); // CLK
	pinMode(12, INPUT);  // MISO
	pinMode(11, OUTPUT); // MOSI

	pinMode(HLG::SS_PIN, OUTPUT); // SS

	//
	// Per HLG req, send 3 nops during during setup
	//
	uint32_t nops[3];

	// Clear buffer to zero
	memset(nops, 0, HLG_ARRAY_SIZE(nops) * sizeof(uint32_t));

	WriteBufferToSPI(nops, HLG_ARRAY_SIZE(nops));
}

void 
HLG::setColor(uint8_t r, uint8_t g, uint8_t b)
{
	//
	// 8 bits opcode MSB
	// 8 bits red
	// 8 bits green
	// 8 bits blue LSB
	//
	uint32_t dataToSend[1];	
	memset(dataToSend, 0, HLG_ARRAY_SIZE(dataToSend) * sizeof(uint32_t));

	dataToSend[0] = ((HLG_OPCODE_SET_COLOR << 24) | (r << 16) | (g << 8) | (b));

	WriteBufferToSPI(dataToSend, HLG_ARRAY_SIZE(dataToSend));
}

void
HLG::clearScreen()
{
	// 8 bits opcode
	uint16_t dataToSend[1];
	dataToSend[0] = (HLG_OPCODE_CLEAR_SCREEN << 8);

	WriteBufferToSPI(dataToSend, HLG_ARRAY_SIZE(dataToSend));
}

void 
HLG::drawPixel(coord_t x, coord_t y)
{
	//
	// 8 bits opcode
	// 11 bits x (MSB first) -- Only the 11 LSBs are used
	// 11 bits y (MSG first) -- Only the 11 LSBs are used
	//
	uint32_t dataToSend[1];
	memset(dataToSend, 0, HLG_ARRAY_SIZE(dataToSend) * sizeof(uint32_t));

	dataToSend[0] = ((HLG_OPCODE_SET_COLOR << 24) |
			      (x << 13) | 
			      (y << 2));

	WriteBufferToSPI(dataToSend, HLG_ARRAY_SIZE(dataToSend));
}


void
HLG::drawRect(coord_t topLeftX, coord_t topLeftY, coord_t botRightX, coord_t botRightY)
{
	//
	// SPI Command seq MSB first:
	//
	// 8 bits opcode
	// 11 bits topLeftX
	// 11 bits topLeftY
	// 2 bits unused
	// -- end first word
	// 11 bits botRightX
	// 11 bits botRightY
	// 10 bits unused
	// -- end second word
	//
	uint32_t dataToSend[2];
	memset(dataToSend, 0, HLG_ARRAY_SIZE(dataToSend) * sizeof(uint32_t));

	dataToSend[0] = ((HLG_OPCODE_DRAW_RECT << 24) | (topLeftX << 13) | (topLeftY << 2));
	dataToSend[1] = ((botRightX << 21) | (botRightY << 10));

	WriteBufferToSPI(dataToSend, HLG_ARRAY_SIZE(dataToSend));
}


void 
HLG::drawLine(coord_t ax, coord_t ay, coord_t bx, coord_t by)
{
	//
	// SPI Command seq MSB first:
	//
	// 8 bits opcode
	// 11 bits ax
	// 11 bits ay
	// 2 bits unused
	// -- end first word
	// 11 bits bx
	// 11 bits by
	// 10 bits unused
	// -- end second word
	//
	uint32_t dataToSend[2];
	memset(dataToSend, 0, HLG_ARRAY_SIZE(dataToSend) * sizeof(uint32_t));

	dataToSend[0] = ((HLG_OPCODE_DRAW_LINE << 24) | (ax << 13) | (ay << 2));
	dataToSend[1] = ((bx << 21) | (by << 10));

	WriteBufferToSPI(dataToSend, HLG_ARRAY_SIZE(dataToSend));
}

void 
HLG::drawCircle(coord_t centerX, coord_t centerY, coord_t radius)
{
	//
	// SPI Command seq MSB first:
	//
	// 8 bits opcode
	// 11 bits centerX
	// 11 bits centerY
	// 2 bits unused
	// -- end first word
	// 11 bits radius
	// 21 bits unused
	// -- end second word
	//
	uint32_t dataToSend[2];
	memset(dataToSend, 0, HLG_ARRAY_SIZE(dataToSend) * sizeof(uint32_t));

	dataToSend[0] = ((opcode << 24) | (centerX << 13) | (centerY << 2));
	dataToSend[1] = (radius << 21);

	WriteBufferToSPI(dataToSend, HLG_ARRAY_SIZE(dataToSend));
}

void 
HLG::drawText(coord_t x, coord_t y, const char *text)
{
	//
	// SPI Command seq MSB first:
	//
	// 8 bits opcode
	// 11 bits x
	// 11 bits y
	// 2 bits unused
	// -- end first word
	// 4 chars at eight bits each per word
	// for the whole text string, terminated by a all 0s word (null terminator)
	//
	uint32_t dataToSend[1];
	memset(dataToSend, 0, HLG_ARRAY_SIZE(dataToSend) * sizeof(uint32_t));

	dataToSend[0] = ((opcode << 24) | (x << 13) | (y << 2));

	WriteBufferToSPI(dataToSend, HLG_ARRAY_SIZE(dataToSend));
	WriteBufferToSPI((uint32_t*) text, strlen(text) / 4);
}
