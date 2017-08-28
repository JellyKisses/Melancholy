#pragma once

#include "../util/util.h"


namespace me::core
{
	class Transformable3D
	{
	protected:
		glm::mat4 m_Translation;
		glm::mat4 m_Rotation;
		glm::mat4 m_Scale;

		glm::vec3 m_CacheTranslation;
		glm::vec3 m_CacheRotation;
		glm::vec3 m_CacheScale;

	public:
		Transformable3D();
		~Transformable3D();

		void move(const glm::vec3& xyz);
		void move(const glm::float32& x, const glm::float32& y,const glm::float32& z);
		void setPosition(const glm::vec3& xyz);
		void setPosition(const glm::float32& x, const glm::float32& y, const glm::float32& z);
		const glm::vec3& getPosition();
		void rotate(const glm::float32& a, const glm::vec3& xyz);
		void rotate(const glm::float32& a, const glm::float32& x, const glm::float32& y, const glm::float32& z);
		void setRotation(const glm::float32& a, glm::vec3 xyz);
		void setRotation(const glm::float32& a, const glm::float32& x, const glm::float32& y, const glm::float32& z);
		const glm::vec3& getRotation();
		void scale(const glm::vec3& xyz);
		void scale(const glm::float32& x, const glm::float32& y, const glm::float32& z);
		void setScale(const glm::vec3& xyz);
		void setScale(const glm::float32& x, const glm::float32& y, const glm::float32& z);
		const glm::vec3& getScale();
	};
	class Transformable2D
	{
	protected:
		glm::vec2 m_Translation;
		glm::vec2 m_Origin;

	public:
		Transformable2D();
		~Transformable2D();

		void move(const glm::vec2& xy);
		void move(const glm::float32& x, const glm::float32& y);
		void setPosition(const glm::vec2& xy);
		void setPosition(const glm::float32& x, const glm::float32& y);
		const glm::vec2& getPosition();
		void setOrigin(const glm::vec2& xy);
		void setOrigin(const glm::float32& x, const glm::float32& y);
		const glm::vec2& getOrigin();

		const glm::vec2& getTransformPoint(const glm::vec2& xy);
		const glm::vec2& getTransformPoint(const glm::float32& x, const glm::float32 & y);
	};

}