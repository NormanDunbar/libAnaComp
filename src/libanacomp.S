//------------------------------------------------------------
// To quote the Arduino source code, a "bit of a hack", 
// however, we need it as it makes the Assembly Language code
// much much much more readable!
//------------------------------------------------------------
#define __SFR_OFFSET 0

//------------------------------------------------------------
// Get the gcc-as assembler to read in the register and bit
// settings for our Arduino board.
//------------------------------------------------------------
#include "libanacomp.h"

//------------------------------------------------------------
// We can't read in the header file typedefs as they break the 
// assembly pass. Code duplication is a bad thing! :o(
//------------------------------------------------------------
.equ acAIN1_PD7,8
.equ isrBit,7

//------------------------------------------------------------
// From here onwards, everything goes into the code segment
// which is cunningly named ".text" and not ".code". Hmmm.
//------------------------------------------------------------
.section .text

//------------------------------------------------------------
// Ensure our library functions can be seen from the C++ code
// in sketches.
//------------------------------------------------------------
.global acBegin
.global acDisable


//------------------------------------------------------------
// Disable the AC interrupt and leave R18 holding the ACSR
// value just written back.
//------------------------------------------------------------
acIntOff:
    in r18,ACSR             // Get current ACSR value.
    cbr r18,(1 << ACIE)     // Clear interrupt enable bit.
    out ACSR,r18            // Disable AC interrupt.
    ret                     // Return with R18 = new ACSR.


//------------------------------------------------------------
// Initialises the analog comparator and activates it.
// Disables GPIO on pins D6 and D7 if they are selected as 
// AINx pins.
//
// Parameters:
// -- R24:R25 = AIN0 selector, D6 or 1v1. R25 = 0.
// -- R22:R23 = AIN1 selector, D7 or A0-A7. R23 = 0.
// -- R20:R21 = Interrupts on/off & trigger. R21 = 0.
//
// The initial value in R18, with bits ACD and ACI set, will:
// -- Disable the AC, as ACD is set.
// -- Clear the AC interrupt flag, as ACI is set.
// -- Disables the AC interrupt, as ACIE is clear.
// -- Set AIN0/D6 as the reference voltage, as ACBG is clear.
//
// Then:
// -- ADCSRB.ACME is always set to 1.
// -- Write ADCSRB config.
//
// -- If required, enable interrupt, ACIE = 1
//                 ACIS1:0 = R20 for trigger.
//
// -- To set D6 as AIN0, ACSR.ACBG = 0,
//                       DIDR1.AIN0D = 1.
//
// -- To set 1v1 as AIN0, ACSR.ACBG = 1.
// -- Write ACSR config.
//
// -- To set D7 as AIN1, ADCSRA.ADEN = 1,
//                       DIDR1.AIN1D = 1.
// -- Write DIDR1 config.
//
// -- To set A0-A7 as AIN1, ADCSRA.ADEN = 0,
//                          ADMUX,MUX2:0 = An, n = 0 to 7.
// -- Write ADCSRB and ADMUX config.
//------------------------------------------------------------
// Working registers:
// -- R18 = Configures ACSR, ADCSRA and ADCSRB.
// -- R19 = Configures DIDR1.
// -- R20 = Configures ADMUX.
//------------------------------------------------------------
acBegin:
    //--------------------------------------------------------
    // Disable AC and clear pending interrupts.
    //--------------------------------------------------------
    ldi r18,(1 << ACD)|(1 << ACI)
    out ACSR,r18            // Disable AC for configuration

    //--------------------------------------------------------
    // ADCSRB.ACME is always 1 regardless of AIN1 pin.
    //--------------------------------------------------------
    lds r18,ADCSRB          // Current ADCSRB value.
    sbr r18,(1 << ACME)     // Always set this bit.
    sts ADCSRB,r18          // Write config to ADCSRB.

    //--------------------------------------------------------
    // Interrupt required?
    //--------------------------------------------------------
    clr r21                 // Use R21 for ACSR config.
                            // ACD = 0, AC will be enabled.
    sbrc r20,isrBit         // Interrupt on if R20.7 clear.
    rjmp acReference        // Skip if no interrupt.

    //--------------------------------------------------------
    // Interrupt is on.
    //--------------------------------------------------------
    sbr r21,(1 << ACIE)     // Enable interrupt.
    or r21,r20              // And set interrupt trigger.

acReference:
    //--------------------------------------------------------
    // Reference voltage.
    //--------------------------------------------------------
    lds r19,DIDR1           // In case we use D6/D7 pins.

    or r21,r24              // D6/1v1 for reference voltage.
    sbrs r21,ACBG           // ACBG = 0, AIN0 = D6.
    sbr r19,(1 << AIN0D)    // Need to disable D6 GPIO.

    //out ACSR,r21            // Write config to ACSR.

    //--------------------------------------------------------
    // Sample voltage.
    //--------------------------------------------------------
    lds r18,ADCSRA          // Current ADCSRA value.

    //--------------------------------------------------------
    // D7 as AIN1?
    //--------------------------------------------------------
    cpi r22,acAIN1_PD7      // Check for AIN1 = D7.
    brne acSetupA0_A7       // Nope!

acSetupD7:
    //--------------------------------------------------------
    // D7 is indeed AIN1.
    //--------------------------------------------------------
    sbr r18,(1 << ADEN)     // Set D7 as AIN1.
    sts ADCSRA,r18          // Write config to ADCSRA.

    //--------------------------------------------------------
    // Disable GPIO for D7/PD7
    //--------------------------------------------------------
    sbr r19,(1 << AIN1D)    // Need to disable D7 GPIO. 
    sts DIDR1,r18           // Write config to DIDR1.
    rjmp acWriteACSR        // Finished configuration.

acSetupA0_A7:
    //--------------------------------------------------------
    // One of A0 through A7 is the AIN1 pin chosen, not D7.
    //--------------------------------------------------------
    cbr r18,(1 << ADEN)     // AIN1 must be A0-A7.
    sts ADCSRA,r18          // Configure D7 or A0-A7.
    lds r18,ADMUX           // Current ADMUX value.
    or r18,r22              // Select A0-A7 as AIN1.
    sts ADMUX,r18           // Write config to ADMUX.

acWriteACSR:
    out ACSR,r21            // Write config to ACSR & enable.
    ret


//------------------------------------------------------------
// Disable the analog comparator and enable GPIO on pins
// D6 and D7 if one or both were disabled.
//------------------------------------------------------------
acDisable:
    rcall acIntOff          // Disable interrupt, gets ACSR
                            // value into R18.

    //--------------------------------------------------------
    // Disable the AC and clear pending interrupt flag.
    //--------------------------------------------------------
    sbr r18,((1 << ACD)|(1 << ACI))
    out ACSR,r18            // Disable Analog comparator   .         

    //--------------------------------------------------------
    // Re-enable GPIO on D6 and/or D7 as necessary.
    //--------------------------------------------------------
    lds r18,DIDR1           // Current DIDR1 value.
    sbrs r18,(1 << AIN0D)   // Don't change if set.
    cbr r18,(1 << AIN0D)
    sbrs r18,(1 << AIN1D)   // Don't change if set.
    cbr r18,(1 << AIN1D)
    sts ACSR,r18            // Enable GPIO for D6/D7.
    ret
