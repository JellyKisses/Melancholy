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
	IN_Position = vec3(model * vec4(vpIn, 1.0));
	IN_TexCoord = texCoordIn;
	IN_Normal = mat3(model) * normalIn;
	IN_Color = vec3(texCoordIn.x, 1.0, texCoordIn.y);
	
	gl_Position = projection * view * vec4(vpIn, 1.0);
}