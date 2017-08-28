#version 430
layout (location = 0) in vec3 vpIn;
layout (location = 1) in vec2 texCoordIn;
layout (location = 2) in vec3 normalIn;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 IN_Position;
out vec2 IN_TexCoord;
out vec3 IN_Normal;


void main()
{ 
	IN_Position = vec3(model * vec4(vpIn, 1.0));
	IN_TexCoord = texCoordIn;
	IN_Normal = mat3(model) * normalIn;
	
	gl_Position = projection * view * vec4(vpIn, 1.0);
}