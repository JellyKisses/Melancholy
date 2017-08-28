#include "terrain.h"


namespace me::gfx
{
	Terrain::TerrainCell::TerrainCell() : m_Initialized(false)
	{
		glGenBuffers(1, &m_Vbo);
	}
	Terrain::TerrainCell::~TerrainCell()
	{

	}
	const bool Terrain::TerrainCell::initialize()
	{
		//if (m_Vertices.empty())
		//{
		//	throw util::RuntimeError(util::RuntimeError::ErrorType::Initialize, "Terrain Cell", "It has no vertices");
		//	return false;
		//}

		glm::vec3 v[25] =
		{
			{ 0.000000f, 0.000000f,  0.000000f },
			{ 0.000000f, 0.000000f,  1.500000f },
			{ 1.500000f, 0.000000f,  0.000000f },
			{ 0.000000f, 0.000000f, -1.500000f },
			{ -1.500000f, 0.000000f,  0.000000f },
			{ -1.500000f, 0.000000f,  1.500000f },
			{ 1.500000f, 0.000000f,  1.500000f },
			{ 1.500000f, 0.000000f, -1.500000f },
			{ -1.500000f, 0.000000f, -1.500000f },
			{ -3.000000f, 0.000000f, -1.500000f },
			{ -3.000000f, 0.000000f,  0.000000f },
			{ -3.000000f, 0.000000f,  1.500000f },
			{ -1.500000f, 0.000000f,  3.000000f },
			{ 0.000000f, 0.000000f,  3.000000f },
			{ 1.500000f, 0.000000f,  3.000000f },
			{ 3.000000f, 0.000000f,  1.500000f },
			{ 3.000000f, 0.000000f,  0.000000f },
			{ 3.000000f, 0.000000f, -1.500000f },
			{ 1.500000f, 0.000000f, -3.000000f },
			{ 0.000000f, 0.000000f, -3.000000f },
			{ -1.500000f, 0.000000f, -3.000000f },
			{ -3.000000f, 0.000000f,  3.000000f },
			{ 3.000000f, 0.000000f,  3.000000f },
			{ -3.000000f, 0.000000f, -3.000000f },
			{ 3.000000f, 0.000000f, -3.000000f }
		};
		glm::vec2 t[25] =
		{
			{ 0.5000f, 0.7500f },
			{ 0.2500f, 0.5000f },
			{ 0.5000f, 0.5000f },
			{ 0.2500f, 0.7500f },
			{ 0.0000f, 0.5000f },
			{ 0.5000f, 1.0000f },
			{ 0.2500f, 1.0000f },
			{ 0.0000f, 0.7500f },
			{ 0.5000f, 0.2500f },
			{ 0.2500f, 0.2500f },
			{ 0.5000f, 0.0000f },
			{ 0.0000f, 0.2500f },
			{ 0.2500f, 0.0000f },
			{ 0.7500f, 0.5000f },
			{ 0.7500f, 0.2500f },
			{ 1.0000f, 0.5000f },
			{ 0.7500f, 0.0000f },
			{ 1.0000f, 0.2500f },
			{ 0.7500f, 0.7500f },
			{ 1.0000f, 0.7500f },
			{ 0.7500f, 1.0000f },
			{ 0.0000f, 1.0000f },
			{ 0.0000f, 0.0000f },
			{ 1.0000f, 0.0000f },
			{ 1.0000f, 1.0000f }
		};
		glm::vec3 n[1] =
		{
			{ 0.0000f, 1.0000f, 0.0000f }
		};
		//m_Vertices =
		//{
		//	util::Vertex(v[5 - 1],t[1 - 1],n[0]),			 util::Vertex(v[2 - 1],t[2 - 1],n[0]),		 util::Vertex(v[1 - 1],t[3 - 1],n[0]),
		//	util::Vertex(v[6 - 1],t[4 - 1],n[0]),			 util::Vertex(v[14 - 1],t[5 - 1],n[0]),		 util::Vertex(v[2 - 1],t[2 - 1],n[0]),
		//	util::Vertex(v[11 - 1],t[6 - 1],n[0]),			 util::Vertex(v[6 - 1],t[4 - 1],n[0]),		 util::Vertex(v[5 - 1],t[1 - 1],n[0]),
		//	util::Vertex(v[12 - 1],t[7 - 1],n[0]),			 util::Vertex(v[13 - 1],t[8 - 1],n[0]),		 util::Vertex(v[6 - 1],t[4 - 1],n[0]),
		//	util::Vertex(v[2 - 1],t[2 - 1],n[0]),			 util::Vertex(v[3 - 1],t[9 - 1],n[0]),		 util::Vertex(v[1 - 1],t[3 - 1],n[0]),
		//	util::Vertex(v[7 - 1],t[10 - 1],n[0]),			 util::Vertex(v[17 - 1],t[11 - 1],n[0]),		 util::Vertex(v[3 - 1],t[9 - 1],n[0]),
		//	util::Vertex(v[14 - 1],t[5 - 1],n[0]),			 util::Vertex(v[7 - 1],t[10 - 1],n[0]),		 util::Vertex(v[2 - 1],t[2 - 1],n[0]),
		//	util::Vertex(v[15 - 1],t[12 - 1],n[0]),			 util::Vertex(v[16 - 1],t[13 - 1],n[0]),		 util::Vertex(v[7 - 1],t[10 - 1],n[0]),
		//	util::Vertex(v[3 - 1],t[9 - 1],n[0]),			 util::Vertex(v[4 - 1],t[14 - 1],n[0]),		 util::Vertex(v[1 - 1],t[3 - 1],n[0]),
		//	util::Vertex(v[8 - 1],t[15 - 1],n[0]),			 util::Vertex(v[20 - 1],t[16 - 1],n[0]),		 util::Vertex(v[4 - 1],t[14 - 1],n[0]),
		//	util::Vertex(v[17 - 1],t[11 - 1],n[0]),			 util::Vertex(v[8 - 1],t[15 - 1],n[0]),		 util::Vertex(v[3 - 1],t[9 - 1],n[0]),
		//	util::Vertex(v[18 - 1],t[17 - 1],n[0]),			 util::Vertex(v[19 - 1],t[18 - 1],n[0]),		 util::Vertex(v[8 - 1],t[15 - 1],n[0]),
		//	util::Vertex(v[4 - 1],t[14 - 1],n[0]),			 util::Vertex(v[5 - 1],t[1 - 1],n[0]),		 util::Vertex(v[1 - 1],t[3 - 1],n[0]),
		//	util::Vertex(v[9 - 1],t[19 - 1],n[0]),			 util::Vertex(v[11 - 1],t[6 - 1],n[0]),		 util::Vertex(v[5 - 1],t[1 - 1],n[0]),
		//	util::Vertex(v[20 - 1],t[16 - 1],n[0]),			 util::Vertex(v[9 - 1],t[19 - 1],n[0]),		 util::Vertex(v[4 - 1],t[14 - 1],n[0]),
		//	util::Vertex(v[21 - 1],t[20 - 1],n[0]),			 util::Vertex(v[10 - 1],t[21 - 1],n[0]),		 util::Vertex(v[9 - 1],t[19 - 1],n[0]),
		//	util::Vertex(v[5 - 1],t[1 - 1],n[0]),			 util::Vertex(v[6 - 1],t[4 - 1],n[0]),		 util::Vertex(v[2 - 1],t[2 - 1],n[0]),
		//	util::Vertex(v[6 - 1],t[4 - 1],n[0]),			 util::Vertex(v[13 - 1],t[8 - 1],n[0]),		 util::Vertex(v[14 - 1],t[5 - 1],n[0]),
		//	util::Vertex(v[11 - 1],t[6 - 1],n[0]),			 util::Vertex(v[12 - 1],t[7 - 1],n[0]),		 util::Vertex(v[6 - 1],t[4 - 1],n[0]),
		//	util::Vertex(v[12 - 1],t[7 - 1],n[0]),			 util::Vertex(v[22 - 1],t[22 - 1],n[0]),		 util::Vertex(v[13 - 1],t[8 - 1],n[0]),
		//	util::Vertex(v[2 - 1],t[2 - 1],n[0]),			 util::Vertex(v[7 - 1],t[10 - 1],n[0]),		 util::Vertex(v[3 - 1],t[9 - 1],n[0]),
		//	util::Vertex(v[7 - 1],t[10 - 1],n[0]),			 util::Vertex(v[16 - 1],t[13 - 1],n[0]),		 util::Vertex(v[17 - 1],t[11 - 1],n[0]),
		//	util::Vertex(v[14 - 1],t[5 - 1],n[0]),			 util::Vertex(v[15 - 1],t[12 - 1],n[0]),		 util::Vertex(v[7 - 1],t[10 - 1],n[0]),
		//	util::Vertex(v[15 - 1],t[12 - 1],n[0]),			 util::Vertex(v[23 - 1],t[23 - 1],n[0]),		 util::Vertex(v[16 - 1],t[13 - 1],n[0]),
		//	util::Vertex(v[3 - 1],t[9 - 1],n[0]),			 util::Vertex(v[8 - 1],t[15 - 1],n[0]),		 util::Vertex(v[4 - 1],t[14 - 1],n[0]),
		//	util::Vertex(v[8 - 1],t[15 - 1],n[0]),			 util::Vertex(v[19 - 1],t[18 - 1],n[0]),		 util::Vertex(v[20 - 1],t[16 - 1],n[0]),
		//	util::Vertex(v[17 - 1],t[11 - 1],n[0]),			 util::Vertex(v[18 - 1],t[17 - 1],n[0]),		 util::Vertex(v[8 - 1],t[15 - 1],n[0]),
		//	util::Vertex(v[18 - 1],t[17 - 1],n[0]),			 util::Vertex(v[25 - 1],t[24 - 1],n[0]),		 util::Vertex(v[19 - 1],t[18 - 1],n[0]),
		//	util::Vertex(v[4 - 1],t[14 - 1],n[0]),			 util::Vertex(v[9 - 1],t[19 - 1],n[0]),		 util::Vertex(v[5 - 1],t[1 - 1],n[0]),
		//	util::Vertex(v[9 - 1],t[19 - 1],n[0]),			 util::Vertex(v[10 - 1],t[21 - 1],n[0]),		 util::Vertex(v[11 - 1],t[6 - 1],n[0]),
		//	util::Vertex(v[20 - 1],t[16 - 1],n[0]),			 util::Vertex(v[21 - 1],t[20 - 1],n[0]),		 util::Vertex(v[9 - 1],t[19 - 1],n[0]),
		//	util::Vertex(v[21 - 1],t[20 - 1],n[0]),			 util::Vertex(v[24 - 1],t[25 - 1],n[0]),		 util::Vertex(v[10 - 1],t[21 - 1],n[0]),
		//};
		m_Vertices = 
		{
			util::Vertex(0.0,0.5,-1.0, 0.5,1.0,0.0,0.0,0.0),
			util::Vertex(-0.5,-0.5,-1.0, 0.0,0.0,0.0,0.0,0.0),
			util::Vertex(0.5,-0.5,-1.0, 1.0,0.0,0.0,0.0,0.0)
		};
		
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(util::Vertex) * m_Vertices.size(), m_Vertices.data(), GL_STATIC_DRAW);

		m_Initialized = true;
		return true;
	}
	void Terrain::TerrainCell::draw(const glm::float64_t& delta)
	{
		if (!m_Initialized) initialize();
		
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
	}


	Terrain::Terrain()
	{
		glGenVertexArrays(1, &m_Vao);
		glBindVertexArray(m_Vao);

		m_TerrainCells.push_back(new TerrainCell());
		m_TerrainCells.at(0)->initialize();

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(util::Vertex), (GLvoid*)(0 * sizeof(glm::float32)));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(util::Vertex), (GLvoid*)(3 * sizeof(glm::float32)));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(util::Vertex), (GLvoid*)(5 * sizeof(glm::float32)));

	}
	Terrain::~Terrain()
	{

	}
	void Terrain::draw(const glm::float64_t& delta)
	{
		glBindVertexArray(m_Vao);

		for (auto& it : m_TerrainCells)
		{
			it->draw(delta);
		}

	}
}