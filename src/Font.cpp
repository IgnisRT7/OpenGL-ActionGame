/**
*	@file FontRenderer.cpp
*/
#include "Font.h"
#include <fstream>
#include <sstream>

#pragma warning(disable: 4996)

namespace Font {

	struct Vertex {
		glm::vec3 position;
		glm::vec2 texcoord;
		glm::vec4 color;
	};

	Buffer& Buffer::Instance(){

		static Buffer buffer;
		return buffer;
	}

	FontDataPtr Buffer::CreateFontFromFile(const char* filename){
		
		struct Impl : FontData {  };
		auto p = std::make_shared<Impl>();

		if (!p) {
			std::cerr << "[Error]: FontBuffer create error!" << std::endl;
			return {};
		}

		const std::unique_ptr<FILE, decltype(&fclose)> fp(fopen(filename, "r"), fclose);
		if (!fp) {
			std::cerr << "ERROR: '" << filename << "'のオープンに失敗" << std::endl;
			return {};
		}
		
		//info行を読み込む
		int line = 1;	//読み込む行番号(エラー表示用)
		int spacing[2];	// 1行目の読み込みチェック用
		int ret = fscanf(fp.get(), "info face=\"%*[^\"]\" size=%*d bold=%*d italic=%*d charset=%*s unicode=%*d"
			" stretchH=%*d smooth=%*d aa=%*d padding=%*d,%*d,%*d,%*d spacing=%d,%d%*[^\n]",
			&spacing[0], &spacing[1]);
		if (ret < 2) {
			std::cerr << "[エラー]" << __func__ << ":" <<
				filename << "の読み込みに失敗(" << line << "行目)\n";
			return {};
		}
		++line;

		//common行を読み込む
		float scaleH;
		ret = fscanf(fp.get(),
			" common lineHeight=%f base=%f scaleW=%*d scaleH=%f pages=%*d packed=%*d%*[^\n]",
			&p->lineHeight, &p->base, &scaleH);
		if (ret < 3) {
			std::cerr << "[エラー]" << __func__ << ":" << filename << "の読み込みに失敗(" << line << "行目)\n";
			return {};
		}
		++line;

		//page行を読み込む
		std::vector<std::string> texNameList;
		texNameList.reserve(16);
		for (;;) {

			int id;
			char tex[256];
			ret = fscanf(fp.get(), " page id=%d file=\"%255[^\"]\"", &id, tex);
			if (ret < 2) {
				break;
			}
			tex[sizeof(tex) / sizeof(tex[0]) - 1] = '\0';	//0終端を保証する
			if (texNameList.size() <= static_cast<size_t>(id)) {
				texNameList.resize(id + 1);
			}
			texNameList[id] = std::string("res/font/") + tex;
			++line;
		}
		if (texNameList.empty()) {
			std::cerr << "[エラー]" << __func__ << ":" << filename << "の読み込みに失敗(" << line << "行目)\n";
			return {};
		}
		
		//chars行を読み込む
		int charCount;	//char行の数
		ret = fscanf(fp.get(), " chars count=%d", &charCount);
		if (ret < 1) {
			std::cerr << "[エラー]" << __func__ << ":" << filename << "の読み込みに失敗(" << line << "行目)\n";
			return {};
		}
		line++;

		//char行を読み込む
		p->characterInfoList.clear();
		p->characterInfoList.resize(65536);	//16bitで表せる範囲を確保
		for (int i = 0; i < charCount; ++i) {
			FontData::CharacterInfo info;
			ret = fscanf(fp.get(),
				" char id=%d x=%f y=%f width=%f height=%f xoffset=%f yoffset=%f xadvance=%f"
				" page=%d chnl=%*d",
				&info.id, &info.uv.x, &info.uv.y, &info.size.x, &info.size.y,
				&info.offset.x, &info.offset.y, &info.xadvance, &info.page);
			if (ret < 9) {
				std::cerr << "[エラー]" << __func__ << ":" << filename << "の読み込みに失敗(" << line << "行目)\n";
				return {};
			}

			//フォントファイルは左上が原点なので、openGLの座標系(左下原点)に変換
			info.uv.y = scaleH - info.uv.y - info.size.y;

			if (static_cast<size_t>(info.id) < p->characterInfoList.size()) {
				p->characterInfoList[info.id] = info;
			}
			++line;
		}

		for (auto& texName : texNameList) {
			p->textureList.push_back(Texture::Buffer::Instance().LoadFromFile(texName.c_str()));
		}

		fontList[filename] = p;

		return p;
	}

	FontDataPtr Buffer::GetFont(const char* name){

		auto itr = fontList.find(name);
		if (itr != fontList.end()) {
			return itr->second;
		}
		else {
			return {};
		}
	}

	bool Renderer::Init(size_t maxChar, glm::ivec2 screenSize) {

		if (maxChar > (USHRT_MAX + 1) / 4) {
			std::cerr << "[Warning]: FontRenderer::Init " << maxChar << "は設定可能な最大文字数を超えています" << std::endl;
			maxChar = (USHRT_MAX + 1) / 4;
		}

		//1つの文字描画に必要な頂点数(4) * 最大文字描画数
		vboCapacity = static_cast<GLsizei>(maxChar * 4);
		vbo.Init("FontVBO", GL_ARRAY_BUFFER, vboCapacity * sizeof(Vertex));

		//インデックスデータの初期化処理 (1文字 = 4頂点, 左周りに 0 1 2,2 3 0 の2つの三角形)
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

		//TODO: スクリーンサイズは可変なため修正用コードを入れる必要がある
		reciprocalScreenSize = 1 / screenSize;

		return true;
	}

	void Renderer::MapBuffer(){

		vertices.clear();
		primitives.clear();
		vertices.reserve(vbo.Size());
	}

	void Renderer::UnMapBuffer(){

		vbo.BufferSubData(0, vertices.size() * sizeof(Vertex), vertices.data());
		vertices.clear();
		vertices.shrink_to_fit();
	}

	void Renderer::Draw(){

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		vao.Bind();
		auto progShader = shader.lock();
		progShader->UseProgram();

		//平衡投影、原点は画面の中心
		const glm::vec2 halfScreenSize = screenSize * 0.5f;
		const glm::mat4x4 matProj = glm::ortho(
			-halfScreenSize.x, halfScreenSize.x, -halfScreenSize.y, halfScreenSize.y,
			1.0f, 1000.0f);
		const glm::mat4x4 matView = glm::lookAt(glm::vec3(0, 0, 100), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		progShader->SetViewProjectionMatrix(matProj * matView);

		for (const Primitive& primitive : primitives) {

			progShader->BindTexture(GL_TEXTURE0, primitive.texture.lock()->Id());
			glDrawElements(GL_TRIANGLES, primitive.count, GL_UNSIGNED_SHORT, reinterpret_cast<const GLvoid*>(primitive.offset));
		}

		progShader->BindTexture(0, 0);
		vao.UnBind();
	}

	void Renderer::AddString(const glm::vec3& position, const wchar_t* str, FontDataPtr font){

		if (font.expired()) {
			std::cout << "[Error]: 無効なフォントデータが設定されているため描画できません" << std::endl;
			return;
		}

		glm::vec2 tmpPos = position;
		auto rawFont = font.lock();
		shader = rawFont->progFont;

		for (const wchar_t* itr = str; *itr; itr++) {

			auto& info = rawFont->characterInfoList[*itr];

			//スプライトの座標が画像の中心をしていするが、フォントは左上を指定する
			//そこで、その差を打ち消すための補正値を計算する
			//const float baseX = info.size.x * 0.5f + info.offset.x;
			//const float baseY = rawFont->base - info.size.y * 0.5f - info.offset.y;
			//glm::vec3 pos = glm::vec3(position + glm::vec2(baseX, baseY), 0);

			AddVertices(info, rawFont->textureList[info.page], position);

			tmpPos.x += info.xadvance;

		}
	}

	void Renderer::AddVertices(FontData::CharacterInfo& charaData, Texture::Image2DPtr texture, glm::vec3 position){

		struct Rect {
			glm::vec2 origin;
			glm::vec2 size;
		};

		if (texture.expired()) {
			return;
		}
		auto sTexture = texture.lock();

		glm::vec2 texSize = glm::vec2(sTexture->Width(), sTexture->Height());
		glm::vec2 recpTexSize = glm::vec2(1) / texSize;

		//0 - 1 の範囲に収める
		Rect rect = { charaData.uv,charaData.size };
		rect.origin *= recpTexSize;
		rect.size *= recpTexSize;
		
		glm::vec2 halfSize = charaData.size * 0.5f;
		glm::mat4  matTRS = glm::translate(glm::identity<glm::mat4>(), position);

		Vertex vertex[4];

		//
		// 3- -2
		// |  /| 
		// |/" |
		// 0- -1

		vertex[0].position = matTRS * glm::vec4(-halfSize.x, -halfSize.y, 0, 1);
		vertex[1].position = matTRS * glm::vec4( halfSize.x, -halfSize.y, 0, 1);
		vertex[2].position = matTRS * glm::vec4( halfSize.x,  halfSize.y, 0, 1);
		vertex[3].position = matTRS * glm::vec4(-halfSize.x,  halfSize.y, 0, 1);

		vertex[0].texcoord = glm::vec2(rect.origin.x, rect.origin.y + rect.size.y);
		vertex[1].texcoord = glm::vec2(rect.origin + rect.size);
		vertex[2].texcoord = glm::vec2(rect.origin.x + rect.size.x, rect.origin.y);
		vertex[3].texcoord = glm::vec2(rect.origin);

		vertex[0].color = glm::vec4(1);
		vertex[1].color = glm::vec4(1);
		vertex[2].color = glm::vec4(1);
		vertex[3].color = glm::vec4(1);

		vertices.insert(vertices.end(), vertex, vertex + 4);
		
		//プリミティブの追加

		if (primitives.empty()) {
			primitives.push_back({ 6,0,texture });
		}
		else {

			auto& primitive = primitives.back();
			if (primitive.texture.lock() == texture.lock()) {
				primitive.count += 6;
			}
			else {
				primitives.push_back({ 6,primitive.offset + primitive.count * sizeof(GLushort),texture });
			}
		}
	}

}