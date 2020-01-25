/**
*	@file	Texture.h
*	@brief	テクスチャの生成・管理
*	@author	tn-mai(講義資料製作者)
*/

#pragma once

#include <memory>
#include <GL/glew.h>
#include <unordered_map>

namespace Texture {

	class Image2D;
	using Image2DPtr = std::weak_ptr<Image2D>;	///< テクスチャポインタ
	using Image2DPtr_s = std::shared_ptr<Image2D>;	///< テクスチャポインタ(shared用)

	/**
	*	テクスチャクラス
	*/
	class Image2D {
		friend class Buffer;
	public:

		/**
		*	テクスチャの取得
		*
		*	@return テクスチャID
		*/
		GLuint Id() const { return texId; }

		/**
		*	テクスチャターゲットを取得
		*
		*	@return テクスチャターゲット
		*/
		GLuint Target() const { return target; }

		/**
		*	テクスチャ・ラップ・モードの設定
		*
		*	適用するテクスチャ・ラップ・モード
		*/
		void SetWrapMode(GLenum mode);

		/**
		*	テクスチャが有効か否かを取得
		*
		*	@retval true	有効化
		*	@retval false	無効化
		*/
		bool IsValid() const { return texId != 0; }

		/**
		*	テクスチャの横幅を取得
		*
		*	@return テクスチャの横幅
		*/
		GLsizei Width() const { return width; }

		/**
		*	テクスチャの縦幅を取得
		*
		*	@return テクスチャの縦幅
		*/
		GLsizei Height() const { return height; }

		/**
		*	テクスチャ名を取得する
		*
		*	@return テクスチャ名
		*/
		std::string Name() const { texName; }

		/**
		*	テクスチャ名を設定する
		*
		*	@param name	適用するテクスチャ名
		*/	
		void Name(const char* name) { texName = name; }

	private:

		Image2D() = default;
		~Image2D();
		Image2D(const Image2D&) = delete;
		Image2D& operator=(const Image2D&) = delete;

		GLenum target = GL_TEXTURE_2D;	///< テクスチャ・ターゲット
		GLuint texId = 0;				///< テクスチャのID
		int width = 0;					///< テクスチャの幅
		int height = 0;					///< テクスチャの高さ
		std::string texName;			///< テクスチャ名(デフォルトでファイル名)
	};

	/**
	*	テクスチャ保管クラス
	*/
	class Buffer {
	public:

		/**
		*	シングルトンインスタンスの取得
		*
		*	@return シングルトンインスタンス
		*/
		static Buffer& Instance();

		/**
		*	ファイルから2Dテクスチャを読み込む
		*
		*	@param	filename ファイル名
		*
		*	@return 作成に成功した場合はテクスチャポインタを返す
		*			失敗した場合はnullptrを返す
		*/
		static Image2DPtr LoadFromFile(const char*);

		/**
		*	2Dテクスチャを作成する
		*
		*	@param width	テクスチャの幅(ピクセル数)
		*	@param height	テクスチャの高さ(ピクセル数)
		*	@param iformat	テクスチャのデータ形式
		*	@param format	アクセスする要素
		*	@param data		テクスチャのデータへのポインタ
		*
		*	@return 作成に成功した場合はテクスチャポインタを返す
		*			失敗した場合はnullptrを返す
		*/
		static Image2DPtr Create(int width, int height, GLenum iformat, GLenum format, const void* data);

		/**
		*	バッファ内から指定したテクスチャ王ジェクトを取得します
		*
		*	@param name	テクスチャオブジェクト名
		*	
		*	@retval texture	見つかったテクスチャオブジェクト名
		*	@retval nullptr	指定したテクスチャが存在していなかった
		*/
		Image2DPtr FindTexture(const char* name);
	
	private:

		Buffer() = default;
		~Buffer();
		Buffer(const Buffer&) = delete;
		const Buffer& operator=(const Buffer&) = delete;

	private:

		using ImageMapType = std::unordered_map<std::string, Image2DPtr_s >;	///< テクスチャ保存用キータイプ
		
		
		ImageMapType imageList;	///< テクスチャリスト

	};
}