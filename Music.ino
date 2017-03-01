/*
  was: Multiple tone player
  Now: Play some music!

 Plays a sequence of notes that make up a song

 circuit:
 * Some kind of speaker on pin 11

 created 6 December 2014
 by Hank Barta
 based on a snippet from Greg Borenstein later modified by Tom Igoe
 Original music arranged from a composition byu James Lord Pierpont.

 This example code is in the public domain.

 */
 
 #define ALL 1
 
#include "pitches.h"
#include "EventFramework.h"

//#define NO_PPROGMEM

#if defined NO_PPROGMEM
  #define PROGMEM
  #define pgm_read_word(x) (*(x))
#endif

#define REST 0
static const int interNoteDelay = 20;
static const int speaker1 = 11;
//static const int measure = 500;
static const int measure = 1000; // change this value to change the pace of the music playback

static const int whole = measure - interNoteDelay;
static const int half = measure / 2 - interNoteDelay;
static const int quarter = measure / 4 - interNoteDelay;
static const int quarter_triplet = (measure*2) / (3*3) - interNoteDelay;
static const int eighth = measure / 8 - interNoteDelay;
static const int sixteenth = measure / 19 - interNoteDelay;
static const int dotted_eighth = (measure * 3) / 16 - interNoteDelay;
static const int dotted_quarter = (measure * 3) / 8 - interNoteDelay;
static const int dotted_half = (measure * 3) / 4 - interNoteDelay;

typedef unsigned long ulong; // unsigned long int gets a bit tedious

typedef struct {
  int  note_;      // pitch or 0 for a rest
  int  duration_;  // duration of the note
  int getNote() const { return pgm_read_word(&note_); };
  int getDuration() const { return pgm_read_word(&duration_); };
} Note;

// a song is a sequence of phrases. Many are repeated so instead of just
// slavishly copying them, we write them once and reuse them as needed.
typedef struct {
  const Note* notes_;      // an array of notes
  int   noteCount_;  // number of notes in the array
  const PROGMEM Note * getNotePtr() const { return (const PROGMEM Note *) pgm_read_word(&notes_); }
  const int getNoteCount() const { return pgm_read_word(&noteCount_); }
} Phrase;

// Some macros to make construction of songs a little easier
#define ARRAY_COUNT(X) (sizeof(X)/sizeof(X[0]))
#define PHRASE(X) {X, ARRAY_COUNT(X)}


static const Note  Jingle_Bells_melody_1[] PROGMEM = {
  { NOTE_D4, quarter }, { NOTE_B4, quarter }, { NOTE_A4, quarter }, { NOTE_G4, quarter },
  { NOTE_D4, half }, { REST, quarter }, { NOTE_D4, quarter },
  { NOTE_D4, quarter }, { NOTE_B4, quarter }, { NOTE_A4, quarter }, { NOTE_G4, quarter },
  { NOTE_E4, half }, { REST, quarter }, { NOTE_E4, quarter },
  { NOTE_E4, quarter }, { NOTE_C5, quarter }, { NOTE_B4, quarter }, { NOTE_A4, quarter },
};

static const Note  Jingle_Bells_melody_2[] PROGMEM  = {
  { NOTE_FS4, half }, { REST, quarter }, { NOTE_FS4, quarter },
  { NOTE_D5, quarter }, { NOTE_D5, quarter }, { NOTE_C5, quarter }, { NOTE_A4, quarter },
  { NOTE_B4, half }, { REST, quarter }, { NOTE_D4, quarter },
};

static const Note  Jingle_Bells_melody_3[] PROGMEM  = {
  { NOTE_D5, quarter }, { NOTE_D5, quarter }, { NOTE_D5, quarter }, { NOTE_D5, quarter },
  { NOTE_E5, quarter }, { NOTE_D5, quarter }, { NOTE_C5, quarter }, { NOTE_A4, quarter },
  { NOTE_G4, half}, { NOTE_D5, half },
};

static const Note  Jingle_Bells_chorus[] PROGMEM  =  {
  { NOTE_B4, quarter }, { NOTE_B4, quarter }, { NOTE_B4, half },
  { NOTE_B4, quarter }, { NOTE_B4, quarter }, { NOTE_B4, half },
  { NOTE_B4, quarter }, { NOTE_D5, quarter }, { NOTE_G4, dotted_quarter }, { NOTE_A4, eighth },
  { NOTE_B4, whole },
  { NOTE_C5, quarter }, { NOTE_C5, quarter }, { NOTE_C5, quarter }, { NOTE_C5, quarter },
  { NOTE_C5, quarter }, { NOTE_B4, quarter }, { NOTE_B4, quarter }, { NOTE_B4, eighth }, { NOTE_B4, eighth },
};

static const Note  Jingle_Bells_end1[] PROGMEM =  {
  { NOTE_B4, quarter }, { NOTE_A4, quarter }, { NOTE_A4, quarter }, { NOTE_B4, quarter },
  { NOTE_A4, half }, { NOTE_D5, half },
};

static const Note  Jingle_Bells_end2[] PROGMEM =  {
  { NOTE_D5, quarter }, { NOTE_D5, quarter }, { NOTE_C5, quarter }, { NOTE_A4, quarter },
  { NOTE_G4, whole },
};

static const Phrase Jingle_Bells[] PROGMEM = {
  PHRASE(Jingle_Bells_melody_1),
  PHRASE(Jingle_Bells_melody_2),
  PHRASE(Jingle_Bells_melody_1),
  PHRASE(Jingle_Bells_melody_3),
  PHRASE(Jingle_Bells_chorus),
  PHRASE(Jingle_Bells_end1),
  PHRASE(Jingle_Bells_chorus),
  PHRASE(Jingle_Bells_end2),
};

#if defined ALL
static const Note  Seven_Nation_Army_notes[] PROGMEM  = {
  { NOTE_E3, 730 },
  { NOTE_E3, 280 },
  { NOTE_G3, 310 },
  { NOTE_E3, 370 },
  { NOTE_D3, 340 },
  { NOTE_C3, 900 },
  { NOTE_B2, 835 },
  { REST,    300 },
  { NOTE_E3, 730 },
  { NOTE_E3, 310 },
  { NOTE_G3, 310 },
  { NOTE_E3, 390 },
  { NOTE_D3, 320 },
  { NOTE_C3, 320 },
  { NOTE_D3, 320 },
  { NOTE_C3, 330 },
  { NOTE_B2, 700 },
};

static const Phrase Seven_Nation_Army[]  PROGMEM  = {
    PHRASE(Seven_Nation_Army_notes),
};

static const Note  Toms_Flourish_Notes[]  PROGMEM = {
  { NOTE_C3, 50 }, { REST, 200 }, { NOTE_E3, 50 }, { REST, 200 }, { NOTE_G3, 50 }, { REST, 200 },
  { NOTE_C3, 50 }, { REST, 200 }, { NOTE_F3, 50 }, { REST, 200 }, { NOTE_A3, 50 }, { REST, 200 },
  { REST, 30 },
  { NOTE_C3, 75 },  { NOTE_E3, 75 }, { NOTE_G3, 75 },
  { NOTE_C3, 60 },  { NOTE_F3, 60 }, { NOTE_A3, 60 },
  { NOTE_C3, 50 },  { NOTE_E3, 50 }, { NOTE_G3, 50 },
  { NOTE_C4, 50 },  { NOTE_E4, 50 }, { NOTE_G4, 50 },
  { NOTE_C3, 50 },  { NOTE_F3, 50 }, { NOTE_A3, 50 },
  { NOTE_C4, 50 },  { NOTE_F4, 50 }, { NOTE_A4, 50 },
  { NOTE_C3, 50 },  { NOTE_E3, 50 }, { NOTE_G3, 50 },
  { NOTE_C3, 50 },  { NOTE_F3, 50 }, { NOTE_A3, 50 },
  { NOTE_C3, 40 },  { NOTE_F3, 40 }, { NOTE_A3, 40 },
  { NOTE_C4, 40 },  { NOTE_F4, 40 }, { NOTE_A4, 40 },
  { NOTE_C5, 30 },  { NOTE_F5, 30 }, { NOTE_A5, 30 },
  { NOTE_C4, 25 },  { NOTE_F4, 25 }, { NOTE_A4, 25 },
  { NOTE_C6, 20 },  { NOTE_F6, 20 }, { NOTE_A6, 20 },
  { NOTE_C7, 15 },  { NOTE_C6, 15 },  { NOTE_C5, 15 },  { NOTE_C4, 15 },  { NOTE_C3, 15 },  { NOTE_C6, 120 },
  { REST, 3*1000 },
};

static const Phrase Toms_Flourish[]  PROGMEM  = {
    PHRASE(Toms_Flourish_Notes),
};

#define S1(n) (n*8/6)
static const Note Ode_to_Joy_Notes_1[]  PROGMEM  = {
    { NOTE_B3, S1(quarter) }, { NOTE_B3, S1(quarter) }, { NOTE_C4, S1(quarter) }, { NOTE_D4, S1(quarter) }, 
    { NOTE_D4, S1(quarter) }, { NOTE_C4, S1(quarter) }, { NOTE_B3, S1(quarter) }, { NOTE_A3, S1(quarter) }, 
    { NOTE_G3, S1(quarter) }, { NOTE_G3, S1(quarter) }, { NOTE_A3, S1(quarter) }, { NOTE_B3, S1(quarter) }, 
};

static const Note Ode_to_Joy_Notes_2[]  PROGMEM = {
    { NOTE_B3, S1(dotted_quarter) }, { NOTE_A3, S1(eighth) }, { NOTE_A3, S1(half) }, 
};

static const Note Ode_to_Joy_Notes_3[]  PROGMEM = {
    { NOTE_A3, S1(dotted_quarter) }, { NOTE_G3, S1(eighth) }, { NOTE_G3, S1(half) }, 
};

static const Note Ode_to_Joy_Notes_4[]  PROGMEM = {
    { NOTE_A3, S1(quarter) }, { NOTE_A3, S1(quarter) }, { NOTE_B3, S1(quarter) }, { NOTE_G3, S1(quarter) }, 
    { NOTE_A3, S1(quarter) }, { NOTE_B3, S1(eighth) }, { NOTE_C4, S1(eighth) }, { NOTE_B3, S1(quarter) }, { NOTE_G3, S1(quarter) }, 
    { NOTE_A3, S1(quarter) }, { NOTE_B3, S1(eighth) }, { NOTE_C4, S1(eighth) }, { NOTE_B3, S1(quarter) }, { NOTE_A3, S1(quarter) }, 
    { NOTE_G3, S1(quarter) }, { NOTE_A3, S1(quarter) }, { NOTE_D3, S1(half) }, 
};


static const Phrase Ode_to_Joy[]  PROGMEM = {
    PHRASE(Ode_to_Joy_Notes_1),
    PHRASE(Ode_to_Joy_Notes_2),
    PHRASE(Ode_to_Joy_Notes_1),
    PHRASE(Ode_to_Joy_Notes_3),
    PHRASE(Ode_to_Joy_Notes_4),
    PHRASE(Ode_to_Joy_Notes_1),
    PHRASE(Ode_to_Joy_Notes_3),
};

#define S2(n) (n*5/3)
static const Note Up_on_the_Housetop_Notes_1[]  PROGMEM  = {
  { NOTE_F4, S2(quarter) },  { NOTE_F4, S2(eighth) },  { NOTE_G4, S2(eighth) },  { NOTE_F4, S2(quarter) },  { NOTE_D4, S2(quarter) },  
  { NOTE_AS3, S2(quarter) },  { NOTE_D4, S2(quarter) },  { NOTE_F4, S2(half) },  
  { NOTE_G4, S2(quarter) },  { NOTE_G4, S2(quarter) },  { NOTE_F4, S2(quarter) },  { NOTE_D4, S2(quarter) },  
  { NOTE_C4, S2(quarter) },  { NOTE_F4, S2(quarter) },  { NOTE_F4, S2(half) },  
  { NOTE_F4, S2(quarter) },  { NOTE_F4, S2(eighth) },  { NOTE_G4, S2(eighth) },  { NOTE_F4, S2(quarter) },  { NOTE_D4, S2(eighth) },  { NOTE_D4, S2(eighth) },  
  { NOTE_AS3, S2(quarter) },  { NOTE_D4, S2(quarter) },  { NOTE_F4, S2(half) },  
  { NOTE_G4, S2(quarter) },  { NOTE_G4, S2(eighth) },  { NOTE_G4, S2(eighth) },  { NOTE_F4, S2(eighth) },  { NOTE_F4, S2(eighth) },  { NOTE_D4, S2(quarter) },  
  { NOTE_C4, S2(quarter) },  { NOTE_F4, S2(quarter) },  { NOTE_AS3, S2(half) },  
  { NOTE_DS4, S2(quarter) },  { NOTE_DS4, S2(quarter) },  { NOTE_G4, S2(half) },  
  { NOTE_F4, S2(quarter) },  { NOTE_F4, S2(eighth) },  { NOTE_F4, S2(eighth) },  { NOTE_D4, S2(half) },  
  { NOTE_C4, S2(quarter) },  { NOTE_C4, S2(quarter) },  { NOTE_DS4, S2(half) },  
  { NOTE_D4, S2(quarter) },  { NOTE_F4, S2(eighth) },  { NOTE_F4, S2(eighth) },  { NOTE_AS3, S2(quarter) },  { NOTE_D4, S2(quarter) },  
  { NOTE_F4, S2(quarter) },  { NOTE_F4, S2(eighth) },  { NOTE_G4, S2(eighth) },  { NOTE_F4, S2(quarter) },  { NOTE_D4, S2(quarter) },  
  { NOTE_DS4, S2(quarter) },  { NOTE_F4, S2(quarter) },  { NOTE_G4, S2(half) },  
  { NOTE_F4, S2(quarter) },  { NOTE_F4, S2(eighth) },  { NOTE_G4, S2(eighth) },  { NOTE_F4, S2(quarter) },  { NOTE_D4, S2(eighth) },  { NOTE_D4, S2(eighth) },  
  { NOTE_C4, S2(quarter) },  { NOTE_F4, S2(quarter) },  { NOTE_AS3, S2(half) },  
};

static const Phrase Up_on_the_Housetop[]  PROGMEM = {
  PHRASE(Up_on_the_Housetop_Notes_1),
};
#endif // defined ALL
static const Note Amazing_1[]  PROGMEM  = {
  { NOTE_AF4, eighth }, { NOTE_AF4, eighth }, { NOTE_AF4, eighth }, { NOTE_AF4, eighth },
  { NOTE_AF4, eighth }, { NOTE_G4, dotted_half+eighth },
  { REST, half }, { NOTE_AF4, eighth }, { NOTE_AF4, eighth }, { NOTE_AF4, eighth }, { NOTE_AF4, eighth }, 
  { NOTE_AF4, quarter }, { NOTE_G4, eighth }, { NOTE_F4, eighth }, { NOTE_EF4, eighth }, { NOTE_F4, eighth }, { NOTE_G4, eighth }, { NOTE_F4, eighth+quarter },
  { REST, eighth }, { NOTE_AF4, quarter }, { NOTE_AF4, eighth }, { NOTE_AF4, eighth }, { NOTE_AF4, eighth },
  { NOTE_AF4, eighth }, { NOTE_BF4, dotted_half+eighth }, 
  { REST, eighth }, { NOTE_BF3, eighth }, { NOTE_BF3, eighth }, { NOTE_G4, quarter }, { NOTE_F4, eighth }, { NOTE_F4, eighth }, { NOTE_EF4, eighth }, 
  { NOTE_EF4, whole },
  { REST, whole },
  { NOTE_AF4, eighth },
//  { NOTE_AF4, eighth }, { NOTE_AF4, quarter }, { NOTE_G4, quarter }, { NOTE_G4, quarter },
//  { NOTE_F4, quarter }, { NOTE_EF4, quarter }, { NOTE_EF4, quarter }, { NOTE_EF4, quarter }, { NOTE_F4, quarter }, { NOTE_G4, quarter },
//  { NOTE_F4, eighth }, { NOTE_EF4, eighth }, { NOTE_EF4, eighth+half }, { REST, half },
  
};
static const Phrase Amazing[]  PROGMEM = {
  PHRASE(Amazing_1),
};



class NotePlayerTimer:
  public efl::Timer  // periodic timer by default
{
  private:
    virtual bool callback(ulong late) {
      char buff[30];
      if ( pSong == 0 )    // a null pointer would be bad! (should assert())
        return false;
      if ( !playing )
        return false;       // disable the timer
      if ( pSong[phraseIndex].getNoteCount() > noteIndex ) { // still note(s) to play?
        setPeriod(pSong[phraseIndex].getNotePtr()[noteIndex].getDuration() + interNoteDelay); // timer to start the next note
	//Serial.print("play ");Serial.print(pSong[phraseIndex].notes[noteIndex].getNote());
	//Serial.print(" for ");Serial.println(pSong[phraseIndex].notes[noteIndex].getDuration());

        tone(speaker1, pSong[phraseIndex].getNotePtr()[noteIndex].getNote(), pSong[phraseIndex].getNotePtr()[noteIndex].getDuration());
        noteIndex++;            // next note to play
        return true;
      }
      else {
        phraseIndex++;                          // try the next phrase
        if ( phraseIndex < phraseCount ) {      // another phrase?
          noteIndex = 0;                      // index the first note in the new phrase
          if ( pSong[phraseIndex].getNoteCount() > noteIndex ) { // still note(s) to play?
            setPeriod(pSong[phraseIndex].getNotePtr()[noteIndex].getDuration() + interNoteDelay); // timer to start the next note
            tone(speaker1, pSong[phraseIndex].getNotePtr()[noteIndex].getNote(), pSong[phraseIndex].getNotePtr()[noteIndex].getDuration());
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
    Phrase const * pSong;          // pointer to the song we're playing right now.
  public:
    NotePlayerTimer(ulong c = 1):
      efl::Timer(c, 1) {  // start as tick periodic. We'll update the period based on the note duration.
    };
    void play( const Phrase *phrases, int sizeofPhrases, efl::LL<efl::Timer> &listElt) {
      char buff[30];
      pSong = phrases;
      phraseCount = sizeofPhrases;
      playing = true;
      phraseIndex = noteIndex = 0;
      listElt.add();
      return;
    }
    void stop() {
      playing = false;
    };
    bool isPlaying() {
      return playing;
    };

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
    HeartbeatTimer( int x = 0, int y = 0, int z = 0):
      efl::Timer(500, 250), heartbeatState(false) {
      pinMode(heartbeatPin, OUTPUT);      // should this be done in setup?
    };
};

HeartbeatTimer  heartBeat(0);          // not sure why but removing the first ctor arg causes compiler error
efl::LL<efl::Timer> hb(&heartBeat);

NotePlayerTimer notePlayer(0);
efl::LL<efl::Timer> np(&notePlayer);


void setup() {
  Serial.begin(115200);
  hb.add();
}

#define F1  100
#define t1  F1
#define F2  200
#define t2 F2


void loop() {
  static int state=0;
  if ( !notePlayer.isPlaying() ) {
    delay(1000);
    efl::LL<efl::Timer>::doItems();
    if( state >= 6)
      state = 5;
    switch(state) {
#if defined ALL
    case 0:
      notePlayer.play(Up_on_the_Housetop, ARRAY_COUNT(Up_on_the_Housetop), np);
      break;
    case 1:
      notePlayer.play(Ode_to_Joy, ARRAY_COUNT(Ode_to_Joy), np);
      break;
    case 2:
      notePlayer.play(Jingle_Bells, ARRAY_COUNT(Jingle_Bells), np);
      break;
    case 3:
      notePlayer.play(Toms_Flourish, ARRAY_COUNT(Toms_Flourish), np);
      break;
    case 4:
      notePlayer.play(Seven_Nation_Army, ARRAY_COUNT(Seven_Nation_Army), np);
      break;
#endif //defined ALL
    case 5:
      notePlayer.play(Amazing, ARRAY_COUNT(Amazing), np);
      break;

    default:
      //state = 2;
	;
    }
    state++;
      
  }
  efl::LL<efl::Timer>::doItems();

  delay(1);
}
