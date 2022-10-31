#pragma once

#include <Component/SimpleScene.h>

class Laborator1 : public SimpleScene
{
	public:
		Laborator1();
		~Laborator1();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;


		GLclampf background_red;
		GLclampf background_green;
		GLclampf background_blue;

		std::string obj_meshes[4] = { "box", "teapot", "quad", "sphere" };
		std::string third_obj_mesh;
		glm::vec3 third_obj_position;


		glm::vec3 first_obj_position;
		bool go_up = false, go_down = false;
		float acceleration = 9.81f;
		float speed = 0.0f;
};
