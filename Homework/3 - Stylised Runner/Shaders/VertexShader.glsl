#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Output to fragment shader
out vec3 position;
out vec3 normal;
out vec2 texcoord;

void main()
{
	// Send data to fragment shader
	position = v_position;
	normal = v_normal;
	texcoord = v_texture_coord;

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
