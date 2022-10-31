#version 330
 
uniform sampler2D texture_1;
uniform sampler2D texture_2;

uniform int mix_textures;
uniform float time;
 
in vec2 texcoord;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO : calculate the out_color using the texture2D() function
	//out_color = vec4(1);
	vec4 color1;
	if (time != 0.0)
	{
		color1 = texture2D(texture_1, texcoord + vec2(time/10.0, 0));
	}
	else
	{
		color1 = texture2D(texture_1, texcoord);
	}
	if (mix_textures != 0)
	{
		vec4 color2 = texture2D(texture_2, texcoord);
		out_color = mix(color1, color2, 0.5);
	}
	else
	{
		out_color = color1;
	}
	
	if (out_color.a < 0.5)
		discard;
	
}