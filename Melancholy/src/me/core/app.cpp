#include "app.h"


namespace me::core
{
	App* g_AppInstance = 0;

	AppInfo::AppInfo() : ww(800), wh(600), fw(800), fh(600), full(false), vsync(false)
	{

	}
	AppInfo::AppInfo(glm::uint32 ww, glm::uint32 wh, glm::uint32 fw, glm::uint32 fh, bool fullscreen, bool vsync, AntiAliasing aa) : ww(ww), wh(wh), fw(fw), fh(fh), full(fullscreen), vsync(vsync), aa(aa)
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
				else if (buf == "aa=")
				{
					in >> buf;
					if (buf == "None" || buf == "none")
					{
						aa = AntiAliasing::None;
					}
					else if (buf == "fxaa" || buf == "FXAA")
					{
						aa = AntiAliasing::FXAA;
					}
					else if (buf == "msaax2" || buf == "msaaX2" || buf == "MSAAx2")
					{
						aa = AntiAliasing::MSAAx2;
					}
					else if (buf == "msaax4" || buf == "msaaX4" || buf == "MSAAx4")
					{
						aa = AntiAliasing::MSAAx4;
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
		glDeleteVertexArrays(1, &m_Post_VAO);
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

		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		glGenTextures(1, &m_FBO_col);
		glBindTexture(GL_TEXTURE_2D, m_FBO_col);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_AppInfo.getResolution().x, m_AppInfo.getResolution().y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FBO_col, 0);

		glGenRenderbuffers(1, &m_FBO_dep);
		glBindRenderbuffer(GL_RENDERBUFFER, m_FBO_dep);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_AppInfo.getResolution().x, m_AppInfo.getResolution().y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_FBO_dep);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			throw util::RuntimeError(util::RuntimeError::ErrorType::Create, "Framebuffer");
			return false;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		const static glm::float32 _Post_Vertices[] = {
			-1.f, 1.f, 0.f, 1.f,
			-1.f, -1.f, 0.f, 0.f,
			1.f, -1.f, 1.f, 0.f,
			-1.f, 1.f, 0.f, 1.f,
			1.f, -1.f, 1.f, 0.f,
			1.f, 1.f, 1.f, 1.f
		};

		glGenVertexArrays(1, &m_Post_VAO);
		glGenBuffers(1, &m_Post_VBO);
		glBindVertexArray(m_Post_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_Post_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_Post_Vertices), &_Post_Vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::float32), (GLvoid*)(0 * sizeof(glm::float32)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::float32), (GLvoid*)(2 * sizeof(glm::float32)));

		m_Post_Shader.loadFromFile("data/glsl/v_post_shader.glsl", Shader::Type::Vertex);
		m_Post_Shader.loadFromFile("data/glsl/f_post_shader.glsl", Shader::Type::Fragment);

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

			glEnable(GL_DEPTH_TEST);
			glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
			glClearColor(0.f, 0.f, 0.f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			getScene(m_CurrentScene)->draw(delta);
		
			glDisable(GL_DEPTH_TEST);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClearColor(0.f, 0.f, 0.f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_Post_Shader.bind();
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_FBO_col);
			m_Post_Shader.addUniformI("tex", m_FBO_col);
			m_Post_Shader.addUniformF("res", m_AppInfo.getResolution());
			switch (m_AppInfo.aa)
			{
			default:
			case AppInfo::AntiAliasing::None: m_Post_Shader.addUniformI("lvl", 0); break;
			case AppInfo::AntiAliasing::FXAA: m_Post_Shader.addUniformI("lvl", 1); break;
			case AppInfo::AntiAliasing::MSAAx2: m_Post_Shader.addUniformI("lvl", 2); break;
			case AppInfo::AntiAliasing::MSAAx4: m_Post_Shader.addUniformI("lvl", 3); break;
			}

			glBindVertexArray(m_Post_VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			

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