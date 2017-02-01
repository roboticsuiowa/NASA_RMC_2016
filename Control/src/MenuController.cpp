#include "../MenuController.h"
#include "../util/splitString.h"


std::string MenuController::getMainMenu()
{
	m_menu_state = main;
	std::string menu_text = "1) CONTROL ROBOT\n2) Send Test Message To Server Without Arduino\n3) Send Test Message To Server With Arduino\n\nEnter a number that is 1-3: ";
	return menu_text;
}
std::string MenuController::getRobotMenu()
{
	m_menu_state = robot_control;
	return "\nMODE KEY: DESIRED ACTION <user input>\nSTOP ROBOT: <1>\nMOVE STRAIGHT FORWARD: <2 powerLevel>\nMOVE STRAIGHT REVERSE: <3 powerLevel>\nTURN RIGHT: <4 powerLevel>\nTURN LEFT: <5 powerLevel>\nLOWER DIGGER/BUCKET: <6>\nRAISE DIGGER/BUCKET:<7>\nREQUEST SENSOR DATA <8>\nREQUEST IMAGE: <9>\n\nPRINT KEY <help>\nBACK TO MAIN MENU <exit>";
}
bool MenuController::processInput()
{
	std::string input=getInput(); //getting the input that was set in the main loop
	//std::cout<<input<<std::endl;
	if(m_menu_state == main) //if we are procesing an input to the main menu
	{
		if(!isMainInputValid(input))
		{
			std::cerr << "invalid input\nmust be a single digit 1-3"<<std::endl;
			return false; //if the input is the incorrect level don't proccess it
		} 
		int input_switch = stoi(input); //can't switch a string so making it an int
		switch(input_switch)
		{
			case 1:
				std::cout<<getRobotMenu()<<std::endl;
				break;
			case 2:
			{	
				std::string message = obtainAndFormatTestMessage();
				sendData(message);
				break;
			} //need brackets in order to set var message
			case 3:
			{
				std::string message = obtainAndFormatTestMessage();
				sendData(message);
				break;
			}
			default:
				std::cerr << "invalid input\nmust be a single digit 1-3" << std::endl;
				return false; //did not successfully process input
				break;
		}	
		return true; //successfully processed input	
	} 
	else if (m_menu_state == robot_control) 
	{
		if(getInput() == "help") std::cout<<"\n"<<getRobotMenu()<<std::endl;
		else if(getInput()=="exit") std::cout<<"\n"<<getMainMenu()<<std::endl;
		else if(!isRobotInputValid(getInput())) //didn't process input
		{ 
			std::cerr<<"invalid input\n"<<getRobotMenu()<<std::endl;
			return false; 
		}
		else
		{
			std::string packet = formatPacketToRobot(getInput());
			sendData(packet);
		}
		return true;
	}
	else
	{
		std::cerr << "FUCK SOMETHING WENT WRONG IN MENUCONTROLLER" << std::endl;
		return false;
	}
}
std::string MenuController::obtainAndFormatTestMessage(bool to_arduino)
{
	std::string packet="[T";
	std:string message;
	std::cout<<"enter message to send: ";
	getline(std::cin, message);
	std::cout<<std::endl;
	packet += to_arduino ? "A"+message : message;
	packet +="]"; //ending packet
	return packet;
}
bool MenuController::isMainInputValid(std::string input)
{
	return input.length()>1 ? false : true;
}
bool MenuController::isRobotInputValid(std::string input)
{
	//as of right now it doesn't check the spacebar!!!
	std::vector<std::string> commands = splitString(input);
	//if # of args is invalid or arg is out of range
	int mode;
	int power_level;
	// if the command is the right number of arguments
	if(commands.size()>2 || commands.size()==0) return false; 
	try //if the command is 1 or 2 numbers (1 # for mode and 1 # for powerlevel)
	{ 
		mode = std::stoi(commands[0]);
		if(commands.size() == 2) power_level = std::stoi(commands[1]);
	}
	catch(...)  //catch all exceptions
	{
		return false;
	}
	if(mode < 1 || mode > 9) return false; //checking mode
	//if arg is supposed to have a mode and a power level
	if((mode >=2  && mode <= 5)){ //check powerlevel
		if(commands.size()!=2) return false; //size should be 2 for this mode
		if (power_level > 127 || power_level < 0) return false; //make sure power level is valid
	}
	//if arg is just supposed to have a mode
	if((mode==1) || (mode > 5 && mode < 9)){
		if(commands.size()!=1) return false; //input size should only be one for this input
	}
	return true; //commamd was valid!
}
bool MenuController::inMainMenu()
{
	 return m_menu_state == main ?  true : false;
}
std::string MenuController::formatPacketToRobot(std::string input)
{
	std::string packet="[";
	std::vector<std::string> commands = splitString(input);
	//if the command is meant to control the robot otherwise it's meant to get sensor data
	int mode = (std::stoi(commands[0]));
	if(mode<=7) packet+="M";
	else if(mode==8) packet+="S";
	else if (mode==9) packet+="I";
	//need another 0 if the command is only one digit
	if(mode < 10) {	
		packet+= commands[0];

	//get the power level if there is one
	if(commands.size()==2){
		if(commands[1].length()==1) packet+="00"+commands[1];
		else if(commands[1].length()==2) packet+="0"+commands[1];
		else packet+=commands[1];
		}
	}
	packet+="]"; //closing the packet
	return packet;

}

