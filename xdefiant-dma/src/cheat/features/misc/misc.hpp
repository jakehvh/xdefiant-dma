#pragma once

#include "../../menu/menu.hpp"
#include "../actor_cache/actor_cache.hpp"

namespace misc
{
	struct f_weapon_data
	{
	public:
		int reload_time_ms;
		int empty_clip_reload_time_ms;
		float zoom_in_duration;
		int rpm;
		int sprint_shoot_delay_ms;
		float spread_min;
		float spread_max;
		float crouch_spread_min;
		float crouch_spread_max;
		float crouch_slide_spread_min;
		float crouch_slide_spread_max;
		float in_air_spread_min;
		float in_air_spread_max;
		float movement_modifier;
		int bullets_per_round;
		int agent_bullet_penetration_count;
		float aiming_move_speed_modifier;
		float max_jump_height_aiming_factor;
		float sway_movement_speed;
	};

	inline std::unordered_map<uintptr_t, f_weapon_data> cached_weapon_data{ };

	bool run( );
}