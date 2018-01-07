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
#include <stdint.h>
#include <util/delay.h>

#include "i2c_master.h"
#include "controller.h"

#define CONTROLLER_ADDR (0x52<<1) //device address

uint8_t controller_init(void) {
  // ===========================================
  // init controller
  i2c_start_wait(CONTROLLER_ADDR | I2C_WRITE);
  i2c_write(0xf0);
  i2c_write(0x55);
  i2c_stop();
  _delay_ms(1);

  i2c_start_wait(CONTROLLER_ADDR | I2C_WRITE);
  i2c_write(0xfb);
  i2c_write(0x00);
  i2c_stop();
  _delay_ms(1);
  // ===========================================

  // ===========================================
  // deactivate encryption

  // send 0xf0 0xaa
  i2c_start_wait(CONTROLLER_ADDR | I2C_WRITE);
  i2c_write(0xf0);
  i2c_write(0xaa);
  i2c_stop();
  _delay_ms(1);

  // send 0x40 + 6 zero bytes
  i2c_start_wait(CONTROLLER_ADDR | I2C_WRITE);
  i2c_write(0x40);

  for (uint8_t i = 0; i < 6; i++)
    i2c_write(0x00);

  i2c_stop();
  _delay_ms(1);

  // send 0x40 + 6 zero bytes
  i2c_start_wait(CONTROLLER_ADDR | I2C_WRITE);
  i2c_write(0x40);

  for (uint8_t i = 0; i < 6; i++)
    i2c_write(0x00);

  i2c_stop();
  _delay_ms(1);

  // send 0x40 + 4 zero bytes
  i2c_start_wait(CONTROLLER_ADDR | I2C_WRITE);
  i2c_write(0x40);

  for (uint8_t i = 0; i < 4; i++)
    i2c_write(0x00);

  i2c_stop();
  _delay_ms(1);
  // ===========================================

  return 0;
}

uint8_t controller_read(ContollerData *n) {
  i2c_start_wait(CONTROLLER_ADDR | I2C_WRITE);
  i2c_write(0x00);
  i2c_stop();

  i2c_start_wait(CONTROLLER_ADDR | I2C_READ);
  n->byte3 = (i2c_readAck()); // i2c_read(I2C_ACK);
  n->byte0 = (i2c_readAck()); // i2c_read(I2C_ACK);
  n->byte1 = (i2c_readAck()); // i2c_read(I2C_ACK);
  n->byte2 = (i2c_readAck()); // i2c_read(I2C_ACK);
  n->byte4 = (i2c_readAck()); // i2c_read(I2C_ACK);
  n->byte5 = (i2c_readNak()); // i2c_read(I2C_NOACK);
  i2c_stop();

  return 0;
}
