#version 430

out vec4 FragColor;

in vec2 in_Position;
in vec2 in_TexCoord;

uniform sampler2D u_tex;
uniform int		  u_lvl;
uniform vec2	  u_res;

bool showEdges = false;


void main()
{
	vec4 rgbM = texture(u_tex, in_TexCoord);
	
	if(u_lvl == 1)
	{
		const float fxaa_span_max = 8.f;
		const float fxaa_reduce_min = 1.f / 128.f;
		const float fxaa_reduce_mul = 1.f / 8.f;

		vec3 luma = vec3(0.299, 0.587, 0.114);
		vec2 texOffset = vec2(1.f / u_res.x, 1.f / u_res.y);
		float lumaTL = dot(luma, texture(u_tex, in_TexCoord + vec2(-1.f, -1.f) * texOffset).rgb);
		float lumaTR = dot(luma, texture(u_tex, in_TexCoord + vec2( 1.f, -1.f) * texOffset).rgb);
		float lumaBL = dot(luma, texture(u_tex, in_TexCoord + vec2(-1.f,  1.f) * texOffset).rgb);
		float lumaBR = dot(luma, texture(u_tex, in_TexCoord + vec2( 1.f,  1.f) * texOffset).rgb);
		float lumaM  = dot(luma, rgbM.rgb);

		vec2 dir;
		dir.x = -((lumaTL + lumaTR) - (lumaBL + lumaBR));
		dir.y =  ((lumaTL + lumaBL) - (lumaTR + lumaBR));

		float dirReduce = max((lumaTL + lumaTR + lumaBL + lumaBR) * (fxaa_reduce_mul * .25), fxaa_reduce_min);
		float inverseDirAdjustment = 1.f / (min(abs(dir.x), abs(dir.y)) + dirReduce);

		dir = min(vec2(fxaa_span_max, fxaa_span_max), 
			  max(vec2(-fxaa_span_max, -fxaa_span_max), dir * inverseDirAdjustment)) * texOffset;
		
		vec3 result1 = (1.f / 2.f) * (
			texture(u_tex, in_TexCoord + (dir * vec2(1.f / 3.f - .5f))).rgb + 
			texture(u_tex, in_TexCoord + (dir * vec2(2.f / 3.f - .5f))).rgb);
	
		vec3 result2 = result1 * (1.f / 2.f) + (1.f / 4.f) * (
			texture(u_tex, in_TexCoord + (dir * vec2(0.f / 3.f - .5f))).rgb + 
			texture(u_tex, in_TexCoord + (dir * vec2(2.f / 3.f - .5f))).rgb);

		float lumaMin = min(lumaM, min(min(lumaTL, lumaTR), min(lumaBL, lumaBR)));
		float lumaMax = max(lumaM, max(max(lumaTL, lumaTR), max(lumaBL, lumaBR)));
		float lumaResult2 = dot(luma, result2);

		if(lumaResult2 < lumaMin || lumaResult2 > lumaMax)
			FragColor = vec4(result1, 1.f);
		else
			FragColor = vec4(result2, 1.f);
	}
	else if(u_lvl == 2)
	{
		FragColor = rgbM;
	}
	else if(u_lvl == 3)
	{
		FragColor = rgbM;
	}
	else
	{
		FragColor = rgbM;
	}
	
	if(showEdges)
	{
		FragColor.r = 1.f;
	}
}