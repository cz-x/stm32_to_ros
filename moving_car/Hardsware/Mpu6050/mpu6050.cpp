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

//���mpu�Ƿ����
bool MPU::check_mpu()
{
	uint16_t i, j;
	
	if( check_id(MPU6050_SLAVE_ADDRESS, MPU6050_WHO_AM_I) == 0x68 )
	{
		for ( i = 0; i <1000; i++)
			for ( j = 0; j < 1000; j++);
		
		write_to_register(MPU6050_SLAVE_ADDRESS, MPU6050_RA_PWR_MGMT_1, 0x00);		    //�������״̬
		write_to_register(MPU6050_SLAVE_ADDRESS, MPU6050_RA_SMPLRT_DIV, 0x07);				//�����ǲ����ʣ�1KHz
		write_to_register(MPU6050_SLAVE_ADDRESS, MPU6050_RA_CONFIG, 0x06);						//��ͨ�˲��������ã���ֹƵ����1K��������5K
		write_to_register(MPU6050_SLAVE_ADDRESS, MPU6050_RA_ACCEL_CONFIG, 0x00);			//���ü��ٶȴ�����������2Gģʽ�����Լ�
		write_to_register(MPU6050_SLAVE_ADDRESS, MPU6050_RA_GYRO_CONFIG, 0x18);				//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
		
		return true;
	}
	else
		return false;
		
}

//��ȡ�Ǽ��ٶ�����
void MPU::measure_gyroscope()
{
	uint8_t gyr_read = 0;
	
	send_value(MPU6050_SLAVE_ADDRESS, MPU6050_GYRO_OUT);
	Wire.requestFrom(MPU6050_SLAVE_ADDRESS, 6);
	
	while (Wire.available())
	{
		gyro_buffer[gyr_read] = Wire.read();
		gyr_read++;
	}
	
	raw_rotation.x = (float)( (gyro_buffer[0] << 8) | gyro_buffer[1] );
	raw_rotation.y = (float)( (gyro_buffer[2] << 8) | gyro_buffer[3] );
	raw_rotation.z = (float)( (gyro_buffer[4] << 8) | gyro_buffer[5] );
}


//��ȡ���ٶȼ�����
void MPU::measure_acceleration()
{
	uint8_t acc_read = 0;
	
	send_value(MPU6050_SLAVE_ADDRESS, MPU6050_GYRO_OUT);
	Wire.requestFrom(MPU6050_SLAVE_ADDRESS, 6);
	
	while (Wire.available())
	{
		acc_buffer[acc_read] = Wire.read();
		acc_read++;
	}
	
	raw_acceleration.x = (float)( (acc_buffer[0] << 8) | acc_buffer[1] );
	raw_acceleration.y = (float)( (acc_buffer[2] << 8) | acc_buffer[3] );
	raw_acceleration.z = (float)( (acc_buffer[4] << 8) | acc_buffer[5] );
}

//��ȡ�¶ȴ���������
#if 0
void measure_temperature()
{
	uint8_t tem_read = 0;
	
	send_value(MPU6050_SLAVE_ADDRESS, MPU6050_RA_TEMP_OUT_H);
	Wire.requestFrom(MPU6050_SLAVE_ADDRESS, 2);
	
	while (Wire.available())
	{
		tem_buffer[tem_read] = Wire.read();
		tem_read++;
	}
	raw_temperature.x
	
}
#endif

