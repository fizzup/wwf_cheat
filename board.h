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
