/**
 * @file charger_discharger.h
 * @author Juan J. Rojas
 * @date 7 Aug 2018
 * @brief State machine header file for Charge and Discharge System.
 * @par Institution:
 * LaSEINE / CeNT. Kyushu Institute of Technology.
 * @par Mail (after leaving Kyutech):
 * juan.rojas@tec.ac.cr
 * @par Git repository:
 * https://bitbucket.org/juanjorojash/cell_charger_discharger
 */

#ifndef CHARGER_DISCHARGER_H
    #define CHARGER_DISCHARGER_H
    // PIC16F1786 Configuration Bit Settings

    // 'C' source line config statements
    #pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
    #pragma config WDTE = OFF       // Watchdog Timer Disabled (WDT disabled)
    #pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
    #pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)//If this is enabled, the Timer0 module will not work properly.
    #pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
    #pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
    #pragma config BOREN = OFF       // Brown-out Reset Enable (Brown-out Reset enabled)
    #pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
    #pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
    #pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

    // CONFIG2
    #pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
    #pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable bit (Vcap functionality is disabled on RA6.)
    #pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
    #pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
    #pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
    #pragma config LPBOR = OFF      // Low Power Brown-Out Reset Enable Bit (Low power brown-out is disabled)
    #pragma config LVP = OFF        // Low-Voltage Programming Enable (Low-voltage programming disabled)//IF THIS IN ON MCLR is always enabled

    #include <xc.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>
    #include <stdint.h> // To include uint8_t and uint16_t
    #include <string.h>
    #include <stdbool.h> // Include bool type
    /** This is the State Machine enum*/
	enum states { 
        STANDBY = 0, ///< "Stand by" state, defined by function @link fSTANDBY() @endlink
        IDLE = 1, ///< "Idle" state, defined by function @link fIDLE() @endlink
        FAULT = 2, ///< "Fault" state, defined by function @link fFAULT() @endlink
        ISDONE = 3, ///< "Is done" state, defined by function @link fISDONE() @endlink
        WAIT = 4, ///< "Wait" state, defined by function @link fWAIT() @endlink
        PREDISCHARGE = 5, ///< "Predischarge" state, defined by function @link fDISCHARGE() @endlink
        CHARGE = 6, ///< "Charge" state, defined by function @link fCHARGE() @endlink
        DISCHARGE = 7, ///< "Discharge" state, defined by function @link fDISCHARGE() @endlink
        POSTCHARGE = 8, ///< "Postcharge" state, defined by function @link fCHARGE() @endlink
        DS_DC_res = 9, ///< "Discharged state DC resistance" state, defined by function @link fDC_res() @endlink
        CS_DC_res = 10, ///< "Charged state DC resistance" state, defined by function @link fDC_res() @endlink
        PS_DC_res = 11 ///< "Postcharged state DC resistance" state, defined by function @link fDC_res() @endlink
    };    
    void fSTANDBY(void);
    void fIDLE(void);
    void fCHARGE(void);
    void fDISCHARGE(void);
    void fDC_res(void);
    void fWAIT(void);
    void fISDONE(void);
    void fFAULT(void);
    void start_state_machine(void);
    void state_machine(void);
    void param(void);
    void converter_settings(void);
    void initialize(void);
    void pid(float feedback, unsigned setpoint);
    void set_DC(void);
    void read_ADC(void);
    void log_control(void);
    void display_value(int value);
    void cc_cv_mode(float current_voltage, unsigned int reference_voltage, char CC_mode_status);
    void control_loop(void);
    void calculate_avg(void);
    void UART_interrupt_enable(void);
    void UART_send_char(char bt);
    char UART_get_char(void); 
    void UART_send_string(char* st_pt);
    void Cell_ON(void);
    void Cell_OFF(void);
    void timing(void);
    #define     _XTAL_FREQ              32000000 ///< Frequency to coordinate delays, 32 MHz
    #define     ERR_MAX                 250 ///< Maximum permisible error, useful to avoid ringing
    #define     ERR_MIN                 -250 ///< Minimum permisible error, useful to avoid ringing
    #define     V_CHAN                  0b01101 ///< Definition of ADC channel for voltage measurements. AN13(RB5) 
    #define     I_CHAN                  0b01011 ///< Definition of ADC channel for current measurements. AN11(RB4)
    #define     T_CHAN                  0b01100 ///< Definition of ADC channel for temperature measurements. AN12(RB0)
    #define     CELL1_ON                PORTAbits.RA7 = 1 ///< Turn on Cell #1
    #define     CELL2_ON                PORTAbits.RA6 = 1 ///< Turn on Cell #2
    #define     CELL3_ON                PORTCbits.RC0 = 1 ///< Turn on Cell #3
    #define     CELL4_ON                PORTCbits.RC1 = 1 ///< Turn on Cell #4
    #define     CELL1_OFF               PORTAbits.RA7 = 0 ///< Turn off Cell #1
    #define     CELL2_OFF               PORTAbits.RA6 = 0 ///< Turn off Cell #2
    #define     CELL3_OFF               PORTCbits.RC0 = 0 ///< Turn off Cell #3
    #define     CELL4_OFF               PORTCbits.RC1 = 0 ///< Turn off Cell #4
    #define     AD_SET_CHAN(x)          { ADCON0bits.CHS = x; __delay_us(1); } ///< Set the ADC channel to @p x and wait for 5 microseconds.
    #define     AD_CONVERT()            { GO_nDONE = 1; while(GO_nDONE);} ///< Start the conversion and wait until it is finished
    #define     AD_RESULT()             { ad_res = 0; ad_res = (ADRESL & 0xFF)|((ADRESH << 8) & 0xF00);} ///< Store conversion result in #ad_res
    /** @brief Stop the converter*/
    /** Set @p conv to zero, turn off the main relay (@p RA1), set the duty cycle in @p DC_MIN, 
    turn off all the cell relays in the switcher board, disable the logging of data to the terminal 
    and the UART reception interrupts.
    */
    #define     STOP_CONVERTER()        { conv = 0; RA1 = 1; dc = DC_MIN; set_DC(); Cell_OFF(); LOG_OFF();}
    #define     UART_INT_ON()           { while(RCIF) clear = RC1REG; RCIE = 1; } ///< Clear transmission buffer and turn ON UART transmission interrupts.
    //#define   UART_INT_OFF()          { log_on = 0; }  ///< Turn OFF UART transmission interrupts. //CHECK THIS
    #define     LOG_ON()                { log_on = 1; }  ///< Turn OFF logging in the terminal.
    #define     LOG_OFF()               { log_on = 0; }  ///< Turn ON logging in the terminal.
    #define     RESET_TIME()            { minute = 0; second = -1; } ///< Reset timers.
    #define     DC_MIN                  25  ///< Minimum possible duty cycle, set around @b 0.05
    #define     DC_START                51  ///< Initial duty cycle, set around @b 0.1
    //It seems that above 0.8 of DC the losses are so high that I don't get anything similar to the transfer function
    #define     DC_MAX                  385  ///< Maximum possible duty cycle, set around @b 0.8
    #define     COUNTER                 250  ///< Counter value, needed to obtained one second between counts.
    #define     CV_kp                   0.4  ///< Proportional constant for CV mode
    #define     CV_ki                   0.5  ///< Integral constant for CV mode 
    #define     LINEBREAK               { UART_send_char(10); } ///< Send a linebreak to the terminal
    //Chemistry definition
    #define     LI_ION_CHEM             0 ///< Set this definition to 1 and NI_MH_CHEM to 0 to set the test Li-Ion cells  
    #define     NI_MH_CHEM              1 ///< Set this definition to 1 and LI_ION_CHEM to 0 to set the test Ni-MH cells
    //General definitions
    #define     WAIT_TIME               600 ///< Time to wait before states, set to 10 minutes
    #define     DC_RES_SECS             14 ///< How many seconds the DC resistance process takes
    //Li-Ion definitions
    #define     Li_Ion_CV               4200 ///< Li-Ion constant voltage setting in mV
    #define     Li_Ion_CAP              3250 ///< Li-Ion capacity setting in mAh
    #define     Li_Ion_EOC_I            100 ///< Li-Ion end-of-charge current in mA
    #define     Li_Ion_EOD_V            3000 ///< Li_Ion end-of-discharge voltage in mV
    //Ni-MH definitions
    #define     Ni_MH_CV                1700 ///< Ni-MH constant voltage setting in mV
    #define     Ni_MH_CAP               2000 ///< Ni-MH capacity setting in mAh
    #define     Ni_MH_EOC_DV            10 ///< Ni-MH end-fo-charge voltage drop in mV
    #define     Ni_MH_EOD_V             1000 ///< Ni-MH end-of-discharge voltage in mV
    //Variables
    unsigned char               option = 0; ///< Four different options, look into @link param() @endlink for details
    unsigned                    capacity; ///< Definition of capacity per cell according to each chemistry
    unsigned                    i_char; ///< Charging current in mA
    unsigned                    i_disc; ///< Discharging current in mA
    unsigned char               cell_count = 49; ///< Cell counter from '1' to '4'. Initialized as '1'
    unsigned char               cell_max = 0; ///< Number of cells to be tested. Initialized as 0
    unsigned                    wait_count = WAIT_TIME; ///< Counter for waiting time between states. Initialized as @link WAIT_TIME @endlink
    unsigned                    dc_res_count = DC_RES_SECS; ///< Counter for DC resistance. Initialized as @link DC_RES_SECS @endlink
    unsigned char               state = STANDBY; ///< Used with store the value of the @link states @endlink enum. Initialized as @link STANDBY @endlink
    unsigned char               prev_state = STANDBY; ///< Used to store the previous state. Initialized as @link STANDBY @endlink  
    unsigned                    EOC_current; ///< End-of-charge current in mA
    unsigned                    EOD_voltage; ///< End-of-dischage voltage in mV
    float                       v_1_dcres; ///< First voltage measured during DC resistance state 
    float                       i_1_dcres; ///< First current measured during DC resistance state  
    float                       v_2_dcres; ///< Second voltage measured during DC resistance state 
    float                       i_2_dcres; ///< Second current measured during DC resistance state
    float                       dc_res_val; ///< To store the operation of obtained from the DC resistance state
    unsigned                    conv = 0; ///< Turn controller ON(1) or OFF(0). Initialized as 0
    unsigned                    count = COUNTER; ///< Counter that should be cleared every second. Initialized as #COUNTER 
    /**< Every control loop cycle this counter will be decreased. This variable is used to calculate the averages and to trigger
    all the events that are done every second.*/
    unsigned                    ad_res; ///< Result of an ADC measurement.
    float                       v;  ///< Last voltage ADC measurement.
    float                       i;  ///< Last current ADC measurement.
    float                       t;  ///<  Last temperature ADC measurement.
    float                       vprom = 0;  ///< Last one-second-average of #v . Initialized as 0
    float                       iprom = 0;  ///< Last one-second-average of #i . Initialized as 0
    float                       tprom = 0;  ///< Last one-second-average of #t . Initialized as 0
    float                       qprom = 0;  ///< Integration of #i . Initialized as 0
    int                         vmax = 0;   ///< Maximum recorded average voltage. 
    float                       proportional;  ///< Proportional component of PI compensator
    float                       integral;  ///< Integral component of PI compensator
    float                       kp;  ///< Proportional compesator gain
    float                       ki;  ///< Integral compesator gain      
    unsigned                    vref = 0;  ///< Voltage setpoint. Initialized as 0
    unsigned                    iref = 0;  ///< Current setpoint. Initialized as 0
    char                        cmode;  ///< CC / CV selector. CC: <tt> cmode = 1 </tt>. CV: <tt> cmode = 0 </tt>   
    unsigned                    dc = 0;  ///< Duty cycle
    char                        clear;  ///< Variable to clear the transmission buffer of UART
    unsigned                    log_on = 0; ///< Variable to indicate if the log is activated 
    char                        log_buffer[8]={0}; ///< Temporary buffer used between loops of #log_control()  
    int                         ip_buff = 0;  ///< Current buffer to send to the terminal usign #log_control()
    int                         vp_buff = 0;  ///< Voltage buffer to send to the terminal usign #log_control()
    int                         tp_buff = 0;  ///< Temperature buffer to send to the terminal usign @log_control().
    unsigned                    qp_buff = 0;  ///< Capacity buffer to send to the terminal usign #log_control() .
    int                         second = 0; ///< Seconds counter, resetted after 59 seconds.
    int                         minute = 0; ///< Minutes counter, only manually reset
    unsigned                    timeout = 0;
    //Strings
    char const                  comma = ',';
    char const                  colons = ':'; 
    char const                  S_str = 'S';
    char const                  C_str = 'C';
    char const                  V_str = 'V';
    char const                  I_str = 'I';
    char const                  T_str = 'T';
    char const                  Q_str = 'Q';
    char const                  R_str = 'R';
    char const                  W_str = 'W';
    char const                  press_s_str[] = "Press 's' to start: ";
    char const                  starting_str[] = "Starting...";
    char const                  done_str[] = "DONE";
    char const                  num_1and2_str[] = "Please input a number between 1 and 2";
    char const                  num_1and3_str[] = "Please input a number between 1 and 3";
    char const                  num_1and4_str[] = "Please input a number between 1 and 4";         
    char const                  param_def_str[] = "---Parameter definition for charger and discharger---";
    char const                  restarting_str[] = "Restarting...";
    char const                  chem_def_liion[] = "Chemistry defined as Li-Ion";
    char const                  chem_def_nimh[] = "Chemistry defined as Ni-MH";
    char const                  mV_str[] = " mV";
    char const                  mAh_str[] = " mAh";
    char const                  mA_str[] = " mA";
    char const                  EOD_V_str[] = "End of discharge voltage: ";
    char const                  EOC_I_str[] = "End of charge current: ";
    char const                  EOC_DV_str[] = "End of charge voltage drop: ";
    char const                  cho_bet_str[] = "Chose between following options: ";
    char const                  quarter_c_str[] = "(1) 0.25C";
    char const                  half_c_str[] = "(2) 0.50C";
    char const                  one_c_str[] = "(3) 1C";
    char const                  cell_str[] = "Cell "; 
    char const                  dis_def_quarter_str[] = "Discharge current defined as 0.25C";
    char const                  dis_def_half_str[] = "Discharge current defined as 0.5C";
    char const                  dis_def_one_str[] = "Discharge current defined as 1C";
    char const                  char_def_quarter_str[] = "Charge current defined as 0.25C";
    char const                  char_def_half_str[] = "Charge current defined as 0.5C";
    char const                  char_def_one_str[] = "Charge current defined as 1C";
    char const                  cv_val_str[] = "Constant voltage value: ";
    char const                  nom_cap_str[] = "Nominal capacity: ";
    char const                  def_char_curr_str[] = "Define charge current (input the number): ";
    char const                  def_disc_curr_str[] = "Define discharge current (input the number): ";
    char const                  def_num_cell_str[] = "Define number of cells to be tested (input the number, maximum of 4): ";
    char const                  num_cell_str[] = "Number of cells to be tested: ";
    char const                  one_str[] = "1";
    char const                  two_str[] = "2";
    char const                  three_str[] = "3";
    char const                  four_str[] = "4";
    char const                  op_1_str[] = "(1) Predischarge->Charge->Discharge->Postcharge";
    char const                  op_2_str[] = "(2) Charge->Discharge";
    char const                  op_3_str[] = "(3) Only Charge";
    char const                  op_4_str[] = "(4) Only Discharge";
    char const                  op_1_sel_str[] = "Predischarge->Charge->Discharge->Postcharge selected...";
    char const                  op_2_sel_str[] = "Charge->Discharge selected...";
    char const                  op_3_sel_str[] = "Only Charge selected...";
    char const                  op_4_sel_str[] = "Only Discharge selected...";
    char const                  cell_below_str[] = "Cell below 0.9V or not present";

#endif /* CHARGER_DISCHARGER_H*/

