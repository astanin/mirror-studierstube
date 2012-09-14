//////////////////////////////////////////////////////////////////////////////
//
// PocketHAL
// Copyright 2004 by Thierry Tremblay
//
//////////////////////////////////////////////////////////////////////////////

#include "utility.h"


namespace PHAL
{

//////////////////////////////////////////////////////////////////////////////
//
// Static data
//
//////////////////////////////////////////////////////////////////////////////

static const uint8_t VGAFONT[] =
{
    #include "VGAFont.h"
};

static const int CHAR_WIDTH  = 8;       // 8 pixels wide
static const int CHAR_HEIGHT = 16;      // 16 pixels high
static const int CHAR_SIZE   = 16;      // 16 bytes / character



//////////////////////////////////////////////////////////////////////////////
//
// Utility functions
//
//////////////////////////////////////////////////////////////////////////////

void Clear( Surface* surface, uint16_t color )
{
    // We operate on two pixels at a time. The count
    // thus represents the number of pixel pairs.
    unsigned  count    = surface->GetHeight() * surface->GetPitch() >> 2;
    uint32_t* pixels   = (uint32_t*)surface->GetPixels();
    uint32_t  color32  = (color << 16) | (color);

    while (count--)
        *pixels++ = color32;
}



bool DrawChar( Surface* surface, int x, int y, char character, uint16_t color )
{
    // Check surface dimensions
    if (x < 0 || y < 0 || x + CHAR_WIDTH > (int)surface->GetWidth() || y + CHAR_HEIGHT > (int)surface->GetHeight())
        return false;

    // Draw the char
    const uint8_t* glyph = &VGAFONT[character * CHAR_SIZE];

    for (int row = 0; row != CHAR_HEIGHT; ++row, ++glyph)
    {
        uint16_t* pixels = surface->GetPixels( x, y + row );

        if (*glyph & 0x80) *pixels = color; ++pixels;
        if (*glyph & 0x40) *pixels = color; ++pixels;
        if (*glyph & 0x20) *pixels = color; ++pixels;
        if (*glyph & 0x10) *pixels = color; ++pixels;
        if (*glyph & 0x08) *pixels = color; ++pixels;
        if (*glyph & 0x04) *pixels = color; ++pixels;
        if (*glyph & 0x02) *pixels = color; ++pixels;
        if (*glyph & 0x01) *pixels = color; ++pixels;
    }

    return true;
}



void DrawText( Surface* surface, int x, int y, const char* text, uint16_t color )
{
    // Draw each character...
    for ( ; *text; ++text, x += CHAR_WIDTH)
    {
        if (!DrawChar( surface, x, y, *text, color ))
            return;
    }
}




} // end of namespace PHAL
