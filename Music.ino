/*
  was: Multiple tone player
  Now: Play some music!

 Plays a sequence of notes that make up a song

 circuit:
 * Some kind of speaker on pin 8

 created 6 December 2014
 by Hank Barta
 based on a snippet from Greg Borenstein later modified by Tom Igoe
 Original music arranged from a composition byu James Lord Pierpont.

This example code is in the public domain.

 */
#include "pitches.h"
#include "EventFramework.h"

#define REST 0
static const int interNoteDelay = 20;
static const int speaker1 = 6;
static const int measure = 800;
//static const int measure = 2400;

static const int whole = measure-interNoteDelay;
static const int half = measure/2-interNoteDelay;
static const int quarter = measure/4-interNoteDelay;
static const int eighth = measure/8-interNoteDelay;
static const int sixteenth = measure/19-interNoteDelay;
static const int dotted_eighth = (measure*3)/16-interNoteDelay;
static const int dotted_quarter = (measure*3)/8-interNoteDelay;
static const int dotted_half = (measure*3)/4-interNoteDelay;

typedef unsigned long ulong; // unsigned long int gets a bit tedious

typedef struct {
  int  note;      // pitch or 0 for a rest
  int  duration;  // duration of the note
} Note;

// a song is a sequence of phrases. Many are repeated so instead of just
// slavishly copying them, we write them once and reuse them as needed.
typedef struct {
  Note* notes;      // an array of notes
  int   noteCount;  // number of notes in the array
} Phrase;

// Some macros to make construction of songs a little easier
#define ARRAY_COUNT(X) (sizeof(X)/sizeof(X[0]))
#define PHRASE(X) {X, ARRAY_COUNT(X)}


Note  Jingle_Bells_melody_1[] = {
  { NOTE_D4, quarter }, { NOTE_B4, quarter }, { NOTE_A4, quarter }, { NOTE_G4, quarter }, 
  { NOTE_D4, half }, { REST, quarter }, { NOTE_D4, quarter }, 
  { NOTE_D4, quarter }, { NOTE_B4, quarter }, { NOTE_A4, quarter }, { NOTE_G4, quarter }, 
  { NOTE_E4, half }, { REST, quarter }, { NOTE_E4, quarter }, 
  { NOTE_E4, quarter }, { NOTE_C5, quarter }, { NOTE_B4, quarter }, { NOTE_A4, quarter }, };

Note  Jingle_Bells_melody_2[] = {
  { NOTE_FS4, half }, { REST, quarter }, { NOTE_FS4, quarter }, 
  { NOTE_D5, quarter }, { NOTE_D5, quarter }, { NOTE_C5, quarter }, { NOTE_A4, quarter },
  { NOTE_B4, half }, { REST, quarter }, { NOTE_D4, quarter },
};

Note  Jingle_Bells_melody_3[] = {
{ NOTE_D5, quarter }, { NOTE_D5, quarter }, { NOTE_D5, quarter }, { NOTE_D5, quarter }, 
  { NOTE_E5, quarter }, { NOTE_D5, quarter }, { NOTE_C5, quarter }, { NOTE_A4, quarter }, 
  { NOTE_G4, half}, { NOTE_D5, half }, };

Note  Jingle_Bells_chorus[] = {
  { NOTE_B4, quarter }, { NOTE_B4, quarter }, { NOTE_B4, half },
  { NOTE_B4, quarter }, { NOTE_B4, quarter }, { NOTE_B4, half },
  { NOTE_B4, quarter }, { NOTE_D5, quarter }, { NOTE_G4, dotted_quarter }, { NOTE_A4, eighth },
  { NOTE_B4, whole },
  { NOTE_C5, quarter }, { NOTE_C5, quarter }, { NOTE_C5, quarter }, { NOTE_C5, quarter },
  { NOTE_C5, quarter }, { NOTE_B4, quarter }, { NOTE_B4, quarter }, { NOTE_B4, eighth }, { NOTE_B4, eighth },
};

Note  Jingle_Bells_end1[] = {
  { NOTE_B4, quarter }, { NOTE_A4, quarter }, { NOTE_A4, quarter }, { NOTE_B4, quarter },  
  { NOTE_A4, half }, { NOTE_D5, half },
};

Note  Jingle_Bells_end2[] = {
  { NOTE_D5, quarter }, { NOTE_D5, quarter }, { NOTE_C5, quarter }, { NOTE_A4, quarter },
  { NOTE_G4, whole },
};
  
Phrase  Jingle_Bells[] = {
    PHRASE(Jingle_Bells_melody_1),
    PHRASE(Jingle_Bells_melody_2),
    PHRASE(Jingle_Bells_melody_1),
    PHRASE(Jingle_Bells_melody_3),
    PHRASE(Jingle_Bells_chorus),
    PHRASE(Jingle_Bells_end1),
    PHRASE(Jingle_Bells_chorus),
    PHRASE(Jingle_Bells_end2),
};



class NotePlayerTimer:
    public efl::Timer  // periodic timer by default
{
private:
    virtual bool callback(ulong late) {
        char buff[30];

        if( pSong == 0 )     // a null pointer would be bad! (should assert())
            return false;
        if( !playing )
            return false;       // disable the timer
        if( pSong[phraseIndex].noteCount > noteIndex ) { // still note(s) to play?
            setPeriod(pSong[phraseIndex].notes[noteIndex].duration+interNoteDelay); // timer to start the next note
            tone(speaker1, pSong[phraseIndex].notes[noteIndex].note, pSong[phraseIndex].notes[noteIndex].duration);
            noteIndex++;            // next note to play
            return true;
        }
        else {
            phraseIndex++;                          // try the next phrase
            if( phraseIndex < phraseCount ) {       // another phrase?
                noteIndex=0;                        // index the first note in the new phrase
                if( pSong[phraseIndex].noteCount > noteIndex ) { // still note(s) to play?
                    setPeriod(pSong[phraseIndex].notes[noteIndex].duration+interNoteDelay); // timer to start the next note
                    tone(speaker1, pSong[phraseIndex].notes[noteIndex].note, pSong[phraseIndex].notes[noteIndex].duration);
                    noteIndex++;            // next note to play
                    return true;
                }
                else { // odd situation - phrase with no notes
                    playing = false;
                    return 0;
                }
            }           // no more phrases
            else {
                playing = false;
                return 0;
            }

        }
        // unreachable? return true;
    };
    int     noteIndex;      // current position in the phrase
    int     phraseIndex;    // phrase we are currently working on
    int     phraseCount;    // how many phrases make up this song

    bool    playing;        // provide easy way to tell if we're playiing something
    Phrase* pSong;          // pointer to the song we're playing right now.
public:
    NotePlayerTimer(ulong c=1):
        efl::Timer(c,1) {   // start as tick periodic. We'll update the period based on the note duration.
    };
    void play( Phrase *phrases, int sizeofPhrases, efl::LL<efl::Timer> &listElt) {
        char buff[30];
        pSong = phrases;
        phraseCount = sizeofPhrases;
        playing = true;
        phraseIndex = noteIndex=0;
        listElt.add();
        return;
    }
    void stop() { playing = false; };
    bool isPlaying() { return playing; };
    
};


class HeartbeatTimer:
    public efl::Timer  // periodic timer by default
{
private:
    static const int heartbeatPin = 13;
    virtual bool callback(ulong late) {
        heartbeatState ^= 1;
        digitalWrite(heartbeatPin, heartbeatState);
        return true;
    };
    bool        heartbeatState;
public:
    HeartbeatTimer( int x=0, int y=0, int z=0):
        efl::Timer(500,250),heartbeatState(false) {
        pinMode(heartbeatPin, OUTPUT);      // should this be done in setup?
    };
};

  HeartbeatTimer  heartBeat( 0  );          // not sure why but removing the first ctor arg causes compiler error
  efl::LL<efl::Timer> hb(&heartBeat);

  NotePlayerTimer notePlayer(0);
  efl::LL<efl::Timer> np(&notePlayer);


void setup() {
  Serial.begin(115200);
  hb.add();
}

void loop() {
  efl::LL<efl::Timer>::doItems();
  if( !notePlayer.isPlaying() ) {
    delay(1000);
    efl::LL<efl::Timer>::doItems(); 
    notePlayer.play(Jingle_Bells, ARRAY_COUNT(Jingle_Bells), np);
  }

  delay(1);
}
