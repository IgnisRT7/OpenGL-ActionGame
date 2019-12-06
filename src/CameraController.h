/**
*	@file CameraController.h
*/
#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class DefaultCamera {
public:

	DefaultCamera() = default;
	virtual ~DefaultCamera() = default;
	DefaultCamera(const DefaultCamera&) = delete;
	const DefaultCamera& operator=(const DefaultCamera&) = delete;

	void Position(glm::vec3 position) { this->position += position; }
	const glm::vec3& Position() const { return position; }
	void Rotation(const glm::vec3& rotation) { this->rotation = glm::quat(rotation); CalculateMatrix(); }
	const glm::vec3& Rotation(void) const { return glm::eulerAngles(rotation); }

	void ProjMatParam(float fov, float aspect, float near, float far) { 
		this->fov = fov, this->aspect = aspect; this->near = near; this->far = far; }

	void LookAt(glm::vec3 position, glm::vec3 target) { this->position = position; this->target = target; CalculateMatrix(); }

	/**
	*	�r���[��Ԃ̍s����擾
	*/
	const glm::mat4& ViewMatrix() const { return matView; }

	/**
	*	�ˉe�ϊ��s����擾
	*/
	const glm::mat4& ProjectionMatrix() const { return matProj;	};

	/**
	*	�r���[��ԁE�ˉe�ϊ����|�����s����擾
	*/
	const glm::mat4& ViewProjMatrix() const { return matView * matProj; };

	/**
	*	�r���[��ԏ�̋t��]�s����擾
	*
	*	tips ���̊֐��͎�Ƀp�[�e�B�N���Ŏg�p����܂�	
	*/
	const glm::vec4& ViewInvMatrix() const;

	void SetTransform(glm::vec3 pos, glm::vec3 rot) { position = pos; rotation = rot; CalculateMatrix(); }
	
	// void Update(float deltaTime){ CalculateMatrix();}

protected:

	/**
	*	�s��̍Čv�Z���s���܂�
	*/
	virtual void CalculateMatrix();

private:

	glm::vec3 position;		///< �ʒu���
	glm::quat rotation;		///< ��]���

	glm::vec3 target;		///< ���Ă������

	glm::mat4 matView = glm::identity<glm::mat4>();			
	glm::mat4 matRotInvView = glm::identity<glm::mat4>();	
	glm::mat4 matProj = glm::identity<glm::mat4>();

	float fov = 45.0f;
	float aspect = 1.0f;
	float near;
	float far;
};