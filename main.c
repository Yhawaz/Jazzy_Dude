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

#define SAXTABLE_SIZE 37u

static const uint8 SaxTable[SAXTABLE_SIZE] = {
 139, 166, 193, 213, 230, 210, 148, 125,
    135, 124,  83,  56,  48,   8,   0,  53,
     82,  79,  89, 115, 137, 152, 163, 175,
    175, 159, 143, 132, 120, 113, 114, 118,
    117, 118, 133, 163, 189
};

static const float SaxFreq [12]={196, 207.66, 220.01, 233.09, 246.95, 261.63, 277.19, 293.67, 311.13, 329.63, 349.23, 370};





//Global Vals(For Debugging Via LCD ISR)
//
    //sax vars's
    static float sax_current_note_freq=196;
    static float sax_atk_rate,sax_decay_rate,sax_release_rate;
    static float sax_env_lvl;
    uint8_t sax_env_state;


    static uint8 sax_sample;
    static int16_t sax_zero_centered;
    static uint8_t sax_enveloped_note;
    float sax_env_out;

    static float SAX_ATTACK_TIME = .01;
    static float SAX_SUSTAIN_LEVEL = .8;
    static float SAX_RELEASE_TIME = .2;
    static float SAX_DECAY_TIME =.1;
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
    led_Write(~led_Read());
}
void Envelope_Release(uint8_t *env_state){
  if(*env_state!=IDLE){
    *env_state= RELEASE;
  }
}

static struct env_return Envelope_Process(float current_note_freq, uint8_t tbl_size,uint8_t env_state,
                              float env_lvl,float atk_rate,float decay_rate,float sustain_level,
                              float release_rate)
{
    // compute time per tick (seconds)
    led1_Write(~led1_Read());
    float isr_freq = current_note_freq * tbl_size;
    float dt       = 1.0f / isr_freq; //time passed which is pretty versitle imo
    switch (env_state) {
        case IDLE:
            env_lvl=0;
           // return 0.0f;
        case ATTACK:
            env_lvl += atk_rate * dt;
            if (env_lvl >= 1.0f) {
               //we hit that peak
                env_lvl = 1.0f;
                env_state = DECAY;
            }
            break;
        case DECAY:
            env_lvl -= decay_rate * dt;
            if (env_lvl <= sustain_level) {
                  env_lvl = sustain_level;
                  env_state = SUSTAIN;
            }
            break;
        case SUSTAIN:
            // hold
            break;
        case RELEASE:
            env_lvl -= release_rate * dt;
            if (env_lvl <= 0.0f) {
                env_lvl = 0.0f;
                env_state = IDLE;
            }
            break;
    }
   
    struct env_return ret;
    ret.env_lvl=env_lvl;
    ret.env_state=env_state;
    

    return ret;
}
//Sax ISR
CY_ISR(Sax_ISR)
{
    static uint8 idx = 0;
    sax_sample = SaxTable[idx++];
    if (idx >= SAXTABLE_SIZE) idx = 0;
    //pretty easy to debug tbh
    struct env_return sax_env_out = Envelope_Process(sax_current_note_freq,SAXTABLE_SIZE,sax_env_state,sax_env_lvl,sax_atk_rate,sax_decay_rate,SAX_SUSTAIN_LEVEL,sax_release_rate);
    sax_env_lvl=sax_env_out.env_lvl;
    sax_env_state=sax_env_out.env_state;
    //awes*ome bug needs to be int16not uint cause we need -128
    int16_t zero_centered=(int16_t)sax_sample-128;
    int16_t scaled=(int16_t)zero_centered*sax_env_out.env_lvl;
    sax_enveloped_note=(uint8_t)(scaled+128);
    //Adsr Code Goes Here we can cur from here
    SaxDac_SetValue(sax_enveloped_note);
    //sax_isr_ClearPending();
}

CY_ISR(Lcd_ISR){
    LCD_Char_1_Position(0, 6);
    LCD_Char_1_PrintNumber(sax_env_state);
}
int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
 
    //Initalize ISR For Sax
  
    Sax_Env_Init();
    LCD_Char_1_Start();					// initialize lcd
	LCD_Char_1_ClearDisplay();
	LCD_Char_1_PrintString("ADC : ");  
    sax_isr_StartEx(Sax_ISR);
    lcd_isr_StartEx(Lcd_ISR);
    sax_clk_SetDivider(413);
    //Initliaze Envelope_Process;
    //Start Sax Dac
    SaxDac_Start(); 
    SaxDac_SetValue(255);
    for(;;)
    {
        sax_clk_SetDivider(3000000/(SaxFreq[0]*37));
        sax_current_note_freq=SaxFreq[0];
        Envelope_Pressed(&sax_env_state);
        CyDelay(300);
        Envelope_Release(&sax_env_state);
        CyDelay(100);
        sax_clk_SetDivider(3000000/(SaxFreq[4]*37));
        sax_current_note_freq=SaxFreq[4];
        Envelope_Pressed(&sax_env_state);
        CyDelay(100);
        Envelope_Release(&sax_env_state);
        CyDelay(300);
        sax_clk_SetDivider(3000000/(SaxFreq[2]*37));
        sax_current_note_freq=SaxFreq[2];
        Envelope_Pressed(&sax_env_state);
        CyDelay(500);
        Envelope_Release(&sax_env_state);
        CyDelay(500);
    }
}

/* [] END OF FILE */
