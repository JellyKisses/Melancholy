#include "camera.h"
#include "app.h"


namespace me::core
{
	Camera3D::Camera3D() : m_Position(0.f, 0.f, 0.f), m_WorldUp(0.f, 1.f, 0.f), m_Yaw(0.f), m_Pitch(0.f)
	{
		update(1);
	}
	Camera3D::Camera3D(const glm::vec3& position, const glm::vec3& up, const glm::float32& yaw, const glm::float32& pitch) : m_Position(position), m_WorldUp(up), m_Yaw(yaw), m_Pitch(pitch)
	{
		update(1);
	}
	Camera3D::Camera3D(const glm::float32& posX, const glm::float32& posY, const glm::float32& posZ, const glm::float32& upX, const glm::float32& upY, const glm::float32& upZ, const glm::float32& yaw, const glm::float32& pitch) : m_Position(posX, posY, posZ), m_WorldUp(upX, upY, upZ), m_Yaw(yaw), m_Pitch(pitch)
	{
		update(1);
	}
	Camera3D::~Camera3D()
	{
		m_Position = glm::vec3();
		m_Front = glm::vec3();
		m_Up = glm::vec3();
		m_Right = glm::vec3();
		m_WorldUp = glm::vec3();
		m_Yaw = 0.f;
		m_Pitch = 0.f;
	}
	void Camera3D::setPosition(const glm::vec3& xyz)
	{
		m_Position = xyz;
	}
	void Camera3D::setPosition(const glm::float32 x, const glm::float32& y, const glm::float32& z)
	{
		m_Position = glm::vec3(x, y, z);
	}
	const glm::vec3& Camera3D::getPosition()
	{
		return m_Position;
	}
	void Camera3D::setFront(const glm::vec3& xyz)
	{
		m_Front = xyz;
	}
	void Camera3D::setFront(const glm::float32 x, const glm::float32& y, const glm::float32& z)
	{
		m_Front = glm::vec3(x, y, z);
	}
	const glm::vec3& Camera3D::getFront()
	{
		return m_Front;
	}
	void Camera3D::setUp(const glm::vec3& xyz)
	{
		m_Up = xyz;
	}
	void Camera3D::setUp(const glm::float32& x, const glm::float32& y, const glm::float32& z)
	{
		m_Up = glm::vec3(x, y, z);
	}
	const glm::vec3& Camera3D::getUp()
	{
		return m_Up;
	}
	void Camera3D::setRight(const glm::vec3 xyz)
	{
		m_Right = xyz;
	}
	void Camera3D::setRight(const glm::float32& x, const glm::float32& y, const glm::float32& z)
	{
		m_Right = glm::vec3(x, y, z);
	}
	const glm::vec3& Camera3D::getRight()
	{
		return m_Right;
	}
	void Camera3D::setWorldUp(const glm::vec3& xyz)
	{
		m_WorldUp = xyz;
	}
	void Camera3D::setWorldUp(const glm::float32& x, const glm::float32& y, const glm::float32& z)
	{
		m_WorldUp = glm::vec3(x, y, z);
	}
	const glm::vec3& Camera3D::getWorldUp()
	{
		return m_WorldUp;
	}
	void Camera3D::setYaw(const glm::float32& x)
	{
		m_Yaw = x;
	}
	const glm::float32 Camera3D::getYaw()
	{
		return m_Yaw;
	}
	void Camera3D::setPitch(const glm::float32& x)
	{
		m_Pitch = x;
	}
	const glm::float32 Camera3D::getPitch()
	{
		return m_Pitch;
	}
	void Camera3D::update(const glm::float64_t delta)
	{
		m_Yaw = glm::mod(m_Yaw, 360.f);

		m_Front = glm::normalize(glm::vec3(glm::cos(glm::radians(m_Yaw)) *
			glm::cos(glm::radians(m_Pitch)),
			glm::sin(glm::radians(m_Pitch)),
			glm::sin(glm::radians(m_Yaw)) *
			glm::cos(glm::radians(m_Pitch))));
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));

		const float vel_grv = 5.f;
		float vel_acc;
		float vel_bnd;

		if (glfwGetKey(g_AppInstance->m_GlfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			vel_bnd = 10.f;
			vel_acc = 2.f;
		}
		else
		{
			vel_bnd = 5.f;
			vel_acc = 1.f;
		}

		if (glfwGetKey(g_AppInstance->m_GlfwWindow, GLFW_KEY_W) == GLFW_PRESS)
		{
			m_Velocity[0] += glm::vec3(0.f, 0.f, -vel_acc);
		}
		if (glfwGetKey(g_AppInstance->m_GlfwWindow, GLFW_KEY_A) == GLFW_PRESS)
		{
			m_Velocity[0] += glm::vec3(-vel_acc, 0.f, 0.f);
		}
		if (glfwGetKey(g_AppInstance->m_GlfwWindow, GLFW_KEY_S) == GLFW_PRESS)
		{
			m_Velocity[0] += glm::vec3(0.f, 0.f, vel_acc);
		}
		if (glfwGetKey(g_AppInstance->m_GlfwWindow, GLFW_KEY_D) == GLFW_PRESS)
		{
			m_Velocity[0] += glm::vec3(vel_acc, 0.f, 0.f);
		}

		/**/ if (m_Velocity[0].x > 0.f) m_Velocity[0].x -= vel_grv * delta;
		else if (m_Velocity[0].x < 0.f) m_Velocity[0].x += vel_grv * delta;
		/**/ if (m_Velocity[0].y > 0.f) m_Velocity[0].y -= vel_grv * delta;
		else if (m_Velocity[0].y < 0.f) m_Velocity[0].y += vel_grv * delta;
		/**/ if (m_Velocity[0].z > 0.f) m_Velocity[0].z -= vel_grv * delta;
		else if (m_Velocity[0].z < 0.f) m_Velocity[0].z += vel_grv * delta;
		
		if ((m_Velocity[0].x < 0.f && m_Velocity[1].x > 0.f) || (m_Velocity[0].x > 0.f && m_Velocity[1].x < 0.f)) m_Velocity[0].x = 0;
		if ((m_Velocity[0].y < 0.f && m_Velocity[1].y > 0.f) || (m_Velocity[0].y > 0.f && m_Velocity[1].y < 0.f)) m_Velocity[0].y = 0;
		if ((m_Velocity[0].z < 0.f && m_Velocity[1].z > 0.f) || (m_Velocity[0].z > 0.f && m_Velocity[1].z < 0.f)) m_Velocity[0].z = 0;

		

		m_Velocity[0] = glm::clamp(m_Velocity[0], glm::vec3(-vel_bnd), glm::vec3(vel_bnd));
		m_Position += m_Velocity[0] * glm::vec3(delta);

		m_Velocity[1] = m_Velocity[0];
	}

}