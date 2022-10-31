#version 330

// Get values from vertex shader
in vec3 position;
in vec3 normal;
in vec2 texcoord;

layout(location = 0) out vec4 out_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform vec3 object_color;
uniform int light_source;
uniform float light_height; // for lamps
uniform int background; // for skybox and score

// Uniforms for other lamps
uniform float lampx1, lampx2;
uniform float lampz[8];
uniform float lampcolor_r[8], lampcolor_g[8], lampcolor_b[8];
uniform int lampspot[8];

// Uniforms for texture properties
uniform sampler2D texture;
uniform int texture_active;


void main()
{
	if (background != 0)
	{
		// skybox and score aren't affected by light sources
		vec4 color = texture2D(texture, texcoord);
		if (color.a < 0.1)
		{
			discard;
		}
		else
		{
			out_color = vec4(texture2D(texture, texcoord).xyz, 1);
		}
		return;
	}

	// compute world space vectors
	vec3 world_pos = (Model * vec4(position, 1)).xyz;
	vec3 N = normalize(mat3(Model) * normal);
	vec3 L = normalize(light_position - world_pos);
	vec3 V = normalize(eye_position - world_pos);
	vec3 H = normalize(L + V);

	// ambient light component
	float ambient_light = 0.8f;
	ambient_light = ambient_light * material_kd;

	// diffuse light component
	float intensity = 3.0f;
	float diffuse_light = material_kd * intensity * max(dot(N, L), 0);

	// specular light component
	float specular_light = 0;

	if (diffuse_light > 0)
	{
		// Blinn model
		specular_light = material_ks * intensity * pow(max(dot(N, H), 0), material_shininess);
	}

	// compute light
	float d = distance(light_position, world_pos);
	// constant attenuation factor
	float Kc = 1.0f;
	// liniar attenuation factor
	float Kl = 0.1f;
	// quadratic attenuation factor
	float Kq = 0.05f;
	float attenuation = 1.0f / (Kc + Kl*d + Kq*d*d);
	float light = ambient_light + attenuation * (diffuse_light + specular_light);

	float ke = 1.0;
	// color before considering lamps
	vec3 color;
	
	if (light_source != 0 && world_pos.y >= light_height)
	{
		// light bulb color
		color = object_color * ke;
	}
	else if (texture_active != 0)
	{
		color = texture2D(texture, texcoord).xyz * light;
	}
	else
	{
		color = object_color * light;
	}

	// add diffuse and specular components for the lamps
	int nr_lamps = 16;
	vec3 lamppos[16];
	vec3 lampcolor[16];
	int spot[16];
	for (int i = 0; i < nr_lamps/2; ++i)
	{
		lamppos[2*i] = vec3(lampx1, light_height, lampz[i]);
		lamppos[2*i+1] = vec3(lampx2, light_height, lampz[i]);
		lampcolor[2*i] = lampcolor[2*i+1] = vec3(lampcolor_r[i], lampcolor_g[i], lampcolor_b[i]);
		spot[2*i] = spot[2*i+1] = lampspot[i];
	}

	vec3 lamp_light_color = vec3(0, 0, 0);
	for (int i = 0; i < nr_lamps; ++i)
	{
		vec3 L = normalize(lamppos[i] - world_pos);
		vec3 H = normalize(L + V);

		// diffuse light
		float intens = 0.7f;
		float diffuse = material_kd * intens * max(dot(N, L), 0);

		// specular light
		float specular = 0;

		if (diffuse > 0)
		{
			// Blinn model
			specular = material_ks * intens * pow(max(dot(N, H), 0), material_shininess);
		}

		float d = distance(lamppos[i], world_pos);
		// constant attenuation factor
		float kc = 1.0f;
		// liniar attenuation factor
		float kl = 0.5f;
		// quadratic attenuation factor
		float kq = 0.2f;
		float att = 1.0f / (kc + kl*d + kq*d*d);
		//float lamp_light = att * (diffuse + specular);
		float lamp_light = 0;
		if (spot[i] == 0)
		{
			// point type light source
			lamp_light = att * (diffuse + specular);
		}
		else
		{
			// spot type light source
			vec3 light_direction = vec3(0, -1, 0);
			float cutoff_angle = radians(45.0);
			float spot_light = dot(-L, light_direction);
			float spot_light_limit = cos(cutoff_angle);

			if (spot_light > spot_light_limit)
			{
				// Quadratic attenuation
				float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
				float light_att_factor = pow(linear_att, 2);
				float spot_intensity = 5.0f;
				lamp_light = spot_intensity * light_att_factor * att * (diffuse + specular);
			}
			else
			{
				// no spot light
				lamp_light = 0;
			}
		}

		lamp_light_color += lampcolor[i] * lamp_light;
	}

	color = color + lamp_light_color;
	
	// write pixel output color
	out_color = vec4(color, 1);
}
