#include "terrain.h"
#include "../core/image.h"


namespace me::gfx
{
	Terrain::TerrainCell::TerrainCell() : m_Initialized(false)
	{
		glGenBuffers(1, &m_Vbo);
	}
	Terrain::TerrainCell::~TerrainCell()
	{

	}
	const bool Terrain::TerrainCell::initialize(const glm::ivec2 position, const std::array<glm::float32, 25>& heights)
	{
		if (heights.empty())
		{
			throw util::RuntimeError(util::RuntimeError::ErrorType::Initialize, "Terrain Cell", "It has no vertices");
			return false;
		}

		glm::vec3 v[25] =
		{
			{  0.000000f * (position.x * 2.f), heights.at(12),  0.000000f * (position.y * 2.f)},
			{  0.000000f * (position.x * 2.f), heights.at(17),  0.500000f * (position.y * 2.f)},
			{  0.500000f * (position.x * 2.f), heights.at(13),  0.000000f * (position.y * 2.f)},
			{  0.000000f * (position.x * 2.f), heights.at( 7), -0.500000f * (position.y * 2.f)},
			{ -0.500000f * (position.x * 2.f), heights.at(11),  0.000000f * (position.y * 2.f)},
			{ -0.500000f * (position.x * 2.f), heights.at(16),  0.500000f * (position.y * 2.f)},
			{  0.500000f * (position.x * 2.f), heights.at(18),  0.500000f * (position.y * 2.f)},
			{  0.500000f * (position.x * 2.f), heights.at( 8), -0.500000f * (position.y * 2.f)},
			{ -0.500000f * (position.x * 2.f), heights.at( 6), -0.500000f * (position.y * 2.f)},
			{ -1.000000f * (position.x * 2.f), heights.at( 5), -0.500000f * (position.y * 2.f)},
			{ -1.000000f * (position.x * 2.f), heights.at(10),  0.000000f * (position.y * 2.f)},
			{ -1.000000f * (position.x * 2.f), heights.at(15),  0.500000f * (position.y * 2.f)},
			{ -0.500000f * (position.x * 2.f), heights.at(21),  1.000000f * (position.y * 2.f)},
			{  0.000000f * (position.x * 2.f), heights.at(22),  1.000000f * (position.y * 2.f)},
			{  0.500000f * (position.x * 2.f), heights.at(23),  1.000000f * (position.y * 2.f)},
			{  1.000000f * (position.x * 2.f), heights.at(19),  0.500000f * (position.y * 2.f)},
			{  1.000000f * (position.x * 2.f), heights.at(14),  0.000000f * (position.y * 2.f)},
			{  1.000000f * (position.x * 2.f), heights.at(19), -0.500000f * (position.y * 2.f)},
			{  0.500000f * (position.x * 2.f), heights.at( 3), -1.000000f * (position.y * 2.f)},
			{  0.000000f * (position.x * 2.f), heights.at(10), -1.000000f * (position.y * 2.f)},
			{ -0.500000f * (position.x * 2.f), heights.at( 1), -1.000000f * (position.y * 2.f)},
			{ -1.000000f * (position.x * 2.f), heights.at(20),  1.000000f * (position.y * 2.f)},
			{  1.000000f * (position.x * 2.f), heights.at(24),  1.000000f * (position.y * 2.f)},
			{ -1.000000f * (position.x * 2.f), heights.at( 0), -1.000000f * (position.y * 2.f)},
			{  1.000000f * (position.x * 2.f), heights.at( 4), -1.000000f * (position.y * 2.f)}
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
		m_Vertices =
		{
			util::Vertex(v[5 - 1],t[1 - 1],n[0]),			 util::Vertex(v[2 - 1],t[2 - 1],n[0]),		 util::Vertex(v[1 - 1],t[3 - 1],n[0]),
			util::Vertex(v[6 - 1],t[4 - 1],n[0]),			 util::Vertex(v[14 - 1],t[5 - 1],n[0]),		 util::Vertex(v[2 - 1],t[2 - 1],n[0]),
			util::Vertex(v[11 - 1],t[6 - 1],n[0]),			 util::Vertex(v[6 - 1],t[4 - 1],n[0]),		 util::Vertex(v[5 - 1],t[1 - 1],n[0]),
			util::Vertex(v[12 - 1],t[7 - 1],n[0]),			 util::Vertex(v[13 - 1],t[8 - 1],n[0]),		 util::Vertex(v[6 - 1],t[4 - 1],n[0]),
			util::Vertex(v[2 - 1],t[2 - 1],n[0]),			 util::Vertex(v[3 - 1],t[9 - 1],n[0]),		 util::Vertex(v[1 - 1],t[3 - 1],n[0]),
			util::Vertex(v[7 - 1],t[10 - 1],n[0]),			 util::Vertex(v[17 - 1],t[11 - 1],n[0]),		 util::Vertex(v[3 - 1],t[9 - 1],n[0]),
			util::Vertex(v[14 - 1],t[5 - 1],n[0]),			 util::Vertex(v[7 - 1],t[10 - 1],n[0]),		 util::Vertex(v[2 - 1],t[2 - 1],n[0]),
			util::Vertex(v[15 - 1],t[12 - 1],n[0]),			 util::Vertex(v[16 - 1],t[13 - 1],n[0]),		 util::Vertex(v[7 - 1],t[10 - 1],n[0]),
			util::Vertex(v[3 - 1],t[9 - 1],n[0]),			 util::Vertex(v[4 - 1],t[14 - 1],n[0]),		 util::Vertex(v[1 - 1],t[3 - 1],n[0]),
			util::Vertex(v[8 - 1],t[15 - 1],n[0]),			 util::Vertex(v[20 - 1],t[16 - 1],n[0]),		 util::Vertex(v[4 - 1],t[14 - 1],n[0]),
			util::Vertex(v[17 - 1],t[11 - 1],n[0]),			 util::Vertex(v[8 - 1],t[15 - 1],n[0]),		 util::Vertex(v[3 - 1],t[9 - 1],n[0]),
			util::Vertex(v[18 - 1],t[17 - 1],n[0]),			 util::Vertex(v[19 - 1],t[18 - 1],n[0]),		 util::Vertex(v[8 - 1],t[15 - 1],n[0]),
			util::Vertex(v[4 - 1],t[14 - 1],n[0]),			 util::Vertex(v[5 - 1],t[1 - 1],n[0]),		 util::Vertex(v[1 - 1],t[3 - 1],n[0]),
			util::Vertex(v[9 - 1],t[19 - 1],n[0]),			 util::Vertex(v[11 - 1],t[6 - 1],n[0]),		 util::Vertex(v[5 - 1],t[1 - 1],n[0]),
			util::Vertex(v[20 - 1],t[16 - 1],n[0]),			 util::Vertex(v[9 - 1],t[19 - 1],n[0]),		 util::Vertex(v[4 - 1],t[14 - 1],n[0]),
			util::Vertex(v[21 - 1],t[20 - 1],n[0]),			 util::Vertex(v[10 - 1],t[21 - 1],n[0]),		 util::Vertex(v[9 - 1],t[19 - 1],n[0]),
			util::Vertex(v[5 - 1],t[1 - 1],n[0]),			 util::Vertex(v[6 - 1],t[4 - 1],n[0]),		 util::Vertex(v[2 - 1],t[2 - 1],n[0]),
			util::Vertex(v[6 - 1],t[4 - 1],n[0]),			 util::Vertex(v[13 - 1],t[8 - 1],n[0]),		 util::Vertex(v[14 - 1],t[5 - 1],n[0]),
			util::Vertex(v[11 - 1],t[6 - 1],n[0]),			 util::Vertex(v[12 - 1],t[7 - 1],n[0]),		 util::Vertex(v[6 - 1],t[4 - 1],n[0]),
			util::Vertex(v[12 - 1],t[7 - 1],n[0]),			 util::Vertex(v[22 - 1],t[22 - 1],n[0]),		 util::Vertex(v[13 - 1],t[8 - 1],n[0]),
			util::Vertex(v[2 - 1],t[2 - 1],n[0]),			 util::Vertex(v[7 - 1],t[10 - 1],n[0]),		 util::Vertex(v[3 - 1],t[9 - 1],n[0]),
			util::Vertex(v[7 - 1],t[10 - 1],n[0]),			 util::Vertex(v[16 - 1],t[13 - 1],n[0]),		 util::Vertex(v[17 - 1],t[11 - 1],n[0]),
			util::Vertex(v[14 - 1],t[5 - 1],n[0]),			 util::Vertex(v[15 - 1],t[12 - 1],n[0]),		 util::Vertex(v[7 - 1],t[10 - 1],n[0]),
			util::Vertex(v[15 - 1],t[12 - 1],n[0]),			 util::Vertex(v[23 - 1],t[23 - 1],n[0]),		 util::Vertex(v[16 - 1],t[13 - 1],n[0]),
			util::Vertex(v[3 - 1],t[9 - 1],n[0]),			 util::Vertex(v[8 - 1],t[15 - 1],n[0]),		 util::Vertex(v[4 - 1],t[14 - 1],n[0]),
			util::Vertex(v[8 - 1],t[15 - 1],n[0]),			 util::Vertex(v[19 - 1],t[18 - 1],n[0]),		 util::Vertex(v[20 - 1],t[16 - 1],n[0]),
			util::Vertex(v[17 - 1],t[11 - 1],n[0]),			 util::Vertex(v[18 - 1],t[17 - 1],n[0]),		 util::Vertex(v[8 - 1],t[15 - 1],n[0]),
			util::Vertex(v[18 - 1],t[17 - 1],n[0]),			 util::Vertex(v[25 - 1],t[24 - 1],n[0]),		 util::Vertex(v[19 - 1],t[18 - 1],n[0]),
			util::Vertex(v[4 - 1],t[14 - 1],n[0]),			 util::Vertex(v[9 - 1],t[19 - 1],n[0]),		 util::Vertex(v[5 - 1],t[1 - 1],n[0]),
			util::Vertex(v[9 - 1],t[19 - 1],n[0]),			 util::Vertex(v[10 - 1],t[21 - 1],n[0]),		 util::Vertex(v[11 - 1],t[6 - 1],n[0]),
			util::Vertex(v[20 - 1],t[16 - 1],n[0]),			 util::Vertex(v[21 - 1],t[20 - 1],n[0]),		 util::Vertex(v[9 - 1],t[19 - 1],n[0]),
			util::Vertex(v[21 - 1],t[20 - 1],n[0]),			 util::Vertex(v[24 - 1],t[25 - 1],n[0]),		 util::Vertex(v[10 - 1],t[21 - 1],n[0]),
		};

		m_MaxWidth = m_MaxHeight = m_MaxDepth = -1000000.f;
		m_MinWidth = m_MinHeight = m_MinDepth =  1000000.f;

		for (int i = 0; i < m_Vertices.size(); i++)
		{
			glm::float32 width  = m_Vertices.at(i).m_Position.x;
			glm::float32 height = m_Vertices.at(i).m_Position.y;
			glm::float32 depth  = m_Vertices.at(i).m_Position.z;

			if (width > m_MaxWidth)
			{
				m_MaxWidth = width;
			}
			if (width < m_MinWidth)
			{
				m_MinWidth = width;
			}
			if (height > m_MaxHeight)
			{
				m_MaxHeight = height;
			}
			if (height < m_MinHeight)
			{
				m_MinHeight = height;
			}
			if (depth > m_MaxDepth)
			{
				m_MaxDepth = depth;
			}
			if (depth < m_MinDepth)
			{
				m_MinDepth = depth;
			}
		}

		m_PositionX = (m_MaxWidth - m_MinWidth) + m_MinWidth;
		m_PositionY = (m_MaxHeight - m_MinHeight) + m_MinHeight;
		m_PositionZ = (m_MaxDepth - m_MinDepth) + m_MinDepth;

		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(util::Vertex) * m_Vertices.size(), m_Vertices.data(), GL_STATIC_DRAW);

		m_Initialized = true;
		return true;
	}
	void Terrain::TerrainCell::draw(const glm::float64_t& delta)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
	}
	const bool Terrain::TerrainCell::getDimensions(float& maxWidth, float& maxHeight, float& maxDepth, float& minWidth, float& minHeight, float& minDepth)
	{
		maxWidth  = m_MaxWidth;
		maxHeight = m_MaxHeight;
		maxDepth  = m_MaxDepth;
		minWidth  = m_MinWidth;
		minWidth  = m_MinHeight;
		minDepth  = m_MinDepth;
		return true;
	}

	Terrain::Terrain()
	{
		glGenVertexArrays(1, &m_Vao);
		glBindVertexArray(m_Vao);

		//CHANGE THIS!
		m_Size = glm::uvec2(4, 4);

		core::Image height = util::perlin(4.0f, 0.5f, m_Size.x * 5, m_Size.y * 5);

		glm::uint32 i = 0;
		for (int x = 0; x < m_Size.x; ++x)
		{
			for (int y = 0; y < m_Size.y; ++y)
			{
				m_TerrainCells.push_back(new TerrainCell());
				m_TerrainCells.at(i)->initialize
				(glm::ivec2(x, y), {
					static_cast<glm::float32>(height.getPixel(x + 0,y + 0).r)/32.f,static_cast<glm::float32>(height.getPixel(x + 1,y + 0).r)/32.f,static_cast<glm::float32>(height.getPixel(x + 2,y + 0).r/32.f),static_cast<glm::float32>(height.getPixel(x + 3,y + 0).r/32.f),static_cast<glm::float32>(height.getPixel(x + 4,y + 0).r/32.f),
					static_cast<glm::float32>(height.getPixel(x + 0,y + 1).r)/32.f,static_cast<glm::float32>(height.getPixel(x + 1,y + 1).r)/32.f,static_cast<glm::float32>(height.getPixel(x + 2,y + 1).r/32.f),static_cast<glm::float32>(height.getPixel(x + 3,y + 1).r/32.f),static_cast<glm::float32>(height.getPixel(x + 4,y + 1).r/32.f),
					static_cast<glm::float32>(height.getPixel(x + 0,y + 2).r)/32.f,static_cast<glm::float32>(height.getPixel(x + 1,y + 2).r)/32.f,static_cast<glm::float32>(height.getPixel(x + 2,y + 2).r/32.f),static_cast<glm::float32>(height.getPixel(x + 3,y + 2).r/32.f),static_cast<glm::float32>(height.getPixel(x + 4,y + 2).r/32.f),
					static_cast<glm::float32>(height.getPixel(x + 0,y + 3).r)/32.f,static_cast<glm::float32>(height.getPixel(x + 1,y + 3).r)/32.f,static_cast<glm::float32>(height.getPixel(x + 2,y + 3).r/32.f),static_cast<glm::float32>(height.getPixel(x + 3,y + 3).r/32.f),static_cast<glm::float32>(height.getPixel(x + 4,y + 3).r/32.f),
					static_cast<glm::float32>(height.getPixel(x + 0,y + 4).r)/32.f,static_cast<glm::float32>(height.getPixel(x + 1,y + 4).r)/32.f,static_cast<glm::float32>(height.getPixel(x + 2,y + 4).r/32.f),static_cast<glm::float32>(height.getPixel(x + 3,y + 4).r/32.f),static_cast<glm::float32>(height.getPixel(x + 4,y + 4).r/32.f)
				});
				++i;
			}
		}

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
	const glm::uvec2 Terrain::getSize()
	{
		return m_Size;
	}
	const bool Terrain::getTriHeight(const glm::float32 x, const glm::float32 z, glm::float32& height, const glm::vec3 t0, const glm::vec3 t1, const glm::vec3 t2)
	{
		glm::vec3 startVector, directionVector, edge1, edge2, normal;
		glm::vec3 Q, e1, e2, e3, edgeNormal, temp;
		float magnitude, D, denominator, numerator, t, determinant;


		// Starting position of the ray that is being cast.
		startVector = glm::vec3(x, 0.f, z);

		// The direction the ray is being cast.
		directionVector = glm::vec3(0.f, -1.f, 0.f);

		// Calculate the two edges from the three points given.
		edge1 = t1 - t0;
		edge2 = t2 - t0;

		// Calculate the normal of the triangle from the two edges.
		normal.x = (edge1.y * edge2.z) - (edge1.z * edge2.y);
		normal.y = (edge1.z * edge2.x) - (edge1.x * edge2.z);
		normal.z = (edge1.x * edge2.y) - (edge1.y * edge2.x);

		magnitude = glm::sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));
		normal = normal / magnitude;

		// Find the distance from the origin to the plane.
		D = ((-normal.x * t0.x) + (-normal.y * t0.y) + (-normal.z * t0.z));

		// Get the denominator of the equation.
		denominator = ((normal.x * directionVector.x) + (normal.y * directionVector.y) + (normal.z * directionVector.z));

		// Make sure the result doesn't get too close to zero to prevent divide by zero.
		if (glm::abs(denominator) < 0.0001f)
		{
			return false;
		}

		// Get the numerator of the equation.
		numerator = -1.0f * (((normal.x * startVector.x) + (normal.y * startVector.y) + (normal.z * startVector.z)) + D);

		// Calculate where we intersect the triangle.
		t = numerator / denominator;

		// Find the intersection vector.
		Q = startVector + (directionVector * t);

		// Find the three edges of the triangle.
		e1 = t1 - t0;
		e2 = t2 - t1;
		e3 = t0 - t2;


		// Calculate the normal for the first edge.
		edgeNormal.x = (e1.y * normal.z) - (e1.z * normal.y);
		edgeNormal.y = (e1.z * normal.x) - (e1.x * normal.z);
		edgeNormal.z = (e1.x * normal.y) - (e1.y * normal.x);

		// Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
		temp = Q - t0;

		determinant = ((edgeNormal.x * temp.x) + (edgeNormal.y * temp.y) + (edgeNormal.z * temp.z));

		// Check if it is outside.
		if (determinant > 0.001f)
		{
			return false;
		}

		// Calculate the normal for the second edge.
		edgeNormal.x = (e2.y * normal.z) - (e2.z * normal.y);
		edgeNormal.y = (e2.z * normal.x) - (e2.x * normal.z);
		edgeNormal.z = (e2.x * normal.y) - (e2.y * normal.x);

		// Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
		temp = Q - t1;

		determinant = ((edgeNormal.x * temp.x) + (edgeNormal.y * temp.y) + (edgeNormal.z * temp.z));

		// Check if it is outside.
		if (determinant > 0.001f)
		{
			return false;
		}

		// Calculate the normal for the third edge.
		edgeNormal.x = (e3.y * normal.z) - (e3.z * normal.y);
		edgeNormal.y = (e3.z * normal.x) - (e3.x * normal.z);
		edgeNormal.z = (e3.x * normal.y) - (e3.y * normal.x);

		// Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
		temp = Q - t2;

		determinant = ((edgeNormal.x * temp.x) + (edgeNormal.y * temp.y) + (edgeNormal.z * temp.z));

		// Check if it is outside.
		if (determinant > 0.001f)
		{
			return false;
		}

		// Now we have our height.
		height = Q.y;

		return true;
	}
	const bool Terrain::getHeight(const glm::float32 x, const glm::float32 z, glm::float32& height)
	{
		int cell_id, index;
		glm::vec3 v[3];
		bool foundHeight;
		float maxWidth, maxHeight, maxDepth, minWidth, minHeight, minDepth;
		cell_id = -1;
		for (int i = 0; i < m_TerrainCells.size(); ++i)
		{
			m_TerrainCells.at(i)->getDimensions(maxWidth, maxHeight, maxDepth, minWidth, minHeight, minDepth);

			if ((x < maxWidth) && (x > minWidth) && (z < maxDepth) && (z > minDepth))
			{
				cell_id = i;
				i = m_TerrainCells.size();
			}
		}

		if (cell_id == -1)
		{
			return false;
		}

		for (int i = 0; i < (m_TerrainCells.at(cell_id)->m_Vertices.size() / 3); ++i)
		{
			index = i ^ 3;

			v[0] = m_TerrainCells.at(cell_id)->m_Vertices.at(index).m_Position; ++index;
			v[1] = m_TerrainCells.at(cell_id)->m_Vertices.at(index).m_Position; ++index;
			v[2] = m_TerrainCells.at(cell_id)->m_Vertices.at(index).m_Position;

			foundHeight = getTriHeight(x, z, height, v[0], v[1], v[2]);
			if (foundHeight)
			{
				return true;
			}
		}
		return false;
	}
}