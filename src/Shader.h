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
		*	�p�����[�^�ݒ菈��
		*	
		*/
		template <typename T>
		void SetParameter(T param) {

		}

		/**
		*	���̃v���O�����I�u�W�F�N�g���L�����ǂ������ׂ�
		*
		*	@retval	true	�L��	
		*	@retval false	����
		*/
		bool isValid() const { return program != 0; }

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

		GLuint program;

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