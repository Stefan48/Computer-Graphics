#include "Tema3.h"
#include "Transform2D.h"
#include "Transform3D.h"

#include <vector>
#include <string>
#include <iostream>
#include <time.h>

#include <Core/Engine.h>
using namespace std;

Tema3::Tema3()
{
}

Tema3::~Tema3()
{
}

void Tema3::Init()
{
	srand((unsigned int)time(NULL));

	camera = GetSceneCamera();
	projectionMatrix = camera->GetProjectionMatrix();
	camera_FoVy = camera->GetFieldOfViewY();
	camera_position = glm::vec3(0, 2, 5);

	camera->SetPerspective(camera_FoVy, window->props.aspectRatio, 0.01f, 400);
	camera->SetPosition(camera_position);
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();

	// Load textures
	const string textureLoc = "Source/Laboratoare/Tema3/Textures/";

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "grass.jpg").c_str(), GL_REPEAT);
		textures["grass"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "crate.jpg").c_str(), GL_REPEAT);
		textures["crate"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "wood.jpeg").c_str(), GL_REPEAT);
		textures["wood"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "ring-base-color.png").c_str(), GL_REPEAT);
		textures["ring-base-color"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "cloud-background1.jpg").c_str(), GL_REPEAT);
		textures["background"] = texture;
	}

	// load digits textures
	for (int i = 0; i < 10; ++i)
	{
		Texture2D* texture = new Texture2D();
		std::string digit = std::to_string(i);
		texture->Load2D((textureLoc + "/Digits/" + digit + ".png").c_str(), GL_REPEAT);
		textures[digit] = texture;
	}
	
	// Create shader "ShaderCustom3D" for RenderSimpleMesh
	{
		Shader *shader = new Shader("ShaderCustom3D");
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Import box mesh
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	// Import sphere mesh
	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Import ring mesh
	{
		Mesh* mesh = new Mesh("ring");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "ring2.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create square mesh
	{
		vector<glm::vec3> vertices
		{
			glm::vec3(0.5f,   0.5f, 0.0f),
			glm::vec3(-0.5f,  0.5f, 0.0f),
			glm::vec3(-0.5f, -0.5f, 0.0f),
			glm::vec3(0.5f,  -0.5f, 0.0f)
		};

		vector<glm::vec3> normals
		{
			glm::vec3(0, 1, 1),
			glm::vec3(1, 0, 1),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0)
		};

		vector<glm::vec2> textureCoords
		{
			glm::vec2(1.0f, 0.0f),
			glm::vec2(0.0f, 0.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(1.0f, 1.0f)
		};

		vector<unsigned short> indices =
		{
			0, 1, 3,
			1, 2, 3
		};

		Mesh* mesh = new Mesh("square");
		mesh->InitFromData(vertices, normals, textureCoords, indices);
		meshes[mesh->GetMeshID()] = mesh;
	}


	// Create cube mesh
	{
		vector<glm::vec3> vertices
		{
			// sides
			glm::vec3(-1, -1, 1),
			glm::vec3(1, -1, 1),
			glm::vec3(1, -1, -1),
			glm::vec3(-1, -1, -1),

			glm::vec3(-1, 1, 1),
			glm::vec3(1, 1, 1),
			glm::vec3(1, 1, -1),
			glm::vec3(-1, 1, -1),

			// bottom
			glm::vec3(-1, -1, 1),
			glm::vec3(1, -1, 1),
			glm::vec3(1, -1, -1),
			glm::vec3(-1, -1, -1),

			// top
			glm::vec3(-1, 1, 1),
			glm::vec3(1, 1, 1),
			glm::vec3(1, 1, -1),
			glm::vec3(-1, 1, -1)
		};

		vector<glm::vec3> normals
		{
			glm::vec3(0, 1, 1),
			glm::vec3(1, 0, 1),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),

			glm::vec3(0, 1, 1),
			glm::vec3(1, 0, 1),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),

			glm::vec3(0, 1, 1),
			glm::vec3(1, 0, 1),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),

			glm::vec3(0, 1, 1),
			glm::vec3(1, 0, 1),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0)
		};

		vector<glm::vec2> texture_coords
		{
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),

			glm::vec2(0.0f, 1.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(1.0f, 1.0f),

			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),

			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f)
		};

		vector<unsigned short> indices =
		{
			0, 1, 5,    0, 5, 4,
			1, 2, 6,    1, 6, 5,
			2, 3, 7,    2, 7, 6,
			3, 0, 4,    3, 4, 7,

			8, 9, 10,   8, 10, 11,
			12, 13, 14, 12, 14, 15
		};
		meshes["cube"] = new Mesh("cube");
		meshes["cube"]->InitFromData(vertices, normals, texture_coords, indices);
		meshes["cube"]->SetDrawMode(GL_TRIANGLES);
	}

	// Create lamp mesh
	{
		vector<glm::vec3> vertices
		{
			// sides
			glm::vec3(-1, -20, 1),
			glm::vec3(1, -20, 1),
			glm::vec3(1, -20, -1),
			glm::vec3(-1, -20, -1),

			glm::vec3(-1, 20, 1),
			glm::vec3(1, 20, 1),
			glm::vec3(1, 20, -1),
			glm::vec3(-1, 20, -1),

			// bottom
			glm::vec3(-1, -20, 1),
			glm::vec3(1, -20, 1),
			glm::vec3(1, -20, -1),
			glm::vec3(-1, -20, -1),

			// top
			glm::vec3(-1, 20, 1),
			glm::vec3(1, 20, 1),
			glm::vec3(1, 20, -1),
			glm::vec3(-1, 20, -1),

			// bulb
			glm::vec3(-3, 20, 3),
			glm::vec3(3, 20, 3),
			glm::vec3(3, 20, -3),
			glm::vec3(-3, 20, -3),

			glm::vec3(-3, 26, 3),
			glm::vec3(3, 26, 3),
			glm::vec3(3, 26, -3),
			glm::vec3(-3, 26, -3)
		};

		vector<glm::vec3> normals
		{
			glm::vec3(0, 1, 1),
			glm::vec3(1, 0, 1),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),

			glm::vec3(0, 1, 1),
			glm::vec3(1, 0, 1),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),

			glm::vec3(0, 1, 1),
			glm::vec3(1, 0, 1),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),

			glm::vec3(0, 1, 1),
			glm::vec3(1, 0, 1),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),

			glm::vec3(0, 1, 1),
			glm::vec3(1, 0, 1),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),

			glm::vec3(0, 1, 1),
			glm::vec3(1, 0, 1),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0)
		};

		vector<glm::vec2> texture_coords
		{
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),

			glm::vec2(0.0f, 1.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(1.0f, 1.0f),

			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),

			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),

			glm::vec2(0.0f, 0.0f),
			glm::vec2(0.0f, 0.0f),
			glm::vec2(0.0f, 0.0f),
			glm::vec2(0.0f, 0.0f),

			glm::vec2(0.0f, 0.0f),
			glm::vec2(0.0f, 0.0f),
			glm::vec2(0.0f, 0.0f),
			glm::vec2(0.0f, 0.0f)
		};

		vector<unsigned short> indices =
		{
			// sides
			0, 1, 5,    0, 5, 4,
			1, 2, 6,    1, 6, 5,
			2, 3, 7,    2, 7, 6,
			3, 0, 4,    3, 4, 7,
			// bottom
			8, 9, 10,   8, 10, 11,
			// top
			12, 13, 14, 12, 14, 15,
			// bulb sides
			16, 17, 21, 16, 21, 20,
			17, 18, 22, 17, 22, 21,
			18, 19, 23, 18, 23, 22,
			19, 16, 20, 19, 20, 23,
			// bulb bottom
			16, 17, 18, 16, 18, 19,
			// bulb top
			20, 21, 22, 20, 22, 23
		};
		meshes["lamp"] = new Mesh("lamp");
		meshes["lamp"]->InitFromData(vertices, normals, texture_coords, indices);
		meshes["lamp"]->SetDrawMode(GL_TRIANGLES);
	}

	//Initialize objects and parameters

	// initialize light parameters and material properties
	lightPosition = glm::vec3(0, 10, 1);
	materialKd = 0.5f;
	materialKs = 0.5f;
	platform_material_shininess = 5;
	sphere_material_shininess = 15;
	lamp_material_shininess = 10;
	crate_material_shininess = 5;
	ring_material_shininess = 10;

	// initialize platform parameters
	platform_scale_x = 1.8f;
	platform_scale_y = 0.25f;
	platform_scale_z_min = 1.8f;
	platform_scale_z_max = 3.4f;
	platform_scale_z_med = 2.6f;
	platform_y = 0;
	platform_z_initial = -platform_scale_z_med * 1.5f; //-platform_scale_z_med / 2.0f; //0.0f;
	lane1_x = -2.7f;
	lane2_x = 0.0f;
	lane3_x = 2.7f;
	platform_distance_min = 0.1f * platform_scale_z_med;
	platform_distance_max = 0.4f * platform_scale_z_med;
	platform_distance_step = 0.02f * platform_scale_z_med;
	platforms_per_lane = 8;
	lane_speed = 2.5f;
	//lane_speed = 0;
	lane_speed_min = 1.7f;
	lane_speed_max = 3.3f;
	lane_speed_step = 0.2f;
	// crate parameters
	crate_scale = 0.4f;
	crate_y = platform_y + (platform_scale_y + crate_scale) / 2.0f;
	// ring parameters
	ring_scale = 0.15f;
	ring_y = 0.8f;
	ring_rotation_step = 2.0f;
	score = 0;

	// initialize platforms
	platforms1.push_back(Platform(platform_z_initial, platform_scale_z_med, false, false));
	platforms2.push_back(Platform(platform_z_initial, platform_scale_z_med, false, false));
	platforms3.push_back(Platform(platform_z_initial, platform_scale_z_med, false, false));

	// initialize sphere parameters
	sphere_color = glm::vec3(0.33f, 0.17f, 0.07f);
	sphere_scale = 0.5f;
	sphere_y_initial = platform_scale_y / 2.0f + sphere_scale / 2.0f;
	sphere_speed_x = 5.0f;
	sphere_speed_y = 0.0f;
	sphere_jump_speed_switch = 7.0f;
	sphere_jump_speed_fwd = 12.0f;
	gravity = 25.0f;
	sphere_next_platform = 0;

	// initialize sphere
	sphere_x = 0;
	sphere_y = sphere_y_initial;
	sphere_z = 0;
	sphere_up = sphere_down = sphere_left = sphere_right = false;

	// initialize lamp parameters
	lamp_scale = 0.05f;
	lamp_x1 = lane1_x / 2.0f;
	lamp_x2 = lane3_x / 2.0f;
	lamp_y = 1.0f;
	lamp_initial_z = 0.0f;
	lamp_light_height = 40 * lamp_scale;
	lamps_nr = 8;
	lamp_distance = 5.0f;
	lamp_erase_z = 10.0f;

	// initialize lamps
	float r = (rand() % 256) / 255.0f, g = (rand() % 256) / 255.0f, b = (rand() % 256) / 255.0f;
	lamps.push_back(Lamp(lamp_initial_z, glm::vec3(r, g, b), false));
	for (int i = 1; i < lamps_nr; ++i)
	{
		r = (rand() % 256) / 255.0f; g = (rand() % 256) / 255.0f; b = (rand() % 256) / 255.0f;
		lamps.push_back(Lamp(lamps[i-1].z - lamp_distance, glm::vec3(r, g, b), !lamps[i-1].spot));
	}
	
	// initialize skybox parameters
	skybox_scale = 100.0f;

	// initialize score display parameters
	score_scale = 0.4f;
	score_position_x = 4.5f;
	score_position_y = 4.5f;
	score_position_z = 0.0f;
	score_offset = 0.25f;
}


void Tema3::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color, unsigned int shininess, Texture2D* texture, bool light_source, bool background)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Bind model matrix
	int loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Set color
	int loc_color = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(loc_color, 1, glm::value_ptr(color));

	// Set texture
	if (texture != NULL)
	{
		// activate texture location
		glActiveTexture(GL_TEXTURE0);
		// bind the texture id
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
		// send texture uniforms
		glUniform1i(glGetUniformLocation(shader->program, "texture"), 0);
		glUniform1i(glGetUniformLocation(shader->program, "texture_active"), 1);
	}
	else
	{
		glUniform1i(glGetUniformLocation(shader->program, "texture_active"), 0);
	}

	// Set if the object is the background
	glUniform1i(glGetUniformLocation(shader->program, "background"), (int)background);

	// Set light height (for lamps)
	glUniform1f(glGetUniformLocation(shader->program, "light_height"), lamp_light_height);

	// Set if the object is a light source
	glUniform1i(glGetUniformLocation(shader->program, "light_source"), (int)light_source);

	// Set uniforms for lamps' properties
	glUniform1f(glGetUniformLocation(shader->program, "lampx1"), lamp_x1);
	glUniform1f(glGetUniformLocation(shader->program, "lampx2"), lamp_x2);
	float lamp_z[8];
	float lamp_color_red[8];
	float lamp_color_green[8];
	float lamp_color_blue[8];
	int lamp_spot[8];
	for (int i = 0; i < 8; ++i)
	{
		lamp_z[i] = lamps[i].z;
		lamp_color_red[i] = lamps[i].color.x;
		lamp_color_green[i] = lamps[i].color.y;
		lamp_color_blue[i] = lamps[i].color.z;
		lamp_spot[i] = (int)lamps[i].spot;
	}
	glUniform1fv(glGetUniformLocation(shader->program, "lampz"), 8, lamp_z);
	glUniform1fv(glGetUniformLocation(shader->program, "lampcolor_r"), 8, lamp_color_red);
	glUniform1fv(glGetUniformLocation(shader->program, "lampcolor_g"), 8, lamp_color_green);
	glUniform1fv(glGetUniformLocation(shader->program, "lampcolor_b"), 8, lamp_color_blue);
	glUniform1iv(glGetUniformLocation(shader->program, "lampspot"), 8, lamp_spot);

	// Set shader uniforms for light & material properties
	// Set global light position
	int loc_light_position = glGetUniformLocation(shader->program, "light_position");
	glUniform3fv(loc_light_position, 1, glm::value_ptr(lightPosition));

	// Set eye position (camera position)
	glm::vec3 eyePosition = camera_position;
	int loc_eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3fv(loc_eye_position, 1, glm::value_ptr(eyePosition));

	// Set material properties (kd, ks, shininess)
	int loc_kd = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(loc_kd, materialKd);

	int loc_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(loc_ks, materialKs);

	int loc_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(loc_shininess, shininess);

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}


void Tema3::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor((GLclampf)0.7, (GLclampf)0.63, (GLclampf)0.53, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema3::RenderLanePlatforms(std::vector<Platform> &platforms, float lane_x)
{
	// render lane
	glm::mat4 modelMatrix;
	for (int i = 0; i < platforms.size(); ++i)
	{
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(lane_x, platform_y, platforms[i].z);
		modelMatrix *= Transform3D::Scale(platform_scale_x, platform_scale_y, platforms[i].scale_z);
		RenderSimpleMesh(meshes["box"], shaders["ShaderCustom3D"], modelMatrix, glm::vec3(1,1,1), platform_material_shininess, textures["grass"], false, false);

		if (platforms[i].blocking)
		{
			// render a crate on the platform's center
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(lane_x, crate_y, platforms[i].z);
			modelMatrix *= Transform3D::Scale(crate_scale, crate_scale, crate_scale);
			RenderSimpleMesh(meshes["box"], shaders["ShaderCustom3D"], modelMatrix, glm::vec3(1, 1, 1), crate_material_shininess, textures["crate"], false, false);
		}

		if (platforms[i].collectible)
		{
			// render a ring above the platform's center
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(lane_x, ring_y, platforms[i].z);
			modelMatrix *= Transform3D::RotateOY(platforms[i].ring_rotation);
			modelMatrix *= Transform3D::RotateOZ(RADIANS(90));
			modelMatrix *= Transform3D::Scale(ring_scale, ring_scale, ring_scale);
			RenderSimpleMesh(meshes["ring"], shaders["ShaderCustom3D"], modelMatrix, glm::vec3(1, 1, 1), ring_material_shininess, textures["ring-base-color"], false, false);
		}
	}
}

void Tema3::UpdateLanePlatforms(std::vector<Platform> &platforms, float deltaTime)
{
	// update lane
	// move platforms
	for (int i = 0; i < platforms.size(); ++i)
	{
		platforms[i].z += lane_speed * deltaTime;
		if (platforms[i].z > 2 * platforms[i].scale_z)
		{
			// out of screen
			platforms.erase(platforms.begin() + i);
			i--;
		}
		else if (platforms[i].collectible)
		{
			// update ring rotation
			platforms[i].ring_rotation += ring_rotation_step * deltaTime;
		}
	}
	// generate new platforms
	while (platforms.size() < platforms_per_lane)
	{
		float scale_z = (rand() % (int)((platform_scale_z_max - platform_scale_z_min) * 1000)) / 1000.0f + platform_scale_z_min;
		float dist = (rand() % (int)((platform_distance_max - platform_distance_min) * 1000)) / 1000.0f + platform_distance_min;
		float z = platforms.back().z - platforms.back().scale_z / 2.0f - dist - scale_z / 2.0f;
		// 10% chance of having a blocking crate
		int x = rand() % 100;
		bool blocking = (x < 10);
		// 20% chance of having a collectible ring
		x = rand() % 100;
		bool collectible = (x >= 80);
		platforms.push_back(Platform(z, scale_z, blocking, collectible));
	}
}

bool Tema3::CheckSpherePlatformsCollision(std::vector<Platform> &platforms)
{
	// check collision with the first 3 platforms (others are redundant)
	for (int i = 0; i < platforms.size() && i < 3; ++i)
	{
		if ((sphere_z + sphere_scale / 2.0f >= platforms[i].z - platforms[i].scale_z / 2.0f) &&
			(sphere_z - sphere_scale / 2.0f <= platforms[i].z + platforms[i].scale_z / 2.0f))
			{
				if (platforms[i].blocking || platforms[i].collectible)
				{
					bool is_centered =
						(sphere_z - sphere_scale / 2.0f <= platforms[i].z + crate_scale / 2.0f && sphere_z + sphere_scale / 2.0f >= platforms[i].z + crate_scale / 2.0f) ||
						(sphere_z + sphere_scale / 2.0f >= platforms[i].z - crate_scale / 2.0f && sphere_z - sphere_scale / 2.0f <= platforms[i].z - crate_scale / 2.0f);
					if (is_centered)
					{
						if (platforms[i].blocking)
						{
							// crate collision
							cout << "Crate collision! Game over!\n";
							exit(0);
						}
						if (platforms[i].collectible)
						{
							// ring collected
							score++;
							cout << "Score: " << score << "\n";
							platforms[i].collectible = false;
						}
					}
				}
				return true;
			}
	}
	return false;
}

void Tema3::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);

	// update platforms
	UpdateLanePlatforms(platforms1, deltaTimeSeconds);
	UpdateLanePlatforms(platforms2, deltaTimeSeconds);
	UpdateLanePlatforms(platforms3, deltaTimeSeconds);

	// update lamps
	for (int i = 0; i < lamps.size(); ++i)
	{
		lamps[i].z += lane_speed * deltaTimeSeconds;
	}
	if (lamps[0].z > lamp_erase_z)
	{
		// out of screen
		lamps.erase(lamps.begin());
		// generate new lamp
		float r = (rand() % 256) / 255.0f, g = (rand() % 256) / 255.0f, b = (rand() % 256) / 255.0f;
		float z = lamps.back().z - lamp_distance;
		bool spot = !lamps.back().spot;
		lamps.push_back(Lamp(z, glm::vec3(r, g, b), spot));
	}

	// update sphere
	// update sphere's position
	if (sphere_up)
	{
		sphere_speed_y -= gravity * deltaTimeSeconds;
		sphere_y += sphere_speed_y * deltaTimeSeconds;
		if (sphere_speed_y <= 0)
		{
			sphere_speed_y = 0;
			sphere_up = false;
			sphere_down = true;
		}
	}
	else if (sphere_down)
	{
		sphere_speed_y += gravity * deltaTimeSeconds;
		float deltaY = sphere_speed_y * deltaTimeSeconds;
		sphere_y -= deltaY;
		// if sphere has just fallen below the standard height
		if (sphere_y <= sphere_y_initial && sphere_y + deltaY > sphere_y_initial)
		{
			// correct its height in order to check platform collisions correctly
			sphere_y = sphere_y_initial;
		}

		if (sphere_y < sphere_y_initial - 10.0f)
		{
			// game over
			cout << "\nGame over!\n";
			exit(0);
		}
	}
	if (sphere_left)
	{
		float deltaX = sphere_speed_x * deltaTimeSeconds;
		sphere_x -= deltaX;
		float between_lanes = (lane1_x + lane2_x) / 2.0f;
		if (sphere_down && sphere_next_platform == 1 && sphere_x <= lane1_x)
		{
			sphere_left = false;
			sphere_x = lane1_x;
		}
		else if (sphere_down && sphere_next_platform == 2 && sphere_x <= lane2_x)
		{
			sphere_left = false;
			sphere_x = lane2_x;
		}
		// if crossing from lane 2 to lane 1
		else if (sphere_x <= between_lanes && sphere_x + deltaX > between_lanes)
		{
			sphere_up = false;
			sphere_down = true;
			sphere_speed_y = 0.0f;
		}
		else
		{
			between_lanes = (lane2_x + lane3_x) / 2.0f;
			// if crossing from lane 3 to lane 2
			if (sphere_x <= between_lanes && sphere_x + deltaX > between_lanes)
			{
				sphere_up = false;
				sphere_down = true;
				sphere_speed_y = 0.0f;
			}
		}
	}
	else if (sphere_right)
	{
		float deltaX = sphere_speed_x * deltaTimeSeconds;
		sphere_x += deltaX;
		float between_lanes = (lane1_x + lane2_x) / 2.0f;
		if (sphere_down && sphere_next_platform == 2 && sphere_x >= lane2_x)
		{
			sphere_right = false;
			sphere_x = lane2_x;
		}
		else if (sphere_down && sphere_next_platform == 3 && sphere_x >= lane3_x)
		{
			sphere_right = false;
			sphere_x = lane3_x;
		}
		// if crossing from lane 1 to lane 2
		else if (sphere_x >= between_lanes && sphere_x - deltaX < between_lanes)
		{
			sphere_up = false;
			sphere_down = true;
			sphere_speed_y = 0.0f;
		}
		else
		{
			between_lanes = (lane2_x + lane3_x) / 2.0f;
			// if crossing from lane 2 to lane 3
			if (sphere_x >= between_lanes && sphere_x - deltaX < between_lanes)
			{
				sphere_up = false;
				sphere_down = true;
				sphere_speed_y = 0.0f;
			}
		}
	}
	// check collision with platforms on each lane
	bool collision = false;
	if (sphere_y == sphere_y_initial)
	{
		if (sphere_x == lane1_x)
		{
			// sphere is on lane 1
			collision = CheckSpherePlatformsCollision(platforms1);
		}
		else if (sphere_x == lane2_x)
		{
			// sphere is on lane 2
			collision = CheckSpherePlatformsCollision(platforms2);
		}
		else if (sphere_x == lane3_x)
		{
			// sphere is on lane 3
			collision = CheckSpherePlatformsCollision(platforms3);
		}
		if (!collision)
		{
			sphere_down = true;
			sphere_speed_y = 0.0f;
		}
		else
		{
			sphere_down = false;
			sphere_speed_y = 0.0f;
		}
	}
	
	// render objects
	glm::mat4 modelMatrix = glm::mat4(1);
	
	// render platforms
	RenderLanePlatforms(platforms1, lane1_x);
	RenderLanePlatforms(platforms2, lane2_x);
	RenderLanePlatforms(platforms3, lane3_x);

	// render lamps
	for (int i = 0; i < lamps.size(); ++i)
	{
		// left lamp
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(lamp_x1, lamp_y, lamps[i].z);
		modelMatrix *= Transform3D::Scale(lamp_scale, lamp_scale, lamp_scale);
		RenderSimpleMesh(meshes["lamp"], shaders["ShaderCustom3D"], modelMatrix, lamps[i].color, lamp_material_shininess, textures["wood"], true, false);
		// right lamp
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(lamp_x2, lamp_y, lamps[i].z);
		modelMatrix *= Transform3D::Scale(lamp_scale, lamp_scale, lamp_scale);
		RenderSimpleMesh(meshes["lamp"], shaders["ShaderCustom3D"], modelMatrix, lamps[i].color, lamp_material_shininess, textures["wood"], true, false);
	}

	// render sphere
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(sphere_x, sphere_y, sphere_z);
	modelMatrix *= Transform3D::Scale(sphere_scale, sphere_scale, sphere_scale);
	RenderSimpleMesh(meshes["sphere"], shaders["ShaderCustom3D"], modelMatrix, sphere_color, sphere_material_shininess, NULL, false, false);

	// render skybox
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Scale(skybox_scale, skybox_scale, skybox_scale);
	RenderSimpleMesh(meshes["sphere"], shaders["ShaderCustom3D"], modelMatrix, glm::vec3(1, 1, 1), 0, textures["background"], false, true);

	// render score
	int x = score;
	if (x == 0)
	{
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(score_position_x, score_position_y, score_position_z);
		modelMatrix *= Transform3D::Scale(score_scale, score_scale, score_scale);
		RenderSimpleMesh(meshes["square"], shaders["ShaderCustom3D"], modelMatrix, glm::vec3(1, 1, 1), 0, textures["0"], false, true);
	}
	else
	{
		int cnt = 0, digit;
		while (x)
		{
			digit = x % 10;
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(score_position_x - score_offset * cnt, score_position_y, score_position_z);
			modelMatrix *= Transform3D::Scale(score_scale, score_scale, score_scale);
			RenderSimpleMesh(meshes["square"], shaders["ShaderCustom3D"], modelMatrix, glm::vec3(1, 1, 1), 0, textures[std::to_string(digit)], false, true);
			cnt++;
			x /= 10;
		}
	}
}

void Tema3::FrameEnd()
{
	//DrawCoordinatSystem();
}

void Tema3::OnInputUpdate(float deltaTime, int mods)
{
	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {

	}
}

void Tema3::OnKeyPress(int key, int mods)
{
	// add key press event
	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		if (key == GLFW_KEY_SPACE)
		{
			// jump
			if (sphere_y == sphere_y_initial && !sphere_down)
			{
				sphere_up = true;
				sphere_speed_y = sphere_jump_speed_fwd;
			}
		}
		if (key == GLFW_KEY_A)
		{
			// jump left
			if (sphere_y == sphere_y_initial && !sphere_down && sphere_x != lane1_x)
			{
				sphere_up = true;
				sphere_left = true;
				sphere_speed_y = sphere_jump_speed_switch;
				if (sphere_x == lane2_x)
				{
					sphere_next_platform = 1;
				}
				else
				{
					sphere_next_platform = 2;
				}
			}
		}
		if (key == GLFW_KEY_D)
		{
			// jump right
			if (sphere_y == sphere_y_initial && !sphere_down && sphere_x != lane3_x)
			{
				sphere_up = true;
				sphere_right = true;
				sphere_speed_y = sphere_jump_speed_switch;
				if (sphere_x == lane1_x)
				{
					sphere_next_platform = 2;
				}
				else
				{
					sphere_next_platform = 3;
				}
			}
		}

		if (key == GLFW_KEY_W)
		{
			if (lane_speed < lane_speed_max)
			{
				cout << "speed +\n";
				lane_speed += lane_speed_step;
				platform_distance_min += platform_distance_step;
				platform_distance_max += platform_distance_step;
			}
		}
		if (key == GLFW_KEY_S)
		{
			if (lane_speed > lane_speed_min)
			{
				cout << "speed -\n";
				lane_speed -= lane_speed_step;
				platform_distance_min -= platform_distance_step;
				platform_distance_max -= platform_distance_step;
			}
		}
	}
}

void Tema3::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema3::OnWindowResize(int width, int height)
{
	//glm::ivec2 resolution = window->GetResolution();
}
