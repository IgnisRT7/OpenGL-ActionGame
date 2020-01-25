#version 410

layout(location=0) in vec3 vPosition;
layout(location=1) in vec2 vTexcoord;
layout(location=2) in vec4 vColor;

layout(location=0) out vec2 outTexCoord;
layout(location=1) out vec4 outColor;

uniform mat4 matVP;
uniform mat4 matM;

void main() {

	outTexCoord = vTexcoord;
	outColor = vColor;

	vec4 pos = matM * matVP * vec4(vPosition,1.0);

	gl_Position = pos;
}