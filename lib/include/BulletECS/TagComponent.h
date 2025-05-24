#pragma once
#include <string>
namespace BulletECS
{
	const std::string NO_TAG = "Unnamed-Entity";
	struct TagComponent
	{
		TagComponent(const std::string& n) : name(n) {}
		std::string name;
	};
}