# Supported Controllers

This is an overview of all tested controllers.
Since they all mostly use the same datasets, it is very likely that other controllers will work too.
Pressing the pushbutton switches the Nunchuk64 to different modes.

- Mode OFF ... LED is OFF
- Mode ON  ... LED is ON (same as mode OFF, but exchanges Control Port 1 and Port 2)
- Mode F1  ... LED flashes 1 time
- Mode F2  ... LED flashes 2 times
- Mode F3  ... LED flashes 3 times

Depending on the mode the controllers behaves differently.
Nunchuk64 can perform these different controls on each individual C64 Control Port:

- UP
- DOWN
- LEFT
- RIGHT
- FIRE
- AUTOFIRE
- PADDLE X
- PADDLE Y

An extra function is SPACE, which triggers FIRE on controller port 1.
This is very handy because you can use it to press the space bar.

## NES Classic Mini Clone
Wireless Controller for Nintendo NES Mini Classic Edition.

Seen here: https://www.ebay.com/itm/272765627224
> Button A+ and B+ are same as A and B but with autofire

![NES Classic Mini Clone](nes_classic_mini_clone.jpg)

DRIVER: driver_nes_classic.c

| Item          |Mode OFF  |Mode ON   |Mode F1   |Mode F2   |Mode F3   |
| --------------|----------|----------|----------|----------|----------|
| Joystick UP   |UP        |UP        |          |          |          |
| Joystick DOWN |DOWN      |DOWN      |          |          |          |
| Joystick LEFT |LEFT      |LEFT      |          |          |          |
| Joystick RIGHT|RIGHT     |RIGHT     |          |          |          |
| A             |UP        |UP        |          |          |          |
| B             |FIRE      |FIRE      |          |          |          |
| A+            |UP        |UP        |          |          |          |
| B+            |FIRE      |FIRE      |          |          |          |
| SELECT        |FIRE      |FIRE      |          |          |          |
| START         |SPACE     |SPACE     |          |          |          |


## SNES Classic Mini Clone
Wireless Controller for Nintendo SNES Mini Classic Edition.

Seen here: https://www.ebay.com/itm/162874713501

![SNES Classic Mini Clone](snes_classic_mini_clone.jpg)

Wired Controller for Nintendo SNES Mini Classic Edition.

Seen here: https://www.ebay.at/itm/222674575659

![SNES Classic Mini Clone](snes_classic_mini_clone_wired.jpg)

DRIVER: driver_wii_classic.c

| Item          |Mode OFF  |Mode ON   |Mode F1   |Mode F2   |Mode F3   |
| --------------|----------|----------|----------|----------|----------|
| Joystick UP   |UP        |UP        |          |          |          |
| Joystick DOWN |DOWN      |DOWN      |          |          |          |
| Joystick LEFT |LEFT      |LEFT      |          |          |          |
| Joystick RIGHT|RIGHT     |RIGHT     |          |          |          |
| A             |UP        |UP        |          |          |          |
| B             |FIRE      |FIRE      |          |          |          |
| X             |FIRE      |FIRE      |          |          |          |
| Y             |AUTOFIRE  |AUTOFIRE  |          |          |          |
| SELECT        |FIRE      |FIRE      |          |          |          |
| START         |SPACE     |SPACE     |          |          |          |
| L             |LEFT      |LEFT      |          |          |          |
| R             |RIGHT     |RIGHT     |          |          |          |

## Wii classic controller
Wii Classic Controller for Nintendo Wii Remote. (Wired)

Seen here: https://www.ebay.com/itm/291645811767

![Wii Classic](wii_classic.jpg)

DRIVER: driver_wii_classic.c

| Item          |Mode OFF  |Mode ON   |Mode F1   |Mode F2   |Mode F3   |
| --------------|----------|----------|----------|----------|----------|
| Joystick UP   |UP        |UP        |          |          |          |
| Joystick DOWN |DOWN      |DOWN      |          |          |          |
| Joystick LEFT |LEFT      |LEFT      |          |          |          |
| Joystick RIGHT|RIGHT     |RIGHT     |          |          |          |
| A             |UP        |UP        |          |          |          |
| B             |FIRE      |FIRE      |          |          |          |
| X             |FIRE      |FIRE      |          |          |          |
| Y             |AUTOFIRE  |AUTOFIRE  |          |          |          |
| SELECT        |FIRE      |FIRE      |          |          |          |
| HOME          |-         |-         |          |          |          |
| START         |SPACE     |SPACE     |          |          |          |
| L             |LEFT      |LEFT      |          |          |          |
| ZL            |-         |-         |          |          |          |
| R             |RIGHT     |RIGHT     |          |          |          |
| ZR            |-         |-         |          |          |          |

## Wii Nunchuck
Wii Nunchuck Controller for Nintendo Wii Remote

![Wii Nunchuk](wii_nunchuk.jpg)

DRIVER: driver_nunchuk.c

| Item          |Mode OFF  |Mode ON   |Mode F1   |Mode F2   |Mode F3   |
| --------------|----------|----------|----------|----------|----------|
| Analog Joy X  |LEFT/RIGHT|LEFT/RIGHT|-         |-         |          |
| Analog Joy Y  |UP/DOWN   |UP/DOWN   |-         |-         |          |
| Accel X       |-         |-         |LEFT/RIGHT|LEFT/RIGHT|          |
| Accel Y       |-         |-         |UP/DOWN   |UP/DOWN   |          |
| Accel Z       |-         |-         |-         |-         |          |
| C             |AUTOFIRE  |AUTOFIRE  |AUTOFIRE  |AUTOFIRE  |          |
| Z             |FIRE      |FIRE      |FIRE      |FIRE      |          |
