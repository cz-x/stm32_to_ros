#include <stdio.h>
#include "hardwareserial.h"

#include "gy85.h"
#include "motor.h"
#include "encoder.h"
#include "battery.h"
#include "led.h"
#include "PID.h"
#include "Kinematics.h"

#include <ros.h>
#include <riki_msgs/Velocities.h>
#include <geometry_msgs/Twist.h>

#include <riki_msgs/PID.h>
#include <riki_msgs/Imu.h>
#include <riki_msgs/Battery.h>

#include <geometry_msgs/Vector3.h>
#include <ros/time.h>
#include "config.h"

int Motor::counts_per_rev_ = COUNTS_PER_REV;

double required_angular_vel_z = 0;
double required_linear_vel_x = 0;
double required_linear_vel_y = 0;
uint32_t previous_command_time = 0;

bool is_first = true;
bool accel, gyro, mag;

PID motor1_pid(-255, 255, K_P, K_I, K_D);
PID motor2_pid(-255, 255, K_P, K_I, K_D);
PID motor3_pid(-255, 255, K_P, K_I, K_D);
PID motor4_pid(-255, 255, K_P, K_I, K_D);
 
Motor motor1(MOTOR1, 254, 575);
Motor motor2(MOTOR2, 254, 575);
Motor motor3(MOTOR3, 254, 575);
Motor motor4(MOTOR4, 254, 575);

Battery bat(25, 10.6, 12.6);
Kinematics kinematics(MAX_RPM, WHEEL_DIAMETER, FR_WHEELS_DISTANCE, LR_WHEELS_DISTANCE);
Gy85  imu;
Led led;

void pid_callback( const riki_msgs::PID& pid);
void command_callback( const geometry_msgs::Twist& cmd_msg);

ros::NodeHandle  nh;

riki_msgs::Imu raw_imu_msg;
riki_msgs::Velocities raw_vel_msg;
riki_msgs::Battery raw_battery_msg;

//��������
ros::Subscriber<geometry_msgs::Twist> cmd_sub("cmd_vel", command_callback);
ros::Subscriber<riki_msgs::PID> pid_sub("pid", pid_callback);

//��������
ros::Publisher raw_vel_pub("raw_vel", &raw_vel_msg);
ros::Publisher raw_imu_pub("raw_imu", &raw_imu_msg);
ros::Publisher raw_battery_pub("battery", &raw_battery_msg);


void pid_callback( const riki_msgs::PID& pid)
{
    motor1_pid.updateConstants(pid.p, pid.i, pid.d);
    motor2_pid.updateConstants(pid.p, pid.i, pid.d);
    motor3_pid.updateConstants(pid.p, pid.i, pid.d);
    motor4_pid.updateConstants(pid.p, pid.i, pid.d);
}

void command_callback( const geometry_msgs::Twist& cmd_msg)
{
	required_linear_vel_x = cmd_msg.linear.x;
	required_linear_vel_y = cmd_msg.linear.y;
	required_angular_vel_z = cmd_msg.angular.z;

    previous_command_time = millis();
}

void move_base()
{
    motor1.updateSpeed(en_pos1);
    motor2.updateSpeed(en_pos2);
    motor3.updateSpeed(en_pos3);
    motor4.updateSpeed(en_pos4);
    
    Kinematics::rpm req_rpm = kinematics.getRPM(required_linear_vel_x, required_linear_vel_y, required_angular_vel_z);
 
		motor1.spin(motor1_pid.compute(req_rpm.motor3, motor1.rpm));
    motor2.spin(motor2_pid.compute(req_rpm.motor4, motor2.rpm));
    motor3.spin(motor3_pid.compute(req_rpm.motor2, motor3.rpm));
    motor4.spin(motor4_pid.compute(req_rpm.motor1, motor4.rpm));
    
		Kinematics::velocities current_vel;
 
    current_vel = kinematics.getVelocities(motor1.rpm, motor2.rpm, motor3.rpm, motor4.rpm);
	
	 //fill in the object
    raw_vel_msg.linear_x = current_vel.linear_x;
    raw_vel_msg.linear_y = 0.0;
    raw_vel_msg.angular_z = current_vel.angular_z;

    //publish raw_vel_msg object to ROS
    raw_vel_pub.publish(&raw_vel_msg);
}

void check_imu()
{
    gyro = imu.check_gyroscope();
    accel = imu.check_accelerometer();
    mag = imu.check_magnetometer();

    if (!accel){
        nh.logerror("Accelerometer NOT FOUND!");
    }   

    if (!gyro){
        nh.logerror("Gyroscope NOT FOUND!");
    }   

    if (!mag){
        nh.logerror("Magnetometer NOT FOUND!");
    }
    is_first = false;
}

void publish_imu()
{
    //geometry_msgs::Vector3 acceler, gyro, mag;
    //this function publishes raw IMU reading
    //measure accelerometer
    if (accel){
        imu.measure_acceleration();
        raw_imu_msg.linear_acceleration = imu.raw_acceleration;
    }

    //measure gyroscope
    if (gyro){
        imu.measure_gyroscope();
        raw_imu_msg.angular_velocity = imu.raw_rotation;
    }

    //measure magnetometer
    if (mag){
        imu.measure_magnetometer();
        raw_imu_msg.magnetic_field = imu.raw_magnetic_field;
    }

    //publish raw_imu_msg object to ROS
    raw_imu_pub.publish(&raw_imu_msg);

}


void stop_base()
{
	required_linear_vel_x = 0;
	required_linear_vel_y = 0;
	required_angular_vel_z = 0;
}

void publishBAT()
{
	raw_battery_msg.battery = bat.get_volt();
	raw_battery_pub.publish(&raw_battery_msg);
}

void print_debug()
{
    char buffer[50]; 
    
    sprintf (buffer, "Encoder Left Rear: %d, Right Rear: %d", en_pos1, en_pos2);
		nh.loginfo(buffer);
	
		sprintf (buffer, "Encoder Right Front: %d, Left Front: %d", en_pos3, en_pos4);
    nh.loginfo(buffer);
    //sprintf (buffer, "get line speed : %f, pwm: %d", required_linear_vel, pwm);
    //nh->loginfo(buffer);
}

int main(void) 
{
	bool OnOff = true;
	uint32_t previous_battery_debug_time = 0;
	uint32_t previous_debug_time = 0;
	uint32_t previous_imu_time = 0;
	uint32_t previous_control_time = 0;
	uint32_t publish_vel_time = 0;
	char battery_buffer[]= "The voltage is lower than 11.3V,Please charge! ";

	SystemInit();
	initialise();
    
  motor1.init();
	motor2.init();	
  motor3.init();
	motor4.init();	
        
  encoder_init(ENCODER1);
	encoder_init(ENCODER2);
	encoder_init(ENCODER3);
	encoder_init(ENCODER4);
    
	led.init();
	imu.init();
	bat.init();

		nh.initNode();	
		nh.advertise(raw_vel_pub);
    nh.advertise(raw_imu_pub);
    nh.advertise(raw_battery_pub);
    nh.subscribe(pid_sub);
    nh.subscribe(cmd_sub);

	while (!nh.connected()){
		nh.spinOnce();
	}   
	nh.loginfo("Rikibase Connected!");
  
	led.on_off(OnOff);


    while(1){
		if ((millis() - previous_control_time) >= (1000 / COMMAND_RATE)){
			 move_base();
            previous_control_time = millis();
        }

        if ((millis() - previous_command_time) >= 400){
            stop_base();
        }

        if ((millis() - previous_imu_time) >= (1000 / IMU_PUBLISH_RATE)){
                if(is_first){
                        //sanity check if the IMU exits
                        check_imu();
                } else{
                        //publish the IMU data
                        publish_imu();
                }
                previous_imu_time = millis();
        }
        
        if( (millis() - previous_battery_debug_time) >= (1000 / BAT_PUBLISH_RATE)){
            if(bat.get_volt() < 11.300000){
                OnOff = !OnOff;
                led.on_off(OnOff);
                nh.logwarn(battery_buffer);			
            }
            publishBAT();
            previous_battery_debug_time = millis();		
        }

        if(DEBUG){
            if ((millis() - previous_debug_time) >= (1000 / DEBUG_RATE)) {
                print_debug();
                previous_debug_time = millis();
            }
        }
        nh.spinOnce();
    }

}


