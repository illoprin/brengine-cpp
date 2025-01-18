#include "level_model.h"

/*
	Indicate vertices of quads in CCW starting from left bottom
*/
static void bl_TriangulateWall(
	glm::vec3 v1,
	glm::vec3 v2,
	glm::vec3 v3,
	glm::vec3 v4,
	float wl,              // Wall length
	float wh,              // Wall height
	glm::vec3 nrm,         // Normal
	int tid,          // Texture id
	int ttype,         // Texture type
	float glowi,           // Glow intensity
	std::vector<b_Level::LevelVertex>& vl
)
{
	// Left top triangle
	b_Level::LevelVertex lv1{v1, {0, 0}, nrm, tid, ttype, glowi};
	b_Level::LevelVertex lv2{v4, {0, wh}, nrm, tid, ttype, glowi};
	b_Level::LevelVertex lv3{v3, {wl, wh}, nrm, tid, ttype, glowi};

	// Right top triangle
	b_Level::LevelVertex lv4{v1, {0, 0}, nrm, tid, ttype, glowi};
	b_Level::LevelVertex lv5{v3, {wl, wh}, nrm, tid, ttype, glowi};
	b_Level::LevelVertex lv6{v2, {wl, 0}, nrm, tid, ttype, glowi};
	
	// Push it to list
	vl.push_back(lv1); vl.push_back(lv2);
	vl.push_back(lv3); vl.push_back(lv4);
	vl.push_back(lv5); vl.push_back(lv6);
};


/*
	Vertices order is from left bottom in CCW
*/
static void bl_TriangulateFloor(
	std::vector<glm::vec2>& sv,
	const b_Level::Sector& s,
	int tid,          // Texture id
	int ttype,         // Texture type
	float glowi,           // Glow intensity
	std::vector<b_Level::LevelVertex>& vl
)
{
	// TODO: triangulate complex meshes

	// Floor is 3 vertices
	if (sv.size() == 3)
	{
		glm::vec3 fv1 {sv[0].x, s.floor_height, sv[0].y};
		glm::vec3 fv2 {sv[1].x, s.floor_height, sv[1].y};
		glm::vec3 fv3 {sv[2].x, s.floor_height, sv[2].y};

		// Calculate texcoords
		std::vector<glm::vec2> tc;
		for (glm::vec2& v : sv)
		{
			tc.push_back((v - sv[0]));
		}

		// Only one triangle
		b_Level::LevelVertex lv1{fv1, tc[0], {0, 1, 0}, tid, ttype, glowi};
		b_Level::LevelVertex lv2{fv2, tc[1], {0, 1, 0}, tid, ttype, glowi};
		b_Level::LevelVertex lv3{fv3, tc[2], {0, 1, 0}, tid, ttype, glowi};
		// Push vertices to list
		vl.push_back(lv1);
		vl.push_back(lv2);
		vl.push_back(lv3);
	}
	// Floor is 4 vertices
	if (sv.size() == 4)
	{
		glm::vec3 fv1 {sv[0].x, s.floor_height, sv[0].y};
		glm::vec3 fv2 {sv[1].x, s.floor_height, sv[1].y};
		glm::vec3 fv3 {sv[2].x, s.floor_height, sv[2].y};
		glm::vec3 fv4 {sv[3].x, s.floor_height, sv[3].y};

		glm::vec3 fn {0, 1, 0};
		glm::vec3 cn {0,-1, 0};

		// Calculate texcoords
		std::vector<glm::vec2> tc;
		for (glm::vec2& v : sv)
		{
			tc.push_back((v - sv[0]));
		}

		// Left top triangle
		b_Level::LevelVertex lv1{fv2, tc[1], {0, 1, 0}, tid, ttype, glowi};
		b_Level::LevelVertex lv2{fv4, tc[3], {0, 1, 0}, tid, ttype, glowi};
		b_Level::LevelVertex lv3{fv3, tc[2], {0, 1, 0}, tid, ttype, glowi};

		// Right bottom triangle
		b_Level::LevelVertex lv4{fv1, tc[0], {0, 1, 0}, tid, ttype, glowi};
		b_Level::LevelVertex lv5{fv2, tc[1], {0, 1, 0}, tid, ttype, glowi};
		b_Level::LevelVertex lv6{fv3, tc[2], {0, 1, 0}, tid, ttype, glowi};

		// Push it to list

		// Left top triangle
		vl.push_back(lv1);
		vl.push_back(lv2);
		vl.push_back(lv3);
		// Right bottom triangle
		vl.push_back(lv4);
		vl.push_back(lv5);
		vl.push_back(lv6);
	}

};

/*
	Vertices order is from left bottom in clockwise order
*/
static void bl_TriangulateCeiling(
	std::vector<glm::vec2>& sv,
	const b_Level::Sector& s,
	int tid,          // Texture id
	int ttype,         // Texture type
	float glowi,           // Glow intensity
	std::vector<b_Level::LevelVertex>& vl
)
{
	// TODO: triangulate complex meshes

	// Ceiling is 3 vertices
	if (sv.size() == 3)
	{
		glm::vec3 fv1 {sv[0].x, s.ceiling_height, sv[0].y};
		glm::vec3 fv2 {sv[1].x, s.ceiling_height, sv[1].y};
		glm::vec3 fv3 {sv[2].x, s.ceiling_height, sv[2].y};

		// Calculate texcoords
		std::vector<glm::vec2> tc;
		for (glm::vec2& v : sv)
		{
			tc.push_back((v - sv[0]));
		}

		// Only one triangle
		b_Level::LevelVertex lv1{fv3, tc[2], {0, -1, 0}, tid, ttype, glowi};
		b_Level::LevelVertex lv2{fv2, tc[1], {0, -1, 0}, tid, ttype, glowi};
		b_Level::LevelVertex lv3{fv1, tc[0], {0, -1, 0}, tid, ttype, glowi};
		// Push vertices to list
		vl.push_back(lv1);
		vl.push_back(lv2);
		vl.push_back(lv3);
	}
	// Ceiling is 4 vertices
	if (sv.size() == 4)
	{
		glm::vec3 fv1 {sv[0].x, s.ceiling_height, sv[0].y}; // 2
		glm::vec3 fv2 {sv[1].x, s.ceiling_height, sv[1].y}; // 1
		glm::vec3 fv3 {sv[2].x, s.ceiling_height, sv[2].y};
		glm::vec3 fv4 {sv[3].x, s.ceiling_height, sv[3].y};

		glm::vec3 fn {0, 1, 0};
		glm::vec3 cn {0,-1, 0};

		// Calculate texcoords
		std::vector<glm::vec2> tc;
		for (glm::vec2& v : sv)
		{
			tc.push_back((v - sv[0]));
		}

		// Left top triangle
		b_Level::LevelVertex lv1{fv2, tc[1], {0, -1, 0}, tid, ttype, glowi};
		b_Level::LevelVertex lv2{fv4, tc[3], {0, -1, 0}, tid, ttype, glowi};
		b_Level::LevelVertex lv3{fv3, tc[2], {0, -1, 0}, tid, ttype, glowi};

		// Right bottom triangle
		b_Level::LevelVertex lv4{fv1, tc[0], {0, -1, 0}, tid, ttype, glowi};
		b_Level::LevelVertex lv5{fv2, tc[1], {0, -1, 0}, tid, ttype, glowi};
		b_Level::LevelVertex lv6{fv3, tc[2], {0, -1, 0}, tid, ttype, glowi};

		// Push it to list

		// Left top triangle
		vl.push_back(lv3);
		vl.push_back(lv2);
		vl.push_back(lv1);
		// Right bottom triangle
		vl.push_back(lv6);
		vl.push_back(lv5);
		vl.push_back(lv4);
	}

};

static void bl_BuildAppearanceData (
	const b_Game::LevelAppearanceData& ap,
	int& tid, int& ttype, float& gi
)
{
	if (ap.icolor == 0) 
	{
		tid = ap.itexture - 1;
		ttype = b_Level::BL_TEXTURE_FLAT;
	}
	else
	{
		tid = ap.icolor - 1;
		ttype = b_Level::BL_TEXTURE_COLOR;
	};
	gi = ap.glow_intensity;
	if (gi > 0.f) ttype = b_Level::BL_GLOWING;
};

static bool bl_isUniqueVec2InList(glm::vec2& v, std::vector<glm::vec2>& l)
{
	if (l.size() > 0)
	{
		int comps = 0;
		for (unsigned i = 0; i < l.size(); i++)
		{
			glm::vec2& c = l[i];
			if (c == v) comps++;
		}
		return comps == 0;
	}
	return true;
};


void b_Level::LevelDataToVertices(
	b_Level::LevelData&       ld,
	b_Game::GameData&         gd,
	std::vector<LevelVertex>& vl
)
{
	for (b_Level::Sector& sector : ld.sectors)
	{
		if (sector.wall_num == 0)
			continue;
		else
		{
			std::vector<glm::vec2> sv; // Sector vertices
			for (unsigned i = 0; i < sector.wall_num; i++)
			{
				// Indicate vertices of quads in counter clockwise order
				// starting from left bottom

				b_Level::Wall& w = ld.walls[sector.wall_start + i];
				glm::vec2& v1 = ld.verts[w.v1];
				glm::vec2& v2 = ld.verts[w.v2];

				if (bl_isUniqueVec2InList(v1, sv)) sv.push_back(v1);
				if (bl_isUniqueVec2InList(v2, sv)) sv.push_back(v2);

				// Wall line
				glm::vec2 line = v2 - v1;
				// Get wall normal in 3D
				glm::vec3 normal = glm::cross(
					glm::vec3(0, 1, 0),
					glm::normalize(glm::vec3(line.x, 0, line.y))
				);
				// Wall height if not portal - abs differecne of ceiling height and floor height
				float h_delta;

				// Build wall appearance
				int tid = 0;
				int ttype = BL_TEXTURE_NULL;
				float gi = 0.f;

				if (w.iappearance != 0)
				{
					b_Game::LevelAppearanceData& wa =
						gd.lvl_appearance.at(w.iappearance - 1);
					bl_BuildAppearanceData(wa, tid, ttype, gi);
				}
				

				// If it is not portal - form solid wall
				if (w.portal == 0)
				{
					
					// Define wall quad vertices
					glm::vec3 qv1{ v1.x, sector.floor_height, v1.y };
					glm::vec3 qv2{ v2.x, sector.floor_height, v2.y };
					glm::vec3 qv3{ v2.x, sector.ceiling_height, v2.y };
					glm::vec3 qv4{ v1.x, sector.ceiling_height, v1.y };
					
					h_delta = fabs(sector.ceiling_height - sector.floor_height);

					// Triangulate wall and push to list 
					bl_TriangulateWall(
						qv1, qv2, qv3, qv4,
						glm::length(line), h_delta, normal,
						tid, ttype, gi, vl
					);
				}
				// If it is portal - form top and bottom quad triangles
				else
				{
					b_Level::Sector& p_sector = ld.sectors.at(w.portal - 1);

					// Form bottom triangles, if floor height of current sector
					// is less then floor height of next sector
					if (sector.floor_height < p_sector.floor_height)
					{
						glm::vec3 vqb1 = { v1.x, sector.floor_height, v1.y };
						glm::vec3 vqb2 = { v2.x, sector.floor_height, v2.y };
						glm::vec3 vqb3 = { v2.x, p_sector.floor_height, v2.y };
						glm::vec3 vqb4 = { v1.x, p_sector.floor_height, v1.y };

						h_delta = p_sector.floor_height - sector.floor_height;

						// Triangulate wall and push to list 
						bl_TriangulateWall(
							vqb1, vqb2, vqb3, vqb4,
							glm::length(line), h_delta, normal,
							tid, ttype, gi, vl
						);
					}
					// Form top triangles, if ceiling height of current sector
					// is bigger then ceiling height of next sector  
					if (sector.ceiling_height > p_sector.ceiling_height)
					{
						glm::vec3 vqt1 = { v1.x, p_sector.ceiling_height, v1.y };
						glm::vec3 vqt2 = { v2.x, p_sector.ceiling_height, v2.y };
						glm::vec3 vqt3 = { v2.x, sector.ceiling_height, v2.y };
						glm::vec3 vqt4 = { v1.x, sector.ceiling_height, v1.y };

						h_delta = sector.ceiling_height - p_sector.ceiling_height;

						// Triangulate wall and push to list 
						bl_TriangulateWall(
							vqt1, vqt2, vqt3, vqt4,
							glm::length(line), h_delta, normal,
							tid, ttype, gi, vl
						);
					}

				}
			}
			
			// Form floor and ceiling
			int tid = 0;
			int ttype = BL_TEXTURE_NULL;
			float gi = 0.f;

			// Triangulate floor and push it to list
			// Floor is in clockwise order
			// Build appearance of floor
			if (sector.iafloor != 0)
			{
				const b_Game::LevelAppearanceData& sa =
					gd.lvl_appearance.at(sector.iafloor - 1);
				bl_BuildAppearanceData(sa, tid, ttype, gi);
			}
			bl_TriangulateFloor(sv, sector, tid, ttype, gi, vl);

			// Triangulate ceiling and push it to list
			// Ceiling is in counter-clockwise order
			// Build appearance of ceiling
			if (sector.iaceil != 0)
			{
				const b_Game::LevelAppearanceData& sa =
					gd.lvl_appearance.at(sector.iaceil - 1);
				bl_BuildAppearanceData(sa, tid, ttype, gi);
			};
			bl_TriangulateCeiling(sv, sector, tid, ttype, gi, vl);
			
		}

	}
	LOG_MSG("Level.%s total face count is: %ld", ld.name.c_str(), vl.size() / 3);
};

void b_Level::printLevelVertices(
	const std::vector<b_Level::LevelVertex>& l
)
{
	printf("\n");
	
	for (unsigned i = 0; i < l.size(); ++i)
	{
		const b_Level::LevelVertex& v = l[i];
		printf("Vertex %u: vX: %.2f vY: %.2f vZ: %.2f U: %.2f V: %.2f nX: %.2f nY: %.2f nZ: %.2f tid: %d tt: %d gi: %.2f\n",
			i, v.position.x, v.position.y, v.position.z,
			v.texcoord.x, v.texcoord.y,
			v.normal.x, v.normal.y, v.normal.z,
			v.textureId, v.textureType, v.glow_intensity);
	};

	printf("\n");
};