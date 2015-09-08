#ifndef __BOARD_H__
#define __BOARD_H__

#include <string>
#include <list>
#include <iostream>
#include <vector>
#include "board.h"
#include "play.h"
#include "anationary.h"
#include "letters.h"
#include "dictionary.h"
#include "square.h"
#include "tile.h"

class Letters;

class Board
{
public:
    Board( const std::string& boardFileName, const std::string& game = "wwf" );

    Board( const Board& board );
    
    ~Board() {};

    const Board& operator=( const Board& rhs );

    std::list<Play> findAllPlays( const Tiles& rack ) const;

    void makePlay( int row, int col, bool horiz, const Tiles& word );

    bool isEmpty( void ) const;

    const Square& square( int row, int col ) const
        { return mBoardLayout[row][col]; };

    bool partOfLastPlay( int row, int col ) const;

    void save( void );

    static const Letters& letters( void );

private:
    void initBoardFromFile( const std::string& file );

    int score( int row, int col, bool horiz, const Tiles& word,
        const Tiles& rack ) const;

    bool isValid( int row, int col, bool horiz, const Tiles& word,
        const Tiles& rack ) const;

    Tiles getVerticalWord( int row, int col, const Tile& tile ) const;
    Tiles getHorizontalWord( int row, int col, const Tile& tile ) const;

    void setLastPlay( int row, int col, bool horiz, int len );

    static const int SIZE = 15;

    Square mBoardLayout[SIZE][SIZE];

    int mLowestRow;
    int mHighestRow;

    int mLowestCol;
    int mHighestCol;

    struct
    {
    public:
        int mRow;
        int mCol;
        bool mHorizontal;
        int mLength;
    } mLastPlay;

    int mBingoScore;
    std::string mBoardFileName;

    static CompiledDictionary mDictionary;
    static Anationary mAnationary;
    static Letters mLetters;

friend std::ostream& operator<<( std::ostream& os, const Board& board );
};

#endif
