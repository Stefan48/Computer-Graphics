#pragma once
#include <Component/SimpleScene.h>
#include <Core/Engine.h>
#include <Core/GPU/Mesh.h>

class Tema3 : public SimpleScene
{
public:
	Tema3();
	~Tema3();

	void Init() override;

private:
	struct Platform
	{
		float z;
		float scale_z;
		bool blocking;
		bool collectible;
		float ring_rotation;

		Platform(float z, float scale_z, bool blocking, bool collectible)
		{
			this->z = z;
			this->scale_z = scale_z;
			this->blocking = blocking;
			this->collectible = collectible;
			this->ring_rotation = RADIANS(90);
		}
	};

	struct Lamp
	{
		float z;
		glm::vec3 color;
		bool spot;

		Lamp(float z, glm::vec3 color, bool spot)
		{
			this->z = z;
			this->color = color;
			this->spot = spot;
		}
	};

	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color, unsigned int shininess, Texture2D* texture, bool light_source, bool background);

	void UpdateLanePlatforms(std::vector<Platform> &platforms, float deltaTime);
	void RenderLanePlatforms(std::vector<Platform> &platforms, float lane_x);
	bool CheckSpherePlatformsCollision(std::vector<Platform> &platforms);

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:

	// camera parameters
	EngineComponents::Camera *camera;
	glm::mat4 projectionMatrix;
	float camera_FoVy;
	glm::vec3 camera_position;

	// light parameters
	glm::vec3 lightPosition;
	float materialKd;
	float materialKs;
	unsigned int platform_material_shininess;
	unsigned int sphere_material_shininess;
	unsigned int lamp_material_shininess;
	unsigned int crate_material_shininess;
	unsigned int ring_material_shininess;

	// platform parameters
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
	// crate parameters
	float crate_scale;
	float crate_y;
	// ring parameters
	float ring_scale;
	float ring_y;
	float ring_rotation_step;
	int score;

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

	// lamp parameters
	float lamp_scale;
	float lamp_x1, lamp_x2;
	float lamp_y;
	float lamp_initial_z;
	float lamp_light_height;
	int lamps_nr;
	float lamp_distance;
	float lamp_erase_z;
	std::vector<Lamp> lamps;

	// texture parameters
	std::unordered_map<std::string, Texture2D*> textures;

	// skybox parameters
	float skybox_scale;

	// score display parameters
	float score_scale;
	float score_position_x, score_position_y, score_position_z;
	float score_offset;
};
