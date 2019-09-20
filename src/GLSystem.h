/**
*	GLSystem.h
*/

#pragma once

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
		bool isInitialized;				///< �������t���O
	};

}