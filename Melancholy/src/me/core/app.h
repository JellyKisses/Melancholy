#pragma once

#include "../util/util.h"
#include "../gfx/text.h"
#include "scene.h"


namespace me::core
{
	class AppInfo
	{
	public:
		glm::uint32 ww;
		glm::uint32 wh;
		glm::uint32 fw;
		glm::uint32 fh;
		bool	 full;
		bool	 vsync;

	public:
		AppInfo();
		AppInfo(const glm::uint32& ww, const glm::uint32& wh, const glm::uint32& fw, const glm::uint32& fh, const bool& fullscreen, const bool& vsync);
		~AppInfo();

		const bool loadFromFile(const std::string& file);
		const glm::uvec2& getResolution();

	private:
		const bool isNumber(const std::string& str);

	};

	class App
	{
	public:
		App();
		~App();
		const bool create();
		const bool run();
		const bool addScene(const Scene* scene, const std::string& call);
		const bool setScene(const std::string& call);
		Scene* getScene(const std::string& call);
		Scene* getScene();

	public:
		GLFWwindow* m_GlfwWindow;
		AppInfo		m_AppInfo;
		std::map<std::string, Scene*> m_Scenes;
		std::string m_CurrentScene;

	};

	extern App* g_AppInstance;
}