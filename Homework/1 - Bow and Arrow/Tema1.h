#pragma once
#include <Component/SimpleScene.h>
#include <Core/Engine.h>
#include <vector>

class Tema1 : public SimpleScene
{
public:
	Tema1();
	~Tema1();

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
	struct Arrow
	{
		float x, y; // arrowhead coordinates
		float angle;
		float speed;
		float stopped_timer;

		Arrow(float x, float y, float angle, float speed) {
			this->x = x;
			this->y = y;
			this->angle = angle;
			this->speed = speed;
			this->stopped_timer = 1.0f;
		}
	};

	struct Balloon
	{
		float initial_x;
		float x, y; // center coordinates
		float speed;
		bool color; // 1 is red, 0 is yellow
		bool popped;
		float popped_scale;
		float swing_angle;

		Balloon(float initial_x, float y, float speed, bool color, float swing_angle) {
			this->initial_x = initial_x;
			this->x = this->initial_x + 30.0f * sin(swing_angle);
			this->y = y;
			this->speed = speed;
			this->color = color;
			this->popped = false;
			this->popped_scale = 1.0f;
			this->swing_angle = swing_angle;
		}
	};

	struct Shuriken
	{
		float x, y; // center coordinates
		float speed;
		float rotation;
		float stopped_timer;

		Shuriken(float x, float y, float speed, float rotation) {
			this->x = x;
			this->y = y;
			this->speed = speed;
			this->rotation = rotation;
			this->stopped_timer = 1.0f;
		}
	};

	glm::mat3 modelMatrix;
	GLenum polygonMode;
	const float circumference = 6.2831f;

	// bow parameters
	const float bow_x = 15;
	float bow_y, bow_initial_y;
	float bow_offset, bow_offset_step, bow_offset_max, bow_offset_min;
	float bow_scale_x, bow_scale_y;
	// arrow parameters
	float arrow_scale_x, arrow_scale_y;
	float arrow_initial_x;
	float arrow_speed_current_percentage, arrow_speed_current_step, arrow_speed_max;
	float stopped_timer_step;
	std::vector<Arrow> arrows;
	// power bar parameters
	float power_bar_scale;
	float power_bar_x, power_bar_y, power_bar_y_offset;
	// balloon parameters
	float balloon_scale_x, balloon_scale_y;
	float balloon_speed_min, balloon_speed_max;
	float balloon_timer, balloon_timer_step;
	float balloon_pop_scale_step;
	float swing_step;
	std::vector<Balloon> balloons;
	// shuriken parameters
	float shuriken_scale;
	float shuriken_speed_min, shuriken_speed_max;
	float shuriken_rotation_step;
	float shuriken_timer, shuriken_timer_step;
	std::vector<Shuriken> shurikens;
	// crosses parameters
	float cross_scale;
	const float cross_rotation = 0.7853f; // PI/4
	float cross1_x, cross2_x, cross3_x, cross_y;
	int nr_lives;
	int score;
	int difficulty; // 1 is normal, 2 is hard
};
