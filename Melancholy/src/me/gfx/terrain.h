#pragma once

#include "../util/util.h"


namespace me::gfx
{
	class Terrain
	{
	private:
		class TerrainCell
		{
		public:
			TerrainCell();
			~TerrainCell();

			const bool initialize();

			void draw(const glm::float64_t& delta);

		public:
			bool m_Initialized;
			GLuint m_Vbo;
			std::vector<util::Vertex> m_Vertices;

		public:

		};

	public:
		Terrain();
		~Terrain();

		void draw(const glm::float64_t& delta);

	private:
		bool m_Initialized;
		GLuint m_Vao;
		std::vector<TerrainCell*> m_TerrainCells;

	};
}