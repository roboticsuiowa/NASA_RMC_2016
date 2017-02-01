#include "../lib/PracticalSocket.h"
#include "../XboxController.h"

#include <iostream>
#include <string.h>
#include <memory>
#include <stdlib.h>
#define DEBUG 0

// To compile: g++ --std=c++11 -o xboxtest.exe tests/XboxControllerTest.cpp src/UserController.cpp src/XboxController.cpp lib/PracticalSocket.cpp lib/CXboxController.cpp
int main (int argc, char** argv)
{
	std::string address;
	unsigned short port;
#if DEBUG
	port=1024;
	address="127.0.0.1";
#else
	if(argc != 3)
	{
 	  	cerr << "Usage: " << argv[0] << " <Server Address> <Server Port>" << endl;
    	exit(1);
    }
    else
    {
    	address=argv[1];
    	try{ port=atoi(argv[2]);}
    	catch(...){
    		std::cerr<<"invalid port number"<<std::endl;
    		exit(1);
    	}
    }
#endif
	XboxController xboxcontroller(address, port); //construct xbox controller object
	xboxcontroller.initializeController();
	std::string server_data;

    while(true)
    {
		std::cout<<"\n\n"<<xboxcontroller.getMenu()<<std::endl;
		bool success=xboxcontroller.processInput();
		if(success)
		{
			// send the data to the server
			xboxcontroller.sendData(xboxcontroller.getInput());
			if((xboxcontroller.getInput() == "[S]") || (xboxcontroller.getInput() == "[I]"))
            {
                server_data=xboxcontroller.receiveData(); //try to get messages back from the server
                if(server_data.length()>=1) std::cout<<"Server: "<<server_data<<std::endl;
            }
		}
		else
		{
			// Exited for some reason or another
			return 0;
		}
	}
    return 0;
}



