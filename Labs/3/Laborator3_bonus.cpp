#include "Laborator3.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;

Laborator3::Laborator3()
{
}

Laborator3::~Laborator3()
{
}

void Laborator3::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);
	float squareSide = 100;

	// compute coordinates of square center
	float cx = corner.x + squareSide / 2;
	float cy = corner.y + squareSide / 2;
	
	// initialize tx and ty (the translation steps)
	translateX = 0;
	translateY = 0;

	// initialize sx and sy (the scale factors)
	scaleX = 1;
	scaleY = 1;
	
	// initialize angularStep
	angularStep = 0;
	

	Mesh* square1 = Object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
	AddMeshToList(square1);
	
	Mesh* square2 = Object2D::CreateSquare("square2", corner, squareSide, glm::vec3(0, 1, 0));
	AddMeshToList(square2);

	Mesh* square3 = Object2D::CreateSquare("square3", corner, squareSide, glm::vec3(0, 0, 1));
	AddMeshToList(square3);


	square_side = 100;
	corner1_x = 575;
	corner1_y = 325;
	scale1 = 0.5f;
	scale2 = 0.75f;
	angle2 = 0;
	angle2_step = 1.5f;
	radius2 = 200;
	scale3 = 0.3f;
	angle3 = 0;
	angle3_step = 2.5f;
	radius3 = 90;
}

void Laborator3::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator3::Update(float deltaTimeSeconds)
{
	// TODO: update steps for translation, rotation, scale, in order to create animations
	
	// render square 1
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(corner1_x, corner1_y);
	modelMatrix *= Transform2D::Scale(scale1, scale1);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	// render square 2
	modelMatrix = glm::mat3(1);
	// update rotation angle for square 2
	angle2 += angle2_step * deltaTimeSeconds;
	while (angle2 > circumference) {
		angle2 -= circumference;
	}
	// rotate around first square's center
	modelMatrix *= Transform2D::Translate(corner1_x + square_side * scale1 / 2.0f, corner1_y + square_side * scale1 / 2.0f);
	modelMatrix *= Transform2D::Rotate(angle2);
	modelMatrix *= Transform2D::Translate(-(corner1_x + square_side * scale1 / 2.0f), -(corner1_y + square_side * scale1 / 2.0f));
	// translate by radius2 on X axis
	modelMatrix *= Transform2D::Translate(radius2, 0);
	// make square 1 and square 2 have the same center
	modelMatrix *= Transform2D::Translate(corner1_x + square_side * (scale1 - scale2) / 2.0f, corner1_y + square_side * (scale1 - scale2) / 2.0f);
	modelMatrix *= Transform2D::Scale(scale2, scale2);
	RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

	// render square 3
	modelMatrix = glm::mat3(1);
	// update rotation angle for square 3
	angle3 += angle3_step * deltaTimeSeconds;
	while (angle3 > circumference) {
		angle3 -= circumference;
	}
	// compute second square's center
	float square2center_x = corner1_x + square_side * scale1 / 2.0f + radius2 * cos(angle2);
	float square2center_y = corner1_y + square_side * scale1 / 2.0f + radius2 * sin(angle2);
	// rotate around second square's center
	modelMatrix *= Transform2D::Translate(square2center_x, square2center_y);
	modelMatrix *= Transform2D::Rotate(angle2 + angle3);
	modelMatrix *= Transform2D::Translate(-square2center_x, -square2center_y);
	// translate by radius 3 on X axis
	modelMatrix *= Transform2D::Translate(radius3, 0);
	// make square 2 and square 3 have the same center
	modelMatrix *= Transform2D::Translate(square2center_x - square_side * scale3 / 2.0f, square2center_y - square_side * scale3 / 2.0f);
	modelMatrix *= Transform2D::Scale(scale3, scale3);
	RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
}

void Laborator3::FrameEnd()
{

}

void Laborator3::OnInputUpdate(float deltaTime, int mods)
{
	
}

void Laborator3::OnKeyPress(int key, int mods)
{
	// add key press event
}

void Laborator3::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator3::OnWindowResize(int width, int height)
{
}
