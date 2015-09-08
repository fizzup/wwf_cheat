#include "play.h"
#include "board.h"

void Play::play( Board& board ) const
{
    board.makePlay( mRow, mCol, mHorizontal, mTiles );
}
