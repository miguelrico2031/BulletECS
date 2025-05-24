/*
* This example shows a standard usage of this library for a physics simulation involving lots of rigid bodies,
* taking advantage of the data locality offered by the ECS architecture.
* The examples also uses the library's Component Pool data structure to create a custom Lifetime Component for entities.
* All dynamic rigid bodies have a lifetime componetn in this example, and when its lifetime gets to 0 they are destroyed,
* and a new rigidbody entity is spawned.
*/

#include <BulletECS/BulletECS.h>
#include <chrono>
#include <vector>

class ScopedTimer
{
public:
	ScopedTimer(const std::string& name) : m_name(name), m_startTime(std::chrono::high_resolution_clock::now()) {}
	
	~ScopedTimer()
	{
		auto endTime = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - m_startTime).count();
		std::cout << "Timer [" << m_name << "] took " << duration << " microseconds.\n";
	}

private:
	std::string m_name;
	std::chrono::high_resolution_clock::time_point m_startTime;
};

struct LifeTimeComponent
{
	LifeTimeComponent(int stepsAlive) : remainingSteps(stepsAlive) {}
	int remainingSteps;
};

struct ExtendedWorld
{
	BulletECS::PhysicsWorld physicsWorld = BulletECS::PhysicsWorld({ 0, -10, 0 });
	BulletECS::ComponentPool<LifeTimeComponent> lifetimeComponentPool = {};
	size_t aliveMovingEntities = 0;
	size_t maxAliveMovingEntities = 1000;
	int movingEntitiesStepsAlive = 100;
	
	// you can do something like for(Entity& e : extendedWorld.lifeTimeComponentPool) {...} but it is weird
	// so this method makes it clearer and follows the PhysicsWorld.iterate[Mutable]EntitiesWith...() syntax
	BulletECS::ComponentPool<LifeTimeComponent>& iterateMutableEntitiesWithLifeTimes()
	{
		return lifetimeComponentPool;
	}
	const BulletECS::ComponentPool<LifeTimeComponent>& iterateEntitiesWithLifeTimes() const
	{
		return lifetimeComponentPool;
	}
};

BulletECS::Entity createFloor(ExtendedWorld& world);
BulletECS::Entity spawnMovingEntity(ExtendedWorld& world, btVector3 spawnPositon);

//Spawn system
int spawnMovingEntities(ExtendedWorld& world, int maxSpawns = -1);

//Lifetime System
void processEntitiesWithLifeTimes(ExtendedWorld& world, std::vector<BulletECS::Entity>& entitiesToDestroy);
int destroyEntitiesWithEndedLifeTimes(ExtendedWorld& world, std::vector<BulletECS::Entity>& entitiesToDestroy);

//Logging alive rigid bodies system
void logStepInfo(const ExtendedWorld& world, int step, int spawned, int destroyed);



void main()
{
	ScopedTimer timer("BulletECS_Simulation");
	ExtendedWorld world;
	
	createFloor(world);

	spawnMovingEntities(world, 500);
	//loop
	for (size_t i = 0; i < 500; i++)
	{
		//1. spawn new entities
		int spawned = spawnMovingEntities(world, 15);
		//2. update physics simulation
		world.physicsWorld.stepSimulation(1.0f / 60.0f, 10);
		//3. update lifetimes and destroy dead entities
		std::vector<BulletECS::Entity> entitiesToDestroy;
		processEntitiesWithLifeTimes(world, entitiesToDestroy);
		int destroyed = destroyEntitiesWithEndedLifeTimes(world, entitiesToDestroy);
		//4. log
		logStepInfo(world, i, spawned, destroyed);
	}
}

BulletECS::Entity createFloor(ExtendedWorld& world)
{
	BulletECS::PhysicsWorld& pw = world.physicsWorld;
	BulletECS::Entity floor = pw.createEntity();
	btTransform floorTransform = btTransform::getIdentity();
	floorTransform.setOrigin({ 0, -1, 0 });
	pw.addMotionState(floor, floorTransform);
	pw.setBoxCollider(floor, { 50, 1, 50 });
	pw.addRigidBody(floor, 0, 0.5f);
	pw.addTag(floor, "floor");
	return floor;
}

BulletECS::Entity spawnMovingEntity(ExtendedWorld& world, btVector3 spawnPositon)
{
	static size_t id = 0;
	world.aliveMovingEntities++;
	BulletECS::PhysicsWorld& pw = world.physicsWorld;
	BulletECS::Entity sphere = pw.createEntity();
	btTransform sphereTransform = btTransform::getIdentity();
	sphereTransform.setOrigin(spawnPositon);
	pw.addMotionState(sphere, sphereTransform);
	pw.setSphereCollider(sphere, 1);
	pw.addRigidBody(sphere, 1, 0.75f);
	pw.addTag(sphere, "sphere_" + std::to_string(id++));
	world.lifetimeComponentPool.add(sphere, world.movingEntitiesStepsAlive);
	return sphere;
}

int spawnMovingEntities(ExtendedWorld& world, int maxSpawns)
{
	const btVector3 spawnPosition = { 0, 10, 0 };
	int spawned = 0;
	while (world.aliveMovingEntities < world.maxAliveMovingEntities)
	{
		if (maxSpawns > -1 && spawned >= maxSpawns)
		{
			break;
		}

		spawnMovingEntity(world, spawnPosition);
		spawned++;
	}
	
	return spawned;
}

void processEntitiesWithLifeTimes(ExtendedWorld& world, std::vector<BulletECS::Entity>& entitiesToDestroy)
{
	// update each entity's lifetime and add the dead ones (0 steps left) to the dead vector
	for (BulletECS::Entity& e : world.iterateMutableEntitiesWithLifeTimes())
	{
		LifeTimeComponent* lifeTime = world.lifetimeComponentPool.get(e);
		lifeTime->remainingSteps--;
		if (lifeTime->remainingSteps <= 0)
		{
			entitiesToDestroy.push_back(e);
		}
	}
}

int destroyEntitiesWithEndedLifeTimes(ExtendedWorld& world, std::vector<BulletECS::Entity>& entitiesToDestroy)
{
	// destroy all entities in the dead vector
	int destroyed = 0;
	while (!entitiesToDestroy.empty())
	{
		BulletECS::Entity e = entitiesToDestroy.back();
		entitiesToDestroy.pop_back();

		world.lifetimeComponentPool.remove(e);
		world.physicsWorld.destroyEntity(e);
		destroyed++;
	}
	world.aliveMovingEntities -= destroyed;
	return destroyed;
}

void logStepInfo(const ExtendedWorld& world, int step, int spawned, int destroyed)
{
	std::cout << "Step " << step << " -----------------------------\n";
	std::cout << "\tSpawned entities: " << spawned << ".\n";
	std::cout << "\tDestroyed entities: " << destroyed << ".\n";
	std::cout << "\tAlive entities: " << world.aliveMovingEntities << ".\n";

	const BulletECS::PhysicsWorld& pw = world.physicsWorld;
	//in this example all rigidbody entities have tags so we're not checking that
	//also, in the library, all rigid bodies must have motion states
	//note that we're not modifying any components' data, so we use the const iterator getter and a const ref to the world
	/*for (BulletECS::Entity& e : world.iterateEntitiesWithLifeTimes())
	{
		const std::string& tag = pw.getTag(e);

		btTransform transform;
		const btDefaultMotionState* motionState = pw.getMotionState(e);
		motionState->getWorldTransform(transform);
		btVector3 position = transform.getOrigin();

		std::cout << "\tMoving entity [" << tag << "] position: " << position.x() << ',' << position.y() << ',' << position.z() << '\n';
	}*/
}
