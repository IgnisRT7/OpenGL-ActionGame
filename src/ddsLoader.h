/**
*	@file ddsLoader.h
*/

#pragma once
#include <stdio.h>
#include <sys/stat.h>
#include <string>
#include <GL/glew.h>

namespace DDSLoader{

	/**
	* FOURCCを作成する.
	*/
	#define MAKE_FOURCC(a, b, c, d) \
	  static_cast<uint32_t>(a + (b << 8) + (c << 16) + (d << 24))

	/**
	* DDS画像情報.
	*/
	struct DDSPixelFormat
	{
		uint32_t size; ///< この構造体のバイト数(32).
		uint32_t flgas; ///< 画像に含まれるデータの種類を示すフラグ.
		uint32_t fourCC; ///< 画像フォーマットを示すFOURCC.
		uint32_t rgbBitCount; ///< 1ピクセルのビット数.
		uint32_t redBitMask; ///< 赤要素が使う部分を示すビット.
		uint32_t greenBitMask; ///< 緑要素が使う部分を示すビット.
		uint32_t blueBitMask; ///< 青要素が使う部分を示すビット.
		uint32_t alphaBitMask; ///< 透明要素が使う部分を示すビット.
	};

	/**
	* DDSファイルヘッダ.
	*/
	struct DDSHeader
	{
		uint32_t size;  ///< この構造体のバイト数(124).
		uint32_t flags; ///< どのパラメータが有効かを示すフラグ.
		uint32_t height; ///< 画像の高さ(ピクセル数).
		uint32_t width; ///< 画像の幅(ピクセル数).
		uint32_t pitchOrLinearSize; ///< 横のバイト数または画像1枚のバイト数.
		uint32_t depth; ///< 画像の奥行き(枚数)(3次元テクスチャ等で使用).
		uint32_t mipMapCount; ///< 含まれているミップマップレベル数.
		uint32_t reserved1[11]; ///< (将来のために予約されている).
		DDSPixelFormat ddspf; ///< DDS画像情報.
		uint32_t caps[4]; ///< 含まれている画像の種類.
		uint32_t reserved2; ///< (将来のために予約されている).
	};

	/**
	*	バイト列から数値を復元する
	*
	*	@param p		バイト列へのポインタ
	*	@param offset	数値のオフセット
	*	@param size		数値のバイト数(1～4)
	*
	*	@return 復元した数値
	*/
	uint32_t Get(const uint8_t* p, size_t offset, size_t size);

	/**
	* バッファからDDS画像情報を読み出す.
	*
	* @param buf 読み出し元バッファ.
	*
	* @return 読み出したDDS画像情報.
	*/
	DDSPixelFormat ReadDDSPixelFormat(const uint8_t* buf);

	/**
	* バッファからDDSファイルヘッダを読み出す.
	*
	* @param buf 読み出し元バッファ.
	*
	* @return 読み出したDDSファイルヘッダ.
	*/
	DDSHeader ReadDDSHeader(const uint8_t* buf);

	/**
	* DDSファイルからテクスチャを作成する.
	*
	* @param filename DDSファイル名.
	* @param st       DDSファイルステータス.
	* @param buf      ファイルを読み込んだバッファ.
	* @param header   DDSヘッダ格納先へのポインタ.
	*
	* @retval 0以外 作成したテクスチャID.
	* @retval 0     作成失敗.
	*/
	GLuint LoadDDS(const char* filename, const struct stat& st,
		const uint8_t* buf, DDSHeader* pHeader);
}