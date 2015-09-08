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

#include "undo.h"

Undo::Undo( void )
    : mUndoStack(),
      mRedoStack()
{
}

void Undo::save( const Board& board )
{
    if ( !mRedoStack.empty() )
    {
        mRedoStack.erase( mRedoStack.begin(), mRedoStack.end() );
    }

    mUndoStack.push_back( board );
}

Board Undo::undo( const Board& currentBoard )
{
    if ( mUndoStack.size() == 1 )
    {
        return currentBoard;
    }
    else
    {
        Board board = mUndoStack.back();

        mUndoStack.pop_back();
        mRedoStack.push_back( currentBoard );

        return board;
    }
}

Board Undo::redo( const Board& currentBoard )
{
    if ( mRedoStack.empty() )
    {
        return currentBoard;
    }
    else
    {
        Board board = mRedoStack.back();

        mRedoStack.pop_back();
        mUndoStack.push_back( currentBoard );

        return board;
    }
}
