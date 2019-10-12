#version 410

layout(location=0) in vec2 outTexcoord;
layout(location=1) in vec4 outColor;

out vec4 fragColor;

uniform sampler2D texSampler;

void main(){

	fragColor = texture(texSampler,outTexcoord);
}