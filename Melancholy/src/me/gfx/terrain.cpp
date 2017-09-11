#include "terrain.h"
#include "../core/image.h"


namespace me::gfx
{
	Terrain::TerrainCell::TerrainCell() : m_Initialized(false)
	{
		glGenBuffers(1, &m_Vbo);
		glGenBuffers(1, &m_Ebo);
	}
	Terrain::TerrainCell::~TerrainCell()
	{
		glDeleteBuffers(1, &m_Vbo);
		glDeleteBuffers(1, &m_Ebo);
	}
	const bool Terrain::TerrainCell::initialize(const glm::ivec2 offset, const std::array<glm::float32, 25>& heights)
	{
		if (heights.empty())
		{
			throw util::RuntimeError(util::RuntimeError::ErrorType::Initialize, "Terrain Cell", "It has no vertices");
			return false;
		}
		else
		{
			m_Heights = heights;
		}

		m_Offset = offset;

		calcVertices();
		calcIndices();

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
		glBufferData(GL_ARRAY_BUFFER, sizeof(util::Vertex) * m_Vertices.size(), &m_Vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uint32) * m_Indices.size(), &m_Indices[0], GL_STATIC_DRAW);

		m_Initialized = true;
		return true;
	}
	void Terrain::TerrainCell::draw(const glm::float64_t& delta)
	{
		glDrawElements(GL_TRIANGLE_STRIP, m_Indices.size(), GL_UNSIGNED_INT, 0);
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
	const bool Terrain::TerrainCell::calcVertices()
	{
		if (m_Heights.empty()) return false;

		m_Vertices.clear();
		m_Indices.clear();

		int i = 0;
		for (int y = 0; y < 5; y++)
		{
			for (int x = 0; x < 5; x++)
			{
				glm::float32 halfWidth = (5.f - 1.f) / 2.f;
				glm::float32 halfHeight = (5.f - 1.f) / 2.f;

				m_Vertices.push_back(util::Vertex((static_cast<glm::float32>(x) - halfWidth) - (4 * m_Offset.x), m_Heights.at(i), (static_cast<glm::float32>(y) - halfHeight) - (4 * m_Offset.y),
					0.f, 1.f, 0.f,
					static_cast<glm::float32>(x) / 4, static_cast<glm::float32>(y) / 4));

				if (i % 3 == 0 && i != 0)
				{
					glm::vec3 u = m_Vertices.at(i - 1).m_Position - m_Vertices.at(i - 2).m_Position;
					glm::vec3 v = m_Vertices.at(i    ).m_Position - m_Vertices.at(i - 2).m_Position;

					m_Vertices.at(i).m_Normal.x = m_Vertices.at(i - 1).m_Normal.x = m_Vertices.at(i).m_Normal.x = (u.y * v.z) - (u.z * v.y);
					m_Vertices.at(i).m_Normal.y = m_Vertices.at(i - 1).m_Normal.y = m_Vertices.at(i).m_Normal.y = (u.z * v.x) - (u.x * v.z);
					m_Vertices.at(i).m_Normal.z = m_Vertices.at(i - 1).m_Normal.z = m_Vertices.at(i).m_Normal.z = (u.x * v.y) - (u.y * v.x);
				}
				i++;
			}
		}

		return true;
	}
	const bool Terrain::TerrainCell::calcIndices()
	{
		glm::int32 numIndices = 10 * 4 + 3;
		m_Indices.resize(numIndices);

		int index = 0;
		for (int z = 0; z < 4; z++)
		{
			if (z % 2 == 0)
			{
				int x;
				for (x = 0; x < 5; x++)
				{
					m_Indices.at(index++) = x + (z * 5);
					m_Indices.at(index++) = x + (z * 5) + 5;
				}
				if (z != 3)
				{
					m_Indices.at(index++) = (--x + (z * 5));
				}
			}
			else
			{
				int x;
				for (x = 4; x >= 0; x--)
				{
					m_Indices.at(index++) = x + (z * 5);
					m_Indices.at(index++) = x + (z * 5) + 5;
				}
				if (z != 3)
				{
					m_Indices.at(index++) = (++x + (z * 5));
				}
			}
		}

		return true;
	}

	Terrain::Terrain()
	{
		glGenVertexArrays(1, &m_Vao);
	}
	Terrain::~Terrain()
	{
		glDeleteVertexArrays(1, &m_Vao);
	}
	const bool Terrain::initialize()
	{
		glBindVertexArray(m_Vao);
		//CHANGE THIS!
		m_Size = glm::uvec2(4, 4);

		core::Image height = util::perlin(4.0f, 0.5f, 1 + m_Size.x * 5, 1 + m_Size.y * 5);

		const static glm::float32 scale = 128.f;

		if (m_Size != glm::uvec2(0, 0))
		{
			for (int x = 0; x < m_Size.x; x++)
			{
				for (int y = 0; y < m_Size.y; y++)
				{
					TerrainCell *cell = new TerrainCell();
					cell->initialize(
					glm::ivec2(x, y),
					{
						glm::mix(static_cast<glm::float32>(height.getPixel(1 + (x * 5 + 0), 1 + (y * 5 + 0)).r) / scale, static_cast<glm::float32>(height.getPixel(0 + (x * 5 + 0), 0 + (y * 5 + 0)).r) / scale, 0.5f),
						glm::mix(static_cast<glm::float32>(height.getPixel(1 + (x * 5 + 0), 1 + (y * 5 + 1)).r) / scale, static_cast<glm::float32>(height.getPixel(0 + (x * 5 + 0), 0 + (y * 5 + 1)).r) / scale, 0.5f),
						glm::mix(static_cast<glm::float32>(height.getPixel(1 + (x * 5 + 0), 1 + (y * 5 + 2)).r) / scale, static_cast<glm::float32>(height.getPixel(0 + (x * 5 + 0), 0 + (y * 5 + 2)).r) / scale, 0.5f),
						glm::mix(static_cast<glm::float32>(height.getPixel(1 + (x * 5 + 0), 1 + (y * 5 + 3)).r) / scale, static_cast<glm::float32>(height.getPixel(0 + (x * 5 + 0), 0 + (y * 5 + 3)).r) / scale, 0.5f),
						glm::mix(static_cast<glm::float32>(height.getPixel(1 + (x * 5 + 0), 1 + (y * 5 + 4)).r) / scale, static_cast<glm::float32>(height.getPixel(0 + (x * 5 + 0), 0 + (y * 5 + 4)).r) / scale, 0.5f),
						
						0.f, 0.f, 0.f, 0.f, 0.f,
						0.f, 0.f, 0.f, 0.f, 0.f,
						0.f, 0.f, 0.f, 0.f, 0.f,
						0.f, 0.f, 0.f, 0.f, 0.f
					});

					m_TerrainCells.push_back(cell);
				}
			}
	
			return true;
		}
	
		return false;
	}
	void Terrain::draw(const glm::float64_t& delta)
	{

		for (auto& it : m_TerrainCells)
		{
			glBindVertexArray(m_Vao);
			glBindBuffer(GL_ARRAY_BUFFER, it->m_Vbo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, it->m_Ebo);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(util::Vertex), (GLvoid*)(0 * sizeof(glm::float32)));
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(util::Vertex), (GLvoid*)(3 * sizeof(glm::float32)));
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(util::Vertex), (GLvoid*)(5 * sizeof(glm::float32)));

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