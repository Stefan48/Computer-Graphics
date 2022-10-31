#include "Tema2.h"
#include "Transform2D.h"
#include "Transform3D.h"

#include <vector>
#include <string>
#include <iostream>
#include <time.h>

#include <Core/Engine.h>
using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{
	srand((unsigned int)time(NULL));

	camera = GetSceneCamera();
	projectionMatrix = camera->GetProjectionMatrix();
	camera_FoVy = camera->GetFieldOfViewY();
	camera_position_default = glm::vec3(0, 2, 5);
	camera_position = camera_position_default;

	// get UI parameters
	glm::ivec2 resolution = window->GetResolution();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	viewMatrixUI = camera->GetViewMatrix();
	projectionMatrixUI = camera->GetProjectionMatrix();

	//switch back to 3D camera configuration
	camera->SetPerspective(camera_FoVy, window->props.aspectRatio, 0.01f, 400);
	camera->SetPosition(camera_position_default);
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();


	// Create shader "ShaderCustom3D" for RenderSimpleMesh
	{
		Shader *shader = new Shader("ShaderCustom3D");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
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

	// Create reservoir mesh (for fuel bar)
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(4, 4, 0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(4, 5, 0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(5, 4, 0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(-4, 5, 0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(-4, 4, 0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(-5, 4, 0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(-5, -4, 0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(-4, -5, 0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(-4, -4, 0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(4, -4, 0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(4, -5, 0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(5, -4, 0), glm::vec3(1, 1, 1)),
		};
		vector<unsigned short> indices =
		{
			0, 1, 2,
			0, 3, 1,
			0, 4, 3,
			4, 5, 3,
			4, 8, 5,
			8, 6, 5,
			8, 7, 6,
			8, 9, 7,
			7, 9, 10,
			9, 11, 10,
			9, 2, 11,
			9, 0, 2
		};
		meshes["reservoir"] = new Mesh("reservoir");
		meshes["reservoir"]->InitFromData(vertices, indices);
		meshes["reservoir"]->SetDrawMode(GL_TRIANGLES);
	}
	// Create fuel mesh (for fuel bar)
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(4, 8, 0), glm::vec3(0, 1, 0)),
			VertexFormat(glm::vec3(-4, 8, 0), glm::vec3(0, 1, 0)),
			VertexFormat(glm::vec3(-4, 0, 0), glm::vec3(0, 1, 0)),
			VertexFormat(glm::vec3(4, 0, 0), glm::vec3(0, 1, 0))
		};
		vector<unsigned short> indices =
		{
			0, 1, 2,
			0, 2, 3
		};
		meshes["fuel"] = new Mesh("fuel");
		meshes["fuel"]->InitFromData(vertices, indices);
		meshes["fuel"]->SetDrawMode(GL_TRIANGLES);
	}
	

	//Initialize objects and parameters

	// initialize camera parameters
	camera_first_person_active = false;
	camera_first_person_offset = glm::vec3(0, 0.3f, 3.75f);

	// initialize fuel bar parameters
	fuelbar_scale_x = 4.0f;
	fuelbar_scale_y = 6.0f;
	fuelbar_x = 50.0f;
	fuelbar_reservoir_y = 50.0f;
	fuelbar_fuel_y = fuelbar_reservoir_y - 4.0f * fuelbar_scale_y;
	fuel_percentage = 1.0f;
	fuel_loss_step = 0.04f;
	fuel_loss_step_step = 0.0032f; // value based on lane_speed_step

	// initialize light parameters and material properties
	lightPosition = glm::vec3(0, 10, 1);
	materialKd = 0.5f;
	materialKs = 0.5f;
	platform_material_shininess = 5;
	sphere_material_shininess = 15;

	// initialize platform parameters
	platform_color_blue = glm::vec3(0.14f, 0.56f, 0.9f);
	platform_color_red = glm::vec3(0.9f, 0.17f, 0.11f);
	platform_color_yellow = glm::vec3(1.0f, 1.0f, 0.0f);
	platform_color_orange = glm::vec3(0.98f, 0.47f, 0.05f);
	platform_color_green = glm::vec3(0.0f, 1.0f, 0.0f);
	platform_color_purple = glm::vec3(0.61f, 0.31f, 0.9f);
	platform_scale_x = 1.8f;
	platform_scale_y = 0.25f;
	platform_scale_z_min = 1.8f;
	platform_scale_z_max = 3.4f;
	platform_scale_z_med = 2.6f;
	platform_y = 0;
	platform_z_initial = -platform_scale_z_med / 2.0f;
	lane1_x = -2.1f;
	lane2_x = 0.0f;
	lane3_x = 2.1f;
	platform_distance_min = 0.1f * platform_scale_z_med;
	platform_distance_max = 0.4f * platform_scale_z_med;
	platform_distance_step = 0.02f * platform_scale_z_med;
	platforms_per_lane = 8;
	lane_speed = 2.5f;
	lane_speed_min = 1.7f;
	lane_speed_max = 3.3f;
	lane_speed_step = 0.2f;
	platform_yellow_fuel_loss = 0.3f;
	platform_green_fuel_gain = 0.4f;
	platform_orange_timer = 0.0f;
	platform_orange_timer_limit = 1.0f;
	platform_orange_timer_step = 0.3f;
	platform_orange_active = false;
	// orange platform doubles lane speed
	platform_orange_speed_bonus = lane_speed;

	// initialize platforms
	platforms1.push_back(Platform(platform_z_initial, platform_scale_z_med, platform_color_blue));
	platforms2.push_back(Platform(platform_z_initial, platform_scale_z_med, platform_color_blue));
	platforms3.push_back(Platform(platform_z_initial, platform_scale_z_med, platform_color_blue));

	// initialize sphere parameters
	sphere_color = glm::vec3(0.7f, 0, 0.3f);
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
	sphere_effect_immunity = false;
}

glm::vec3 Tema2::GeneratePlatformColor()
{
	int x = rand() % 100;
	// 30% blue
	if (x < 30) return platform_color_blue;
	// 25% green
	if (x < 55) return platform_color_green;
	// 20% yellow
	if (x < 75) return platform_color_yellow;
	// 15% orange
	if (x < 90) return platform_color_orange;
	// 10% red
	return platform_color_red;
}

void Tema2::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color, unsigned int shininess, bool applyNoise)
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

	// Set time
	int loc_time = glGetUniformLocation(shader->program, "time");
	glUniform1f(loc_time, Engine::GetElapsedTime());

	// Set noise
	int loc_apply_noise = glGetUniformLocation(shader->program, "apply_noise");
	glUniform1i(loc_apply_noise, (int)applyNoise);

	// Set shader uniforms for light & material properties
	// Set light position
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

void Tema2::RenderMeshUI(Mesh * mesh, Shader * shader, const glm::mat3 &modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrixUI));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrixUI));

	glm::mat3 mm = modelMatrix;
	glm::mat4 model = glm::mat4(
		mm[0][0], mm[0][1], mm[0][2], 0.f,
		mm[1][0], mm[1][1], mm[1][2], 0.f,
		0.f, 0.f, mm[2][2], 0.f,
		mm[2][0], mm[2][1], 0.f, 1.f);

	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(model));
	mesh->Render();
}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor((GLclampf)0.02, (GLclampf)0, (GLclampf)0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::RenderLanePlatforms(std::vector<Platform> &platforms, float lane_x)
{
	// render lane
	glm::mat4 modelMatrix;
	for (int i = 0; i < platforms.size(); ++i)
	{
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(lane_x, platform_y, platforms[i].z);
		modelMatrix *= Transform3D::Scale(platform_scale_x, platform_scale_y, platforms[i].scale_z);
		RenderSimpleMesh(meshes["box"], shaders["ShaderCustom3D"], modelMatrix, platforms[i].color, platform_material_shininess);
	}
}

void Tema2::UpdateLanePlatforms(std::vector<Platform> &platforms, float deltaTime)
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
	}
	// generate new platforms
	while (platforms.size() < platforms_per_lane)
	{
		float scale_z = (rand() % (int)((platform_scale_z_max - platform_scale_z_min) * 1000)) / 1000.0f + platform_scale_z_min;
		float dist = (rand() % (int)((platform_distance_max - platform_distance_min) * 1000)) / 1000.0f + platform_distance_min;
		float z = platforms.back().z - platforms.back().scale_z / 2.0f - dist - scale_z / 2.0f;
		glm::vec3 color = GeneratePlatformColor();
		platforms.push_back(Platform(z, scale_z, color));
	}
}

glm::vec3 Tema2::CheckSpherePlatformsCollision(std::vector<Platform> &platforms)
{
	// check collision with the first 3 platforms (others are redundant)
	// returns color of the platform with which the sphere collided
	// return value (0,0,0) means no collision was detected

	for (int i = 0; i < platforms.size() && i < 3; ++i)
	{
		if ((sphere_z + sphere_scale / 2.0f >= platforms[i].z - platforms[i].scale_z / 2.0f) &&
			(sphere_z - sphere_scale / 2.0f <= platforms[i].z + platforms[i].scale_z / 2.0f))
			{
				glm::vec3 color = platforms[i].color;
				platforms[i].color = platform_color_purple;
				return color;
			}
	}
	return glm::vec3(0, 0, 0);
}

void Tema2::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);

	// update fuel
	fuel_percentage -= fuel_loss_step * deltaTimeSeconds;
	if (fuel_percentage <= 0.0f)
	{
		cout << "\nOut of fuel! Game over!\n";
		exit(0);
	}

	// update platforms
	UpdateLanePlatforms(platforms1, deltaTimeSeconds);
	UpdateLanePlatforms(platforms2, deltaTimeSeconds);
	UpdateLanePlatforms(platforms3, deltaTimeSeconds);
	// update orange platform's bonus speed timer (if active)
	if (platform_orange_active)
	{
		platform_orange_timer += platform_orange_timer_step * deltaTimeSeconds;
		if (platform_orange_timer >= platform_orange_timer_limit)
		{
			// speed bonus time has expired
			platform_orange_active = false;
			platform_orange_timer = 0.0f;
			lane_speed -= platform_orange_speed_bonus;
		}
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
	glm::vec3 collision_platform = glm::vec3(0, 0, 0);
	if (sphere_y == sphere_y_initial)
	{
		if (sphere_x == lane1_x)
		{
			// sphere is on lane 1
			collision_platform = CheckSpherePlatformsCollision(platforms1);
		}
		else if (sphere_x == lane2_x)
		{
			// sphere is on lane 2
			collision_platform = CheckSpherePlatformsCollision(platforms2);
		}
		else if (sphere_x == lane3_x)
		{
			// sphere is on lane 3
			collision_platform = CheckSpherePlatformsCollision(platforms3);
		}
		if (collision_platform == glm::vec3(0, 0, 0))
		{
			sphere_down = true;
			sphere_speed_y = 0.0f;
		}
		else
		{
			sphere_down = false;
			sphere_speed_y = 0.0f;
			if (collision_platform == platform_color_red)
			{
				// red platform collision -> game over
				cout << "\nRed platform contact! Game over!\n";
				exit(0);
			}
			else if (collision_platform == platform_color_yellow)
			{
				if (!sphere_effect_immunity)
				{
					sphere_effect_immunity = true;
					// yellow platform collision -> fuel loss
					fuel_percentage = max(0.0f, fuel_percentage - platform_yellow_fuel_loss);
				}
			}
			else if (collision_platform == platform_color_orange)
			{
				// orange platform collision -> activate bonus speed (or reset timer if already active)
				platform_orange_timer = 0.0f;
				if (!platform_orange_active)
				{
					platform_orange_active = true;
					lane_speed += platform_orange_speed_bonus;
				}
			}
			else if (collision_platform == platform_color_green)
			{
				if (!sphere_effect_immunity)
				{
					sphere_effect_immunity = true;
					// green platform collision -> fuel gain
					fuel_percentage = min(1.0f, fuel_percentage + platform_green_fuel_gain);
				}
			}
		}
	}

	// update camera
	if (camera_first_person_active)
	{
		camera->SetPosition(glm::vec3(sphere_x, sphere_y, sphere_z) + camera_first_person_offset);
		camera->Update();
	}
	
	// render objects
	glm::mat4 modelMatrix = glm::mat4(1);
	
	// render platforms
	RenderLanePlatforms(platforms1, lane1_x);
	RenderLanePlatforms(platforms2, lane2_x);
	RenderLanePlatforms(platforms3, lane3_x);

	// render sphere
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(sphere_x, sphere_y, sphere_z);
	modelMatrix *= Transform3D::Scale(sphere_scale, sphere_scale, sphere_scale);
	RenderSimpleMesh(meshes["sphere"], shaders["ShaderCustom3D"], modelMatrix, sphere_color, sphere_material_shininess, platform_orange_active);
	
	// render fuel bar
	glm::mat3 modelMatrixUI = glm::mat3(1);
	modelMatrixUI *= Transform2D::Translate(fuelbar_x, fuelbar_reservoir_y);
	modelMatrixUI *= Transform2D::Scale(fuelbar_scale_x, fuelbar_scale_y);
	RenderMeshUI(meshes["reservoir"], shaders["VertexColor"], modelMatrixUI);

	modelMatrixUI = glm::mat3(1);
	modelMatrixUI *= Transform2D::Translate(fuelbar_x, fuelbar_fuel_y);
	modelMatrixUI *= Transform2D::Scale(fuelbar_scale_x, fuelbar_scale_y * fuel_percentage);
	RenderMeshUI(meshes["fuel"], shaders["VertexColor"], modelMatrixUI);
}

void Tema2::FrameEnd()
{
	//DrawCoordinatSystem();
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {

	}
}

void Tema2::OnKeyPress(int key, int mods)
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
				sphere_effect_immunity = false;
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
				sphere_effect_immunity = false;
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
				sphere_effect_immunity = false;
			}
		}

		if (key == GLFW_KEY_W)
		{
			// cannot change speed while in burst speed mode
			if (lane_speed < lane_speed_max && !platform_orange_active)
			{
				cout << "speed +\n";
				lane_speed += lane_speed_step;
				platform_distance_min += platform_distance_step;
				platform_distance_max += platform_distance_step;
				platform_orange_speed_bonus = lane_speed;
				fuel_loss_step += fuel_loss_step_step;
			}
		}
		if (key == GLFW_KEY_S)
		{
			// cannot change speed while in burst speed mode
			if (lane_speed > lane_speed_min && !platform_orange_active)
			{
				cout << "speed -\n";
				lane_speed -= lane_speed_step;
				platform_distance_min -= platform_distance_step;
				platform_distance_max -= platform_distance_step;
				platform_orange_speed_bonus = lane_speed;
				fuel_loss_step -= fuel_loss_step_step;
			}
		}

		if (key == GLFW_KEY_C)
		{
			if (camera_first_person_active)
			{
				camera_first_person_active = false;
				camera->SetPosition(camera_position_default);
				camera->Update();
			}
			else
			{
				camera_first_person_active = true;
				camera->SetPosition(glm::vec3(sphere_x, sphere_y, sphere_z) + camera_first_person_offset);
				camera->Update();
			}
			
		}
	}
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
	//glm::ivec2 resolution = window->GetResolution();
}
