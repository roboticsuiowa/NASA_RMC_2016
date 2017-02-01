//============================================================================
// Name        : ControlSystem.cpp
// Description : Main Robot Code
// Compiling   : g++ -Wall -std=c++11 -o ControlSystem.exe ControlSystem.cpp src/ThreaddedNetwork.cpp lib/PracticalSocket.cpp src/Arduino.cpp -lpthread

//============================================================================
#include <iostream>
#include "ThreaddedNetwork.h"
#include "Arduino.h"
#include <vector>
#include <string>

std::string leftPad(unsigned int num)
{
  if(num == 0) {return std::string("000");}
  std::string out;
  if(num < 10)
  {
    std::string out = "00";
  }
  else if(num < 100)
  {
    std::string out = "0";
  }
  else if(num > 999)
  {
    return "999";
  }
  out += num;
  return out;
}

void handleMovementCommand(std::string cmd, Arduino& motor_arduino)
{
  char mode = cmd.at(1); // mode is the second letter
  std::string speed = std::stoi(cmd.substring(2));
  if(mode == '0') // stop
  {
    motor_arduino.write('s');
  }
  else if(mode == '1') // forward
  {
    std::string out = "";
    out += "!01";
    out += leftPad(speed);
    out += "!11";
    out += leftPad(speed);
    std::cout << out << std::endl;
    motor_arduino.write(out);
  }
}

int main(int argc, char *argv[])
{
  /* Read Command Line Arguments */
  /* Correct Usage: ./ControlSystem.exe 12345 */
  if (argc != 2)
  {
    cerr << "Usage: " << argv[0] << " <Server Port>" << endl;
    exit(1);
  }
  unsigned short server_port = atoi(argv[1]);


  /* Display welcome message */
  std::cout << std::endl
            << "University of Iowa Robotics Club Presents:" << std::endl
            << "2016 NASA Robotic Mining Competition Robot" << std::endl
            << "------------------------------------------" << std::endl
            << std::endl;

  /* Construct robot components */
  ThreaddedNetwork network(server_port);
  Arduino motor_arduino("dev/ttyACM0");


  /* Main program loop */
  while(true)
  {
    /* check for any new messages from the server */
    std::vector<std::string> newMessages = network.getNewMessages();
    if(newMessages.size() > 0)
    {
      /* Processes incoming messages */
      for(unsigned int i=0; i<newMessages.size(); i++)
      {
        /* for now just print the messages */
        std::cout << newMessages.at(i) << std::endl;

        /* pseudo-code */
        switch(newMessages.at(i)[0])
        {
          case 'M': // movement command
            handleMovementCommand(newMessages.at(i), motor_arduino);
            break;
          case 'S': // sensor request
            break;
          case 'I': // image request
            break;
          case 'T': // test message
            break;
          default:  // unknown message
            break;
        }
      }
    }

    /* Update robot state */
    //

    /* Send back data */
    //
  }
  return 0;
}
