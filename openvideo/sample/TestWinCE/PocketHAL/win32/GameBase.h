//////////////////////////////////////////////////////////////////////////////
//
// PocketHAL
// Copyright 2004 by Thierry Tremblay
//
//////////////////////////////////////////////////////////////////////////////

#ifndef POCKETHAL_GAMEBASE_H
#define POCKETHAL_GAMEBASE_H

#include "../IGame.h"



namespace PHAL
{

//////////////////////////////////////////////////////////////////////////////
//
// GameBase
//
//////////////////////////////////////////////////////////////////////////////

class GameBase : public IGame
{
public:

    GameBase();
    ~GameBase();


    // Set the app's UID for mutual exclusion
    void SetUID( unsigned uid )     { m_uid = uid; }


protected:
    
    // Initialization
    bool Initialize( const Config& config );

    // Return the window handle
    HWND GetWindow() const          { return m_hwnd; }

    // Run
    bool Run();

    // Reset inactivity (prevent the screen from dimming)
    void ResetInactivity() const;


private:

    // Message handler
    LRESULT MessageHandler( UINT msg, WPARAM wparam, LPARAM lparam );

    // Data
    HWND     m_hwnd;        // Window handle
    bool     m_bSuspended;  // Are we suspended?
    unsigned m_uid;         // Unique ID

    // Static interface
    static GameBase* s_instance;
    static LRESULT CALLBACK MessageHandler( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );
};



} // end of namespace PHAL



//////////////////////////////////////////////////////////////////////////////
//
// Program entry point declaration
//
//////////////////////////////////////////////////////////////////////////////

#define PHAL_DECLARE_ENTRY_POINT( GameClass )                           \
                                                                        \
    int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPTSTR, int )   \
    {                                                                   \
        GameClass game;                                                 \
        game.SetUID( GameClass::UID );                                  \
        game.Run();                                                     \
        return EXIT_SUCCESS;                                            \
    }



#endif
