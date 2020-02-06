/**
*	@file ddsLoader.cpp
*/
#include "ddsLoader.h"

#include <iostream>
#include <vector>
#include <algorithm>

namespace DDSLoader {

	uint32_t Get(const uint8_t* p, size_t offset, size_t size) {
		uint32_t n = 0;
		p += offset;
		for (size_t i = 0; i < size; ++i) {
			n += p[i] << (i * 8);
		}
		return n;
	}

	DDSPixelFormat ReadDDSPixelFormat(const uint8_t* buf)
	{
		DDSPixelFormat tmp;
		tmp.size = Get(buf, 0, 4);
		tmp.flgas = Get(buf, 4, 4);
		tmp.fourCC = Get(buf, 8, 4);
		tmp.rgbBitCount = Get(buf, 12, 4);
		tmp.redBitMask = Get(buf, 16, 4);
		tmp.greenBitMask = Get(buf, 20, 4);
		tmp.blueBitMask = Get(buf, 24, 4);
		tmp.alphaBitMask = Get(buf, 28, 4);
		return tmp;
	}

	DDSHeader ReadDDSHeader(const uint8_t* buf)
	{
		DDSHeader tmp = {};
		tmp.size = Get(buf, 0, 4);
		tmp.flags = Get(buf, 4, 4);
		tmp.height = Get(buf, 8, 4);
		tmp.width = Get(buf, 12, 4);
		tmp.pitchOrLinearSize = Get(buf, 16, 4);
		tmp.depth = Get(buf, 20, 4);
		tmp.mipMapCount = Get(buf, 24, 4);
		tmp.ddspf = ReadDDSPixelFormat(buf + 28 + 4 * 11);
		for (int i = 0; i < 4; ++i) {
			tmp.caps[i] = Get(buf, 28 + 4 * 11 + 32 + i * 4, 4);
		}
		return tmp;
	}

	GLuint LoadDDS(const char* filename, const struct stat& st,
		const uint8_t* buf, DDSHeader* pHeader) {

		if (st.st_size < 128) {
			std::cerr << "WARNING: " << filename << "はDDSファイルではありません." << std::endl;
			return 0;
		}
		const DDSHeader header = ReadDDSHeader(buf + 4);
		if (header.size != 124) {
			std::cerr << "WARNING: " << filename << "はDDSファイルではありません." << std::endl;
			return 0;
		}

		GLenum iformat = GL_RGBA8;
		GLenum format = GL_RGBA;
		uint32_t blockSize = 16;
		bool isCompressed = false;
		if (header.ddspf.flgas & 0x04) {
			// 圧縮フォーマット
			switch (header.ddspf.fourCC) {
			case MAKE_FOURCC('D', 'X', 'T', '1'):
				iformat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				blockSize = 8;
				break;
			case MAKE_FOURCC('D', 'X', 'T', '2'):
			case MAKE_FOURCC('D', 'X', 'T', '3'):
				iformat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				break;
			case MAKE_FOURCC('D', 'X', 'T', '4'):
			case MAKE_FOURCC('D', 'X', 'T', '5'):
				iformat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				break;
			case MAKE_FOURCC('B', 'C', '4', 'U'):
				iformat = GL_COMPRESSED_RED_RGTC1;
				break;
			case MAKE_FOURCC('B', 'C', '4', 'S'):
				iformat = GL_COMPRESSED_SIGNED_RED_RGTC1;
				break;
			case MAKE_FOURCC('B', 'C', '5', 'U'):
				iformat = GL_COMPRESSED_RG_RGTC2;
				break;
			case MAKE_FOURCC('B', 'C', '5', 'S'):
				iformat = GL_COMPRESSED_SIGNED_RG_RGTC2;
				break;
			default:
				std::cerr << "WARNING: " << filename << "は未対応のDDSファイルです." << std::endl;
				return 0;
			}
			isCompressed = true;
		}
		else if (header.ddspf.flgas & 0x40) {
			// 無圧縮フォーマット
			if (header.ddspf.redBitMask == 0xff) {
				iformat = header.ddspf.alphaBitMask ? GL_RGBA8 : GL_RGB8;
				format = header.ddspf.alphaBitMask ? GL_RGBA : GL_RGB;
			}
			else if (header.ddspf.blueBitMask == 0xff) {
				iformat = header.ddspf.alphaBitMask ? GL_RGBA8 : GL_RGB8;
				format = header.ddspf.alphaBitMask ? GL_BGRA : GL_BGR;
			}
			else {
				std::cerr << "WARNING: " << filename << "は未対応のDDSファイルです." << std::endl;
				return 0;
			}
		}
		else {
			std::cerr << "WARNING: " << filename << "は未対応のDDSファイルです." << std::endl;
			return 0;
		}

		const bool isCubemap = header.caps[1] & 0x200;
		const GLenum target = isCubemap ? GL_TEXTURE_CUBE_MAP_POSITIVE_X : GL_TEXTURE_2D;
		const int faceCount = isCubemap ? 6 : 1;

		GLuint texId;
		glGenTextures(1, &texId);
		glBindTexture(isCubemap ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, texId);
		const uint8_t* data = buf + 128;
		for (int faceIndex = 0; faceIndex < faceCount; ++faceIndex) {
			GLsizei curWidth = header.width;
			GLsizei curHeight = header.height;
			for (int mipLevel = 0; mipLevel < static_cast<int>(header.mipMapCount);
				++mipLevel) {
				uint32_t imageBytes;
				if (isCompressed) {
					imageBytes = ((curWidth + 3) / 4) * ((curHeight + 3) / 4) * blockSize;
					glCompressedTexImage2D(target + faceIndex, mipLevel, iformat,
						curWidth, curHeight, 0, imageBytes, data);
				}
				else {
					imageBytes = curWidth * curHeight * 4;
					glTexImage2D(target + faceIndex, mipLevel, iformat,
						curWidth, curHeight, 0, format, GL_UNSIGNED_BYTE, data);
				}
				const GLenum result = glGetError();
				if (result != GL_NO_ERROR) {
					std::cerr << "WARNING: " << filename << "の読み込みに失敗("
						<< std::hex << result << ")." << std::endl;
				}
				curWidth = std::max(1, curWidth / 2);
				curHeight = std::max(1, curHeight / 2);
				data += imageBytes;
			}
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, header.mipMapCount - 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, header.mipMapCount <= 1 ? GL_LINEAR : GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);

		*pHeader = header;
		return texId;
	}
}
