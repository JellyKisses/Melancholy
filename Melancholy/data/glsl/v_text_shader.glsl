#version 430
layout (location = 0) in vec2 vpIn;
layout (location = 1) in vec2 texCoordIn;
layout (location = 2) in vec4 ColorIn;

uniform mat4 u_Projection;

out vec2 In_TexCoord;
out vec4 In_Color;


void main()
{ 
	gl_Position = u_Projection * vec4(vpIn.xy, 0.0, 1.0);
	In_TexCoord = texCoordIn;
	In_Color = ColorIn;	
}