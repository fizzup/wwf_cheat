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

#include "window.h"
#include "character.h"
#include "attributes.h"
#include "screen.h"

Window::Window( const std::string& name, const std::string& title,
        int row, int col, int h, int w )
    : mRow( row ),
      mCol( col ),
      mHeight( h ),
      mWidth( w ),
      mTitle( title ),
      mName( name )
{
    // Each window must have at least enough room for the borders and title
    if ( mHeight < 3 )
    {
        mHeight = 3;
    }

    if ( mWidth < mTitle.length() + 2 )
    {
        mWidth = mTitle.length() + 2;
    }

    drawFrame();
}

void Window::setCursor( int row, int col, const Attributes& attr )
{
    mCurRow = row;
    mCurCol = col;
    mCurAttr = attr;
}

void Window::typeChar( char ch, bool protectBorder )
{
    int r = mCurRow;
    int c = mCurCol + 1;

    if ( protectBorder && c > mWidth - 2 )
    {
        c = 1;
        r += 1;
    }

    if ( !protectBorder && c > mWidth - 1 )
    {
        c = 0;
        r += 1;
    }

    if ( protectBorder && r > mHeight - 2 )
    {
        r = 2;
    }
    
    if ( !protectBorder && r > mHeight - 1 )
    {
        r = 0;
    }

    addChar( Character( r, c, ch, mCurAttr ) );

    mCurRow = r;
    mCurCol = c;
}

void Window::typeLine( const std::string& s, bool protectBorder )
{
    if ( protectBorder && mCurRow > mHeight - 2 )
    {
        mCurRow = 2;
    }

    if ( protectBorder && mCurRow < 2 )
    {
        mCurRow = 2;
    }

    if ( !protectBorder && mCurRow > mHeight - 1 )
    {
        mCurRow = 0;
    }

    std::string::const_iterator i = s.begin();

    while ( i != s.end() )
    {
        typeChar( *i, protectBorder );
        ++i;
    }

    mCurRow++;
    mCurCol = 0;
    if ( protectBorder && mCurRow > mHeight - 2 )
    {
        mCurRow = 2;
    }

    if ( !protectBorder && mCurRow > mHeight - 1 )
    {
        mCurRow = 0;
    }
}

void Window::chAttr( int row, int col, int h, int w, const Attributes& attr )
{
    std::list<Character>::iterator i = mContent.begin();

    while ( i != mContent.end() )
    {
        if ( i->row() >= row &&
             i->row() < row + h &&
             i->col() >= col &&
             i->col() < col + w )
        {
            i->attr( attr );
        }

        ++i;
    }
}

void Window::paint( void ) const
{
    std::list<Character>::const_iterator i = mContent.begin();

    while ( i != mContent.end() )
    {
        move( mRow + i->row(), mCol + i->col() );
        i->attr().set();
        addch( i->ch() );
        i->attr().clear();

        ++i;
    }
}

void Window::drawFrame( void )
{
    Attributes borderAttr( Bold, Black, White );
    Attributes titleAttr( Bold, White, Cyan );
    int r, c;

    // Title bar
    addChar( Character( 0, 0, ' ', titleAttr ) );
    for( int i = 0; i < mWidth - 2; i++ )
    {
        if ( i < mTitle.length() )
        {
            typeChar( mTitle[i], false );
        }
        else
        {
            typeChar( ' ', false );
        }
    }
    typeChar( ' ', false );

    // Line under title
    addChar( Character( 1, 0, mCorner, borderAttr ) );
    for ( c = 1; c < mWidth - 1; c++ )
    {
        typeChar( mHorizontal, false );
    }
    typeChar( mCorner, false );

    // Body
    for ( r = 2; r < mHeight - 1; r++ )
    {
        addChar( Character( r, 0, mVertical, borderAttr ) );
        for ( c = 1; c < mWidth - 1; c++ )
        {
            typeChar( ' ', false );
        }
        typeChar( mVertical, false );
    }

    // Bottom line
    addChar( Character( mHeight - 1, 0, mCorner, borderAttr ) );
    for ( c = 1; c < mWidth - 1; c++ )
    {
        typeChar( mHorizontal, false );
    }
    typeChar( mCorner, false );
}

void Window::setFocus( Screen& s ) const
{
    s.cursorLocation( mRow + mCurRow, mCol + mCurCol + 1 );
}
