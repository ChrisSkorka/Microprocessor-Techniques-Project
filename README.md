# Microprocessor-Techniques-Project
Major project for microprocessor techniques. Remote controlled walking Lego robot.

##Requirements
- Phone app
  - Flutter app
  - Buttons for control
  - Bluetooth IO
  - If more time:
    - Rotating compass
    - Rotating heading selector
- External devices
  - 2xCompass sensors - determine orientation of each half
  - 1xInfrared LED - send commands to the motor controller
  - 1xBluetooth receiver - receive commands from app
- Microprocessor
  - Recieve commands from phone
  - Update heading direction
  - Motor update loop
    - Get compass readings
    - Update steering motor accordingly
    - Send IR commands
      - Requires two timers
- Lego
  - 4 legged rogot
  - Baring in the middel to turn
  - IR reciever/motor controller
  - Walking mechanisim

##Microprocessor Software design
- Interrupt timer (10Hz) - read compass and update heading
  - Read compass reading and compare to determine heading/steering
  - Calculate required steering speed
  - Set IR code to approriate code for stearing and walking speed
    - select appropriate speed for walking and steering
    - write bits to varaible
    - start update IR LED timer (below)
- Interrupt timers - update IR LED - send IR codes
  - Timer1 send one shot fixed width pulse (high)
  - Timer2 waits high time + low time and then interrupts and to process next bit
  - If end of array, LED off and stop timer
- Interrupt UART - update required heading
  - Update heading variable
  - Update walking speed variable
- Main program - setup above and then nothing

##Technical Details

###IR code transmission
Clock: 38kHz

| bit   | high cycles | low cycles |
|-------|-------------|------------|
| start | 6           | 39         |
| stop  | 6           | 39 (100 for pause between messages ) |
| low   | 6           | 10         |
| high  | 6           | 21         |
Max bits to transmit: 522

EXPERIMENTAL: the clock cycle can possible be reduced to a third, reciever should be able to interpret slighlty off timing
Clock: 12.666kHz

| bit   | high cycles | low cycles |
|-------|-------------|------------|
| start | 2           | 13         |
| stop  | 2           | 13         |
| low   | 2           | 3          |
| high  | 2           | 7          |
Max bits to transmit: 174

###IR codes

16 bit message + start/stop bit:

| start | Toggle | Escape | Channel1 | Channel1 | Address | 1 | Mode | Output | D3 | D2 | D1 | D0 | L3 | L2 | L1 | L0 | stop |
|-------|--------|--------|----------|----------|---------|---|------|-------|----|----|----|----|----|----|----|----|------|

Relevant bits that change for each transmission
Output: select motor A or B
D: Data (speed)
L: LRC (redundency check)

Each message bit is represented by 2 bits in memory

| value | bit   | high | low |
|-------|-------|------|-----|
| 00    | low   | 6    | 10  |
| 01    | high  | 6    | 21  |
| 10    | start | 6    | 39  |
| 11    | stop  | 6    | 100 |
Note the times above is measured in cycles of 38kHz
The stop bit low is 100 cycles rather than 39 for a pause between messages

###Internal IR Code Structure:
Sends two messages (motor A and B) each 16 + 2 bits
Total bits transmitted: 36 (2 x (16 + 2))
Total internal bits to represent message: 72 (2 x 36)
Bits sent: number of bits (internal representation) processed
Message ends after processing 72 bits

###UART Commands (8bit):

mode:

| value | meaning      |
|-------|--------------|
| 0     | direct mode  |
| 1     | heading mode |

Mode direct (0), phone sends whether to steer left or right:

| bit     | 7    | 6      | 5      | 4      | 3      | 2    | 1    | 0    |
|---------|------|--------|--------|--------|--------|------|------|------|
| purpose | mode | speed3 | speed2 | speed1 | speed0 | dir2 | dir1 | dir0 |

speed

| value | direction  | speed |
|-------|------------|-------|
| 0000  | stationary | 0%    |
| 0001  | forwards   | 13%   |
| 0010  | forwards   | 25%   |
| 0011  | forwards   | 38%   |
| 0100  | forwards   | 50%   |
| 0101  | forwards   | 63%   |
| 0110  | forwards   | 75%   |
| 0111  | forwards   | 88%   |
| 1110  | forwards   | 100%  |
| 1001  | backwards  | 13%   |
| 1010  | backwards  | 25%   |
| 1011  | backwards  | 38%   |
| 1100  | backwards  | 50%   |
| 1101  | backwards  | 63%   |
| 1110  | backwards  | 75%   |
| 1111  | backwards  | 88%   |
| 1110  | backwards  | 100%  |

dir - direction

| value | direction | degree |
|-------|-----------|--------|
| 000   | streight  |        |
| 001   | left      | 33%    |
| 010   | left      | 67%    |
| 011   | left      | 100%   |
| 101   | right     | 33%    |
| 110   | right     | 67%    |
| 111   | right     | 100%   |

Mode heading (1), phone sends compass heading to follow:

| bit     | 7     | 6       | 5       | 4    | 3    | 2    | 1    | 0    |
|---------|-------|---------|---------|------|------|------|------|------|
| purpose | mode  | speed1  | speed0  | dir4 | dir3 | dir2 | dir1 | dir0 |

speed

| value | direction  | speed |
|-------|------------|-------|
| 00    | stationary | 0%    |
| 01    | forwards   | 100%  |
| 11    | backwards  | 100%  |

dir - direction
heading degree = dir * (360/32)
accurate to 11.25 degrees

##Decission Making
###Determine Steering Motor Speed
Mode 0: Direct steering
- Get compass difference as percentage of max difference
- Get steering percentage
- Compute difference
- Set steering motor speed corresponding to difference

```
steering = (front_heading - back_heading) / max_steering
difference = target_steering - steering
IF absolute difference < 5%:
  motor_speed = 0%
ELSE
  motor_speed = 100% in direction of target_steering
END IF
```

Mode 1: Heading control
- Get compass difference as percentage of max difference
- Get front compass reading
- Compute heading difference between front heading reading and target heading
- If heading is not similar (with in 10 degrees)
  - If robot is not already max steering
  - Motor speed is max in required direction

```
steering = (front_heading - back_heading) / max_steering
difference = target_heading - front_heading
IF absolute difference > 10 degrees:
  IF steering close to 100% in direction of difference:
    motor_speed = 0
  ELSE
    motor_speed = max in direction of difference
  END IF
ELSE
  motor_speed = 0
END IF
```

##Pin Connections and Systems

| System         | FunctionID | Pins          | Mode        | Interrupt |
|----------------|------------|---------------|-------------|-----------|
| System Timer   | SysTick    |               | Count down  |           |
| Timer main     | T0         |               | Count down  | 0         |
| GPIO IR LED    | GPIO B     | PB6           | Out         |           |
| I2C Compass 1  | I2C0, GPIO | PB2, PB3, PB4 | Out, IO, In |           |
| I2C Compass 2  | I2C1, GPIO | PA6, PA7, PB5 | Out, IO, In |           |
| UART Bluetooth | U1         | PB0, PB1      | In, Out     | Receive   |
