/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 LeafLabs LLC.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @file Wire.h
 * @author Trystan Jones <crenn6977@gmail.com>
 * @brief Wire library, uses the WireBase to create the primary interface
 *        while keeping low level interactions invisible to the user.
 */

/*
 * Library updated by crenn to follow new Wire system.
 * Code was derived from the original Wire for maple code by leaflabs and the
 * modifications by gke and ala42.
 */

#ifndef _WIRE_H_
#define _WIRE_H_

#include "WireBase.h"

/*
 * On the Maple, let the default pins be in the same location as the Arduino
 * pins
 */

#define SOFT_STANDARD 27
#define SOFT_FAST 0

#define HIGH true
#define LOW  false

#define I2C_DELAY(x) do{for(int i=0;i<x;i++) {asm volatile("nop");}}while(0)

#define BUFFER_LENGTH 64


class TwoWire : public WireBase {
 public:
    uint8_t 		i2c_delay;

	void set_scl(bool);
	void set_sda(bool);

	bool sda_read();

    /*
     * Creates a Start condition on the bus 在总线上创建启动条件
     */
    void i2c_start();

    /*
     * Creates a Stop condition on the bus 在总线上创建停止条件
     */
    void  i2c_stop();

    /*
     * Gets an ACK condition from a slave device on the bus 从总线上的从属设备获取ACK条件
     */
    bool i2c_get_ack();

    /*
     * Creates a ACK condition on the bus 在总线上创建ACK条件
     */
    void i2c_send_ack();

    /*
     * Creates a NACK condition on the bus  在总线上创建一个NACK条件
     */
    void i2c_send_nack();

    /*
     * Shifts in the data through SDA and clocks SCL for the slave device 通过SDA转换数据，并为从设备时钟SCL
     */
    uint8_t i2c_shift_in();

    /*
     * Shifts out the data through SDA and clocks SCL for the slave device 通过SDA输出数据，并为从设备设置时钟SCL
     */
    void i2c_shift_out(uint8_t);
 protected:
    /*
     * Processes the incoming I2C message defined by WireBase 处理由WireBase定义的传入I2C消息
     */
    uint8_t process();
 public:
	TwoWire(uint8_t delay=SOFT_FAST);

    /*
     * Sets pins SDA and SCL to OUPTUT_OPEN_DRAIN, joining I2C bus as
     * master. This function overwrites the default behaviour of
     * .begin(uint8) in WireBase 将引脚SDA和SCL设置为OUPTUT_OPEN_DRAIN，
        以主控身份加入I2C总线。这个函数覆盖了WireBase中.begin(uint8)的默认行为
     */
    void begin(uint8_t = 0x00);

    /*
     * If object is destroyed, set pin numbers to 0. 如果对象被销毁，将pin码设置为0。
     */
    ~TwoWire();
};


#endif // _WIRE_H_
