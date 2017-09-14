#pragma once

#include "../util/util.h"
#include "../gfx/text.h"
#include "manager.h"
#include "scene.h"

namespace me::core
{
	class Texture;

	class AppInfo
	{
	public:
		enum class AntiAliasing
		{
			None,
			FXAA,
			MSAAx2,
			MSAAx4
		};

	public:
		glm::uint32 ww;
		glm::uint32 wh;
		glm::uint32 fw;
		glm::uint32 fh;
		bool	 full;
		bool	 vsync;
		AntiAliasing aa;

	public:
		AppInfo();
		AppInfo(glm::uint32 ww, glm::uint32 wh, glm::uint32 fw, glm::uint32 fh, bool fullscreen, bool vsync, AntiAliasing aa);
		~AppInfo();

		bool loadFromFile(const std::string& file);
		const glm::uvec2& getResolution();

	private:
		bool isNumber(const std::string& str);

	};

	class App
	{
	public:
		App();
		~App();
		bool create();
		bool run();
		bool addScene(const Scene* scene, const std::string& call);
		bool setScene(const std::string& call);
		Scene* getScene(const std::string& call);
		Scene* getScene();

	public:
		GLFWwindow* m_GlfwWindow;
		AppInfo		m_AppInfo;
		std::map<std::string, Scene*> m_Scenes;
		std::string m_CurrentScene;
		GLuint		m_FBO;
		GLuint		m_FBO_col;
		GLuint		m_FBO_dep;
		GLuint		m_Post_VAO, m_Post_VBO;
		Shader		m_Post_Shader;

	};

	extern App* g_AppInstance;
	extern Manager<Texture>* g_Textures;
}