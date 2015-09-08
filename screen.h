// Copyright (c) 2015 Michael Hamilton
//
// This file is part of wwf_cheat.

// wwf_cheat is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.

// wwf_cheat is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with wwf_cheat.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <ncurses.h>
#include <string>
#include "window.h"


class Screen
{
public:
    Screen( bool raw = true, bool echo = false, bool keypad = true );

    ~Screen( void );

    // Sets/gets whether the terminal is in raw input mode or cbreak mode.
    void raw( bool raw );
    bool raw( void ) { return mRaw; };

    // Sets/gets whether the terminal echos characters typed by the user.
    void echo( bool echo );
    bool echo( void ) { return mEcho; };

    // Sets/gets whether keypad terminal characters are interpreted (F1,F2,...)
    void keypad( bool keypad );
    bool keypad( void ) { return mKeypad; };

    // Opening, closing, and getting a window to print some stuff on.
    // The window takes a column/row to draw the border, and two more rows
    // at the top for the title. Defaults to the whole screen.
    Window& openWindow( const std::string& name, const std::string& title, 
                     int row = 0, int col = 0, int h = -1, int w = -1 );
    Window& window( const std::string& name );
    void closeWindow( const std::string& name );

    void cursorLocation( int row, int col );

    void refresh( void ) const;

    int height( void ) const { return LINES; };

    int width( void ) const { return COLS; };

protected:

private:
    bool mRaw;
    bool mEcho;
    bool mKeypad;
    bool mColor;

    int mCursorRow;
    int mCursorCol;

    std::list<Window> mWindows;
};

#endif
