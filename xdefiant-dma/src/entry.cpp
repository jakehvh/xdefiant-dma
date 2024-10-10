#include "cheat/features/visuals/visuals.hpp"
#include "cheat/features/misc/misc.hpp"
#include "utils/dependencies/d3d11_overlay/d3d11_overlay.hpp"

int main_thread( )
{
	utils::setup_console( );
	utils::print( e_print_type::normal, "initializing [1/4]\n" );
	utils::print( e_print_type::normal, "loading pcileech...\n" );


	if ( !utils::load_leechcore_and_vmm( ) )
	{
		utils::throw_error( "failed to load leechcore and/or vmm!" );
		return 100;
	}

	if ( !utils::initialize_memprocfs( ) )
	{
		utils::throw_error( "failed to initialize pcileech!" );
		return 100;
	}

	if ( !utils::key::init( ) )
	{
		utils::throw_error( "failed to load keys!" );
		return 101;
	}

	if ( !utils::cursor::init( ) )
		utils::print( e_print_type::error, "ffailed to load cursor! (ignore on win11)\n" );

	system( "cls" );
	utils::print( e_print_type::normal, "initializing [2/4]\n" );
	utils::print( e_print_type::normal, "loading kmbox...\n" );

	utils::kmb.is_connected = utils::kmb.init( );
	if ( !utils::kmb.is_connected )
		utils::print( e_print_type::error, "failed to find active kmbox (ignore if using memory aimbot!)\n" );

	utils::sleep( 1s );

	utils::target_process = utils::process( "XDefiant.exe" );
	if ( !utils::target_process.get_base( ).as<uintptr_t>( ) )
	{
		utils::throw_error( "failed to get target process information!" );
		return 102;
	}

	system( "cls" );
	utils::print( e_print_type::normal, "initializing [3/4]\n" );
	utils::print( e_print_type::normal, "updating directory table base...\n" );

	if ( !utils::target_process.update_directory_table_base( ) )
	{
		utils::throw_error( "failed to update directory table base!" );
		return 103;
	}

	VMMDLL_ConfigSet( utils::vmm_handle, VMMDLL_OPT_CONFIG_TICK_PERIOD, 30 );
	VMMDLL_ConfigSet( utils::vmm_handle, VMMDLL_OPT_CONFIG_READCACHE_TICKS, 30 );
	VMMDLL_ConfigSet( utils::vmm_handle, VMMDLL_OPT_CONFIG_TLBCACHE_TICKS, 200 );
	VMMDLL_ConfigSet( utils::vmm_handle, VMMDLL_OPT_CONFIG_PROCCACHE_TICKS_PARTIAL, 3600 );
	VMMDLL_ConfigSet( utils::vmm_handle, VMMDLL_OPT_CONFIG_PROCCACHE_TICKS_TOTAL, 3600 );

	sdk::ctx::process_base = utils::target_process.get_base( ).as<uintptr_t>( );

	system( "cls" );
	utils::print( e_print_type::good, "initialized!\n" );

	std::thread( [&]( )
	{
		while ( auth::heart_beat_thread_check )
		{
			actor_cache::update( );
			Sleep( menu::cfg::cache_update_rate.value );
		}
	} ).detach( );

	std::thread( [&]( )
	{
		while ( auth::heart_beat_thread_check )
		{
			utils::key::update( );
			utils::sleep( 1ms );
		}
	} ).detach( );

	std::thread( [&]( )
	{
		while ( auth::heart_beat_thread_check )
		{
			misc::run( );
			utils::sleep( 1ms );
		}
	} ).detach( );

	std::thread( [&]( )
	{
		while ( auth::heart_beat_thread_check )
		{
			aimbot::run_triggerbot( );
			utils::sleep( 1ms );
		}
	} ).detach( );

#ifndef RELEASE_EXE
	std::thread( [&]( )
	{
		while ( auth::heart_beat_thread_check )
		{
			auth::heart_beat_request.game = "xdefiant";
			auth::heart_beat_request.version = "0.6.2";

			auto result = auth::send_heartbeat( auth::heart_beat_request );

			if ( result.status != auth::HEART_BEAT_OK )
				auth::heart_beat_thread_check = false;

			Sleep( result.interval * 1000 );
		}
	} ).detach( );
#endif

	d3d11_overlay( "[paionia.gg]: overlay", &sdk::ctx::screen_size, false ).run( visuals::render );

	system( "pause" );

	VMMDLL_Close( utils::vmm_handle );
}

int main( int argument_count, char** arguments )
{

#ifndef RELEASE_EXE
	if ( argument_count == 0 )
	{
		utils::throw_error( "critical error has occurred!" );
		return EXIT_FAILURE;
	}

	auth::heart_beat_request.token = arguments[ 1 ];
#endif

	main_thread( );

	return 0;
}