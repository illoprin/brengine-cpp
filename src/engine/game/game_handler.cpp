#include "game_handler.h"

using namespace b_Game;

GameHandler::GameHandler(std::string filename)
{
	// Load game data
	GameDataFromBGD(filename, game_data);
	build_texture_storage();
	build_palette();
	game_data.print();
};

void GameHandler::installLevel(uint16_t index)
{
	std::string level_name = game_data.levels[index];
	std::unique_ptr<b_Level::Level> lvl_ptr{
		new b_Level::Level{level_name, &game_data}
	};

	level_up = std::move(lvl_ptr);
	level_up->usePalette(&t_palette);
	level_up->useAtlas(&t_atlas);
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

	GLsizei flat_size = B_FLAT_TEXTURE_SIZE;
	
	// Init OpenGL texture object
	// All textures will be one size
	t_atlas.width = flat_size;
	t_atlas.height = flat_size;
	t_atlas.components = 3;
	t_atlas.layers = (GLsizei)game_data.textures.size();
	t_atlas.InitStorage(GL_RGB8);

	unsigned index = 0u;
	for(const auto& pair : game_data.textures)
	{
		std::string file_path
			{fs::path(FS_ASSETS_PATH) / "textures" / (pair.second + ".bmp")};
		
		// Load bytes from file
		stbi_set_flip_vertically_on_load(1);
		unsigned char* bytes = stbi_load(
			file_path.c_str(), &width, &height, &channels, 0
		);
		stbi_set_flip_vertically_on_load(0);

		if (bytes != NULL)
		{
			// Load bytes to OpenGL
			t_atlas.setImagePointer(
				index, 0, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes
			);			
			
			// Release bytes
			stbi_image_free(bytes);
			printf(
				"GameHandler - Texture with path %s loaded, storage pointer index is %u\n", file_path.c_str(), index
			);
			++index;
		}
		else
		{
			printf("GameHandler - Could not load texture with path %s\n", file_path.c_str());
		};

	};

	t_atlas.setFiltering(GL_NEAREST);
	t_atlas.setWrapping(GL_REPEAT);
};

void GameHandler::build_palette()
{
	// Init OpenGL texture object
	t_palette.width = BGD_PALETTE_TILE_SIZE;
	t_palette.height = BGD_PALETTE_TILE_SIZE;
	t_palette.layers = game_data.palletes.size();
	t_palette.components = 3;
	t_palette.InitStorage(GL_RGB8);

	uint16_t pw = BGD_PALETTE_TILE_SIZE;
	unsigned index = 0u;
	for (const auto& pair : game_data.palletes)
	{
		// Get clamped color
		const glm::vec3& colc = pair.second;
		unsigned char r = (unsigned char)(colc.r * 255.f);
		unsigned char g = (unsigned char)(colc.g * 255.f);
		unsigned char b = (unsigned char)(colc.b * 255.f);

		// Allocate memory for pixels
		unsigned char* pixels = (unsigned char*)malloc(pw * pw * 3);

		// Fill this pixel area with one color
		for (unsigned i = 0; i < pw * pw; ++i)
		{
			pixels[i * 3 + 0] = r;
			pixels[i * 3 + 1] = g;
			pixels[i * 3 + 2] = b;
		};

		// Load pixels to storage
		t_palette.setImagePointer(
			index, 0, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels
		);
		++index;

		// Release pixels
		free(pixels);
	};

	t_palette.setFiltering(GL_NEAREST);
	t_palette.setWrapping(GL_REPEAT);
};

GameHandler::~GameHandler()
{
	releaseLevel();
};