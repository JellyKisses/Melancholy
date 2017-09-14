#include "scene.h"
#include "app.h"
#include "camera.h"
#include "shader.h"
#include "texture.h"
#include "../gfx/world.h"

namespace me::core
{
	Scene::Scene() : m_Delta(0.), m_Loaded(false)
	{

	}
	Scene::~Scene()
	{
		m_Shaders.clear();
		g_Textures->clear();
	}


	bool Scene::load()
	{

		if (!m_Loaded)
		{	
			m_World = std::make_shared<gfx::World>();
			m_World->initialize();

			m_Camera = std::make_shared<Camera3D>(0.f, 5.0f, 0.f, 0.f, 1.f, 0.f, 270.f, -45.f);

			Texture tex;

			if (!tex.loadFromFile("data/tex/0_01.png", false)) g_Textures->add("Grass01", tex);
			if (!tex.loadFromFile("data/tex/1_01_01.png", false)) g_Textures->add("Tile01v01", tex);

			addShader("world_shader", Shader());
			getShader("world_shader")->loadFromFile("data/glsl/v_world_shader.glsl", Shader::Type::Vertex);
			getShader("world_shader")->loadFromFile("data/glsl/f_world_shader.glsl", Shader::Type::Fragment);

			m_Loaded = true;
		}

		return true;
	}
	bool Scene::isLoaded()
	{
		return m_Loaded;
	}

	void Scene::setCallback(Callbacks type, Callback&& func)
	{
		switch (type)
		{
		case Callbacks::Update:			m_Callbacks.Update = func; break;
		case Callbacks::Render:			m_Callbacks.Render = func; break;

		case Callbacks::Shown:			m_Callbacks.Shown = func; break;
		case Callbacks::Hidden:			m_Callbacks.Hidden = func; break;
		case Callbacks::Exposed:		m_Callbacks.Exposed = func; break;
		case Callbacks::Moved:			m_Callbacks.Moved = func; break;
		case Callbacks::Resized:		m_Callbacks.Resized = func; break;
		case Callbacks::Minimized:		m_Callbacks.Minimized = func; break;
		case Callbacks::Maximized:		m_Callbacks.Maximized = func; break;
		case Callbacks::Restored:		m_Callbacks.Restored = func; break;
		case Callbacks::Enter:			m_Callbacks.Enter = func; break;
		case Callbacks::Leave:			m_Callbacks.Leave = func; break;
		case Callbacks::FocusGained:	m_Callbacks.FocusGained = func; break;
		case Callbacks::FocusLost:		m_Callbacks.FocusLost = func; break;
		case Callbacks::Quit:			m_Callbacks.Quit = func; break;
		}
	}
	void Scene::exeCallback(Callbacks type)
	{
		switch (type)
		{
		case Callbacks::Update:			((Callback)m_Callbacks.Update)(); break;
		case Callbacks::Render:			((Callback)m_Callbacks.Render)(); break;

		case Callbacks::Shown:			((Callback)m_Callbacks.Shown)(); break;
		case Callbacks::Hidden:			((Callback)m_Callbacks.Hidden)(); break;
		case Callbacks::Exposed:		((Callback)m_Callbacks.Exposed)(); break;
		case Callbacks::Moved:			((Callback)m_Callbacks.Moved)(); break;
		case Callbacks::Resized:		((Callback)m_Callbacks.Resized)(); break;
		case Callbacks::Minimized:		((Callback)m_Callbacks.Minimized)(); break;
		case Callbacks::Maximized:		((Callback)m_Callbacks.Maximized)(); break;
		case Callbacks::Restored:		((Callback)m_Callbacks.Restored)(); break;
		case Callbacks::Enter:			((Callback)m_Callbacks.Enter)(); break;
		case Callbacks::Leave:			((Callback)m_Callbacks.Leave)(); break;
		case Callbacks::FocusGained:	((Callback)m_Callbacks.FocusGained)(); break;
		case Callbacks::FocusLost:		((Callback)m_Callbacks.FocusLost)(); break;
		case Callbacks::Quit:			((Callback)m_Callbacks.Quit)(); break;
		}
	}

	void Scene::addShader(const std::string& id, Shader shader)
	{
		if (m_Shaders.find(id) == m_Shaders.end())
			m_Shaders[id] = std::make_shared<Shader>(shader);
	}
	std::shared_ptr<Shader> Scene::getShader(const std::string& id)
	{
		if (m_Shaders.find(id) != m_Shaders.end())
			return m_Shaders.at(id);
		else
			throw util::RuntimeError(util::RuntimeError::ErrorType::Find, std::string("Shader: ") + id, "It Doesn't Exist");
		return 0;
	}
	const glm::float64 Scene::getDelta()
	{
		return m_Delta;
	}

	void Scene::draw(const glm::float64& delta)
	{
		m_Delta = delta;

		getShader("world_shader")->bind();
		getShader("world_shader")->addUniformF("u_projection", glm::perspective(glm::radians(40.f), static_cast<glm::float32_t>(g_AppInstance->m_AppInfo.getResolution().x) / static_cast<glm::float32_t>(g_AppInstance->m_AppInfo.getResolution().y), 0.1f, 1024.f));
		getShader("world_shader")->addUniformF("u_view", glm::lookAt(m_Camera->getPosition(), m_Camera->getPosition() + m_Camera->getFront(), m_Camera->getUp()));
		getShader("world_shader")->addUniformF("u_model", glm::mat4());
		getShader("world_shader")->addUniformF("u_position", m_Camera->getPosition());

		if (glfwGetKey(g_AppInstance->m_GlfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			exit(0);
		}

		m_Camera->update(delta);
		m_World->draw(delta);
	}
}