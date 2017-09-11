#version 430
in vec3 vpIn;
in vec2 texCoordIn;
in vec3 normalIn;

out vec2 IN_Position;
out vec2 IN_TexCoord;

void main()
{ 
	IN_Position = vec2(vpIn.xy);
	IN_TexCoord = texCoordIn;
	
	gl_Position = vec4(vpIn, 1.0);
}	