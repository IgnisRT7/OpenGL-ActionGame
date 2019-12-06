/**
*	@file GamePad.h
*/
#pragma once

#include "GLSystem.h"

#include <vector>
#include <array>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

//#include "EnumOperator.h"
namespace JoyStick{

	/**
	*	�W���C�X�e�B�b�N�̃A�i���O���͑��uID
	*
	*	@note XBOX360�R���g���[���[�
	*/
	enum GAMEPAD_AXES {
		GAMEPAD_AXES_LEFT_X,	///< ���X�e�B�b�N��X��
		GAMEPAD_AXES_LEFT_Y,	///< ���X�e�B�b�N��Y��
		GAMEPAD_AXES_TRIGGER,	///< �A�i���O�g���K�[
		GAMEPAD_AXES_RIGHT_Y,	///< �E�X�e�B�b�N��X��
		GAMEPAD_AXES_RIGHT_X,	///< �E�X�e�B�b�N��Y��
	};

	/**
	*	�W���C�X�e�B�b�N�̃f�W�^�����͑��u
	*
	*	@note XBOX360�R���g���[���[�
	*/
	enum GAMEPAD_BUTTON {
		GAMEPAD_BUTTON_A,		///< A�{�^��
		GAMEPAD_BUTTON_B,		///< B�{�^��
		GAMEPAD_BUTTON_X,		///< X�{�^��
		GAMEPAD_BUTTON_Y,		///< Y�{�^��
		GAMEPAD_BUTTON_L,		///< L�{�^��
		GAMEPAD_BUTTON_R,		///< R�{�^��
		GAMEPAD_BUTTON_BACK,	///< Back�{�^��
		GAMEPAD_BUTTON_START,	///< Start�{�^��
		GAMEPAD_BUTTON_L_THUMB,///< ���X�e�B�b�N�{�^��
		GAMEPAD_BUTTON_R_THUMB,///< �E�X�e�B�b�N�{�^��
		GAMEPAD_BUTTON_UP,	///< ��L�[
		GAMEPAD_BUTTON_RIGHT,	///< �E�L�[
		GAMEPAD_BUTTON_DOWN,	///< ���L�[
		GAMEPAD_BUTTON_LEFT,	///< ���L�[
	};

	/**
	*	���ۂɎg�p����{�^���̗�
	*/
	enum class Button : uint32_t {
		
		A = 0x0001,
		B = 0x0002,
		X = 0x0004,
		Y = 0x0008,
		L = 0x0010,
		R = 0x0020,
		START = 0x0040,
		SELECT = 0x0080,
		UP = 0x0100,
		DOWN = 0x0200,
		LEFT = 0x0400,
		RIGHT = 0x0800,

		ALLBITS = A | B | X | Y | L | R | START | SELECT,
	};

	/**
	*	�L�[�̊��蓖�Ă��s�����߂̍\����
	*/
	struct KeyMap {
		int keyID;			///< gl input id
		uint32_t gamepadBit;	///< bit to assign
	};

	/**
	*	�R���g���[���[1�䕪�̃N���X
	*/
	class ControllPad {
		friend class Input;

		ControllPad() = default;
		~ControllPad() = default;
		ControllPad(const ControllPad&) = delete;
		const ControllPad& operator=(const ControllPad&) = delete;

		void Update();

	private:

		enum class ButtonState : char {
			release,
			startPress,
			press,
		};
		std::vector<std::pair<ButtonState, int> > buttons;	///< �{�^���̓��͏��(�᎟���̓��͏��)

		std::vector<KeyMap> keymapList;
	};
}

class Input {
public:

	Input() = default;
	Input(const Input&) = delete;
	
	void Init();
	void Update();

private:

	//�L�[�̉��z���蓖�ď��
	std::vector<JoyStick::KeyMap> PadToPadMap;
	std::vector<JoyStick::KeyMap> keyToPadMap;

	enum class KeyState : char {
		release,
		startPress,
		press,
	};
	std::array <KeyState, GLFW_KEY_LAST + 1> keyState; ///< �L�[�̓��͏��i�[�z��
	
	uint32_t buttons = 0;
	uint32_t buttonDown = 0;
};

namespace enum_concept {

///	template<> struct has_bitwise_operators<Button> : std::true_type {};
///	template<> struct has_and_or_operators<GamePad::Button> : std::true_type {};
}











