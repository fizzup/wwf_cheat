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

#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "attributes.h"

class Character
{
public:
    Character( int row, int col, char ch, const Attributes& attr ):
        mRow( row ),
        mCol( col ),
        mChar(ch),
        mAttr(attr)
    {};
    ~Character( void ) {};

    int row( void ) const { return mRow; };

    int col( void ) const { return mCol; };

    char ch( void ) const { return mChar; };

    const Attributes& attr( void ) const { return mAttr; };
    void attr( const Attributes& attr ) { mAttr = attr; };
private:
    int mRow;
    int mCol;
    char mChar;
    Attributes mAttr;
};

#endif
