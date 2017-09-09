#include "app.h"


namespace me::core
{
	App* g_AppInstance = 0;

	AppInfo::AppInfo() : ww(800), wh(600), fw(800), fh(600), full(false), vsync(false)
	{

	}
	AppInfo::AppInfo(const glm::uint32& ww, const glm::uint32& wh, const glm::uint32& fw, const glm::uint32& fh, bool& fullscreen, bool& vsync) : ww(ww), wh(wh), fw(fw), fh(fh), full(fullscreen), vsync(vsync)
	{

	}
	AppInfo::~AppInfo()
	{
		ww = 0;
		wh = 0;
		fw = 0;
		fh = 0;
		full = false;
		vsync = false;
	}
	bool AppInfo::loadFromFile(const std::string& file)
	{
		std::string buf;
		std::ifstream in;
		in.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			in.open(file);

			while (!in.eof())
			{
				in >> buf;
				if (buf == "fullResW=")
				{
					in >> buf;
					if (isNumber(buf))
					{
						fw = std::atoi(buf.c_str());
					}
					else
					{
						throw util::RuntimeError(util::RuntimeError::ErrorType::Read, file, buf + " is an invalid term");
						return false;
					}
				}
				else if (buf == "fullResH=")
				{
					in >> buf;
					if (isNumber(buf))
					{
						fh = std::atoi(buf.c_str());
					}
					else
					{
						throw util::RuntimeError(util::RuntimeError::ErrorType::Read, file, buf + " is an invalid term");
						return false;
					}
				}
				else if (buf == "windResW=")
				{
					in >> buf;
					if (isNumber(buf))
					{
						ww = std::atoi(buf.c_str());
					}
					else
					{
						throw util::RuntimeError(util::RuntimeError::ErrorType::Read, file, buf + " is an invalid term");
						return false;
					}
				}
				else if (buf == "windResH=")
				{
					in >> buf;
					if (isNumber(buf))
					{
						wh = std::atoi(buf.c_str());
					}
					else
					{
						throw util::RuntimeError(util::RuntimeError::ErrorType::Read, file, buf + " is an invalid term");
						return false;
					}
				}
				else if (buf == "fullScr=")
				{
					in >> buf;
					if (buf == "true" || buf == "True" || buf == "1")
					{
						vsync = true;
					}
					else if (buf == "false" || buf == "False" || buf == "0")
					{
						vsync = false;
					}
					else
					{
						throw util::RuntimeError(util::RuntimeError::ErrorType::Read, file, buf + " is an invalid term");
						return false;
					}
				}
				else if (buf == "vSync=")
				{
					in >> buf;
					if (buf == "true" || buf == "True" || buf == "1")
					{
						full = true;
					}
					else if (buf == "false" || buf == "False" || buf == "0")
					{
						full = false;
					}
					else
					{
						throw util::RuntimeError(util::RuntimeError::ErrorType::Read, file, buf + " is an invalid term");
						return false;
					}
				}
				else
				{
					/*Error?*/
				}

			}

			in.close();
		}
		catch (std::ifstream::failure e)
		{
			throw util::RuntimeError(util::RuntimeError::ErrorType::Read, file, std::string("Config: ") + e.what());
			return false;
		}

		return true;
	}
	const glm::uvec2& AppInfo::getResolution()
	{
		return glm::uvec2(full ? fw : ww, full ? fh : wh);
	}
	bool AppInfo::isNumber(const std::string& str)
	{
		return !str.empty() && std::find_if(str.begin(), str.end(), [](char c) {return !std::isdigit(c); }) == str.end();
	}

	App::App()
	{
		g_AppInstance = this;
	}
	App::~App()
	{
		g_AppInstance = 0;
		//m_Scenes.clear();
	}

	bool App::create()
	{
		m_AppInfo.loadFromFile("Melancholy.ini");

		if (glfwInit() != GLFW_TRUE)
		{
			throw util::RuntimeError(util::RuntimeError::ErrorType::Initialize, "GLFW");
			return false;
		}
		
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		if (!(m_GlfwWindow = glfwCreateWindow(m_AppInfo.getResolution().x, m_AppInfo.getResolution().y, "Melancholy", m_AppInfo.full ? glfwGetPrimaryMonitor() : 0, 0)))
		{
			throw util::RuntimeError(util::RuntimeError::ErrorType::Create, "Window");
			return false;
		}

		glfwMakeContextCurrent(m_GlfwWindow);

		glewExperimental = GL_TRUE;
		const GLenum glewInitSuccess = glewInit();
		if (glewInitSuccess != GLEW_OK)
		{
			throw util::RuntimeError(util::RuntimeError::ErrorType::Initialize, "OpenGL4.3", reinterpret_cast<const char*>(glewGetErrorString(glewInitSuccess)));
		}

		glViewport(0, 0, m_AppInfo.getResolution().x, m_AppInfo.getResolution().y);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_DEPTH_TEST);
		
		glClearColor(0.5, 0.5, 0.5, 1.0);

		glfwSwapInterval(1);

		glfwSetCursorPosCallback(m_GlfwWindow, [](GLFWwindow* window, double x, double y)
		{

		});
		glfwSetMouseButtonCallback(m_GlfwWindow, [](GLFWwindow* window, int button, int action, int mods)
		{

		});

		addScene(new Scene(), "example_scene");
		setScene("example_scene");

		return true;
	}
	bool App::run()
	{
		glm::float64 currentTime = glfwGetTime();
		glm::float64 previousTime = currentTime;
		glm::float64 delta = previousTime - currentTime;

		while (!glfwWindowShouldClose(m_GlfwWindow))
		{
			currentTime = glfwGetTime();
			delta = currentTime - previousTime;
			previousTime = currentTime;

			glfwPollEvents();

			if (!getScene()->isLoaded()) getScene()->load();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			getScene(m_CurrentScene)->draw(delta);
		
			glfwSwapBuffers(m_GlfwWindow);
		}

		return true;
	}
	bool App::addScene(const Scene* scene, const std::string& call)
	{
		if (m_Scenes.find(call) == m_Scenes.end())
			m_Scenes.insert(std::pair<std::string, Scene*>(call, const_cast<Scene*>(scene)));

		return true;
	}
	bool App::setScene(const std::string& call)
	{
		m_CurrentScene = call;
	}
	Scene* App::getScene(const std::string& call)
	{
		if (m_Scenes.find(call) != m_Scenes.end())
			return m_Scenes.at(call);
		else
			throw util::RuntimeError(util::RuntimeError::ErrorType::Find, std::string("Scene: ") + call, "It Doesn't Exist");
		return 0;
	}
	Scene* App::getScene()
	{
		return getScene(m_CurrentScene);
	}

}