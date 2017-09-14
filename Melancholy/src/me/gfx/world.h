#pragma once

#include "../util/util.h"
#include "../core/mesh.h"
#include "../core/model.h"
#include "../core/manager.h"

namespace me::gfx
{
	class World
	{
	private:
		bool m_Loaded;
		core::Model m_Floor;
		core::Model m_Walls;
		core::Mesh  m_Mesh;

	public:
		World();
		~World();

		bool draw(const glm::float64& delta);
		bool initialize();
		bool isLoaded();

	};
}