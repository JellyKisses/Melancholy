#version 430

out vec4 FragColor;

flat in vec3 IN_Position;
flat in vec3 IN_Normal;
flat in vec2 IN_TexCoord;
flat in vec3 IN_Color;


//CAMERA
uniform vec3  C_position;
uniform sampler2D tex;

void main()
{
	const vec3 intensity = vec3(1.f);
	const vec3 ambientColor = vec3(0.1f);
	const vec3 specularColor = vec3(1.f);

	vec3 normal = normalize(IN_Normal);
	vec3 surfaceToCamera = normalize(C_position - IN_Position);

	vec3 surfaceToLight = normalize(vec3(1.f, 2.f, 0.f));
	float attenuation = 1.f;
	
	vec3 ambient = texture(tex, IN_TexCoord).xyz * ambientColor * intensity;
	
	float diffuseCoefficient = max(0.f, dot(normal, surfaceToLight));
	vec3 diffuse = diffuseCoefficient * texture(tex, IN_TexCoord).xyz * intensity;

	vec3 gamma = vec3(1.f / 1.8f);

	vec3 finalColor = ambient + attenuation*(diffuse);

	FragColor = vec4(finalColor, 1.f);
}