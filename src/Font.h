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
		friend class Renderer;
	public:

		/**
		*	�t�H���g���L�����ۂ����擾
		*
		*	@retval true	�L����
		*	@retval false	������
		*/
		bool IsValid() const { return valid; }

	private:

		FontData() = default;
		~FontData() {}
		FontData(const FontData&) = delete;
		const FontData& operator=(const FontData&) = delete;

	private:

		/// �t�H���g���
		struct CharacterInfo {
			int id = -1;						///< �����R�[�h
			int page = 0;						///< �������܂܂��t�H���g�e�N�X�`���ԍ�
			glm::vec2 uv = glm::vec2(0);		///< �t�H���g�摜�̃e�N�X�`�����W
			glm::vec2 size = glm::vec2(0);		///< �t�H���g�摜�̕\���T�C�Y
			glm::vec2 offset = glm::vec2(0);	///< �\���ʒu�����炷����
			float xadvance = 0;					///< �J�[�\����i�߂鋗��
		};

		std::vector<Texture::Image2DPtr> textureList;	///< �t�H���g�Ɏg�p����e�N�X�`��
		std::vector<CharacterInfo> characterInfoList;	///< �������ۑ��f�[�^
		Shader::ProgramPtr progFont = nullptr;			///< �t�H���g�`��p�V�F�[�_
		std::string texFilename;						///< �t�H���g�e�N�X�`���p�X

		glm::vec2 scale = {};		///<�����X�P�[��
		bool valid = true;			///< �t�H���g�L�����

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
		*	�o�b�t�@�̃C���X�^���X�̎擾
		*
		*	@return �o�b�t�@�̃C���X�^���X
		*/
		static Buffer& Instance();

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
		FontDataPtr CreateFontFromFile(const char* filename);

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

		Buffer() = default;
		~Buffer() {}
		Buffer(const Buffer&) = delete;
		const Buffer& operator=(const Buffer&) = delete;
		
	private:

		std::unordered_map<std::string, std::shared_ptr<FontData> > fontList;	///< �t�H���g�f�[�^�i�[��

	};


	class Renderer {
		friend class FontData;
		friend class Buffer;
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
		void AddString(const glm::vec2& position, const wchar_t* str, FontDataPtr font);

		/**
		*	�E�C���h�E�T�C�Y��ύX����
		*/
		void WindowSize(glm::ivec2 newSize) { windowSize = newSize; }

		/**
		*	�V�F�[�_�v���O�����̐ݒ�
		*
		*	@param program	�v���O�����I�u�W�F�N�g�̃|�C���^
		*/
		void Shader(Shader::ProgramPtr program) { shader = program; }

	private:

		glm::ivec2 windowSize;			///< �K�p����Ă���E�C���h�E�T�C�Y
		BufferObject vbo, ibo;			///< �t�H���g�̒��_�f�[�^�i�[�ꏊ
		VertexArrayObject vao;	
		glm::vec2 screenSize;			///< �X�N���[���T�C�Y
		glm::vec2 reciprocalScreenSize;	///< �X�N���[���T�C�Y�̋t��
		GLsizei vboCapacity;			///< �`��Ɏg�p����VBO�̃L���p�V�e�B

		Shader::ProgramPtr shader;		///< �`��Ɏg�p�����V�F�[�_

		// �t�H���g�p���_�\����
		struct Vertex {
			glm::vec3 position;
			glm::vec2 texcoord;
			glm::vec4 color;
		};

		// �v���~�e�B�u�\����
		struct Primitive {
			size_t count;					///< �v���~�e�B�u��
			size_t offset;					///< ���_�f�[�^�擪����̃I�t�Z�b�g
			Texture::Image2DPtr texture;	///< �g�p���Ă���e�N�X�`��
		};

		std::vector<Vertex> vertices;		///< ���_�o�b�t�@�֊i�[���邽�߂̑O�o�b�t�@
		std::vector<Primitive> primitives;	///< �v���~�e�B�u�o�b�t�@
	};








}