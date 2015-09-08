#include <ncurses.h>
#include "screen.h"

Screen::Screen( bool r, bool e, bool k )
    : mRaw( r ),
      mEcho( e ),
      mKeypad( k ),
      mCursorRow( 0 ),
      mCursorCol( 0 )
{
    initscr();

    if ( mRaw ) raw(); else cbreak();

    if ( mEcho ) echo(); else noecho();

    ::keypad( stdscr, mKeypad );

    if ( has_colors() == TRUE )
    {
        start_color();
    }
}

Screen::~Screen( void )
{
    endwin();
}

void Screen::raw( bool r )
{
    if ( mRaw && !r )
    {
        cbreak();
    }
    if ( !mRaw && r )
    {
        ::raw();
    }

    mRaw = r;
}

void Screen::echo( bool e )
{
    if ( mEcho && !e )
    {
        noecho();
    }
    if ( !mEcho && e )
    {
        ::echo();
    }

    mEcho = e;
}

void Screen::keypad( bool k )
{
    ::keypad( stdscr, k );
    
    mKeypad = k;
}

Window& Screen::openWindow( const std::string& name, const std::string& title, 
                 int row, int col, int h, int w )
{
    if ( h == -1 )
    {
        h = LINES;
    }
    if ( w == -1 )
    {
        w = COLS;
    }

    Window win( name, title, row, col, h, w );
    mWindows.push_back( win );

    return mWindows.back();
}

Window& Screen::window( const std::string& name )
{
    std::list<Window>::iterator i = mWindows.begin();

    while ( i != mWindows.end() )
    {
        if ( i->name() == name )
        {
            return *i;
        }
        ++i;
    }

    throw ( "Window with that name does not exist" );
}

void Screen::closeWindow( const std::string& name )
{
    std::list<Window>::iterator i = mWindows.begin();

    while ( i != mWindows.end() )
    {
        if ( i->name() == name )
        {
            mWindows.erase( i );
            return;
        }
        ++i;
    }
}

void Screen::cursorLocation( int row, int col )
{
    mCursorRow = row;
    mCursorCol = col;
}

void Screen::refresh( void ) const
{
    erase();

    std::list<Window>::const_iterator i = mWindows.begin();

    while ( i != mWindows.end() )
    {
        i->paint();
        ++i;
    }

    move( mCursorRow, mCursorCol );

    ::refresh();
}
