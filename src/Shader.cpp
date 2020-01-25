/**
*	@file Shader.cpp
*/
#include "Shader.h"
#include "DebugLogger.h"
#include <iostream>
#include <string>

#define TO_STRING(var) #var

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

		auto& inst = DebugLogger::LogBuffer::Instance();

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

			inst.Log(errorStr);
			return false;
		}

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

		auto& inst = DebugLogger::LogBuffer::Instance();

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
					std::string logstr("shader compile faild! error message: ");
					logstr.append(buf.data());
					inst.Log(logstr.c_str());
				}
			}
			glDeleteShader(shader);
			return 0;
		}

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

		DebugLogger::Log("Compiling vs Shader... ", DebugLogger::Infomation, false);
		GLuint vs = CompileShader(GL_VERTEX_SHADER, vsCode);
		DebugLogger::Log("Compiling fs Shader... ", DebugLogger::Infomation, false);
		GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fsCode);
		if (!vs || !fs) {
			return 0;
		}

		DebugLogger::Log("LinkState... ", DebugLogger::Infomation, false);

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

					std::string logstr("Program link faild! error message:");
					logstr.append(buf.data());
					DebugLogger::Log(logstr.c_str(), DebugLogger::Error);
				}
			}
			glDeleteProgram(program);
			return 0;
		}

		DebugLogger::Log("all complete.");
		return program;
	}

	void Program::BindTexture(GLuint unit, GLuint texture, GLuint type){

		if (unit >= GL_TEXTURE0 && static_cast<GLenum>(GL_TEXTURE0 + samperCount)) {

			glActiveTexture(unit);
			glBindTexture(type, texture);
		}
	}

	void Program::SetViewProjectionMatrix(const glm::mat4& matVP){

		if (locationList.find(LocationType::ViewProjectionMatrix) != locationList.end()) {

			glUniformMatrix4fv(locationList[LocationType::ViewProjectionMatrix], 1, GL_FLOAT, &matVP[0][0]);
		}
		/*
		if (matVPLocation >= 0) {

			glUniformMatrix4fv(matVPLocation, 1, GL_FLOAT, &matVP[0][0]);
		}*/
	}

	void Program::SetModelMatrix(const glm::mat4& matM) {

		if (locationList.find(LocationType::ViewProjectionMatrix) != locationList.end()) {

			glUniformMatrix4fv(locationList[LocationType::ModelMatrix], 1, GL_FLOAT, &matM[0][0]);
		}
	}

	Program::~Program() {

		if (program) {
			glDeleteProgram(program);
		}
	}

	bool Program::UseProgram() const{

		glUseProgram(program);
		return glGetError() == GL_NO_ERROR;
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

		DebugLogger::LogBuffer& logInst = DebugLogger::LogBuffer::Instance();

		std::string logstr("Creating '' Program");
		logstr.insert(sizeof("Creating '") - 1, shaderName);
		DebugLogger::Log(logstr.c_str(),DebugLogger::Infomation);

		struct Impl : Program { Impl() {} ~Impl() {} };
		ProgramPtr_s p = std::make_shared<Impl>();

		if (!p) {
			DebugLogger::Log("Could not allocate memory!!", DebugLogger::Error);
			return {};
		}

		std::vector<char> vsBuf, fsBuf;
		DebugLogger::Log("Reading... ", DebugLogger::Infomation, false);
		if (!ReadFromFile(vsPass, vsBuf) || !ReadFromFile(fsPass, fsBuf)) {
			DebugLogger::Log("Read failed!");
		}
		DebugLogger::Log("complete.");

		p->program = CreateShaderProgram(vsBuf.data(), fsBuf.data());

		logstr.clear();

		//サンプラーの数と位置を取得する
		GLint activeUniforms;
		glGetProgramiv(p->program, GL_ACTIVE_UNIFORMS, &activeUniforms);
		logstr = "ActiveUniforms size:";
		logstr.append(std::to_string(activeUniforms));
		DebugLogger::Log(logstr.c_str());
		for (int i = 0; i < activeUniforms; ++i) {
			GLint size;
			GLenum type;
			GLchar name[128];

			glGetActiveUniform(p->program, i, sizeof(name), nullptr, &size, &type, name);
			
			logstr.clear();
			logstr = "Uniform name: ";
			logstr.append(name);
			logstr.append(" type: ");
			logstr.append(std::to_string(type));
			logstr.append(" size ");
			logstr.append(std::to_string(size));
			DebugLogger::Log(logstr.c_str());

			if (type == GL_SAMPLER_2D) {
				p->samperCount = size;
				p->samplerLocation = glGetUniformLocation(p->program, name);
				if (p->samplerLocation < 0) {

					std::string errstr = "ERROR: プログラム'""'の作成に失敗しました";
					errstr.insert(sizeof("ERROR: プログラム'") - 1, vsPass);
					DebugLogger::Log(errstr.c_str());
					return {};
				}
				continue;
			}

			//ロケーションの取得
			for (auto loc = locationNameMap.begin(); loc != locationNameMap.end(); loc++) {
			
				auto location = glGetUniformLocation(p->program, loc->second.c_str());
				if (location >= 0) {
					p->locationList[loc->first] = location;
				}
			}
		}

		//matVP用ロケーション位置を取得する
		p->matVPLocation = glGetUniformLocation(p->program, "matVP");

		auto& inst = Buffer::Instance();

		//シェーダ名重複防止処理
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