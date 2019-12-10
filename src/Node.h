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
	*	�s��̍Čv�Z����
	*/
	void CalculateTransform();

private:

	glm::vec3 position;		///< ���W���
	glm::vec3 rotation;		///< ��]���
	glm::vec3 scale;		///< �g�k���

	//Collision colData;	///< �R���W�������

	bool isActive;			///< �A�N�e�B�u��Ԃ��ۂ�

};