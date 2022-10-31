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

	// initialize tx, ty and tz (the translation steps)
	translateX = 0;
	translateY = 0;
	translateZ = 0;
	translateStep = 2.0f;

	// initialize sx, sy and sz (the scale factors)
	scaleX = 1;
	scaleY = 1;
	scaleZ = 1;
	scaleStep = 2.0f;
	scaleTopLimit = 3.0f;
	scaleBottomLimit = 0.01f;
	
	// initialize angularSteps
	rotationOX = 0;
	rotationOY = 0;
	rotationOZ = 0;
	angularStepOX = 2.0f;
	angularStepOY = 2.0f;
	angularStepOZ = 2.0f;

	// initialize jumping cube parameters
	go_up = true;
	tx4 = 0;
	ty4 = 0;
	step_x = 0.5f;
	step_y = 5.0f;
	rot4 = 0;
	step_rot = circumference / 4.0f;
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
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-2.5f, 0.5f,-1.5f);
	modelMatrix *= Transform3D::Translate(translateX, translateY, translateZ);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	// 2nd cube
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0.0f, 0.5f, -1.5f);
	modelMatrix *= Transform3D::Scale(scaleX, scaleY, scaleZ);
	RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

	// 3rd cube
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(2.5f, 0.5f, -1.5f);
	//modelMatrix *= Transform3D::Translate(0, -1, -1);
	modelMatrix *= Transform3D::RotateOX(rotationOX);
	//modelMatrix *= Transform3D::Translate(0, 1, 1);
	modelMatrix *= Transform3D::RotateOY(rotationOY);
	modelMatrix *= Transform3D::RotateOZ(rotationOZ);
	//modelMatrix *= Transform3D::Translate(2.5f, 0.5f, -1.5f);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	// 4th cube (jumping)
	tx4 += step_x * deltaTimeSeconds;
	if (go_up) {
		ty4 += step_y * deltaTimeSeconds;
		if (ty4 >= step_y) {
			ty4 = step_y;
			go_up = false;
		}
	}
	else {
		ty4 -= step_y * deltaTimeSeconds;
		if (ty4 <= 0) {
			ty4 = 0;
			go_up = true;
		}
	}
	rot4 -= step_rot * deltaTimeSeconds;
	while (rot4 < 0) {
		rot4 += circumference;
	}
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(1.5f, 0.0f, -1.5f);
	modelMatrix *= Transform3D::Translate(tx4, ty4, 0);
	modelMatrix *= Transform3D::RotateOZ(rot4);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
}

void Laborator4::FrameEnd()
{
	DrawCoordinatSystem();
}

void Laborator4::OnInputUpdate(float deltaTime, int mods)
{
	// TODO
	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
		if (window->KeyHold(GLFW_KEY_D)) {
			translateX += translateStep * deltaTime;
		}
		if (window->KeyHold(GLFW_KEY_A)) {
			translateX -= translateStep * deltaTime;
		}
		if (window->KeyHold(GLFW_KEY_E)) {
			translateY += translateStep * deltaTime;
		}
		if (window->KeyHold(GLFW_KEY_Q)) {
			translateY -= translateStep * deltaTime;
		}
		if (window->KeyHold(GLFW_KEY_S)) {
			translateZ += translateStep * deltaTime;
		}
		if (window->KeyHold(GLFW_KEY_W)) {
			translateZ -= translateStep * deltaTime;
		}

		if (window->KeyHold(GLFW_KEY_1)) {
			scaleX = min(scaleTopLimit, scaleX + scaleStep * deltaTime);
			scaleY = min(scaleTopLimit, scaleY + scaleStep * deltaTime);
			scaleZ = min(scaleTopLimit, scaleZ + scaleStep * deltaTime);
		}
		if (window->KeyHold(GLFW_KEY_2)) {
			scaleX = max(scaleBottomLimit, scaleX - scaleStep * deltaTime);
			scaleY = max(scaleBottomLimit, scaleY - scaleStep * deltaTime);
			scaleZ = max(scaleBottomLimit, scaleZ - scaleStep * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_3)) {
			rotationOX += angularStepOX * deltaTime;
			while (rotationOX > circumference) {
				rotationOX -= circumference;
			}
		}
		if (window->KeyHold(GLFW_KEY_4)) {
			rotationOX -= angularStepOX * deltaTime;
			while (rotationOX < 0) {
				rotationOX += circumference;
			}
		}
		if (window->KeyHold(GLFW_KEY_5)) {
			rotationOY += angularStepOY * deltaTime;
			while (rotationOY > circumference) {
				rotationOY -= circumference;
			}
		}
		if (window->KeyHold(GLFW_KEY_6)) {
			rotationOY -= angularStepOY * deltaTime;
			while (rotationOY < 0) {
				rotationOY += circumference;
			}
		}

		if (window->KeyHold(GLFW_KEY_7)) {
			rotationOZ += angularStepOZ * deltaTime;
			while (rotationOZ > circumference) {
				rotationOZ -= circumference;
			}
		}
		if (window->KeyHold(GLFW_KEY_8)) {
			rotationOZ-= angularStepOZ * deltaTime;
			while (rotationOZ < 0) {
				rotationOZ += circumference;
			}
		}
	}
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
