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
*	1,�t�H���g�ǂݍ��� -����Ƃ��ēǂݍ��߂�t�H���g��1��ނ̂�
*	2,
*
*
*/

	//�O���錾
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

		/// �t�H���g���
		struct FontInfo {
			int id = -1;		///< �����R�[�h
			glm::u16vec2 uv[2];	///< �t�H���g�摜�̃e�N�X�`�����W
			glm::vec2 size;		///< �t�H���g�摜�̕\���T�C�Y
			glm::vec2 offset;	///< �\���ʒu�����炷����
			float xadvance = 0;	///< �J�[�\����i�߂鋗��
		};

		std::vector<TexturePtr> fontTexture;	///< �t�H���g�Ɏg�p����e�N�X�`��
		Shader::ProgramPtr progFont = nullptr;	///< �t�H���g�`��p�V�F�[�_

		
	};

	class Buffer {
	public:

		/**
		*	�t�H���g���摜�t�@�C������쐬����
		*
		*	@detail �쐬�����t�H���g�̓t�H���g�o�b�t�@�Ɋi�[����Ă���A
		*			�t�H���g�f�[�^���擾����ɂ�GetFont�֐����g�p���ĉ�����
		*
		*	@param filename	�g�p����t�H���g��
		*
		*	@return �t�H���g�I�u�W�F�N�g
		*/
		static FontDataPtr CreateFontFromFile(const char* filename);

		/**
		*	�t�H���g���擾����
		*
		*	@param name	�t�H���g��
		*
		*	@return	�t�H���g�I�u�W�F�N�g
		*/
		FontDataPtr GetFont(const char* name);

		/**
		*	�t�H���g���폜���܂�
		*
		*	@param name	�폜����t�H���g��
		*/
		void DestroyFont(const char* name);

	private:
		
		std::unordered_map<std::string, FontDataPtr> fontList;
		
	
	};


	class Renderer {
	public:

		/**
		*	����������
		*
		*	@param maxChar	�`��\�ȍő啶����
		*	@param screenSize	�`�悷���ʂ̃T�C�Y
		*/
		bool Init(size_t maxChar, glm::ivec2 screenSize);

		/**
		*	�o�b�t�@���}�b�s���O�J�n����
		*/
		void MapBuffer();

		/**
		*	�}�b�s���O���I������
		*/
		void UnMapBuffer();

		/**
		*	�`�揈��
		*/
		void Draw();

		/**
		*	�����񏑂����ݏ���
		*
		*	@param
		*/
		void AddString();

		/**
		*	�E�C���h�E�T�C�Y��ύX����
		*/
		void WindowSize(glm::ivec2 newSize) { windowSize = newSize; }

	private:

		glm::ivec2 windowSize;	///< �K�p����Ă���E�C���h�E�T�C�Y
		BufferObject vbo, ibo;	///< �t�H���g�̒��_�f�[�^�i�[�ꏊ
		VertexArrayObject vao;	
		glm::vec2 reciprocalScreenSize;	///< �X�N���[���T�C�Y�̋t��
		GLsizei vboCapacity;	///< �`��Ɏg�p����VBO�̃L���p�V�e�B
	};








}