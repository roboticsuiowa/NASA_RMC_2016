#ifndef ARDUINO_H_
#define ARDUINO_H_
#include <iostream>
#include "lib/arduino-serial-lib.h"

class Arduino{
	
	public:
		/**
		 *
		 */
		Arduino(const char* port, int baud_rate=9600)
		{
			fd=-1;
			fd = serialport_init(port, baud_rate); //opening port
    		(fd == -1) ? std::cout<< "couldn't open port for arduino! :(" << std::endl : std::cout<< "opened port " << port << std::endl;
    		serialport_flush(fd);
    	}
		/**
		 * Description: writes the string to the arduino
		 * Input: the message to be written
		 */
		void write(std::string message);

	private:
		int fd;
};

#endif
