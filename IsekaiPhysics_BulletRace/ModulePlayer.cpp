#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "ModuleSceneIntro.h"
#include <math.h>

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 1.5f, 4);
	car.chassis_offset.Set(0, 1.5, 0);
	car.mass = 350.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 1000; //friction value changed
	car.maxSuspensionForce = 10000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.5f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;



	vehicle = App->physics->AddVehicle(car);
	vehicle->isPlayer = true;
	vehicle->SetPos(0, 22, -5);
	vehicle->collision_listeners.add(this);

	cd = 0.47f;

	life = App->audio->LoadFx("Assets/life.wav");
	lose = App->audio->LoadFx("Assets/Lose.wav");
	win = App->audio->LoadFx("Assets/Win.wav");
	check = App->audio->LoadFx("Assets/check.wav");
	

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
		LOG("%d count, %d total", spawnCount, App->scene_intro->checkPoints.Count());
		if(spawnCount >= App->scene_intro->checkPoints.Count())
		{
			hasWon = true;
		}
		else 
		{
			hasWon = false;
		}

		if (hasWon == true) {
			lifes = 3;
			life_ready = false;
			life_cd = 20;
			
			App->audio->PlayFx(win);

			turn = acceleration = brake = 0.0f;

			btTransform tr;
			tr.setIdentity();
			btQuaternion quat;
			quat.setEulerZYX(0, 0, 0);
			tr.setRotation(quat);
			tr.setOrigin(btVector3(0, 22, -5));

			vehicle->vehicle->getRigidBody()->setCenterOfMassTransform(tr);
			turn = acceleration = brake = 0.0f;
			vehicle->vehicle->getRigidBody()->setLinearVelocity(btVector3(0, 0, 0));
			vehicle->vehicle->getRigidBody()->setAngularVelocity(btVector3(0, 0, 0));

			for (size_t i = 0; i < App->scene_intro->checkPoints.Count(); i++)
			{
				App->scene_intro->checkPoints[i].color.r = 1;
				App->scene_intro->checkPoints[i].color.g = 0;
				App->scene_intro->checkPoints[i].color.b = 0;
			}

			spawnCount = -1;
			hasWon = false;
		}

		if (hasStart == false) {
			for (size_t i = 0; i < App->scene_intro->cubes.Count(); i++)
			{
				int space = 0;
				int num = 0;
				if (App->scene_intro->cubes[i].physObject->isLife == true) {

					//App->scene_intro->cubes[i].physObject->SetPos(vehicle->vehicle->getChassisWorldTransform().getOrigin().getX() + 1.5f - 1.5f * num, vehicle->vehicle->getChassisWorldTransform().getOrigin().getY() + 3, vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ());
					//App->scene_intro->cubes[i].SetPos(vehicle->vehicle->getChassisWorldTransform().getOrigin().getX() + 1.5f - 1.5f * num, vehicle->vehicle->getChassisWorldTransform().getOrigin().getY() + 3, vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ());

					lifeList.Insert(&App->scene_intro->cubes[i], lifeList.Count());
					App->scene_intro->cubes[i].physObject->SetAsSensor(true);
				}
			}
			hasStart = true;
		}
		else {
			int num = 0;
			for (size_t i = 0; i < lifeList.Count(); i++)
			{
				lifeList[i]->physObject->SetPos(0, 22, -5);
				lifeList[i]->SetPos(0, 22, -5);

				if (lifes > i) {
					//if (App->scene_intro->cubes[i].physObject->isLife == true) {
					//lifeList[i]->color.a = 0.5f;
					lifeList[i]->physObject->SetPos(vehicle->vehicle->getChassisWorldTransform().getOrigin().getX(), vehicle->vehicle->getChassisWorldTransform().getOrigin().getY() + 3 + (1.5 * num), vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ());
					lifeList[i]->SetPos(vehicle->vehicle->getChassisWorldTransform().getOrigin().getX(), vehicle->vehicle->getChassisWorldTransform().getOrigin().getY() + 3 + (1.5 * num), vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ());
					//lifeList.At(i)->Render();
					//cubes.At(i)->Render();
					num++;
					//}
				}
				
			}
		}
		
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			cd = 1000; //para comprobar que funciona el drag aereo, esto equivale a mucho viento en contra. 
		}

		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		{
			cd = 0.47f;
		}

	turn = acceleration = brake = 0.0f;
	vx = vehicle->GetKmh() * 3600 / 1000;
	surface = (vehicle->info.chassis_size.y * 0.5) * (vehicle->info.chassis_size.x * 0.5);
	Fdx = vx * vx * surface * cd;

	a= Fdx / vehicle->info.mass;

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		acceleration = MAX_ACCELERATION-a;
	}

	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if(turn < TURN_DEGREES)
			turn +=  TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if(turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		//brake = BRAKE_POWER;
		acceleration = -MAX_ACCELERATION * 0.5;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		brake = BRAKE_POWER;
	}

	
	
	

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);
	mat4x4 rot;
	rot.rotate(0,(0,0,0));
	
	vehicle->Render();

	char title[80];
	sprintf_s(title, "%.1f Km/h", vehicle->GetKmh());
	App->window->SetTitle(title);

	vec3 pos = vec3(vehicle->vehicle->getRigidBody()->getCenterOfMassTransform().getOrigin().getX(), vehicle->vehicle->getRigidBody()->getCenterOfMassTransform().getOrigin().getY(), vehicle->vehicle->getRigidBody()->getCenterOfMassTransform().getOrigin().getZ());

	if (!life_ready)
	{
		life_cd--;
		if (life_cd <= 0)
		{
			life_ready = true;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
	{
		turn = acceleration = brake = 0.0f;

		btTransform tr;
		tr.setIdentity();
		btQuaternion quat;
		quat.setEulerZYX(0, spawnRotY, 0);
		LOG("%f", spawnRotY);
		tr.setRotation(quat);
		tr.setOrigin(btVector3(spawnPos.x, spawnPos.y, spawnPos.z));

		vehicle->vehicle->getRigidBody()->setCenterOfMassTransform(tr);
		turn = acceleration = brake = 0.0f;
		vehicle->vehicle->getRigidBody()->setLinearVelocity(btVector3(0, 0, 0));
		vehicle->vehicle->getRigidBody()->setAngularVelocity(btVector3(0, 0, 0));
	}
	
	if (in_lava)
	{
		turn = acceleration = brake = 0.0f;

		btTransform tr;
		tr.setIdentity();
		btQuaternion quat;
		quat.setEulerZYX(0, spawnRotY, 0);
		tr.setRotation(quat);
		tr.setOrigin(btVector3(spawnPos.x, spawnPos.y, spawnPos.z));

		vehicle->vehicle->getRigidBody()->setCenterOfMassTransform(tr);
		turn = acceleration = brake = 0.0f;
		vehicle->vehicle->getRigidBody()->setLinearVelocity(btVector3(0, 0, 0));
		vehicle->vehicle->getRigidBody()->setAngularVelocity(btVector3(0, 0, 0));

		in_lava = false;
	}

	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody3D* body1, PhysBody3D* body2) {

	if (body1->isPlayer == true && body2->isCheckPoint == true) {
		for (size_t i = 0; i < App->scene_intro->checkPoints.Count(); i++)
		{
			if (body2 == App->scene_intro->checkPoints[i].physObject) {
				if (App->scene_intro->checkPoints[i].color.r == 1 &&
					App->scene_intro->checkPoints[i].color.g == 0 &&
					App->scene_intro->checkPoints[i].color.b == 0) {

					if (i != 0)
					{
						App->scene_intro->checkPoints[i].color.r = 0;
						App->scene_intro->checkPoints[i].color.g = 1;
						App->scene_intro->checkPoints[i].color.b = 0;
					}
					
					spawnCount++;

					App->audio->PlayFx(check);
				}

				spawnPos.x = App->scene_intro->checkPoints[i].transform[12];
				spawnPos.y = App->scene_intro->checkPoints[i].transform[13];
				spawnPos.z = App->scene_intro->checkPoints[i].transform[14];

				spawnRotY = App->scene_intro->checkPoints[i].rotY;
				
			}
		}
		
	}

	

	/*if (body1->isPlayer == true && body2->isWin == true) {
		for (size_t i = 0; i < App->scene_intro->checkPoints.Count(); i++)
		{
			if (body2 == App->scene_intro->checkPoints[i].physObject) {
				if (App->scene_intro->checkPoints[i].color.r == 1 &&
					App->scene_intro->checkPoints[i].color.g == 0 &&
					App->scene_intro->checkPoints[i].color.b == 0) {

					App->scene_intro->checkPoints[i].color.r = 1;
					App->scene_intro->checkPoints[i].color.g = 0.7;
					App->scene_intro->checkPoints[i].color.b = 0;
				}
				
				spawnPos.x = App->scene_intro->checkPoints[i].transform[12];
				spawnPos.y = App->scene_intro->checkPoints[i].transform[13];
				spawnPos.z = App->scene_intro->checkPoints[i].transform[14];
			}
		}

	}*/

	if (body1->isPlayer == true && body2->isDeath == true)
	{
		if (lifes > 1)
		{
			if (life_ready)
			{
				lifes--;
				life_ready = false;
				life_cd = 20;

				App->audio->PlayFx(life);
			}
		}
		else
		{
			
			if (life_ready)
			{
				lifes = 3;
				life_ready = false;
				life_cd = 20;

				hasLose = true;
				spawnCount = 0;
				App->audio->PlayFx(lose);
			}
			
		}

		in_lava = true;
		

		if (hasWon == false) {
			if (hasLose)
			{
				for (size_t i = 0; i < App->scene_intro->checkPoints.Count(); i++)
				{
					App->scene_intro->checkPoints[i].color.r = 1;
					App->scene_intro->checkPoints[i].color.g = 0;
					App->scene_intro->checkPoints[i].color.b = 0;
				}

				spawnPos.x = 0;
				spawnPos.y = 22;
				spawnPos.z = -5;
				spawnRotY = 0;

				hasLose = false;
			}
		}
	}

}


