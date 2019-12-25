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
	using ProgramPtr = std::weak_ptr<Program>;
	using ProgramPtr_s = std::shared_ptr<Program>;

	/**
	*	プログラムオブジェクトの管理クラス
	*/
	class Program {
		friend class Buffer;
	public:

		/**
		*	プログラム作成処理
		*
		*	@param vsPass 頂点シェーダのファイルパス
		*	@param fsPass フラグメントシェーダのファイルパス
		*/
		static ProgramPtr Create(const char* vsPass, const char* fsPass);

		GLuint Id() const { return program; }
		/**
		*	プログラムオブジェクトの有効化
		*
		*	@retval true	有効化成功
		*	@retval false	有効化失敗
		*/
		bool UseProgram() const;

		/**
		*	このプログラムオブジェクトが有効化どうか調べる
		*
		*	@retval	true	有効
		*	@retval false	無効
		*/
		bool isValid() const { return program != 0; }

		/**
		*	テクスチャのバインドをします
		*
		*	@param unit	適用するテクスチャのユニット番号
		*/
		void BindTexture(GLuint unit, GLuint texture, GLuint type = GL_TEXTURE_2D);

		/**
		*	ビュー射影変換行列の設定処理
		*
		*	@param matVP	ビュー射影変換行列
		*/
		void SetViewProjectionMatrix(const glm::mat4& matVP);
		
		/**
		*	時間の設定処理
		*/
		void SetTime(float time);

	private:

		Program() = default;
		~Program();
		Program(const Program&) = delete;
		const Program& operator=(const Program&) = delete;

	private:

		GLint matVPLocation = -1;	///< MVPロケーション位置

		int samperCount = 0;		///< テクスチャサンプラー数
		GLint samplerLocation = -1;	///< テクスチャサンプラー位置

		GLuint program = -1;		///< プログラムオブジェクトID

	};

	/**
	*	プログラムオブジェクトのバッファクラス
	*/
	class Buffer {
	public:

		/**
		*	バッファインスタンスの取得
		*
		*	@return シェーダバッファインスタンス
		*/
		static Buffer& Instance();

		/**
		*	プログラム作成処理
		*
		*	@param vsPass	頂点シェーダのファイル名
		*	@param fsPass	頂点シェーダのファイル名
		*	@param shaderName	シェーダ名
		*/
		static ProgramPtr Create(const char* vsPass, const char* fsPass, const char* shaderName = "no name");


	private:

		Buffer() = default;
		~Buffer();
		Buffer(const Buffer&) = delete;
		const Buffer& operator=(const Buffer&) = delete;

		using ShaderMapType = std::unordered_map<std::string, ProgramPtr_s>;
		ShaderMapType shaderList;	///< シェーダのリスト
	};


}