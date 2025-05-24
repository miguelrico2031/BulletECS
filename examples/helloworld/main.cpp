/*
* This is a Hello World program to run a basic Bullet physics simulation.
* This runs exactly the same simulation as the Bullet's HelloWorld example: https://github.com/bulletphysics/bullet3/blob/master/examples/HelloWorld/HelloWorld.cpp
* Bullet is a very OOP library, and this example is very object-oriented. In other examples, a more ECS-oriented usage is shown.
*/
#include <BulletECS/PhysicsWorld.h>
#include <iostream>
void logEntities(const BulletECS::PhysicsWorld& world);
void main()
{
	BulletECS::PhysicsWorld world({ 0, -10, 0 });

	BulletECS::Entity floor = world.createEntity();
	btTransform floorTransform = btTransform::getIdentity();
	floorTransform.setOrigin({ 0, -56, 0 });
	world.addMotionState(floor, floorTransform);
	world.setBoxCollider(floor, { 50, 50, 50 });
	world.addRigidBody(floor, 0); //mass = 0 means static

	BulletECS::Entity sphere = world.createEntity();
	btTransform sphereTransform = btTransform::getIdentity();
	sphereTransform.setOrigin({ 2, 10, 0 });
	world.addMotionState(sphere, sphereTransform);
	world.setSphereCollider(sphere, 1);
	world.addRigidBody(sphere, 1);


	for (size_t i = 0; i < 150; i++)
	{
		world.stepSimulation(1.0f / 60.0f, 10);
		
		logEntities(world);
	}
}
void logEntities(const BulletECS::PhysicsWorld& world)
{
	for (BulletECS::Entity e : world.iterateMotionStates())
	{
		btTransform transform;
		const btDefaultMotionState* motionState = world.getMotionState(e);
		motionState->getWorldTransform(transform);
		btVector3 position = transform.getOrigin();
		std::cout << "world pos object " << e.ID << " = " << position.x() << "," << position.y() << "," << position.z() << "\n";
	}
}