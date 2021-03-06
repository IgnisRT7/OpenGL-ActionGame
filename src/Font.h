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

	//前方宣言
	class FontData;
	using FontDataPtr = std::weak_ptr<FontData>;

	/**
	*	フォント情報が格納されているクラス
	*/
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

		/**
		*	描画に使用するシェーダの設定
		*
		*	@param shader	適用するシェーダ
		*/
		void Shader(Shader::ProgramPtr shader) { progFont = shader; }

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
		Shader::ProgramPtr progFont;					///< フォント描画用シェーダ
		std::string texFilename;						///< フォントテクスチャパス

		glm::vec2 scale = {};		///<文字スケール
		bool valid = true;			///< フォント有効状態

		float lineHeight = 0;
		float base = 0;
	};

	class Character {
	public:




	private:

		std::string str;	///< 描画用文字列
		glm::vec3 pos;
		glm::vec2 k;
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
		*	@detail 作成したフォントはこのバッファに格納されており、
		*			フォントデータを取得するにはGetFont関数を使用すること
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

		Renderer() = default;
		~Renderer() {}
		Renderer(const Renderer&) = delete;
		const Renderer& operator=(const Renderer&) = delete;

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
		void AddString(const glm::vec3& position, const wchar_t* str, FontDataPtr font);

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
		void AddVertices(FontData::CharacterInfo& charaData,Texture::Image2DPtr texture,glm::vec3 position);

	private:

		glm::ivec2 windowSize = glm::ivec2(2048,1086);	///< 適用されているウインドウサイズ
		BufferObject vbo, ibo;							///< フォントの頂点データ格納場所
		VertexArrayObject vao;	
		glm::vec2 screenSize = glm::vec2(2048, 1086);	///< スクリーンサイズ
		glm::vec2 reciprocalScreenSize = glm::vec2(1) / screenSize;	///< スクリーンサイズの逆数
		GLsizei vboCapacity = 0;						///< 描画に使用するVBOのキャパシティ

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

		struct CharInfo {
			
		};

		std::vector<Vertex> vertices;		///< 頂点バッファへ格納するための前バッファ
		std::vector<Primitive> primitives;	///< プリミティブバッファ
	};

}