#version 330

// TODO: get vertex attributes from each location


// position
layout(location = 0) in vec3 v_position;
// normal
layout(location = 1) in vec3 v_normal;
// texture
layout(location = 2) in vec2 tex_coord;
// color
layout(location = 3) in vec3 v_color;

/*
// position
layout(location = 0) in vec3 v_position;
// color
layout(location = 1) in vec3 v_color;
// texture
layout(location = 2) in vec2 tex_coord;
// normal
layout(location = 3) in vec3 v_normal;
*/

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// application time
uniform float Apptime;

// TODO: output values to fragment shader

out vec3 frag_color;
out vec3 frag_normal;
out float frag_apptime;

void main()
{
	// TODO: send output to fragment shader

	frag_color = v_color;
	frag_normal = v_normal;
	frag_apptime = Apptime;

	// TODO: compute gl_Position

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
