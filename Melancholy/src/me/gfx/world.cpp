#include "world.h"


namespace me::gfx
{
	World::World() : m_Loaded(false)
	{

	}
	World::~World()
	{

	}

	bool World::draw(const glm::float64& delta)
	{
		if (!m_Loaded)
		{
			initialize();
		}

		return true;
	}
	bool World::initialize()
	{
		if (!m_Loaded)
		{


			m_Loaded = true;
		}

		return true;
	}
	bool World::isLoaded()
	{
		return m_Loaded;
	}
}
