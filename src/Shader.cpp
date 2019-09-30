/**
*	@file Shader.cpp
*/
#include "Shader.h"

#include <iostream>

namespace Shader {

	/**
	*	ファイルからシェーダデータを取り出す
	*
	*	@param filename	読み込むファイル名
	*	@param buf		読み込み後のデータ
	*
	*	@retval true	読み込み成功
	*	@retval false	読み込み失敗
	*/
	bool ReadFromFile(const char* filename, std::vector<char>& buf) {

		struct stat st;
		if (stat(filename, &st)) {
			return false;
		}

		FILE* fp = NULL;
		fopen_s(&fp, filename, "rb");
		if (!fp) {
			return false;
		}

		buf.resize(st.st_size + 1);
		const size_t readSize = fread(buf.data(), 1, st.st_size, fp);
		fclose(fp);
		if (readSize != st.st_size) {
			return false;
		}

		buf.back() = '\0';
		return true;
	}


	/**
	* シェーダコードをコンパイルする.
	*
	* @param type シェーダの種類.
	* @param string シェーダコードへのポインタ.
	*
	* @return 作成したシェーダオブジェクト.
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
					std::cerr << "\n[Error]: シェーダのコンパイルに失敗￥n" << buf.data() << std::endl;
				}
			}
			glDeleteShader(shader);
			return 0;
		}

		std::cout << "completed." << std::endl;
		return shader;
	}

	/**
	* プログラムオブジェクトを作成する.
	*
	* @param vsCode 頂点シェーダコードへのポインタ.
	* @param fsCode フラグメントシェーダコードへのポインタ.
	*
	* @return 作成したプログラムオブジェクト.
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

	ProgramPtr Shader::Program::Create(const char* vsPass, const char* fsPass) {

		std::cout << "[Info]: Creating Program...";

		struct Impl : Program { Impl() {} ~Impl() {} };
		ProgramPtr p = std::make_shared<Impl>();
		
		if (!p) {
			std::cout << "\n[Error]: Could not allocate memory!!" << std::endl;
			return {};
		}

		std::vector<char> vsBuf, fsBuf;
		ReadFromFile(vsPass, vsBuf);
		ReadFromFile(fsPass, fsBuf);

		p->program = CreateShaderProgram(vsBuf.data(), fsBuf.data());

		//サンプラーの数と位置を取得する
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
					std::cerr << "ERROR: プログラム'" << vsPass << "'の作成に失敗しました" << std::endl;
					return {};
				}
				break;
			}
		}

		//matVP用ロケーション位置を取得する
		p->matVPLocation = glGetUniformLocation(p->program, "matVP");
		
		return p;
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

}