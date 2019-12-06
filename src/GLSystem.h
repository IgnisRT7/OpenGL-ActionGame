/**
*	GLSystem.h
*/

#pragma once

#include "GamePad.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace GLSystem {

	/**
	*	GLを利用したシステム管理クラス
	*	tips このクラスはシングルトンで管理される
	*
	*
	*
	*
	*
	*/
	class Window {
	public:

		/**
		*	シングルトンインスタンスの取得
		*
		*	@return シングルトンインスタンス
		*/
		static Window& Instance();

		/**
		*	画面の横幅を取得
		*
		*	@return 画面の横幅
		*/
		int Width() const { return width; }

		/**
		*	画面の縦幅を取得
		*
		*	@return 画面縦幅
		*/
		int Height() const { return height; }

		/**
		*	初期化処理
		*
		*	@param width	ウインドウの横幅
		*	@param height	ウインドウの縦幅
		*	@param title	ウインドウのタイトル名
		*
		*	@return true	初期化成功
		*	@return false	初期化失敗
		*/
		bool Init(int width,int height,const char* title);

		/**
		*	ウィンドウを閉じるべきか調べる
		*
		*	@return true	閉じる
		*	@return false	閉じない
		*/
		bool ShouldClose();

		/**
		*	バックバッファのデータをフロントバッファに反映させる
		*/
		void SwapBuffers();

		const GLFWwindow* operator()() { return window; }

		///---- ここからはのちに実装予定 ----///
		/*
		*	経過時間の計算処理
		*	入力情報の取得

		*/

	private:

		Window() = default;
		~Window();
		Window(const Window&) = delete;
		const Window& operator=(const Window&) = delete;

		GLFWwindow* window = nullptr;	///< ウインドウ管理ハンドル
		Input input;					///< 入力情報管理

		bool isInitialized = false;		///< 初期化済みフラグ
		bool glfwInitialized = false;	///< GLFW初期化済みフラグ

		

		int height = 0;					///< ウインドウの縦幅
		int width = 0;					///< ウインドウの横幅
	};

}