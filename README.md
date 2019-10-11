# PT-Symmetry-Controller

PT-Symmetric Quantum Theory is a non-Hermitian area of physics whose study has relatively recently gained a significant amount of popularity. The interplay between energy additions and subtractions to and from a system can lead to features that present new and interesting research topics.

This mechanical PT-Symmetry-Controller project is an analog to a quantum mechanical PT-symmetric system. It consists of two pendulums that are energetically coupled together by a string that connects their pivot points. The amount of energy that can be transferred between the pendulums is determined by the tautness of the coupling string.

One of these pendulums has energy put into its swing while the other pendulum has energy taken out of its swing. This is done by pulsing the pendulum's corresponding solenoid at specific times such that a magnet, which is attached to the string of each pendulum, has a force exerted on it. The driven pendulum has its corresponding solenoid pulsed when it is swinging towards its solenoid, while the dampened pendulum has its corresponding solenoid pulsed when it is swinging away from its solenoid.

Each pendulum has a photogate that assists with determining its swing direction. Each photogate is placed slightly offset from the resting position of the corresponding pendulum. This allows for two asymmetrical partial periods to be determined. This information is then used by the computer to determine when to pulse the corresponding solenoid. The two photogates interface with a LabQuest 2, which needs to be connected to the computer that is running the program.

Finally, a custom circuit is used to help pulse the solenoids. The computer program communicates with a microcontroller using a USB to MIDI connection. Based on the MIDI message that is sent to the microcontroller, and with the help of high amperage transistors, current can be sent to and cut off from each corresponding solenoid.

With this entire setup, one can setup either a broken (insufficient coupling), or an unbroken (sufficient coupling) PT-symmetric mechanical system.

## Requirements for...

### Electronics

- Any PDF reader to view the PDF of the schematic.

- [Autodesk Eagle](https://www.autodesk.com/products/eagle/free-download) >= 9.5 to modify the schematic, board layout, and/or Gerber files.

### Firmware

#### Software

- [MPLAB X IDE](https://www.microchip.com/mplab/mplab-x-ide) >= 5.25 to modify and develop the firmware.

- MPLAB X IPE >= 5.25 to program the PIC microcontrollers.

  - While installing the IDE, you are presented with the choice of also installed the IPE.
  
  - The versions of MPLAB X IDE and MPLAB X IPE should be the same.
  
#### Hardware

- [PICkit 4 In-Circuit Debugger/Programmer](https://www.microchip.com/developmenttools/ProductDetails/PG164140) to flash the PIC microcontrollers with new firmware.

### LabQuest 2 Software

- [Microsoft Visual Studio IDE](https://visualstudio.microsoft.com/) >= 2019

Using the Visual Studio Installer that comes with the IDE install the "Desktop development with C++" workload:
  
#### Hardware

- [LabQuest 2](https://www.vernier.com/products/interfaces/labq2/)

- 2 [Vernier Photogates](https://www.vernier.com/products/sensors/photogates/vpg-btd/)

### PT-Symmetry-Controller Software

#### Software

- Both [OpenJDK](https://adoptopenjdk.net/) and [OpenJFX](https://gluonhq.com/products/javafx/) >= 11 to run and develop the software.
  - The versions of OpenJDK and OpenJFX should be the same.
  
- [Intellij IDEA](https://www.jetbrains.com/idea/download/) >= 2019.2 (recommended, but not necessary).

#### Hardware 

- [USB to MIDI cable](https://www.amazon.com/HDE-Synthesizer-Microphone-Instrument-Converter/dp/B00D3QFHN8/)
