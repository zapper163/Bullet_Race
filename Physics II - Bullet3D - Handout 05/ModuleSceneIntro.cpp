#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePhysics3D.h"
#include "PhysVehicle3D.h"


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	// Lava
	CreateCube(1000, 1, 1000, 0, 0, 0, 0, 0, 0, 0, 2, 0.5, 0);

	// Map
	CreateCube(20, 1, 20, 0, 20, -5, 0, 0, 0, 0, 1, 0, 0);
	CreateCube(10, 1, 100, 0, 20, 55, 0, 0, 0, 0, 0.42, 0.42, 0.42);
	CreateCube(100, 1, 10, -55, 20, 100, 0, 0, 0, 0, 0.42, 0.42, 0.42);
	CreateCube(10, 1, 10, -110, 20, 100, 0, 0, 0, 0, 0.42, 0.42, 0.42);
	CreateCube(10, 1, 42, -110, 27, 75, 1, 0, 0, 20, 0.42, 0.42, 0.42);
	CreateCube(10, 1, 10, -110, 34, 50, 0, 0, 0, 0, 0.42, 0.42, 0.42);
	CreateCube(42, 1, 10, -85, 41, 50, 0, 0, 1, 20, 0.42, 0.42, 0.42);




	
	if(!city_init)
	{
		for (size_t i = 0; i < cubes.Count(); i++)
		{
			App->physics->AddBody(*cubes.At(i), 0);
		}
		
		city_init = true;
	}

	for (size_t i = 0; i < cubes.Count(); i++)
	{
		cubes.At(i)->Render();
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

void ModuleSceneIntro::CreateCube(double long_x, double long_y, double long_z, double pos_x, double pos_y, double pos_z, double rot_x, double rot_y, double rot_z, float angle, float r, float g, float b)
{
	Cube c(long_x, long_y, long_z);
	c.SetPos(pos_x, pos_y, pos_z);
	c.color.r = r;
	c.color.g = g;
	c.color.b = b;

	if (angle != 0)
	{
		c.SetRotation(angle, vec3(rot_x, rot_y, rot_z));
	}

	cubes.Insert(c, cubes.Count());
}