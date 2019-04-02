#ifndef MPU6050_H
#define MPU6050_H

#include "include.h"

#define MPU6050_ADDR       0xd0    //AD0 = 0  ±µÿ÷∑
#define CONFIG             0x1a
#define GYRO_CONFIG        0x1b
#define ACCEL_CONFIG       0x1c	
#define INT_PIN_CFG        0x37 
#define MPU6050_BURST_ADDR 0x3b 
#define USER_CTLR          0x6a	       
#define PWR_MGMT1          0x6b	     
#define PWR_MGMT2          0x6c
#define MPU6050_ID_ADDR	   0x75
#define MPU6050_ID	       0x68


typedef struct {
	float x;
	float y;
	float z;
} Vector;

class MPU {
	public:
		void init();
		void send_value(int dev_addr, uint8_t value);
		uint8_t check_id(int dev_addr, uint8_t res_addr);
		void write_to_register(int dev_addr, uint8_t reg_addr, uint8_t reg_value);

		bool check_gyroscope();
		void measure_gyroscope();

		bool check_accelerometer();
		void measure_acceleration();

		//bool check_magnetometer();
		//void measure_magnetometer();

		geometry_msgs::Vector3 raw_acceleration, raw_rotation, raw_magnetic_field;

	private:
		uint8_t gyro_buffer[6];
		uint8_t acc_buffer[6];
		uint8_t mag_buffer[6];
		TwoWire Wire;
};


#endif 
