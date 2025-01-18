#pragma once

// Player
#define PLAYER_SPEED 3.f
#define PLAYER_SPEED_MODIFER 1.78f // Sprint ratio

/* 	Enum design:
 *		PL - name of entity
 *		M - type of action (movent current ex)
 *		FORWARD - direction
 */
enum PlayerActionList {
	PL_M_FORWARD = 0,
	PL_M_BACKWARD,
	PL_M_LEFT,
	PL_M_RIGHT,
	PL_M_MODIFY,
};