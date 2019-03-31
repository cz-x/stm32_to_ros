#pragma once
#include "hardwareserial.h"
HardwareSerial Serial(SERIAL1);
#define SERIAL_CLASS  HardwareSerial

class STM32Hardware {
  public:
  	STM32Hardware(SERIAL_CLASS* io , long baud= 115200){
      iostream = io;
      baud_ = baud;
    }
    STM32Hardware()
    {
      iostream = &Serial;
      baud_ = 115200;
    }
    STM32Hardware(STM32Hardware& h){
	  this->iostream = iostream;
      this->baud_ = h.baud_;
    }

    void setBaud(long baud){
      this->baud_= baud;
    }

    int getBaud(){return baud_;}

		//初始化
    void init(){
      iostream->begin(baud_);
    }

		//读串口函数
    int read(){
      if(iostream->available()){
	  	return iostream->read();
      }else{
	    return -1;
      }
    };

		//写串口函数
    void write(uint8_t* data, int length){
      for(int i=0; i<length; i++){
		  iostream->write(data[i]);
      }
    }

		//时间返回函数
    unsigned long time(){return millis();}

  protected:
    SERIAL_CLASS* iostream;
    long baud_;
};
