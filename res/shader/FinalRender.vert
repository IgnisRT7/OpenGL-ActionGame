#version 410

layout(location=0) in vec3 vPosition;
layout(location=1) in vec2 vTexcoord;
layout(location=2) in vec4 vColor;

layout(location=0) out vec2 outTexcoord;
layout(location=1) out vec4 outColor;

uniform mat4 matVP;

void main(){

	outTexcoord = vTexcoord;
	outColor = vColor;

	gl_Position = matVP * vec4(vPosition,1.0);
}

