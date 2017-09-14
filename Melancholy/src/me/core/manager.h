#pragma once

#include "../util/util.h"


namespace me::core
{
	template<typename Type>
	class Manager
	{
	public:
		Manager() : m_VectorBuilt(true), m_cVectorBuilt(true)
		{

		}
		~Manager()
		{

		}

		void add(const std::string& id, Type data)
		{
			invalidate();
			m_Map[id] = std::make_shared<Type>(data);
		}
		Type const_get(const std::string& id)
		{
			if (!exists(id)) return Type();
			return *m_Map[id];
		}
		std::vector<Type> const_getAll()
		{
			if (!m_cVectorBuilt) buildVector();
			return m_cVector;
		}
		std::shared_ptr<Type> get(const std::string& id)
		{
			if (!exists(id)) return 0;
			invalidate();
			return m_Map[id];
		}
		std::vector<std::shared_ptr<Type>> getAll()
		{
			if (!m_VectorBuilt) buildVector();
			invalidate();
			return m_Vector;
		}
		void erase(const std::string& id)
		{
			if (exists(id))
			{
				m_Map.erase(id);
				invalidate();
			}
		}
		void clear()
		{
			m_Map.clear();
			m_Vector.clear();
			m_cVector.clear();
		}
		bool exists(const std::string& id) const
		{
			return m_Map.find(id) != m_Map.end();
		}

	protected:
		void buildVector()
		{
			if (!m_VectorBuilt)
			{
				m_Vector.clear();
				for (auto& i : m_Map)
				{
					m_Vector.push_back(i.second);
				}
				m_VectorBuilt = true;
			}
			if (!m_cVectorBuilt)
			{
				m_cVector.clear();
				for (const auto& i : m_Vector)
				{
					m_cVector.push_back(*i);
				}
				m_cVectorBuilt = true;
			}
		}
		void invalidate()
		{
			m_VectorBuilt = false;
			m_cVectorBuilt = false;
		}

	protected:
		std::map<std::string, std::shared_ptr<Type>> m_Map;
		std::vector<std::shared_ptr<Type>> m_Vector;
		std::vector<Type> m_cVector;
		bool m_VectorBuilt;
		bool m_cVectorBuilt;
	};
}