//============================================================
// LibAnaComp-noInterrupt.
//
// Breadboard:
//
// D13 is not used in this sketch. If you are using the 
// breadboard layout for all the examples, just ignore D13.
//
// D8 is connected to a 330R resistor, the other end of which 
// is connected to a green LED's anode. The cathode connects 
// to GND.
//
// D6 is connected directly to the Arduino's 3.3V port on the
// header socket.
//
// D7 is connected to the wiper, middle terminal, of a 10K 
// potentiometer. The value isn't important though. The other
// two terminals are connected to 5V and GND.
//
// Description:
//
// When the potentiometer is adjusted the green LED will turn
// on and off according to the value of the ACO bit in the
// ACSR register. If the voltage on AIN0/D6 is higher than 
// that on AIN1/D7 the ACO bit will be set, and the green LED
// will light. Otherwise, the green LED will go out.
//============================================================


//------------------------------------------------------------
// We need this for the libAnaComp library.
//------------------------------------------------------------
#include "libanacomp.h"

//------------------------------------------------------------
// The LED on pin D8 is a green one and is controlled by the 
// Analog Comparator's ACO bit.
//------------------------------------------------------------
const uint8_t greenLED = 8;

//------------------------------------------------------------
// The LED pin is an output. Then initialise the Analog 
// Comparator, without interrupts. The AC is then started.
//------------------------------------------------------------
void setup() {
    pinMode(greenLED, OUTPUT);
    acBegin(acAIN0_D6, acAIN1_D7, acNONE);
}

//------------------------------------------------------------
// Each time through the loop, check if the state of the ACO 
// bit in the ACSR register and set the green LED to reflect 
// that state. Must clear ACI interrupt flag as we are not
// using interrupts.
//------------------------------------------------------------
void loop() {
    digitalWrite(8, !!(ACSR & (1 << ACO)));
    ACSR |= (1 << ACI);
}


