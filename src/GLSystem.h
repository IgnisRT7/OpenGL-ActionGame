/**
*	GLSystem.h
*/

#pragma once

#include "GamePad.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace GLSystem {

	/**
	*	GL�𗘗p�����V�X�e���Ǘ��N���X
	*	tips ���̃N���X�̓V���O���g���ŊǗ������
	*
	*
	*
	*
	*
	*/
	class Window {
	public:

		/**
		*	�V���O���g���C���X�^���X�̎擾
		*
		*	@return �V���O���g���C���X�^���X
		*/
		static Window& Instance();

		/**
		*	��ʂ̉������擾
		*
		*	@return ��ʂ̉���
		*/
		int Width() const { return width; }

		/**
		*	��ʂ̏c�����擾
		*
		*	@return ��ʏc��
		*/
		int Height() const { return height; }

		/**
		*	����������
		*
		*	@param width	�E�C���h�E�̉���
		*	@param height	�E�C���h�E�̏c��
		*	@param title	�E�C���h�E�̃^�C�g����
		*
		*	@return true	����������
		*	@return false	���������s
		*/
		bool Init(int width,int height,const char* title);

		/**
		*	�E�B���h�E�����ׂ������ׂ�
		*
		*	@return true	����
		*	@return false	���Ȃ�
		*/
		bool ShouldClose();

		/**
		*	�o�b�N�o�b�t�@�̃f�[�^���t�����g�o�b�t�@�ɔ��f������
		*/
		void SwapBuffers();

		const GLFWwindow* operator()() { return window; }

		///---- ��������͂̂��Ɏ����\�� ----///
		/*
		*	�o�ߎ��Ԃ̌v�Z����
		*	���͏��̎擾

		*/

	private:

		Window() = default;
		~Window();
		Window(const Window&) = delete;
		const Window& operator=(const Window&) = delete;

		GLFWwindow* window = nullptr;	///< �E�C���h�E�Ǘ��n���h��
		Input input;					///< ���͏��Ǘ�

		bool isInitialized = false;		///< �������ς݃t���O
		bool glfwInitialized = false;	///< GLFW�������ς݃t���O

		

		int height = 0;					///< �E�C���h�E�̏c��
		int width = 0;					///< �E�C���h�E�̉���
	};

}