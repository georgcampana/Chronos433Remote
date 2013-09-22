// system
#include <project.h>

// driver
#include "rf1a.h"
#include "simpliciti.h"
#include "display.h"
#include "ports.h"

// logic
#include "home.h"
#include "timer.h"
#include "radio.h"

// Define section
#define st(x)                                           do { x } while (__LINE__ == -1)
#define ENTER_CRITICAL_SECTION(x)       st(x = __get_interrupt_state(); __disable_interrupt(); )
#define EXIT_CRITICAL_SECTION(x)    __set_interrupt_state(x)

#define HOMEALARM_REGS_CONFIG	(0)
#define HOMEGATE_REGS_CONFIG	(1)

// *************************************************************************************************
// Prototypes section
void SetupRadio433_OOK(u8 config);
void mx_hremote(u8 line);
void sx_hremote(u8 line);
void display_hremote(u8 line, u8 update);
void mx_hgate(u8 line);
void sx_hgate(u8 line);
void display_hgate(u8 line, u8 update);


void tx_ook_sequence(u8* seq, u8 len, u8 configtype);

//static u8 alarmoffseq[] = {0x8e,0x8e,0x8e,0x8e,0x8e,0x8e,0x88,0x8e,
//							 0x88,0x88,0x88,0xee,0x80,0x00,0x00,0x00 };
// bits are inverted !!!! 0 = radio mod on ;  1 = radio mod off (OOK)
static u8 alarmoffseq[] = {0x71,0x71,0x71,0x71,0x71,0x71,0x77,0x71,
							 0x11,0x77,0x77,0x11,0x7f,0xff,0xff,0xff };

static u8 alarmonseq[] = {0x71,0x71,0x71,0x71,0x71,0x71,0x77,0x71,
							 0x11,0x11,0x77,0x77,0x7f,0xff,0xff,0xff };

static char onoff_toggle = 0; // 0 = off , 1 = on (default is off)


static u8 gateseq[] = {0xf9,0x69,0x59,0x99,0x5c,0x9a, 0xf9,0x69,0x59,0x99,0x5c,0x9a, 0xf9,0x69,0x59,0x99,0x5c,0x9a };


// HOME ALARM
void display_hremote(u8 line, u8 update)
{
    if (update == DISPLAY_LINE_UPDATE_FULL)
    {
        if(onoff_toggle == 0) {
        	display_chars(LCD_SEG_L2_5_0, (u8 *) " AL.OF", SEG_ON);
        }
        else {
        	display_chars(LCD_SEG_L2_5_0, (u8 *) " AL.ON", SEG_ON);
        }
    }
}

void sx_hremote(u8 line)
{
    // Exit if battery voltage is too low for radio operation
    if (sys.flag.low_battery)
        return;

    // Turn off the backlight
    P2OUT &= ~BUTTON_BACKLIGHT_PIN;
    P2DIR &= ~BUTTON_BACKLIGHT_PIN;

    BUTTONS_IE &= ~BUTTON_BACKLIGHT_PIN;
    
    if(onoff_toggle == 0) {
    	tx_ook_sequence(alarmoffseq, sizeof(alarmoffseq),HOMEALARM_REGS_CONFIG);
    	onoff_toggle = 1;
    }
    else {
    	tx_ook_sequence(alarmonseq, sizeof(alarmonseq),HOMEALARM_REGS_CONFIG);
    	onoff_toggle = 0;
    }
    BUTTONS_IE |= BUTTON_BACKLIGHT_PIN;
}

//  HOME GATE

void display_hgate(u8 line, u8 update)
{
    if (update == DISPLAY_LINE_UPDATE_FULL)
    {
		display_chars(LCD_SEG_L2_5_0, (u8 *) " HGATE", SEG_ON);
    }
}


void sx_hgate(u8 line)
{
    // Exit if battery voltage is too low for radio operation
    if (sys.flag.low_battery)
        return;

    // Turn off the backlight
    P2OUT &= ~BUTTON_BACKLIGHT_PIN;
    P2DIR &= ~BUTTON_BACKLIGHT_PIN;

    BUTTONS_IE &= ~BUTTON_BACKLIGHT_PIN;

   	tx_ook_sequence(gateseq, sizeof(gateseq),HOMEGATE_REGS_CONFIG);

    BUTTONS_IE |= BUTTON_BACKLIGHT_PIN;
}

//
// Rf settings for CC430
//

void SetupRadio433_OOK(u8 config) {
	
	if(config == HOMEALARM_REGS_CONFIG) { // Home Alarm
		WriteSingleReg(IOCFG0,0x06);  //GDO0 Output Configuration
		WriteSingleReg(FIFOTHR,0x47); //RX FIFO and TX FIFO Thresholds
		WriteSingleReg(PKTLEN,0x30);  //Packet Length
		WriteSingleReg(PKTCTRL0,0x00);//Packet Automation Control
		WriteSingleReg(FSCTRL1,0x06); //Frequency Synthesizer Control
		WriteSingleReg(FREQ2,0x10);   //Frequency Control Word, High Byte
		WriteSingleReg(FREQ1,0xB0);   //Frequency Control Word, Middle Byte
		WriteSingleReg(FREQ0,0x3F);   //Frequency Control Word, Low Byte
		WriteSingleReg(MDMCFG4,0xF6); //Modem Configuration
		WriteSingleReg(MDMCFG3,0x90); //Modem Configuration
		WriteSingleReg(MDMCFG2,0x30); //Modem Configuration
		WriteSingleReg(DEVIATN,0x15); //Modem Deviation Setting
		WriteSingleReg(MCSM0,0x10);   //Main Radio Control State Machine Configuration
		WriteSingleReg(FOCCFG,0x16);  //Frequency Offset Compensation Configuration
		WriteSingleReg(AGCCTRL1,0x00);//AGC Control
		WriteSingleReg(WORCTRL,0xFB); //Wake On Radio Control
		WriteSingleReg(FREND0,0x11);  //Front End TX Configuration
		WriteSingleReg(FSCAL3,0xE9);  //Frequency Synthesizer Calibration
		WriteSingleReg(FSCAL2,0x2A);  //Frequency Synthesizer Calibration
		WriteSingleReg(FSCAL1,0x00);  //Frequency Synthesizer Calibration
		WriteSingleReg(FSCAL0,0x1F);  //Frequency Synthesizer Calibration
		WriteSingleReg(TEST2,0x81);   //Various Test Settings
		WriteSingleReg(TEST1,0x35);   //Various Test Settings
		WriteSingleReg(TEST0,0x09);   //Various Test Settings
	}
	else {	// HOMEGATE_REGS_CONFIG
		WriteSingleReg(IOCFG0,0x06);  //GDO0 Output Configuration
		WriteSingleReg(FIFOTHR,0x47); //RX FIFO and TX FIFO Thresholds
		WriteSingleReg(PKTLEN,0x36);  //Packet Length
		WriteSingleReg(PKTCTRL0,0x00);//Packet Automation Control
		WriteSingleReg(FSCTRL1,0x06); //Frequency Synthesizer Control
		WriteSingleReg(FREQ2,0x10);   //Frequency Control Word, High Byte
		WriteSingleReg(FREQ1,0xB0);   //Frequency Control Word, Middle Byte
		WriteSingleReg(FREQ0,0x71);   //Frequency Control Word, Low Byte
		WriteSingleReg(MDMCFG4,0xF4); //Modem Configuration
		WriteSingleReg(MDMCFG3,0x02); //Modem Configuration
		WriteSingleReg(MDMCFG2,0x33); //Modem Configuration
		WriteSingleReg(DEVIATN,0x15); //Modem Deviation Setting
		WriteSingleReg(MCSM0,0x10);   //Main Radio Control State Machine Configuration
		WriteSingleReg(FOCCFG,0x16);  //Frequency Offset Compensation Configuration
		WriteSingleReg(AGCCTRL1,0x00);//AGC Control
		WriteSingleReg(WORCTRL,0xFB); //Wake On Radio Control
		WriteSingleReg(FREND0,0x11);  //Front End TX Configuration
		WriteSingleReg(FSCAL3,0xE9);  //Frequency Synthesizer Calibration
		WriteSingleReg(FSCAL2,0x2A);  //Frequency Synthesizer Calibration
		WriteSingleReg(FSCAL1,0x00);  //Frequency Synthesizer Calibration
		WriteSingleReg(FSCAL0,0x1F);  //Frequency Synthesizer Calibration
		WriteSingleReg(TEST2,0x81);   //Various Test Settings
		WriteSingleReg(TEST1,0x35);   //Various Test Settings
		WriteSingleReg(TEST0,0x09);   //Various Test Settings
		
	}
}

void tx_ook_sequence(u8* seq, u8 len, u8 configtype) {

	u8 *data=seq;
	u16 int_state;
	u8 c = len;

    // Prepare radio for RF communication
    open_radio();
    
    SetupRadio433_OOK(configtype);
	
	display_symbol(LCD_ICON_BEEPER1, SEG_ON_BLINK_ON);
    display_symbol(LCD_ICON_BEEPER2, SEG_ON_BLINK_ON);
    display_symbol(LCD_ICON_BEEPER3, SEG_ON_BLINK_ON);
	
	// Debounce button event
    Timer0_A4_Delay(CONV_MS_TO_TICKS(BUTTONS_DEBOUNCE_TIME_OUT));
	
	
	ENTER_CRITICAL_SECTION(int_state);
	
	Strobe(RF_SFTX);
	
    /* Wait for radio to be ready for next instruction */
    while (!(RF1AIFCTL1 & RFINSTRIFG)) ;

    /* Write cmd: TXFIFOWR */
    RF1AINSTRB = 0x7F;

    // NOTE: sent three time
    do
    {
        /* Wait for radio to be ready to accept the data */
		while (!(RF1AIFCTL1 & RFDINIFG)) ;

        /* Write one byte to FIFO */
        RF1ADINB   = *data;

        data++;
        c--;

    } while (c);
    c=len; data=seq;
    do
    {
        /* Wait for radio to be ready to accept the data */
		while (!(RF1AIFCTL1 & RFDINIFG)) ;

        /* Write one byte to FIFO */
        RF1ADINB   = *data;

        data++;
        c--;

    } while (c);
    c=len; data=seq;
    do
    {
        /* Wait for radio to be ready to accept the data */
		while (!(RF1AIFCTL1 & RFDINIFG)) ;

        /* Write one byte to FIFO */
        RF1ADINB   = *seq;

        seq++;
        c--;

    } while (c);
    
    Strobe(RF_STX);
    
    // wait for interrupt
    while(!(RF1AIFG & BIT9));
    // clear interrupt
    (RF1AIFG &= BIT9);
    
	EXIT_CRITICAL_SECTION(int_state);


    // Clear icons
    display_symbol(LCD_ICON_BEEPER1, SEG_OFF_BLINK_OFF);
    display_symbol(LCD_ICON_BEEPER2, SEG_OFF_BLINK_OFF);
    display_symbol(LCD_ICON_BEEPER3, SEG_OFF_BLINK_OFF);


    // Powerdown radio
    close_radio();
    
    //display_chars(LCD_SEG_L2_5_0, (u8 *) " A.OFF", SEG_ON);

}

