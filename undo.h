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

#ifndef __UNDO_H__
#define __UNDO_H__

#include <list>
#include "board.h"

class Undo
{
public:
    Undo( void );
    Undo( const Undo& undo )
        : mUndoStack( undo.mUndoStack ),
          mRedoStack( undo.mRedoStack )
    { }

    const Undo& operator =( const Undo& rhs )
    {
        mUndoStack = rhs.mUndoStack;
        mRedoStack = rhs.mRedoStack;

        return *this;
    }

    void save( const Board& board );
    Board undo( const Board& currentBoard );
    Board redo( const Board& currentBoard );
protected:
    std::list<Board> mUndoStack;
    std::list<Board> mRedoStack;
private:
};

#endif
