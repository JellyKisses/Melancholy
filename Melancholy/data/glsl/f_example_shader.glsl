#version 430

out vec4 FragColor;

flat in vec3 IN_Position;
flat in vec3 IN_Normal;
flat in vec2 IN_TexCoord;
flat in vec3 IN_Color;


//CAMERA
uniform vec3  C_position;
uniform sampler2D tex;

void main()
{
	FragColor = vec4(IN_Color, 1.0) * texture(tex, IN_TexCoord);
}