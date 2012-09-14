//////////////////////////////////////////////////////////////////////////////
//
// PocketHAL
// Copyright 2004 by Thierry Tremblay
//
//////////////////////////////////////////////////////////////////////////////

#ifndef POCKETHAL_GAME_H
#define POCKETHAL_GAME_H

#include <PocketHAL/PocketHAL.h>
#include "Win32/GameBase.h"



namespace PHAL
{

class Display;


//////////////////////////////////////////////////////////////////////////////
//
// Game
//
//////////////////////////////////////////////////////////////////////////////

class Game : public GameBase
{
public:

    Game();
    ~Game();

    // Start running the game, returns false if initialization failed.
    bool Run();


protected:

    // Get access to the display
    Display* GetDisplay()   { return m_display; }

    // Shutdown the game
    void Shutdown();


    // Game callbacks
    virtual bool OnInitialize();            // Return true or false depending on success
    virtual void OnShutdown();              // Game is shutting down, game loop won't be called again
    virtual bool OnGameLoop();              // Game loop, return true to continue running or false to shutdown
    virtual void OnSuspend();               // Game is suspended (ex: in the background)
    virtual void OnResume();                // Game can resume

    // Keys/button press/release events
    virtual void OnKeyDown( int key );      // A key was pressed
    virtual void OnKeyUp( int key );        // A key was released

    // Stylus events
    virtual void OnStylusDown( Point p );   // The stylus/pointer is down
    virtual void OnStylusMove( Point p );   // The stylus/pointer is moving
    virtual void OnStylusUp( Point p );     // The stylus/pointer is up


protected:
    
    // Data
    Config      m_config;       // Configuration

    // Retrieve the FPS string
    const char* GetFPSString() const    { return m_fpsString; }


private:

    // Initialize the framework
    bool Initialize();

    // Build the FPS string
    void BuildFPSString( int fps );

    // Data
    bool        m_bShutdown;            // Shutdown?
    Display*    m_display;              // Display
    uint32_t    m_lastInactivityReset;  // Last time we resetted inactivity

    // FPS related variables
    uint32_t    m_lastFPSTime;          // Clock time used to calculate fps
    uint32_t    m_nbFrames;             // Number of frames rendered
    int         m_fps;                  // Frame per second (*100)
    char        m_fpsString[7];         // FPS string
};



} // end of namespace PHAL



#endif
