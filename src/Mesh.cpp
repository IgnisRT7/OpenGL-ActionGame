/**
*	@file Mesh.cpp
*/
#include "Mesh.h"

namespace Mesh{

	StaticMeshPtr Mesh::Buffer::Create() {

		struct Impl : StaticMesh {};

		auto p = std::make_shared<Impl>();

		return p;
	}

}