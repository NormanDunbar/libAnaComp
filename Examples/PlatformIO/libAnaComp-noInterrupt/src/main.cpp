//============================================================
// LibAnaComp-noInterrupt.
//
// Breadboard:
//
// PB5 is not used in this sketch. If you are using the 
// breadboard layout for all the examples, just ignore PB5.
//
// PB0 is connected to a 330R resistor, the other end of which 
// is connected to a green LED's anode. The cathode connects 
// to GND.
//
// PD6 is connected directly to the Arduino's 3.3V port on the
// header socket.
//
// PD7 is connected to the wiper, middle terminal, of a 10K 
// potentiometer. The value isn't important though. The other
// two terminals are connected to 5V and GND.
//
// Description:
//
// When the potentiometer is adjusted the green LED will turn
// on and off according to the value of the ACO bit in the
// ACSR register. If the voltage on AIN0/PD6 is higher than 
// that on AIN1/PD7 the ACO bit will be set, and the green LED
// will light. Otherwise, the green LED will go out.
//============================================================

//------------------------------------------------------------
// These headers are required to define the registers and bit
// names for our board.
//------------------------------------------------------------
#include <avr/io.h>

//------------------------------------------------------------
// We need this for the libAnaComp library.
//------------------------------------------------------------
#include "libanacomp.h"

//------------------------------------------------------------
// The LED on pin PB0 is a green one and is controlled by the 
// Analog Comparator's ACO bit.
//------------------------------------------------------------
const uint8_t greenLED = PORTB0;

//------------------------------------------------------------
// The LED pin is an output. Then initialise the Analog 
// Comparator, without interrupts. The AC is then started.
//------------------------------------------------------------
int main() {
    DDRB |= (1 << greenLED);
    acBegin(acAIN0_D6, acAIN1_D7, acNONE);

//------------------------------------------------------------
// Each time through the loop, check if the state of the ACO 
// bit in the ACSR register and set the green LED to reflect 
// that state. Must clear ACI interrupt flag.
//------------------------------------------------------------
    while (1) {
        if (ACSR & (1 << ACO))
            PORTB |= (1 << greenLED);

        else
            PORTB &= ~(1 << greenLED);
        }
        
        ACSR |= (1 << ACI);
}

