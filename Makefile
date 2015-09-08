all: wwf scrabble dc dictionary

CPPFLAGS=-g #-O2 -fomit-frame-pointer -DUSE_ASSEMBLY -DGCC

clean:
	rm -f *.o wwf dc scrabble dictionary

wwf: wwf.o window.o attributes.o screen.o board.o letters.o anationary.o fingerprint.o play.o dictionary.o undo.o tile.o
	g++ -g -o wwf wwf.o window.o attributes.o screen.o board.o letters.o anationary.o fingerprint.o play.o dictionary.o undo.o tile.o -lncurses

dc: dc.o dictionary.o
	g++ -g -o dc dc.o dictionary.o

scrabble: wwf
	ln -sf wwf scrabble

dictionary: words dc
	./dc words

wwf.o: wwf.cpp anationary.h play.h board.h letters.h
board.o: board.cpp anationary.h play.h letters.h board.h tile.h square.h dictionary.h fingerprint.h
letters.o: letters.cpp letters.h tile.h
anationary.o: anationary.cpp anationary.h fingerprint.h tile.h
fingerprint.o: fingerprint.cpp fingerprint.h tile.h
play.o: play.cpp play.h board.h
dictionary.o: dictionary.cpp dictionary.h
dc.o: dc.cpp dictionary.h
screen.o: screen.cpp screen.h
attributes.o: attributes.cpp attributes.h
window.o: window.cpp window.h
undo.o: undo.cpp undo.h board.h
tile.o: tile.cpp tile.h
