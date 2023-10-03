# libAnaComp
An Arduino (or PlatformIO) library to allow simple access to the ATmega168/328 Analog(ue) Comparator.

I wrote this as part of a new book I'm writing and decided to set it free into the wide world, as it shouldn't be kept in captivity! 

Seriously, this was written as part of a new book, that much is true, and I've decided to make it available to anyone who finds it and wants it.

The library will work on an ATmega168 or ATmega328 board, and *should* be suitable for others which use the same register names for the Analog(ue) Comparator. It was written to explain how to pass parameters from Arduino (or AVR) C++ using the Arduino IDE or the excellent [PlatformIO System](https://platformio.org "https://platformio.org") -- *disclaimer*, I'm a moderator on the forums -- or any other development system for the Arduino and/or AVR C/C++.

The code is written in AVR Assembly Language, and the registers used are:

* DIDR1, bits AIN0D and AIN1D.
* ADCSRA, bit ADEN.
* ADCSRB, bit ACME.
* ADMUX, bits MUX2, MUX1 and MUX0.
* ACSR, bits ACD, ACI, ACIE, ACBG, ACIS0 and ACIS1.

## Installation

### The Arduino IDE

* Sketch->Include Library->Add Zip Library.
* Navigate to where you downloaded the zip file.
* Double-click the zip file.
* Done.

Alternatively:

* Unzip the downloaded zip file.
* Copy the directory, `libanacomp` and all of its contents, into the `libraries` folder within your Arduino Sketchbook Location.

Your sketchbook location is found under File->Preferences->Sketchbook Location, and is *usually* your $HOME/Arduino, or similar on Windows.

### PlatformIO

To install globally, sort of:

* Create a folder, name it something meaningful. Mine is `platformio_libraries`.
* Create a folder, `libanacomp` within the above.
* Unzip the downloaded zip file.
* Copy the contents on the `src` file, `libanacomp.h` and `libanacomp.S` to the newly creates `libanacomp` folder.
* In your projects, which need to use the library, add the following to your `platformio.ini` file:
  ```
  lib_extra_dirs = /path/to/platformio_libraries
  ```

To install for each individual project:

Installing globally, even as described above, is somewhat frowned upon these days, so:

* Create a project in the usual manner.
* Edit the `platformio.ini` file and add:
  ```
  lib_deps = https://github.com/NormanDunbar/libanacomp.git
  ```
* Compile your program. The latest version of the library will be downloaded and built into your project. See [PlatformIO Docs](https://docs.platformio.org/en/latest/projectconf/sections/env/options/library/lib_deps.html#lib-deps "https://docs.platformio.org/en/latest/projectconf/sections/env/options/library/lib_deps.html#lib-deps") for details of the `lib_deps` parameter settings.

## Examples

There are two supplied examples, well 4, two for the Arduino and 2 for PlatformIO/AVR C++. These are:

* `LibAnaComp-example` which uses the Analog(ue) Comparator's interrupt to turn an LED on and off accordingly. There is another LED on the Arduino pin D13, aka PORTB5, which flashes every 2 seconds. This sketch demonstrates the ability of an interrupt routine to be called even while the Arduino is sitting in a `delay()` loop.
* LibAnaComp-noInterrupt` which uses polling in the `loop()` to determine when the LED should be toggled. As there are no interrupts here, the built in LED on D13/PORTB5 is *not* blinking. If it was, you would have a tiny, tiny, fraction of a second to get the Arduino to recognise that you had triggered the Analog(ue) Comparator. 
