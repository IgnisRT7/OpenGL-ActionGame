/**
*	@file Shader.cpp
*/
#include "Shader.h"

#include <iostream>
#include <string>

namespace Shader {

	/**
	*	�t�@�C������V�F�[�_�f�[�^�����o��
	*
	*	@param filename	�ǂݍ��ރt�@�C����
	*	@param buf		�ǂݍ��݌�̃f�[�^
	*
	*	@retval true	�ǂݍ��ݐ���
	*	@retval false	�ǂݍ��ݎ��s
	*/
	bool ReadFromFile(const char* filename, std::vector<char>& buf) {

		std::cout << "Reading '" << filename << "' shader...";

		try {

			struct stat st;
			if (stat(filename, &st)) {
				throw(" error. Invalid file format!! \n");
			}

			FILE* fp = NULL;
			fopen_s(&fp, filename, "rb");
			if (!fp) {
				throw(" error. Could not open file!! \n");
			}

			buf.resize(st.st_size + 1);
			const size_t readSize = fread(buf.data(), 1, st.st_size, fp);
			fclose(fp);
			if (readSize != st.st_size) {
				throw(" error. Could not read all data!! \n");
			}
		}
		catch (const char* errorStr) {
			std::cout << errorStr << std::endl;
			return false;
		}

		buf.back() = '\0';
		return true;
	}


	/**
	* �V�F�[�_�R�[�h���R���p�C������.
	*
	* @param type �V�F�[�_�̎��.
	* @param string �V�F�[�_�R�[�h�ւ̃|�C���^.
	*
	* @return �쐬�����V�F�[�_�I�u�W�F�N�g.
	*/
	GLuint CompileShader(GLenum type, const GLchar* string){

		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &string, nullptr);
		glCompileShader(shader);
		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen) {
				std::vector<char> buf;
				buf.resize(infoLen);
				if (static_cast<int>(buf.size()) >= infoLen) {
					glGetShaderInfoLog(shader, infoLen, NULL, buf.data());
					std::cerr << "\n[Error]: �V�F�[�_�̃R���p�C���Ɏ��s��n" << buf.data() << std::endl;
				}
			}
			glDeleteShader(shader);
			return 0;
		}

		std::cout << "completed." << std::endl;
		return shader;
	}

	/**
	* �v���O�����I�u�W�F�N�g���쐬����.
	*
	* @param vsCode ���_�V�F�[�_�R�[�h�ւ̃|�C���^.
	* @param fsCode �t���O�����g�V�F�[�_�R�[�h�ւ̃|�C���^.
	*
	* @return �쐬�����v���O�����I�u�W�F�N�g.
	*/
	GLuint CreateShaderProgram(const GLchar* vsCode, const GLchar* fsCode){

		GLuint vs = CompileShader(GL_VERTEX_SHADER, vsCode);
		GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fsCode);
		if (!vs || !fs) {
			return 0;
		}
		GLuint program = glCreateProgram();
		glAttachShader(program, fs);
		glDeleteShader(fs);
		glAttachShader(program, vs);
		glDeleteShader(vs);
		glLinkProgram(program);
		GLint linkStatus = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE) {
			GLint infoLen = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen) {
				std::vector<char> buf;
				buf.resize(infoLen);
				if (static_cast<int>(buf.size()) >= infoLen) {
					glGetProgramInfoLog(program, infoLen, NULL, buf.data());
					std::cerr << "[Error]: Link faild!\n Desc: " << buf.data() << std::endl;
				}
			}
			glDeleteProgram(program);
			return 0;
		}
		return program;
	}

	void Program::BindTexture(GLuint unit, GLuint texture, GLuint type){

		if (unit >= GL_TEXTURE0 && static_cast<GLenum>(GL_TEXTURE0 + samperCount)) {

			glActiveTexture(unit);
			glBindTexture(type, texture);
		}
	}

	void Program::SetViewProjectionMatrix(const glm::mat4& matVP){
		
		if (matVPLocation >= 0) {

			glUniformMatrix4fv(matVPLocation, 1, GL_FLOAT, &matVP[0][0]);
		}

	}

	Program::~Program() {

		if (program) {
			glDeleteProgram(program);
		}
	}

	bool Program::UseProgram() const{

		glUseProgram(program);
		return glGetError == GL_NO_ERROR;
	}


	Buffer& Buffer::Instance() {

		static Buffer instance;
		return instance;
	}

	Buffer::~Buffer() {

		shaderList.clear();
		std::cout << "[Info]: ShaderResource is Cleard!" << std::endl;
	}

	ProgramPtr Buffer::Create(const char* vsPass, const char* fsPass, const char* shaderName){

		std::cout << "[Info]: Creating Program...";

		struct Impl : Program { Impl() {} ~Impl() {} };
		ProgramPtr_s p = std::make_shared<Impl>();

		if (!p) {
			std::cout << "\n[Error]: Could not allocate memory!!" << std::endl;
			return {};
		}

		std::vector<char> vsBuf, fsBuf;
		ReadFromFile(vsPass, vsBuf);
		ReadFromFile(fsPass, fsBuf);

		p->program = CreateShaderProgram(vsBuf.data(), fsBuf.data());

		//�T���v���[�̐��ƈʒu���擾����
		GLint activeUniforms;
		glGetProgramiv(p->program, GL_ACTIVE_UNIFORMS, &activeUniforms);
		for (int i = 0; i < activeUniforms; ++i) {
			GLint size;
			GLenum type;
			GLchar name[128];
			glGetActiveUniform(p->program, i, sizeof(name), nullptr, &size, &type, name);
			if (type == GL_SAMPLER_2D) {
				p->samperCount = size;
				p->samplerLocation = glGetUniformLocation(p->program, name);
				if (p->samplerLocation < 0) {
					std::cerr << "ERROR: �v���O����'" << vsPass << "'�̍쐬�Ɏ��s���܂���" << std::endl;
					return {};
				}
				break;
			}
		}

		//matVP�p���P�[�V�����ʒu���擾����
		p->matVPLocation = glGetUniformLocation(p->program, "matVP");

		auto& inst = Buffer::Instance();

		//�V�F�[�_���d���h�~����
		int counter = 0;
		std::string newShaderName;
		do {

			newShaderName = counter == 0 ? shaderName : shaderName + std::to_string(counter);
			counter++;

		} while (inst.shaderList.find(newShaderName) != inst.shaderList.end());

		inst.shaderList[newShaderName] = p;

		return p;
	}

}