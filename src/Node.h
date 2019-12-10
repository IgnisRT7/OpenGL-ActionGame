#pragma once

#include <glm/glm.hpp>

class Node {
public:

	Node() = default;
	virtual ~Node() = default;
	//Node(const Node&);

public:

	virtual bool Init() = 0;

	virtual void Update(float deltaTime) = 0;

	/**
	*	行列の再計算処理
	*/
	void CalculateTransform();

private:

	glm::vec3 position;		///< 座標情報
	glm::vec3 rotation;		///< 回転情報
	glm::vec3 scale;		///< 拡縮情報

	//Collision colData;	///< コリジョン情報

	bool isActive;			///< アクティブ状態か否か

};