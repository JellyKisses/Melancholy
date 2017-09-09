#pragma once

#include "../util/util.h"


namespace me::gfx
{
	class World
	{
	private:
		bool m_Loaded;
		GLuint m_VBO_Floor;
		GLuint m_VBO_Wall;
		GLuint m_VAO;

	public:
		World();
		~World();

		bool draw(const glm::float64& delta);
		bool initialize();
		bool isLoaded();

	};
}