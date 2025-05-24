/*
* This example shows a standard usage of this library for a physics simulation involving lots of rigid bodies,
* taking advantage of the data locality offered by the ECS architecture.
* The examples also uses the library's Component Pool data structure to create a custom Lifetime Component for entities.
* All dynamic rigid bodies have a lifetime componetn in this example, and when its lifetime gets to 0 they are destroyed,
* and a new rigidbody entity is spawned.
*/

#include <chrono>
#include <iostream>	
#include "BulletECSSim.h"
#include "RawBulletSim.h"
class ScopedTimer
{
public:
	ScopedTimer(const std::string& name, long long* output = nullptr)
		: m_name(name), m_startTime(std::chrono::high_resolution_clock::now()), m_output(output) {}

	~ScopedTimer()
	{
		auto endTime = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_startTime).count();
		std::cout << "Timer [" << m_name << "] took " << duration << " milliseconds.\n";
		if (m_output)
		{
			*m_output = duration;
		}
	}

private:
	std::string m_name;
	std::chrono::high_resolution_clock::time_point m_startTime;
	long long* m_output;
};

void main()
{
	size_t steps = 300;
	long long bulletEcsInit;
	long long bulletEcsRun;
	long long bulletEcsDispose;
	long long rawBulletInit;
	long long rawBulletRun;
	long long rawBulletDispose;
	{
		ScopedTimer timer("Raw Bullet init", &rawBulletInit);
		RawBulletSim::initialize();
	}
	{
		ScopedTimer timer("Raw Bullet run", &rawBulletRun);
		RawBulletSim::run(steps);
	}
	{	
		ScopedTimer timer("Raw Bullet dispose", &rawBulletDispose);
		RawBulletSim::dispose();
	}
	{
		ScopedTimer timer("BulletECS init", &bulletEcsInit);
		BulletECSSim::initialize();
	}
	{
		ScopedTimer timer("BulletECS run", &bulletEcsRun);
		BulletECSSim::run(steps);
	}
	{
		ScopedTimer timer("BulletECS dispose", &bulletEcsDispose);
		BulletECSSim::dispose();
	}

	long long bulletEcsTotal = bulletEcsInit + bulletEcsRun + bulletEcsDispose;
	long long rawBulletTotal = rawBulletInit + rawBulletRun + rawBulletDispose;
	std::cout << "------------------------------------------------------------------------------------------------\n";
	std::cout << "------------------------------------------------------------------------------------------------\n";
	std::cout << "------------------------------------------------------------------------------------------------\n";
	std::cout << "Raw Bullet:\n";
	std::cout << "\tInit: " << rawBulletInit << " ms\n";
	std::cout << "\tRun: " << rawBulletRun << " ms\n";
	std::cout << "\tDispose: " << rawBulletDispose << " ms\n";
	std::cout << "\tTOTAL: " << rawBulletTotal << " ms\n";
	std::cout << "BulletECS:\n";
	std::cout << "\tInit: " << bulletEcsInit << " ms\n";
	std::cout << "\tRun: " << bulletEcsRun << " ms\n";
	std::cout << "\tDispose: " << bulletEcsDispose << " ms\n";
	std::cout << "\tTOTAL: " << bulletEcsTotal << " ms\n";
}



