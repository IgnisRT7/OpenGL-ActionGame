/**
*	@file Shader.h
*/
#pragma once
#include <unordered_map>
#include <memory>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Shader {

	//前方宣言
	class Program;
	using ProgramPtr = std::shared_ptr<Program>;

	/**
	*	プログラムオブジェクトの管理クラス
	*/
	class Program {
	public:

		/**
		*	プログラム作成処理
		*
		*	@param vsPass 頂点シェーダのファイルパス
		*	@param fsPass フラグメントシェーダのファイルパス
		*/
		static ProgramPtr Create(const char* vsPass, const char* fsPass);

		/**
		*	プログラムオブジェクトの有効化
		*
		*	@retval true	有効化成功
		*	@retval false	有効化失敗
		*/
		bool UseProgram() const;

		/**
		*	パラメータ設定処理
		*	
		*/
		template <typename T>
		void SetParameter(T param) {

		}

		/**
		*	このプログラムオブジェクトが有効化どうか調べる
		*
		*	@retval	true	有効	
		*	@retval false	無効
		*/
		bool isValid() const { return program != 0; }

		/**
		*	ビュー射影変換行列の設定処理
		*
		*	@param matVP	ビュー射影変換行列
		*/
		void SetViewProjectionMatrix(glm::mat4 matVP);

	private:

		Program() = default;
		~Program();
		Program(const Program&) = delete;
		const Program& operator=(const Program&) = delete;

	private:

		GLuint program;

	};



	/**
	*	プログラムオブジェクトのバッファクラス
	*/
	class Buffer {
	public:

		/**
		*	プログラム作成処理
		*
		*	@param vsPass
		*	@param fsPass
		*/
		static bool Create(const char* vsPass, const char* fsPass);


	private:

		std::unordered_map<std::string, ProgramPtr> shaderList;
	};


}