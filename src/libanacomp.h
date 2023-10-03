#pragma once

//------------------------------------------------------------
// Header required by both assembler and C++ compiler.
//------------------------------------------------------------
#include <avr/io.h>

#ifndef __ASSEMBLER__

//------------------------------------------------------------
// Header only needed by the C++ compiler.
//------------------------------------------------------------
#include <stdint.h>

//------------------------------------------------------------
// Typedef for the reference voltage pin selector.
//------------------------------------------------------------
typedef enum acReferencePin : uint8_t {
    acAIN0_D6 = 0,
    acAIN0_PD6 = acAIN0_D6,
    acAIN0_Internal = (1 << ACBG),
    acAIN0_1v1 = acAIN0_Internal
} acReferencePin_t;


//------------------------------------------------------------
// Typedef for the sampled voltage pin selector.
//------------------------------------------------------------
typedef enum acSampleVoltage : uint8_t {
    // Arduino pin numbers
    acAIN1_A0 = 0,
    acAIN1_A1 = 1,
    acAIN1_A2 = 2,
    acAIN1_A3 = 3,
    acAIN1_A4 = 4,
    acAIN1_A5 = 5,
    acAIN1_D7 = 8,
    // AVR pin names
    acAIN1_PC0 = acAIN1_A0,
    acAIN1_PC1 = acAIN1_A1,
    acAIN1_PC2 = acAIN1_A2,
    acAIN1_PC3 = acAIN1_A3,
    acAIN1_PC4 = acAIN1_A4,
    acAIN1_PC5 = acAIN1_A5,
    acAIN1_PD7 = acAIN1_D7,
    // AVR ADC pin names, note two extras
    // for the surface mount variant.
    acAIN1_ADC0 = acAIN1_A0,
    acAIN1_ADC1 = acAIN1_A1,
    acAIN1_ADC2 = acAIN1_A2,
    acAIN1_ADC3 = acAIN1_A3,
    acAIN1_ADC4 = acAIN1_A4,
    acAIN1_ADC5 = acAIN1_A5,
    acAIN1_ADC6 = 6,
    acAIN1_ADC7 = 7
} acSamplePin_t;

//------------------------------------------------------------
// Typedef for the interrupt trigger stimulus.
// Bit 7 set = no interrupts.
//------------------------------------------------------------
typedef enum acTrigger : uint8_t {
    acTOGGLE  = 0,
    acFALLING = (1 << ACIS1),
    acRISING  = ((1 << ACIS1) | (1 << ACIS0)),
    acNONE    = (1 << 7)
} acTrigger_t;

//------------------------------------------------------------
// Function prototypes are all defined in C call convention.
//------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

    //--------------------------------------------------------
    // Configure and enable the Analog Comparator.
    //--------------------------------------------------------
    void acBegin(acReferencePin_t ref = acAIN0_PD6, 
                 acSamplePin_t samp = acAIN1_PD7,
                 acTrigger_t trigger = acNONE);

    //--------------------------------------------------------
    // Disable and reset the Analog Comparator.
    //--------------------------------------------------------
    void acDisable();


#ifdef __cplusplus
};
#endif

#endif // __ASSEMBLER__
