#include <vector>
#include <iostream>
#include <sstream>
#include "screen.h"
#include "board.h"
#include "play.h"
#include "attributes.h"
#include "square.h"
#include "undo.h"
#include "tile.h"

Attributes NormalAttr( Normal, Black, White );

void updateBoard( Window& boardWin, const Board& board );
Tiles updateRack( Screen& s );
void updatePlays( Window& playsWin, const Board& board, const Tiles& rack );
void makePlay( Window& playsWin, Window& boardWin, Board& board, const Screen& s );
void trialPlay( Window& boardWin, Board &board, const Play& play, const Screen& s );
void opponentPlay( Board& board, Window& playsWin, Window& boardWin, Screen& s );
void instructions( Window& commandsWin,
        const std::list<std::string>& instructions );
void instructions( Window& commandsWin, const std::string& instructions );

static int MaxPlays;
static std::vector<Play> BestPlays;

static Undo UndoStore;

using namespace std;

int main( int argc, char**argv )
{
    std::string boardName = "board";
    std::string game = argv[0];
    game = game.substr( game.find_last_of( "/" ) +  1);

    if ( argc > 1 )
    {
        boardName = argv[1];
    }

    Board board( boardName, game );
    UndoStore.save( board );

    Screen s;
    Tiles rack;

    Window& playsWin =
        s.openWindow( "Plays", "Available Plays", 1, 50, s.height() - 2, 29 );
    MaxPlays = s.height() - 5;

    Window& commandsWin =
        s.openWindow( "Commands", "Instructions", 1, 1, s.height() - 2, 30 );

    Window& boardWin =
        s.openWindow( "Board", "Board", 1, 32, 18, 17 );

    updateBoard( boardWin, board );

    std::list<std::string> inst;

    char command;
    do
    {
        inst = std::list<std::string>();
        inst.push_back( "'p' - play from the rack" );
        inst.push_back( "'w' - place a word" );
        inst.push_back( "'u' - undo" );
        inst.push_back( "'r' - redo" );
        inst.push_back( "'s' - save" );
        inst.push_back( "'q' - quit" );
        instructions( commandsWin, inst );

        s.refresh();
        command = getch();

        switch( command )
        {
        case 'p':
            inst = std::list<std::string>();
            inst.push_back( "Type in your rack." );
            inst.push_back( "Use '.' for a blank tile." );
            inst.push_back( "Press <ESC> to cancel." );
            instructions( commandsWin, inst );

            rack = updateRack( s );
            updatePlays( playsWin, board, rack );
            instructions( commandsWin, "Choose a play." );
            makePlay( playsWin, boardWin, board, s );
            playsWin.clear();
            break;
        case 'w':
            instructions( commandsWin, "Type the word." );
            opponentPlay( board, playsWin, boardWin, s );
            break;
        case 'u':
            board = UndoStore.undo( board );
            updateBoard( boardWin, board );
            break;
        case 'r':
            board = UndoStore.redo( board );
            updateBoard( boardWin, board );
            break;
        case 's':
            board.save();
        }
        s.refresh();
    } while ( command != 'q' );


    return 0;
}

void updateBoard( Window& boardWin, const Board& board )
{
    Attributes normalAttr( NormalAttr );
    Attributes blankAttr( Normal, White, Cyan );
    Attributes lastPlayAttr( Bold, White, Black );
    Attributes lastPlayBlankAttr( Bold, Cyan, Black );
    Attributes doubleLetterAttr( Dim, White, Blue );
    Attributes tripleLetterAttr( Dim, White, Green );
    Attributes doubleWordAttr( Dim, White, Red );
    Attributes tripleWordAttr( Dim, White, Yellow );

    for ( int row = 0; row < 15; row++ )
    {
        for ( int col = 0; col < 15; col++ )
        {
            Square sq = board.square( row, col );

            Attributes attr;
            char ch;

            if ( sq.isPlayed() )
            {
                if( board.partOfLastPlay( row, col ) )
                {
                    if ( sq.score() == 0 )
                    {
                        attr = lastPlayBlankAttr;
                    }
                    else
                    {
                        attr = lastPlayAttr;
                    }
                }
                else
                {
                    if ( sq.score() == 0 )
                    {
                        attr = blankAttr;
                    }
                    else
                    {
                        attr = normalAttr;
                    }
                }
                ch = sq.letter();
            }
            else
            {
                if ( sq.letterFactor() == 2 )
                {
                    attr = doubleLetterAttr;
                    ch = '2';
                }
                else if ( sq.letterFactor() == 3 )
                {
                    attr = tripleLetterAttr;
                    ch = '3';
                }
                else if ( sq.wordFactor() == 2 )
                {
                    attr = doubleWordAttr;
                    ch = '2';
                }
                else if ( sq.wordFactor() == 3 )
                {
                    attr = tripleWordAttr;
                    ch = '3';
                }
                else
                {
                    attr = normalAttr;
                    ch = ' ';
                }
            }
            boardWin.addChar( Character( row + 2, col + 1, ch, attr ) );
        }
    }
}

#define ALT_BACKSPACE 127
Tiles updateRack( Screen& s )
{
    int ch;
    Tiles rack;

    Window& rackWin =
        s.openWindow( "Rack", "Rack", 19, 32, 4, 17 );

    rackWin.clear();
    rackWin.setCursor( 2, 0 );
    rackWin.setFocus( s );
    s.refresh();

    while ( ( ch = getch() ) != '\n' )
    {
        if ( ch == KEY_BACKSPACE || ch == ALT_BACKSPACE )
        {
            if ( !rack.empty() )
            {
                rack.pop_back();
            }
        }
        else if ( ch == '\033' )
        {
            s.closeWindow( "Rack" );
            s.cursorLocation( 0, 0 );
            return Tiles();
        }
        else if ( rack.size() < 7 && ( ( 'a' <= ch && ch <= 'z' ) || ch == '.') )
        {
            rack.push_back( Board::letters().tile( ch, ch == '.' ) );
        }

        rackWin.clear();
        rackWin.setCursor( 2, 0 );
        Tiles::iterator i = rack.begin();
        int c = 0;
        while ( i != rack.end() )
        {
            rackWin.addChar( Character( 2, ++c, i->letter(), NormalAttr ) );
            ++i;
        }

        rackWin.setFocus( s );
        s.refresh();
    }

    s.closeWindow( "Rack" );
    s.cursorLocation( 0, 0 );
    return rack;
}

void updatePlays( Window& playsWin, const Board& board, const Tiles& rack )
{
    BestPlays = std::vector<Play>();

    std::list<Play> plays = board.findAllPlays( rack );
    std::list<Play>::reverse_iterator ri = plays.rbegin();

    playsWin.clear();
    playsWin.setCursor( 2, 0, NormalAttr );
    while ( ri != plays.rend() && BestPlays.size() < MaxPlays )
    {
        std::ostringstream plaStr;
        plaStr << *ri;
        playsWin.typeLine( plaStr.str() );

        BestPlays.push_back( *ri );
        ++ri;
    }
}

void makePlay( Window& playsWin, Window& boardWin, Board& board, const Screen& s )
{
    if ( BestPlays.size() == 0 )
    {
        return;
    }

    int currentPlay = 0;
    int ch;
    Attributes highlightAttr( Bold, White, Magenta );
    do
    {
        // highlight the line in playsWin
        playsWin.chAttr( currentPlay + 2, 1, 1, 27, highlightAttr );

        // redraw the board with the play highlighted
        updateBoard( boardWin, board );
        trialPlay( boardWin, board, BestPlays[currentPlay], s );

        s.refresh();
        ch = getch();
        playsWin.chAttr( currentPlay + 2, 1, 1, 27, NormalAttr );
        switch ( ch )
        {
        case KEY_UP:
            currentPlay--;
            if ( currentPlay < 0 ) currentPlay = 0;
            break;
        case KEY_DOWN:
            currentPlay++;
            if ( currentPlay == BestPlays.size() )
                currentPlay = BestPlays.size() - 1;
            break;
        case '\n':
            UndoStore.save( board );
            BestPlays[currentPlay].play( board );
            updateBoard( boardWin, board );
            break;
        case '\033':
            updateBoard( boardWin, board );
            break;
        }
    } while ( ch != '\033' && ch != '\n' );
}

void trialPlay( Window& boardWin, Board &board, const Play& play, const Screen& s )
{
    Attributes highlightAttr( Bold, White, Magenta );
    Attributes highlightBlankAttr( Bold, Cyan, Magenta );
    int r = play.row();
    int c = play.col();
    Tiles word = play.tiles();
    bool h = play.horizontal();

    Tiles::iterator i = word.begin();
    while ( i != word.end() )
    {
        if ( i->isBlank() )
        {
            boardWin.addChar(
                Character( r + 2, c + 1, i->letter(), highlightBlankAttr ) );
        }
        else
        {
            boardWin.addChar(
                Character( r + 2, c + 1, i->letter(), highlightAttr ) );
        }
        if ( h ) ++c; else ++r;
        ++i;
    }
}

void opponentPlay( Board& board, Window& playsWin, Window& boardWin, Screen& s )
{
    Window& wordWin = 
        s.openWindow( "Word", "Opponent's Word", 19, 32, 4, 17 );

    wordWin.setCursor( 2, 0 );
    wordWin.setFocus( s );
    s.refresh();

    Tiles word;
    int ch;
    while ( ( ch = getch() ) != '\n' )
    {
        if ( ch == KEY_BACKSPACE || ch == ALT_BACKSPACE )
        {
            if ( word.size() > 0 )
            {
                word.pop_back();
            }
        }
        else if ( ch == '\033' )
        {
            s.closeWindow( "Word" );
            s.cursorLocation( 0, 0 );
            return;
        }
        else if ( word.size() < 15 )
        {
            if ( 'a' <= ch && ch <= 'z' )
            {
                word.push_back( Board::letters().tile( ch, false ) );
            }
            else if ( 'A' <= ch  && ch <= 'Z' )
            {
                word.push_back( Board::letters().tile( ch ^ 0x20, true ) );
            }
        }

        wordWin.clear();
        wordWin.setCursor( 2, 0 );
        Tiles::iterator i = word.begin();
        int c = 0;
        Attributes blankAttr( Normal, Cyan, White );
        while ( i != word.end() )
        {
            if ( i->isBlank() )
            {
                wordWin.addChar( Character( 2, ++c, i->letter(), blankAttr ) );
            }
            else
            {
                wordWin.addChar( Character( 2, ++c, i->letter(), NormalAttr ) );
            }
            ++i;
        }

        wordWin.setFocus( s );
        s.refresh();
    }

    s.closeWindow( "Word" );
    s.cursorLocation( 0, 0 );

    BestPlays = std::vector<Play>();

    std::list<Play> plays = board.findAllPlays( word );
    std::list<Play>::reverse_iterator ri = plays.rbegin();

    playsWin.clear();
    playsWin.setCursor( 2, 0, NormalAttr );
    
    // Remove all the blanks for testing if the word matches the played
    // word.
    Tiles::iterator bi = word.begin();
   
    while ( bi != word.end() )
    {
        bi->isBlank( false );
        bi++;
    }
    std::string wordStr = Tile::letters( word );

    while ( ri != plays.rend() && BestPlays.size() < MaxPlays )
    {
        Tiles play = ri->tiles();
        Tiles::iterator bi = play.begin();

        while ( bi != play.end() )
        {
            bi->isBlank( false );
            bi++;
        }
        std::string playStr = Tile::letters( play );

        if ( wordStr == playStr )
        {
            std::ostringstream plaStream;
            plaStream << *ri;
            playsWin.typeLine( plaStream.str() );

            BestPlays.push_back( *ri );
        }
        ++ri;
    }

    makePlay( playsWin, boardWin, board, s );

    playsWin.clear();

    s.refresh();
}

void instructions( Window& commandsWin,
        const std::list<std::string>& instructions )
{
    commandsWin.clear();
    commandsWin.setCursor(0,0);

    std::list<std::string>::const_iterator i = instructions.begin();
    while ( i != instructions.end() )
    {
        commandsWin.typeLine( *i );
        ++i;
    }
}

void instructions( Window& commandsWin, const std::string& instructions )
{
    commandsWin.clear();
    commandsWin.setCursor(0,0);

    commandsWin.typeLine( instructions );
}
