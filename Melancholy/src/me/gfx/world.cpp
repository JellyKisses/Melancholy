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

		m_Mesh.draw(delta);
		m_Floor.draw(delta);
		m_Walls.draw(delta);
		
		return true;
	}
	bool World::initialize()
	{
		if (!m_Loaded)
		{
			m_Walls.loadFromFile("data/obj/test.obj");

			m_Mesh.getVertices().push_back(util::Vertex(1.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f, 0.f));
			m_Mesh.getVertices().push_back(util::Vertex(1.f, -1.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f));
			m_Mesh.getVertices().push_back(util::Vertex(-1.f, -1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f));
			m_Mesh.getVertices().push_back(util::Vertex(-1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f));

			m_Mesh.getIndices().push_back(0);
			m_Mesh.getIndices().push_back(1);
			m_Mesh.getIndices().push_back(3);
			m_Mesh.getIndices().push_back(1);
			m_Mesh.getIndices().push_back(2);
			m_Mesh.getIndices().push_back(3);
			
			

			m_Loaded = true;
		}

		return true;
	}
	bool World::isLoaded()
	{
		return m_Loaded;
	}
}
