/**
*	@file FontRenderer.cpp
*/
#include "Font.h"
#include <fstream>
#include <sstream>

namespace Font {

	struct Vertex {
		glm::vec3 position;
		glm::vec2 texcoord;
		glm::vec4 color;
	};

	FontDataPtr Buffer::CreateFontFromFile(const char* filename){

		struct Impl : FontData {  };
		auto p = std::make_shared<Impl>();

		if (!p) {
			std::cerr << "[Error]: FontBuffer create error!" << std::endl;
			return {};
		}

		std::ifstream ifs(filename);
		if (ifs.fail()) {
			std::cerr << "[Error]: FontBuffer isvalid pass!" << std::endl;
			return{};
		}int i = 0;
		
		//1行ずつ読み込み
		char lineBuf[256];
		int line = 1;		///< 読み込む行
		while (ifs.getline(lineBuf, sizeof(char) * 256)) {
			
			std::string lineStr(lineBuf);

			if (line == 2) {	//scale x,y	

				size_t findNum = lineStr.find("scaleW");
				if (findNum != std::string::npos) {
					auto numstr = lineStr.substr(findNum + 7, 3);
					p->scale.x = atoi(numstr.c_str());
				}
				if (findNum != std::string::npos) {
					findNum = lineStr.find("scaleH");
					auto numstr = lineStr.substr(findNum + 7, 3);
					p->scale.x = atoi(numstr.c_str());
				}
			}
			else if (line == 3) {

				size_t findNum = lineStr.find("file");

				lineStr.

				char tex[128];
				ret = fscanf(fp.get(), " page id=%*d file=%127s", tex);
				if (ret < 1) {
					std::cerr << "ERROR: '" << filename << "'の読み込みに失敗(line=" << line <<
						")" << std::endl;
					return false;
				}
				std::string texName;
				std::string res("Res/Texture/");
				texName.assign(tex + 1, tex + strlen(tex) - 1);
			}

			line++;
		}

		return p;
	}

	bool Renderer::Init(size_t maxChar, glm::ivec2 screenSize) {

		if (maxChar > (USHRT_MAX + 1) / 4) {
			std::cerr << "[Warning]: FontRenderer::Init " << maxChar << "は設定可能な最大文字数を超えています" << std::endl;
			maxChar = (USHRT_MAX + 1) / 4;
		}

		//1つの文字描画に必要な頂点数(4) * 最大文字描画数
		vboCapacity = static_cast<GLsizei>(maxChar * 4);
		vbo.Init("FontVBO", GL_ARRAY_BUFFER, vboCapacity * sizeof(Vertex));

		//インデックスデータの初期化処理
		std::vector<GLushort> indexBuf;
		indexBuf.resize(maxChar * 6);
		GLushort* p = indexBuf.data();
		for (GLushort i = 0; i < maxChar * 4; i += 4) {
			for (GLushort n : {0, 1, 2, 2, 3, 0}) {
				*(p++) = i * n;
			}
		}
		ibo.Init("FontIBO", GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6 * maxChar, indexBuf.data());

		vao.Init(vbo.GetId(), ibo.GetId());
		if (vao.Bind()) {

			vao.VertexAttribPointer(0, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, position)));
			vao.VertexAttribPointer(1, 2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, texcoord)));
			vao.VertexAttribPointer(2, 4, GL_FLOAT, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, color)));
		}
		vao.UnBind(true);

		reciprocalScreenSize = 1 / screenSize;

		return false;
	}

}