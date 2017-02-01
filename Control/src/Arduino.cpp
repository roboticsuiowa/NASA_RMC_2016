#include "../Arduino.h"
#define BUF_MAX 256   // default max buffer size
#define TIMEOUT 5000 // default timeout
#define DEBUG 1


void Arduino::write(std::string message)
{
   std::cout<<"writing: "<<message<<std::endl;
   const char* ard_message=message.c_str();
   int rd = serialport_write(fd, ard_message);
   if(rd == -1) std::cout<<"error writing messages to arduino!" <<std::endl;

#if DEBUG //if we are debugging let's get the messages back
   	char eolchar = '\n';
   	char* buf=(char*)malloc(BUF_MAX*sizeof(char));
	  memset(buf,0,BUF_MAX); //clearing buffer
    serialport_read_until(fd, buf, eolchar, BUF_MAX, TIMEOUT); //'Arduino received <message>'
    std::cout<<"Arduino Echoed: "<<buf<<std::endl;
    free(buf);
#endif   	

}