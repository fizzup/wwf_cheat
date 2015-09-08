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
