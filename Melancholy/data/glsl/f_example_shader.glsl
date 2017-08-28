#version 430

out vec4 FragColor;

in vec3 IN_Position;
in vec3 IN_Normal;
in vec2 IN_TexCoord;


//CAMERA
uniform vec3  C_position;
uniform sampler2D tex;

void main()
{
	//FragColor = vec4(IN_TexCoord, 1.0, 1.0);
	FragColor = texture(tex, IN_TexCoord);
}