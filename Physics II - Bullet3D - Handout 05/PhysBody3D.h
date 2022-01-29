#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2List.h"
#include "Primitive.h"

class btRigidBody;
class Module;
class Cube;

// =================================================
struct PhysBody3D
{
	friend class ModulePhysics3D;
public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	void SetAsSensor(bool is_sensor);
	void SetId(int id);

private:
	btRigidBody* body = nullptr;

public:
	p2List<Module*> collision_listeners;
	Cube* cubeSkin = nullptr;

	bool isCheckPoint = false;
	bool isPlayer = false;
	bool isDeath = false;
	bool isWin = false;
	bool isLife = false;

	int id;
	bool is_sensor;
};

#endif // __PhysBody3D_H__