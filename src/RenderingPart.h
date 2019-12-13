/**
*	@file RenderingPart.h
*/
#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace Rendering {

	/**
	*	頂点データ構造体
	*/
	struct Vertex {

		glm::vec3 position;	///< 頂点座標
		glm::vec2 texcoord;	///< テクスチャ座標
		glm::vec4 color;	///< 頂点色
	};

	const Vertex vertices[] = {

		//ウインドウ全体の矩形
		{ { -1.0, -1.0, 0 },{ 0, 1 },{ 1, 0, 0, 1} },		// left - down
		{ { 1.0, -1.0, 0 },{ 1, 1 },{ 0, 1, 0, 1} },		// right - down
		{ { 1.0, 1.0, 0 },{ 1, 0 },{ 0, 0, 1, 1} },			// right - up
		{ { -1.0, 1.0, 0 },{ 0, 0 },{ 0.8, 0.8, 0.8, 1} },	// left - up

		//メイン用の矩形
		{ { -1.0, -1.0, 0 },{ 0, 1 },{ 1, 0, 0, 1} },
		{ { 0.33, -1.0, 0 },{ 1, 1 },{ 0, 1, 0, 1} },
		{ { 0.33, 0.33, 0 },{ 1, 0 },{ 0, 0, 1, 1} },
		{ { -1.0, 0.33, 0 },{ 0, 0 },{ 0.8, 0.8, 0.8, 1} },

		//サブ用の矩形
		{ { -1.0, 0.33, 0 },{ 0, 1 },{ 1, 0, 0, 1} },
		{ { -0.35, 0.33, 0 },{ 1, 1 },{ 0, 1, 0, 1} },
		{ { -0.35, 1.0, 0 },{ 1, 0 },{ 0, 0, 1, 1} },
		{ { -1.0, 1.0, 0 },{ 0, 0 },{ 0.8, 0.8, 0.8, 1} },
	};

	const GLuint indices[] = {
	0,1,2,2,3,0,
	4,5,6,6,7,4,
	8,9,10,10,11,8,
	};

	/**
	*	描画する矩形の種類
	*/
	enum RenderPartType {
		FULL_WINDOW = 0,		//フル描画の
		MAIN_PART,
		SUB_WORLDPOSITION,
		SUB_WORLDNORMAL,
		SUB_,
	
	};



	/**
	*	部分描画データ
	*/
	struct RenderingPart {
		GLsizei size;	///< 描画するインデックス数
		GLvoid* offset;	///< 描画開始インデックスのバイトオフセット
	};

	/**
	*	RenderingPartを作成する
	*
	*	@param size 描画するインデックス数
	*	@param offset 描画開始インデックスのオフセット(インデックス単位)
	*
	*	@return 作成した部分描画オブジェクト
	*/
	RenderingPart MakePart(GLsizei size, GLsizei offset) {
		return { size, reinterpret_cast<GLvoid*>(offset * sizeof(GLuint)) };
	}

	RenderingPart parts[] = {
		MakePart(6, 0),
		MakePart(6, 6),
		MakePart(6, 12),
	};
}