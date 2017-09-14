#include "model.h"
#include "mesh.h"


namespace me::core
{
	Model::Model()
	{
		
	}
	Model::~Model()
	{
		m_Meshes.clear();
		m_Textures.clear();
	}

	bool Model::draw(const glm::float64& delta)
	{
		if (m_File.empty()) return true;

		for (auto& i : m_Meshes)
		{
			if (!i.draw(delta))
			{
				return false;
			}
		}
	}
	bool Model::loadFromFile(const std::string& file)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			throw util::RuntimeError(util::RuntimeError::ErrorType::Read, "Model: " + file);
			return false;
		}

		m_Directory = file.substr(0, file.find_last_of('/'));
		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode *node, const aiScene *scene)
	{
		for (glm::size_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(processMesh(mesh, scene));
		}
		for (glm::size_t i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}
	Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
	{
		std::vector<util::Vertex> vertices;
		std::vector<glm::uint32> indices;
		std::vector<Texture*> textures;

		for (glm::size_t i = 0; i < mesh->mNumVertices; i++)
		{
			util::Vertex vertex;
			glm::vec3 vector;

			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.m_Position = vector;

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.m_TexCoord = vec;
			}
			else
			{
				vertex.m_TexCoord = glm::vec2(0.f, 0.f);
			}

			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.m_Tangent = vector;

			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.m_Bitangent = vector;
			vertices.push_back(vertex);
		}
		for (glm::size_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for (glm::size_t j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, Texture::Type::Diffuse);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		auto normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, Texture::Type::Normal);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		//CHANGE
		return Mesh();
	}
	std::vector<Texture*> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type0, Texture::Type type1)
	{
		std::vector<Texture*> textures;

		for (glm::size_t i = 0; i < mat->GetTextureCount(type0); i++)
		{
			aiString str;
			mat->GetTexture(type0, i, &str);

			bool skip = false;
			for (glm::size_t j = 0; j < m_Textures.size(); j++)
			{
				if (std::strcmp(m_Textures[j]->getFile().c_str(), str.C_Str()) == 0)
				{
					textures.push_back(m_Textures.at(j));
					skip = true;
					break;
				}
			}
			if (!skip)
			{
				Texture* texture = new Texture();
				//CHANGE
				//texture->loadFromFile(str.C_Str(), type1);
				textures.push_back(texture);
				m_Textures.push_back(texture);
			}
		}

		return textures;
	}

}