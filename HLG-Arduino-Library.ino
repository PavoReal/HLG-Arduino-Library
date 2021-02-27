// Include the HLG reference library
#include "HLG.h"

// Declare the HLG object
HLG hlg;

// Declare the (x, y) for the blue rect
// in pixel space
HLG::coord_t x;
HLG::coord_t y;

//
// Runs once at startup
//
void 
setup() 
{
	// Setup the HLG with 1920x1080 resolution
	hlg.init(HLG::RESOLUTION_MODE_1920x1080);

	// Set our (x, y) to (0, 0) -- the top left of the screen
	x = 0;
	y = 0;
}

//
// Runs in a loop until power is lost or the reset button is pressed
//
void 
loop() 
{
    // Set the drawing color to pure red
    hlg.setColor(HLG::COLOR_RED);


    // Clear the screen
    hlg.clearScreen();

    // If our the right side of our rect to past the screen edge
    // reset the rect to (0, 0)
    if ((x + 100) >= 1920)
    {
    	x = 0;
    	y = 0;
    }
    // The the bottom of the rect is past the screen edge
    // reset the rect to (0, 0)
    else if ((y + 150) >= 1080)
    {
    	y = 0;
    	x = 0;
    }

    // Set the drawing color to pure blue
    hlg.setColor(HLG::COLOR_BLUE);
    // Draw our rectangle
    hlg.drawRect(x, y, x + 100, y + 150);

    // Move the rectangle a bit to the right
    x += 10;

    // Move the rect a bit down
    y += 10;

    // Delay for 100 ms
    delay(1);
}
