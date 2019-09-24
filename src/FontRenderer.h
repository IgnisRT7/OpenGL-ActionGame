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
	public:

	private:

		FontData() = default;
		~FontData() {}
		FontData(const FontData&) = delete;
		const FontData& operator=(const FontData&) = delete;

		/// フォント情報
		struct FontInfo {
			int id = -1;		///< 文字コード
			glm::u16vec2 uv[2];	///< フォント画像のテクスチャ座標
			glm::vec2 size;		///< フォント画像の表示サイズ
			glm::vec2 offset;	///< 表示位置をずらす距離
			float xadvance = 0;	///< カーソルを進める距離
		};

		std::vector<TexturePtr> fontTexture;	///< フォントに使用するテクスチャ
		Shader::ProgramPtr progFont = nullptr;	///< フォント描画用シェーダ

		
	};

	class Buffer {
	public:

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
		static FontDataPtr CreateFontFromFile(const char* filename);

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
		
		std::unordered_map<std::string, FontDataPtr> fontList;
		
	
	};


	class Renderer {
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
		void AddString();

		/**
		*	ウインドウサイズを変更する
		*/
		void WindowSize(glm::ivec2 newSize) { windowSize = newSize; }

	private:

		glm::ivec2 windowSize;	///< 適用されているウインドウサイズ
		BufferObject vbo, ibo;	///< フォントの頂点データ格納場所
		VertexArrayObject vao;	
		glm::vec2 reciprocalScreenSize;	///< スクリーンサイズの逆数
		GLsizei vboCapacity;	///< 描画に使用するVBOのキャパシティ
	};








}