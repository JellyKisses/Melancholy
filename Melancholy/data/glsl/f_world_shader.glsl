#version 430

out vec4 FragColor;

in vec3 in_Position;
in vec2 in_TexCoord;
in vec3 in_Normal;
in vec3 in_Tangent;
in vec3 in_Bitangent;

uniform mat4		u_projection;
uniform mat4		u_view;
uniform mat4		u_model;

uniform vec3		u_position;
const	int			c_tex_max = 16;
uniform sampler2D	u_tex_diff[c_tex_max];
uniform sampler2D	u_tex_norm[c_tex_max];


void main()
{
	const vec3 intensity = vec3(1.f);
	const vec3 ambientColor = vec3(0.1f);
	const vec3 specularColor = vec3(1.f);

	vec3 normal = normalize(in_Normal);
	vec3 surfaceToCamera = normalize(u_position - in_Position);

	vec3 surfaceToLight = normalize(vec3(1.f, 2.f, 0.f));
	float attenuation = 1.f;
	
	vec3 ambient = texture(u_tex_diff[0], in_TexCoord).rgb * ambientColor * intensity;
	
	float diffuseCoefficient = max(0.f, dot(normal, surfaceToLight));
	vec3 diffuse = diffuseCoefficient * texture(u_tex_diff[0], in_TexCoord).xyz * intensity;

	vec3 gamma = vec3(1.f / 1.8f);

	vec3 finalColor = ambient + attenuation*(diffuse);

	//FragColor = vec4(finalColor, 1.f);
	FragColor = mix(texture(u_tex_diff[0], in_TexCoord), texture(u_tex_diff[1], in_TexCoord), .5f);
}