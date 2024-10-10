#include "actor_cache.hpp"

#define LAST_IND(x, part_type) (sizeof(x) / sizeof(part_type) - 1)
#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN
#define LOW_IND(x, part_type) LAST_IND(x, part_type)
#define HIGH_IND(x, part_type) 0
#else
#define HIGH_IND(x, part_type) LAST_IND(x, part_type)
#define LOW_IND(x, part_type) 0
#endif

#define DWORDn(x, n) (*((uint32_t*)&(x) + n))
#define LODWORD(x) DWORDn(x, LOW_IND(x, uint32_t))

std::mutex update_mutex;
bool actor_cache::update( )
{
	std::lock_guard lock( update_mutex );
	auto clear_cache = [ ]( )->void
	{
		agents.clear( );
		gadgets.clear( );
		actor_count = 0;
	};

	sdk::ctx::world = sdk::get_world( );
	//printf( "world: 0x%llx\n", sdk::ctx::world );
	if ( !sdk::ctx::world )
	{
		clear_cache( );
		return true;
	}

	sdk::ctx::view_data = sdk::get_view_data( sdk::ctx::world );
	//printf( "view_data: 0x%llx\n", sdk::ctx::view_data );
	if ( !sdk::ctx::view_data )
	{
		clear_cache( );
		return true;
	}

	auto v5 = utils::read<unsigned int>( sdk::ctx::world + ( 0x898 + 0x50 ) );
	LODWORD( v5 ) = v5 & 0x7FFFFFFF;
	actor_count = v5;

	auto player_list = sdk::get_player_list( sdk::ctx::world );
	//printf( "player_list: 0x%llx\n", player_list );
	//printf( "actor_count: %i\n", actor_count );
	if ( !player_list )
	{
		clear_cache( );
		return true;
	}

	sdk::ctx::local_player = utils::read<uintptr_t>( player_list );
	//printf( "local_player: 0x%llx\n", sdk::ctx::local_player );
	if ( sdk::ctx::local_player )
	{
		sdk::ctx::local_team_id = utils::read<int>( sdk::decrypt_team_component( sdk::ctx::local_player ) + 0x10 );
		sdk::ctx::crosshair = sdk::decrypt_crosshair_component( sdk::ctx::local_player );

		sdk::ctx::crosshair_unk1 = utils::read<uintptr_t>( sdk::ctx::crosshair + 0x30 );

		sdk::ctx::local_skill_script_vars = utils::read<uintptr_t>( sdk::ctx::local_player + sdk::offsets::skill_script_vars );
		sdk::ctx::weapon_data = utils::read<uintptr_t>( sdk::ctx::crosshair_unk1 + 0x1d2f0 ); //ak rpm = 600 sig: 40 56 41 55 41 56 41 57 48 83 EC ? 45 33 F6 48 8D 42 ? 45 8B C6 4C 8B FA 4C 8B E9 0F 1F 40 ?
		sdk::ctx::weapon_data_2 = utils::read<uintptr_t>( utils::read<uintptr_t>( sdk::ctx::weapon_data + 0x240 ) );
	}

	/*
	for ( int i = 0x0; i < 0x1000; i++ )
	{
		char entity_type[ 0x400 ]{ };
		char entity_name[ 0x400 ]{ };
		int level { };

		utils::read( sdk::ctx::local_player + i, entity_type, 0x400 );
		utils::read( sdk::ctx::local_player + i, entity_name, 0x400 );
		utils::read( sdk::ctx::local_player + i, &level, 0x4 );

		if ( !strcmp( entity_type, "soldier" ) )
			printf( "entity_type: 0x%x\n", i );

		if ( !strcmp( entity_name, "BastianSuter69" ) )
			printf( "entity_name: 0x%x\n", i );

		if ( level == 51 )
			printf( "level: 0x%x\n", i );
	}
	*/

	//auto index_module = sdk::get_index_module( );

	//auto recoil_pattern_indexer = index_module.get_indexer( sdk::e_attributes::WeaponRecoilPatternModFinal );
	//auto recoil_pattern_horizontal_indexer = index_module.get_indexer( sdk::e_attributes::WeaponRecoilPatternHorizontalModFinal );
	//auto recoil_pattern_vertical_indexer = index_module.get_indexer( sdk::e_attributes::WeaponRecoilPatternVerticalModFinal );
	//auto aim_sway_speed_indexer = index_module.get_indexer( sdk::e_attributes::AimSwaySpeedModifierFinal );

	//printf( "%i, %s\n", recoil_pattern_indexer.get_index( ), recoil_pattern_indexer.get_name( ) );

		/*
	auto attribute_manager = sdk::get_attribute_manager( sdk::ctx::local_player );

	sdk::ctx::attributes::recoil_pattern = attribute_manager.get_attribute( recoil_pattern_indexer.get_index( ) );
	sdk::ctx::attributes::recoil_pattern_horizontal = attribute_manager.get_attribute( recoil_pattern_horizontal_indexer.get_index( ) );
	sdk::ctx::attributes::recoil_pattern_vertical = attribute_manager.get_attribute( recoil_pattern_vertical_indexer.get_index( ) );
	sdk::ctx::attributes::aim_sway_speed = attribute_manager.get_attribute( aim_sway_speed_indexer.get_index( ) );
	*/

	utils::scatter player_read { };
	std::vector<uintptr_t> actors { };
	actors.resize( actor_count );

	for ( int i = 0; i < actor_count; i++ )
		player_read.push_back_read<uintptr_t>( player_list + i * 0x8, &actors[ i ] );

	player_read.execute_read( );

	std::vector<f_agent> agents_tmp{ };
	std::vector<f_gadget> gadgets_tmp{ };
	for ( auto& actor : actors )
	{
		char entity_type[ 0x400 ] { };
		char entity_name[ 0x400 ] { };

		player_read.push_back_read( actor + sdk::offsets::entity_type, entity_type, 0x400 );
		player_read.push_back_read( actor + sdk::offsets::entity_name, entity_name, 0x400 );

		player_read.execute_read( );

		//printf( "0x%llx\n", actor );

		//printf( "0x%llx | %s | %s\n", actor, entity_type, entity_name );

		if ( !strcmp( entity_type, "soldier" ) )
			agents_tmp.push_back( { actor } );
		else
			gadgets_tmp.push_back( { actor, entity_name } );
	}

	for ( auto& agent : agents_tmp )
	{
		if ( !agent.addr )
			continue;

		player_read.push_back_read<uintptr_t>( agent.addr + sdk::offsets::skill_script_vars, &agent.skill_script_vars );
		player_read.push_back_read( agent.addr + sdk::offsets::entity_name, agent.player_name, 0x400 );
		player_read.push_back_read<int>( agent.addr + sdk::offsets::level, &agent.level );
		//player_read.push_back_read<uintptr_t>( agent.addr + sdk::offsets::animation_1, &agent.animation_1 );
		//player_read.push_back_read<uintptr_t>( agent.addr + sdk::offsets::animation_2, &agent.animation_2 );

		agent.team_component = sdk::decrypt_team_component( agent.addr );
		agent.health_component = sdk::decrypt_health_component( agent.addr );
	}

	player_read.execute_read( );

	for ( auto& agent : agents_tmp )
	{
		if ( !agent.addr )
			continue;

		agent.animation_component = sdk::decrypt_animation_component( agent.addr );

		player_read.push_back_read<uintptr_t>( agent.animation_component + sdk::offsets::bone_data_1, &agent.bone_data_1 );
		player_read.push_back_read<uintptr_t>( agent.animation_component + sdk::offsets::bone_data_2, &agent.bone_data_2 );
	}

	player_read.execute_read( );

	for ( auto& agent : agents_tmp )
	{
		agent.bone_data = sdk::decrypt_bone_data( agent.bone_data_1, agent.bone_data_2 );

		//player_read.push_back_read<uintptr_t>( agent.bone_data + sdk::offsets::bone_matrix_1, &agent.bone_matrix_1 );
		//player_read.push_back_read<uintptr_t>( agent.bone_data + sdk::offsets::bone_matrix_2, &agent.bone_matrix_2 );
	}

	player_read.execute_read( );

	for ( auto& agent : agents_tmp )
	{
		agent.bone_matrix = sdk::decrypt_bone_matrix( agent.bone_data );

		/*
		printf( "%s\n", agent.player_name );
		printf( "animation_component: 0x%llx\n", agent.animation_component );
		printf( "health_component: 0x%llx\n", agent.health_component );
		printf( "team_component: 0x%llx\n", agent.team_component );
		printf( "bone_data: 0x%llx\n", agent.bone_data );
		printf( "bone_matrix: 0x%llx\n", agent.bone_matrix );
		*/
	}

	/*
	printf( "world: 0x%llx\n", sdk::ctx::world );
	printf( "view_data: 0x%llx\n", sdk::ctx::view_data );
	printf( "player_list: 0x%llx\n", player_list );
	printf( "local_player: 0x%llx\n", sdk::ctx::local_player );
	printf( "local_team_id: %d\n", sdk::ctx::local_team_id );
	printf( "crosshair_unk1: 0x%llx\n", sdk::ctx::crosshair_unk1 );
	printf( "weapon_data: 0x%llx\n", sdk::ctx::weapon_data );
	printf( "weapon_data_2: 0x%llx\n", sdk::ctx::weapon_data_2 );
	*/

	player_read.close( );

	agents = agents_tmp;
	gadgets = gadgets_tmp;
	agents_tmp.clear( );
	gadgets_tmp.clear( );

	return true;
}