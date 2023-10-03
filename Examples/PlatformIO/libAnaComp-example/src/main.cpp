//============================================================
// LibAnaComp-Example.
//============================================================
//
//------------------------------------------------------------
// Breadboard Layout:
//------------------------------------------------------------
// D13 is connected to a 330R resistor, the other end of which 
// is connected to a red LED's anode. The cathode connects to 
// GND.
//
// D8 is connected to a 330R resistor, the other end of which 
// is connected to a green LED's anode. The cathode connects 
// to GND.
//
// D6 is connected directly to the Arduino's 3.3V pin on the
// header socket.
//
// D7 is connected to the wiper (middle terminal) of a 10K 
// potentiometer. The value isn't important though. The other
// two terminals are connected to 5V and GND, either way
// around, it makes little difference.
//-----------------------------------------------------------
// Description:
//-----------------------------------------------------------
// When the potentiometer is adjusted the green LED will turn
// on and off according to the value of the ACO bit in the
// ACSR register. If the voltage on AIN0/D6 is higher than 
// that on AIN1/D7 the ACO bit will be set, and the green LED
// will light. Otherwise, the green LED will go out.
//
// The interrupt for the Analog Comparator is configured to
// trigger the ISR whenever the ACO bit changes from 0 to 1 or
// from 1 or 0.
//
// Because this sketch uses the AC interrupt, the green LED is
// always responsive to the turning of the potentiometer. If
// we were reading the ACSR.ACO bit in the loop, without the
// interrupt, it would be very difficult to get the LED to 
// toggle correctly. Don't believe me? Try it!
//============================================================


//------------------------------------------------------------
// These headers are required to define the registers and bit
// names for our board; to enable interrupt handling; and to 
// define the AVRlib delay functions. 
//------------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

//------------------------------------------------------------
// We need this for the libAnaComp library.
//------------------------------------------------------------
#include "libanacomp.h"

//------------------------------------------------------------
// A constant to define how long we delay between toggles of 
// the red LED on D13.
//------------------------------------------------------------
const uint16_t redDelay = 2000;

//------------------------------------------------------------
// The two LEDs are a red one on D13 which simply blinks every
// two seconds; and a green one on D8 controlled by the Analog
// Comparator interrupt.
//------------------------------------------------------------
const uint8_t greenLED = PORTB0;            // D8
const uint8_t redLED = PORTB5;              // D13

//------------------------------------------------------------
// The two LEDs, pins PB0 and PB5, are outputs. Initialise
// the Analog Comparator, set it into interrupt mode with the 
// ISR executed every time the ACO bit in the ACSR register
// toggles. The AC is then started.
//------------------------------------------------------------
int main() {
    DDRB |= (1 << redLED)|(1 << greenLED);
    acBegin(acAIN0_D6, acAIN1_D7, acTOGGLE);
    sei();


//------------------------------------------------------------
// Each time through the loop flash the red LED on D13.
//------------------------------------------------------------
    while (1) {
        // Toggles the redLED every 'redDelay' milliseconds.
        PORTB |= (1 << redLED);
        _delay_ms(redDelay);
        PORTB &= ~(1 << redLED);
        _delay_ms(redDelay);
    }
}


//------------------------------------------------------------
// The ISR for the Analog Comparator sets the LED on PB0 to
// the same state as the ACO bit in the ACSR register.
//------------------------------------------------------------
ISR(ANALOG_COMP_vect) {
    if (ACSR & (1 << ACO))
        PORTB |= (1 << greenLED);
    else
        PORTB &= ~(1 << greenLED);
}
