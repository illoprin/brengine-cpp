#ifndef B_ASSETS
#define B_ASSETS

#include <model/model.h>
#include <texture/texture_model.h>
#include <mesh/base_mesh.h>
#include <mesh/simple_mesh.h>
#include <gui/gui_font.h>

namespace b_AssetManager
{
	void InitAssets();
	b_Texture::TextureModel* getTextureNull();
	b_Texture::TextureModel* getTextureNullAlpha();
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