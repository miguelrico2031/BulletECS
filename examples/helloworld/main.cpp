#include <BulletECS/PhysicsWorld.h>
#include <iostream>


void main()
{
	BulletECS::PhysicsWorld world({ 0, -9.81f, 0 });

	BulletECS::Entity floor = world.createEntity();
	btTransform floorTransform = btTransform::getIdentity();
	floorTransform.setOrigin({ 0, -0.5f, 0 });
	world.addMotionState(floor, floorTransform);
	world.setBoxCollider(floor, { 50, 1, 50 });
	world.addRigidBody(floor, 0); //0 means static

	BulletECS::Entity sphere = world.createEntity();
	btTransform sphereTransform = btTransform::getIdentity();
	sphereTransform.setOrigin({ 0, 10, 0 });
	world.addMotionState(sphere, sphereTransform);
	world.setSphereCollider(sphere, 0.5f);
	world.addRigidBody(sphere, 2);

	for (size_t i = 0; i < 150; i++)
	{
		world.stepSimulation(1.0f / 60.0f);

		//btRigidBody* sphereRb = world.getRigidBody(sphere);
		btDefaultMotionState* sphereMotion = world.getMotionState(sphere);
		sphereMotion->getWorldTransform(sphereTransform);
		btVector3 position = sphereTransform.getOrigin();
		std::cout << "Sphere pos: " << position.x() << ", " << position.y() << ", " << position.z() << "\n";
	}

	world.destroyEntity(floor);
	std::cout << "--------------------------------------Floor Destroyed--------------------------------------\n";

	for (size_t i = 0; i < 150; i++)
	{
		world.stepSimulation(1.0f / 60.0f);

		//btRigidBody* sphereRb = world.getRigidBody(sphere);
		btDefaultMotionState* sphereMotion = world.getMotionState(sphere);
		sphereMotion->getWorldTransform(sphereTransform);
		btVector3 position = sphereTransform.getOrigin();
		std::cout << "Sphere pos: " << position.x() << ", " << position.y() << ", " << position.z() << "\n";
	}

	world.destroyEntity(sphere);


}