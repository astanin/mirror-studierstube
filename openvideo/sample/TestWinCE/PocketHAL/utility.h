//////////////////////////////////////////////////////////////////////////////
//
// PocketHAL
// Copyright 2004 by Thierry Tremblay
//
//////////////////////////////////////////////////////////////////////////////

#ifndef POCKETHAL_UTIL_H
#define POCKETHAL_UTIL_H

#include <PocketHAL/surface.h>

#undef DrawText     // Prevent conflict with win32's DrawText()



namespace PHAL
{

//////////////////////////////////////////////////////////////////////////////
//
// Color constants
//
//////////////////////////////////////////////////////////////////////////////

const uint16_t COLOR_BLACK      = 0x0000;
const uint16_t COLOR_DARKGRAY   = 0x4208;
const uint16_t COLOR_GRAY       = 0x8410;
const uint16_t COLOR_LIGHTGRAY  = 0xC618;
const uint16_t COLOR_WHITE      = 0xFFFF;

const uint16_t COLOR_RED        = 0x1F << 11;
const uint16_t COLOR_GREEN      = 0x3F << 5;
const uint16_t COLOR_BLUE       = 0x1F;

const uint16_t COLOR_YELLOW     = COLOR_RED | COLOR_GREEN;
const uint16_t COLOR_PINK       = COLOR_RED | COLOR_BLUE;
const uint16_t COLOR_CYAN       = COLOR_GREEN | COLOR_BLUE;



//////////////////////////////////////////////////////////////////////////////
//
// Utility functions
//
//////////////////////////////////////////////////////////////////////////////

// Clear the surface with the specified color
void Clear( Surface* surface, uint16_t color );

// Draw a character at the specified location
// Return true on success or false if the character would be drawn outside the surface dimensions.
bool DrawChar( Surface* surface, int x, int y, char character, uint16_t color = COLOR_WHITE );

// Draw some text at the specified location
void DrawText( Surface* surface, int x, int y, const char* text, uint16_t color = COLOR_WHITE );



} // end of namespace PHAL



#endif
