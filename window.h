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

#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <string>
#include <list>
#include "character.h"

class Screen;

class Window
{
public:
    ~Window( void ) {};

    void clear( void ) { mContent = std::list<Character>(); drawFrame(); };
    void addChar( const Character& ch ) {
        mContent.push_back( ch );
        mCurRow = ch.row();
        mCurCol = ch.col();
        mCurAttr = ch.attr();
    };

    void setCursor( int row, int col, const Attributes& attr = Attributes() );
    void typeChar( char ch, bool protectBorder = true );
    void typeLine( const std::string& s, bool protectBorder = true );

    void chAttr( int row, int col, int h, int w, const Attributes& attr );

    void paint( void ) const;

    const std::string& name( void ) const { return mName; };

    void setFocus( Screen& s ) const;
protected:
    Window( const std::string& name, const std::string& title,
            int row, int col, int h, int w);
private:
    std::list<Character> mContent;
    int mRow;
    int mCol;
    int mHeight;
    int mWidth;

    std::string mTitle;
    std::string mName;

    int mCurRow;
    int mCurCol;
    Attributes mCurAttr;

    static const char mCorner = ' ';
    static const char mHorizontal = ' ';
    static const char mVertical = ' ';

    void drawFrame( void );

friend class Screen;
};

#endif
