#pragma once
#include <Component/SimpleScene.h>

class Laborator4 : public SimpleScene
{
	public:
		Laborator4();
		~Laborator4();

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
		glm::mat4 modelMatrix;
		GLenum polygonMode;
		const float circumference = 6.2831f;

		float translateX;
		float translateY;
		float translateZ;
		float translateStep;

		float scaleX;
		float scaleY;
		float scaleZ;
		float scaleStep;
		float scaleTopLimit;
		float scaleBottomLimit;

		float rotationOX;
		float rotationOY;
		float rotationOZ;
		float angularStepOX;
		float angularStepOY;
		float angularStepOZ;

		// jumping cube parameters
		bool go_up;
		float tx4;
		float ty4;
		float step_x;
		float step_y;
		float rot4;
		float step_rot;
};
