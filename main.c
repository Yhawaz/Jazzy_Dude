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
#define SAMPLE_RATE 24000.0
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

const float SaxFreq [12]={196, 207.66, 220.01, 233.09, 246.95, 261.63, 277.19, 293.67, 311.13, 329.63, 349.23, 370};
//Global Vals(For Debugging Via LCD ISR)
//
    //global
    uint8_t new_idx;
    //sax vars's
    float sax_current_note_freq=196;
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

void Envelope_Pressed(uint8_t *env_state){
    *env_state=ATTACK;
   // led_Write(~led_Read());
}
void Envelope_Release(uint8_t *env_state){
  if(*env_state!=IDLE){
    *env_state= RELEASE;
  }
}

void calc_phase(float note_freq,uint32_t *phase_inc){
    *phase_inc=(uint32_t) ((note_freq)*(1 << PHASE_BITS))/SAMPLE_RATE;
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
   calc_phase(SaxFreq[note],inst_phase_inc); 
   //calculate time to play note
   float msPerQuarter=60000/(float)bpm;
   time_scale=4.0/(float)note_type;
   duration=msPerQuarter*time_scale;
   //dt=(uint32_t)duration+.5;
   dt=1;
   //use that calculation to find the sacles
   SAX_ATTACK_TIME=((.12f*dt>90.0)? 90.0:.12f*dt)/1000;
   //SAX_RELEASE_TIME=(dt<250.0)? 0:(.05*dt)/1000;
   SAX_RELEASE_TIME=0;
   SAX_DECAY_TIME= ((dt<180) ? .07f*dt: (.2*dt>110)? 110:.2*dt)/1000;
   
   //calculate time it takes to play note
    
   testy_Write(~testy_Read());
   Envelope_Pressed(env_state);
   CyDelay(dt);
   testy_Write(~testy_Read());
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
    led_Write(1);
    sax_env_out = Envelope_Process(SAMPLE_RATE,&sax_env_state,&sax_env_lvl,sax_atk_rate,sax_decay_rate,SAX_SUSTAIN_LEVEL,sax_release_rate);
    
    led_Write(0);
    int16_t zero_centered=(int16_t)sax_sample-128;
    float scaled=(float)zero_centered*sax_env_out;
    sax_enveloped_note=(uint8_t)(scaled+128);
    //set the value
    SaxDac_SetValue(sax_enveloped_note);

    
    
    
}

CY_ISR(Lcd_ISR){
    LCD_Char_1_Position(0, 6);
    LCD_Char_1_PrintNumber(dt);
 
}
int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    //init 
    Sax_Env_Init();
    LCD_Char_1_Start();					// initialize lcd
	  LCD_Char_1_ClearDisplay();
	  LCD_Char_1_PrintString("ADC : ");  
    sax_isr_StartEx(Sax_ISR);
    lcd_isr_StartEx(Lcd_ISR);
   // sax_clk_SetDivider(413);
    //Initliaze Envelope_Process;
    //Start Sax Dac
    SaxDac_Start(); 
    SaxDac_SetValue(255);
    for(;;)
    {
       play_sax(16,120,5,&sax_current_note_freq,&sax_phase_inc,&sax_env_state);
       testy_Write(~testy_Read());
       CyDelay(100);
       play_sax(16,120,6,&sax_current_note_freq,&sax_phase_inc,&sax_env_state);
       testy_Write(~testy_Read());
       CyDelay(100);
       play_sax(4,120,7,&sax_current_note_freq,&sax_phase_inc,&sax_env_state);
       testy_Write(~testy_Read());
       CyDelay(100);
       //play_sax(4,120,1,&sax_current_note_freq,&sax_phase_inc,&sax_env_state);
       //play_sax(4,120,2,&sax_current_note_freq,&sax_phase_inc,&sax_env_state);
       //play_sax(4,120,3,&sax_current_note_freq,&sax_phase_inc,&sax_env_state);
       /*
       // sax_clk_SetDivider(3000000/(SaxFreq[0]*32));
        sax_current_note_freq=SaxFreq[0];
        calc_phase(sax_current_note_freq,&sax_phase_inc);
        Envelope_Pressed(&sax_env_state);
        CyDelay(600);
        Envelope_Release(&sax_env_state);
        CyDelay(100);
        //sax_clk_SetDivider(3000000/(SaxFreq[4]*32));
        sax_current_note_freq=SaxFreq[4];
        calc_phase(sax_current_note_freq,&sax_phase_inc);
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
