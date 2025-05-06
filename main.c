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

    //EnveleopeGlobals
    static float current_note_freq=196;
    static float atk_rate,decay_rate,release_rate;
    static float env_lvl;
    uint8_t env_state;


    static uint8 sax_sample;
    static int16_t zero_centered;
    static uint8_t enveloped_note;
    float env_out;

 //Envelope Vars;
    enum {
        IDLE,
        ATTACK,
        DECAY,
        SUSTAIN,
        RELEASE
    };

static float ATTACK_TIME = .01;
static float SUSTAIN_LEVEL = .8;
static float RELEASE_TIME = .2;
static float DECAY_TIME =.1;
//

void Envelope_Init(void){
    atk_rate  = 1.0f / ATTACK_TIME;                 
    decay_rate   = (1.0f - SUSTAIN_LEVEL) / DECAY_TIME;
    release_rate =  SUSTAIN_LEVEL        / RELEASE_TIME;
    env_lvl    = 0.0f;
    env_state    = IDLE;
}

void Envelope_Pressed(void){
    env_state=ATTACK;
  
    
}
void Envelope_Release(void){
  if(env_state!=IDLE){
    env_state= RELEASE;
  }
}

static float Envelope_Process(void)
{
    // compute time per tick (seconds)
    float isr_freq = current_note_freq * SAXTABLE_SIZE;
    float dt       = 1.0f / isr_freq; //time passed which is pretty versitle imo
    switch (env_state) {
        case IDLE:
            env_lvl=0;
            return 0.0f;
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
            if (env_lvl <= SUSTAIN_LEVEL) {
                  env_lvl = SUSTAIN_LEVEL;
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

    return env_lvl;
}

//Sax ISR
CY_ISR(Sax_ISR)
{
    static uint8 idx = 0;
    sax_sample = SaxTable[idx++];
    if (idx >= SAXTABLE_SIZE) idx = 0;
    //pretty easy to debug tbh
    env_out=Envelope_Process();


    //awes*ome bug needs to be int16not uint cause we need -128
    int16_t zero_centered=(int16_t)sax_sample-128;

    int16_t scaled=(int16_t)zero_centered*env_out;

    enveloped_note=(uint8_t)(scaled+128);

    //Adsr Code Goes Here we can cur from here
    

     SaxDac_SetValue(enveloped_note);

         
    //sax_isr_ClearPending();

}

CY_ISR(Lcd_ISR){
    LCD_Char_1_Position(0, 6);
    LCD_Char_1_PrintNumber(enveloped_note);


}
int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
 
    //Initalize ISR For Sax
    Envelope_Init();
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
        Envelope_Pressed();
        CyDelay(300);
        Envelope_Release();
        CyDelay(100);
        sax_clk_SetDivider(3000000/(SaxFreq[4]*37));
        Envelope_Pressed();
        CyDelay(100);
        Envelope_Release();
        CyDelay(300);
        sax_clk_SetDivider(3000000/(SaxFreq[2]*37));
        Envelope_Pressed();
        CyDelay(500);
        Envelope_Release();
        CyDelay(500);
    }
}

/* [] END OF FILE */
