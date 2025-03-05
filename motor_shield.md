# Motor shield

## Functions

- `set_speed`

## How does the shield work (in combination with the library)

The shield can control four different DC motors. Each motor has a PWM pin, that controls how much voltage is going to the motors. 
To decide which motors are turning which direction a bit register is used. Each motor has 2 connectors, (I will call them a + and - for this explanation). In the bit register every bit is assigned to a plus or minus for each motor. So if M1+ is on and M1- is off, M1 is going to be turned on forwards. 

> [!NOTE]
> This is just what I conclude after observations, it is possible this isn't exactly what is happening.