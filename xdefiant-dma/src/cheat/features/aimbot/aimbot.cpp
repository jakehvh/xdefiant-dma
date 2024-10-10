#include "aimbot.hpp"

float aimbot::calc_distance( math::vec2 pos )
{
	math::vec2 screen_center{ sdk::ctx::screen_size.x / 2.f, sdk::ctx::screen_size.y / 2.f };
	return std::sqrtf( std::powf( ( pos.x - screen_center.x ), 2.f ) + std::powf( ( pos.y - screen_center.y ), 2.f ) );
}

actor_cache::f_agent aimbot::get_target( bool cache_target )
{
	actor_cache::f_agent best_target{ };
	float best_distance = FLT_MAX;
	float best_pdistance = FLT_MAX;

	std::vector<actor_cache::f_agent> possible_targets { };

	for ( auto& agent : actor_cache::agents )
	{
		if ( agent.addr == sdk::ctx::local_player || agent.current_health <= 0.f || agent.team_id == sdk::ctx::local_team_id )
			continue;

		auto p_dist = sdk::ctx::local_location.dist_to( agent.location );
		if ( p_dist > menu::vis::player::max_dist.value )
			continue;

		auto hitbox = agent.pelvis_pos;
		math::vec2 hitbox_screen{ };

		if ( !sdk::ctx::view.world_to_screen( hitbox, &hitbox_screen ) )
			continue;

		auto dist = calc_distance( hitbox_screen );
		if ( cache_target )
		{
			if ( agent.addr == cached_target.addr && dist <= menu::aim::fov.value )
			{
				best_target = agent;
				return best_target;
			}
		}

		if ( menu::aim::prioritize_dist.value && dist <= menu::aim::fov.value )
			possible_targets.push_back( agent );

		if ( !menu::aim::prioritize_dist.value )
		{
			if ( dist < best_distance && dist <= menu::aim::fov.value )
			{
				best_target = agent;
				best_distance = dist;
			}
		}
	}

	if ( menu::aim::prioritize_dist.value )
	{
		for ( auto& agent : possible_targets )
		{
			auto p_dist = sdk::ctx::local_location.dist_to( agent.location );
			if ( p_dist < best_pdistance )
			{
				best_target = agent;
				best_pdistance = p_dist;
			}
		}
	}

	if ( cache_target )
		cached_target = best_target;

	return best_target;
}

math::vec3 aimbot::get_best_bone_position( actor_cache::f_agent target )
{
	std::vector<math::vec3>bone_positons{ };

	if ( menu::aim::hitboxes.value[ 0 ] )
		bone_positons.push_back( target.head_pos );

	if ( menu::aim::hitboxes.value[ 1 ] )
		bone_positons.push_back( target.neck_pos );

	if ( menu::aim::hitboxes.value[ 2 ] )
	{
		bone_positons.push_back( target.upper_spine_pos );
		bone_positons.push_back( target.lower_spine_pos );
	}

	if ( menu::aim::hitboxes.value[ 3 ] )
		bone_positons.push_back( target.pelvis_pos );

	if ( menu::aim::hitboxes.value[ 4 ] )
	{
		bone_positons.push_back( target.left_shoulder_pos );
		bone_positons.push_back( target.left_elbow_pos );
		bone_positons.push_back( target.right_shoulder_pos );
		bone_positons.push_back( target.right_elbow_pos );
	}

	if ( menu::aim::hitboxes.value[ 5 ] )
	{
		bone_positons.push_back( target.left_hand_pos );
		bone_positons.push_back( target.right_hand_pos );
	}

	if ( menu::aim::hitboxes.value[ 6 ] )
	{
		bone_positons.push_back( target.left_thigh_pos );
		bone_positons.push_back( target.left_calf_pos );
		bone_positons.push_back( target.right_thigh_pos );
		bone_positons.push_back( target.right_calf_pos );
	}

	if ( menu::aim::hitboxes.value[ 7 ] )
	{
		bone_positons.push_back( target.left_foot_pos );
		bone_positons.push_back( target.right_foot_pos );
	}

	if ( bone_positons.empty( ) )
		return target.head_pos;

	math::vec3 best_bone_position{ };
	float best_distance = FLT_MAX;

	for ( auto& bone_position : bone_positons )
	{
		math::vec2 bone_screen{ };
		if ( !sdk::ctx::view.world_to_screen( bone_position, &bone_screen ) )
			continue;

		auto dist = calc_distance( bone_screen );
		if ( dist < best_distance )
		{
			best_bone_position = bone_position;
			best_distance = dist;
		}
	}

	return best_bone_position;
}

//uintptr_t aimbot::get_crosshair_target( )
//{
//	auto entity_component = utils::read<uintptr_t>( aimbot::crosshair + 0x88 );
//	auto entity_ptr = utils::read<uintptr_t>( entity_component + 0x30 );
//
//	std::uintptr_t crosshair_target = NULL;
//	for ( const auto& agent : actor_cache::soldiers )
//	{
//		if ( agent.addr == entity_ptr && !(agent.current_health <= 0) && agent.addr != sdk::ctx::local_player && agent.team_id != sdk::ctx::local_team_id )
//		{
//			crosshair_target = agent.addr;
//			break;
//		}
//	}
//
//	return crosshair_target;
//}

constexpr int map_and_clamp( float value, float min_from, float max_from, int min_to, int max_to )
{
	return std::clamp( static_cast< int >( ( value - min_from ) / ( max_from - min_from ) * ( max_to - min_to ) + min_to ), min_to, max_to );
}

bool aimbot::run( )
{
	if ( !menu::aim::enabled.value )
		return true;

	math::vec2 screen_center{ sdk::ctx::screen_size.x / 2.f, sdk::ctx::screen_size.y / 2.f };

	if ( menu::aim::draw_fov.value )
		draw_cmds->circle( screen_center, menu::aim::fov.value, menu::aim::fov_col.value, 200, 2.f );

	if ( menu::aim::draw_deadzone.value )
		draw_cmds->circle( screen_center, menu::aim::deadzone.value, menu::aim::deadzone_col.value, 200, 2.f );

	if ( !utils::key( menu::aim::key.key ).is_held( ) )
	{
		cached_target.addr = 0;
		sdk::ctx::aim_target = 0;
		sdk::ctx::aim_pos = { 0, 0 };
		return true;
	}

	auto target = get_target( menu::aim::cache_target.value );
	if ( !target.addr )
		return true;

	sdk::ctx::aim_target_location = target.location;

	auto position = get_best_bone_position( target );
	math::vec2 screen { };

	aim_pos_world = position;

	if ( !sdk::ctx::view.world_to_screen( position, &screen ) )
		return true;

	auto dist = calc_distance( screen );
	if ( dist < menu::aim::deadzone.value )
		return true;

	if ( menu::aim::aim_line.value )
		draw_cmds->line( screen_center, screen, menu::aim::aim_line_col.value, 2.f );

	sdk::ctx::aim_target = target.addr;
	sdk::ctx::aim_pos = screen;

	math::vec2 delta = screen - screen_center;
	math::vec2 smoothed { };

	smoothed.x = delta.x / map_and_clamp( menu::aim::smooth_x.value, 0, 100, 1, 9 );
	smoothed.y = delta.y / map_and_clamp( menu::aim::smooth_y.value, 0, 100, 1, 9 );

	if ( !menu::aim::psilent.value )
	{
		auto time_ms = std::chrono::duration_cast< std::chrono::milliseconds >( std::chrono::system_clock::now( ).time_since_epoch( ) ).count( );
		if ( time_ms > time_ms_old )
		{
			utils::kmb.move_mouse( smoothed.x, smoothed.y );
			time_ms_old = time_ms + static_cast< long long >( menu::cfg::kmb_rate_limit.value );
		}
	}

	static std::once_flag spawn_psilent_thread { };
	std::call_once( spawn_psilent_thread, [ ]( )
	{
		std::thread( [&]( )
		{
			while ( true )
			{
				if ( !menu::aim::psilent.value )
					continue;

				if ( sdk::ctx::aim_target )
					utils::write<math::vec3>( sdk::ctx::crosshair + 0x58, aim_pos_world );
			}
		} ).detach( );
	} );

	return true;
}

bool aimbot::run_triggerbot( )
{
	if ( !menu::aim::trigger_enabled.value )
		return true;

	if ( !sdk::ctx::has_kmbox )
		return true;

	if ( !utils::key( menu::aim::trigger_key.key ).is_held( ) )
		return true;

	if ( !sdk::ctx::crosshair_target )
		return true;

	if ( utils::kmb.is_kmnet( ) )
	{
		utils::kmb.left_click( menu::aim::trigger_delay.value );
	}
	else
	{
		auto time_ms = std::chrono::duration_cast< std::chrono::milliseconds >( std::chrono::system_clock::now( ).time_since_epoch( ) ).count( );
		if ( time_ms > time_ms_old )
		{
			utils::kmb.left_click( menu::aim::trigger_delay.value );
			time_ms_old = time_ms + static_cast< long long >( menu::cfg::kmb_rate_limit.value );
		}
	}

	return true;
}