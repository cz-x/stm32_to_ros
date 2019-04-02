#include "mpu6050.h"

void MPU::init()
{
	Wire.begin();
	
}

void MPU::send_value(int dev_addr, uint8_t value)
{
	Wire.beginTransmission(dev_addr);
	Wire.write(value);
	Wire.endTransmission();
}

uint8_t MPU::check_id(int dev_addr, uint8_t reg_addr)
{
	Wire.beginTransmission(dev_addr);
	Wire.write(reg_addr);
	Wire.endTransmission();
	Wire.requestFrom(dev_addr, 1);
	return Wire.read();
}

void MPU::write_to_register(int dev_addr, uint8_t reg_addr, uint8_t reg_value)
{
	Wire.beginTransmission(dev_addr);
	Wire.write(reg_addr);
	Wire.write(reg_value);
	Wire.endTransmission();
}

//检查陀螺仪
bool MPU::check_gyroscope()
{


}

//读取陀螺仪数据
void MPU::measure_gyroscope()
{

}

//检查加速度计
bool MPU::check_accelerometer()
{

}

//读取加速度计数据
void MPU::measure_acceleration()
{

}