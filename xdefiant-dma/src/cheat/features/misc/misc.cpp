#include "misc.hpp"

bool misc::run( )
{
	if ( !sdk::ctx::local_player )
		return true;

#ifndef RELEASE_EXE
	static long long time_s_old;
	auto time_s = std::chrono::duration_cast< std::chrono::seconds >( std::chrono::system_clock::now( ).time_since_epoch( ) ).count( );

	if ( time_s > time_s_old )
	{
		auto result = auth::send_heartbeat( auth::heart_beat_request );

		if ( result.status != auth::HEART_BEAT_OK )
			auth::heart_beat_thread_check = false;

		time_s_old = time_s + result.interval;
	}
#endif

	static std::unordered_map<int, float>viewmodel_fovs{ { 0, -0.f }, { 1, -1.f }, { 2, -6.f }, { 3, -5.f }, { 4, -11.f } };
	auto get_weapon_data = [ ]( )->f_weapon_data
	{
		auto pos = cached_weapon_data.find( sdk::ctx::weapon_data );
		if ( pos == cached_weapon_data.end( ) )
		{
			utils::scatter read{ };
			f_weapon_data weapon_data{ };

			read.push_back_read<float>( sdk::ctx::weapon_data + sdk::offsets::my_zoom_in_duration, &weapon_data.zoom_in_duration );
			read.push_back_read<int>( sdk::ctx::weapon_data + sdk::offsets::my_rpm, &weapon_data.rpm );
			read.push_back_read<int>( sdk::ctx::weapon_data + sdk::offsets::my_sprint_shoot_delay_ms, &weapon_data.sprint_shoot_delay_ms );
			read.push_back_read<int>( sdk::ctx::weapon_data + sdk::offsets::my_reload_time_ms, &weapon_data.reload_time_ms );
			read.push_back_read<int>( sdk::ctx::weapon_data + sdk::offsets::my_empty_clip_reload_time_ms, &weapon_data.empty_clip_reload_time_ms );
			read.push_back_read<float>( sdk::ctx::weapon_data + sdk::offsets::my_spread_min, &weapon_data.spread_min );
			read.push_back_read<float>( sdk::ctx::weapon_data + sdk::offsets::my_spread_max, &weapon_data.spread_max );
			read.push_back_read<float>( sdk::ctx::weapon_data + sdk::offsets::my_crouch_spread_min, &weapon_data.crouch_spread_min );
			read.push_back_read<float>( sdk::ctx::weapon_data + sdk::offsets::my_crouch_spread_max, &weapon_data.crouch_spread_max );
			read.push_back_read<float>( sdk::ctx::weapon_data + sdk::offsets::my_crouch_slide_spread_min, &weapon_data.crouch_slide_spread_min );
			read.push_back_read<float>( sdk::ctx::weapon_data + sdk::offsets::my_crouch_slide_spread_max, &weapon_data.crouch_slide_spread_max );
			read.push_back_read<float>( sdk::ctx::weapon_data + sdk::offsets::my_in_air_spread_min, &weapon_data.in_air_spread_min );
			read.push_back_read<float>( sdk::ctx::weapon_data + sdk::offsets::my_in_air_spread_max, &weapon_data.in_air_spread_max );
			read.push_back_read<float>( sdk::ctx::weapon_data + sdk::offsets::my_movement_modifier, &weapon_data.movement_modifier );
			read.push_back_read<float>( sdk::ctx::weapon_data + sdk::offsets::my_jump_max_height_aiming_factor, &weapon_data.max_jump_height_aiming_factor );
			read.push_back_read<int>( sdk::ctx::weapon_data_2 + sdk::offsets::my_bullets_per_round, &weapon_data.bullets_per_round );
			read.push_back_read<int>( sdk::ctx::weapon_data + sdk::offsets::my_agent_bullet_penetration_count, &weapon_data.agent_bullet_penetration_count );
			read.push_back_read<float>( sdk::ctx::weapon_data + sdk::offsets::my_aiming_move_speed_modifier, &weapon_data.aiming_move_speed_modifier );
			read.push_back_read<float>( sdk::ctx::weapon_data + sdk::offsets::my_sway_movement_speed, &weapon_data.sway_movement_speed );


			read.execute_read( );
			read.close( );

			cached_weapon_data.emplace( sdk::ctx::weapon_data, weapon_data );
		}
		else
			return pos->second;
	};

	auto old_weapon_data = get_weapon_data( );

	utils::scatter write{ };

	if ( !sdk::ctx::weapon_data || !sdk::ctx::weapon_data_2 )
		return true;

	if ( menu::misc::instant_swap.value )
	{
		write.push_back_write<float>( sdk::ctx::local_skill_script_vars + 0x158, 0.01f );
		write.push_back_write<float>( sdk::ctx::local_skill_script_vars + 0x168, 0.01f );
	}

	/*
	if ( menu::misc::no_recoil.value )
	{
		write.push_back_write<float>( sdk::ctx::attributes::recoil_pattern, 0.f );
		write.push_back_write<float>( sdk::ctx::attributes::recoil_pattern_horizontal, 0.f );
		write.push_back_write<float>( sdk::ctx::attributes::recoil_pattern_vertical, 0.f );
	}

	if ( menu::misc::no_sway.value )
		write.push_back_write<float>( sdk::ctx::attributes::aim_sway_speed, 0.f );
	*/

	bool instant_reload = menu::misc::instant_reload.value;
	bool instant_ads = menu::misc::instant_ads.value;
	bool mod_rpm = menu::misc::modify_rpm.value;
	bool instant_fire = menu::misc::instant_fire.value;
	bool no_spread = menu::misc::no_spread.value;
	bool double_jump = menu::misc::jump_height.value != 0;
	bool mod_bullet_count = menu::misc::modify_bullet_count.value;
	bool omni_sprint = menu::misc::omni_sprint.value;
	bool shoot_through_players = menu::misc::shoot_through_players.value;
	bool remove_penalty = menu::misc::remove_penalty.value;

	write.push_back_write<int>( sdk::ctx::weapon_data + sdk::offsets::my_reload_time_ms, instant_reload ? 1 : old_weapon_data.reload_time_ms );
	write.push_back_write<int>( sdk::ctx::weapon_data + sdk::offsets::my_empty_clip_reload_time_ms, instant_reload ? 1 : old_weapon_data.empty_clip_reload_time_ms );
	write.push_back_write<float>( sdk::ctx::weapon_data + sdk::offsets::my_zoom_in_duration, instant_ads ? 0.001f : old_weapon_data.zoom_in_duration );
	write.push_back_write<int>( sdk::ctx::weapon_data + sdk::offsets::my_rpm, mod_rpm ? menu::misc::rpm_value.value : old_weapon_data.rpm );
	write.push_back_write<int>( sdk::ctx::weapon_data + sdk::offsets::my_sprint_shoot_delay_ms, instant_fire ? 0 : old_weapon_data.sprint_shoot_delay_ms );
	write.push_back_write<float>( sdk::ctx::weapon_data + sdk::offsets::my_spread_min, no_spread ? 0.f : old_weapon_data.spread_min );
	write.push_back_write<float>( sdk::ctx::weapon_data + sdk::offsets::my_spread_max, no_spread ? 0.f : old_weapon_data.spread_max );
	write.push_back_write<float>( sdk::ctx::weapon_data + sdk::offsets::my_crouch_spread_min, no_spread ? 0.f : old_weapon_data.crouch_spread_min );
	write.push_back_write<float>( sdk::ctx::weapon_data + sdk::offsets::my_crouch_spread_max, no_spread ? 0.f : old_weapon_data.crouch_spread_max );
	write.push_back_write<float>( sdk::ctx::weapon_data + sdk::offsets::my_crouch_slide_spread_min, no_spread ? 0.f : old_weapon_data.crouch_slide_spread_min );
	write.push_back_write<float>( sdk::ctx::weapon_data + sdk::offsets::my_crouch_slide_spread_max, no_spread ? 0.f : old_weapon_data.crouch_slide_spread_max );
	write.push_back_write<float>( sdk::ctx::weapon_data + sdk::offsets::my_in_air_spread_min, no_spread ? 0.f : old_weapon_data.in_air_spread_min );
	write.push_back_write<float>( sdk::ctx::weapon_data + sdk::offsets::my_in_air_spread_max, no_spread ? 0.f : old_weapon_data.in_air_spread_max );
	write.push_back_write<float>( sdk::ctx::weapon_data + sdk::offsets::my_movement_modifier, no_spread ? 0.f : old_weapon_data.movement_modifier );
	write.push_back_write<float>( sdk::ctx::weapon_data + sdk::offsets::my_jump_max_height_aiming_factor, double_jump ? 0.94f * ( menu::misc::jump_height.value + 1 ) : old_weapon_data.max_jump_height_aiming_factor );
	write.push_back_write<int>( sdk::ctx::weapon_data_2 + sdk::offsets::my_bullets_per_round, mod_bullet_count ? menu::misc::bullet_count.value : old_weapon_data.bullets_per_round );
	write.push_back_write<float>( sdk::ctx::weapon_data + sdk::offsets::my_first_person_fov_chance, viewmodel_fovs[ menu::vis::local::viewmodel_fov.value ] );
	write.push_back_write<float>( sdk::ctx::weapon_data + sdk::offsets::my_first_person_zoom_fov_change, viewmodel_fovs[ menu::vis::local::viewmodel_fov.value ] );
	write.push_back_write<int>( sdk::ctx::weapon_data + sdk::offsets::my_agent_bullet_penetration_count, shoot_through_players ? 6 : old_weapon_data.agent_bullet_penetration_count );
	write.push_back_write<float>( sdk::ctx::weapon_data + sdk::offsets::my_aiming_move_speed_modifier, omni_sprint ? 2.5f : old_weapon_data.aiming_move_speed_modifier );
	write.push_back_write<float>( sdk::ctx::weapon_data + sdk::offsets::my_sway_movement_speed, remove_penalty ? 0.f : old_weapon_data.sway_movement_speed );

	write.execute_write( );
	write.close( );

	//utils::write<DWORD>( sdk::ctx::weapon_data + 0x608, 6 ); //myAgentBulletPenetrationCount

	return true;
}