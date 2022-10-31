#pragma once
#include <Component/SimpleScene.h>
#include <Core/Engine.h>
#include <Core/GPU/Mesh.h>

class Tema2 : public SimpleScene
{
public:
	Tema2();
	~Tema2();

	void Init() override;

private:
	struct Platform
	{
		float z;
		float scale_z;
		// color : {"blue", "red", "yellow", "orange", "green", "purple"}
		glm::vec3 color;

		Platform(float z, float scale_z, glm::vec3 color)
		{
			this->z = z;
			this->scale_z = scale_z;
			this->color = color;
		}

	};

	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color, unsigned int shininess, bool applyNoise = false);
	void RenderMeshUI(Mesh * mesh, Shader * shader, const glm::mat3 &modelMatrix);

	void UpdateLanePlatforms(std::vector<Platform> &platforms, float deltaTime);
	void RenderLanePlatforms(std::vector<Platform> &platforms, float lane_x);
	glm::vec3 GeneratePlatformColor();
	glm::vec3 CheckSpherePlatformsCollision(std::vector<Platform> &platforms);

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:

	// UI parameters
	glm::mat4 viewMatrixUI;
	glm::mat4 projectionMatrixUI;

	// camera parameters
	EngineComponents::Camera *camera;
	glm::mat4 projectionMatrix;
	float camera_FoVy;
	glm::vec3 camera_position_default;
	glm::vec3 camera_position;
	bool camera_first_person_active;
	glm::vec3 camera_first_person_offset;

	// fuel parameters
	float fuelbar_scale_x, fuelbar_scale_y;
	float fuelbar_x, fuelbar_reservoir_y, fuelbar_fuel_y;
	float fuel_percentage;
	float fuel_loss_step;
	// fuel_loss_step varies with lane speed
	float fuel_loss_step_step;

	// light parameters
	glm::vec3 lightPosition;
	float materialKd;
	float materialKs;
	unsigned int platform_material_shininess;
	unsigned int sphere_material_shininess;

	// platform parameters
	glm::vec3 platform_color_blue;
	glm::vec3 platform_color_red;
	glm::vec3 platform_color_yellow;
	glm::vec3 platform_color_orange;
	glm::vec3 platform_color_green;
	glm::vec3 platform_color_purple;
	float platform_y;
	float platform_scale_x, platform_scale_y;
	float platform_scale_z_min, platform_scale_z_med, platform_scale_z_max;
	float platform_z_initial;
	float platform_distance_min, platform_distance_max;
	// platform_distance_min/max varies with lane speed
	float platform_distance_step;
	float lane1_x, lane2_x, lane3_x;
	float lane_speed, lane_speed_step, lane_speed_min, lane_speed_max;
	int platforms_per_lane;
	float platform_yellow_fuel_loss;
	float platform_green_fuel_gain;
	float platform_orange_timer, platform_orange_timer_limit, platform_orange_timer_step;
	bool platform_orange_active;
	float platform_orange_speed_bonus;
	std::vector<Platform> platforms1;
	std::vector<Platform> platforms2;
	std::vector<Platform> platforms3;

	// sphere parameters
	glm::vec3 sphere_color;
	float sphere_scale;
	float sphere_y_initial;
	float sphere_x, sphere_y, sphere_z;
	bool sphere_up, sphere_down, sphere_left, sphere_right;
	float sphere_speed_x, sphere_speed_y;
	float sphere_jump_speed_switch, sphere_jump_speed_fwd;
	float gravity;
	int sphere_next_platform;
	bool sphere_effect_immunity;
};
