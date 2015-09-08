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

#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <vector>
#include "anationary.h"
#include "play.h"
#include "letters.h"
#include "board.h"
#include "dictionary.h"
#include "tile.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define PRED(a) (MAX((a)-1,0))
#define SUCC(a) (MIN((a)+1,SIZE-1))

const char* LOWVIDEO = "\033[2m";
const char* BOLD = "\033[1m";
const char* NORMAL = "\033[0m";

const char* BBLK = "\033[40m";
const char* BRED = "\033[41m";
const char* BGRN = "\033[42m";
const char* BYEL = "\033[43m";
const char* BBLU = "\033[44m";
const char* BMAG = "\033[45m";
const char* BCYN = "\033[46m";

const char* FWHT = "\033[37m";

Letters Board::mLetters;
CompiledDictionary Board::mDictionary( "dictionary" );
Anationary Board::mAnationary;

Board::Board( const std::string& boardFileName, const std::string& game )
    : mBoardFileName( boardFileName ),
      mLowestRow( SIZE ),
      mHighestRow( 0 ),
      mLowestCol( SIZE ),
      mHighestCol( 0 )
{
    mLetters = Letters( std::string( "letters." ) + game );
    mAnationary = Anationary( "words" );

    for ( int i = 0; i < SIZE; i++ )
    {
        for ( int j = 0; j < SIZE; j++ )
        {
            mBoardLayout[i][j] = Square();
        }
    }

    initBoardFromFile( std::string( "board." ) + game );

    if ( game == "scrabble" )
    {
        mBingoScore = 50;
    }
    else
    {
        mBingoScore = 35;
    }

    initBoardFromFile( mBoardFileName );

    mLastPlay.mRow = -1;
    mLastPlay.mCol = -1;
    mLastPlay.mLength = 0;
    mLastPlay.mHorizontal = true;
}

void Board::initBoardFromFile( const std::string& file )
{
    std::ifstream bf;
    bf.open( file.c_str() );

    for ( int i = 0; i < SIZE; i++ )
    {
        for( int j = 0; j < SIZE; j++ )
        {
            char ch = bf.get();
            switch ( ch ) 
            {
            case '#':
                mBoardLayout[i][j] = Square( 1, 3 );
                break;
            case '@':
                mBoardLayout[i][j] = Square( 1, 2 );
                break;
            case '3':
                mBoardLayout[i][j] = Square( 3, 1 );
                break;
            case '2':
                mBoardLayout[i][j] = Square( 2, 1 );
                break;
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
                mBoardLayout[i][j].play( mLetters.tile( ch ) );
                mLowestRow = MIN( mLowestRow, PRED( i ) );
                mHighestRow = MAX( mHighestRow, SUCC( i ) );

                mLowestCol = MIN( mLowestCol, PRED( j ) );
                mHighestCol = MAX( mHighestCol, SUCC( j ) );
                break;
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
                mBoardLayout[i][j].play( Tile( ch ^ 0x20, 0, false ) );
                mLowestRow = MIN( mLowestRow, PRED( i ) );
                mHighestRow = MAX( mHighestRow, SUCC( i ) );

                mLowestCol = MIN( mLowestCol, PRED( j ) );
                mHighestCol = MAX( mHighestCol, SUCC( j ) );
                break;
            }
        }

        char newlineGraveyard = bf.get();
    }

    bf.close();

}

Board::Board( const Board& board )
    : mBingoScore( board.mBingoScore ),
      mBoardFileName( board.mBoardFileName ),
      mLastPlay( board.mLastPlay ),
      mHighestRow( board.mHighestRow ),
      mHighestCol( board.mHighestCol ),
      mLowestRow( board.mLowestRow ),
      mLowestCol( board.mLowestCol )
{
    for ( int i = 0; i < SIZE; i++ )
    {
        for ( int j = 0; j < SIZE; j++ )
        {
            mBoardLayout[i][j] = board.mBoardLayout[i][j];
        }
    }
}

const Board& Board::operator=( const Board& rhs )
{
    mBingoScore = rhs.mBingoScore;
    mBoardFileName = rhs.mBoardFileName;
    mLastPlay = rhs.mLastPlay;
    mHighestRow = rhs.mHighestRow;
    mHighestCol = rhs.mHighestCol;
    mLowestRow = rhs.mLowestRow;
    mLowestCol = rhs.mLowestCol;
    for ( int i = 0; i < SIZE; i++ )
    {
        for ( int j = 0; j < SIZE; j++ )
        {
            mBoardLayout[i][j] = rhs.mBoardLayout[i][j];
        }
    }

    return *this;
}
    
std::list<Play> Board::findAllPlays( const Tiles& rack ) const
{
    std::list<Play> plays;

    if ( isEmpty() )
    {
        // An empty board is a special case, because we don't have to
        // check for validity as long as we require the word to be on the
        // middle square. This simplifies the validity testing method.
        std::list<Tiles> anagrams = mAnationary.tanagrams( Tile::letters( rack ) );

		std::list<Tiles>::const_iterator i = anagrams.begin();
		while ( i != anagrams.end() )
		{
			for( int j = SIZE/2 - i->size() + 1; j < SIZE/2 + 1; j++ )
			{
                int sc = score( SIZE/2, j, true, ( *i ), rack );

				plays.push_back(
					Play( SIZE/2, j, true, (*i), sc )
				);
			}

			++i;
		}
    }
    else
    {
        // This is the general case of playing a word on a board that is
        // in progress. Try all rows, then all columns. Make anagrams out
        // of all letters in the rack plus all letters already in the
        // row or column. The validity check will make sure that we only
        // play the letters already on the board in the same place that they
        // are already.
        int i, j;

        for ( i = mLowestRow; i <= mHighestRow; i++ )
        {
            Tiles existingTiles;
            for( j = 0; j < SIZE; j++ )
            {
                if ( mBoardLayout[i][j].isPlayed() )
                {
                    const Square sq( mBoardLayout[i][j] );

                    existingTiles.push_back( Tile( sq.letter(), sq.score(),
                            sq.isBlank() ) );
                }
            }

            Tiles row = rack;
            row.insert( row.end(), existingTiles.begin(), existingTiles.end() );
            std::list<Tiles> anagrams = mAnationary.tanagrams( Tile::letters( row ) );

            std::list<Tiles>::const_iterator ai = anagrams.begin();
            while ( ai != anagrams.end() )
            {
                int length = ai->size();
                for( j = 0; j <= SIZE - length; j++ )
                {
                    if ( isValid( i, j, true, *ai, rack ) )
                    {
                        int sc = score( i, j, true, *ai, rack );
                        plays.push_back( Play( i, j, true, *ai, sc ) );
                    }
                }

                ++ai;
            }
        }

        for ( j = mLowestCol; j <= mHighestCol; j++ )
        {
            Tiles existingTiles;
            for ( i = 0; i < SIZE; i++ )
            {
                if ( mBoardLayout[i][j].isPlayed() )
                {
                    const Square sq( mBoardLayout[i][j] );

                    existingTiles.push_back( Tile( sq.letter(), sq.score(),
                            sq.isBlank() ) );
                }
            }

            Tiles col = rack;
            col.insert( col.end(), existingTiles.begin(), existingTiles.end() );
            
            std::list<Tiles> anagrams = mAnationary.tanagrams( Tile::letters( col ) );
            std::list<Tiles>::const_iterator ai = anagrams.begin();
            while ( ai != anagrams.end() )
            {
                int length = ai->size();
                for ( i = 0; i <= SIZE - length; i++ )
                {
                    if ( isValid( i, j, false, *ai, rack ) )
                    {
                        int sc = score( i, j, false, *ai, rack );
                        plays.push_back( Play( i, j, false, *ai, sc ) );
                    }
                }

                ++ai;
            }
        }
    }
    
    plays.sort();

    return plays;
}

void Board::makePlay( int row, int col, bool horiz, const Tiles& word )
{
    int i = row;
    int j = col;
    Tiles::const_iterator wi = word.begin();

    while ( wi != word.end() )
    {
        if ( !mBoardLayout[i][j].isPlayed() )
        {
            mLowestRow = MIN( mLowestRow, PRED( i ) );
            mHighestRow = MAX( mHighestRow, SUCC( i ) );

            mLowestCol = MIN( mLowestCol, PRED( j ) );
            mHighestCol = MAX( mHighestCol, SUCC( j ) );

            if ( wi->isBlank() )
            {
                Tile tile( *wi );
                tile.score( 0 );
                tile.isBlank( false );
                mBoardLayout[i][j].play( tile );
            }
            else
            {
                mBoardLayout[i][j].play( *wi );
            }
        }

        ++wi;

        if ( horiz )
        {
            j++;
        }
        else
        {
            i++;
        }
    }

    setLastPlay( row, col, horiz, word.size() );
}

int Board::score( int row, int col, bool horiz, const Tiles& word,
        const Tiles& rack ) const
{
    int i = row;
    int j = col;
    Tiles::const_iterator wi = word.begin();
    int sc = 0;
    int tilesPlayed = 0;

    int wordMultiplier = 1;

    // Score the base word.
    while ( wi != word.end() )
    {
        // multipliers are set to 1 when a square is played
        int letterMultiplier = mBoardLayout[i][j].letterFactor();
        wordMultiplier *= mBoardLayout[i][j].wordFactor();

        if ( !mBoardLayout[i][j].isPlayed() )
        {
            tilesPlayed += 1;
            sc += letterMultiplier * wi->score();
        }
        else
        {
            sc += mBoardLayout[i][j].score();
        }

        ++wi;
        horiz ? j++ : i++;
    }

    sc *= wordMultiplier;

    if ( tilesPlayed == 7 )
    {
        sc += mBingoScore;
    }

    // Score any words due to adjoining letters
    i = row;
    j = col;
    wi = word.begin();

    while ( wi != word.end() )
    {
        // If this is a new tile, look for an adjoining word.
        if ( !mBoardLayout[i][j].isPlayed() )
        {
            int letterMultiplier = mBoardLayout[i][j].letterFactor();
            int wordMultiplier = mBoardLayout[i][j].wordFactor();

            Tiles adjoiningWord;

            if ( horiz )
            {
                if ( mBoardLayout[PRED(i)][j].isPlayed() ||
                     mBoardLayout[SUCC(i)][j].isPlayed() )
                {
                    adjoiningWord = getVerticalWord( i, j, *wi );
                }
            }
            else
            {
                if ( mBoardLayout[i][PRED(j)].isPlayed() ||
                     mBoardLayout[i][SUCC(j)].isPlayed() )
                {
                    adjoiningWord = getHorizontalWord( i, j, *wi );
                }
            }

            if ( !adjoiningWord.empty() )
            {
                Tiles::const_iterator awi = adjoiningWord.begin();
                int wordScore = 0;

                while ( awi != adjoiningWord.end() )
                {
                    wordScore += awi->score();
                    ++awi;
                }

                // If the letter was played on a letter multiplier, then
                // it needs to get the "rest" of it's value - it's already
                // been counted once as part of "adjoiningWord".
                wordScore += wi->score() * ( letterMultiplier - 1 );

                wordScore *= wordMultiplier;

                sc += wordScore;
            }
        }

        ++wi;
        horiz ? j++ : i++;
    }

    return sc;
}

bool Board::isValid( int row, int col, bool horiz,
                     const Tiles& word, const Tiles& rack ) const
{
    bool includesExistingLetter = false;
    bool extendsExistingWord = false;
    int len = word.size();

    // This code is to set up a simple conditional breakpoint when the
    // program is found to have excluded an allowed word, or included a
    // bogus one. The variable name is from the first example that the
    // breakpoint was used for - "hotdog" played as "hotdogthebe".
    int hotdog=0;

    if ( horiz && row == 13 && col == 3 && Tile::letters( word ) == "s.q" )
    {
        hotdog = 1;
    }

    // Test that the word does not go past the edge of the board
    if ( horiz )
    {
        if ( col + len > SIZE )
        {
            return false;
        }
    }
    else if ( row + len > SIZE )
    {
        return false;
    }

    // Test that letters in word match letters on the board or are new,
    // that there is at least one new letter played, and that the word
    // includes an existing letter.
    int i = row;
    int j = col;
    Tiles::const_iterator wi = word.begin();
    bool atLeastOneNewLetter = false;

    while ( wi != word.end() )
    {
        // If the board already has a character, then it must match the tile
        // in the word that we're playing.
        if ( mBoardLayout[i][j].isPlayed() )
        {
            if ( mBoardLayout[i][j].hasTile( *wi ) )
            {
                // The word must include an existing letter, or extend a word
                includesExistingLetter = true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            atLeastOneNewLetter = true;
        }

        ++wi;
        horiz ? j++ : i++;
    }

    if ( !atLeastOneNewLetter )
    {
        return false;
    }

    // Test that each new letter in word comes from the rack. We anagram
    // with all letters in the rack and on the board, which gives us a bunch
    // of invalid words because it would move existing tiles around on the
    // board. We'll need to weed those out.
    std::vector<int> remainingRack( 26 );
    int remainingBlanks = 0;
    Tiles::const_iterator ri = rack.begin();

    while ( ri != rack.end() )
    {
        if ( ri->isBlank() )
        {
            remainingBlanks += 1;
        }
        else
        {
            remainingRack[ ri->letter() - 'a' ] += 1;
        }
        ++ri;
    }

    i = row;
    j = col;
    wi = word.begin();

    while ( wi != word.end() )
    {
        if ( !mBoardLayout[i][j].isPlayed() )
        {
            if ( wi->isBlank() && remainingBlanks > 0 )
            {
                remainingBlanks -= 1;
            }
            else if ( remainingRack[ wi->letter() - 'a' ] > 0 )
            {
                remainingRack[ wi->letter() - 'a' ] -= 1;
            }
            else
            {
                return false;
            }
        }

        ++wi;
        horiz ? j++ : i++;
    }

    // Test that each existing letter touching a new letter forms a word
    i = row;
    j = col;
    wi = word.begin();

    while ( wi != word.end() )
    {
        // If we're laying a new tile, look for an adjoining word
        if ( !mBoardLayout[i][j].isPlayed() )
        {
            Tiles adjoiningWord;
            Tile playedLetter( *wi );

            playedLetter.isBlank( false );

            if ( horiz )
            {
                if ( mBoardLayout[PRED(i)][j].isPlayed() ||
                     mBoardLayout[SUCC(i)][j].isPlayed() )
                {
                    adjoiningWord = getVerticalWord( i, j, playedLetter );
                }
            }
            else
            {
                if ( mBoardLayout[i][PRED(j)].isPlayed() ||
                     mBoardLayout[i][SUCC(j)].isPlayed() )
                {
                    adjoiningWord = getHorizontalWord( i, j, playedLetter );
                }
            }

            if ( !adjoiningWord.empty() )
            {
                if ( mDictionary.find( Tile::letters( adjoiningWord ) ) )
                {
                    extendsExistingWord = true;
                }
                else
                {
                    return false;
                }
            }
        }

        if ( horiz )
        {
            j++;
        }
        else
        {
            i++;
        }

        ++wi;
    }

    // Test that there is no existing letter immediately before or 
    // immetiately after the word.
    if ( horiz )
    {
        if ( col > 0 && mBoardLayout[row][col-1].isPlayed() )
        {
            return false;
        }
        if ( ( col + len < SIZE ) && mBoardLayout[row][col+len].isPlayed() )
        {
            return false;
        }
    }
    else
    {
        if ( row > 0 && mBoardLayout[row-1][col].isPlayed() )
        {
            return false;
        }
        if ( ( row + len < SIZE ) && mBoardLayout[row+len][col].isPlayed() )
        {
            return false;
        }
    }

    return includesExistingLetter || extendsExistingWord;
}

Tiles Board::getVerticalWord( int row, int col, const Tile& tile ) const
{
    int i = row;
    Tiles word;

    while( i > 0 && mBoardLayout[PRED(i)][col].isPlayed() )
    {
        i--;
    }

    while( i < SIZE && ( i == row || mBoardLayout[i][col].isPlayed() ) )
    {
        const Square& sq( mBoardLayout[i][col] );

        if ( i == row )
        {
            word.push_back( tile );
        }
        else
        {
            word.push_back( Tile( sq.letter(), sq.score(), sq.isBlank() ) );
        }

        i++;
    }

    return word;
}

Tiles Board::getHorizontalWord( int row, int col, const Tile& tile ) const
{
    int j = col;
    Tiles word;

    while( j > 0 && mBoardLayout[row][PRED(j)].isPlayed() )
    {
        j--;
    }

    while( j < SIZE && ( j == col || mBoardLayout[row][j].isPlayed() ) )
    {
        const Square& sq( mBoardLayout[row][j] );

        if ( j == col )
        {
            word.push_back( tile );
        }
        else
        {
            word.push_back( Tile( sq.letter(), sq.score(), sq.isBlank() ) );
        }

        j++;
    }

    return word;
}

bool Board::isEmpty( void ) const
{
    return !mBoardLayout[SIZE/2][SIZE/2].isPlayed();
}

void Board::setLastPlay( int row, int col, bool horiz, int length )
{
    mLastPlay.mRow = row;
    mLastPlay.mCol = col;
    mLastPlay.mHorizontal = horiz;
    mLastPlay.mLength = length;
}

bool Board::partOfLastPlay( int row, int col ) const
{
    if ( mLastPlay.mHorizontal )
    {
        if ( row != mLastPlay.mRow )
        {
            return false;
        }
        if ( mLastPlay.mCol <= col && col < mLastPlay.mCol + mLastPlay.mLength )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if ( col != mLastPlay.mCol )
        {
            return false;
        }
        if ( mLastPlay.mRow <= row && row < mLastPlay.mRow + mLastPlay.mLength )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

std::ostream& operator<<( std::ostream& os, const Board& board )
{
    char hex[] = "0123456789ABCDEF";

    os << "   123456789ABCDEF" << std::endl;
    for ( int i = 0; i < board.SIZE; i++ )
    {
        os << hex[i + 1] << ": ";

        for ( int j = 0; j < board.SIZE; j++ )
        {
            if ( board.mBoardLayout[i][j].isPlayed() )
            {
                if ( board.partOfLastPlay( i, j ) )
                {
                    os << BOLD << BBLK << FWHT;
                }
                os << board.mBoardLayout[i][j].letter() << NORMAL;
            }
            else
            {
                int lf = board.mBoardLayout[i][j].letterFactor();
                int wf = board.mBoardLayout[i][j].wordFactor();

                if ( wf == 1 and lf == 1 )
                {
                    os << ' ' << NORMAL;
                }
                else
                {
                    if ( lf == 2 )
                    {
                        os << BBLU << FWHT << '2' << NORMAL;
                    }
                    else if ( lf == 3 )
                    {
                        os << BGRN << FWHT << '3' << NORMAL;
                    }

                    if ( wf == 2 )
                    {
                        os << BRED << FWHT << '2' << NORMAL;
                    }
                    else if ( wf == 3 )
                    {
                        os << BYEL << FWHT << '3' << NORMAL;
                    }
                }
            }
        }
        os << std::endl;
    }

    return os;
}

void Board::save( void )
{
    std::ofstream bf;
    bf.open( mBoardFileName.c_str() );

    for ( int i = 0; i < SIZE; i++ )
    {
        for( int j = 0; j < SIZE; j++ )
        {
            Square sq = mBoardLayout[i][j];
            if ( sq.isPlayed() )
            {
                if ( sq.score() > 0 )
                {
                    bf << sq.letter();
                }
                else
                {
                    bf << ((char)( sq.letter() ^ 0x20 ));
                }
            }
            else
            {
                int lf = mBoardLayout[i][j].letterFactor();
                int wf = mBoardLayout[i][j].wordFactor();

                if ( wf == 1 and lf == 1 )
                {
                    bf << ' ';
                }
                else
                {
                    if ( lf == 2 )
                    {
                        bf << '2';
                    }
                    else if ( lf == 3 )
                    {
                        bf << '3';
                    }

                    if ( wf == 2 )
                    {
                        bf << '@';
                    }
                    else if ( wf == 3 )
                    {
                        bf << '#';
                    }
                }
            }
        }
        bf << std::endl;
    }

    bf.close();
}

const Letters& Board::letters( void )
{
    return mLetters;
}
