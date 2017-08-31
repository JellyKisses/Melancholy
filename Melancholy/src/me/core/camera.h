#pragma once

#include "../util/util.h"


namespace me::core
{
	class Camera3D
	{
	public:
		glm::vec3 m_Position;
		glm::vec3 m_Front;
		glm::vec3 m_Up;
		glm::vec3 m_Right;
		glm::vec3 m_WorldUp;
		glm::vec3 m_Velocity[2];

		glm::float32 m_Yaw;
		glm::float32 m_Pitch;

		bool m_Moving;

	public:
		Camera3D();
		Camera3D(const glm::vec3& position = glm::vec3(0, 0, 0), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f), const glm::float32& yaw = 0.f, const glm::float32& pitch = 0.f);
		Camera3D(const glm::float32& posX = 0.f, const glm::float32& posY = 0.f, const glm::float32& posZ = 0.f, const glm::float32& upX = 0.f, const glm::float32& upY = 1.f, const glm::float32& upZ = 0.f, const glm::float32& yaw = 0.f, const glm::float32& pitch = 0.f);
		virtual ~Camera3D();

		void setPosition(const glm::vec3& xyz);
		void setPosition(const glm::float32 x, const glm::float32& y, const glm::float32& z);
		const glm::vec3& getPosition();

		void setFront(const glm::vec3& xyz);
		void setFront(const glm::float32 x, const glm::float32& y, const glm::float32& z);
		const glm::vec3& getFront();

		void setUp(const glm::vec3& xyz);
		void setUp(const glm::float32& x, const glm::float32& y, const glm::float32& z);
		const glm::vec3& getUp();

		void setRight(const glm::vec3 xyz);
		void setRight(const glm::float32& x, const glm::float32& y, const glm::float32& z);
		const glm::vec3& getRight();

		void setWorldUp(const glm::vec3& xyz);
		void setWorldUp(const glm::float32& x, const glm::float32& y, const glm::float32& z);
		const glm::vec3& getWorldUp();

		void setYaw(const glm::float32& x);
		const glm::float32 getYaw();

		void setPitch(const glm::float32& x);
		const glm::float32 getPitch();

		void update(const glm::float64_t delta);

	};


}