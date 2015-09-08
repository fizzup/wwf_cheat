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

#ifndef __FINGERPRINT_H__
#define __FINGERPRINT_H__

#include <string>
#include <vector>
#include <list>
#include "tile.h"

// A fingerprint of a word, or a rack of tiles, is the count of
// each letter type in the word or rack. For example, "anagram" has a
// fingerprint of a3g1m1n1r1 (zeroes skipped). If a word has a lower or equal
// number for each of its letters when compared to a rack, then it's a
// sub-anagram of the rack. Continuing with the example, "manga" (a2g1m1n1)
// is a a sub-anagram of "anagram", but "grandma" (a2d1g1m1n1r1) is not - it has
// d1 in its fingerprint, but "anagram" has d0. Fingerprints make searching for
// anagrams faster because they save working out all the candidate words
// that a rack can form and then looking them up in a dictionary.
// Additionally, Fingerprints model blank tiles in a rack elegantly, but
// finding all candidate words when there are blanks in the rack is very
// slow.

class Fingerprint
{
public:
    Fingerprint( const std::string& word );
    ~Fingerprint( void ) {};

    bool isSubAnagram( const Fingerprint& rack ) const;

    const std::string& word( void ) const
        { return mWord; };

    // This method is scabble-specific. Everything else about the class
    // is a generic anagram finder. This can be removed, and the class
    // can be re-used without linking against any of the scrabble classes.
    std::list<Tiles> adjustForBlanks( const Fingerprint& rack ) const;

private:
    static const int LETTERS = 26;
    std::string mWord;
    std::vector<int> mLetterCount;
    int mBlankCount;

    static int mLetterSequence[LETTERS];
    static int mLetterUnsequence[LETTERS];

    static bool isBlank( char ch );
};

#endif
