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
	*	ビュー空間の行列を取得
	*/
	const glm::mat4& ViewMatrix() const { return matView; }

	/**
	*	射影変換行列を取得
	*/
	const glm::mat4& ProjectionMatrix() const { return matProj;	};

	/**
	*	ビュー空間・射影変換を掛けた行列を取得
	*/
	const glm::mat4& ViewProjMatrix() const { return matView * matProj; };

	/**
	*	ビュー空間上の逆回転行列を取得
	*
	*	tips この関数は主にパーティクルで使用されます	
	*/
	const glm::vec4& ViewInvMatrix() const;

	void SetTransform(glm::vec3 pos, glm::vec3 rot) { position = pos; rotation = rot; CalculateMatrix(); }
	
	// void Update(float deltaTime){ CalculateMatrix();}

protected:

	/**
	*	行列の再計算を行います
	*/
	virtual void CalculateMatrix();

private:

	glm::vec3 position;		///< 位置情報
	glm::quat rotation;		///< 回転情報

	glm::vec3 target;		///< 視ている方向

	glm::mat4 matView = glm::identity<glm::mat4>();			
	glm::mat4 matRotInvView = glm::identity<glm::mat4>();	
	glm::mat4 matProj = glm::identity<glm::mat4>();

	float fov = 45.0f;
	float aspect = 1.0f;
	float near;
	float far;
};