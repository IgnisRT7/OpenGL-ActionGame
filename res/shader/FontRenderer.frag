#version 410

layout(location=0) in vec2 inTexcoord;
layout(location=1) in vec4 inColor;

out vec4 fragColor;

uniform sampler2D texSampler;

void main(){

	fragColor = texture(texSampler, inTexcoord) * inColor;
	//fragColor = vec4(1);
}