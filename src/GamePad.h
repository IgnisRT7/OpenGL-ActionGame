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
	*	ジョイスティックのアナログ入力装置ID
	*
	*	@note XBOX360コントローラー基準
	*/
	enum GAMEPAD_AXES {
		GAMEPAD_AXES_LEFT_X,	///< 左スティックのX軸
		GAMEPAD_AXES_LEFT_Y,	///< 左スティックのY軸
		GAMEPAD_AXES_TRIGGER,	///< アナログトリガー
		GAMEPAD_AXES_RIGHT_Y,	///< 右スティックのX軸
		GAMEPAD_AXES_RIGHT_X,	///< 右スティックのY軸
	};

	/**
	*	ジョイスティックのデジタル入力装置
	*
	*	@note XBOX360コントローラー基準
	*/
	enum GAMEPAD_BUTTON {
		GAMEPAD_BUTTON_A,		///< Aボタン
		GAMEPAD_BUTTON_B,		///< Bボタン
		GAMEPAD_BUTTON_X,		///< Xボタン
		GAMEPAD_BUTTON_Y,		///< Yボタン
		GAMEPAD_BUTTON_L,		///< Lボタン
		GAMEPAD_BUTTON_R,		///< Rボタン
		GAMEPAD_BUTTON_BACK,	///< Backボタン
		GAMEPAD_BUTTON_START,	///< Startボタン
		GAMEPAD_BUTTON_L_THUMB,///< 左スティックボタン
		GAMEPAD_BUTTON_R_THUMB,///< 右スティックボタン
		GAMEPAD_BUTTON_UP,	///< 上キー
		GAMEPAD_BUTTON_RIGHT,	///< 右キー
		GAMEPAD_BUTTON_DOWN,	///< 下キー
		GAMEPAD_BUTTON_LEFT,	///< 左キー
	};

	/**
	*	実際に使用するボタンの列挙
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
	*	キーの割り当てを行うための構造体
	*/
	struct KeyMap {
		int keyID;			///< gl input id
		uint32_t gamepadBit;	///< bit to assign
	};

	/**
	*	コントローラー1台分のクラス
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
		std::vector<std::pair<ButtonState, int> > buttons;	///< ボタンの入力情報(低次元の入力情報)

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

	//キーの仮想割り当て情報
	std::vector<JoyStick::KeyMap> PadToPadMap;
	std::vector<JoyStick::KeyMap> keyToPadMap;

	enum class KeyState : char {
		release,
		startPress,
		press,
	};
	std::array <KeyState, GLFW_KEY_LAST + 1> keyState; ///< キーの入力情報格納配列
	
	uint32_t buttons = 0;
	uint32_t buttonDown = 0;
};

namespace enum_concept {

///	template<> struct has_bitwise_operators<Button> : std::true_type {};
///	template<> struct has_and_or_operators<GamePad::Button> : std::true_type {};
}











