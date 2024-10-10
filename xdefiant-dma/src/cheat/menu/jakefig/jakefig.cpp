#include "jakefig.hpp"

void jakefig::initialize( )
{
    auto path = std::filesystem::current_path( ).string( ) + "\\paionia\\";
  
    if ( !std::filesystem::exists( path ) )
        std::filesystem::create_directories( path );

    for ( int i = 1; i < 4; i++ )
    {
		auto file = path + "slot" + std::to_string( i ) + ".pi";
        
        if ( !std::filesystem::exists( file ) )
        {
			std::ofstream ofs( file );
            ofs.close( );
		}
	}

    //aim
    add_item( &menu::aim::enabled.value, "aim", "enabled", menu::aim::enabled.value );
    add_item( &menu::aim::key.key, "aim", "key", menu::aim::key.key );
    add_item( &menu::aim::smooth_x.value, "aim", "smooth_x", menu::aim::smooth_x.value );
    add_item( &menu::aim::smooth_y.value, "aim", "smooth_y", menu::aim::smooth_y.value );
    add_item( &menu::aim::fov.value, "aim", "fov", menu::aim::fov.value );
    add_item( &menu::aim::draw_fov.value, "aim", "draw_fov", menu::aim::draw_fov.value );
    add_item( &menu::aim::fov_col.value.Value.x, "aim", "fov_col_r", menu::aim::fov_col.value.Value.x );
    add_item( &menu::aim::fov_col.value.Value.y, "aim", "fov_col_g", menu::aim::fov_col.value.Value.y );
    add_item( &menu::aim::fov_col.value.Value.z, "aim", "fov_col_b", menu::aim::fov_col.value.Value.z );
    add_item( &menu::aim::fov_col.value.Value.w, "aim", "fov_col_a", menu::aim::fov_col.value.Value.w );
    add_item( &menu::aim::deadzone.value, "aim", "deadzone", menu::aim::deadzone.value );
    add_item( &menu::aim::draw_deadzone.value, "aim", "draw_deadzone", menu::aim::draw_deadzone.value );
    add_item( &menu::aim::deadzone_col.value.Value.x, "aim", "deadzone_col_r", menu::aim::deadzone_col.value.Value.x );
    add_item( &menu::aim::deadzone_col.value.Value.y, "aim", "deadzone_col_g", menu::aim::deadzone_col.value.Value.y );
    add_item( &menu::aim::deadzone_col.value.Value.z, "aim", "deadzone_col_b", menu::aim::deadzone_col.value.Value.z );
    add_item( &menu::aim::deadzone_col.value.Value.w, "aim", "deadzone_col_a", menu::aim::deadzone_col.value.Value.w );
    add_item( &menu::aim::hitboxes.value[ 0 ], "aim", "hitboxes_head", menu::aim::hitboxes.value[ 0 ] );
    add_item( &menu::aim::hitboxes.value[ 1 ], "aim", "hitboxes_neck", menu::aim::hitboxes.value[ 1 ] );
    add_item( &menu::aim::hitboxes.value[ 2 ], "aim", "hitboxes_chest", menu::aim::hitboxes.value[ 2 ] );
    add_item( &menu::aim::hitboxes.value[ 3 ], "aim", "hitboxes_stomach", menu::aim::hitboxes.value[ 3 ] );
    add_item( &menu::aim::hitboxes.value[ 4 ], "aim", "hitboxes_arms", menu::aim::hitboxes.value[ 4 ] );
    add_item( &menu::aim::hitboxes.value[ 5 ], "aim", "hitboxes_hands", menu::aim::hitboxes.value[ 5 ] );
    add_item( &menu::aim::hitboxes.value[ 6 ], "aim", "hitboxes_legs", menu::aim::hitboxes.value[ 6 ] );
    add_item( &menu::aim::hitboxes.value[ 7 ], "aim", "hitboxes_feet", menu::aim::hitboxes.value[ 7 ] );
    add_item( &menu::aim::cache_target.value, "aim", "cache_target", menu::aim::cache_target.value );
    add_item( &menu::aim::prioritize_dist.value, "aim", "prioritize_dist", menu::aim::prioritize_dist.value );
    add_item( &menu::aim::psilent.value, "aim", "psilent", menu::aim::psilent.value );
    add_item( &menu::aim::aim_line.value, "aim", "aim_line", menu::aim::aim_line.value );
    add_item( &menu::aim::aim_line_col.value.Value.x, "aim", "aim_line_col_r", menu::aim::aim_line_col.value.Value.x );
    add_item( &menu::aim::aim_line_col.value.Value.y, "aim", "aim_line_col_g", menu::aim::aim_line_col.value.Value.y );
    add_item( &menu::aim::aim_line_col.value.Value.z, "aim", "aim_line_col_b", menu::aim::aim_line_col.value.Value.z );
    add_item( &menu::aim::aim_line_col.value.Value.w, "aim", "aim_line_col_a", menu::aim::aim_line_col.value.Value.w );

    add_item( &menu::aim::trigger_enabled.value, "aim", "trigger_enabled", menu::aim::trigger_enabled.value );
    add_item( &menu::aim::trigger_key.key, "aim", "trigger_key", menu::aim::trigger_key.key );
    add_item( &menu::aim::trigger_delay.value, "aim", "trigger_delay", menu::aim::trigger_delay.value );
    //aim



    //visuals
    add_item( &menu::vis::player::bbox.value, "visuals", "player_bbox", menu::vis::player::bbox.value );
    add_item( &menu::vis::player::bbox_col.value.Value.x, "visuals", "player_bbox_col_r", menu::vis::player::bbox_col.value.Value.x );
    add_item( &menu::vis::player::bbox_col.value.Value.y, "visuals", "player_bbox_col_g", menu::vis::player::bbox_col.value.Value.y );
    add_item( &menu::vis::player::bbox_col.value.Value.z, "visuals", "player_bbox_col_b", menu::vis::player::bbox_col.value.Value.z );
    add_item( &menu::vis::player::bbox_col.value.Value.w, "visuals", "player_bbox_col_a", menu::vis::player::bbox_col.value.Value.w );
    add_item( &menu::vis::player::bbox_type.value, "visuals", "player_bbox_type", menu::vis::player::bbox_type.value );
    add_item( &menu::vis::player::name.value, "visuals", "player_name", menu::vis::player::name.value );
    add_item( &menu::vis::player::name_col.value.Value.x, "visuals", "player_name_col_r", menu::vis::player::name_col.value.Value.x );
    add_item( &menu::vis::player::name_col.value.Value.y, "visuals", "player_name_col_g", menu::vis::player::name_col.value.Value.y );
    add_item( &menu::vis::player::name_col.value.Value.z, "visuals", "player_name_col_b", menu::vis::player::name_col.value.Value.z );
    add_item( &menu::vis::player::name_col.value.Value.w, "visuals", "player_name_col_a", menu::vis::player::name_col.value.Value.w );
    add_item( &menu::vis::player::health.value, "visuals", "player_health", menu::vis::player::health.value );
    add_item( &menu::vis::player::health_col_full.value.Value.x, "visuals", "player_health_col_full_r", menu::vis::player::health_col_full.value.Value.x );
    add_item( &menu::vis::player::health_col_full.value.Value.y, "visuals", "player_health_col_full_g", menu::vis::player::health_col_full.value.Value.y );
    add_item( &menu::vis::player::health_col_full.value.Value.z, "visuals", "player_health_col_full_b", menu::vis::player::health_col_full.value.Value.z );
    add_item( &menu::vis::player::health_col_full.value.Value.w, "visuals", "player_health_col_full_a", menu::vis::player::health_col_full.value.Value.w );
    add_item( &menu::vis::player::health_col_empty.value.Value.x, "visuals", "player_health_col_empty_r", menu::vis::player::health_col_empty.value.Value.x );
    add_item( &menu::vis::player::health_col_empty.value.Value.y, "visuals", "player_health_col_empty_g", menu::vis::player::health_col_empty.value.Value.y );
    add_item( &menu::vis::player::health_col_empty.value.Value.z, "visuals", "player_health_col_empty_b", menu::vis::player::health_col_empty.value.Value.z );
    add_item( &menu::vis::player::health_col_empty.value.Value.w, "visuals", "player_health_col_empty_a", menu::vis::player::health_col_empty.value.Value.w );
    add_item( &menu::vis::player::skeleton.value, "visuals", "player_skeleton", menu::vis::player::skeleton.value );
    add_item( &menu::vis::player::skeleton_col.value.Value.x, "visuals", "player_skeleton_col_r", menu::vis::player::skeleton_col.value.Value.x );
    add_item( &menu::vis::player::skeleton_col.value.Value.y, "visuals", "player_skeleton_col_g", menu::vis::player::skeleton_col.value.Value.y );
    add_item( &menu::vis::player::skeleton_col.value.Value.z, "visuals", "player_skeleton_col_b", menu::vis::player::skeleton_col.value.Value.z );
    add_item( &menu::vis::player::skeleton_col.value.Value.w, "visuals", "player_skeleton_col_a", menu::vis::player::skeleton_col.value.Value.w );
    add_item( &menu::vis::player::skeleton_thickness.value, "visuals", "player_skeleton_thickness", menu::vis::player::skeleton_thickness.value );
    add_item( &menu::vis::player::snaplines.value, "visuals", "player_snaplines", menu::vis::player::snaplines.value );
    add_item( &menu::vis::player::snaplines_col.value.Value.x, "visuals", "player_snaplines_col_r", menu::vis::player::snaplines_col.value.Value.x );
    add_item( &menu::vis::player::snaplines_col.value.Value.y, "visuals", "player_snaplines_col_g", menu::vis::player::snaplines_col.value.Value.y );
    add_item( &menu::vis::player::snaplines_col.value.Value.z, "visuals", "player_snaplines_col_b", menu::vis::player::snaplines_col.value.Value.z );
    add_item( &menu::vis::player::snaplines_col.value.Value.w, "visuals", "player_snaplines_col_a", menu::vis::player::snaplines_col.value.Value.w );
    add_item( &menu::vis::player::flags.value[ 0 ], "visuals", "player_flags_distance", menu::vis::player::flags.value[ 0 ] );
    add_item( &menu::vis::player::flags.value[ 1 ], "visuals", "player_flags_health", menu::vis::player::flags.value[ 1 ] );
    add_item( &menu::vis::player::flags.value[ 2 ], "visuals", "player_flags_level", menu::vis::player::flags.value[ 2 ] );
    add_item( &menu::vis::player::flags.value[ 3 ], "visuals", "player_flags_sprinting", menu::vis::player::flags.value[ 3 ] );
    add_item( &menu::vis::player::flags.value[ 4 ], "visuals", "player_flags_crouched", menu::vis::player::flags.value[ 4 ] );
    add_item( &menu::vis::player::flags.value[ 5 ], "visuals", "player_flags_in_shield", menu::vis::player::flags.value[ 5 ] );
    add_item( &menu::vis::player::max_dist.value, "visuals", "player_max_dist", menu::vis::player::max_dist.value );

    add_item( &menu::vis::gadgets::name.value, "visuals", "gadgets_name", menu::vis::gadgets::name.value );
    add_item( &menu::vis::gadgets::name_col.value.Value.x, "visuals", "gadgets_name_col_r", menu::vis::gadgets::name_col.value.Value.x );
    add_item( &menu::vis::gadgets::name_col.value.Value.y, "visuals", "gadgets_name_col_g", menu::vis::gadgets::name_col.value.Value.y );
    add_item( &menu::vis::gadgets::name_col.value.Value.z, "visuals", "gadgets_name_col_b", menu::vis::gadgets::name_col.value.Value.z );
    add_item( &menu::vis::gadgets::name_col.value.Value.w, "visuals", "gadgets_name_col_a", menu::vis::gadgets::name_col.value.Value.w );
    add_item( &menu::vis::gadgets::distance.value, "visuals", "gadgets_distance", menu::vis::gadgets::distance.value );
    add_item( &menu::vis::gadgets::distance_col.value.Value.x, "visuals", "gadgets_distance_col_r", menu::vis::gadgets::distance_col.value.Value.x );
    add_item( &menu::vis::gadgets::distance_col.value.Value.y, "visuals", "gadgets_distance_col_g", menu::vis::gadgets::distance_col.value.Value.y );
    add_item( &menu::vis::gadgets::distance_col.value.Value.z, "visuals", "gadgets_distance_col_b", menu::vis::gadgets::distance_col.value.Value.z );
    add_item( &menu::vis::gadgets::distance_col.value.Value.w, "visuals", "gadgets_distance_col_a", menu::vis::gadgets::distance_col.value.Value.w );
    add_item( &menu::vis::gadgets::max_dist.value, "visuals", "gadgets_max_dist", menu::vis::gadgets::max_dist.value );

    add_item( &menu::vis::local::crosshair.value, "visuals", "local_crosshair", menu::vis::local::crosshair.value );
    add_item( &menu::vis::local::crosshair_col.value.Value.x, "visuals", "local_crosshair_col_r", menu::vis::local::crosshair_col.value.Value.x );
    add_item( &menu::vis::local::crosshair_col.value.Value.y, "visuals", "local_crosshair_col_g", menu::vis::local::crosshair_col.value.Value.y );
    add_item( &menu::vis::local::crosshair_col.value.Value.z, "visuals", "local_crosshair_col_b", menu::vis::local::crosshair_col.value.Value.z );
    add_item( &menu::vis::local::crosshair_col.value.Value.w, "visuals", "local_crosshair_col_a", menu::vis::local::crosshair_col.value.Value.w );
    add_item( &menu::vis::local::viewmodel_fov.value, "visuals", "local_viewmodel_fov", menu::vis::local::viewmodel_fov.value );
    //visuals


    //misc
    add_item( &menu::misc::no_recoil.value, "misc", "no_recoil", menu::misc::no_recoil.value );
    add_item( &menu::misc::no_sway.value, "misc", "no_sway", menu::misc::no_sway.value );
    add_item( &menu::misc::no_spread.value, "misc", "no_spread", menu::misc::no_spread.value );
    add_item( &menu::misc::remove_penalty.value, "misc", "remove_penalty", menu::misc::remove_penalty.value );
    add_item( &menu::misc::instant_swap.value, "misc", "instant_swap", menu::misc::instant_swap.value );
    add_item( &menu::misc::instant_reload.value, "misc", "instant_reload", menu::misc::instant_reload.value );
    add_item( &menu::misc::instant_ads.value, "misc", "instant_ads", menu::misc::instant_ads.value );
    add_item( &menu::misc::instant_fire.value, "misc", "instant_fire", menu::misc::instant_fire.value );
    add_item( &menu::misc::modify_rpm.value, "misc", "modify_rpm", menu::misc::modify_rpm.value );
    add_item( &menu::misc::rpm_value.value, "misc", "rpm_value", menu::misc::rpm_value.value );
    add_item( &menu::misc::modify_bullet_count.value, "misc", "modify_bullet_count", menu::misc::modify_bullet_count.value );
    add_item( &menu::misc::bullet_count.value, "misc", "bullet_count", menu::misc::bullet_count.value );
    add_item( &menu::misc::jump_height.value, "misc", "jump_height", menu::misc::jump_height.value );
    add_item( &menu::misc::omni_sprint.value, "misc", "omni_sprint", menu::misc::omni_sprint.value );
    add_item( &menu::misc::shoot_through_players.value, "misc", "shoot_through_players", menu::misc::shoot_through_players.value );

    //misc

    add_item( &menu::cfg::cache_update_rate.value, "cfg", "cache_update_rate", menu::cfg::cache_update_rate.value );
    add_item( &menu::cfg::kmb_rate_limit.value, "cfg", "kmb_rate_limit", menu::cfg::kmb_rate_limit.value );
    add_item( &menu::cfg::screen_width.value, "cfg", "screen_width", menu::cfg::screen_width.value );
    add_item( &menu::cfg::screen_height.value, "cfg", "screen_height", menu::cfg::screen_height.value );
}

void jakefig::add_item( bool* pointer, std::string category, std::string name, bool default_value )
{
    booleans.push_back( { pointer, category, name, default_value } );
}

void jakefig::add_item( int* pointer, std::string category, std::string name, int default_value )
{
    ints.push_back( { pointer, category, name, default_value } );
}

void jakefig::add_item( float* pointer, std::string category, std::string name, float default_value )
{
    floats.push_back( { pointer, category, name, default_value } );
}

bool jakefig::save( std::string path )
{
    nlohmann::json data;

    auto checks = [&]( std::string category ) -> void
    {
        if ( data.find( category ) == data.end( ) )
            data[ category ] = nlohmann::json( );
    };

    for ( auto& b : booleans )
    {
        checks( b.category );
        data[ b.category ][ b.name ] = *b.pointer;
    }

    for ( auto& i : ints )
    {
        checks( i.category );
        data[ i.category ][ i.name ] = *i.pointer;
    }

    for ( auto& f : floats )
    {
        checks( f.category );
        data[ f.category ][ f.name ] = *f.pointer;
    }

    std::ofstream reader( path );
    reader.clear( );
    reader << std::setw( 4 ) << data.dump( ) << std::endl;
    reader.close( );

    return true;
}

inline bool file_exists( const std::string& name )
{
    struct stat buffer;
    return ( stat( name.c_str( ), &buffer ) == 0 );
}

bool jakefig::load( std::string path )
{
    if ( !file_exists( path ) )
        return false;

    std::ifstream in{ path };
    if ( !in.is_open( ) || in.fail( ) )
    {
        in.close( );
        return false;
    }

    std::string rdata;
    in.seekg( 0, std::ios::end );
    rdata.resize( in.tellg( ) );
    in.seekg( 0, std::ios::beg );
    in.read( &rdata[ 0 ], rdata.size( ) );
    in.close( );

    if ( rdata.empty( ) )
        return false;

    nlohmann::json data{ nlohmann::json::parse( rdata ) };

    auto checks = [ ]( nlohmann::json& d, std::string& c, std::string& n ) -> bool
    {
        return d.find( c ) != d.end( ) && d[ c ].find( n ) != d[ c ].end( );
    };

    for ( auto& b : booleans )
        if ( checks( data, b.category, b.name ) )
            *b.pointer = data[ b.category ][ b.name ].get<bool>( );

    for ( auto& i : ints )
        if ( checks( data, i.category, i.name ) )
            *i.pointer = data[ i.category ][ i.name ].get<int>( );

    for ( auto& f : floats )
        if ( checks( data, f.category, f.name ) )
            *f.pointer = data[ f.category ][ f.name ].get<float>( );

    return true;
}