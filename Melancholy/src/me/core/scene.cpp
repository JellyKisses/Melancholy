#include "scene.h"
#include "app.h"
#include "camera.h"
#include "shader.h"
#include "../gfx/terrain.h"


namespace me::core
{
	Scene::Scene() : m_Delta(0.), m_Loaded(false)
	{

	}
	Scene::~Scene()
	{
		m_Shaders.clear();
	}


	const bool Scene::load()
	{
		if (!m_Loaded)
		{	
			m_Terrain = new gfx::Terrain();
			
			m_Camera = new Camera3D(0.f, 5.0f, 0.f, 0.f, 1.f, 0.f, 90.f, -45.f);
			
			test_Font = new gfx::Font();
			test_Font->loadFromFile("data/fon/munro.ttf");

			test_Text = new gfx::Text("Hello World!", *test_Font);

			addShader(new Shader(), "example_shader");
			getShader("example_shader")->loadFromFile("data/glsl/v_example_shader.glsl", Shader::Type::Vertex);
			getShader("example_shader")->loadFromFile("data/glsl/f_example_shader.glsl", Shader::Type::Fragment);

			test_Texture = new Texture();
			test_Texture->loadFromFile("data/tex/test01.png");

			m_Loaded = true;
		}

		return true;
	}
	const bool Scene::isLoaded()
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

	void Scene::addShader(Shader *shader, const std::string& call)
	{
		if (m_Shaders.find(call) == m_Shaders.end())
			m_Shaders.insert(std::pair<std::string, Shader*>(call, shader));
	}
	Shader* Scene::getShader(const std::string& call)
	{
		if (m_Shaders.find(call) != m_Shaders.end())
			return m_Shaders.at(call);
		else
			throw util::RuntimeError(util::RuntimeError::ErrorType::Find, std::string("Shader: ") + call, "It Doesn't Exist");
		return 0;
	}

	const glm::float64_t Scene::getDelta()
	{
		return m_Delta;
	}

	void Scene::draw(const glm::float64_t& delta)
	{
		m_Delta = delta;

		getShader("example_shader")->bind();
		//getShader("example_shader")->addUniformF("model", glm::mat4());
		getShader("example_shader")->addUniformF("view", glm::lookAt(m_Camera->getPosition(), m_Camera->getPosition() + m_Camera->getFront(), m_Camera->getUp()));
		getShader("example_shader")->addUniformF("projection", glm::perspective(glm::radians(70.f), static_cast<glm::float32_t>(g_AppInstance->m_AppInfo.getResolution().x) / static_cast<glm::float32_t>(g_AppInstance->m_AppInfo.getResolution().y), 0.1f, 1024.f));
		getShader("example_shader")->addUniformI("tex", test_Texture->getID(GL_TEXTURE0));

		if (glfwGetKey(g_AppInstance->m_GlfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			exit(0);
		}

		m_Camera->update(delta);
		m_Terrain->draw(delta);
		test_Text->draw();
	}

}