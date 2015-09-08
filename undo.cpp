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
