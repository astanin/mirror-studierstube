//////////////////////////////////////////////////////////////////////////////
//
// PocketHAL
// Copyright 2004 by Thierry Tremblay
//
//////////////////////////////////////////////////////////////////////////////

#include "game.h"



namespace PHAL
{

//////////////////////////////////////////////////////////////////////////////
//
// Game
//
//////////////////////////////////////////////////////////////////////////////

Game::Game()
:   m_display( 0 )
{
}



Game::~Game()
{
    delete m_display;
}



bool Game::Initialize()
{
    m_bShutdown = false;

    if (!GameBase::Initialize( m_config ))
        return false;

    DisplayParameters params( m_config.m_orientation );
    
    m_display = Display::Create( GameBase::GetWindow(), params );
    if (!m_display)
        return false;

    m_lastInactivityReset = GetClock();

    return true;
}



bool Game::Run()
{
    if (!Initialize())
        return false;

    if (!OnInitialize())
        return false;

    return GameBase::Run();
}



void Game::Shutdown()
{
    m_bShutdown = true;
}



void Game::BuildFPSString( int fps )
{
    if (fps > 99999)
        fps = 99999;

    int i0 = fps / 10000; fps -= i0 * 10000;
    int i1 = fps / 1000; fps -= i1 * 1000;
    int i2 = fps / 100; fps -= i2 * 100;
    int i3 = fps / 10; fps -= i3 * 10;
    int i4 = fps;

    m_fpsString[0] = char('0' + i0);
    m_fpsString[1] = char('0' + i1);
    m_fpsString[2] = char('0' + i2);
    m_fpsString[3] = char('.');
    m_fpsString[4] = char('0' + i3);
    m_fpsString[5] = char('0' + i4);
    m_fpsString[6] = char(0);
}



//////////////////////////////////////////////////////////////////////////////
// IGame callbacks
//////////////////////////////////////////////////////////////////////////////
   
bool Game::OnInitialize()
{
    m_lastFPSTime = GetClock();
    m_nbFrames    = 0;
    m_fps         = 0;

    BuildFPSString( 0 );

    return true;
}


void Game::OnShutdown()
{
    delete m_display;
    m_display = 0;
}


bool Game::OnGameLoop()
{
    ++m_nbFrames;

    // Check if it is time to calculate the FPS
    uint32_t now   = GetClock();
    uint32_t delta = now - m_lastFPSTime;
    
    if (delta >= 500)
    {
        // Calculate FPS
        m_fps = (m_nbFrames*100000) / delta;

        BuildFPSString( m_fps );

        // Reset vars
        m_lastFPSTime = now;
        m_nbFrames    = 0;
    }


    // Reset inactivity once in a while
    if (now - m_lastInactivityReset >= 5000)
    {
        m_lastInactivityReset = now;
        ResetInactivity();
    }


    return !m_bShutdown;
}


void Game::OnSuspend()
{
    #if defined(PHAL_PLATFORM_SYMBIAN)
    m_display->HandleForegroundEvent( false );
    #endif
}


void Game::OnResume()
{
    #if defined(PHAL_PLATFORM_SYMBIAN)
    m_display->HandleForegroundEvent( true );
    #endif

    m_lastInactivityReset = GetClock();
}


void Game::OnKeyDown( int key )
{
    Shutdown();
}


void Game::OnKeyUp( int key )
{
}


void Game::OnStylusDown( Point p )
{
}


void Game::OnStylusMove( Point p )
{
}


void Game::OnStylusUp( Point p )
{
}




} // end of namespace PHAL
