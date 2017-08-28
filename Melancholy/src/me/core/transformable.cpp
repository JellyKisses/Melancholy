#include "transformable.h"


namespace me::core
{
	Transformable3D::Transformable3D() {}
	Transformable3D::~Transformable3D() {}

	void Transformable3D::move(const glm::vec3& xyz)
	{
		m_Translation = glm::translate(m_Translation, xyz);
		m_CacheTranslation += xyz;
	}
	void Transformable3D::move(const glm::float32& x, const glm::float32& y, const glm::float32& z)
	{
		move(glm::vec3(x, y, z));
	}
	void Transformable3D::setPosition(const glm::vec3& xyz)
	{
		m_Translation = glm::translate(glm::mat4(), xyz);
		m_CacheTranslation = xyz;
	}
	void Transformable3D::setPosition(const glm::float32& x, const glm::float32& y, const glm::float32& z)
	{
		setPosition(glm::vec3(x, y, z));
	}
	const glm::vec3& Transformable3D::getPosition()
	{
		return m_CacheTranslation;
	}
	void Transformable3D::rotate(const glm::float32& a, const glm::vec3& xyz)
	{
		m_Rotation = glm::rotate(m_Rotation, a, xyz);
		m_CacheRotation += xyz * a;
	}
	void Transformable3D::rotate(const glm::float32& a, const glm::float32& x, const glm::float32& y, const glm::float32& z)
	{
		rotate(a, glm::vec3(x, y, z));
	}
	void Transformable3D::setRotation(const glm::float32& a, glm::vec3 xyz)
	{
		m_Rotation = glm::rotate(glm::mat4(), a, xyz);
		m_CacheRotation = xyz * a;
	}
	void Transformable3D::setRotation(const glm::float32& a, const glm::float32& x, const glm::float32& y, const glm::float32& z)
	{
		setRotation(a, glm::vec3(x, y, z));
	}
	const glm::vec3& Transformable3D::getRotation()
	{
		return m_CacheRotation;
	}
	void Transformable3D::scale(const glm::vec3& xyz)
	{
		m_Scale = glm::scale(m_Scale, xyz);
		m_CacheScale += xyz;
	}
	void Transformable3D::scale(const glm::float32& x, const glm::float32& y, const glm::float32& z)
	{
		scale(glm::vec3(x, y, z));
	}
	void Transformable3D::setScale(const glm::vec3& xyz)
	{
		m_Scale = glm::scale(glm::mat4(), xyz);
		m_CacheScale = xyz;
	}
	void Transformable3D::setScale(const glm::float32& x, const glm::float32& y, const glm::float32& z)
	{
		setScale(glm::vec3(x, y, z));
	}
	const glm::vec3& Transformable3D::getScale()
	{
		return m_CacheScale;
	}

	Transformable2D::Transformable2D() {}
	Transformable2D::~Transformable2D() {}
	void Transformable2D::move(const glm::vec2& xy)
	{
		m_Translation += xy;
	}
	void Transformable2D::move(const glm::float32& x, const glm::float32& y)
	{
		move(glm::vec2(x, y));
	}
	void Transformable2D::setPosition(const glm::vec2& xy)
	{
		m_Translation = xy;
	}
	void Transformable2D::setPosition(const glm::float32& x, const glm::float32& y)
	{
		setPosition(glm::vec2(x, y));
	}
	const glm::vec2& Transformable2D::getPosition()
	{
		return m_Translation;
	}
	void Transformable2D::setOrigin(const glm::vec2& xy)
	{
		m_Origin = xy;
	}
	void Transformable2D::setOrigin(const glm::float32& x, const glm::float32& y)
	{
		setOrigin(glm::vec2(x, y));
	}
	const glm::vec2& Transformable2D::getOrigin()
	{
			return m_Origin;
	}
	const glm::vec2& Transformable2D::getTransformPoint(const glm::vec2& xy)
	{
		return glm::vec2(xy + (m_Translation + m_Origin));
	}
	const glm::vec2& Transformable2D::getTransformPoint(const glm::float32& x, const glm::float32 & y)
	{
		return getTransformPoint(glm::vec2(x, y));
	}
}