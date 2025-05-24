#include "RawBulletSim.h"
#include <btBulletDynamicsCommon.h>
#include <iostream>
#include <unordered_map>
#include <vector>

static constexpr int START_LIFE_TIME = 100;
static constexpr size_t MAX_ALIVE_MOVING_BODIES = 1000;
static size_t aliveMovingBodies = 0;
static std::unordered_map<btRigidBody*, int> bodiesLifeTimes;
static btDiscreteDynamicsWorld* world = nullptr;
btDefaultCollisionConfiguration* collisionConfiguration = nullptr;
btCollisionDispatcher* dispatcher = nullptr;
btBroadphaseInterface* overlappingPairCache = nullptr;
btSequentialImpulseConstraintSolver* solver = nullptr;
btBoxShape* floorShape = nullptr;
btSphereShape* sphereShape = nullptr;

static void createFloor();
static void spawnOneMovingBody(btVector3 spawnPosition);
static int spawnMovingBodies(int maxSpawns = -1);

static void processBodiesWithLifeTimes(std::vector<btRigidBody*>& bodiesToDestroy);
static int destroyBodiesWithEndedLifeTimes(std::vector<btRigidBody*>& bodiesToDestroy);

static void logStepInfo(int step, int spawned, int destroyed);

void RawBulletSim::initialize()
{
	//world and bullet objects creation and init
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;
	world = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	world->setGravity(btVector3(0, -10, 0));

	floorShape = new btBoxShape({ 50, 1, 50 });
	sphereShape = new btSphereShape(1);
	
	//create floor and initial spawning
	createFloor();


	spawnMovingBodies(500);
}

void RawBulletSim::run(size_t steps)
{
	
	//loop
	for (size_t i = 0; i < steps; i++)
	{
		//1. spawn new bodies
		int spawned = spawnMovingBodies(15);
		//2. update physics simulation
		world->stepSimulation(1.0f / 60.0f, 10);
		//3. update lifetimes and destroy dead bodies
		std::vector<btRigidBody*> bodiesToDestroy;
		processBodiesWithLifeTimes( bodiesToDestroy);
		int destroyed = destroyBodiesWithEndedLifeTimes(bodiesToDestroy);
		//4. log
		logStepInfo(i, spawned, destroyed);
	}
}

void RawBulletSim::dispose()
{
	for (int i = world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = world->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		world->removeRigidBody(body);
		delete body;
	}

	delete floorShape;
	delete sphereShape;

	//delete dynamics world
	delete world;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;

	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;


	aliveMovingBodies = 0;
	bodiesLifeTimes.clear();
}

void createFloor()
{
	btTransform tr = btTransform::getIdentity();
	tr.setOrigin({ 0, -1, 0 });
	btDefaultMotionState* motionState = new btDefaultMotionState(tr);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, motionState, floorShape, { 0, 0, 0 });
	rbInfo.m_restitution = 0.5f;
	btRigidBody* body = new btRigidBody(rbInfo);
	world->addRigidBody(body);
}

void spawnOneMovingBody(btVector3 spawnPosition)
{

	aliveMovingBodies++;

	btTransform tr = btTransform::getIdentity();
	tr.setOrigin(spawnPosition);

	float mass = 1;
	btVector3 localInertia;
	sphereShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* motionState = new btDefaultMotionState(tr);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, sphereShape, localInertia);
	rbInfo.m_restitution = 0.75f;
	btRigidBody* body = new btRigidBody(rbInfo);

	bodiesLifeTimes[body] = START_LIFE_TIME;

	world->addRigidBody(body);
}

int spawnMovingBodies(int maxSpawns)
{
	const btVector3 spawnPosition = { 0, 10, 0 };
	int spawned = 0;
	while (aliveMovingBodies < MAX_ALIVE_MOVING_BODIES)
	{
		if (maxSpawns > -1 && spawned >= maxSpawns)
		{
			break;
		}
		spawnOneMovingBody(spawnPosition);
		spawned++;

	}
	return spawned;
}

void processBodiesWithLifeTimes(std::vector<btRigidBody*>& bodiesToDestroy)
{
	for (auto& pair : bodiesLifeTimes)
	{
		pair.second--;
		if (pair.second <= 0)
		{
			bodiesToDestroy.push_back(pair.first);
		}
	}
}

int destroyBodiesWithEndedLifeTimes(std::vector<btRigidBody*>& bodiesToDestroy)
{
	int destroyed = 0;
	while (!bodiesToDestroy.empty())
	{
		btRigidBody* body = bodiesToDestroy.back();
		bodiesToDestroy.pop_back();

		bodiesLifeTimes.erase(body);

		delete body->getMotionState();
		world->removeRigidBody(body);
		delete body;

		destroyed++;
	}
	aliveMovingBodies -= destroyed;
	return destroyed;
}

void logStepInfo(int step, int spawned, int destroyed)
{
	std::cout << "Step " << step << " -----------------------------\n";
	std::cout << "\tSpawned rigid bodies: " << spawned << ".\n";
	std::cout << "\tDestroyed rigid bodies: " << destroyed << ".\n";
	std::cout << "\tAlive rigid bodies: " << aliveMovingBodies << ".\n";
}
