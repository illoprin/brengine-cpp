#include "level_mesh.h"

/*
	Indicate vertices of quads in CCW starting from left bottom
*/
static void TriangulateWall(
	glm::vec3 v1,
	glm::vec3 v2,
	glm::vec3 v3,
	glm::vec3 v4,
	glm::vec3 nq,
	Triangle& t1,
	Triangle& t2,
	float wl, // Wall length
	float wh // Wall height
)
{
	v1.x *= LVL_SCALING; v1.z *= LVL_SCALING;
	v2.x *= LVL_SCALING; v2.z *= LVL_SCALING;
	v3.x *= LVL_SCALING; v3.z *= LVL_SCALING;
	v4.x *= LVL_SCALING; v4.z *= LVL_SCALING;

	float _wl = wl * LVL_SCALING;
	float _wh = wh * LVL_SCALING;
	
	// Left top triangle
	t1 = {
		{
			{v1.x, v1.y, v1.z, 0, 0, nq.x, nq.y, nq.z},
			{v4.x, v4.y, v4.z, 0, _wh, nq.x, nq.y, nq.z},
			{v3.x, v3.y, v3.z, _wl, _wh, nq.x, nq.y, nq.z},
		}
	};
	// Right bottom triangle
	t2 = {
		{
			{v1.x, v1.y, v1.z, 0, 0, nq.x, nq.y, nq.z},
			{v3.x, v3.y, v3.z, _wl, _wh, nq.x, nq.y, nq.z},
			{v2.x, v2.y, v2.z, _wl, 0, nq.x, nq.y, nq.z},
		}
	};
};

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
	uint16_t tid,          // Texture id
	uint8_t ttype,         // Texture type
	float glowi,           // Glow intensity
	std::vector<b_Level::LevelVertex>& vl
)
{

};

static void bl_TriangulateFloor(
	std::vector<glm::vec2>& sv,
	const b_Level::Sector& s,
	uint16_t tid,          // Texture id
	uint8_t ttype,         // Texture type
	float glowi,           // Glow intensity
	std::vector<b_Level::LevelVertex>& vl
)
{
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

		
		Triangle f{
			{
				{fv3.x, fv3.y, fv3.z, tc[2].x, tc[2].y, 0, 1, 0},
				{fv2.x, fv2.y, fv2.z, tc[1].x, tc[1].y, 0, 1, 0},
				{fv1.x, fv1.y, fv1.z, tc[0].x, tc[0].y, 0, 1, 0},
			}
		};
		tris.push_back(f);
	}
	// 4 Vertices
	if (sv.size() == 4)
	{
		Triangle t_flt, t_frb; // Floor tris

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

		TriangulateFloor(fv1, fv2, fv3, fv4, fn,
			t_flt, t_frb,
			tc[0], tc[1], tc[2], tc[3]);

		tris.push_back(t_flt);
		tris.push_back(t_frb);
	}

};

static void bl_TriangulateCeiling(
	std::vector<glm::vec2>& sv,
	const b_Level::Sector& s,
	uint16_t tid,          // Texture id
	uint8_t ttype,         // Texture type
	float glowi,           // Glow intensity
	std::vector<b_Level::LevelVertex>& vl
)
{

};

static void bl_BuildAppearanceData (
	const b_Game::LevelAppearanceData& ap,
	uint16_t& tid, uint8_t& ttype, float& gi
)
{
	if (ap.icolor == 0) 
	{
		tid = ap.itexture;
		ttype = b_Level::BL_TEXTURE_FLAT;
	}
	else
	{
		tid = ap.icolor;
		ttype = b_Level::BL_TEXTURE_COLOR;
	};
	gi = ap.glow_intensity;
	if (gi > 0.f) ttype = b_Level::BL_GLOWING;

};


static void TriangulateFloor(
	glm::vec3 v1,
	glm::vec3 v2,
	glm::vec3 v3,
	glm::vec3 v4,
	glm::vec3 nq,
	Triangle& t1,
	Triangle& t2,
	glm::vec2& vt1,
	glm::vec2& vt2,
	glm::vec2& vt3,
	glm::vec2& vt4
)
{
	v1.x *= LVL_SCALING; v1.z *= LVL_SCALING;
	v2.x *= LVL_SCALING; v2.z *= LVL_SCALING;
	v3.x *= LVL_SCALING; v3.z *= LVL_SCALING;
	v4.x *= LVL_SCALING; v4.z *= LVL_SCALING;

	
	// Left top triangle
	t1 = {
		{
			{v1.x, v1.y, v1.z, vt1.x, vt1.y, nq.x, nq.y, nq.z},
			{v4.x, v4.y, v4.z, vt4.x, vt4.y, nq.x, nq.y, nq.z},
			{v3.x, v3.y, v3.z, vt3.x, vt3.y, nq.x, nq.y, nq.z},
		}
	};
	// Right bottom triangle
	t2 = {
		{
			{v1.x, v1.y, v1.z, vt1.x, vt1.y, nq.x, nq.y, nq.z},
			{v3.x, v3.y, v3.z, vt3.x, vt3.y, nq.x, nq.y, nq.z},
			{v2.x, v2.y, v2.z, vt2.x, vt2.y, nq.x, nq.y, nq.z},
		}
	};
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
	for (auto& pair : ld.sectors)
	{
		const b_Level::Sector& sector = pair.second;
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
				if (bl_isUniqueVec2InList(v1, sv)) sv.push_back(v2);

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
				uint16_t tid; uint8_t ttype; float gi;
				if (w.iappearance != 0)
				{
					b_Game::LevelAppearanceData& wa =
						gd.lvl_appearance.at(w.iappearance);
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
					b_Level::Sector& p_sector = ld.sectors.at(w.portal);

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
			uint16_t tid = 0;
			uint8_t ttype = BL_TEXTURE_NULL;
			float gi = 0.f;

			// Triangulate floor and push it to list
			// Floor is in clockwise order
			// Build appearance of floor
			if (sector.iafloor != 0)
			{
				const b_Game::LevelAppearanceData& sa =
					gd.lvl_appearance.at(sector.iafloor);
				bl_BuildAppearanceData(sa, tid, ttype, gi);
			}
			bl_TriangulateFloor(sv, sector, tid, ttype, gi, vl);

			// Triangulate ceiling and push it to list
			// Ceiling is in counter-clockwise order
			// Build appearance of ceiling
			if (sector.iaceil != 0)
			{
				const b_Game::LevelAppearanceData& sa =
					gd.lvl_appearance.at(sector.iafloor);
				bl_BuildAppearanceData(sa, tid, ttype, gi);
			};
			bl_TriangulateCeiling(sv, sector, tid, ttype, gi, vl);
			
		}

	}
	printf("Level %s total face count is: %ld\n", ld.name.c_str(), vl.size() / 3);
};