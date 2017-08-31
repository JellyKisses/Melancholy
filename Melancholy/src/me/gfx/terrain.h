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

			const bool initialize(const glm::ivec2 offset, const std::array<glm::float32, 25>& heights);

			void draw(const glm::float64_t& delta);
			const bool getDimensions(float& maxWidth, float& maxHeight, float& maxDepth, float& minWidth, float& minHeight, float& minDepth);

		public:
			bool m_Initialized;
			GLuint m_Vbo;
			std::vector<util::Vertex> m_Vertices;
			glm::float32 m_MaxWidth;
			glm::float32 m_MaxHeight;
			glm::float32 m_MaxDepth;
			glm::float32 m_MinWidth;
			glm::float32 m_MinHeight;
			glm::float32 m_MinDepth;
			glm::float32 m_PositionX;
			glm::float32 m_PositionY;
			glm::float32 m_PositionZ;

		public:

		};
		const bool getTriHeight(const glm::float32 x, const glm::float32 z, glm::float32& height, const glm::vec3 t0, const glm::vec3 t1, const glm::vec3 t2);

	public:
		Terrain();
		~Terrain();

		void draw(const glm::float64_t& delta);
		const glm::uvec2 getSize();
		const bool getHeight(const glm::float32 x, const glm::float32 z, glm::float32& height);

	private:
		bool m_Initialized;
		GLuint m_Vao;
		glm::uvec2 m_Size;
		std::vector<TerrainCell*> m_TerrainCells;

	};
}