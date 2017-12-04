#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <windows.h>
#include <mesh.h>

typedef OpenMesh::TriMesh_ArrayKernelT<>  MyMesh;
bool isChange;
GLint TextureFromFile(const char* path, string directory);

class Model
{
public:
	/*  Functions   */
	// Constructor, expects a filepath to a 3D model.
	Model(GLchar* path)
	{
		this->loadModel(path);
		isChange = false;
	}

	Model()
	{

	}

	// Draws the model, and thus all its meshes
	void Draw(Shader shader)
	{

		for (GLuint i = 0; i < this->meshes.size(); i++) {
			
			this->meshes[i].Draw(shader);
		}

	}
	void upDate() {
		for (int i = 0; i < 20; i++)
		{
			upDateMesh();
		}
		meshes[0].setupMesh();
	}
	void upDateMesh()
	{
		int i = 0;
		for (auto it = meshOpen.vertices_begin(); it != meshOpen.vertices_end(); ++it)
		{

			glm::vec3 delta;
			auto point = meshOpen.point(*it);
			auto is_boundary = meshOpen.is_boundary(*it);

				float x = 0, y = 0, z = 0;
				int nd = 0;
				for (auto vv_it = meshOpen.vv_iter(*it); vv_it.is_valid(); vv_it++)
				{
					nd++;
					auto point2 = meshOpen.point(*vv_it);
					x += point2[0];
					y += point2[1];
					z += point2[2];
				}
				x /= nd;
				y /= nd;
				z /= nd;

				delta.x = point[0] - x;
				delta.y = point[1] - y;
				delta.z = point[2] - z;
			

			meshes[0].vertices[i].delta = delta;
			i++;
		}
		upDateVertices();
		
		
		isChange = true;
	}

	void upDateVertices() {
		int i = 0;
		for (auto it = meshOpen.vertices_begin(); it != meshOpen.vertices_end(); ++it) {
			auto point = meshOpen.point(*it);
			point[0] -= meshes[0].vertices[i].delta.x * 0.5;
			point[1] -= meshes[0].vertices[i].delta.y * 0.5;
			point[2] -= meshes[0].vertices[i].delta.z * 0.5;
			meshOpen.set_point(*it, point);
			meshes[0].vertices[i].Position -= meshes[0].vertices[i].delta*glm::vec3(0.5, 0.5, 0.5);

			i++;
		}
		isChange = false;
	}


private:
	/*  Model Data  */
	vector<Mesh> meshes;
	string directory;
	MyMesh meshOpen;
	vector<Texture> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

	void ReadMesh(string filename)
	{

		/*QByteArray byfilename = filename.toLocal8Bit();
		ptr_mesh_->LoadFromOBJFile(byfilename.data());*/
		OpenMesh::IO::Options opt_read = 0x0040;
		meshOpen.request_vertex_texcoords2D();
		if (!OpenMesh::IO::read_mesh(meshOpen, filename, opt_read))
		{
			std::cerr << "Cannot Open mesh to file '1.obj'" << std::endl;
			//return 1;
		}
		meshOpen.request_vertex_normals();
		OpenMesh::IO::Options opt;
		if (!opt.check(OpenMesh::IO::Options::VertexNormal))
		{
			// we need face normals to update the vertex normals  
			meshOpen.request_face_normals();

			// let the mesh update the normals  
			meshOpen.update_normals();

			// dispose the face normals, as we don't need them anymore  
			meshOpen.release_face_normals();
		}

	}

	/*void upDateMesh()
	{
		for (auto it = meshOpen.vertices_begin(); it != meshOpen.vertices_end(); ++it)
		{
			int i = 0;
			glm::vec3 delta;
			auto point = meshOpen.point(*it);
			auto is_boundary = meshOpen.is_boundary(*it);
			if (is_boundary)
			{
				delta = glm::vec3(0,0,0);
			}
			else
			{
				int x = 0, y = 0, z = 0;
				for (auto vv_it = meshOpen.vv_iter(*it); vv_it.is_valid(); vv_it++)
				{
					auto point2 = meshOpen.point(*vv_it);
					x += point2[0];
					y += point2[1];
					z += point2[2];
				}
				x /= 3;
				y /= 3;
				z /= 3;

				delta.x = x / 3 - point[0];
				delta.y = y / 3 - point[1];
				delta.z = z / 3 - point[2];
			}

			i++;
			meshes[0].vertices[i].delta = delta;
		}
	}*/


	void loadModel(string path)
	{
		// Read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
		ReadMesh(path);

		// Check for errors
		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		// Retrieve the directory path of the filepath
		this->directory = path.substr(0, path.find_last_of('/'));

		// Process ASSIMP's root node recursively
		this->processNode(scene->mRootNode, scene);
	}


	// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene)
	{
		// Process each mesh located at the current node
		for (GLuint i = 0; i < node->mNumMeshes; i++)
		{
			// The node object only contains indices to index the actual objects in the scene. 
			// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			this->meshes.push_back(this->processMesh(mesh, scene));
			upDateMesh();
		}
		// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (GLuint i = 0; i < node->mNumChildren; i++)
		{
			this->processNode(node->mChildren[i], scene);
		}

	}

	Mesh processMesh(aiMesh* meshAi, const aiScene* scene)
	{

		// Data to fill
		vector<Vertex> vertices;
		vector<GLuint> indices;
		vector<Texture> textures;
		for (auto it = meshOpen.vertices_begin(); it != meshOpen.vertices_end(); ++it)
		{
			auto point = meshOpen.point(*it);
			auto normal = meshOpen.normal(*it);
			auto texcoord = meshOpen.texcoord2D(*it);
			Vertex vertex;
			glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
							  // Positions
			vector.x = point[0];
			vector.y = point[1];
			vector.z = point[2];
			vertex.Position = vector;

			// Normals
			vector.x = normal[0];
			vector.y = normal[1];
			vector.z = normal[2];
			vertex.Normal = vector;

			glm::vec3 delta;
			//auto point = meshOpen.point(*it);

			float x = 0, y = 0, z = 0;
			int nd = 0;
			for (auto vv_it = meshOpen.vv_iter(*it); vv_it.is_valid(); vv_it++)
			{
				nd++;
				auto point2 = meshOpen.point(*vv_it);
				x += point2[0];
				y += point2[1];
				z += point2[2];
			}
			x /= nd;
			y /= nd;
			z /= nd;

			delta.x = point[0] - x;
			delta.y = point[1] - y;
			delta.z = point[2] - z;
			vertex.delta = delta;

			vector.x = 0.1;
			vector.y = 0.1;
			vector.z = 0.1;
			vertex.weight = vector;


			glm::vec2 vec;
			vec.x = texcoord[0];
			vec.y = texcoord[1];
			vertex.TexCoords = vec;
			vertices.push_back(vertex);
			//	glTexCoord2fv(texcoord.data());
			//	glNormal3fv(normal.data());
			//	glVertex3fv(point.data());
		}
		// Walk through each of the mesh's vertices
		//for (GLuint i = 0; i < meshAi->mNumVertices; i++)
		//{
		//	Vertex vertex;
		//	glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		//					  // Positions
		//	vector.x = meshAi->mVertices[i].x;
		//	vector.y = meshAi->mVertices[i].y;
		//	vector.z = meshAi->mVertices[i].z;
		//	
		//	vertex.Position = vector;
		//	// Normals
		//	vector.x = meshAi->mNormals[i].x;
		//	vector.y = meshAi->mNormals[i].y;
		//	vector.z = meshAi->mNormals[i].z;
		//	//vector = glm::vec3(1.0f, 0.0f, 0.0f);
		//	vertex.Normal = vector;
		//	// Texture Coordinates
		//	if (meshAi->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		//	{
		//		glm::vec2 vec;
		//		// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
		//		// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
		//		vec.x = meshAi->mTextureCoords[0][i].x;
		//		vec.y = meshAi->mTextureCoords[0][i].y;
		//		vertex.TexCoords = vec;
		//	}
		//	else
		//		vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		//	vertices.push_back(vertex);
		//}
		// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (auto it1 = meshOpen.faces_begin(); it1 != meshOpen.faces_end(); ++it1)
		{
			auto cfv_it = meshOpen.cfv_iter(*it1);
			int i0 = cfv_it->idx();
			++cfv_it;
			int i1 = cfv_it->idx();
			++cfv_it;
			int i2 = cfv_it->idx();

			indices.push_back(i0);
			indices.push_back(i1);
			indices.push_back(i2);

		}
		//for (GLuint i = 0; i < meshAi->mNumFaces; i++)
		//{
		//	aiFace face = meshAi->mFaces[i];
		//	// Retrieve all indices of the face and store them in the indices vector
		//	for (GLuint j = 0; j < face.mNumIndices; j++)
		//		indices.push_back(face.mIndices[j]);
		//}
		// Process materials
		if (meshAi->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[meshAi->mMaterialIndex];
			// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
			// Same applies to other texture as the following list summarizes:
			// Diffuse: texture_diffuseN
			// Specular: texture_specularN
			// Normal: texture_normalN

			// 1. Diffuse maps
			vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. Specular maps
			vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		// Return a mesh object created from the extracted mesh data
		return Mesh(vertices, indices, textures);
	}

	// Checks all material textures of a given type and loads the textures if they're not loaded yet.
	// The required info is returned as a Texture struct.
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
	{
		vector<Texture> textures;
		for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			GLboolean skip = false;
			for (GLuint j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{   // If texture hasn't been loaded already, load it
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(), this->directory);
				texture.type = typeName;
				texture.path = str;
				textures.push_back(texture);
				this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}
		}
		return textures;
	}
};




GLint TextureFromFile(const char* path, string directory)
{
	//Generate texture ID and load texture data 
	string filename = string(path);
	filename = directory + '/' + filename;
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}