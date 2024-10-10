#pragma once

#include "../../menu/menu.hpp"
#include "../actor_cache/actor_cache.hpp"

namespace aimbot
{
	inline float calc_distance( math::vec2 pos );
	inline actor_cache::f_agent get_target( bool is_triggerbot = false );
	inline math::vec3 get_best_bone_position( actor_cache::f_agent target );
	//inline uintptr_t get_crosshair_target( );

	bool run( );
	bool run_triggerbot( );

	inline actor_cache::f_agent cached_target{ };
	inline math::vec3 aim_pos_world{};
	inline long long time_ms_old{};
}