# Project 2, maze solver

A maze solver, and catch robot car

```txt
|--📂 maze_car              // Folder with al the file the arduino scetch uses
|  |-- maze_car.ino         // The main sketch that is to be uploaded to the microcontroller
|  |-- motor_shield.h       // Headerfile of the library motor_shield
|  |-- motor_shield.cpp     // Library that contains functions to make the motor shield work
|
|-- README.md               // Project description
|-- .gitignore              // Git ingore file
```

## Libraries

### motor_shield

This is a library we wrote to use with the `L293D Motor Shield Module`.

## planning

1. Componenten
    1. Motor -> library schrijven
    2. Sensoren -> ff uitzoeken hoe ze werken
    3. Achterlicht -> library uitzoeken
2. Algoritmes
    1. Niet tegen muur aan rijden
    2. Doolhof ding
    3. Tikkertje vluchten
    4. Tikkertje jagen
