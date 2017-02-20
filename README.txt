WARNING: ARDUINO CODE HAS NOT BEEN TESTED! ADDED FOR ACCESS AND TESTING

Main control systems folder (Control in new git, ControlSystems in old git)
	(NOTE: all the .cpp files for the .h files are in the src folder!)

	arduino.h : for writing messages to the Arduino motor controller (which acts as intermediary between Nook and all of the other motor controllers)
	ControlSystem.cpp : runs on the Nook on the robot to control the robot
	MenuController.h : Alternative for running the control just with the laptop, no XBox controller (instance created by MenuControllerTest.cpp)
	XBoxController.h : Control code for use with XBox (instance created by XBoxControllerTest.cpp)
	UserController.h : abstract parent class for Menu/XBoxController
	ThreadedNetwork.h : Networking implemented with threads, called by ControlSystem on the Nook on the robot
				Sticks the messages into the queue, and the ControlSystem then reads those messages.

Four subfolders:
lib
	arduino-serial-lib.cpp/.h 
	CXBOXController.cpp/.h
	PracticalSocket.cpp/.h
	Xinput.cpp/h : also related to controlling the XBox
src
	Arduino.cpp
	MenuControler.cpp
	ThreadedNetwork.cpp
	XBoxController.cpp
tests
	MenuControllerTest.cpp
	XBoxControllerTest.cpp
util
	splitstring.h : implementation for parsing the messages
	ThreadSafeQueue.h : implementation of a queue for use by the threaded network


Architectural Setup
Control laptop (client?) -> Nook (server) -> Arduino -> Motor controllers
	
	