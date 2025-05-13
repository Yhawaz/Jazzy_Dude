/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.ff
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>

//Hardcoded
#define SAXTABLE_SIZE 64u
#define PIANOTABLE_SIZE 64u
#define SAX_SAMPLE_RATE 24000.0
#define PIANO_SAMPLE_RATE 9000.0
#define PHASE_BITS 24

//Tables for Freq's and Waveforms
const uint8 SaxTable[SAXTABLE_SIZE] = {    220, 241, 244, 238, 220, 183, 139, 115,
    117, 131, 142, 146, 140, 122,  95,  73,
     67,  69,  64,  42,  12,   0,   0,  32,
     71,  97, 103,  98,  90,  88,  93, 105,
    118, 131, 140, 148, 154, 159, 166, 174,
    182, 185, 183, 176, 164, 153, 146, 142,
    136, 129, 124, 121, 118, 117, 121, 127,
    127, 124, 124, 125, 121, 119, 137, 179  
};

const uint8 PianoTable[PIANOTABLE_SIZE] = {
    133, 130, 122, 109,  94,  91, 120, 168,
    189, 182, 182, 192, 204, 223, 243, 253,
    255, 246, 223, 190, 153, 120,  94,  76,
     57,  26,  10,   7,  17,  31,  46,  53,
     54,  56,  54,  63,  82, 100, 101,  91,
    101, 130, 141, 148, 163, 170, 177, 181,
    183, 190, 196, 199, 191, 174, 146, 112,
     95,  88,  78,  68,  70,  83, 107, 129
};

//Note Tables
const float SaxFreq [12]={196, 207.66, 220.01, 233.09, 246.95, 261.63, 277.19, 293.67, 311.13, 329.63, 349.23, 370};
const float PianoFreq [12]={196, 207.66, 220.01, 233.09, 246.95, 261.63, 277.19, 293.67, 311.13, 329.63, 349.23, 370};
static const char *NoteNames[12]={"A","As","B","C","Cs","D","Ds","E","F","Fs","G","Gs"};

//Global Vals(For Debugging Via LCD ISR)
    uint16_t pot1_value;
    uint16_t pot2_value;

    float sax_current_note_freq;
    float sax_atk_rate,sax_decay_rate,sax_release_rate;
    float sax_env_lvl;
    uint8_t sax_env_state;
    uint32_t sax_phase;
    uint32_t sax_phase_inc;

    uint8 sax_sample;
    int16_t sax_zero_centered;
    uint8_t sax_enveloped_note;
    float sax_env_out;

    float SAX_ATTACK_TIME = .1;
    float SAX_SUSTAIN_LEVEL = .7;
    float SAX_RELEASE_TIME = .3;
    float SAX_DECAY_TIME =.2;

    float piano_current_note_freq;
    float piano_atk_rate,piano_decay_rate,piano_release_rate;
    float piano_env_lvl;
    uint8_t piano_env_state;
    uint32_t piano_phase;
    uint32_t piano_phase_inc;

    uint8 piano_sample;
    int16_t piano_zero_centered;
    uint8_t piano_enveloped_note;
    float piano_env_out;

    float PIANO_ATTACK_TIME = .02;
    float PIANO_SUSTAIN_LEVEL = .7;
    float PIANO_RELEASE_TIME = .3;
    float PIANO_DECAY_TIME =.5;

    uint8_t bpm;
    
    typedef struct bar_note{
        uint8_t action;
        uint8_t note;
    } Bar_Note;
    typedef Bar_Note bar[16];
    typedef Bar_Note chart[12][16];
    
    //Beautiufl beautiful state variables
    //where in the bar we are 
    uint8_t bar_idx;
    //where in the chart we are
    uint8_t chard_idx;
    uint8_t mode=1;
    uint32_t bpm_array[7]={60,75,90,105,120,135,150};
   
 //Envelope Vars;
    enum {
        IDLE,
        ATTACK,
        DECAY,
        SUSTAIN,
        RELEASE
    };

    enum notes{
      A,
      As,
      B,
      C,
      Cs,
      D,
      Ds, 
      E,
      F,
      Fs,
      G,
      Gs
    };

    //3 states tbh, for each note in bar
    enum type_note{
      PLAY_NOTE,
      END_PLAY_NOTE,
      END_NOTE,
      NOP, 
    };

//Charts
chart megalovania_improved = {
  {
    {PLAY_NOTE, D}, {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, D},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, Ds}, {NOP, 0}, {NOP, 0},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, Gs}, {END_PLAY_NOTE, 0}
  },
  {
    {NOP, 0}, {PLAY_NOTE, G}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {NOP, 0}, {PLAY_NOTE, F}, {NOP, 0}, {NOP, 0},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, D}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, F},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, G}, {NOP, 0}, {END_NOTE, 0}
  },
  {
    {PLAY_NOTE, C}, {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, C},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, Ds}, {NOP, 0}, {NOP, 0},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, Gs}, {END_PLAY_NOTE, 0}
  },
  {
    {NOP, 0}, {PLAY_NOTE, G}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {NOP, 0}, {PLAY_NOTE, F}, {NOP, 0}, {NOP, 0},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, D}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, F},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, G}, {NOP, 0}, {END_NOTE, 0}
  },
  {
    {PLAY_NOTE, As}, {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, As},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, Ds}, {NOP, 0}, {NOP, 0},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, Gs}, {END_PLAY_NOTE, 0}
  },
  {
    {NOP, 0}, {PLAY_NOTE, G}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {NOP, 0}, {PLAY_NOTE, F}, {NOP, 0}, {NOP, 0},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, D}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, F},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, G}, {NOP, 0}, {END_NOTE, 0}
  },
  {
    {PLAY_NOTE, Gs}, {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, Gs},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, Ds}, {NOP, 0}, {NOP, 0},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, Gs}, {END_PLAY_NOTE, 0}
  },
  {
    {NOP, 0}, {PLAY_NOTE, G}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {NOP, 0}, {PLAY_NOTE, F}, {NOP, 0}, {NOP, 0},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, D}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, F},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, G}, {NOP, 0}, {END_NOTE, 0}
  },
  {
    {PLAY_NOTE, D}, {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, D},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, Ds}, {NOP, 0}, {NOP, 0},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, Gs}, {END_PLAY_NOTE, 0}
  },
  {
    {NOP, 0}, {PLAY_NOTE, G}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {NOP, 0}, {PLAY_NOTE, F}, {NOP, 0}, {NOP, 0},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, D}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, F},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, G}, {NOP, 0}, {END_NOTE, 0}
  },
  {
    {PLAY_NOTE, C}, {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, C},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, Ds}, {NOP, 0}, {NOP, 0},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, Gs}, {END_PLAY_NOTE, 0}
  },
  {
    {NOP, 0}, {PLAY_NOTE, G}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {NOP, 0}, {PLAY_NOTE, F}, {NOP, 0}, {NOP, 0},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, D}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, F},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, G}, {NOP, 0}, {END_NOTE, 0}
  }
};

// Jazz harmony for Megalovania
chart megalovania_jazz_piano = {
  {
    {PLAY_NOTE, D}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {PLAY_NOTE, F}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {PLAY_NOTE, A}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {NOP, 0}, {NOP, 0}, {PLAY_NOTE, D}, {NOP, 0},
    {END_NOTE, 0}, {PLAY_NOTE, F}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, As}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, C}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {PLAY_NOTE, E}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {PLAY_NOTE, As}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {NOP, 0}, {NOP, 0}, {PLAY_NOTE, F}, {NOP, 0},
    {END_NOTE, 0}, {PLAY_NOTE, A}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, D}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, As}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {PLAY_NOTE, D}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {PLAY_NOTE, F}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {NOP, 0}, {NOP, 0}, {PLAY_NOTE, A}, {NOP, 0},
    {END_NOTE, 0}, {PLAY_NOTE, C}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, G}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, G}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {PLAY_NOTE, As}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {PLAY_NOTE, D}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {NOP, 0}, {NOP, 0}, {PLAY_NOTE, A}, {NOP, 0},
    {END_NOTE, 0}, {PLAY_NOTE, Cs}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, G}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, D}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {PLAY_NOTE, F}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {PLAY_NOTE, A}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {NOP, 0}, {NOP, 0}, {PLAY_NOTE, D}, {NOP, 0},
    {END_NOTE, 0}, {PLAY_NOTE, F}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, As}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, C}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {PLAY_NOTE, E}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {PLAY_NOTE, As}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {NOP, 0}, {NOP, 0}, {PLAY_NOTE, D}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  }
};

// Hopes and Dreams - Undertale
chart hopes_and_dreams_sax = {
  {
    {PLAY_NOTE, B}, {NOP, 0}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, G}, {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, Fs},
    {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, G}, {END_PLAY_NOTE, 0}
  },
  {
    {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, B}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, C}, {NOP, 0}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, B}, {NOP, 0}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, A}, {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, G},
    {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, A}, {END_PLAY_NOTE, 0}
  },
  {
    {PLAY_NOTE, B}, {NOP, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, C}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, D}, {NOP, 0}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, C}, {NOP, 0}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, B}, {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, A},
    {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, B}, {END_PLAY_NOTE, 0}
  },
  {
    {PLAY_NOTE, C}, {NOP, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, D}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, G}, {NOP, 0}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, A}, {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, B},
    {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, C}, {NOP, 0},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, D}, {NOP, 0}, {END_PLAY_NOTE, 0}
  },
  {
    {PLAY_NOTE, G}, {NOP, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, B}, {NOP, 0}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, G}, {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, Fs},
    {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, G}, {END_PLAY_NOTE, 0}
  },
  {
    {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, B}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, D}, {NOP, 0}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, C}, {NOP, 0}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, B}, {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, A},
    {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, G}, {END_PLAY_NOTE, 0}
  },
  {
    {PLAY_NOTE, G}, {NOP, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  }
};

chart his_theme_piano = {
  {
    {PLAY_NOTE, C}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, E}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, G}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, E}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0}
  },
  {
    {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, C}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, E}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, C}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0}
  },
  {
    {PLAY_NOTE, F}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, C}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0}
  },
  {
    {PLAY_NOTE, G}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, B}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, D}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, B}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0}
  },
  {
    {PLAY_NOTE, C}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, E}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, G}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, E}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0}
  },
  {
    {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, C}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, E}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, C}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0}
  },
  {
    {PLAY_NOTE, D}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, F}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, F}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0}
  },
  {
    {PLAY_NOTE, G}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, B}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, F}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, D}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0}
  },
  {
    {PLAY_NOTE, E}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, G}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, B}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, G}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0}
  },
  {
    {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, C}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, G}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, E}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0}
  },
  {
    {PLAY_NOTE, F}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, C}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0}
  },
  {
    {PLAY_NOTE, C}, {NOP, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {END_NOTE, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  }
};
chart piano_harmony = {
  {
    {PLAY_NOTE, D},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {NOP, C},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {PLAY_NOTE, G},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {NOP, C},   {END_NOTE,0}, {NOP,0}, {NOP,0}
  },
  {
    {PLAY_NOTE, D},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {NOP, C},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {PLAY_NOTE, G},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {NOP, C},   {END_NOTE,0}, {NOP,0}, {NOP,0}
  },
  {
    {PLAY_NOTE, D},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {NOP, C},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {PLAY_NOTE, G},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {NOP, C},   {END_NOTE,0}, {NOP,0}, {NOP,0}
  },
  {
    {PLAY_NOTE, D},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {NOP, C},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {PLAY_NOTE, G},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {NOP, C},   {END_NOTE,0}, {NOP,0}, {NOP,0}
  },
  {
    {PLAY_NOTE, D},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {NOP, C},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {PLAY_NOTE, G},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {NOP, C},   {END_NOTE,0}, {NOP,0}, {NOP,0}
  },
  {
    {PLAY_NOTE, D},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {NOP, C},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {PLAY_NOTE, G},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {NOP, C},   {END_NOTE,0}, {NOP,0}, {NOP,0}
  },
  {
    {PLAY_NOTE, D},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {NOP, C},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {PLAY_NOTE, G},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {NOP, C},   {END_NOTE,0}, {NOP,0}, {NOP,0}
  },
  {
    {PLAY_NOTE,D},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {NOP, C},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {PLAY_NOTE, G},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {NOP, C},   {END_NOTE,0}, {NOP,0}, {NOP,0}
  },
  {
    {PLAY_NOTE, D},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {NOP, C},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {PLAY_NOTE, G},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {NOP, C},   {END_NOTE,0}, {NOP,0}, {NOP,0}
  },
  {
    {PLAY_NOTE, D},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {NOP, C},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {PLAY_NOTE, G},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {NOP, C},   {END_NOTE,0}, {NOP,0}, {NOP,0}
  },
  {
    {PLAY_NOTE, D},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {NOP, C},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {PLAY_NOTE, G},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {NOP, C},   {END_NOTE,0}, {NOP,0}, {NOP,0}
  },
  {
    {PLAY_NOTE, D},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {NOP, C},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {PLAY_NOTE, G},   {END_NOTE,0}, {NOP,0}, {NOP,0},
    {NOP, C},   {END_NOTE,0}, {NOP,0}, {NOP,0}
  }
};

chart straight_no_chaser = {
  {
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{PLAY_NOTE, Fs}, // Fs = F#
    {END_NOTE,0},{PLAY_NOTE, E}, {END_NOTE,0},{NOP,0}
  },
  {
    {PLAY_NOTE, D},   {END_PLAY_NOTE,0}, {PLAY_NOTE, Cs},{END_PLAY_NOTE,0},
    {PLAY_NOTE, D},   {END_PLAY_NOTE,0}, {PLAY_NOTE, As},{END_PLAY_NOTE,0},
    {PLAY_NOTE, G},   {END_PLAY_NOTE,0}, {PLAY_NOTE, Fs},{END_PLAY_NOTE,0},
    {PLAY_NOTE, G},   {END_PLAY_NOTE,0}, {PLAY_NOTE, A}, {END_PLAY_NOTE,0}
  },
  {
    {PLAY_NOTE, G},   {END_PLAY_NOTE,0}, {PLAY_NOTE, Fs},{END_PLAY_NOTE,0},
    {PLAY_NOTE, G},   {END_PLAY_NOTE,0}, {PLAY_NOTE, As},{END_PLAY_NOTE,0},
    {PLAY_NOTE, G},   {END_PLAY_NOTE,0}, {PLAY_NOTE, Fs},{END_PLAY_NOTE,0},
    {PLAY_NOTE, G},   {END_PLAY_NOTE,0}, {PLAY_NOTE, A}, {END_PLAY_NOTE,0}
  },
  {
    {PLAY_NOTE, Fs},  {END_PLAY_NOTE,0}, {PLAY_NOTE, G}, {END_PLAY_NOTE,0},
    {PLAY_NOTE, A},   {END_PLAY_NOTE,0}, {PLAY_NOTE, C}, {END_PLAY_NOTE,0},
    {PLAY_NOTE, D},   {END_PLAY_NOTE,0}, {PLAY_NOTE, Fs},{END_PLAY_NOTE,0},
    {PLAY_NOTE, G},   {END_PLAY_NOTE,0}, {END_NOTE,0},       {NOP,0}
  },
  {
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0}
  },
  {
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0}
  },
  {
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0}
  },
  {
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0}
  },
  {
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0}
  },
  {
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0}
  },
  {
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0},{NOP,0}
  }
};
chart good_megalovania={{ {PLAY_NOTE,F},{PLAY_NOTE,F},{PLAY_NOTE,F},{NOP,0},
  {PLAY_NOTE,C},{NOP,0},{NOP,0},{PLAY_NOTE,B},
  {NOP,0},{PLAY_NOTE,B},{NOP,0},{PLAY_NOTE,A},
  {PLAY_NOTE,A},{PLAY_NOTE,F},{PLAY_NOTE,A},{PLAY_NOTE,B}
},{ {PLAY_NOTE,F},{PLAY_NOTE,F},{PLAY_NOTE,F},{NOP,0},
  {PLAY_NOTE,C},{NOP,0},{NOP,0},{PLAY_NOTE,B},
  {NOP,0},{PLAY_NOTE,B},{NOP,0},{PLAY_NOTE,A},
  {PLAY_NOTE,A},{PLAY_NOTE,F},{PLAY_NOTE,A},{PLAY_NOTE,B}
},{ {PLAY_NOTE,F},{PLAY_NOTE,F},{PLAY_NOTE,F},{NOP,0},
  {PLAY_NOTE,C},{NOP,0},{NOP,0},{PLAY_NOTE,B},
  {NOP,0},{PLAY_NOTE,B},{NOP,0},{PLAY_NOTE,A},
  {PLAY_NOTE,A},{PLAY_NOTE,F},{PLAY_NOTE,A},{PLAY_NOTE,B}
},{ {PLAY_NOTE,F},{PLAY_NOTE,F},{PLAY_NOTE,F},{NOP,0},
  {PLAY_NOTE,C},{NOP,0},{NOP,0},{PLAY_NOTE,B},
  {NOP,0},{PLAY_NOTE,B},{NOP,0},{PLAY_NOTE,A},
  {PLAY_NOTE,A},{PLAY_NOTE,F},{PLAY_NOTE,A},{PLAY_NOTE,B}
},{ {PLAY_NOTE,F},{PLAY_NOTE,F},{PLAY_NOTE,F},{NOP,0},
  {PLAY_NOTE,C},{NOP,0},{NOP,0},{PLAY_NOTE,B},
  {NOP,0},{PLAY_NOTE,B},{NOP,0},{PLAY_NOTE,A},
  {PLAY_NOTE,A},{PLAY_NOTE,F},{PLAY_NOTE,A},{PLAY_NOTE,B}
},{ {PLAY_NOTE,F},{PLAY_NOTE,F},{PLAY_NOTE,F},{NOP,0},
  {PLAY_NOTE,C},{NOP,0},{NOP,0},{PLAY_NOTE,B},
  {NOP,0},{PLAY_NOTE,B},{NOP,0},{PLAY_NOTE,A},
  {PLAY_NOTE,A},{PLAY_NOTE,F},{PLAY_NOTE,A},{PLAY_NOTE,B}
},{ {PLAY_NOTE,F},{PLAY_NOTE,F},{PLAY_NOTE,F},{NOP,0},
  {PLAY_NOTE,C},{NOP,0},{NOP,0},{PLAY_NOTE,B},
  {NOP,0},{PLAY_NOTE,B},{NOP,0},{PLAY_NOTE,A},
  {PLAY_NOTE,A},{PLAY_NOTE,F},{PLAY_NOTE,A},{PLAY_NOTE,B}
},{ {PLAY_NOTE,F},{PLAY_NOTE,F},{PLAY_NOTE,F},{NOP,0},
  {PLAY_NOTE,C},{NOP,0},{NOP,0},{PLAY_NOTE,B},
  {NOP,0},{PLAY_NOTE,B},{NOP,0},{PLAY_NOTE,A},
  {PLAY_NOTE,A},{PLAY_NOTE,F},{PLAY_NOTE,A},{PLAY_NOTE,B}
},{ {PLAY_NOTE,F},{PLAY_NOTE,F},{PLAY_NOTE,F},{NOP,0},
  {PLAY_NOTE,C},{NOP,0},{NOP,0},{PLAY_NOTE,B},
  {NOP,0},{PLAY_NOTE,B},{NOP,0},{PLAY_NOTE,A},
  {PLAY_NOTE,A},{PLAY_NOTE,F},{PLAY_NOTE,A},{PLAY_NOTE,B}
},{ {PLAY_NOTE,F},{PLAY_NOTE,F},{PLAY_NOTE,F},{NOP,0},
  {PLAY_NOTE,C},{NOP,0},{NOP,0},{PLAY_NOTE,B},
  {NOP,0},{PLAY_NOTE,B},{NOP,0},{PLAY_NOTE,A},
  {PLAY_NOTE,A},{PLAY_NOTE,F},{PLAY_NOTE,A},{PLAY_NOTE,B}
},{ {PLAY_NOTE,F},{PLAY_NOTE,F},{PLAY_NOTE,F},{NOP,0},
  {PLAY_NOTE,C},{NOP,0},{NOP,0},{PLAY_NOTE,B},
  {NOP,0},{PLAY_NOTE,B},{NOP,0},{PLAY_NOTE,A},
  {PLAY_NOTE,A},{PLAY_NOTE,F},{PLAY_NOTE,A},{PLAY_NOTE,B}
},{ {PLAY_NOTE,F},{PLAY_NOTE,F},{PLAY_NOTE,F},{NOP,0},
  {PLAY_NOTE,C},{NOP,0},{NOP,0},{PLAY_NOTE,B},
  {NOP,0},{PLAY_NOTE,B},{NOP,0},{PLAY_NOTE,A},
  {PLAY_NOTE,A},{PLAY_NOTE,F},{PLAY_NOTE,A},{PLAY_NOTE,B}
}};


chart bad_megalovania = {
  {
    { PLAY_NOTE,    D }, { END_PLAY_NOTE, A  }, { END_PLAY_NOTE, Gs },
    { END_PLAY_NOTE, G }, { END_PLAY_NOTE, F  }, { END_PLAY_NOTE, D  },
    { END_PLAY_NOTE, F }, { END_PLAY_NOTE, G  },
    { END_PLAY_NOTE, D }, { END_PLAY_NOTE, A  }, { END_PLAY_NOTE, Gs },
    { END_PLAY_NOTE, G }, { END_PLAY_NOTE, F  }, { END_PLAY_NOTE, D  },
    { END_PLAY_NOTE, F }, { END_NOTE,      0 }
  },
  {
    { PLAY_NOTE,    D }, { END_PLAY_NOTE, A  }, { END_PLAY_NOTE, Gs },
    { END_PLAY_NOTE, G }, { END_PLAY_NOTE, F  }, { END_PLAY_NOTE, D  },
    { END_PLAY_NOTE, F }, { END_PLAY_NOTE, G  },
    { END_PLAY_NOTE, D }, { END_PLAY_NOTE, A  }, { END_PLAY_NOTE, Gs },
    { END_PLAY_NOTE, G }, { END_PLAY_NOTE,F  }, { END_PLAY_NOTE, D  },
    { END_PLAY_NOTE, F }, { END_NOTE,0 }
  },
  {
    { PLAY_NOTE,    D }, { END_PLAY_NOTE, A  }, { END_PLAY_NOTE, Gs },
    { END_PLAY_NOTE, G }, { END_PLAY_NOTE, F  }, { END_PLAY_NOTE, D  },
    { END_PLAY_NOTE, F }, { END_PLAY_NOTE, G  },
    { END_PLAY_NOTE, D }, { END_PLAY_NOTE, A  }, { END_PLAY_NOTE, Gs },
    { END_PLAY_NOTE, G }, { END_PLAY_NOTE, F  }, { END_PLAY_NOTE, D  },
    { END_PLAY_NOTE, F }, { END_NOTE,      0 }
  },
  {
    { PLAY_NOTE,    D }, { END_PLAY_NOTE, A  }, { END_PLAY_NOTE, Gs },
    { END_PLAY_NOTE, G }, { END_PLAY_NOTE, F  }, { END_PLAY_NOTE, D  },
    { END_PLAY_NOTE, F }, { END_PLAY_NOTE, G  },
    { END_PLAY_NOTE, D }, { END_PLAY_NOTE, A  }, { END_PLAY_NOTE, Gs },
    { END_PLAY_NOTE, G }, { END_PLAY_NOTE, F  }, { END_PLAY_NOTE, D  },
    { END_PLAY_NOTE, F }, { END_NOTE,      0 }
  },
  {
    { PLAY_NOTE,    D }, { END_PLAY_NOTE, A  }, { END_PLAY_NOTE, Gs },
    { END_PLAY_NOTE, G }, { END_PLAY_NOTE, F  }, { END_PLAY_NOTE, D  },
    { END_PLAY_NOTE, F }, { END_PLAY_NOTE, G  },
    { END_PLAY_NOTE, D }, { END_PLAY_NOTE, A  }, { END_PLAY_NOTE, Gs },
    { END_PLAY_NOTE, G }, { END_PLAY_NOTE, F  }, { END_PLAY_NOTE, D  },
    { END_PLAY_NOTE, F }, { END_NOTE,      0 }
  },
  {
    { PLAY_NOTE,    D }, { END_PLAY_NOTE, A  }, { END_PLAY_NOTE, Gs },
    { END_PLAY_NOTE, G }, { END_PLAY_NOTE, F  }, { END_PLAY_NOTE, D  },
    { END_PLAY_NOTE, F }, { END_PLAY_NOTE, G  },
    { END_PLAY_NOTE, D }, { END_PLAY_NOTE, A  }, { END_PLAY_NOTE, Gs },
    { END_PLAY_NOTE, G }, { END_PLAY_NOTE, F  }, { END_PLAY_NOTE, D  },
    { END_PLAY_NOTE, F }, { END_NOTE,      0 }
  },
  {
    { PLAY_NOTE,    D }, { END_PLAY_NOTE, A  }, { END_PLAY_NOTE, Gs },
    { END_PLAY_NOTE, G }, { END_PLAY_NOTE, F  }, { END_PLAY_NOTE, D  },
    { END_PLAY_NOTE, F }, { END_PLAY_NOTE, G  },
    { END_PLAY_NOTE, D }, { END_PLAY_NOTE, A  }, { END_PLAY_NOTE, Gs },
    { END_PLAY_NOTE, G }, { END_PLAY_NOTE, F  }, { END_PLAY_NOTE, D  },
    { END_PLAY_NOTE, F }, { END_NOTE,      0 }
  },

  // Bar 8 – identical
  {
    { PLAY_NOTE,    D }, { END_PLAY_NOTE, A  }, { END_PLAY_NOTE, Gs },
    { END_PLAY_NOTE, G }, { END_PLAY_NOTE, F  }, { END_PLAY_NOTE, D  },
    { END_PLAY_NOTE, F }, { END_PLAY_NOTE, G  },
    { END_PLAY_NOTE, D }, { END_PLAY_NOTE, A  }, { END_PLAY_NOTE, Gs },
    { END_PLAY_NOTE, G }, { END_PLAY_NOTE, F  }, { END_PLAY_NOTE, D  },
    { END_PLAY_NOTE, F }, { END_NOTE,      0 }
  },

  // Bar 9 – identical (louder section starts later; melody still loops)
  {
    { PLAY_NOTE,    D }, { END_PLAY_NOTE, A  }, { END_PLAY_NOTE, Gs },
    { END_PLAY_NOTE, G }, { END_PLAY_NOTE, F  }, { END_PLAY_NOTE, D  },
    { END_PLAY_NOTE, F }, { END_PLAY_NOTE, G  },
    { END_PLAY_NOTE, D }, { END_PLAY_NOTE, A  }, { END_PLAY_NOTE, Gs },
    { END_PLAY_NOTE, G }, { END_PLAY_NOTE, F  }, { END_PLAY_NOTE, D  },
    { END_PLAY_NOTE, F }, { END_NOTE,      0 }
  },

  // Bar 10 – identical
  {
    { PLAY_NOTE,    D }, { END_PLAY_NOTE, A  }, { END_PLAY_NOTE, Gs },
    { END_PLAY_NOTE, G }, { END_PLAY_NOTE, F  }, { END_PLAY_NOTE, D  },
    { END_PLAY_NOTE, F }, { END_PLAY_NOTE, G  },
    { END_PLAY_NOTE, D }, { END_PLAY_NOTE, A  }, { END_PLAY_NOTE, Gs },
    { END_PLAY_NOTE, G }, { END_PLAY_NOTE, F  }, { END_PLAY_NOTE, D  },
    { END_PLAY_NOTE, F }, { END_NOTE,      0 }
  },
  {
    { PLAY_NOTE,    D }, { END_PLAY_NOTE, A  }, { END_PLAY_NOTE, Gs },
    { END_PLAY_NOTE, G }, { END_PLAY_NOTE, F  }, { END_PLAY_NOTE, D  },
    { END_PLAY_NOTE, F }, { END_PLAY_NOTE, G  },
    { END_PLAY_NOTE, D }, { END_PLAY_NOTE, A  }, { END_PLAY_NOTE, Gs },
    { END_PLAY_NOTE, G }, { END_PLAY_NOTE, F  }, { END_PLAY_NOTE, D  },
    { END_PLAY_NOTE, F }, { END_NOTE,      0 }
  },
  {
    { PLAY_NOTE,    D }, { END_PLAY_NOTE, A  }, { END_PLAY_NOTE, Gs },
    { END_PLAY_NOTE, G }, { END_PLAY_NOTE, F  }, { END_PLAY_NOTE, D  },
    { END_PLAY_NOTE, F }, { END_PLAY_NOTE, G  },
    { END_PLAY_NOTE, D }, { END_PLAY_NOTE, A  }, { END_PLAY_NOTE, Gs },
    { END_PLAY_NOTE, G }, { END_PLAY_NOTE, F  }, { END_PLAY_NOTE, D  },
    { END_PLAY_NOTE, F }, { END_NOTE,      0 }
  }
};

chart nop_chart = {
  {
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0}
  },
  {
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0}
  },
  {
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0}
  },
  {
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0}
  },
  {
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0}
  },
  {
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0}
  },
  {
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0}
  },
  {
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0}
  },
  {
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0}
  },
  {
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0}
  },
  {
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0}
  },
  {
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0},
    {NOP,0},{NOP,0},{NOP,0},{NOP,0}
  }
};

chart bossa_main_melody = {
  {
    {PLAY_NOTE, D}, {NOP, 0}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, F}, {NOP, 0}, {END_PLAY_NOTE, 0}, {NOP, 0},
    {PLAY_NOTE, A}, {NOP, 0}, {END_NOTE, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, G}, {NOP, 0}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, B}, {NOP, 0}, {END_PLAY_NOTE, 0}, {NOP, 0},
    {PLAY_NOTE, D}, {NOP, 0}, {END_PLAY_NOTE, 0}, {NOP, 0},
    {PLAY_NOTE, F}, {NOP, 0}, {END_NOTE, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, C}, {NOP, 0}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, E}, {NOP, 0}, {END_PLAY_NOTE, 0}, {NOP, 0},
    {PLAY_NOTE, G}, {NOP, 0}, {END_PLAY_NOTE, 0}, {NOP, 0},
    {PLAY_NOTE, B}, {NOP, 0}, {END_NOTE, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, C}, {NOP, 0}, {END_PLAY_NOTE, 0}, {NOP, 0},
    {PLAY_NOTE, E}, {NOP, 0}, {END_PLAY_NOTE, 0}, {NOP, 0},
    {PLAY_NOTE, G}, {NOP, 0}, {END_NOTE, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, D}, {NOP, 0}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, F}, {NOP, 0}, {END_PLAY_NOTE, 0}, {NOP, 0},
    {PLAY_NOTE, A}, {NOP, 0}, {END_PLAY_NOTE, 0}, {NOP, 0},
    {PLAY_NOTE, C}, {NOP, 0}, {END_NOTE, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, G}, {NOP, 0}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, B}, {NOP, 0}, {END_PLAY_NOTE, 0}, {NOP, 0},
    {PLAY_NOTE, D}, {NOP, 0}, {END_PLAY_NOTE, 0}, {NOP, 0},
    {PLAY_NOTE, F}, {NOP, 0}, {END_NOTE, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, C}, {NOP, 0}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, E}, {NOP, 0}, {END_PLAY_NOTE, 0}, {NOP, 0},
    {PLAY_NOTE, G}, {NOP, 0}, {END_PLAY_NOTE, 0}, {NOP, 0},
    {PLAY_NOTE, B}, {NOP, 0}, {END_NOTE, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, C}, {NOP, 0}, {END_PLAY_NOTE, 0}, {NOP, 0},
    {PLAY_NOTE, E}, {NOP, 0}, {END_PLAY_NOTE, 0}, {NOP, 0},
    {PLAY_NOTE, G}, {NOP, 0}, {END_NOTE, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, D}, {NOP, 0}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, F}, {NOP, 0}, {END_PLAY_NOTE, 0}, {NOP, 0},
    {PLAY_NOTE, A}, {NOP, 0}, {END_PLAY_NOTE, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, G}, {NOP, 0}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, B}, {NOP, 0}, {END_PLAY_NOTE, 0}, {NOP, 0},
    {PLAY_NOTE, D}, {NOP, 0}, {END_PLAY_NOTE, 0}, {NOP, 0},
    {PLAY_NOTE, F}, {NOP, 0}, {END_NOTE, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, C}, {NOP, 0}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, E}, {NOP, 0}, {END_PLAY_NOTE, 0}, {NOP, 0},
    {PLAY_NOTE, G}, {NOP, 0}, {END_PLAY_NOTE, 0}, {NOP, 0},
    {PLAY_NOTE, B}, {NOP, 0}, {END_NOTE, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, C}, {NOP, 0}, {END_PLAY_NOTE, 0}, {NOP, 0},
    {PLAY_NOTE, E}, {NOP, 0}, {END_PLAY_NOTE, 0}, {NOP, 0},
    {PLAY_NOTE, G}, {NOP, 0}, {END_NOTE, 0}, {NOP, 0}
  }
};
chart waltz_piano = {
  {
    {PLAY_NOTE, C}, {NOP, 0}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {PLAY_NOTE, G}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {PLAY_NOTE, B}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {PLAY_NOTE, E}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {PLAY_NOTE, G}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, D}, {NOP, 0}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {PLAY_NOTE, C}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, G}, {NOP, 0}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {PLAY_NOTE, D}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {PLAY_NOTE, F}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, E}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {PLAY_NOTE, B}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {PLAY_NOTE, D}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {PLAY_NOTE, E}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {PLAY_NOTE, G}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, D}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {PLAY_NOTE, C}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, G}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {PLAY_NOTE, D}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {PLAY_NOTE, F}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, E}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {PLAY_NOTE, G}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {PLAY_NOTE, C}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, F}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {PLAY_NOTE, C}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, E}, {NOP, 0}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {PLAY_NOTE, G}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {PLAY_NOTE, B}, {NOP, 0}, {NOP, 0},
    {END_NOTE, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  
  // Bar 12: Am7 - resolution
  {
    {PLAY_NOTE, A}, {NOP, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {END_NOTE, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  }
};

chart bebop_blues_sax = {
  // Bar 1: F7 - with bebop scale
  {
    {PLAY_NOTE, F}, {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, A},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, C}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, Cs},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, D}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, Ds},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, F}, {END_NOTE, 0}, {NOP, 0}
  },
  
  // Bar 2: F7 - descending line
  {
    {PLAY_NOTE, A}, {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, Gs},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, G}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, Fs},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, F}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, Ds}, {END_NOTE, 0}, {NOP, 0}, {NOP, 0}
  },
  
  // Bar 3: F7 - chromatic approach
  {
    {PLAY_NOTE, D}, {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, Ds},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, E}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, F},
    {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, A}, {NOP, 0},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, C}, {END_NOTE, 0}, {NOP, 0}
  },
  
  // Bar 4: F7 - blues lick
  {
    {PLAY_NOTE, Ds}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, D}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, C}, {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, As},
    {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, A}, {NOP, 0},
    {END_NOTE, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  
  // Bar 5: Bb7
  {
    {PLAY_NOTE, As}, {NOP, 0}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, D}, {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, F},
    {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, G}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, Gs}, {END_NOTE, 0}, {NOP, 0}, {NOP, 0}
  },
  
  // Bar 6: Bb7 - altered scale
  {
    {PLAY_NOTE, A}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, As}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, B}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, C}, {NOP, 0},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, Cs}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, D},
    {END_NOTE, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  
  // Bar 7: F7
  {
    {PLAY_NOTE, F}, {NOP, 0}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, C}, {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, A},
    {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, F}, {NOP, 0},
    {END_NOTE, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  
  // Bar 8: F7 - turnaround prep
  {
    {PLAY_NOTE, Ds}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, D}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, C}, {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, B},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, As}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, A},
    {END_NOTE, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  
  // Bar 9: C7
  {
    {PLAY_NOTE, C}, {NOP, 0}, {NOP, 0}, {END_PLAY_NOTE, 0},
    {PLAY_NOTE, E}, {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, G},
    {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, As}, {NOP, 0},
    {END_NOTE, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  
  // Bar 10: Bb7
  {
    {PLAY_NOTE, As}, {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, D},
    {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, F}, {NOP, 0},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, Gs}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  
  // Bar 11: F7
  {
    {PLAY_NOTE, F}, {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, A},
    {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, C}, {NOP, 0},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, Ds}, {NOP, 0}, {END_NOTE, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  
  // Bar 12: C7 - turnaround
  {
    {PLAY_NOTE, C}, {NOP, 0}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, As},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, A}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, Gs},
    {END_PLAY_NOTE, 0}, {PLAY_NOTE, G}, {END_PLAY_NOTE, 0}, {PLAY_NOTE, F},
    {END_NOTE, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  }
};


chart bossa_support_chords = {
  // Bar 1: Dm7 (D & F) 
  {
    {PLAY_NOTE, D}, {END_NOTE, 0}, {NOP, 0}, {NOP, 0},
    {PLAY_NOTE, F}, {END_NOTE, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },

  // Bar 2: G7 (G & B)
  {
    {PLAY_NOTE, G}, {END_NOTE, 0}, {NOP, 0}, {NOP, 0},
    {PLAY_NOTE, B}, {END_NOTE, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },

  // Bar 3: Cmaj7 (C & E)
  {
    {PLAY_NOTE, C}, {END_NOTE, 0}, {NOP, 0}, {NOP, 0},
    {PLAY_NOTE, E}, {END_NOTE, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },

  // Bar 4: Am7 (A & C)
  {
    {PLAY_NOTE, A}, {END_NOTE, 0}, {NOP, 0}, {NOP, 0},
    {PLAY_NOTE, C}, {END_NOTE, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },

  // Bars 5–8: repeat of bars 1–4
  {
    {PLAY_NOTE, D}, {END_NOTE, 0}, {NOP, 0}, {NOP, 0},
    {PLAY_NOTE, F}, {END_NOTE, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, G}, {END_NOTE, 0}, {NOP, 0}, {NOP, 0},
    {PLAY_NOTE, B}, {END_NOTE, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, C}, {END_NOTE, 0}, {NOP, 0}, {NOP, 0},
    {PLAY_NOTE, E}, {END_NOTE, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  },
  {
    {PLAY_NOTE, A}, {END_NOTE, 0}, {NOP, 0}, {NOP, 0},
    {PLAY_NOTE, C}, {END_NOTE, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0},
    {NOP, 0}, {NOP, 0}, {NOP, 0}, {NOP, 0}
  }
};


//Initalize the Rates for the Sax ADSR
void Sax_Env_Init(void){
    sax_atk_rate  = 1.0f / SAX_ATTACK_TIME;                 
    sax_decay_rate   = (1.0f - SAX_SUSTAIN_LEVEL) / SAX_DECAY_TIME;
    sax_release_rate =  SAX_SUSTAIN_LEVEL / SAX_RELEASE_TIME;
    sax_env_lvl    = 0.0f;
    sax_env_state    = IDLE;
}

//Initalize the Rates for the Piano ADSR
void Piano_Env_Init(void){
    piano_atk_rate  = 1.0f / PIANO_ATTACK_TIME;                 
    piano_decay_rate   = (1.0f - PIANO_SUSTAIN_LEVEL) / PIANO_DECAY_TIME;
    piano_release_rate =  PIANO_SUSTAIN_LEVEL / PIANO_RELEASE_TIME;
    piano_env_lvl    = 0.0f;
    piano_env_state    = IDLE;
}

//Make note play when button pressed
void Envelope_Pressed(uint8_t *env_state){
    *env_state=ATTACK;
}
void Envelope_Release(uint8_t *env_state){
  if(*env_state!=IDLE){
    *env_state= RELEASE;
  }
}

// Calculate the Phase Shift needed to get a certain frequency
void calc_phase(float note_freq,uint32_t *phase_inc,int sample_rate){
  //HAVE TO CAST HERE
   double shifted = (double)(note_freq)*(1ULL << PHASE_BITS);
    *phase_inc=(uint32_t) (shifted/sample_rate);
   }

float Envelope_Process(float isr_freq,uint8_t *env_state,
                              float *env_lvl,float atk_rate,float decay_rate,float sustain_level,
                              float release_rate)
{
    //figure time passed since last tick
    float dt       = 1.0f / isr_freq; //time passed which is pretty versitle imo
    switch (*env_state) {
        case IDLE:
            *env_lvl=0;
            break;
        case ATTACK:
            *env_lvl += atk_rate * dt;
            if (*env_lvl >= 1.0f) {
               //we hit that peak
                *env_lvl = 1.0f;
                *env_state = DECAY;
            }
            break;
        case DECAY:
            *env_lvl -= decay_rate * dt;
            if (*env_lvl <= sustain_level) {
                  *env_lvl = sustain_level;
                  *env_state = SUSTAIN;
            }
            break;
        case SUSTAIN:
            // hold
            break;
        case RELEASE:
            *env_lvl -= release_rate * dt;
            if (*env_lvl <= 0.0f) {
                *env_lvl = 0.0f;
                *env_state = IDLE;
            }
            break;
    }
    //if we wanna actually edit the global variable need to pass in address then derefrence as we use it
    return *env_lvl;
}
//get Sax_Note_Func
void play_sax(uint8_t note){
    //modular short hand for palying sax notes
   sax_current_note_freq=SaxFreq[note];
   calc_phase(SaxFreq[note],&sax_phase_inc,SAX_SAMPLE_RATE); 
   Envelope_Pressed(&sax_env_state);
}
//get Piano note_func
void play_piano(uint8_t note){
    //modular short hand for palying piano notes
   piano_current_note_freq=PianoFreq[note];
   calc_phase(PianoFreq[note],&piano_phase_inc,PIANO_SAMPLE_RATE); 
   Envelope_Pressed(&piano_env_state);
}

uint16_t clean_pot(uint16_t pot_val,uint16_t buckets){
    //buckets pot value into however many "buckets"
  //this was kind of a stroke of genius.
    uint16_t clean_pot_val=pot_val;
    if(pot_val & 0x8000)clean_pot_val=0;
    else if(pot_val>=9000)clean_pot_val=0xfff;
    
    //bucket time
    float f = (float)clean_pot_val/(float)4141;
    int b=(int)(f*buckets);
    return b<buckets ? b:buckets-1;
}
//Sax ISR
CY_ISR(Sax_ISR)
{
    //using phase accumulator(assuming isr is hooked indo sample rate)
    sax_phase=(sax_phase_inc+sax_phase) & ((1<<PHASE_BITS)-1);
    uint8_t new_idx= ((uint64_t) sax_phase << 6)>>PHASE_BITS;

    //using old method keeping for debugging /potenital reverts 
    // uint8 idx = 0;
    // if (idx >= SAXTABLE_SIZE) idx = 0;


    //feed dac
    sax_sample = SaxTable[new_idx];
    //call enevelope proccess for sax
    sax_env_out = Envelope_Process(SAX_SAMPLE_RATE,&sax_env_state,&sax_env_lvl,sax_atk_rate,sax_decay_rate,SAX_SUSTAIN_LEVEL,sax_release_rate);
    
    //shift the value
    int16_t zero_centered=(int16_t)sax_sample-128;
    float scaled=(float)zero_centered*sax_env_out;
    sax_enveloped_note=(uint8_t)(scaled+128);

    //set the value
    SaxDac_SetValue(sax_enveloped_note);
}
uint32_t msTicks;

CY_ISR(Tick_ISR){
    msTicks++;
    testy_Write(~testy_Read());
}
CY_ISR(Piano_ISR)
{
    //grab sample from table
    //using phase accumulator(assuming isr is hooked indo sample rate)
    piano_phase=(piano_phase_inc+piano_phase) & ((1<<PHASE_BITS)-1);
    uint8_t new_idx= ((uint64_t) piano_phase << 6)>>PHASE_BITS;

    //using old method 
    // uint8 idx = 0;
    // if (idx >= SAXTABLE_SIZE) idx = 0;

    // feed dac
    piano_sample = PianoTable[new_idx];
    //call enevelope proccess for sax
    piano_env_out = Envelope_Process(PIANO_SAMPLE_RATE,&piano_env_state,&piano_env_lvl,piano_atk_rate,piano_decay_rate,SAX_SUSTAIN_LEVEL,piano_release_rate);
    
    //do that shifting cuzzo
    int16_t zero_centered=(int16_t)piano_sample-128;
    float scaled=(float)zero_centered*piano_env_out;
    piano_enveloped_note=(uint8_t)(scaled+128);
    //set the value
    PianoDac_SetValue(piano_enveloped_note);
                          
}




int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    CySysTickStart();
    CySysTickSetCallback(0,Tick_ISR);

    //init 
    Sax_Env_Init();
    Piano_Env_Init();
    LCD_Char_1_Start();
	  LCD_Char_1_ClearDisplay();
	  LCD_Char_1_PrintString("ADC : ");  

    sax_isr_StartEx(Sax_ISR);
    piano_isr_StartEx(Piano_ISR);
    SaxDac_Start(); 
    PianoDac_Start(); 
    PITCH_ADC_Start();
    PITCH_ADC_StartConvert();
    BPM_ADC_Start();
    BPM_ADC_StartConvert();
    Chart_ADC_Start();
    Chart_ADC_StartConvert();    
    
 

   
    //maybe this wasn't the place to declare all those variables but whatever
    uint8_t chart_idx=0;
    uint8_t bar_idx=0;
    uint32_t LastTick= msTicks;
    uint32_t msPerTick=125;
    uint32_t LEDTick;
    uint8_t cur_inst;
    uint8_t chart_sel;
    struct bar_note sax_cur_note;
    struct bar_note piano_cur_note;
    uint8_t prev_chart_sel;

    for(;;)
    {
        
        mode=Mode_Switch_Read();
        cur_inst=Inst_Switch_Read();
        chart_sel=clean_pot(Chart_ADC_GetResult16(),5);
        if(chart_sel!=prev_chart_sel){
            Envelope_Release(&sax_env_state);
            Envelope_Release(&piano_env_state);
        }
        switch(Mode_Switch_Read()){
        //playing mode
            case 0:
                    Out_1_Write(0);
                    if(cur_inst==1){
                        if(Btn_2_Read()==0){
                            //play the note when the user clicks the button 
                            play_sax(clean_pot(BPM_ADC_GetResult16(),12));
                        }else if(sax_env_state!=RELEASE){
                            //need to release both cause of edge cases
                            Envelope_Release(&sax_env_state);
                            Envelope_Release(&piano_env_state);
                        }
                    }else{
                        if(Btn_2_Read()==0){
                            play_piano(clean_pot(BPM_ADC_GetResult16(),12));
                        }else if(piano_env_state!=RELEASE){
                            //excact same as saxophone
                            Envelope_Release(&piano_env_state);
                            Envelope_Release(&sax_env_state);
                         }
                        
                    }
                break;
            //auto mode
            case 1:
            //update state vars
            //only execute code when its ben 1/16th note worth of time
              if(msTicks-LastTick>=msPerTick){
                
    
                LastTick=msTicks;
                pot1_value=clean_pot(BPM_ADC_GetResult16(),7);
                pot2_value=clean_pot(PITCH_ADC_GetResult16(),12);
                //update our BPM, which is updating the time we weight
                msPerTick=(60000/bpm_array[pot1_value])/4;
                
      
                if(bar_idx>=16){
                    bar_idx=0;
                    chart_idx=(chart_idx+1)%12;
                }
                
              //changing songs we play, could've been a clean function, but i had a deadline so we're doing a long switch statment
              switch(chart_sel){
                case 0:
                    sax_cur_note=good_megalovania[chart_idx][bar_idx];
                    piano_cur_note=nop_chart[chart_idx][bar_idx];
                    break;
                case 1:
                    sax_cur_note=bad_megalovania[chart_idx][bar_idx];
                    piano_cur_note=nop_chart[chart_idx][bar_idx];
                    break;
                case 2:
                    sax_cur_note=bossa_main_melody[chart_idx][bar_idx];
                    piano_cur_note=nop_chart[chart_idx][bar_idx];
                    break;
                case 3:
                    sax_cur_note=nop_chart[chart_idx][bar_idx];
                    piano_cur_note=waltz_piano[chart_idx][bar_idx];
                    break;
                case 4:
                    sax_cur_note=bebop_blues_sax[chart_idx][bar_idx];
                    piano_cur_note=nop_chart[chart_idx][bar_idx];
                    break;
                
              }
              //now mux based off our "NOTE Action"
                sax_cur_note.note=(sax_cur_note.note+pot2_value)%12;
                //mux based of the note actio 
                switch(sax_cur_note.action){
                    case PLAY_NOTE:
                        play_sax(sax_cur_note.note);
                      break;
                    case END_PLAY_NOTE:
                        SAX_RELEASE_TIME=.01;
                        Envelope_Release(&sax_env_state);
                        SAX_RELEASE_TIME=.3;
                        play_sax(sax_cur_note.note);
                      break;
                    case END_NOTE:
                        Envelope_Release(&sax_env_state);
                      break;
                    case NOP:
                      break;
                }
              
              piano_cur_note.note=(piano_cur_note.note+pot2_value)%12;
                //mux based of the note actio 
                switch(piano_cur_note.action){
                    case PLAY_NOTE:
                        play_piano(piano_cur_note.note);
                    //    play_piano(cur_note. note);
                      break;
                    case END_PLAY_NOTE:
                        PIANO_RELEASE_TIME=.01;
                        Envelope_Release(&piano_env_state);
                        PIANO_RELEASE_TIME=.3;
                        play_piano(piano_cur_note.note);
                      break;
                    case END_NOTE:
                        Envelope_Release(&piano_env_state);
                      break;
                    case NOP:
                      break;
                }
                bar_idx++;
                break;
              }
          }
        prev_chart_sel=chart_sel;
        if(msTicks-LEDTick>=350){
            LEDTick=msTicks;
            
            LCD_Char_1_ClearDisplay();
            
            LCD_Char_1_Position(0,14);
            LCD_Char_1_PrintNumber(chart_sel);
          
            if(mode==1){
            LCD_Char_1_Position(0,0);
            LCD_Char_1_PrintString("BPM : ");
            LCD_Char_1_Position(0,6);
            LCD_Char_1_PrintNumber(bpm_array[pot1_value]);
            
            LCD_Char_1_Position(1,0);
            LCD_Char_1_PrintString("Tran: ");
            LCD_Char_1_Position(1,6);
            LCD_Char_1_PrintNumber(pot2_value);
            LCD_Char_1_Position(1,6);
            }
            if(mode==0){
                LCD_Char_1_Position(0,0);
                LCD_Char_1_PrintString("Note:");
                uint8_t note_idx=clean_pot(BPM_ADC_GetResult16(),12);
                LCD_Char_1_PrintString(NoteNames[note_idx]);
            }
            LCD_Char_1_DrawVerticalBG(1,10,3,(sax_env_lvl*17));
            LCD_Char_1_DrawVerticalBG(1,11,3,(piano_env_lvl*14));
        }
        
    }
}

/* [] END OF FILE */
