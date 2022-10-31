#include "Tema1.h"
#include "Transform2D.h"

#include <vector>
#include <string>
#include <iostream>
#include <time.h>

#include <Core/Engine.h>
using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	srand((unsigned int)time(NULL));

	// Create bow mesh
	{
		vector<VertexFormat> vertices;

		for (float i = 0; i <= 1; i += 0.01f) {
			vertices.push_back(VertexFormat(glm::vec3(i, sqrt(1 - i * i), 0)));
		}
		for (float i = 1; i >= 0; i -= 0.01f) {
			vertices.push_back(VertexFormat(glm::vec3(i, -sqrt(1 - i * i), 0)));
		}

		vector<unsigned short> indices;
		for (int i = 0; i < vertices.size(); ++i) {
			indices.push_back(i);
		}

		meshes["bow"] = new Mesh("bow");
		meshes["bow"]->InitFromData(vertices, indices);
		meshes["bow"]->SetDrawMode(GL_LINE_LOOP);
	}
	// Create arrow mesh
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-1.1f, 0, 0)),
			VertexFormat(glm::vec3(-0.25f, 0, 0)),
			VertexFormat(glm::vec3(-0.25f, 0.1f, 0)),
			VertexFormat(glm::vec3(0, 0, 0)),
			VertexFormat(glm::vec3(-0.25f, -0.1f, 0)),
			VertexFormat(glm::vec3(-0.25f, 0, 0))
		};

		vector<unsigned short> indices;
		for (int i = 0; i < vertices.size(); ++i) {
			indices.push_back(i);
		}

		meshes["arrow"] = new Mesh("arrow");
		meshes["arrow"]->InitFromData(vertices, indices);
		meshes["arrow"]->SetDrawMode(GL_LINE_LOOP);
	}
	// Create red balloon mesh
	{
		vector<VertexFormat> vertices;
		vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0))); // center
		// 1-404
		for (float i = 0; i >= -1; i -= 0.01f) {
			vertices.push_back(VertexFormat(glm::vec3(i, -sqrt(1 - i * i), 0), glm::vec3(1, 0, 0)));
		}
		for (float i = -1; i <= 0; i += 0.01f) {
			vertices.push_back(VertexFormat(glm::vec3(i, sqrt(1 - i * i), 0), glm::vec3(1, 0, 0)));
		}
		for (float i = 0; i <= 1; i += 0.01f) {
			vertices.push_back(VertexFormat(glm::vec3(i, sqrt(1 - i * i), 0), glm::vec3(1, 0, 0)));
		}
		for (float i = 1; i >= 0; i -= 0.01f) {
			vertices.push_back(VertexFormat(glm::vec3(i, -sqrt(1 - i * i), 0), glm::vec3(1, 0, 0)));
		}
		// 405-407
		vertices.push_back(VertexFormat(glm::vec3(-0.2f, -1.1f, 0), glm::vec3(1, 0, 0)));
		vertices.push_back(VertexFormat(glm::vec3(0.2f, -1.1f, 0), glm::vec3(1, 0, 0)));
		vertices.push_back(VertexFormat(glm::vec3(0, -0.85f, 0), glm::vec3(1, 0, 0)));
		// 408-415
		vertices.push_back(VertexFormat(glm::vec3(-0.05f, -1.1f, 0), glm::vec3(1, 0, 0)));
		vertices.push_back(VertexFormat(glm::vec3(0, -1.1f, 0), glm::vec3(1, 0, 0)));

		vertices.push_back(VertexFormat(glm::vec3(-0.2f, -1.25f, 0), glm::vec3(1, 0, 0)));
		vertices.push_back(VertexFormat(glm::vec3(-0.15f, -1.25f, 0), glm::vec3(1, 0, 0)));

		vertices.push_back(VertexFormat(glm::vec3(0, -1.35f, 0), glm::vec3(1, 0, 0)));
		vertices.push_back(VertexFormat(glm::vec3(0.05f, -1.35f, 0), glm::vec3(1, 0, 0)));

		vertices.push_back(VertexFormat(glm::vec3(-0.1f, -1.5f, 0), glm::vec3(1, 0, 0)));
		vertices.push_back(VertexFormat(glm::vec3(-0.05f, -1.5f, 0), glm::vec3(1, 0, 0)));
		
		vector<unsigned short> indices;

		for (int i = 2; i <= 404; ++i) {
			indices.push_back(0);
			indices.push_back(i - 1);
			indices.push_back(i);
		}
		indices.push_back(405); indices.push_back(406); indices.push_back(407);
		indices.push_back(408); indices.push_back(410); indices.push_back(411);
		indices.push_back(408); indices.push_back(411); indices.push_back(409);
		indices.push_back(410); indices.push_back(412); indices.push_back(413);
		indices.push_back(410); indices.push_back(413); indices.push_back(411);
		indices.push_back(412); indices.push_back(414); indices.push_back(415);
		indices.push_back(412); indices.push_back(413); indices.push_back(415);

		meshes["red_balloon"] = new Mesh("red_balloon");
		meshes["red_balloon"]->InitFromData(vertices, indices);
		meshes["red_balloon"]->SetDrawMode(GL_TRIANGLES);
	}
	// Create yellow balloon mesh
	{
		vector<VertexFormat> vertices;
		vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 1, 0))); // center
		// 1-404
		for (float i = 0; i >= -1; i -= 0.01f) {
			vertices.push_back(VertexFormat(glm::vec3(i, -sqrt(1 - i * i), 0), glm::vec3(1, 1, 0)));
		}
		for (float i = -1; i <= 0; i += 0.01f) {
			vertices.push_back(VertexFormat(glm::vec3(i, sqrt(1 - i * i), 0), glm::vec3(1, 1, 0)));
		}
		for (float i = 0; i <= 1; i += 0.01f) {
			vertices.push_back(VertexFormat(glm::vec3(i, sqrt(1 - i * i), 0), glm::vec3(1, 1, 0)));
		}
		for (float i = 1; i >= 0; i -= 0.01f) {
			vertices.push_back(VertexFormat(glm::vec3(i, -sqrt(1 - i * i), 0), glm::vec3(1, 1, 0)));
		}
		// 405-407
		vertices.push_back(VertexFormat(glm::vec3(-0.2f, -1.1f, 0), glm::vec3(1, 1, 0)));
		vertices.push_back(VertexFormat(glm::vec3(0.2f, -1.1f, 0), glm::vec3(1, 1, 0)));
		vertices.push_back(VertexFormat(glm::vec3(0, -0.85f, 0), glm::vec3(1, 1, 0)));
		// 408-415
		vertices.push_back(VertexFormat(glm::vec3(-0.05f, -1.1f, 0), glm::vec3(1, 1, 0)));
		vertices.push_back(VertexFormat(glm::vec3(0, -1.1f, 0), glm::vec3(1, 1, 0)));

		vertices.push_back(VertexFormat(glm::vec3(-0.2f, -1.25f, 0), glm::vec3(1, 1, 0)));
		vertices.push_back(VertexFormat(glm::vec3(-0.15f, -1.25f, 0), glm::vec3(1, 1, 0)));

		vertices.push_back(VertexFormat(glm::vec3(0, -1.35f, 0), glm::vec3(1, 1, 0)));
		vertices.push_back(VertexFormat(glm::vec3(0.05f, -1.35f, 0), glm::vec3(1, 1, 0)));

		vertices.push_back(VertexFormat(glm::vec3(-0.1f, -1.5f, 0), glm::vec3(1, 1, 0)));
		vertices.push_back(VertexFormat(glm::vec3(-0.05f, -1.5f, 0), glm::vec3(1, 1, 0)));

		vector<unsigned short> indices;

		for (int i = 2; i <= 404; ++i) {
			indices.push_back(0);
			indices.push_back(i - 1);
			indices.push_back(i);
		}
		indices.push_back(405); indices.push_back(406); indices.push_back(407);
		indices.push_back(408); indices.push_back(410); indices.push_back(411);
		indices.push_back(408); indices.push_back(411); indices.push_back(409);
		indices.push_back(410); indices.push_back(412); indices.push_back(413);
		indices.push_back(410); indices.push_back(413); indices.push_back(411);
		indices.push_back(412); indices.push_back(414); indices.push_back(415);
		indices.push_back(412); indices.push_back(413); indices.push_back(415);

		meshes["yellow_balloon"] = new Mesh("yellow_balloon");
		meshes["yellow_balloon"]->InitFromData(vertices, indices);
		meshes["yellow_balloon"]->SetDrawMode(GL_TRIANGLES);
	}
	// Create shuriken mesh
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(0, 0, 0)),
			VertexFormat(glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(1, 1, 0)),
			VertexFormat(glm::vec3(0, 1, 0)),
			VertexFormat(glm::vec3(-1, 1, 0)),
			VertexFormat(glm::vec3(-1, 0, 0)),
			VertexFormat(glm::vec3(-1, -1, 0)),
			VertexFormat(glm::vec3(0, -1, 0)),
			VertexFormat(glm::vec3(1, -1, 0))
		};
		vector<unsigned short> indices =
		{
			0, 1, 2,
			0, 3, 4,
			0, 5, 6,
			0, 7, 8
		};
		meshes["shuriken"] = new Mesh("shuriken");
		meshes["shuriken"]->InitFromData(vertices, indices);
		meshes["shuriken"]->SetDrawMode(GL_TRIANGLES);
	}
	// Create rectangle mesh (for power bar)
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)),
			VertexFormat(glm::vec3(5, 0, 0), glm::vec3(0, 1, 0)),
			VertexFormat(glm::vec3(5, 1, 0), glm::vec3(0, 1, 0)),
			VertexFormat(glm::vec3(0, 1, 0), glm::vec3(0, 1, 0))
		};
		vector<unsigned short> indices =
		{
			0, 1, 2,
			0, 2, 3
		};
		meshes["rectangle"] = new Mesh("rectangle");
		meshes["rectangle"]->InitFromData(vertices, indices);
		meshes["rectangle"]->SetDrawMode(GL_TRIANGLES);
	}
	// Create cross mesh (for player's lives)
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(2.5f, -2.5f, 0), glm::vec3(0, 1, 0)),
			VertexFormat(glm::vec3(7.5f, -2.5f, 0), glm::vec3(0, 1, 0)),
			VertexFormat(glm::vec3(7.5f, 2.5f, 0), glm::vec3(0, 1, 0)),
			VertexFormat(glm::vec3(2.5f, 2.5f, 0), glm::vec3(0, 1, 0)),
			VertexFormat(glm::vec3(2.5f, 7.5f, 0), glm::vec3(0, 1, 0)),
			VertexFormat(glm::vec3(-2.5f, 7.5f, 0), glm::vec3(0, 1, 0)),
			VertexFormat(glm::vec3(-2.5f, 2.5f, 0), glm::vec3(0, 1, 0)),
			VertexFormat(glm::vec3(-7.5f, 2.5f, 0), glm::vec3(0, 1, 0)),
			VertexFormat(glm::vec3(-7.5f, -2.5f, 0), glm::vec3(0, 1, 0)),
			VertexFormat(glm::vec3(-2.5f, -2.5f, 0), glm::vec3(0, 1, 0)),
			VertexFormat(glm::vec3(-2.5f, -7.5f, 0), glm::vec3(0, 1, 0)),
			VertexFormat(glm::vec3(2.5f, -7.5f, 0), glm::vec3(0, 1, 0))
		};
		vector<unsigned short> indices =
		{
			0, 1, 2,
			0, 2, 3,
			0, 3, 7,
			0, 7, 8,
			3, 4, 5,
			3, 5, 6,
			0, 9, 10,
			0, 10, 11
		};
		meshes["cross"] = new Mesh("cross");
		meshes["cross"]->InitFromData(vertices, indices);
		meshes["cross"]->SetDrawMode(GL_TRIANGLES);
	}

	//Initialize objects and parameters
	
	// set bow parameters
	bow_scale_x = 0.025f * resolution.x;
	bow_scale_y = 0.07f * resolution.y;
	bow_initial_y = (float)resolution.y / 2;
	bow_y = bow_initial_y;
	bow_offset = 0;
	bow_offset_step = (float)resolution.y / 2;
	bow_offset_max = (float)resolution.y / 2 - bow_scale_y / 2;
	bow_offset_min = -(float)resolution.y / 2 + bow_scale_y / 2;

	// set arrow parameters
	arrow_scale_x = 0.07f * resolution.x;
	arrow_scale_y = 0.12f * resolution.y;
	arrow_initial_x = bow_x + arrow_scale_x;
	arrow_speed_current_percentage = 0;
	arrow_speed_current_step = 0.66f; // % per second
	arrow_speed_max = 0.8f * resolution.x; // units per second
	stopped_timer_step = 2.0f;

	// initialize arrow
	arrows.push_back(Arrow(arrow_initial_x, bow_y, 0, 0));

	// set power bar parameters
	power_bar_scale = 0.004f * (resolution.x + resolution.y);
	power_bar_x = bow_x;
	power_bar_y_offset = 0.1f * resolution.y;
	power_bar_y = bow_y - power_bar_y_offset;

	// set balloons parameters
	balloon_scale_x = 0.0225f * resolution.x;
	balloon_scale_y = 0.06f * resolution.y;
	balloon_speed_min = 0.175f * resolution.y;
	balloon_speed_max = 0.5f * resolution.y;
	balloon_timer = 0.0f;
	balloon_timer_step = 1.0f;
	balloon_pop_scale_step = 2.0f;
	swing_step = 2.25f;

	// set shuriken parameters
	shuriken_scale = 0.01f * (resolution.x + resolution.y);
	shuriken_speed_min = 0.2f * resolution.x;
	shuriken_speed_max = 0.5f * resolution.x;
	shuriken_timer = 0;
	shuriken_timer_step = 0.7f;
	shuriken_rotation_step = 3.5f;

	// set cross parameters
	cross_scale = 0.001f * (resolution.x + resolution.y);
	cross1_x = resolution.x * 0.96f;
	cross2_x = cross1_x - 17.5f * cross_scale;
	cross3_x = cross1_x - 35.0f * cross_scale;
	cross_y = resolution.y * 0.95f;
	nr_lives = 3;
	score = 0;
	difficulty = 1;
}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor((GLclampf)0.02, (GLclampf)0, (GLclampf)0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);

	glm::ivec2 resolution = window->GetResolution();

	// update bow position
	bow_y = bow_initial_y + bow_offset;

	// update arrows positions
	int nr_arrows = (int)arrows.size();
	for (int i = 0; i < nr_arrows - 1; ++i) {
		if (arrows[i].speed == 0) {
			arrows[i].stopped_timer -= stopped_timer_step * deltaTimeSeconds;
			if (arrows[i].stopped_timer <= 0.0f) {
				// erase arrow
				arrows.erase(arrows.begin() + i);
				i--;
				nr_arrows--;
			}
		}
		else {
			arrows[i].x += arrows[i].speed * deltaTimeSeconds * cos(arrows[i].angle);
			arrows[i].y += arrows[i].speed * deltaTimeSeconds * sin(arrows[i].angle);
			if (arrows[i].x - arrow_scale_x > resolution.x || arrows[i].y > resolution.y || arrows[i].y < 0) {
				// erase arrow
				arrows.erase(arrows.begin() + i);
				i--;
				nr_arrows--;
			}
		}
	}
	// current arrow (last element of the vector)
	arrows[nr_arrows - 1].x = arrow_initial_x;
	arrows[nr_arrows - 1].y = bow_y;

	// get mouse cursor position to calculate current arrow's angle
	double cursor_x, cursor_y;
	glfwGetCursorPos(glfwGetCurrentContext(), &cursor_x, &cursor_y);
	float angle = (float)atan(((resolution.y - cursor_y) - bow_y) / max(0.001, cursor_x - bow_x));
	arrows.back().angle = angle;

	// update power bar
	power_bar_y = bow_y - power_bar_y_offset;

	// update balloons
	balloon_timer += balloon_timer_step * deltaTimeSeconds;
	float balloon_x, balloon_y, balloon_speed;
	bool balloon_color;
	float balloon_swing_angle;
	while (balloon_timer > 1.0f) {
		balloon_timer -= 1.0f;
		balloon_x = rand() % (int)(resolution.x - arrow_initial_x - 30) + arrow_initial_x + 30; // 30 (30 * sin(2PI)) compensates for balloon's swing
		balloon_y = -balloon_scale_y;
		balloon_speed = rand() % max(1, (int)(balloon_speed_max - balloon_speed_min)) + balloon_speed_min;
		balloon_color = rand() % 2;
		balloon_swing_angle = (float)(rand() % (int)(circumference * 1000.0f)) / 1000.0f;
		balloons.push_back(Balloon(balloon_x, balloon_y, balloon_speed, balloon_color, balloon_swing_angle));
	}
	int nr_balloons = (int)balloons.size();
	for (int i = 0; i < nr_balloons; ++i) {
		if (balloons[i].popped) {
			balloons[i].popped_scale -= balloon_pop_scale_step * deltaTimeSeconds;
			if (balloons[i].popped_scale <= 0.0f) {
				// erase balloon
				balloons.erase(balloons.begin() + i);
				i--;
				nr_balloons--;
			}
		}
		else {
			balloons[i].swing_angle += swing_step * deltaTimeSeconds;
			balloons[i].x = balloons[i].initial_x + 30.0f * sin(balloons[i].swing_angle);
			balloons[i].y += balloons[i].speed * deltaTimeSeconds;
			// if out of screen
			if (balloons[i].y - balloon_scale_y > resolution.y) {
				// erase balloon
				balloons.erase(balloons.begin() + i);
				i--;
				nr_balloons--;
			}
			else {
				// check arrow collision
				// takes into account arrow's rotation
				// does not consider stopped arrows
				for (int j = 0; j < nr_arrows; ++j) {
					if (arrows[j].speed > 0) {
						float arrow_x = arrow_scale_x * cos(arrows[j].angle) + arrows[j].x - arrow_scale_x;
						float arrow_y = arrow_scale_y * sin(arrows[j].angle) + arrows[j].y;
						// ellipse equation
						if ((arrow_x - balloons[i].x) * (arrow_x - balloons[i].x) / (balloon_scale_x * balloon_scale_x)
							+ (arrow_y - balloons[i].y) * (arrow_y - balloons[i].y) / (balloon_scale_y * balloon_scale_y) <= 1) {
							// balloon popped
							balloons[i].popped = true;
							if (balloons[i].color) {
								score++;
								cout << "Score: " << score << '\n';
							}
							else {
								score--;
								cout << "Score: " << score << '\n';
							}
							break;
						}
					}
				}
			}
		}
	}

	// update shurikens
	shuriken_timer += shuriken_timer_step * deltaTimeSeconds;
	float shuriken_x, shuriken_y, shuriken_speed, shuriken_rotation;
	while (shuriken_timer > 1.0f) {
		shuriken_timer -= 1.0f;
		shuriken_x = resolution.x + shuriken_scale;
		shuriken_y = (float)(rand() % (int)resolution.y);
		shuriken_speed = rand() % max(1, (int)(shuriken_speed_max - shuriken_speed_min)) + shuriken_speed_min;
		shuriken_rotation = (float)(rand() % (int)(circumference * 1000.0f)) / 1000.0f;
		shurikens.push_back(Shuriken(shuriken_x, shuriken_y, shuriken_speed, shuriken_rotation));
	}
	int nr_shurikens = (int)shurikens.size();
	for (int i = 0; i < nr_shurikens; ++i) {
		if (shurikens[i].speed == 0) {
			shurikens[i].stopped_timer -= stopped_timer_step * deltaTimeSeconds;
			if (shurikens[i].stopped_timer <= 0.0f) {
				// erase shuriken
				shurikens.erase(shurikens.begin() + i);
				i--;
				nr_shurikens--;
			}
		}
		else {
			shurikens[i].x -= shurikens[i].speed * deltaTimeSeconds;
			shurikens[i].rotation += shuriken_rotation_step * deltaTimeSeconds;
			while (shurikens[i].rotation >= circumference) {
				shurikens[i].rotation -= circumference;
			}
			// if out of screen
			if (shurikens[i].x + shuriken_scale < 0) {
				// erase shuriken
				shurikens.erase(shurikens.begin() + i);
				i--;
				nr_shurikens--;
			}
			else {
				// check bow collision
				// using 3 points on the shuriken's circle
				// left
				float shuriken_x1 = shurikens[i].x - shuriken_scale;
				float shuriken_y1 = shurikens[i].y;
				// up
				float shuriken_x2 = shurikens[i].x;
				float shuriken_y2 = shurikens[i].y + shuriken_scale;
				// down
				float shuriken_x3 = shurikens[i].x;
				float shuriken_y3 = shurikens[i].y - shuriken_scale;
				// ellipse equation
				if ((shuriken_x1 - bow_x) * (shuriken_x1 - bow_x) / (bow_scale_x * bow_scale_x)
					+ (shuriken_y1 - bow_y) * (shuriken_y1 - bow_y) / (bow_scale_y * bow_scale_y) <= 1 ||
					(shuriken_x2 - bow_x) * (shuriken_x2 - bow_x) / (bow_scale_x * bow_scale_x)
					+ (shuriken_y2 - bow_y) * (shuriken_y2 - bow_y) / (bow_scale_y * bow_scale_y) <= 1 ||
					(shuriken_x3 - bow_x) * (shuriken_x3 - bow_x) / (bow_scale_x * bow_scale_x)
					+ (shuriken_y3 - bow_y) * (shuriken_y3 - bow_y) / (bow_scale_y * bow_scale_y) <= 1) {
					// erase shuriken
					shurikens.erase(shurikens.begin() + i);
					i--;
					nr_shurikens--;
					// decrement player's lives
					nr_lives--;
					// print score
					if (nr_lives == 0) {
						cout << "Final score: " << score << '\n';
						exit(0);
					}
				}
				else {
					// check arrow collision
					// takes into account arrow's rotation
					// does not consider current arrow or stopped arrows
					for (int j = 0; j < nr_arrows - 1; ++j) {
						if (arrows[j].speed > 0) {
							float arrow_x = arrow_scale_x * cos(arrows[j].angle) + arrows[j].x - arrow_scale_x;
							float arrow_y = arrow_scale_y * sin(arrows[j].angle) + arrows[j].y;
							// ellipse equation
							float distance = sqrt((shurikens[i].x - arrow_x) * (shurikens[i].x - arrow_x) + (shurikens[i].y - arrow_y) * (shurikens[i].y - arrow_y));
							if (distance <= shuriken_scale) {
								// arrow & shuriken stopped
								score++;
								cout << "Score: " << score << '\n';
								arrows[j].speed = 0;
								shurikens[i].speed = 0;
							}
						}
					}
				}
			}
		}
	}

	// render bow
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(bow_x, bow_y);
	modelMatrix *= Transform2D::Scale(bow_scale_x, bow_scale_y);
	RenderMesh2D(meshes["bow"], shaders["VertexColor"], modelMatrix);

	// render power bar
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(power_bar_x, power_bar_y);
	modelMatrix *= Transform2D::Scale(power_bar_scale * arrow_speed_current_percentage, power_bar_scale);
	RenderMesh2D(meshes["rectangle"], shaders["VertexColor"], modelMatrix);

	// render balloons
	for (int i = 0; i < nr_balloons; ++i) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(balloons[i].x, balloons[i].y);
		modelMatrix *= Transform2D::Scale(balloon_scale_x * balloons[i].popped_scale, balloon_scale_y * balloons[i].popped_scale);
		if (balloons[i].color) {
			RenderMesh2D(meshes["red_balloon"], shaders["VertexColor"], modelMatrix);
		}
		else {
			RenderMesh2D(meshes["yellow_balloon"], shaders["VertexColor"], modelMatrix);
		}
	}

	// render shurikens
	for (int i = 0; i < nr_shurikens; ++i) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(shurikens[i].x, shurikens[i].y);
		modelMatrix *= Transform2D::Rotate(shurikens[i].rotation);
		modelMatrix *= Transform2D::Scale(shuriken_scale, shuriken_scale);
		RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrix);
	}

	// render crosses (player's lives)
	// cross 1
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(cross1_x, cross_y);
	modelMatrix *= Transform2D::Rotate(cross_rotation);
	modelMatrix *= Transform2D::Scale(cross_scale, cross_scale);
	RenderMesh2D(meshes["cross"], shaders["VertexColor"], modelMatrix);
	if (nr_lives > 1) {
		// cross 2
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(cross2_x, cross_y);
		modelMatrix *= Transform2D::Rotate(cross_rotation);
		modelMatrix *= Transform2D::Scale(cross_scale, cross_scale);
		RenderMesh2D(meshes["cross"], shaders["VertexColor"], modelMatrix);
		if (nr_lives > 2) {
			// cross 3
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(cross3_x, cross_y);
			modelMatrix *= Transform2D::Rotate(cross_rotation);
			modelMatrix *= Transform2D::Scale(cross_scale, cross_scale);
			RenderMesh2D(meshes["cross"], shaders["VertexColor"], modelMatrix);
		}
	}

	// render arrows
	for (int i = 0; i < nr_arrows; ++i) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(arrows[i].x, arrows[i].y);

		modelMatrix *= Transform2D::Translate(-arrow_scale_x, 0);;
		modelMatrix *= Transform2D::Rotate(arrows[i].angle);
		modelMatrix *= Transform2D::Translate(arrow_scale_x, 0);

		modelMatrix *= Transform2D::Scale(arrow_scale_x, arrow_scale_y);
		RenderMesh2D(meshes["arrow"], shaders["VertexColor"], modelMatrix);
	}
	
}

void Tema1::FrameEnd()
{
	//DrawCoordinatSystem();
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
		
		if (window->KeyHold(GLFW_KEY_W)) {
			bow_offset = min(bow_offset_max, bow_offset + bow_offset_step * deltaTime);
		}
		if (window->KeyHold(GLFW_KEY_S)) {
			bow_offset = max(bow_offset_min, bow_offset - bow_offset_step * deltaTime);
		}
		if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
			// increase arrow speed
			arrow_speed_current_percentage = min(1.0f, arrow_speed_current_percentage + arrow_speed_current_step * deltaTime);
		}
	}
}

void Tema1::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_1)
	{
		if (difficulty == 2) {
			// switch to normal mode
			difficulty = 1;
			glm::ivec2 resolution = window->GetResolution();
			// set bow parameters
			bow_offset_step = (float)resolution.y / 2;
			// set arrow parameters
			arrow_speed_current_step = 0.66f; // % per second
			arrow_speed_max = 0.8f * resolution.x; // units per second
			stopped_timer_step = 2.0f;

			// set balloons parameters
		
			balloon_speed_min = 0.175f * resolution.y;
			balloon_speed_max = 0.5f * resolution.y;
			balloon_timer_step = 1.0f;
			balloon_pop_scale_step = 2.0f;
			swing_step = 2.25f;

			// set shuriken parameters
			shuriken_speed_min = 0.2f * resolution.x;
			shuriken_speed_max = 0.5f * resolution.x;
			shuriken_timer_step = 0.7f;
			shuriken_rotation_step = 3.5f;

		}
	}
	if (key == GLFW_KEY_2)
	{
		// switch to hard mode
		if (difficulty == 1) {
			difficulty = 2;
			glm::ivec2 resolution = window->GetResolution();
			// set bow parameters
			bow_offset_step = (float)resolution.y * 0.65f;
			// set arrow parameters
			arrow_speed_current_step = 2.0f; // % per second
			arrow_speed_max = 1.5f * resolution.x; // units per second
			stopped_timer_step = 2.25f;

			// set balloons parameters
			balloon_speed_min = 0.5f * resolution.y;
			balloon_speed_max = 0.8f * resolution.y;
			balloon_timer_step = 2.5f;
			balloon_pop_scale_step = 2.25f;
			swing_step = 2.5f;

			// set shuriken parameters
			shuriken_speed_min = 0.3f * resolution.x;
			shuriken_speed_max = 0.75f * resolution.x;
			shuriken_timer_step = 1.65f;
			shuriken_rotation_step = 4.0f;
		}
	}
}

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
	if (button == RI_MOUSE_LEFT_BUTTON_DOWN) {
		// arrow speed starts at 10%
		arrow_speed_current_percentage = 0.1f;
	}
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
	if (button == RI_MOUSE_LEFT_BUTTON_DOWN) {
		arrows[arrows.size() - 1].speed = arrow_speed_max * arrow_speed_current_percentage;
		arrows.push_back(Arrow(arrow_initial_x, bow_y, 0, 0));
		arrow_speed_current_percentage = 0.0f;
	}
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
	glm::ivec2 resolution = window->GetResolution();
	if (difficulty == 1) {
		// update bow parameters
		bow_scale_x = 0.025f * resolution.x;
		bow_scale_y = 0.07f * resolution.y;
		bow_initial_y = (float)resolution.y / 2;
		if (difficulty == 1) {
			bow_offset_step = (float)resolution.y / 2;
		}
		else {
			bow_offset_step = (float)resolution.y * 0.65f;
		}
		bow_offset_max = (float)resolution.y / 2 - bow_scale_y / 2;
		bow_offset_min = -(float)resolution.y / 2 + bow_scale_y / 2;
		// update arrow parameters
		arrow_scale_x = 0.07f * resolution.x;
		arrow_scale_y = 0.12f * resolution.y;
		arrow_initial_x = bow_x + arrow_scale_x;
		if (difficulty == 1) {
			arrow_speed_max = 0.8f * resolution.x;
		}
		else {
			arrow_speed_max = 1.5f * resolution.x;
		}
		//update power bar parameters
		power_bar_scale = 0.004f * (resolution.x + resolution.y);
		power_bar_y_offset = 0.1f * resolution.y;
		// update balloons parameters
		balloon_scale_x = 0.0225f * resolution.x;
		balloon_scale_y = 0.06f * resolution.y;
		if (difficulty == 1) {
			balloon_speed_min = 0.175f * resolution.y;
			balloon_speed_max = 0.5f * resolution.y;
		}
		else {
			balloon_speed_min = 0.5f * resolution.y;
			balloon_speed_max = 0.8f * resolution.y;
		}
		// update shuriken parameters
		shuriken_scale = 0.01f * (resolution.x + resolution.y);
		if (difficulty == 1) {
			shuriken_speed_min = 0.2f * resolution.x;
			shuriken_speed_max = 0.5f * resolution.x;
		}
		else {
			shuriken_speed_min = 0.3f * resolution.x;
			shuriken_speed_max = 0.75f * resolution.x;
		}
		// update cross parameters
		cross_scale = 0.001f * (resolution.x + resolution.y);
		cross1_x = resolution.x * 0.96f;
		cross2_x = cross1_x - 17.5f * cross_scale;
		cross3_x = cross1_x - 35.0f * cross_scale;
		cross_y = resolution.y * 0.95f;
	}
}
