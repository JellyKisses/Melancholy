#version 430

in vec3 out_Position;
in vec2 out_TexCoord;
in vec3 out_Normal;
in vec3 out_Tangent;
in vec3 out_Bitangent;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

out vec3 in_Position;
out vec2 in_TexCoord;
out vec3 in_Normal;
out vec3 in_Tangent;
out vec3 in_Bitangent;

void main()
{ 
	in_Position		= out_Position;
	in_TexCoord		= out_TexCoord;
	in_Normal		= out_Normal;
	in_Tangent		= out_Tangent;
	in_Bitangent	= out_Bitangent;
	
	gl_Position = u_projection * u_view * vec4(out_Position, 1.0);
}	