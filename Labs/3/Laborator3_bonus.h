#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

class Laborator3 : public SimpleScene
{
	public:
		Laborator3();
		~Laborator3();

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

	protected:
		glm::mat3 modelMatrix;
		float translateX, translateY;
		float scaleX, scaleY;
		float angularStep;

		float square_side;
		float corner1_x, corner1_y;
		float scale1;
		float scale2;
		float angle2;
		float angle2_step;
		float radius2;
		float scale3;
		float angle3;
		float angle3_step;
		float radius3;
		const float circumference = 6.2831f; // 2*PI
};
