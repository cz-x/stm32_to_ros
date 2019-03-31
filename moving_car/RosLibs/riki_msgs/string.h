#ifndef ROS_STRING_H
#define ROS_STRING_H

#include "ros/msg.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

namespace std_msgs
{
	
	class String : public ros::Msg 
	{
	public:
		const char* date;

	#if 1
    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_date = strlen(this->date);
      memcpy(outbuffer + offset, &length_date, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->date, length_date);
      offset += length_date;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_date;
      memcpy(&length_date, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_date; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_date-1]=0;
      this->date = (char *)(inbuffer + offset-1);
      offset += length_date;
     return offset;
    }
		
    const char * getType(){ return "std_msgs/String"; };
    const char * getMD5(){ return "992ce8a1687cec8c8bd883ec73ca41d1"; };
	#endif
	
	};
	
}



#endif
