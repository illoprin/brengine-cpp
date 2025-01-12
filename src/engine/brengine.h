#ifndef BRENGINE_3D
#define BRENGINE_3D

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION

#include "util/deps.h"

// Mesh
#include "mesh/base_mesh.h"
#include "mesh/simple_mesh.h"

// Model
#include "model/model.h"

// Level
#include "level/level.h"

// Core
#include "core/camera.h"
#include "core/engine.h"
#include "core/default_assets.h"

// Entity
#include "entity/transform.h"
#include "entity/entity.h"

// GUI
#include "gui/gui_item.h"
#include "gui/gui_image.h"
#include "gui/gui_text.h"
#include "gui/gui_scene.h"
#include "gui/gui_font.h"
#include "gui/debug_gui.h"

// Render
#include "render/master_renderer.h"
#include "render/program.h"

// Renderer types
#include "render/level_renderer.h"
#include "render/entity_renderer.h"
#include "render/gui_renderer.h"

// Texture
#include "texture/texture.h"
#include "texture/texture_model.h"
#include "texture/texture_storage.h"

// Utils
#include "util/utils.h"

// Game
#include "player/editor_controller.h"
#include "game/game_handler.h"
#include "level/level.h"

#endif