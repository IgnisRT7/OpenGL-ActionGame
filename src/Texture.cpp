/**
*	@file Texture.cpp
*/
#include "Texture.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <sys/stat.h>
#include <string>
#include "ddsLoader.h"

namespace Texture {

	uint32_t Get(const uint8_t* p, size_t offset, size_t size) {
		uint32_t n = 0;
		p += offset;
		for (size_t i = 0; i < size; ++i) {
			n += p[i] << (i * 8);
		}
		return n;
	}

	void Image2D::SetWrapMode(GLenum mode){

		if (this->texId) {
			/*
			glBindTexture(,texId);
			glTexParameteri(texId, GL_TEXTURE_WRAP_S, mode);
			glTexParameteri(texId, GL_TEXTURE_WRAP_T, mode);
			glBindTexture(texId, 0);*/
		}
	}

	Image2D::~Image2D() {
		if (texId) {
			glDeleteTextures(1, &texId);
		}
	}

	Buffer& Buffer::Instance() {

		static Buffer instance;
		return instance;
	}

	Image2DPtr Buffer::LoadFromFile(const char* filename){

		struct stat st;
		if (stat(filename, &st)) {
			return {};
		}

		FILE* fp = NULL;
		fopen_s(&fp, filename, "rb");
		if (!fp) {
			//�t�@�C���ǂݍ��ݎ��s
			return {};
		}

		//�f�[�^���o��
		std::vector<uint8_t> buf;
		buf.resize(st.st_size);
		const size_t readSize = fread(buf.data(), 1, st.st_size, fp);
		fclose(fp);
		if (readSize != st.st_size) {
			//�f�[�^�����ׂĎ��o���Ȃ�����
			return {};
		}

		//�w�b�_�����o��
		const uint8_t* pHeader = buf.data();

		//DDS�t�@�C���ǂݍ���
		if (pHeader[0] == 'D' || pHeader[1] == 'D' || pHeader[2] == 'S'
			|| pHeader[3] == ' ') {
			DDSLoader::DDSHeader header;
			const GLuint texId = DDSLoader::LoadDDS(filename, st, buf.data(), &header);
			if (texId) {
				struct impl : Image2D {};
				auto p = std::make_shared<impl>();
				p->width = header.width;
				p->height = header.height;
				p->texId = texId;
				p->texName = filename;

				Buffer::Instance().imageList[filename] = p;
				return p;
			}
		}

		const size_t bmpFileHeaderSize = 14;	//�r�b�g�}�b�v�t�@�C���w�b�_�̃o�C�g��
		const size_t windowsV1HeaderSize = 40;	//�r�b�g�}�b�v���w�b�_�̃o�C�g��

		if (st.st_size < bmpFileHeaderSize + windowsV1HeaderSize) {
			//�V�X�e���T�C�Y���猩�ăr�b�g�}�b�v�t�@�C���ł͂Ȃ�
			return {};
		}
		if (pHeader[0] != 'B' || pHeader[1] != 'M') {
			//Windows�p��BMP�ӂ�����ł͂Ȃ�����
			return {};
		}

		const size_t offsetBytes = Get(pHeader, 10, 4);
		const uint32_t infoSize = Get(pHeader, 14, 4);
		const uint32_t width = Get(pHeader, 18, 4);
		const uint32_t height = Get(pHeader, 22, 4);
		const uint32_t bitCount = Get(pHeader, 28, 2);
		const uint32_t compression = Get(pHeader, 30, 4);
		const size_t pixelBytes = bitCount / 8;
		if (infoSize != windowsV1HeaderSize || bitCount != 24 || compression || (width * pixelBytes) % 4) {
			return {};
		}

		const size_t imageSize = width * height * pixelBytes;
		if (buf.size() < offsetBytes + imageSize) {
			return {};
		}

		auto p = Create(width, height, GL_RGB8, GL_BGR, buf.data() + offsetBytes);
		
		if (!p.expired()) {
			return {};
		}
		
		p.lock()->Name(filename);
		Buffer::Instance().imageList[filename] = p.lock();

		return p;
	}

	Buffer::~Buffer(){
	}

	Image2DPtr Buffer::Create(int width, int height, GLenum iformat, GLenum format, const void* data) {

		GLenum type;

		switch (iformat) {
		case GL_RGB10_A2:type = GL_UNSIGNED_INT_2_10_10_10_REV; break;
		case GL_RGBA16F: type = GL_HALF_FLOAT; break;
		case GL_DEPTH_COMPONENT16:type = GL_HALF_FLOAT; break;
		case GL_DEPTH_COMPONENT24:type = GL_UNSIGNED_INT; break;
		case GL_DEPTH_COMPONENT32:type = GL_UNSIGNED_INT; break;
		case GL_DEPTH_COMPONENT32F:type = GL_FLOAT; break;
		default: type = GL_UNSIGNED_BYTE;
		}

		struct Impl : Image2D {};
		auto p = std::make_shared<Impl>();
		p->width = width;
		p->height = height;
		glGenTextures(1, &p->texId);


		glBindTexture(GL_TEXTURE_2D, p->texId);
		glTexImage2D(GL_TEXTURE_2D, 0, iformat, width, height, 0, format, type, data);
		const GLenum result = glGetError();
		if (result != GL_NO_ERROR) {
			std::cerr << "ERROR �e�N�X�`���̍쐬�Ɏ��s 0x" << std::hex << result << std::endl;
			return {};
		}

		/**
		*	�e�N�X�`���̐ݒ�
		*	�~�b�v�}�b�v���g�p���Ȃ�
		*	�e�N�X�`�����k���\������ہALINEAR��Ԃ��s��
		*	�e�N�X�`�����g��\������ہALINEAR��Ԃ��s��
		*	�e�N�X�`����u�l���͈͊O�̎��A�[�̐F�œh��Ԃ���܂�
		*	�e�N�X�`����v�l���͈͊O�̎��A�[�̐F�œh��Ԃ���܂�
		*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D, 0);

		std::string str = "no name";


		auto& inst = Buffer::Instance();

		//�V�F�[�_���d���h�~����
		int counter = 0;
		std::string newShaderName = "no name";
		do {

			newShaderName = counter == 0 ? "no name" : "no name" + std::to_string(counter);
			counter++;

		} while (inst.imageList.find(newShaderName) != inst.imageList.end());

		inst.imageList[newShaderName] = p;

		return p;
	}

}