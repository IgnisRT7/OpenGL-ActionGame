/**
*	GLSystem.h
*/

#pragma once

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
		bool isInitialized;				///< 初期化フラグ
	};

}