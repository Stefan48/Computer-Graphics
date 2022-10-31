#include "Laborator4.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include "Transform3D.h"

using namespace std;

Laborator4::Laborator4()
{
}

Laborator4::~Laborator4()
{
}

void Laborator4::Init()
{
	polygonMode = GL_FILL;

	Mesh* mesh = new Mesh("box");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	meshes[mesh->GetMeshID()] = mesh;

	// initialize 1st cube
	scale1 = 1.2f;
	rot1 = 0;
	rot1_step = 0.5f;
	radius1 = 1.25f;
	// initialize 2nd cube
	scale2 = 0.7f;
	rot2VsCube1 = 0;
	rot2VsCube1_step = 1.0f;
	rot2VsSelf = 0;
	rot2VsSelf_step = 0.85f;
	radius2 = 3.0f;
	// initialize 3rd cube
	scale3 = 0.3f;
	rot3VsCube2 = 0;
	rot3VsCube2_step = 2.0f;
	rot3VsSelf = 0;
	rot3VsSelf_step = 3.0f;
	radius3 = 1.4f;
}

void Laborator4::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator4::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	// 1st cube
	// update position
	rot1 += rot1_step * deltaTimeSeconds;
	while (rot1 > circumference) {
		rot1 -= circumference;
	}
	// render
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::RotateOY(rot1);
	modelMatrix *= Transform3D::Translate(radius1, 0, 0);
	modelMatrix *= Transform3D::Scale(scale1, scale1, scale1);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	// 2nd cube
	// update position
	rot2VsCube1 += (rot1_step + rot2VsCube1_step) * deltaTimeSeconds;
	//rot2VsCube1 = rot1 + rot2VsCube1_step * deltaTimeSeconds; // <- wrong
	while (rot2VsCube1 > circumference) {
		rot2VsCube1 -= circumference;
	}
	rot2VsSelf += rot2VsSelf_step * deltaTimeSeconds;
	// calculate 1st cube's center
	center1x = radius1 * cos(rot1);
	center1z = radius1 * sin(-rot1);
	// render
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(center1x, 0, center1z);
	modelMatrix *= Transform3D::RotateOY(rot2VsCube1);
	modelMatrix *= Transform3D::Translate(radius2, 0, 0);
	modelMatrix *= Transform3D::RotateOY(rot2VsSelf);
	modelMatrix *= Transform3D::Scale(scale2, scale2, scale2);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	// 3rd cube
	// update position
	rot3VsCube2 += (rot1_step + rot2VsCube1_step + rot3VsCube2_step) * deltaTimeSeconds;
	while (rot3VsCube2 > circumference) {
		rot3VsCube2 -= circumference;
	}
	rot3VsSelf += rot3VsSelf_step * deltaTimeSeconds;
	// calculate 2nd cube's center
	center2x = center1x + radius2 * cos(rot2VsCube1);
	center2z = center1z + radius2 * sin(-rot2VsCube1);
	// render
	modelMatrix = glm::mat4(1);
	
	modelMatrix *= Transform3D::Translate(center2x, 0, center2z);
	modelMatrix *= Transform3D::RotateOY(rot3VsCube2);
	modelMatrix *= Transform3D::Translate(-center2x, 0, -center2z);
	modelMatrix *= Transform3D::Translate(radius3, 0, 0);

	modelMatrix *= Transform3D::Translate(center2x, 0, center2z);
	modelMatrix *= Transform3D::RotateOY(rot3VsSelf);
	modelMatrix *= Transform3D::Scale(scale3, scale3, scale3);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
}

void Laborator4::FrameEnd()
{
	DrawCoordinatSystem();
}

void Laborator4::OnInputUpdate(float deltaTime, int mods)
{
}

void Laborator4::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_SPACE)
	{
		switch (polygonMode)
		{
		case GL_POINT:
			polygonMode = GL_FILL;
			break;
		case GL_LINE:
			polygonMode = GL_POINT;
			break;
		default:
			polygonMode = GL_LINE;
			break;
		}
	}
}

void Laborator4::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator4::OnWindowResize(int width, int height)
{
}
