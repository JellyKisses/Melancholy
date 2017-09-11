#pragma once

#include "../util/util.h"


namespace me::gfx
{
	class World
	{
	private:
		bool m_Loaded;
		util::VertexArray m_Floor;
		util::VertexArray m_Walls;
		GLuint m_VAO;

	public:
		World();
		~World();

		bool draw(const glm::float64& delta);
		bool initialize();
		bool isLoaded();

	};
}