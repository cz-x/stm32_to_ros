#include <stdio.h>
#include "hardwareserial.h"

#include <ros.h>
#include <riki_msgs/Velocities.h>
#include <geometry_msgs/Twist.h>
#include <ros/time.h>

#include "config.h"
#include <riki_msgs/Imu.h>
#include <geometry_msgs/Vector3.h>
#include "mpu6050.h"


bool mpu_stay;
bool is_first = true;
MPU mpu6050;

ros::NodeHandle nh;
riki_msgs::Imu raw_imu_msg;
ros::Publisher raw_imu_pub("raw_imu", &raw_imu_msg);


void check_imu()
{
	mpu_stay = mpu6050.check_mpu();
	
	if (!mpu_stay)
	{
		nh.logerror("IMU NOT FOUND!");
	}
	is_first = false;
	//fbdbfdbdbd
}

void publish_imu()
{
	if(mpu_stay)
	{
		mpu6050.measure_gyroscope();
		mpu6050.measure_acceleration();
		raw_imu_msg.angular_velocity = mpu6050.raw_rotation;
		raw_imu_msg.linear_acceleration = mpu6050.raw_acceleration;
	}
}	


int main()
{
	uint32_t previous_imu_time = 0;
	
	mpu6050.init();
	
	nh.initNode();
	nh.advertise(raw_imu_pub);
	
	while (!nh.connected())
	{
		nh.spinOnce();
	}
	
	while(1)
	{
		 if ((millis() - previous_imu_time) >= (1000 / IMU_PUBLISH_RATE))
		{
				if(is_first){
								//sanity check if the IMU exits
								check_imu();
				} else{
								//publish the IMU data
								publish_imu();
				}
				previous_imu_time = millis();
			}
			
		}
	nh.spinonce();
		
}
