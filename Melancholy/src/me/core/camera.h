#pragma once

#include "../util/util.h"


namespace me::core
{
	class Camera3D
	{
	protected:
		glm::vec3 m_Position;
		glm::vec3 m_Front;
		glm::vec3 m_Up;
		glm::vec3 m_Right;
		glm::vec3 m_WorldUp;
		glm::vec3 m_Velocity;

		glm::float32_t m_Yaw;
		glm::float32_t m_Pitch;

	public:
		Camera3D();
		Camera3D(const glm::vec3& position = glm::vec3(0, 0, 0), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f), const glm::float32_t& yaw = 0.f, const glm::float32_t& pitch = 0.f);
		Camera3D(const glm::float32_t& posX = 0.f, const glm::float32_t& posY = 0.f, const glm::float32_t& posZ = 0.f, const glm::float32_t& upX = 0.f, const glm::float32_t& upY = 1.f, const glm::float32_t& upZ = 0.f, const glm::float32_t& yaw = 0.f, const glm::float32_t& pitch = 0.f);
		virtual ~Camera3D();

		void setPosition(const glm::vec3& xyz);
		void setPosition(const glm::float32_t x, const glm::float32_t& y, const glm::float32_t& z);
		const glm::vec3& getPosition();

		void setFront(const glm::vec3& xyz);
		void setFront(const glm::float32_t x, const glm::float32_t& y, const glm::float32_t& z);
		const glm::vec3& getFront();

		void setUp(const glm::vec3& xyz);
		void setUp(const glm::float32_t& x, const glm::float32_t& y, const glm::float32_t& z);
		const glm::vec3& getUp();

		void setRight(const glm::vec3 xyz);
		void setRight(const glm::float32_t& x, const glm::float32_t& y, const glm::float32_t& z);
		const glm::vec3& getRight();

		void setWorldUp(const glm::vec3& xyz);
		void setWorldUp(const glm::float32_t& x, const glm::float32_t& y, const glm::float32_t& z);
		const glm::vec3& getWorldUp();

		void setYaw(const glm::float32_t& x);
		const glm::float32_t getYaw();

		void setPitch(const glm::float32_t& x);
		const glm::float32_t getPitch();

		void update(const glm::float64_t delta);

	};


}