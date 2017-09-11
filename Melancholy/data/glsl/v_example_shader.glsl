#version 430
in vec3 vpIn;
in vec2 texCoordIn;
in vec3 normalIn;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 IN_Position;
out vec2 IN_TexCoord;
out vec3 IN_Normal;
out vec3 IN_Color;

void main()
{ 
	IN_Position = vpIn;
	IN_TexCoord = texCoordIn*8.f;
	IN_Normal = normalIn;
	
	gl_Position = projection * view * vec4(vpIn, 1.0);
}	