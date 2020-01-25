/**
*	@file	Texture.h
*	@brief	�e�N�X�`���̐����E�Ǘ�
*	@author	tn-mai(�u�`���������)
*/

#pragma once

#include <memory>
#include <GL/glew.h>
#include <unordered_map>

namespace Texture {

	class Image2D;
	using Image2DPtr = std::weak_ptr<Image2D>;	///< �e�N�X�`���|�C���^
	using Image2DPtr_s = std::shared_ptr<Image2D>;	///< �e�N�X�`���|�C���^(shared�p)

	/**
	*	�e�N�X�`���N���X
	*/
	class Image2D {
		friend class Buffer;
	public:

		/**
		*	�e�N�X�`���̎擾
		*
		*	@return �e�N�X�`��ID
		*/
		GLuint Id() const { return texId; }

		/**
		*	�e�N�X�`���^�[�Q�b�g���擾
		*
		*	@return �e�N�X�`���^�[�Q�b�g
		*/
		GLuint Target() const { return target; }

		/**
		*	�e�N�X�`���E���b�v�E���[�h�̐ݒ�
		*
		*	�K�p����e�N�X�`���E���b�v�E���[�h
		*/
		void SetWrapMode(GLenum mode);

		/**
		*	�e�N�X�`�����L�����ۂ����擾
		*
		*	@retval true	�L����
		*	@retval false	������
		*/
		bool IsValid() const { return texId != 0; }

		/**
		*	�e�N�X�`���̉������擾
		*
		*	@return �e�N�X�`���̉���
		*/
		GLsizei Width() const { return width; }

		/**
		*	�e�N�X�`���̏c�����擾
		*
		*	@return �e�N�X�`���̏c��
		*/
		GLsizei Height() const { return height; }

		/**
		*	�e�N�X�`�������擾����
		*
		*	@return �e�N�X�`����
		*/
		std::string Name() const { texName; }

		/**
		*	�e�N�X�`������ݒ肷��
		*
		*	@param name	�K�p����e�N�X�`����
		*/	
		void Name(const char* name) { texName = name; }

	private:

		Image2D() = default;
		~Image2D();
		Image2D(const Image2D&) = delete;
		Image2D& operator=(const Image2D&) = delete;

		GLenum target = GL_TEXTURE_2D;	///< �e�N�X�`���E�^�[�Q�b�g
		GLuint texId = 0;				///< �e�N�X�`����ID
		int width = 0;					///< �e�N�X�`���̕�
		int height = 0;					///< �e�N�X�`���̍���
		std::string texName;			///< �e�N�X�`����(�f�t�H���g�Ńt�@�C����)
	};

	/**
	*	�e�N�X�`���ۊǃN���X
	*/
	class Buffer {
	public:

		/**
		*	�V���O���g���C���X�^���X�̎擾
		*
		*	@return �V���O���g���C���X�^���X
		*/
		static Buffer& Instance();

		/**
		*	�t�@�C������2D�e�N�X�`����ǂݍ���
		*
		*	@param	filename �t�@�C����
		*
		*	@return �쐬�ɐ��������ꍇ�̓e�N�X�`���|�C���^��Ԃ�
		*			���s�����ꍇ��nullptr��Ԃ�
		*/
		static Image2DPtr LoadFromFile(const char*);

		/**
		*	2D�e�N�X�`�����쐬����
		*
		*	@param width	�e�N�X�`���̕�(�s�N�Z����)
		*	@param height	�e�N�X�`���̍���(�s�N�Z����)
		*	@param iformat	�e�N�X�`���̃f�[�^�`��
		*	@param format	�A�N�Z�X����v�f
		*	@param data		�e�N�X�`���̃f�[�^�ւ̃|�C���^
		*
		*	@return �쐬�ɐ��������ꍇ�̓e�N�X�`���|�C���^��Ԃ�
		*			���s�����ꍇ��nullptr��Ԃ�
		*/
		static Image2DPtr Create(int width, int height, GLenum iformat, GLenum format, const void* data);

		/**
		*	�o�b�t�@������w�肵���e�N�X�`�����W�F�N�g���擾���܂�
		*
		*	@param name	�e�N�X�`���I�u�W�F�N�g��
		*	
		*	@retval texture	���������e�N�X�`���I�u�W�F�N�g��
		*	@retval nullptr	�w�肵���e�N�X�`�������݂��Ă��Ȃ�����
		*/
		Image2DPtr FindTexture(const char* name);
	
	private:

		Buffer() = default;
		~Buffer();
		Buffer(const Buffer&) = delete;
		const Buffer& operator=(const Buffer&) = delete;

	private:

		using ImageMapType = std::unordered_map<std::string, Image2DPtr_s >;	///< �e�N�X�`���ۑ��p�L�[�^�C�v
		
		
		ImageMapType imageList;	///< �e�N�X�`�����X�g

	};
}