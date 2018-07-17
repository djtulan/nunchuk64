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
#include <avr/pgmspace.h>

// #include <util/delay.h>
#include "ioconfig.h"

#include "enums.h"
#include "i2c_master.h"
#include "controller.h"

#define CONTROLLER_ADDR (0x52<<1) ///< device address

void controller_init(void) {
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

  // --------------------
  // send 0x00 to register 0xfe
  i2c_start(CONTROLLER_ADDR | I2C_WRITE);
  i2c_write(0xfe);
  i2c_write(0x00);
  i2c_stop();
  // --------------------
}

static void controller_disable_encryption(void) {
  // --------------------
  // send 0xaa to register 0xf0
  i2c_start_wait(CONTROLLER_ADDR | I2C_WRITE);
  i2c_write(0xf0);
  i2c_write(0xaa);
  i2c_stop();
  // --------------------

  // --------------------
  // send 6 zero bytes to register 0x40
  i2c_start_wait(CONTROLLER_ADDR | I2C_WRITE);
  i2c_write(0x40);

  for (uint8_t i = 0; i < 6; i++)
    i2c_write(0x00);

  i2c_stop();
  // --------------------

  // --------------------
  // send 6 zero bytes to register 0x40
  i2c_start_wait(CONTROLLER_ADDR | I2C_WRITE);
  i2c_write(0x40);

  for (uint8_t i = 0; i < 6; i++)
    i2c_write(0x00);

  i2c_stop();
  // --------------------

  // --------------------
  // send 4 zero bytes to register 0x40
  i2c_start_wait(CONTROLLER_ADDR | I2C_WRITE);
  i2c_write(0x40);

  for (uint8_t i = 0; i < 4; i++)
    i2c_write(0x00);

  i2c_stop();
  // --------------------
}

uint8_t controller_read(ContollerData *cd) {
  // --------------------
  // read 6 bytes
  if (i2c_start(CONTROLLER_ADDR | I2C_READ) != 0) {
    return FALSE;
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

  return TRUE;
}


static void read_id(uint8_t id[6]) {
  // --------------------
  // send read request to 0xfa register
  i2c_start(CONTROLLER_ADDR | I2C_WRITE);
  i2c_write(0xFA);
  i2c_stop();
  // --------------------

  // --------------------
  // read 6 bytes
  if (i2c_start(CONTROLLER_ADDR | I2C_READ) != 0) {
    return; // if controller is not responsing
  }

  uint8_t i = 0;

  for (i = 0; i < 5; i++) {
    id[i] = i2c_readAck(); // i2c_read(I2C_ACK);
  }

  id[i] = (i2c_readNak()); // i2c_read(I2C_NOACK);
  i2c_stop();
  // --------------------
}

const uint8_t ID_MAP[MAX_IDs][6] PROGMEM = {
  {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, // ID_Unknown
  {0x00, 0x00, 0xa4, 0x20, 0x00, 0x00}, // ID_Nunchuck
  {0x00, 0x00, 0xa4, 0x20, 0x01, 0x01}, // ID_Wii_Classic
  {0x01, 0x00, 0xa4, 0x20, 0x01, 0x01}, // ID_Wii_Classic_Pro
  {0x01, 0x00, 0xa4, 0x20, 0x00, 0x01}, // ID_NES_Classic_Mini_Clone_Encrypted
  {0x00, 0x00, 0xa4, 0x20, 0x00, 0x01}  // ID_8Bitdo_SF30
};

ControllerID get_id(void) {
  uint8_t id[6];

  memset(id, 0, 6);
  read_id(id);

  // --------------------
  // compare the 6 bytes with known IDs
  for (uint8_t i = 0; i < MAX_IDs; i++) {

    // known controller found?
    if (memcmp_P(&id[0], &ID_MAP[i][0], 6) == 0) {

      switch (i) {
        case ID_Unknown:
          controller_init();
          read_id(id); // update id
          continue;

        case ID_Wii_Classic: {
          ContollerData data;
          controller_read(&data);

          // look if controller sends wired data (8Bitdo_SF30)
          // needs init & encryption afterwards
          if (data.byte[4] == 0x00 && data.byte[5] == 0x00) {
            controller_init();
            controller_disable_encryption();
          }
        }
        break;

        case ID_Wii_Classic_Pro: {
          ContollerData data;
          controller_read(&data);

          // look if controller sends wired data
          // NES Classic Mini Wireless Clone needs encryption & init afterwards
          if (data.byte[4] == 0x00 && data.byte[5] == 0x00) {
            controller_disable_encryption();
            controller_init();
          }
        }
        break;

        case ID_NES_Classic_Mini_Clone_Encrypted:
          controller_disable_encryption();
          controller_init();
          break;

        case ID_8Bitdo_SF30:
          controller_init();
          controller_disable_encryption();

          read_id(id);

          // if controller id has changed, then it is not a ID_8Bitdo_SF30
          // Chinese Item# JYS-NS126 has also same ID, but doesn't need encryption
          if (memcmp_P(&id[0], &ID_MAP[i][0], 6) != 0) {
            controller_init();
          }

          break;

      } // switch

      return i;
    }
  }

  // --------------------

  return MAX_IDs; // no known controller found, return MAX_IDs
}
