#ifndef B_ASSETS
#define B_ASSETS

#include "utils/deps.h"
#include "mesh/model.h"
#include "render/texture.h"
#include "mesh/base_mesh.h"
#include "mesh/simple_mesh.h"
#include "ui/font.h"

namespace b_AssetManager
{
	void InitAssets(Log*);
	TextureImage2D* getTextureNull();
	TextureImage2D* getTextureNullAlpha();
	BaseMesh* getMeshBasicQuad();
	SimpleMesh* getMeshQuad();
	SimpleMesh* getMeshCube();
	b_GUI::b_Font::Font* getDefaultMonoFont();
	b_GUI::b_Font::Font* getDefaultGameFont();
	b_GUI::b_Font::Font* getDefaultSansFont();
	b_Model::ModelTriangles QuadTriangles(float size);
	b_Model::ModelTriangles CubeTriangles(float size);
	void ReleaseAssets();
};

#endif