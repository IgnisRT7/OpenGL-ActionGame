#pragma once
 

namespace ShaderFile {

	static const char* finalRender_vert = "res/shader/FinalRender.vert";
	static const char* finalRender_frag = "res/shader/FinalRender.frag";
	static const char* default_vert = "res/shader/Default.vert";
	static const char* default_frag = "res/shader/Default.frag";
	static const char* fontRenderer_vert = "res/shader/FontRenderer.vert";
	static const char* fontRenderer_frag = "res/shader/FontRenderer.frag";
	static const char* staticMesh_vert = "res/shader/StaticMesh.vert";
	static const char* staticMesh_frag = "res/shader/StaticMesh.frag";
}

namespace TextureFile {

	static const char* testDebugTexture = "res/texture/sampleTex.dds";
	static const char* testDebugTexture2 = "res/texture/debug.dds";
}

#define MESH_FILEPASS "res/mesh/"
namespace MeshFile {

	static const char* treeModel = "red_pine_tree.gltf";
}