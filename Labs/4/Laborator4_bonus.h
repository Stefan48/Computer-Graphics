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
		float scale1, rot1, rot1_step;
		float radius1;
		float center1x, center1z;
		float scale2;
		float rot2VsCube1, rot2VsCube1_step;
		float rot2VsSelf, rot2VsSelf_step;
		float radius2;
		float center2x, center2z;
		float scale3;
		float rot3VsCube2, rot3VsCube2_step;
		float rot3VsSelf, rot3VsSelf_step;
		float radius3;
};
