/**
*	@file GamePad.cpp
*/
#include "GamePad.h"

Input& Input::Instance(){

	static Input inst;
	return inst;
}

void Input::Init(){

	const int joystickCountMax = 4;
	Input& inst = Instance();

	//"glfwKeyID -> bit" conversion mapping
	inst.keyToPadMap = std::vector<JoyStick::KeyMap>
	{
			{ GLFW_KEY_J, static_cast<uint32_t>(JoyStick::Button::A) },
			{ GLFW_KEY_K, static_cast<uint32_t>(JoyStick::Button::B) },
			{ GLFW_KEY_U, static_cast<uint32_t>(JoyStick::Button::X) },
			{ GLFW_KEY_I, static_cast<uint32_t>(JoyStick::Button::Y) },
			{ GLFW_KEY_O, static_cast<uint32_t>(JoyStick::Button::L) },
			{ GLFW_KEY_L, static_cast<uint32_t>(JoyStick::Button::R) },
			{ GLFW_KEY_ENTER, static_cast<uint32_t>(JoyStick::Button::START) },
			{ GLFW_KEY_W, static_cast<uint32_t>(JoyStick::Button::UP) },
			{ GLFW_KEY_A, static_cast<uint32_t>(JoyStick::Button::LEFT) },
			{ GLFW_KEY_S, static_cast<uint32_t>(JoyStick::Button::DOWN) },
			{ GLFW_KEY_D, static_cast<uint32_t>(JoyStick::Button::RIGHT) },
	};

	//"glfwGamepadIndex -> bit" conversion mapping
	inst.PadToPadMap = std::vector<JoyStick::KeyMap>
	{
		{ JoyStick::GAMEPAD_BUTTON_A, static_cast<uint32_t>(JoyStick::Button::A) },
		{ JoyStick::GAMEPAD_BUTTON_B, static_cast<uint32_t>(JoyStick::Button::B) },
		{ JoyStick::GAMEPAD_BUTTON_X, static_cast<uint32_t>(JoyStick::Button::X) },
		{ JoyStick::GAMEPAD_BUTTON_Y, static_cast<uint32_t>(JoyStick::Button::Y) },
		{ JoyStick::GAMEPAD_BUTTON_L, static_cast<uint32_t>(JoyStick::Button::L) },
		{ JoyStick::GAMEPAD_BUTTON_R, static_cast<uint32_t>(JoyStick::Button::R) },
		{ JoyStick::GAMEPAD_BUTTON_START, static_cast<uint32_t>(JoyStick::Button::START) },
	};
	//this->padsState.reserve(16);
	//glfwJoystickIsGamepad()
}

void Input::Update() {

	Input& inst = Instance();
	auto& wndInst = GLSystem::Window::Instance();
	GLFWwindow* window = const_cast<GLFWwindow*>(wndInst());
	auto prevButtons = inst.buttons;

	//update keystate
	for (size_t i = 0; i < inst.keyState.size(); i++) {

		const bool pressed = glfwGetKey(window, i) == GLFW_PRESS;
		if (pressed) {
			if(inst.keyState[i] == KeyState::release){
				inst.keyState[i] = KeyState::startPress;
			}
			else if (inst.keyState[i] != KeyState::startPress) {
				inst.keyState[i] = KeyState::press;
			}
		}
		else if(inst.keyState[i] != KeyState::release){
			inst.keyState[i] = KeyState::release;
		}
	}

	//get inputstate joystick axis and buttons
	int axesCount, buttonCount;
	const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
	const uint8_t* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

	if (axes && buttons && axesCount >= 2 && buttonCount >= 8) {

		//initialize the part ot be updated
		inst.buttons &= ~static_cast<uint32_t>(JoyStick::Button::ALLBITS);

		//threshold for recognizing that the axis has been tilted
		static const float threshould = 0.3f;

		//Y axis of left stick 
		if (axes[JoyStick::GAMEPAD_AXES_LEFT_Y] > threshould) {
			inst.buttons |= static_cast<uint32_t>(JoyStick::Button::UP);
		}
		else if (axes[JoyStick::GAMEPAD_AXES_LEFT_Y] < -threshould) {
			inst.buttons |= static_cast<uint32_t>(JoyStick::Button::DOWN);
		}

		//X axis of left stick
		if (axes[JoyStick::GAMEPAD_AXES_LEFT_X] > threshould) {
			inst.buttons |= static_cast<uint32_t>(JoyStick::Button::LEFT);
		}
		else if (axes[JoyStick::GAMEPAD_AXES_LEFT_X] < -threshould) {
			inst.buttons |= static_cast<uint32_t>(JoyStick::Button::RIGHT);
		}
		/*
		//Y axis of right stick
		if (axes[JoyStick::GAMEPAD_AXES_RIGHT_Y] > threshould) {

		}
		else if (axes[JoyStick::GAMEPAD_AXES_RIGHT_Y] < -threshould) {

		}

		//X axis of right stick
		if (axes[JoyStick::GAMEPAD_AXES_RIGHT_X] > threshould) {

		}
		else if (axes[JoyStick::GAMEPAD_AXES_RIGHT_X] < -threshould) {

		}
		*/

		for (const auto& e : inst.PadToPadMap) {

			if (buttons[e.keyID] == GLFW_PRESS) {
				inst.buttons |= static_cast<uint32_t>(e.gamepadBit);
			}
			else if (buttons[e.keyID] == GLFW_RELEASE) {
				inst.buttons &= ~static_cast<uint32_t>(e.gamepadBit);
			}
		}
	}
	else {
		//Receiving input with keyboard instead of joystick
		
		for (const auto& e : inst.keyToPadMap) {
			const int key = glfwGetKey(window, e.keyID);
			if (key == GLFW_PRESS) {
				inst.buttons |= static_cast<uint32_t>(e.gamepadBit);
			}
			else if (key == GLFW_RELEASE) {
				inst.buttons &= ~static_cast<uint32_t>(e.gamepadBit);
			}
		}
	}

	inst.buttonDown = ~inst.buttons & prevButtons;
}
