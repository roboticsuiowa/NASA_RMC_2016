#include "../XboxController.h" // not sure if this is needed
#include <iostream>
#include <sstream>

std::string XboxController::getMenu()
{
	return "\n---CONTROLS---\nPOWER LEVEL DOWN: <D-PAD DOWN>\nPOWER LEVEL UP: <D-PAD UP>\nSTOP ROBOT: <START>\nMOVE STRAIGHT FORWARD: <A>\nMOVE STRAIGHT REVERSE: <Y>\nTURN RIGHT: <B>\nTURN LEFT: <X>\nDIGGER DROP: <RB>\nRAISE DIGGER: <LB>\nREQUEST SENSOR DATA: <LEFT THUMB>\nREQUEST IMAGE: <RIGHT THUMB>\nEXIT: <SELECT>";
}

bool XboxController::processInput()
{
	if(m_player_controller->IsConnected())
	{
		// continuously poll the xbox controller until a command button is pressed
		while(true)
		{
			m_state = m_player_controller->GetState();

			if((m_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) && (m_xbox_controller.endUp == true) )
			{
				m_xbox_controller.endUp = false;
				int newPowerLevel = getPowerLevel() + 10 < MAX_POWER_LEVEL ? getPowerLevel() + 10 : MAX_POWER_LEVEL;
				setPowerLevel(newPowerLevel);
				std::cout << "Power Level: " << getPowerLevel() << std::endl;
			}
			else if(!(m_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)) {
				m_xbox_controller.endUp = true;
			}

			// Check for d-pad down button (power level decrease)
			if((m_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) && (m_xbox_controller.endDown == true) )
			{
				m_xbox_controller.endDown = false;
				int newPowerLevel = getPowerLevel() - 10 > 0 ? getPowerLevel() - 10 : 0;
				setPowerLevel(newPowerLevel);
				std::cout << "Power Level: " << getPowerLevel() << std::endl;
			}
			else if(!(m_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)) {
				m_xbox_controller.endDown = true;
			}

			// Check for A button (forward)
			if((m_state.Gamepad.wButtons & XINPUT_GAMEPAD_A) && (m_xbox_controller.endA == true) )
			{
				setInput("[M2" + int_to_string(getPowerLevel()) + "]");
				m_xbox_controller.endA = false;
				std::cout << "Go forward at power level: " << getPowerLevel() << std::endl;
				return true;
			}
			else if(!(m_state.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
				m_xbox_controller.endA = true;
			}

			// Check for B button (turn right)
			if((m_state.Gamepad.wButtons & XINPUT_GAMEPAD_B) && (m_xbox_controller.endB == true))
			{
				setInput("[M4" + int_to_string(getPowerLevel()) + "]");
				m_xbox_controller.endB = false;
				std::cout << "Turn right at power level: " << getPowerLevel() << std::endl;
				return true;
			}
			else if(!(m_state.Gamepad.wButtons & XINPUT_GAMEPAD_B)) {
				m_xbox_controller.endB = true;
			}

			// Check for X button (turn left)
			if((m_state.Gamepad.wButtons & XINPUT_GAMEPAD_X) && (m_xbox_controller.endX == true))
			{
				setInput("[M5" + int_to_string(getPowerLevel()) + "]");
				m_xbox_controller.endX = false;
				std::cout << "Turn left at power level: " << getPowerLevel() << std::endl;
				return true;
			}
			else if(!(m_state.Gamepad.wButtons & XINPUT_GAMEPAD_X)) {
				m_xbox_controller.endX = true;
			}

			// Check for Y button (backward)
			if((m_state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) && (m_xbox_controller.endY == true))
			{
				setInput("[M3" + int_to_string(getPowerLevel()) + "]");
				m_xbox_controller.endY = false;
				std::cout << "Go backward at power level: " << getPowerLevel() << std::endl;
				return true;
			}
			else if(!(m_state.Gamepad.wButtons & XINPUT_GAMEPAD_Y)) {
				m_xbox_controller.endY = true;
			}

			// Check for RB button (digger drop)
			if((m_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) && (m_xbox_controller.endRB == true))
			{
				setInput("[M6]");
				m_xbox_controller.endRB = false;
				std::cout << "Dropping Digger" << std::endl;
				return true;
			}
			else if(!(m_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
				m_xbox_controller.endRB = true;
			}

			// Check for LB button (digger raise)
			if((m_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) && (m_xbox_controller.endLB == true))
			{
				setInput("[M7]");
				m_xbox_controller.endLB = false;
				std::cout << "Raising Digger" << std::endl;
				return true;
			}
			else if(!(m_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)) {
				m_xbox_controller.endLB = true;
			}

			// The following buttons were removed since the robot is built
			// to have the digger and bucket work with the same button press
			// instead these buttons will be for data aquisition (image, ir, and laser range finder)

			// Check for Right Thumb button (image)
			if((m_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) && (m_xbox_controller.endRThumb == true))
			{
				setInput("[I]");
				m_xbox_controller.endRThumb = false;
				return true;
			}
			else if(!(m_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)) {
				m_xbox_controller.endRThumb = true;
			}

			// Check for Left Thumb button (ir and laser)
			if((m_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) && (m_xbox_controller.endLThumb == true))
			{
				setInput("[S]");
				m_xbox_controller.endLThumb = false;
				return true;
			}


			else if(!(m_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)) {
				m_xbox_controller.endLThumb = true;
			}


			// Check for start button (stop). I know what you're thinking. Why is start stop? It's close to the right thumb, but still far from the control buttons.
			if((m_state.Gamepad.wButtons & XINPUT_GAMEPAD_START) && (m_xbox_controller.endStart == true))
			{
				setInput("[M1]");
				m_xbox_controller.endStart = false;
				return true;
			}
			else if(!(m_state.Gamepad.wButtons & XINPUT_GAMEPAD_START)) {
				m_xbox_controller.endStart = true;
			}

			// Check for select button (exit)
			if(m_state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
			{
				std::cout << "Exiting Client Program";
				return false;
			}

		}
	}
	else
	{
		// ERROR! PLAYER 1 - XBOX 360 Controller Not Found!
		std::cout << "Controller not found, exiting Client Program";
		return false;
	}
}


std::string int_to_string(int value)
{
    std::ostringstream os;
    os << value;
    std::string temp_string = os.str();
    if(temp_string.length() == 1)
    {
        temp_string = "00" + temp_string;
    }
    else if(temp_string.length() == 2)
    {
        temp_string = "0" + temp_string;
    }
    return temp_string;
}
