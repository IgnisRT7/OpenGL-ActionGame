/**
*	@file FontRenderer.h
*/
#pragma once

#include "Texture.h"
#include "BufferObject.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <unordered_map>
#include <memory>
#include <string>



namespace Font {

/**
*
*	1,フォント読み込み -制約として読み込めるフォントは1種類のみ
*	2,
*
*
*/

	//前方宣言
	class FontData;
	using FontDataPtr = std::weak_ptr<FontData>;

	class FontData {
		friend class Buffer;
		friend class Renderer;
	public:

		/**
		*	フォントが有効か否かを取得
		*
		*	@retval true	有効化
		*	@retval false	無効化
		*/
		bool IsValid() const { return valid; }

	private:

		FontData() = default;
		~FontData() {}
		FontData(const FontData&) = delete;
		const FontData& operator=(const FontData&) = delete;

	private:

		/// フォント情報
		struct CharacterInfo {
			int id = -1;						///< 文字コード
			int page = 0;						///< 文字が含まれるフォントテクスチャ番号
			glm::vec2 uv = glm::vec2(0);		///< フォント画像のテクスチャ座標
			glm::vec2 size = glm::vec2(0);		///< フォント画像の表示サイズ
			glm::vec2 offset = glm::vec2(0);	///< 表示位置をずらす距離
			float xadvance = 0;					///< カーソルを進める距離
		};

		std::vector<Texture::Image2DPtr> textureList;	///< フォントに使用するテクスチャ
		std::vector<CharacterInfo> characterInfoList;	///< 文字情報保存データ
		Shader::ProgramPtr progFont = nullptr;			///< フォント描画用シェーダ
		std::string texFilename;						///< フォントテクスチャパス

		glm::vec2 scale = {};		///<文字スケール
		bool valid = true;			///< フォント有効状態

		float lineHeight = 0;
		float base = 0;
		
	};

	class Character {
	public:



	private:

	};

	class Buffer {
		friend class FontData;
	public:

		/**
		*	バッファのインスタンスの取得
		*
		*	@return バッファのインスタンス
		*/
		static Buffer& Instance();

		/**
		*	フォントを画像ファイルから作成する
		*
		*	@detail 作成したフォントはフォントバッファに格納されており、
		*			フォントデータを取得するにはGetFont関数を使用して下さい
		*
		*	@param filename	使用するフォント名
		*
		*	@return フォントオブジェクト
		*/
		FontDataPtr CreateFontFromFile(const char* filename);

		/**
		*	フォントを取得する
		*
		*	@param name	フォント名
		*
		*	@return	フォントオブジェクト
		*/
		FontDataPtr GetFont(const char* name);

		/**
		*	フォントを削除します
		*
		*	@param name	削除するフォント名
		*/
		void DestroyFont(const char* name);

	private:

		Buffer() = default;
		~Buffer() {}
		Buffer(const Buffer&) = delete;
		const Buffer& operator=(const Buffer&) = delete;
		
	private:

		std::unordered_map<std::string, std::shared_ptr<FontData> > fontList;	///< フォントデータ格納先

	};


	class Renderer {
		friend class FontData;
		friend class Buffer;
	public:

		/**
		*	初期化処理
		*
		*	@param maxChar	描画可能な最大文字数
		*	@param screenSize	描画する画面のサイズ
		*/
		bool Init(size_t maxChar, glm::ivec2 screenSize);

		/**
		*	バッファをマッピング開始する
		*/
		void MapBuffer();

		/**
		*	マッピングを終了する
		*/
		void UnMapBuffer();

		/**
		*	描画処理
		*/
		void Draw();

		/**
		*	文字列書き込み処理
		*
		*	@param
		*/
		void AddString(const glm::vec2& position, const wchar_t* str, FontDataPtr font);

		/**
		*	ウインドウサイズを変更する
		*/
		void WindowSize(glm::ivec2 newSize) { windowSize = newSize; }

		/**
		*	シェーダプログラムの設定
		*
		*	@param program	プログラムオブジェクトのポインタ
		*/
		void Shader(Shader::ProgramPtr program) { shader = program; }

	private:

		glm::ivec2 windowSize;			///< 適用されているウインドウサイズ
		BufferObject vbo, ibo;			///< フォントの頂点データ格納場所
		VertexArrayObject vao;	
		glm::vec2 screenSize;			///< スクリーンサイズ
		glm::vec2 reciprocalScreenSize;	///< スクリーンサイズの逆数
		GLsizei vboCapacity;			///< 描画に使用するVBOのキャパシティ

		Shader::ProgramPtr shader;		///< 描画に使用されるシェーダ

		// フォント用頂点構造体
		struct Vertex {
			glm::vec3 position;
			glm::vec2 texcoord;
			glm::vec4 color;
		};

		// プリミティブ構造体
		struct Primitive {
			size_t count;					///< プリミティブ数
			size_t offset;					///< 頂点データ先頭からのオフセット
			Texture::Image2DPtr texture;	///< 使用しているテクスチャ
		};

		std::vector<Vertex> vertices;		///< 頂点バッファへ格納するための前バッファ
		std::vector<Primitive> primitives;	///< プリミティブバッファ
	};








}