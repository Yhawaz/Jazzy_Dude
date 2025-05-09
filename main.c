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

//Parameters
#define SAXTABLE_SIZE 64u
#define PIANOTABLE_SIZE 256u
#define SAX_SAMPLE_RATE 24000.0
#define PIANO_SAMPLE_RATE 6000.0
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

const uint8 PianoTable[PIANOTABLE_SIZE] = {        114, 115, 115, 112, 110, 108, 107, 106, 106, 104, 103, 101,
    99, 97, 96, 94, 92, 90, 88, 87, 87, 87, 87, 87,
    87, 89, 90, 91, 92, 93, 95, 97, 100, 104, 108, 112,
    117, 121, 126, 129, 133, 136, 139, 141, 142,142, 143, 143, 143,
    142, 141, 140, 139, 138, 136, 134, 132, 130, 127, 126, 124,
    122, 120, 117, 114, 111, 108, 106, 104, 104, 104, 105, 107,
    110, 114, 119, 125, 133, 142, 151, 159, 168, 175, 181, 186,
    190, 193, 194, 195, 194, 193, 191, 189, 188, 187, 186, 186,
    187, 188, 189, 191, 193, 195, 197, 199, 201, 203, 205, 207,
    209, 212, 216, 219, 223, 226, 230, 233, 236, 240, 243, 246,
    249, 250, 252, 253, 253, 254, 254, 255, 255, 255, 254, 253,
    251, 249, 246, 243, 240, 236, 232, 228, 223, 218, 212, 206,
    200, 193, 187, 180, 174, 168, 161, 155, 149, 144, 138, 133,
    128, 123, 119, 114, 110, 106, 102, 99, 96, 94, 91, 89,
    86, 83, 79, 75, 70, 65, 59, 53, 48, 43, 40, 36,
    34, 32, 31, 30, 29, 29, 28, 29, 29, 30, 32, 34,
    37, 39, 42, 45, 47, 49, 51, 53, 56, 59, 62, 65,
    67, 69, 70, 70, 70, 71, 71, 71, 71, 71, 72, 72,
    72, 72, 73, 73, 73, 73, 72, 72, 71, 71, 72, 74,
    77, 79, 82, 85, 87, 90, 94, 98, 102, 105, 109, 112,
    114, 115, 115, 115, 115, 114, 114, 113, 111, 108, 105, 104,
    104, 107, 111
};

const float SaxFreq [12]={196, 207.66, 220.01, 233.09, 246.95, 261.63, 277.19, 293.67, 311.13, 329.63, 349.23, 370};
const float PianoFreq [12]={196, 207.66, 220.01, 233.09, 246.95, 261.63, 277.19, 293.67, 311.13, 329.63, 349.23, 370};
//;const float PianoFreq [12]={350,350,350,350,350,350,350,350,350,350,350,350};
//Global Vals(For Debugging Via LCD ISR)
//
    //global
    uint8_t new_idx;
    //sax vars's
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

    float piano_current_note_freq=196;
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

    float time_scale;
    float duration;
    uint32_t dt;
 //Envelope Vars;
    enum {
        IDLE,
        ATTACK,
        DECAY,
        SUSTAIN,
        RELEASE
    };

    struct env_return {
        float env_lvl;
        uint8_t env_state;
    };

//
void Sax_Env_Init(void){
    sax_atk_rate  = 1.0f / SAX_ATTACK_TIME;                 
    sax_decay_rate   = (1.0f - SAX_SUSTAIN_LEVEL) / SAX_DECAY_TIME;
    sax_release_rate =  SAX_SUSTAIN_LEVEL / SAX_RELEASE_TIME;
    sax_env_lvl    = 0.0f;
    sax_env_state    = IDLE;
}

void Piano_Env_Init(void){
    piano_atk_rate  = 1.0f / PIANO_ATTACK_TIME;                 
    piano_decay_rate   = (1.0f - PIANO_SUSTAIN_LEVEL) / PIANO_DECAY_TIME;
    piano_release_rate =  PIANO_SUSTAIN_LEVEL / PIANO_RELEASE_TIME;
    piano_env_lvl    = 0.0f;
    piano_env_state    = IDLE;
}

void Envelope_Pressed(uint8_t *env_state){
    *env_state=ATTACK;
   // led_Write(~led_Read());
}
void Envelope_Release(uint8_t *env_state){
  if(*env_state!=IDLE){
    *env_state= RELEASE;
  }
}

void calc_phase(float note_freq,uint32_t *phase_inc,int sample_rate){
    
   double shifted = (double)(note_freq)*(1ULL << PHASE_BITS);
    *phase_inc=(uint32_t) (shifted/sample_rate);
   }

float Envelope_Process(float isr_freq,uint8_t *env_state,
                              float *env_lvl,float atk_rate,float decay_rate,float sustain_level,
                              float release_rate)
{
    // compute time per tick (seconds)
    led1_Write(~led1_Read());
   // float isr_freq = current_note_freq * tbl_size;
    float dt       = 1.0f / isr_freq; //time passed which is pretty versitle imo
    switch (*env_state) {
        case IDLE:
            *env_lvl=0;
            break;
           // return 0.0f;
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
    return *env_lvl;
}
//get Sax_Note_Func
void play_sax(uint8_t note_type, uint8_t bpm, uint8_t note, float *inst_freq, uint32_t *inst_phase_inc,uint8_t *env_state){
    
   //calculate note frequency and [hase
   *inst_freq=SaxFreq[note];
   calc_phase(SaxFreq[note],inst_phase_inc,SAX_SAMPLE_RATE); 
   //calculate time to play note
   float msPerQuarter=60000/(float)bpm;
   time_scale=4.0/(float)note_type;
   duration=msPerQuarter*time_scale;
   //dt=(uint32_t)duration+.5;
   dt=100;
   //use that calculation to find the sacles
   SAX_ATTACK_TIME=((.12f*dt>90.0)? 90.0:.12f*dt)/1000;
   //SAX_RELEASE_TIME=(dt<250.0)? 0:(.05*dt)/1000;
   SAX_RELEASE_TIME=0;
   SAX_DECAY_TIME= ((dt<180) ? .07f*dt: (.2*dt>110)? 110:.2*dt)/1000;
   
   //calculate time it takes to play note
    
 //  testy_Write(~testy_Read());

   Envelope_Pressed(env_state);
  testy_Write(~testy_Read());
   CyDelay(dt);
   testy_Write(~testy_Read());
 //  testy_Write(~testy_Read());
   Envelope_Release(env_state);
   CyDelay(SAX_RELEASE_TIME);
}
//Sax ISR

CY_ISR(Sax_ISR)
{
    //grab sample from table
    //using phase accumulator(assuming isr is hooked indo sample rate)
    
    sax_phase=(sax_phase_inc+sax_phase) & ((1<<PHASE_BITS)-1);
    new_idx= ((uint64_t) sax_phase << 6)>>PHASE_BITS;
    //using old method 
    // uint8 idx = 0;
    // if (idx >= SAXTABLE_SIZE) idx = 0;
    //feed dac
    sax_sample = SaxTable[new_idx];
    //call enevelope proccess for sax
    uint8_t meow;
    float rawr;
   
    sax_env_out = Envelope_Process(SAX_SAMPLE_RATE,&sax_env_state,&sax_env_lvl,sax_atk_rate,sax_decay_rate,SAX_SUSTAIN_LEVEL,sax_release_rate);
    
    
    int16_t zero_centered=(int16_t)sax_sample-128;
    float scaled=(float)zero_centered*sax_env_out;
    sax_enveloped_note=(uint8_t)(scaled+128);
    //set the value
    SaxDac_SetValue(sax_enveloped_note);
    
}

CY_ISR(Piano_ISR)
{
    //grab sample from table
    //using phase accumulator(assuming isr is hooked indo sample rate)
    
    piano_phase=(piano_phase_inc+piano_phase) & ((1<<PHASE_BITS)-1);
    new_idx= ((uint64_t) piano_phase << 8)>>PHASE_BITS;
    //using old method 
    // uint8 idx = 0;
    // if (idx >= SAXTABLE_SIZE) idx = 0;
    // feed dac
    piano_sample = PianoTable[new_idx];
    //call enevelope proccess for sax
    // led_Write(1);
    piano_env_out = Envelope_Process(PIANO_SAMPLE_RATE,&piano_env_state,&piano_env_lvl,piano_atk_rate,piano_decay_rate,SAX_SUSTAIN_LEVEL,piano_release_rate);
    
    led_Write(0);
    int16_t zero_centered=(int16_t)piano_sample-128;
    float scaled=(float)zero_centered*piano_env_out;
    piano_enveloped_note=(uint8_t)(scaled+128);
    //set the value
    PianoDac_SetValue(piano_enveloped_note);
    
}

CY_ISR(Lcd_ISR){
    LCD_Char_1_Position(0, 6);
    LCD_Char_1_PrintNumber(sax_phase_inc);
 
}
int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    //init 
    Sax_Env_Init();
    Piano_Env_Init();
    LCD_Char_1_Start();					// initialize lcd
	LCD_Char_1_ClearDisplay();
	LCD_Char_1_PrintString("ADC : ");  
    sax_isr_StartEx(Sax_ISR);
    piano_isr_StartEx(Piano_ISR);
    lcd_isr_StartEx(Lcd_ISR);
   // sax_clk_SetDivider(413);
    //Initliaze Envelope_Process;
    //Start Sax Dac
    SaxDac_Start(); 
    PianoDac_Start(); 
    // SaxDac_SetValue(255);
    for(;;)
    {
        static int x=0;
        if(x>11) x=0;
   //     /*
       sax_current_note_freq=SaxFreq[x++];
        calc_phase(400,&sax_phase_inc,SAX_SAMPLE_RATE);
       play_sax(16,120,x,&sax_current_note_freq,&sax_phase_inc,&sax_env_state);
        CyDelay(100);
    //    */
    /*
      piano_current_note_freq=PianoFreq[x++];
      calc_phase(piano_current_note_freq,&piano_phase_inc,PIANO_SAMPLE_RATE);
      Envelope_Pressed(&piano_env_state);
       CyDelay(75);
       Envelope_Release(&piano_env_state);
        CyDelay(50);
       */
    
        
       
       // testy_Write(~testy_Read());
       // CyDelay(100);
       // play_sax(16,120,6,&sax_current_note_freq,&sax_phase_inc,&sax_env_state);
       // testy_Write(~testy_Read());
       // CyDelay(100);
       // play_sax(4,120,7,&sax_current_note_freq,&sax_phase_inc,&sax_env_state);
       // testy_Write(~testy_Read());
       // CyDelay(100);
       //play_sax(4,120,1,&sax_current_note_freq,&sax_phase_inc,&sax_env_state);
       //play_sax(4,120,2,&sax_current_note_freq,&sax_phase_inc,&sax_env_state);
       //play_sax(4,120,3,&sax_current_note_freq,&sax_phase_inc,&sax_env_state);
       /*
       // sax_clk_SetDivider(3000000/(SaxFreq[0]*32));
       
        calc_phase(sax_current_note_freq,&sax_phase_inc);
        Envelope_Pressed(&sax_env_state);
        CyDelay(600);
        Envelope_Release(&sax_env_state);
        CyDelay(100);
        //sax_clk_SetDivider(3000000/(SaxFreq[4]*32));
        sax_current_note_freq=SaxFreq[4];
       
        Envelope_Pressed(&sax_env_state);
        CyDelay(50);
        Envelope_Release(&sax_env_state);
        CyDelay(100);
        sax_current_note_freq=SaxFreq[5];
        calc_phase(sax_current_note_freq,&sax_phase_inc);
        Envelope_Pressed(&sax_env_state);
        CyDelay(100);
        Envelope_Release(&sax_env_state);
        CyDelay(100);
                sax_current_note_freq=SaxFreq[6];
        calc_phase(sax_current_note_freq,&sax_phase_inc);
        Envelope_Pressed(&sax_env_state);
        CyDelay(50);
        Envelope_Release(&sax_env_state);
        CyDelay(100);
                sax_current_note_freq=SaxFreq[7];
        calc_phase(sax_current_note_freq,&sax_phase_inc);
        Envelope_Pressed(&sax_env_state);
        CyDelay(10);
        Envelope_Release(&sax_env_state);
        CyDelay(100);
        
      //  sax_clk_SetDivider(3000000/(SaxFreq[2]*32));
        sax_current_note_freq=SaxFreq[8];
        calc_phase(sax_current_note_freq,&sax_phase_inc);
        Envelope_Pressed(&sax_env_state);
        CyDelay(100);
        Envelope_Release(&sax_env_state);
        CyDelay(20);
*/
        
    }
}

/* [] END OF FILE */
