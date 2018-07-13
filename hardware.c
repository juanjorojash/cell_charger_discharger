/* Hardware related source file for Charge and Discharge System. */
/* Kyutech Institute of Technology. LaSEINE. Supervisor: Mengu CHO.*/
/* 
 * File:  hardware.c 
 * Author: Juan J. Rojas.
 * Version control in Git
 */
#include "hardware.h"
#include "state_machine.h"

//unsigned int            ad_res;
char const              comma_str[] = ",";
char const              in_arr_str[] = "->";
char const              end_arr_str[] = "<-";
char const              in_wait_str[] = "------------W-";
char const              end_wait_str[] = "-W------------";
char const              hip_str[] = "-";
char const              in_sta_str[] = "S-";
char const              end_sta_str[] = "-S";
char const              C_str[] = "C";
char const              V_str[] = "V";
char const              I_str[] = "I";
char const              T_str[] = "T";


void Initialize_Hardware()
{
	//CLRWDT();
	TMR0IF = 0;                        //Clear timer 0
	ad_res = 0;                        //Clear ADC result variable
	cmode = 1;                         //Start in CC mode
	iref = 0;                  
	vref = 0;
	//STOP_CONVERTER();
}

void Init_Registers()
{
    //-----------------------GENERAL-------------------------------------------
    nWPUEN = 0;           //Allow change of individual WPU
    //-----------------------SYSTEM CLOCK--------------------------------------
    //PLL is always enabled because of configuration bits.
    OSCCONbits.IRCF = 0b1111;           //Set clock to 32MHz (with PLL)
    OSCCONbits.SCS = 0b00;              //Clear to use the result of IRCF
    OSCCONbits.SPLLEN = 1;              //Enable PLL, it gives a problem if is done in the CONFWords
    //System clock set as 32MHz
    //--------------------OUPUTS FOR RELAYS------------------------------------
    TRISA0 = 0;                         //Set RA0 as output. C/D relay
    ANSA0 = 0;                          //Digital
    TRISA1 = 0;                         //Set RA3 as output.ON/OFF relay
    ANSA1 = 0;                          //Digital   
//    //----------------OUTPUTS FOR CELL SWITCHER------------------------------
//    TRISAbits.TRISA7 = 0;               //Set RA7 as output. Cell #1
//    ANSELAbits.ANSA7 = 0;               //Digital
//    TRISAbits.TRISA6 = 0;               //Set RA6 as output. Cell #2
//    ANSELAbits.ANSA6 = 0;               //Digital   //DOES NOT EXIST
//    TRISCbits.TRISC0 = 0;               //Set RC0 as output. Cell #3
//    ANSELCbits.ANSC0 = 0;               //Digital   //DOES NOT EXIST
//    TRISCbits.TRISC1 = 0;               //Set RC1 as output. Cell #4
//    ANSELCbits.ANSC1 = 0;               //Digital   //DOES NOT EXIST
    //-----------TIMER0 FOR CONTROL AND MEASURING LOOP-------------------------
    TMR0IE = 0;                         //Disable timer interruptions
    TMR0CS = 0;                         //Timer set to internal instruction cycle
    OPTION_REGbits.PS = 0b100;          //Prescaler set to 32
    OPTION_REGbits.PSA = 0;             //Prescaler activated
    TMR0IF = 0;                         //Clear timer flag
    //Timer set to 32/4/64/256 = 976.56Hz
    
    //---------------------PSMC/PWM SETTING------------------------------------
    TRISA4 = 1;                         //[Temporary]Set RA4 as input to let it drive from RB3. 
    WPUA4 = 0;                          //Disable WPU for RA4.  
   
    PSMC1CON = 0x00;                    //Clear configuration to start 
    PSMC1MDL = 0x00;                    //No modulation
    PSMC1CLK = 0x01;                    //Driven by 64MHz system clock
    //PSMC1CLKbits.P1CSRC = 0b01;         //Driven by 64MHz system clock
    //PSMC1CLKbits.P1CPRE = 0b00;         //No prescaler (64MHz)
    //Period
    PSMC1PRH = 0x00;                    //No HB
    PSMC1PRL = 0xFF;                    //255 + 1 clock cycles for period that is 4us (250KHz)
    //This set the PWM with 8 bit of resolution
    //Duty cycle
    PSMC1DCH = 0x00;                    //Duty cycle starts in 0   
    PSMC1DCL = 0x00;                    //Duty cycle starts in 0   
    //Phase or rising event
    PSMC1PHH = 0x00;                    //Rising event starts from the beginning
    PSMC1PHL = 0x00;                    //Rising event starts from the beginning
    
    PSMC1STR0bits.P1STRA = 1;           //Single PWM activated in PSMC1A (RCO))
    PSMC1POLbits.P1POLA = 0;            //Active high
    PSMC1OENbits.P1OEA = 1;             //PSMC1A activated in PSMC1A (RC0)
    
    PSMC1PRSbits.P1PRST = 1;            //Period event occurs when PSMC1TMR = PSMC1PR
    PSMC1PHSbits.P1PHST = 1;            //Rising edge event occurs when PSMC1TMR = PSMC1PH
    PSMC1DCSbits.P1DCST = 1;            //Falling edge event occurs when PSMC1TMR = PSMC1DC
    
    PSMC1CON = 0x80;                    //Enable|Load Buffer|Dead band disabled|Single PWM
    //PSMC1TIE = 1;                       //Enable interrupts for Time Based 
    TRISC0 = 0;                         //Set RC0 as output
    
    //---------------------ADC SETTINGS----------------------------------------
    //INTERRUPTS 
    //PIE1bits.ADIE = 1;                  //Activate interrupts
    //INTCONbits.PEIE =1;                 //Pehierals interrupts
    
    //ADC INPUTS//check this after final design
    TRISA3 = 1;                         //RA3, Positive voltage reference 
    ANSA3 = 0;                          //RA3 analog
    WPUA3 = 0;                          //Weak pull up Deactivated
    TRISB4 = 1;                         //RB4, current sensing input    
    ANSB4 = 1;                          //RB4 analog      
    WPUB4 = 0;                          //Weak pull up Deactivated
    TRISB5 = 1;                         //RB5, voltage sensing input
    ANSB5 = 1;                          //RB5 analog
    WPUB5 = 0;                          //Weak pull up Deactivated
    
    ADCON0bits.ADRMD = 0;               //12 bits result
    ADCON1bits.ADCS = 0b110;            //Clock selected as FOSC/64
    ADCON1bits.ADNREF = 0;              //Connected to Vss
    ADCON1bits.ADPREF = 0b01;           //Connected to Vref+
    ADCON1bits.ADFM = 1;                //2's compliment result
    ADCON2bits.CHSN = 0b1111;           //Negative differential input as ADNREF
    ADCON0bits.ADON = 1;                //Turn on the ADC
    
}

void pid(unsigned int feedback, unsigned int setpoint)
{
int 	er;
int		pi;
	er = setpoint - feedback;

	if(er > ERR_MAX) er = ERR_MAX;
	if(er < ERR_MIN) er = ERR_MIN;
    
	proportional = kp * er;
	integral += ki * er * 0.001; //time base is 1Khz (t = 1/1000)

	pi = proportional + integral; 
    
    if (dc + pi >= DC_MAX){
        dc = DC_MAX;
    }else if (dc + pi <= DC_MIN){
        dc = DC_MIN;
    }else{
        dc += pi; //This is the point in which a mix the PWM with the PID
    }       
    set_DC();
}

void set_DC()
{
    PSMC1DCL = dc;     
    PSMC1CONbits.PSMC1LD = 1; //Load Buffer
}

void cc_cv_mode()
{
    if (!count)
    {
        if(v > vref && cmode == 1)
        {
            integral = 0;
            cmode = 0;
            kp = 0.001;
            ki = 0.05;
        }
    }         
}

void log_control()
{
        if (!count)
        {
            if (wait_count < wait_time && wait_count > 1)
            {   
                LINEBREAK;
                UART_send_string(in_wait_str);
                display_value(wait_count);
                UART_send_string(end_wait_str);             
            }
            if (wait_count) wait_count--;
            /*if (dc_res_count)
            {
                UART_send_string(hip_str);
                display_value(dc_res_count);
                UART_send_string(hip_str);               
            }*/
            if (log_on)
            {
                LINEBREAK;
                UART_send_string(in_sta_str);
                display_value(state);
                UART_send_string(end_sta_str);
                UART_send_string(in_arr_str);
                UART_send_string(C_str);
                display_value(cell_count - 48);
                UART_send_string(comma_str);
                UART_send_string(V_str);
                //display_value(v);
                display_value(vprom*10);
                UART_send_string(comma_str);
                UART_send_string(I_str);
                //display_value(i);
                display_value(iprom*10);   
                UART_send_string(comma_str);
                UART_send_string(T_str);
                //display_value(dc);
                display_value(tprom);  
                //UART_send_string(comma_str); //
                //display_value(t);           //
                /*UART_send_string(comma_str);
                UART_send_string("Inc");
                display_value(inc);*/
                UART_send_string(end_arr_str);
            }
            count = COUNTER;
            iprom = 0;
            vprom = 0;
            tprom = 0;
        }
}
//THIS ADC IS WORKING NOW
void read_ADC()
{
    unsigned long opr;
    AD_SET_CHAN(V_CHAN);
    AD_CONVERT();
    AD_RESULT();
    //v = ad_res * 1.23779; //* 1.2207;    
    opr = 1.29296 * ad_res - 10;   //1051/1000 with 5039/4096  10 of bias
    v = opr;    //0 as offset   
    AD_SET_CHAN(I_CHAN);
    AD_CONVERT();
    AD_RESULT();
    opr = 1.23022 * ad_res - 20;     //with 5039/4096  20 of bias
//    i = opr;
    if(opr > 2500)
    {
        opr = opr - 2500;
    }
    else if(i == 2500)
    {
        opr = 0;
    }
    else if(i < 2500)
    {
        opr = 2500 - opr;
    }
    //i=i/0.4;       //A mOhms resistor
    //i = (200/37) * i; //Hall effect sensor  37/200=0.185
    opr = 25 * opr;
    i = opr / 10; //HALL EFFECT ACS723LL
    opr = 0;     
}

void control_loop()
{
    cc_cv_mode();
	if(!cmode)
    {
        pid(v, vref);
    }else
    {
        pid(i, iref);
    }
}

void calculate_avg()
{
    if(!PORTAbits.RA1)
    {
        if (count)
        {
            iprom += i;
            vprom += v;
            tprom += dc * 0.390625;
            count--;
        }
        if (!count)
        {
            iprom = iprom / COUNTER;
            vprom = vprom / COUNTER;
            tprom = tprom / COUNTER;
        }      
    }else
    {
        count--;
        iprom = 0;
        vprom = 0;
        tprom = 0;
    }
}

//**Beginning of the UART related functions. 
void Init_UART()
{
    //****Setting I/O pins for UART****//
    TXSEL = 0;      //RC6 selected as TX
    RXSEL = 0;      //RC7 selected as RX
    //________I/O pins set __________//
    
    /**Initialize SPBRG register for required 
    baud rate and set BRGH for fast baud_rate**/
    //spb = ((_XTAL_FREQ/64)/BAUD_RATE) - 1;
    SP1BRGH = 0x00; 
    SP1BRGL = 51;    
    
    BRGH  = 0;  // for high baud_rate
    BRG16 = 0;  // for 16 bits timer
    //_________End of baud_rate setting_________//
    
    //****Enable Asynchronous serial port*******//
    SYNC  = 0;    // Asynchronous
    SPEN  = 1;    // Enable serial port pins
    //_____Asynchronous serial port enabled_______//

    //**Lets prepare for transmission & reception**//
    TXEN  = 1;    // enable transmission
    CREN  = 1;    // enable reception
    //__UART module up and ready for transmission and reception__//
    
    //**Select 8-bit mode**//  
    TX9   = 0;    // 8-bit reception selected
    RX9   = 0;    // 8-bit reception mode selected
    //__8-bit mode selected__//    
    //INTERRUPTS
}

void UART_interrupt_enable()
{
    while(RCIF){                //clear the reception register
        esc = RC1REG;
        esc = 0;
    }
    RCIE = 1;                   //enable reception interrupts
    TXIE = 0;                   //disable transmision interrupts
    PEIE = 1;                   //enable peripherals interrupts
    GIE = 1;                    //enable global interrupts   
}

//**Function to send one byte of date to UART**//
void UART_send_char(char bt)  
{
    while(0 == TXIF)
    {
    }// hold the program till TX buffer is free
    TX1REG = bt; //Load the transmitter buffer with the received value
}
//_____________End of function________________//

//**Function to get one byte of date from UART**//
char UART_get_char()   
{
    if(OERR) // check for Error 
    {
        CREN = 0; //If error -> Reset 
        CREN = 1; //If error -> Reset 
    }
    
    while(!RCIF);  // hold the program till RX buffer is free
    
    return RC1REG; //receive the value and send it to main function
}
//_____________End of function________________//

//**Function to convert string to byte**//
void UART_send_string(char* st_pt)
{
    while(*st_pt) //if there is a char
        UART_send_char(*st_pt++); //process it as a byte data
}

void display_value(unsigned int value)
{   
    char buffer[8]; 
  
    utoa(buffer,value,10);  
  
    UART_send_string(buffer);
}

void Cell_ON()
{
//    if (cell_count == 49)
//    {
//        CELL1_ON;
//        CELL2_OFF;
//        CELL3_OFF;
//        CELL4_OFF;
//    }else if (cell_count == 50)
//    {
//        CELL1_OFF;
//        CELL2_ON;
//        CELL3_OFF;
//        CELL4_OFF;        
//    }else if (cell_count == 51)
//    {
//        CELL1_OFF;
//        CELL2_OFF;
//        CELL3_ON;
//        CELL4_OFF;        
//    }else if (cell_count == 52)
//    {
//        CELL1_OFF;
//        CELL2_OFF;
//        CELL3_OFF;
//        CELL4_ON;        
//    }
}

void Cell_OFF()
{
//    CELL1_OFF;
//    CELL2_OFF;
//    CELL3_OFF;
//    CELL4_OFF;
}