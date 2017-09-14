#version 430
in vec2 out_Position;
in vec2 out_TexCoord;

out vec2 in_Position;
out vec2 in_TexCoord;

void main()
{ 
	in_Position = out_Position;
	in_TexCoord = out_TexCoord;
	
	gl_Position = vec4(out_Position, 0.0, 1.0);
}	