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

//Awesome Tables

#define SAXTABLE_SIZE 34u

static const uint8 SaxTable[SAXTABLE_SIZE] = {
    128, 151, 174, 195, 213, 229, 242, 250,
    254, 254, 250, 242, 229, 213, 195, 174,
    151, 128, 104,  81,  60,  42,  26,  13,
      5,   1,   1,   5,  13,  26,  42,  60,
     81, 104
};

static const float SaxFreq [12]={196, 207.66, 220.01, 233.09, 246.95, 261.63, 277.19, 293.67, 311.13, 329.63, 349.23, 370};

//Globals

static float current_note_freq=440.0;
static float atk_rate,decay_rate,release_rate;
static float env_lvl;
static uint8_t env_state=0;

//Envelope Vars;
enum {
    ENV_IDLE,
    ENV_ATTACK,
    ENV_DECAY,
    ENV_SUSTAIN,
    ENV_RELEASE
};

static float ATTACK_TIME;
static float SUSTAIN_LEVEL;
static float RELEASE_TIME;
static float DECAY_TIME;
//

void Envelope_Init(void){
    attack_rate  = 1.0f / ATTACK_TIME;                 
    decay_rate   = (1.0f - SUSTAIN_LEVEL) / DECAY_TIME;
    release_rate =  SUSTAIN_LEVEL        / RELEASE_TIME;
    env_level    = 0.0f;
    env_state    = ENV_IDLE;
}

void Envelope_Pressed(){
      env_state=ENV_ATTACK;
}
void Envelope_Release(){
  if(env_state!=IDLE){
    env_state=ENV_RELEASE
  }
}

static float Envelope_Process(void)
{
    // compute time per tick (seconds)
    float isr_freq = current_note_freq * SAXTABLE_SIZE;
    float dt       = 1.0f / isr_freq; //time passed which is pretty versitle imo

    switch (env_state) {
        case ENV_IDLE:
            return 0.0f;
        case ENV_ATTACK:
            env_level += attack_rate * dt;
            if (env_level >= 1.0f) {
               //we hit that peak
                env_level = 1.0f;
                env_state = ENV_DECAY;
            }
            break;
        case ENV_DECAY:
            env_level -= decay_rate * dt;
            if (env_level <= SUSTAIN_LEVEL) {
                  env_level = SUSTAIN_LEVEL;
                  env_state = ENV_SUSTAIN;
            }
            break;
        case ENV_SUSTAIN:
            // hold
            break;
        case ENV_RELEASE:
            env_level -= release_rate * dt;
            if (env_level <= 0.0f) {
                env_level = 0.0f;
                env_state = ENV_IDLE;
            }
            break;
    }
    return env_level;
}

//Sax ISR
CY_ISR(Sax_ISR)
{
    static uint8 idx = 0;
    uint8 sample = SaxTable[idx++];
    if (idx >= SAXTABLE_SIZE) idx = 0;
    //pretty easy to debug tbh
    float env_out=Envelope_Process();

    //awes*ome bug needs to be int16not uint cause we need -128
    int16_t zero_centered=(int16_t)sample-128;

    int16_t scaled=(int16_t)zreo_centered*env_out;

    uint8_t enveloped_noted=(uint8_t)(scaled+128);
    //Adsr Code Goes Here we can cur from here
    

    SaxDac_SetValue(sample);

    //CyDelay(1);
    //sax_isr_ClearPending();

}
int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
 
    //Initalize ISR For Sax
    sax_isr_StartEx(Sax_ISR);
    //Initliaze Envelope_Process;
    //Start Sax Dac
    SaxDac_Start(); 
    SaxDac_SetValue(255);

    
    //Sax_Timer_Start();
    for(;;)
    {

    }
}

/* [] END OF FILE */
