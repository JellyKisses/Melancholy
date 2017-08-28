#version 430

out vec4 FragColor;

in vec2 In_TexCoord;
in vec4 In_Color;

uniform sampler2D u_Tex;


void main()
{
	//FragColor = In_Color * vec4(1.0, 1.0, 1.0, texture(u_Tex, In_TexCoord).r);
	FragColor = In_Color;
}