/***
*	@file Mesh.h
*/
#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include <GL/glew.h>
#include "BufferObject.h"
#include "Texture.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include "json11/json11.hpp"

namespace Mesh {

	//��s�錾
	struct Mesh;
	using MeshPtr = std::shared_ptr<Mesh>;
	class Buffer;
	using MeshBufferPtr = std::shared_ptr<Buffer>;

	//�X�P���^�����b�V���p��s�錾
	struct Node;
	struct ExtendedFile;
	using ExtendedFilePtr = std::shared_ptr<ExtendedFile>;
	class SkeletalMesh;
	using SkeletalMeshPtr = std::shared_ptr<SkeletalMesh>;

	struct Vertex {
		glm::vec3 position;
		glm::vec2 texCoord;
		glm::vec3 normal;
	};

	/*
	*	�v���~�e�B�u�̍ގ�
	*/
	struct Material {
		glm::vec4 baseColor = glm::vec4(1);	/// �F�f�[�^
		Texture::Image2DPtr texture[16];		/// �e�N�X�`���f�[�^�z��
		Shader::ProgramPtr program;			/// �g�p����v���O����
//		Shader::ProgramPtr progSkeletalMesh;	/// �X�P���^�����b�V���p�v���O����
	};

	/**
	*	���_�f�[�^�̕`��p�����[�^
	*/
	struct Primitive {
		GLenum mode = GL_STATIC_DRAW;
		GLsizei count = 0;
		GLenum type = GL_NONE;
		const GLvoid* indices = nullptr;
		GLint baseVertex = 0;
		std::shared_ptr<VertexArrayObject> vao;
		int material = 0;
	};

	/**
	*	���b�V��
	*/
	struct Mesh {
		std::string name;	//���b�V����
		std::vector<Primitive> primitives;
	};

	/**
	*	�t�@�C��
	*/
	struct File {
		std::string name;	//�t�@�C����
		std::vector<Mesh> meshes;
		std::vector<Material> materials;
	};
	using FilePtr = std::shared_ptr<File>;

	/**
	*	���b�V���Ǘ��N���X
	*/
	class Buffer {
	public:

		Buffer() = default;
		~Buffer() = default;

		bool Init(GLsizeiptr vboSize, GLsizeiptr iboSize);
		GLintptr AddVertexData(const void* data, size_t size);
		GLintptr AddIndexData(const void* data, size_t size);
		Primitive CreatePrimitive(size_t count, GLenum type, size_t ioffset, size_t voffset)const;
		Material CreateMaterial(const glm::vec4& color, Texture::Image2DPtr texture) const;
		bool AddMesh(const char* name, const Primitive& primitive, const Material& material);
		bool SetAttribute(Primitive*, int, const json11::Json&, const json11::Json&, const std::vector<std::vector<char> >&);
		bool LoadMesh(const char* path);
		FilePtr GetFile(const char* name) const;
		void SetViewProjectionMatrix(const glm::mat4&) const;
		void SetCameraPosition(const glm::vec3&) const;
		void SetTime(double) const;

		void AddCube(const char* name);
		FilePtr AddPlane(const char* name);

		//�X�P���^���E�A�j���[�V�����ɑΉ��������b�V���̓ǂݍ��݂Ǝ擾
		//bool LoadSkeletalmesh(const char* path);
		//SkeletalMeshPtr GetSkeletalMesh(const char* meshName) const;

		//const Shader::ProgramPtr& GetStaticMeshShader() const { return progStaticMesh; }
		//const Shader::ProgramPtr& GetTerrainShader() const { return progTerrain; }
		//const Shader::ProgramPtr& GetWaterShader() const { return progWater; }

	private:

		BufferObject vbo;
		BufferObject ibo;
		GLintptr vboEnd = 0;
		GLintptr iboEnd = 0;
		std::unordered_map<std::string, FilePtr> files;

		Shader::ProgramPtr progStaticMesh;
		//Shader::ProgramPtr progSkeletalMesh;
		//Shader::ProgramPtr progTerrain;
		//Shader::ProgramPtr progWater;

		struct MeshIndex {
			ExtendedFilePtr file;
			const Node* node = nullptr;
		};
		std::unordered_map<std::string, MeshIndex> meshes;
		std::unordered_map<std::string, ExtendedFilePtr> extendedFiles;
	};

	void Draw(const FilePtr&, const glm::mat4& matM);

}// namespace Mesh




#endif