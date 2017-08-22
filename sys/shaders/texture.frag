#version 440 core
in vec3 vColor;
in vec2 vTexCoord;
out vec4 color;
uniform sampler2D fTexture;

void main()
{
	color = texture(fTexture, vTexCoord);
}