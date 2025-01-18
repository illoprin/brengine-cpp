#include "game_handler.h"

using namespace b_Level;
using namespace b_Game;

GameHandler::GameHandler(std::string filename)
{
	// Load game data
	GameDataFromBGD(filename, game_data);
	
	game_data.print();

	// Building textures
	build_texture_storage();
	build_palette();
};

void GameHandler::installLevel(uint16_t index)
{
	std::string level_name{game_data.levels[index] + ".blf"};
	std::unique_ptr<b_Level::Level> lvl_ptr{
		new b_Level::Level{level_name, &game_data}
	};
	lvl_ptr->usePalette(&t_palette);
	lvl_ptr->useAtlas(&t_atlas);

	level_up = std::move(lvl_ptr);
};

void GameHandler::releaseLevel()
{
	if (this->level_up.get() != nullptr)
		this->level_up.reset();
};

void GameHandler::update()
{
	p_player.update();
	level_up->update();
};

// -- Getters
b_Level::Level* GameHandler::getCurrentLevel() const
{
	return level_up.get();
};

void GameHandler::build_texture_storage()
{
	int width, height, channels;
	
	// Init OpenGL texture object
	// All textures will be one size
	t_atlas.width = B_FLAT_TEXTURE_SIZE;
	t_atlas.height = B_FLAT_TEXTURE_SIZE;
	t_atlas.components = 3;
	t_atlas.layers = (GLsizei)game_data.textures.size();
	t_atlas.InitStorage(GL_RGB8);
	t_atlas.setFiltering(GL_NEAREST);
	t_atlas.setWrapping(GL_REPEAT);

	unsigned index = 0u;
	for(const std::string& file : game_data.textures)
	{
		std::string file_path{TEXTURE_FILE_PATH(file)};
		
		// Load bytes from file
		stbi_set_flip_vertically_on_load(1);
		unsigned char* bytes = stbi_load(
			file_path.c_str(), &width, &height, &channels, 0
		);
		stbi_set_flip_vertically_on_load(0);

		if (bytes != NULL && channels == 3)
		{
			// Load bytes to OpenGL
			t_atlas.setImagePointer(
				index, 0u, 0u, GL_RGB, GL_UNSIGNED_BYTE, bytes
			);

			LOG_MSG("Texture %u loaded to storage", index);
			++index;
			
		}
		else
		{
			LOG_WAR("Could not load texture with path %s\n", file_path.c_str());
		};

		// Release bytes
		if (bytes != NULL) stbi_image_free(bytes);
	};

	
};

void GameHandler::build_palette()
{
	// Init OpenGL texture object
	t_palette.width = B_PALETTE_TILE_SIZE;
	t_palette.height = B_PALETTE_TILE_SIZE;
	t_palette.layers = game_data.pallete.size();
	t_palette.components = 3;
	t_palette.InitStorage(GL_RGB8);
	t_palette.setFiltering(GL_NEAREST);
	t_palette.setWrapping(GL_REPEAT);

	uint16_t pw = B_PALETTE_TILE_SIZE;
	unsigned index = 0u;
	for (const glm::ivec3& color : game_data.pallete)
	{
		// Allocate memory for pixels
		unsigned char* pixels = (unsigned char*)malloc(pw * pw * 3);

		// Fill this pixel area with one color
		for (unsigned i = 0; i < pw * pw; ++i)
		{
			pixels[i * 3 + 0] = (uch)color.r;
			pixels[i * 3 + 1] = (uch)color.g;
			pixels[i * 3 + 2] = (uch)color.b;
		};

		// Load pixels to storage
		t_palette.setImagePointer(
			index, 0u, 0u, GL_RGB, GL_UNSIGNED_BYTE, (void*)pixels
		);
		++index;

		// Release pixels
		free(pixels);
	};
};

GameHandler::~GameHandler()
{
	releaseLevel();
};