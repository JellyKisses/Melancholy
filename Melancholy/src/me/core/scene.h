#pragma once

#include "../util/util.h"


namespace me::core { class Shader; class Camera3D; class Texture; }
namespace me::gfx { class World; }
namespace me::core
{
	class Scene
	{
	private:
		typedef std::function<void()> Callback;
		class CallbackFuncs
		{
		public:
			CallbackFuncs()
			{
				Update = [] {};
				Render = [] {};
				Shown = [] {};
				Hidden = [] {};
				Exposed = [] {};
				Moved = [] {};
				Resized = [] {};
				Minimized = [] {};
				Maximized = [] {};
				Restored = [] {};
				Enter = [] {};
				Leave = [] {};
				FocusGained = [] {};
				FocusLost = [] {};
				Quit = [] {};
			}

		public:
			Callback Update;
			Callback Render;

			Callback Shown;
			Callback Hidden;
			Callback Exposed;
			Callback Moved;
			Callback Resized;
			Callback Minimized;
			Callback Maximized;
			Callback Restored;
			Callback Enter;
			Callback Leave;
			Callback FocusGained;
			Callback FocusLost;
			Callback Quit;
		} m_Callbacks;

	private:
		std::map<std::string, std::shared_ptr<Shader>> m_Shaders;
		std::shared_ptr<gfx::World> m_World;
		std::shared_ptr<Camera3D> m_Camera;
		glm::float64 m_Delta;
		bool m_Loaded;

	public:
		enum class Callbacks
		{
			Update,
			Render,
			Shown,
			Hidden,
			Exposed,
			Moved,
			Resized,
			Minimized,
			Maximized,
			Restored,
			Enter,
			Leave,
			FocusGained,
			FocusLost,
			Quit
		};

	public:
		Scene();
		~Scene();

		bool load();
		bool isLoaded();

		void setCallback(Callbacks type, Callback&& func);
		void exeCallback(Callbacks type);

		void addShader(const std::string& id, Shader shader);
		std::shared_ptr<Shader> getShader(const std::string& id);

		const glm::float64_t getDelta();

		void draw(const glm::float64 &delta);
	};
}