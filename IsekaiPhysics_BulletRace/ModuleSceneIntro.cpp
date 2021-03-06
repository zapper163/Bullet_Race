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

	App->audio->PlayMusic("Assets/music.ogg");

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	// Lava // tiene que ser el primero
	CreateCube(1000, 1, 1000, 0, 0, 0, 0, 0, 0, 0, 2, 0.5, 0, false);

	//Corazones // tres siguientes
	CreateCube(1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, false);
	CreateCube(1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, false);
	CreateCube(1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, false);
	
	// Puerta //
	CreateCube(20, 10, 1, 0, 27, 55, 0, 0, 0, 0, 0.42, 0.42, 1);
	CreateCube(0, 10, 0, 10, 25, 55, 0, 0, 0, 0, 0.42, 0.42, 1);

	// Win // 
	CreateCube(40, 1, 20, -30, 20, 15, 0, 0, 0, 0, 0.42, 0.42, 0.42, true, 1.58f);
	

	// Map
	CreateCube(20, 1, 20, 0, 20, -5, 0, 0, 0, 0, 1, 0, 0, true, 0.0f);
	CreateCube(100, 1, 20, -60, 20, 95, 0, 0, 0, 0, 0.42, 0.42, 0.42, false);
	CreateCube(20, 1, 100, 0, 20, 55, 0, 0, 0, 0, 0.42, 0.42, 0.42, false);
	CreateCube(20, 1, 20, -120, 20, 95, 0, 0, 0, 0, 0.42, 0.42, 0.42, true, 3.16f);
	CreateCube(20, 1, 44, -120, 27.5, 64.5, 1, 0, 0, 20, 0.42, 0.42, 0.42, false);
	CreateCube(20, 1, 20, -120, 35, 34, 0, 0, 0, 0, 0.42, 0.42, 0.42, false);
	CreateCube(42, 1, 20, -91, 42, 34, 0, 0, 1, 20, 0.42, 0.42, 0.42, false);
	CreateCube(100, 1, 20, -21.5, 49.15, 34, 0, 0, 0, 0, 0.42, 0.42, 0.42, false);
	CreateCube(20, 1, 60, 38.5, 49.15, 14, 0, 0, 0, 0, 0.42, 0.42, 0.42, true, 3.16f);
	CreateCube(100, 1, 20, -18.25, 32.09, -6, 0, 0, 1, 20, 0.42, 0.42, 0.42, false);
	CreateCube(20, 1, 22, -105, 15, 15, 0, 0, 0, 0, 0.42, 0.42, 0.42, false);
	CreateCube(50, 1, 20, -90, 15, -6, 0, 0, 0, 0, 0.42, 0.42, 0.42, true, 4.74f);
	CreateCube(50, 1, 20, -75, 21, 15, 0, 0, 1, 15, 0.42, 0.42, 0.42, false);

	CreateCube(1, 10, 40, -10, 25, 15, 0, 0, 0, 0, 0.42, 0.42, 0.42);
	CreateCube(120, 30, 1, -65, 50, 24, 0, 0, 0, 0, 0.42, 0.42, 0.42);
	
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
	if(!city_init)
	{
		for (size_t i = 0; i < cubes.Count(); i++)
		{
			if (i == 0) {
				PhysBody3D* cuboCreado = App->physics->AddBody(*cubes.At(i), 0, false);
				cuboCreado->isDeath = true;
				cubes.At(i)->physObject = cuboCreado;
			}
			else if (i == 1 || i == 2 || i == 3) {
				PhysBody3D* cuboCreado = App->physics->AddBody(*cubes.At(i), 0, false);
				cuboCreado->isLife = true;
				cubes.At(i)->physObject = cuboCreado;
			}
			else if (i == 4) {
				PhysBody3D* cuboCreado = App->physics->AddBody(*cubes.At(i), 1, false);
				cubes.At(i)->physObject = cuboCreado;

			}
			else {
				PhysBody3D* cuboCreado = App->physics->AddBody(*cubes.At(i), 0, false);
				cubes.At(i)->physObject = cuboCreado;
			}
			
			
		}

		App->physics->AddConstraintHinge(*cubes.At(4)->physObject, *cubes.At(5)->physObject, vec3(10, 0, 0), vec3(0, 0, 0), vec3(0, 1, 0), vec3(0, 1, 0), false);

		for (size_t i = 0; i < checkPoints.Count(); i++)
		{
			if (i == 0) {
				PhysBody3D* checkCreado = App->physics->AddBody(*checkPoints.At(i), 0, true);
				checkPoints.At(i)->physObject = checkCreado;

				checkCreado->SetAsSensor(true);
				//checkCreado->SetId(1);
				checkCreado->isWin = true;
				checkPoints.At(i)->physObject = checkCreado;
			}
			else {
				PhysBody3D* checkCreado = App->physics->AddBody(*checkPoints.At(i), 0, true);
				checkPoints.At(i)->physObject = checkCreado;

				checkCreado->SetAsSensor(true);
				//checkCreado->SetId(1);
			}
			
		}

		
		city_init = true;
	}

	for (size_t i = 0; i < cubes.Count(); i++)
	{
		if (i != 4) {
			cubes.At(i)->Render();
		}
	}

	for (size_t i = 0; i < checkPoints.Count(); i++)
	{
		checkPoints.At(i)->Render();
	}

	return UPDATE_CONTINUE;
}


void ModuleSceneIntro::CreateCube(double long_x, double long_y, double long_z, double pos_x, double pos_y, double pos_z, double rot_x, double rot_y, double rot_z, float angle, float r, float g, float b, bool check, float roty)
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

	

	if (check == true) {
		Cube ch(3, 3, 3);
		ch.SetPos(pos_x, pos_y +3, pos_z);
		ch.color.r = 1;
		ch.color.g = 0;
		ch.color.b = 0;
		ch.checkPointCube = true;
		ch.rotY = roty;
		checkPoints.Insert(ch, checkPoints.Count());
	}

	c.checkPointCube = false;

	cubes.Insert(c, cubes.Count());
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{

}
