#pragma once

#include "../util/util.h"
#include "texture.h"


namespace me::core
{
	class Mesh;

	class Model
	{
	private:
		std::vector<Texture*> m_Textures;
		std::vector<Mesh> m_Meshes;
		std::string m_File;
		std::string m_Directory;

	public:
		Model();
		~Model();

		bool draw(const glm::float64& delta);
		bool loadFromFile(const std::string& file);

	private:
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);
		std::vector<Texture*> loadMaterialTextures(aiMaterial *mat, aiTextureType type0, Texture::Type type1);
	};
}