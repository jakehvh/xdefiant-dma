#pragma once

#include "game/game.hpp"
#include "../utils/dependencies/auth/auth.hpp"

namespace sdk::ctx
{
	inline uintptr_t process_base;
	inline math::vec2 screen_size{ };

	inline bool has_kmbox = true;

	inline uintptr_t world{ };
	inline uintptr_t view_data{ };
	inline sdk::c_view view{ };
	inline math::vec3 camera_pos{ };
	inline uintptr_t local_player{ };
	inline uintptr_t local_skill_script_vars{ };
	inline math::vec3 local_location{ };
	inline uintptr_t crosshair_unk1 { };
	inline uintptr_t weapon_data { };
	inline uintptr_t weapon_data_2 { };
	inline uintptr_t local_weapon_mag_size { };
	inline uintptr_t crosshair_entity{ };
	inline uintptr_t crosshair_target;
	inline uintptr_t crosshair;
	inline int local_team_id{ };

	inline uintptr_t aim_target { };
	inline math::vec3 aim_target_location{ };
	inline math::vec2 aim_pos{ };

	namespace attributes
	{
		inline uintptr_t recoil_pattern;
		inline uintptr_t recoil_pattern_horizontal;
		inline uintptr_t recoil_pattern_vertical;
		inline uintptr_t aim_sway_speed;
	}
}