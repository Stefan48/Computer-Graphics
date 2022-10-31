#version 330

// TODO: get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;
uniform int spot_light_active;
uniform float cutoff_angle;

// Uniforms for material properties
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform vec3 object_color;

layout(location = 0) out vec4 out_color;

void main()
{
	vec3 N = world_normal;
	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);

	// TODO: define ambient light component
	float ambient_light = 0.25;
	ambient_light = ambient_light * material_kd;

	// TODO: compute diffuse light component
	float diffuse_light = 0;
	diffuse_light = material_kd * max(dot(N, L), 0); // consider intensity = 1

	// TODO: compute specular light component
	float specular_light = 0;

	if (diffuse_light > 0)
	{
		// Phong model
		//vec3 R = reflect(-L, N);
		//specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess); // consider intensity = 1
		// Blinn model
		specular_light = material_ks * pow(max(dot(N, H), 0), material_shininess); // consider intensity = 1
	}

	// TODO: compute light
	float d = distance(light_position, world_position);
	// constant attenuation factor
	float Kc = 1.0f;
	// liniar attenuation factor
	float Kl = 0.1f;
	// quadratic attenuation factor
	float Kq = 0.05f;
	float attenuation = 1.0f / (Kc + Kl*d + Kq*d*d);
	float light;

	if (spot_light_active == 0)
	{
		light = ambient_light + attenuation * (diffuse_light + specular_light);
	}
	else
	{
		float spot_light = dot(-L, light_direction);
		float spot_light_limit = cos(cutoff_angle);
		
		if (spot_light > spot_light_limit)
		{
			// Quadratic attenuation
			float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
			float light_att_factor = pow(linear_att, 2);
			light = ambient_light + light_att_factor * attenuation * (diffuse_light + specular_light);
		}
		else
		{
			// no spot light
			light = ambient_light;
		}
	}

	// TODO: write pixel out color
	out_color = vec4(object_color * light, 1);
}