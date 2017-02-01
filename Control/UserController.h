/*
 * UserController.h
 * Created on: Apr 7, 2016
 * Author: stephaniesmith
 *
 * Class description: This class is the base class for XboxController or MenuController. UserController will
 * 					  never be instantiated itself b/c it contains an abstract method. UserContoller facilitates
 * 					  sending and receiving data from the robot through a TCPSocket.
 */
#include "lib/PracticalSocket.h" //for TCPSocket
#include <memory> //for smart pointers
#include <iostream>
#ifndef USERCONTROLLER_H_
#define USERCONTROLLER_H_

#define RCVBUFSIZE  50 

class UserController
{
	public:
		/**
	     * Description: constructor creates TCPSocket object
	     * Inputs: the IP Address of the NUC (the computer on the robot)
	     * 		   the port number the NUC is listening on
	     */
		UserController(std::string address, unsigned short port) :
			m_socket(new TCPSocket(address, port)), m_input(""){}

        /**
        * Description: constructor with no arguments for testing purposes.
        */
        UserController(){}
		//make virtual to ensure that actual instantiated object is destructed
		virtual ~UserController(){}
		/**
		 * Description: receives data from the robot. The type of data the user will
		 * 				receive is sensor data if the Control System is not in debug mode. If the
		 * 				Control System is in debug mode, the user will receive what it sent echoed back
		 * Input:  none
		 * Output: character array of data
		 */
		std::string receiveData(void);
		/**
		 * Description: send data to the robot through m_socket. The type of data the user will send are commands or
		 * 				requests.
		 * 				Commands are statements like "move forward"
		 * 				Requests are statements like "get location"
		 * 	Input: a character array of that will either contain a command or request
		 * 	Output: none
		 */
		bool sendData(std::string data);
		/**
		 * Description: method is to implementd in MenuController & XboxController. it will process the user's
		 *              input whether the input is something like a button press or a command line input
		 * Input: void
		 * Output: returns true if processed input successfully, false otherwise
		 */
		virtual bool processInput()=0;
		/**
		 * Description: method sets the input
		 * Input: string that will be placed in the input
		 * Output: void
		 */
		void setInput(std::string input) {m_input = input;};
		/**
		 * Description: method gets the input
		 * Input: void
		 * Output: void
		 */
		std::string getInput() {return m_input;}
		/**
		 * for the user to change ip addresses and stuff (maybe won't do it)
		 */
		std::shared_ptr<TCPSocket> getSocket(){return m_socket;}

	private:
		/**
		 * TCP connection between the user and the robot
		 */
		std::shared_ptr<TCPSocket> m_socket;

		/**
		 * user's command input (can result from keyboard or button press on xbox controller)
		 */
		std::string m_input;
};



#endif /* USERCONTROLLER_H_ */
