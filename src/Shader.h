/**
*	@file Shader.h
*/
#pragma once
#include <unordered_map>
#include <memory>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Shader {

	//�O���錾
	class Program;
	using ProgramPtr = std::shared_ptr<Program>;

	/**
	*	�v���O�����I�u�W�F�N�g�̊Ǘ��N���X
	*/
	class Program {
	public:

		/**
		*	�v���O�����쐬����
		*
		*	@param vsPass ���_�V�F�[�_�̃t�@�C���p�X
		*	@param fsPass �t���O�����g�V�F�[�_�̃t�@�C���p�X
		*/
		static ProgramPtr Create(const char* vsPass, const char* fsPass);

		/**
		*	�v���O�����I�u�W�F�N�g�̗L����
		*
		*	@retval true	�L��������
		*	@retval false	�L�������s
		*/
		bool UseProgram() const;

		/**
		*	���̃v���O�����I�u�W�F�N�g���L�����ǂ������ׂ�
		*
		*	@retval	true	�L��	
		*	@retval false	����
		*/
		bool isValid() const { return program != 0; }

		/**
		*	�e�N�X�`���̃o�C���h�����܂�
		*	
		*	@param unit	�K�p����e�N�X�`���̃��j�b�g�ԍ�
		*/
		void BindTexture(GLuint unit, GLuint texture, GLuint type);

		/**
		*	�r���[�ˉe�ϊ��s��̐ݒ菈��
		*
		*	@param matVP	�r���[�ˉe�ϊ��s��
		*/
		void SetViewProjectionMatrix(glm::mat4 matVP);

	private:

		Program() = default;
		~Program();
		Program(const Program&) = delete;
		const Program& operator=(const Program&) = delete;

	private:

		GLint matVPLocation = -1;

		int samperCount = 0;		///< �e�N�X�`���T���v���[��
		GLint samplerLocation = -1;	///< �e�N�X�`���T���v���[�ʒu

		GLuint program;				///< �v���O�����I�u�W�F�N�gID

	};



	/**
	*	�v���O�����I�u�W�F�N�g�̃o�b�t�@�N���X
	*/
	class Buffer {
	public:

		/**
		*	�v���O�����쐬����
		*
		*	@param vsPass
		*	@param fsPass
		*/
		static bool Create(const char* vsPass, const char* fsPass);


	private:

		std::unordered_map<std::string, ProgramPtr> shaderList;
	};


}