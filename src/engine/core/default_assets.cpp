#include "default_assets.h"

#include "engine.h"

using namespace b_AssetManager;
namespace b_GuiFont = b_GUI::b_Font;

b_Texture::TextureModel* TextureNull;
b_Texture::TextureModel* TextureNullAlpha;
BaseMesh* MeshBasicQuad;
SimpleMesh* MeshQuad;
SimpleMesh* MeshCube;

b_GuiFont::Font* FontSans;
b_GuiFont::Font* FontMono;
b_GuiFont::Font* FontGame;

static float quad_data[] = {
	1.f, -1.f, 1.f, 0.f,    // Left bottom
	-1.f, -1.f, 0.f, 0.f,   // Right bottom
	1.f, 1.f, 1.f, 1.f, 	// Left top
	-1.f, -1.f, 0.f, 0.f, 	// Right bottom
	-1.f, 1.f, 0.f, 1.f, 	// Right top
	1.f, 1.f, 1.f, 1.f, 	// Left top
};

static b_Model::ModelTriangles quad_model{QuadTriangles(1.f)};
static b_Model::ModelTriangles cube_model{CubeTriangles(1.f)};

void b_AssetManager::InitAssets()
{
	// --- Textures
	TextureNull = new b_Texture::TextureModel{};
	TextureNull->FromBMP("NULL");

	TextureNullAlpha = new b_Texture::TextureModel{};
	TextureNullAlpha->FromBMP("NULL_ALPHA");
	TextureNullAlpha->use_transperency_mask = 1;

	// --- Meshes
	GLuint lb;
	MeshBasicQuad = new BaseMesh{"AssetMesh_BasicQuad"};
	lb = MeshBasicQuad->AddBuffer(quad_data, 24 * sizeof(float));
	// 1. Attribute: in_position
	MeshBasicQuad->SetDataPointer(
		lb, 2, 4 * sizeof(float), 0);
	// 2. Attribute: in_texcoord
	MeshBasicQuad->SetDataPointer(
		lb, 2, 4 * sizeof(float), 2 * sizeof(float));
	MeshBasicQuad->setTotal(6); // 6 vertices

	MeshQuad = new SimpleMesh{"AssetMesh_Quad"};
	MeshQuad->initFromModel(&quad_model);

	MeshCube = new SimpleMesh{"AssetMesh_Cube"};
	MeshCube->initFromModel(&cube_model);

	// --- Fonts
	FontMono = new b_GuiFont::Font{"AssetFont_Mono"};
	FontMono->FromTTF("mono", 64, 512, 512);
	
	FontSans = new b_GuiFont::Font{"AssetFont_Sans"};
	FontSans->FromTTF("sans", 64, 1024, 1024);

	FontGame = new b_GuiFont::Font{"AssetFont_Game"};
	FontGame->FromTTF("game", 32, 512, 512);
};

void b_AssetManager::ReleaseAssets()
{
	delete TextureNull;
	delete TextureNullAlpha;

	delete MeshBasicQuad;
	delete MeshQuad;
	delete MeshCube;

	delete FontGame;
	delete FontMono;
	delete FontSans;
};

b_Texture::TextureModel* b_AssetManager::getTextureNull()
{
	return TextureNull;
};
b_Texture::TextureModel* b_AssetManager::getTextureNullAlpha()
{
	return TextureNullAlpha;
};

BaseMesh* b_AssetManager::getMeshBasicQuad()
{
	return MeshBasicQuad;
};
SimpleMesh* b_AssetManager::getMeshQuad()
{
	return MeshQuad;
};
SimpleMesh* b_AssetManager::getMeshCube()
{
	return MeshCube;
};

b_GuiFont::Font* b_AssetManager::getDefaultMonoFont()
{
	return FontMono;
};
b_GuiFont::Font* b_AssetManager::getDefaultGameFont()
{
	return FontGame;
};
b_GuiFont::Font* b_AssetManager::getDefaultSansFont()
{
	return FontSans;
};


b_Model::ModelTriangles b_AssetManager::QuadTriangles(float size)
{
	std::vector<b_Model::Triangle> tris{
		{
			{
				{size, -size, 0.f, 1.f, 0.f, 0.f, 0.f, -1.f},
				{-size, -size, 0.f, 0.f, 0.f, 0.f, 0.f, -1.f},
				{size, size, 0.f, 1.f, 1.f, 0.f, 0.f, -1.f}
			}
		},
		{
			{
				{-size, -size, 0.f, 0.f, 0.f, 0.f, 0.f, -1.f},
				{-size, size, 0.f, 0.f, 1.f, 0.f, 0.f, -1.f},
				{size, size, 0.f, 1.f, 1.f, 0.f, 0.f, -1.f}
			}
		},
	};
	return tris;
};

b_Model::ModelTriangles b_AssetManager::CubeTriangles(float size)
{
	std::vector<b_Model::Triangle> tris(12);
	Triangle front_1{
		{
			{0.5f, 0.5f, -.5f, 1.f, 1.f, 0.f, 0.f, -1.f},
			{0.5f, -0.5f, -.5f, 1.f, 0.f, 0.f, 0.f, -1.f},
			{-0.5f, -0.5f, -.5f, 0.f, 0.f, 0.f, 0.f, -1.f}
		},
	};
	Triangle front_2{
		{
			{-0.5f, -0.5f, -.5f, 0.f, 0.f, 0.f, 0.f, -1.f},
			{-0.5f, 0.5f, -.5f, 0.f, 1.f, 0.f, 0.f, -1.f},
			{0.5f, 0.5f, -.5f, 1.f, 1.f, 0.f, 0.f, -1.f}
		},
	};

	Triangle back_1{
		{
			{-0.5f, 0.5f, 0.5f, 1.f, 1.f, 0.f, 0.f, 1.f},
			{-0.5f, -0.5f, 0.5f, 1.f, 0.f, 0.f, 0.f, 1.f},
			{0.5f, -0.5f, 0.5f, 0.f, 0.f, 0.f, 0.f, 1.f}			
		}
	};
	Triangle back_2{
		{
			{0.5f, -0.5f, 0.5f, 0.f, 0.f, 0.f, 0.f, 1.f},
			{0.5f, 0.5f, 0.5f, 0.f, 1.f, 0.f, 0.f, 1.f},
			{-0.5f, 0.5f, 0.5f, 1.f, 1.f, 0.f, 0.f, 1.f}
		}
	};
	Triangle top_1{
		{
			{-0.5f, 0.5f, 0.5f, 1.f, 1.f, 0.f, 1.f, 0.f},
			{0.5f, 0.5f, 0.5f, 1.f, 0.f, 0.f, 1.f, 0.f},
			{0.5f, 0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f, 0.f}			
		}
	};
	Triangle top_2{
		{
			{0.5f, 0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f, 0.f},
			{-0.5f, 0.5f, -0.5f, 0.f, 1.f, 0.f, 1.f, 0.f},
			{-0.5f, 0.5f, 0.5f, 1.f, 1.f, 0.f, 1.f, 0.f}
		}
	};
	Triangle bottom_1{
		{
			{-0.5f, -0.5f, -0.5f, 1.f, 1.f, 0.f, -1.f, 0.f},
			{0.5f, -0.5f, -0.5f, 1.f, 0.f, 0.f, -1.f, 0.f},
			{0.5f, -0.5f, 0.5f, 0.f, 0.f, 0.f, -1.f, 0.f}
		}
	};
	Triangle bottom_2{
		{
			{0.5f, -0.5f, 0.5f, 0.f, 0.f, 0.f, -1.f, 0.f},
			{-0.5f, -0.5f, 0.5f, 0.f, 1.f, 0.f, -1.f, 0.f},
			{-0.5f, -0.5f, -0.5f, 1.f, 1.f, 0.f, -1.f, 0.f}
		}
	};
	Triangle left_1{
		{
			{0.5f, -0.5f, -.5f, 1.f, 1.f, 1.f, 0.f, 0.f},
			{0.5f, 0.5f, -.5f, 1.f, 0.f, 1.f, 0.f, 0.f},
			{0.5f, 0.5f, .5f, 0.f, 0.f, 1.f, 0.f, 0.f}
		}
	};
	Triangle left_2{
		{
			{0.5f, 0.5f, .5f, 0.f, 0.f, 1.f, 0.f, 0.f},
			{0.5f, -0.5f, .5f, 0.f, 1.f, 1.f, 0.f, 0.f},
			{0.5f, -0.5f, -.5f, 1.f, 1.f, 1.f, 0.f, 0.f}
		}
	};
	Triangle right_1{
		{
			{-0.5f, -0.5f, -0.5f, 1.f, 1.f, -1.f, 0.f, 0.f},
			{-0.5f, -0.5f, 0.5f, 1.f, 0.f, -1.f, 0.f, 0.f},
			{-0.5f, 0.5f, 0.5f, 0.f, 0.f, -1.f, 0.f, 0.f}			
		}
	};
	Triangle right_2{
		{
			{-0.5f, 0.5f, 0.5f, 0.f, 0.f, -1.f, 0.f, 0.f},
			{-0.5f, 0.5f, -0.5f, 0.f, 1.f, -1.f, 0.f, 0.f},
			{-0.5f, -0.5f, -0.5f, 1.f, 1.f, -1.f, 0.f, 0.f}
		}
	};
	tris.push_back(front_1);
	tris.push_back(front_2);
	tris.push_back(back_1);
	tris.push_back(back_2);
	tris.push_back(top_1);
	tris.push_back(top_2);
	tris.push_back(bottom_1);
	tris.push_back(bottom_2);
	tris.push_back(left_1);
	tris.push_back(left_2);
	tris.push_back(right_1);
	tris.push_back(right_2);
	return tris;
};