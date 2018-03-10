//=============================================================================
// *** Nunchuk64 ***
// Copyright (c) Robert Grasböck, All rights reserved.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//=============================================================================
/// @file   controller.c
/// @author Robert Grasböck (robert.grasboeck@gmail.com)
/// @date   December, 2017
/// @brief  i2c controller
//=============================================================================
#include <string.h>

#include <util/delay.h>

#include "i2c_master.h"
#include "controller.h"

#define CONTROLLER_ADDR (0x52<<1) ///< device address

uint8_t controller_init(void) {
  // --------------------
  // send 0x55 to register 0xf0
  i2c_start(CONTROLLER_ADDR | I2C_WRITE);
  i2c_write(0xf0);
  i2c_write(0x55);
  i2c_stop();
  // --------------------

  // --------------------
  // send 0x00 to register 0xfb
  i2c_start(CONTROLLER_ADDR | I2C_WRITE);
  i2c_write(0xfb);
  i2c_write(0x00);
  i2c_stop();
  // --------------------

  return 0;
}

static void controller_disable_encryption(void) {
  // --------------------
  // send 0xaa to register 0xf0
  i2c_start_wait(CONTROLLER_ADDR | I2C_WRITE);
  i2c_write(0xf0);
  i2c_write(0xaa);
  i2c_stop();
  // --------------------

  _delay_ms(1);

  // --------------------
  // send 6 zero bytes to register 0x40
  i2c_start_wait(CONTROLLER_ADDR | I2C_WRITE);
  i2c_write(0x40);

  for (uint8_t i = 0; i < 6; i++)
    i2c_write(0x00);

  i2c_stop();
  // --------------------

  _delay_ms(1);

  // --------------------
  // send 6 zero bytes to register 0x40
  i2c_start_wait(CONTROLLER_ADDR | I2C_WRITE);
  i2c_write(0x40);

  for (uint8_t i = 0; i < 6; i++)
    i2c_write(0x00);

  i2c_stop();
  // --------------------

  _delay_ms(1);

  // --------------------
  // send 4 zero bytes to register 0x40
  i2c_start_wait(CONTROLLER_ADDR | I2C_WRITE);
  i2c_write(0x40);

  for (uint8_t i = 0; i < 4; i++)
    i2c_write(0x00);

  i2c_stop();
  // --------------------

  _delay_ms(1);
}

uint8_t controller_read(ContollerData *cd) {

  _delay_ms(1);

  // --------------------
  // read 6 bytes
  if (i2c_start(CONTROLLER_ADDR | I2C_READ) != 0) {
    return 1;
  }

  uint8_t i = 0;

  for (i = 0; i < 5; i++) {
    cd->byte[i] = i2c_readAck(); // i2c_read(I2C_ACK);
  }

  cd->byte[i] = (i2c_readNak()); // i2c_read(I2C_NOACK);
  i2c_stop();
  // --------------------

  // --------------------
  // send read request to 0x00 register
  // for the next bytes!!!!
  // NOTE this is very important for original Nintendo Controller
  i2c_start(CONTROLLER_ADDR | I2C_WRITE);
  i2c_write(0x00);
  i2c_stop();
  // --------------------

  return 0;
}

const uint8_t ID_MAP[MAX_IDs][6] = {
  {0x00, 0x00, 0xa4, 0x20, 0x00, 0x00}, // ID_Nunchuck
  {0x00, 0x00, 0xa4, 0x20, 0x01, 0x01}, // ID_Classic
  {0x01, 0x00, 0xa4, 0x20, 0x01, 0x01}, // ID_Wii_Classic_Pro
  {0x01, 0x00, 0xa4, 0x20, 0x01, 0x01}  // ID_NES_Classic_Mini_Clone
};

ControllerID get_id(void) {
  // --------------------
  // send read request to 0xfa register
  i2c_start(CONTROLLER_ADDR | I2C_WRITE);
  i2c_write(0xFA);
  i2c_stop();
  // --------------------

  // --------------------
  // read 6 bytes
  if (i2c_start(CONTROLLER_ADDR | I2C_READ) != 0) {
    return MAX_IDs; // if controller is not responsing, return MAX_IDs
  }

  uint8_t i = 0;
  uint8_t id[6];

  for (i = 0; i < 5; i++) {
    id[i] = i2c_readAck(); // i2c_read(I2C_ACK);
  }

  id[i] = (i2c_readNak()); // i2c_read(I2C_NOACK);
  i2c_stop();
  // --------------------

  // --------------------
  // compare the 6 bytes with known IDs
  for (uint8_t i = 0; i < MAX_IDs; i++) {

    // known controller found?
    if (memcmp(&ID_MAP[i][0], &id[0], 6) == 0) {

      // HACK because fucking china clones use the same ID
      if (i == ID_Wii_Classic_Pro) {
        ContollerData data;
        controller_read(&data);

        if (data.byte[4] == 0x00 && data.byte[5] == 0x00) {
          controller_disable_encryption();
          return ID_NES_Classic_Mini_Clone;
        }
      }

      return i;
    }
  }
  // --------------------

  return MAX_IDs; // no known controller found, return MAX_IDs
}
