#pragma once

#include "../util/util.h"
#include "../gfx/terrain.h"
#include "../gfx/text.h"


namespace me::core { class Shader; class Camera3D; }
namespace me::gfx { class Terrain; }
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
		std::map<std::string, Shader*> m_Shaders;
		gfx::Terrain* m_Terrain;
		gfx::Text* test_Text;
		Texture* test_Texture;
		gfx::Font* test_Font;
		glm::float64_t m_Delta;
		Camera3D* m_Camera;
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

		const bool load();
		const bool isLoaded();

		void setCallback(Callbacks type, Callback&& func);
		void exeCallback(Callbacks type);

		void addShader(Shader *shader, const std::string& call);
		Shader* getShader(const std::string& call);

		const glm::float64_t getDelta();

		void draw(const glm::float64_t &delta);
	};
}