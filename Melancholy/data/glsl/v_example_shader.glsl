#version 430
in vec3 vpIn;
in vec2 texCoordIn;
in vec3 normalIn;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

flat out vec3 IN_Position;
flat out vec2 IN_TexCoord;
flat out vec3 IN_Normal;
flat out vec3 IN_Color;

void main()
{ 
	IN_Position = vpIn;
	IN_TexCoord = texCoordIn;
	IN_Normal = normalIn;
	
	gl_Position = projection * view * vec4(vpIn, 1.0);
}	