//////////////////////////////////////////////////////////////////////////////
//
// PocketHAL
// Copyright 2004 by Thierry Tremblay
//
//////////////////////////////////////////////////////////////////////////////

#ifndef POCKETHAL_IGAME_H
#define POCKETHAL_IGAME_H

#include <PocketHAL/defs.h>
#include <PocketHAL/rect.h>



namespace PHAL
{

//////////////////////////////////////////////////////////////////////////////
//
// Configuration
//
//////////////////////////////////////////////////////////////////////////////

class Config
{
public:

    // Constructor: select default values
    Config()
    {
        m_appName     = 0;
        m_orientation = ORIENTATION_NORMAL;
    }

    // Configuration data
    LPCTSTR     m_appName;      // Application name
    Orientation m_orientation;  // Display orientation
};



//////////////////////////////////////////////////////////////////////////////
//
// IGame
//
//////////////////////////////////////////////////////////////////////////////

struct IGame
{
    virtual ~IGame() {}

    // Game callbacks
    virtual bool OnInitialize() = 0;            // Return true or false depending on success
    virtual void OnShutdown() = 0;              // Game is shutting down, game loop won't be called again
    virtual bool OnGameLoop() = 0;              // Game loop, return true to continue running or false to shutdown
    virtual void OnSuspend() = 0;               // Game is suspended (ex: in the background)
    virtual void OnResume() = 0;                // Game can resume

    // Keys/button press/release events
    virtual void OnKeyDown( int key ) = 0;      // A key was pressed
    virtual void OnKeyUp( int key ) = 0;        // A key was released

    // Stylus events
    virtual void OnStylusDown( Point p ) = 0;   // The stylus/pointer is down
    virtual void OnStylusMove( Point p ) = 0;   // The stylus/pointer is moving
    virtual void OnStylusUp( Point p ) = 0;     // The stylus/pointer is up
};


    
} // end of namespace PHAL



#endif
