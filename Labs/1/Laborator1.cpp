#include "Laborator1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>

#include <time.h>

using namespace std;

// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/World.cpp

Laborator1::Laborator1()
{
	srand(time(NULL));
	background_red = 0.0f;
	background_green = 0.0f;
	background_blue = 0.0f;
}

Laborator1::~Laborator1()
{
}

void Laborator1::Init()
{
	// Load a mesh from file into GPU memory
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;

		//
		Mesh *mesh2 = new Mesh("teapot");
		mesh2->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "teapot.obj");
		meshes[mesh2->GetMeshID()] = mesh2;

		Mesh *mesh3 = new Mesh("quad");
		mesh3->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "quad.obj");
		meshes[mesh3->GetMeshID()] = mesh3;

		Mesh *mesh4 = new Mesh("sphere");
		mesh4->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh4->GetMeshID()] = mesh4;

		third_obj_mesh = obj_meshes[rand() % 4];
		third_obj_position = glm::vec3(0, 0.5f, 1);

		first_obj_position = glm::vec3(1.0f, 0.5f, 0.0f);
	}
}

void Laborator1::FrameStart()
{

}

void Laborator1::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->props.resolution;

	// sets the clear color for the color buffer
	//glClearColor(0, 0, 0, 1);
	glClearColor(background_red, background_green, background_blue, 1);

	// clears the color buffer (using the previously set color) and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

	// render the object
	//RenderMesh(meshes["box"], glm::vec3(1.0f, 0.5f, 0.0f), glm::vec3(0.5f));
	if (go_up) {
		first_obj_position.y += speed * deltaTimeSeconds;
		speed -= acceleration * deltaTimeSeconds;
		if (speed <= 0.0f) {
			go_up = false;
			go_down = true;
			speed = 0.0f;
		}
	}
	else if (go_down) {
		speed += acceleration * deltaTimeSeconds;
		first_obj_position.y -= speed * deltaTimeSeconds;
		if (first_obj_position.y < 0.5f) {
			first_obj_position.y = 0.5f;
			go_down = false;
		}
	}
	RenderMesh(meshes["box"], first_obj_position, glm::vec3(0.5f));

	// render the object again but with different properties
	RenderMesh(meshes["box"], glm::vec3(-1, 0.5f, 0));

	// render third object
	//RenderMesh(meshes["teapot"], glm::vec3(0, 0.5f, 1));
	RenderMesh(meshes[third_obj_mesh], third_obj_position);

}

void Laborator1::FrameEnd()
{
	DrawCoordinatSystem();
}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator1::OnInputUpdate(float deltaTime, int mods)
{
	// treat continuous update based on input
	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
		if (window->KeyHold(GLFW_KEY_W)) {
			third_obj_position.z -= 0.5f * deltaTime;
		}
		if (window->KeyHold(GLFW_KEY_S)) {
			third_obj_position.z += 0.5f * deltaTime;
		}
		if (window->KeyHold(GLFW_KEY_A)) {
			third_obj_position.x -= 0.5f * deltaTime;
		}
		if (window->KeyHold(GLFW_KEY_D)) {
			third_obj_position.x += 0.5f * deltaTime;
		}
		if (window->KeyHold(GLFW_KEY_E)) {
			third_obj_position.y += 0.5f * deltaTime;
		}
		if (window->KeyHold(GLFW_KEY_Q)) {
			third_obj_position.y -= 0.5f * deltaTime;
		}
	}
};

void Laborator1::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_F) {
		// do something
		background_red = (float) (rand() % 100) / 100.0f;
		background_green = (float) (rand() % 100) / 100.0f;
		background_blue = (float) (rand() % 100) / 100.0f;
	}

	if (key == GLFW_KEY_G) {
		// do something
		third_obj_mesh = obj_meshes[rand() % 4];
	}

	if (key == GLFW_KEY_SPACE) {
		go_up = true;
		go_down = false;
		speed = 5.0f;
	}
};

void Laborator1::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void Laborator1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void Laborator1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void Laborator1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void Laborator1::OnWindowResize(int width, int height)
{
	// treat window resize event
}
