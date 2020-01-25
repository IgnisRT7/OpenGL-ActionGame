/**
*	@file Shader.h
*/
#pragma once
#include <map>
#include <unordered_map>
#include <memory>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

namespace Shader {

	//�O���錾
	class Program;
	using ProgramPtr = std::weak_ptr<Program>;
	using ProgramPtr_s = std::shared_ptr<Program>;

	/**
	*	���P�[�V�����p�����[�^�ŗ��p����郊�X�g
	*/
	enum class LocationType {

		ModelMatrix,
		ViewMatrix,
		InverseViewMatrix,
		ProjectionMatrix,
		ViewProjectionMatrix,
	};

	/**
	*	���P�[�V�����^�C�v�ɉ������V�F�[�_�̃l�[�����X�g
	*/
	static std::map<LocationType, std::string> locationNameMap = {

		{ LocationType::ModelMatrix, "matModel" },
		{ LocationType::ViewProjectionMatrix, "matVP" },
	};

	/**
	*	�v���O�����I�u�W�F�N�g�̊Ǘ��N���X
	*/
	class Program {
		friend class Buffer;
	public:

		GLuint Id() const { return program; }
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
		void BindTexture(GLuint unit, GLuint texture, GLuint type = GL_TEXTURE_2D);

		/**
		*	�r���[�ˉe�ϊ��s��̐ݒ菈��
		*
		*	@param matVP	�r���[�ˉe�ϊ��s��
		*/
		void SetViewProjectionMatrix(const glm::mat4& matVP);
		
		/**
		*	���f���s��̐ݒ菈��
		*
		*	@param matM		���f���s��
		*/
		void SetModelMatrix(const glm::mat4& matM);

		/**
		*	���Ԃ̐ݒ菈��
		*
		*	@param time	����
		*/
		void SetTime(const float time) {}

		/**
		*	�J�������W�̐ݒ�
		*
		*	@param pos	���W
		*/
		void SetCameraPosition(const glm::vec3& pos) {}

	private:

		Program() = default;
		~Program();
		Program(const Program&) = delete;
		const Program& operator=(const Program&) = delete;

	private:
		
		std::map<LocationType, GLint> locationList;	///< �L���ȃ��P�[�V�������X�g

		GLint matVPLocation = -1;	///< MVP���P�[�V�����ʒu

		int samperCount = 0;		///< �e�N�X�`���T���v���[��
		GLint samplerLocation = -1;	///< �e�N�X�`���T���v���[�ʒu

		GLuint program = -1;		///< �v���O�����I�u�W�F�N�gID

	};

	/**
	*	�v���O�����I�u�W�F�N�g�̃o�b�t�@�N���X
	*/
	class Buffer {
	public:

		/**
		*	�o�b�t�@�C���X�^���X�̎擾
		*
		*	@return �V�F�[�_�o�b�t�@�C���X�^���X
		*/
		static Buffer& Instance();

		/**
		*	�v���O�����쐬����
		*
		*	@param vsPass	���_�V�F�[�_�̃t�@�C����
		*	@param fsPass	���_�V�F�[�_�̃t�@�C����
		*	@param shaderName	�V�F�[�_��
		*/
		static ProgramPtr Create(const char* vsPass, const char* fsPass, const char* shaderName = "no name");


	private:

		Buffer() = default;
		~Buffer();
		Buffer(const Buffer&) = delete;
		const Buffer& operator=(const Buffer&) = delete;

		using ShaderMapType = std::unordered_map<std::string, ProgramPtr_s>;
		ShaderMapType shaderList;	///< �V�F�[�_�̃��X�g
	};


}