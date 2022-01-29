#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 1000.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 10000.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody3D* body1, PhysBody3D* body2) override;

	btVector3 quatToEuler(btQuaternion quat);

public:

	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;

	double surface;
	double a;
	double vx;
	double Fdx;
	double Fdy;
	double cd = 0.47f;

	vec3 spawnPos = (0, 22, -5);
	int spawnCount = 0;

	bool hasWon = false;
	bool hasLose = false;

	bool in_lava = false;

	int lifes = 3;
	bool life_ready = false;
	int life_cd = 20;

	p2DynArray<Cube*> lifeList;

	bool hasStart = false;
};