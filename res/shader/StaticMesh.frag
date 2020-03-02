/**
*	@file StaticMesh.frag
*/
#version 430

layout(location=0) in vec3 inPosition;
layout(location=1) in vec2 inTexCoord;
layout(location=2) in vec3 inNormal;

out vec4 fragColor;

uniform sampler2D texColor;

/*
*	スタティックメッシュ用
*/
void main(){

   fragColor = texture(texColor, inTexCoord);
 // fragColor = vec4(1);
}