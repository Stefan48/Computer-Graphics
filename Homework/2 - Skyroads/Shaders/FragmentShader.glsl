#version 330

// get values from vertex shader
in vec3 color;
in float frag_apply_noise;
in vec2 vUv;
in float noise;

layout(location = 0) out vec4 out_color;

void main()
{
	// write pixel output color

	if (frag_apply_noise != 0.0)
	{
		vec3 c = vec3( vUv * ( 1. - 2. * noise ), 0.0 );
		gl_FragColor = vec4(color + c.rgb, 1.0);
	}
	else
	{
		gl_FragColor = vec4(color, 1.0);
	}
}