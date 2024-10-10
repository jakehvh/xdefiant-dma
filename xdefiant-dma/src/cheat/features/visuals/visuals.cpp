#include "visuals.hpp"

bool visuals::c_player_esp::update_box( )
{
    auto root_pos = m_player.location;
    auto head_pos = m_player.head_pos;
    head_pos.y += 0.3f;

    math::vec2 root_pos_screen{ };
    math::vec2 head_pos_screen{ };

    if ( !sdk::ctx::view.world_to_screen( root_pos, &root_pos_screen ) || !sdk::ctx::view.world_to_screen( head_pos, &head_pos_screen ) )
		return false;

    m_bbox.h = std::abs(static_cast< int >( root_pos_screen.y - head_pos_screen.y ));
    m_bbox.w = std::abs(static_cast< int >( m_bbox.h / 2.f ));
    m_bbox.x = std::abs(static_cast< int >( root_pos_screen.x - m_bbox.w * 0.5f ));
    m_bbox.y = std::abs(static_cast< int >( root_pos_screen.y - m_bbox.h ));

    return true;
}

void visuals::c_player_esp::draw_box( const int box_type, const ImColor color )
{
    if ( box_type == 0 )
        draw_cmds->rect( math::vec2( m_bbox.x, m_bbox.y ), math::vec2( m_bbox.w, m_bbox.h ), color, 0.f, 1.f, true );
    else
    {
        const std::array<std::pair<math::vec2, math::vec2>, 8>outline_positions
        {
            std::make_pair( math::vec2( m_bbox.x - 1.0, m_bbox.y ), math::vec2( m_bbox.x + ( m_bbox.w / 3.0 ) + 1.0, m_bbox.y ) ),
            std::make_pair( math::vec2( m_bbox.x - 1.0, m_bbox.y + m_bbox.h ), math::vec2( m_bbox.x + ( m_bbox.w / 3.0 ) + 1.0, m_bbox.y + m_bbox.h ) ),
            std::make_pair( math::vec2( m_bbox.x + m_bbox.w + 1.0, m_bbox.y ), math::vec2( m_bbox.x + ( m_bbox.w / 1.5 ) - 1.0, m_bbox.y ) ),
            std::make_pair( math::vec2( m_bbox.x + m_bbox.w + 1.0, m_bbox.y + m_bbox.h ), math::vec2( m_bbox.x + ( m_bbox.w / 1.5 ) - 1.0, m_bbox.y + m_bbox.h ) ),
            std::make_pair( math::vec2( m_bbox.x, m_bbox.y ), math::vec2( m_bbox.x, m_bbox.y + ( m_bbox.h / 3.0 ) + 1.0 ) ),
            std::make_pair( math::vec2( m_bbox.x, m_bbox.y + m_bbox.h ), math::vec2( m_bbox.x, m_bbox.y + ( m_bbox.h / 1.5 ) - 1.0 ) ),
            std::make_pair( math::vec2( m_bbox.x + m_bbox.w, m_bbox.y ), math::vec2( m_bbox.x + m_bbox.w, m_bbox.y + ( m_bbox.h / 3.0 ) + 1.0 ) ),
            std::make_pair( math::vec2( m_bbox.x + m_bbox.w, m_bbox.y + m_bbox.h ), math::vec2( m_bbox.x + m_bbox.w, m_bbox.y + ( m_bbox.h / 1.5 ) - 1.0 ) )
        };

        const std::array<std::pair<math::vec2, math::vec2>, 8>positions
        {
            std::make_pair( math::vec2( m_bbox.x, m_bbox.y ), math::vec2( m_bbox.x + ( m_bbox.w / 3.0 ), m_bbox.y ) ),
            std::make_pair( math::vec2( m_bbox.x, m_bbox.y + m_bbox.h ), math::vec2( m_bbox.x + ( m_bbox.w / 3.0 ), m_bbox.y + m_bbox.h ) ),
            std::make_pair( math::vec2( m_bbox.x + m_bbox.w, m_bbox.y ), math::vec2( m_bbox.x + ( m_bbox.w / 1.5 ), m_bbox.y ) ),
            std::make_pair( math::vec2( m_bbox.x + m_bbox.w, m_bbox.y + m_bbox.h ), math::vec2( m_bbox.x + ( m_bbox.w / 1.5 ), m_bbox.y + m_bbox.h ) ),
            std::make_pair( math::vec2( m_bbox.x, m_bbox.y ), math::vec2( m_bbox.x, m_bbox.y + ( m_bbox.h / 3.0 ) ) ),
            std::make_pair( math::vec2( m_bbox.x, m_bbox.y + m_bbox.h ), math::vec2( m_bbox.x, m_bbox.y + ( m_bbox.h / 1.5 ) ) ),
            std::make_pair( math::vec2( m_bbox.x + m_bbox.w, m_bbox.y ), math::vec2( m_bbox.x + m_bbox.w, m_bbox.y + ( m_bbox.h / 3.0 ) ) ),
            std::make_pair( math::vec2( m_bbox.x + m_bbox.w, m_bbox.y + m_bbox.h ), math::vec2( m_bbox.x + m_bbox.w, m_bbox.y + ( m_bbox.h / 1.5 ) ) )
        };

        for ( auto& outline_position : outline_positions )
            draw_cmds->line( outline_position.first, outline_position.second, ImColor( 25, 25, 25, 255 ), 3.f );

        for ( auto& position : positions )
            draw_cmds->line( position.first, position.second, color, 1.f );
    }
}

void visuals::c_player_esp::draw_name( const ImColor color )
{
    draw_cmds->text( smallest_p7, 10.f, math::vec2( m_bbox.x + m_bbox.w / 2.0, m_bbox.y - 2.0 ), color, m_player.player_name, true, true );
}

void visuals::c_player_esp::draw_weapon( const ImColor color )
{
    std::string weapon_name = "none";

    static std::unordered_map<uintptr_t, std::string> cached_weapon_names{ };

    auto pos = cached_weapon_names.find( m_player.current_weapon );
    if ( pos == cached_weapon_names.end( ) )
    {
        auto weapon_2 = utils::read<uintptr_t>( m_player.current_weapon + 0x30 );
        auto weapon_3 = utils::read<uintptr_t>( weapon_2 + 0x8 );

        weapon_name = utils::read_string( weapon_3 + 0x28 );

        cached_weapon_names.emplace( std::make_pair( m_player.current_weapon, weapon_name ) );
    }
    else
        weapon_name = pos->second;

    draw_cmds->text( smallest_p7, 10.f, math::vec2( m_bbox.x + m_bbox.w / 2.0, m_bbox.y + m_bbox.h + 11.0 ), color, weapon_name, true, true );
}

void visuals::c_player_esp::draw_skeleton( const ImColor color, const float thickness )
{
    std::vector<std::vector<math::vec3>>bone_positons
    {
        { m_player.pelvis_pos, m_player.lower_spine_pos, m_player.upper_spine_pos, m_player.neck_pos, m_player.head_pos },
        { m_player.neck_pos, m_player.left_shoulder_pos, m_player.left_elbow_pos, m_player.left_hand_pos },
        { m_player.neck_pos, m_player.right_shoulder_pos, m_player.right_elbow_pos, m_player.right_hand_pos },
        { m_player.pelvis_pos, m_player.left_thigh_pos, m_player.left_knee_pos, m_player.left_calf_pos, m_player.left_foot_pos },
        { m_player.pelvis_pos, m_player.right_thigh_pos, m_player.right_knee_pos, m_player.right_calf_pos, m_player.right_foot_pos }
    };

    for ( int i = 0; i < bone_positons.size( ); i++ )
    {
        math::vec2 parent_bone_pos_screen{ };
        for ( int b = 0; b < bone_positons[ i ].size( ); b++ )
        {
            math::vec3 bone_pos = bone_positons[ i ][ b ];
            math::vec2 bone_pos_screen{ };

            if ( !sdk::ctx::view.world_to_screen( bone_pos, &bone_pos_screen ) )
                break;

            if ( parent_bone_pos_screen.x == 0.0 )
            {
                parent_bone_pos_screen = bone_pos_screen;
                continue;
            }

            draw_cmds->line( bone_pos_screen, parent_bone_pos_screen, color, thickness );
            parent_bone_pos_screen = bone_pos_screen;
        }
    }
}

void visuals::c_player_esp::draw_health_bar( const ImColor color_full, const ImColor color_empty )
{
    const float health_height = std::clamp<float>( m_player.current_health * m_bbox.h / m_player.max_health, 0.f, m_bbox.h );
    const float hp = static_cast< float >( m_player.current_health ) / static_cast< float >( m_player.max_health );

    int health_r = static_cast< int >( std::lerp( color_empty.Value.x, color_full.Value.x, hp ) * 255.f );
    int health_g = static_cast< int >( std::lerp( color_empty.Value.y, color_full.Value.y, hp ) * 255.f );
    int health_b = static_cast< int >( std::lerp( color_empty.Value.z, color_full.Value.z, hp ) * 255.f );

    draw_cmds->rect_filled( math::vec2( m_bbox.x - 5.f, m_bbox.y ), math::vec2( 2.f, m_bbox.h ), ImColor( 25, 25, 25, 255 ), 0.f, true );
    draw_cmds->rect_filled( math::vec2( m_bbox.x - 5.f, m_bbox.y + ( m_bbox.h - health_height ) ), math::vec2( 2.f, health_height ), ImColor( health_r, health_g, health_b, 255 ), 0.f, false );

    m_health_flag_offset += 5.f;
}

void visuals::c_player_esp::draw_flags( const bool* flags, const ImColor color_full, const ImColor color_empty )
{
    std::vector<std::pair<std::string, ImColor>>flag_esp{ };

    auto dist = static_cast< int >( sdk::ctx::local_location.dist_to( m_player.location ) );

    if ( flags[ 0 ] )
        flag_esp.push_back( { std::format( "{}m", dist ), ImColor( 1.f, 1.f, 1.f, 1.f ) } );

    if ( flags[ 1 ] )
    {
        const float hp = static_cast< float >( m_player.current_health ) / static_cast< float >( m_player.max_health );

        int health_r = static_cast< int >( std::lerp( color_empty.Value.x, color_full.Value.x, hp ) * 255.f );
        int health_g = static_cast< int >( std::lerp( color_empty.Value.y, color_full.Value.y, hp ) * 255.f );
        int health_b = static_cast< int >( std::lerp( color_empty.Value.z, color_full.Value.z, hp ) * 255.f );

        std::string health_str = std::to_string( m_player.current_health );
        auto health_str_sz = smallest_p7->CalcTextSizeA( 10.f, FLT_MAX, 0.0f, health_str.c_str( ) );

        draw_cmds->text( smallest_p7, 10.f, math::vec2( m_bbox.x - health_str_sz.x - m_health_flag_offset, m_bbox.y - 3.f ), ImColor( health_r, health_g, health_b, 255 ), health_str, true, false );
    }

    if ( flags[ 2 ] )
        flag_esp.push_back( { std::format( "l{}", m_player.level ), ImColor( 1.f, 1.f, 1.f, 1.f ) } );

    if ( flags[ 3 ] && m_player.is_sprinting )
        flag_esp.push_back( { "sprnt", ImColor( 0.f, 1.f, 1.f, 1.f ) } );

    if ( flags[ 4 ] && m_player.is_crouched )
        flag_esp.push_back( { "crch", ImColor( 1.f, 1.f, 0.f, 1.f ) } );

    if ( flags[ 5 ] && m_player.is_in_riot_shield )
        flag_esp.push_back( { "shld", ImColor( 1.f, 0.f, 0.f, 1.f ) } );

    for ( int i = 0; i < flag_esp.size( ); i++ )
        draw_cmds->text( smallest_p7, 10.f, math::vec2( m_bbox.x + m_bbox.w + 4.0, ( m_bbox.y - 3.0 ) + ( i * 9.0 ) ), flag_esp[ i ].second, flag_esp[ i ].first, true, false );
}

void visuals::c_player_esp::draw_snapline( const int line_pos, const ImColor color )
{
    math::vec2 start_pos{ sdk::ctx::screen_size.x / 2.f, sdk::ctx::screen_size.y / 2.f };
    math::vec2 end_pos{ m_bbox.x + ( m_bbox.w / 2.f ), m_bbox.y + ( m_bbox.h / 2.f ) };

    if ( line_pos == 1 )
    {
        start_pos.y = 0.f;
        end_pos.y = m_bbox.y - 11.f;
    }
    else if ( line_pos == 2 )
    {
        start_pos.y = sdk::ctx::screen_size.y;
        end_pos.y = m_bbox.y + m_bbox.h;
    }

    draw_cmds->line( start_pos, end_pos, color, 2.f );
}

std::once_flag once_flag { };

bool visuals::render( )
{
    std::call_once( once_flag, [ ]
    {
        menu::cfg::screen_width.value = sdk::ctx::screen_size.x;
        menu::cfg::screen_height.value = sdk::ctx::screen_size.y;
    } );

#if defined(_DEBUG) || defined(RELEASE_EXE)
    draw_cmds->text( smallest_p7, 10.f, math::vec2( 10.f, 10.f ), ImColor( 1.f, 1.f, 1.f, 1.f ), std::format( "paionia.gg | dev | {} fps | " __DATE__, static_cast< int >( ImGui::GetIO( ).Framerate ) ), true );

    draw_cmds->text( smallest_p7, 10.f, math::vec2( 10.f, 720.f ), ImColor( 1.f, 1.f, 1.f, 1.f ), std::format( "world: 0x{:x}", sdk::ctx::world ), true );
    draw_cmds->text( smallest_p7, 10.f, math::vec2( 10.f, 732.f ), ImColor( 1.f, 1.f, 1.f, 1.f ), std::format( "actor count: {}", actor_cache::actor_count ), true );
    draw_cmds->text( smallest_p7, 10.f, math::vec2( 10.f, 744.f ), ImColor( 1.f, 1.f, 1.f, 1.f ), std::format( "aim target: 0x{:x}", sdk::ctx::aim_target ), true );
    draw_cmds->text( smallest_p7, 10.f, math::vec2( 10.f, 756.f ), ImColor( 1.f, 1.f, 1.f, 1.f ), std::format( "aim pos: {}, {}", sdk::ctx::aim_pos.x, sdk::ctx::aim_pos.y ), true );
    draw_cmds->text( smallest_p7, 10.f, math::vec2( 10.f, 768.f ), ImColor( 1.f, 1.f, 1.f, 1.f ), std::format( "aim key held: {}", utils::key( menu::aim::key.key ).is_held( ) ? "true" : "false" ), true );
#else
    draw_cmds->text( smallest_p7, 10.f, math::vec2( 10.f, 10.f ), ImColor( 1.f, 1.f, 1.f, 1.f ), std::format( "paionia.gg | {} fps | " __DATE__, static_cast< int >( ImGui::GetIO( ).Framerate ) ), true );
    draw_cmds->text( smallest_p7, 10.f, math::vec2( 10.f, 22.f ), ImColor( 1.f, 1.f, 1.f, 1.f ), std::format( "menu control: {}", jw::main_pc_control ? "main pc" : "radar pc" ), true );
#endif

/*
    std::vector<uint32_t> health_values{ 0x1c4, 0x1c8, 0x388, 0x38c };

    auto local_health_comp = sdk::decrypt_health_component( sdk::ctx::local_player );
    for ( auto& value : health_values )
    {
        int val = utils::read<int>( local_health_comp + value );

        printf( "0x%x: %i\n", value, val );
    }
*/

    if ( !auth::heart_beat_thread_check )
        return true;

    math::vec2 screen_center{ sdk::ctx::screen_size.x / 2.f, sdk::ctx::screen_size.y / 2.f };

    if ( menu::vis::local::crosshair.value )
    {
        draw_cmds->rect( math::vec2( screen_center.x - 10.f, screen_center.y - 2.f ), math::vec2( 8.f, 4.f ), ImColor( 25, 25, 25, 255 ) ); //left
        draw_cmds->rect( math::vec2( screen_center.x + 2.f, screen_center.y - 2.f ), math::vec2( 8.f, 4.f ), ImColor( 25, 25, 25, 255 ) ); //right
        draw_cmds->rect( math::vec2( screen_center.x - 2.f, screen_center.y + 2.f ), math::vec2( 4.f, 8.f ), ImColor( 25, 25, 25, 255 ) ); //bottom
        draw_cmds->rect( math::vec2( screen_center.x - 2.f, screen_center.y - 10.f ), math::vec2( 4.f, 8.f ), ImColor( 25, 25, 25, 255 ) ); //top

        draw_cmds->rect_filled( math::vec2( screen_center.x - 9.f, screen_center.y - 1.f ), math::vec2( 6.f, 2.f ), menu::vis::local::crosshair_col.value ); //left
        draw_cmds->rect_filled( math::vec2( screen_center.x + 3.f, screen_center.y - 1.f ), math::vec2( 6.f, 2.f ), menu::vis::local::crosshair_col.value ); //right
        draw_cmds->rect_filled( math::vec2( screen_center.x - 1.f, screen_center.y + 3.f ), math::vec2( 2.f, 6.f ), menu::vis::local::crosshair_col.value ); //bottom
        draw_cmds->rect_filled( math::vec2( screen_center.x - 1.f, screen_center.y - 9.f ), math::vec2( 2.f, 6.f ), menu::vis::local::crosshair_col.value ); //top
    }


    utils::scatter frame_read{ };
    math::matrix_4x4 view_matrix{ };
    uintptr_t crosshair_target = 0;

    frame_read.push_back_read<uintptr_t>( sdk::ctx::crosshair + 0x88, &sdk::ctx::crosshair_entity );
    frame_read.execute_read( );

    //frame_read.push_back_read<math::vec3>( sdk::ctx::world + sdk::offsets::camera_pos, &sdk::ctx::camera_pos );
    frame_read.push_back_read<math::vec3>( sdk::ctx::local_player + 0x80, &sdk::ctx::local_location );
    frame_read.push_back_read<math::matrix_4x4>( sdk::ctx::view_data + 0x170, &view_matrix );
    frame_read.push_back_read<uintptr_t>( sdk::ctx::crosshair_entity + 0x30, &crosshair_target );

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

    for ( auto& agent : actor_cache::agents )
    {
        //if ( agent.addr == sdk::ctx::local_player )
            //continue;

        frame_read.push_back_read<math::vec3>( agent.addr + sdk::offsets::location, &agent.location );

        frame_read.push_back_read<BYTE>( agent.skill_script_vars + sdk::offsets::is_sprinting_value, &agent.is_sprinting_value );
        frame_read.push_back_read<BYTE>( agent.skill_script_vars + sdk::offsets::is_in_riot_shield_value, &agent.is_in_riot_shield_value );
        frame_read.push_back_read<BYTE>( agent.skill_script_vars + sdk::offsets::is_crouched_value, &agent.is_crouched_value );

        frame_read.push_back_read<int>( agent.team_component + sdk::offsets::team_id, &agent.team_id );

        frame_read.push_back_read<int>( agent.health_component + sdk::offsets::current_health, &agent.current_health );
        frame_read.push_back_read<int>( agent.health_component + sdk::offsets::max_health, &agent.max_health );

        //frame_read.push_back_read<uintptr_t>( agent.weapon_component + 0x158, &agent.current_weapon );

        frame_read.push_back_read<math::matrix_4x4>( agent.bone_matrix + ( sdk::offsets::head << 6 ), &agent.head_matrix );
        frame_read.push_back_read<math::matrix_4x4>( agent.bone_matrix + ( sdk::offsets::neck << 6 ), &agent.neck_matrix );
        frame_read.push_back_read<math::matrix_4x4>( agent.bone_matrix + ( sdk::offsets::upper_spine << 6 ), &agent.upper_spine_matrix );
        frame_read.push_back_read<math::matrix_4x4>( agent.bone_matrix + ( sdk::offsets::lower_spine << 6 ), &agent.lower_spine_matrix );
        frame_read.push_back_read<math::matrix_4x4>( agent.bone_matrix + ( sdk::offsets::pelvis << 6 ), &agent.pelvis_matrix );
        frame_read.push_back_read<math::matrix_4x4>( agent.bone_matrix + ( sdk::offsets::left_shoulder << 6 ), &agent.left_shoulder_matrix );
        frame_read.push_back_read<math::matrix_4x4>( agent.bone_matrix + ( sdk::offsets::left_elbow << 6 ), &agent.left_elbow_matrix );
        frame_read.push_back_read<math::matrix_4x4>( agent.bone_matrix + ( sdk::offsets::left_hand << 6 ), &agent.left_hand_matrix );
        frame_read.push_back_read<math::matrix_4x4>( agent.bone_matrix + ( sdk::offsets::left_thigh << 6 ), &agent.left_thigh_matrix );
        frame_read.push_back_read<math::matrix_4x4>( agent.bone_matrix + ( sdk::offsets::left_knee << 6 ), &agent.left_knee_matrix );
        frame_read.push_back_read<math::matrix_4x4>( agent.bone_matrix + ( sdk::offsets::left_calf << 6 ), &agent.left_calf_matrix );
        frame_read.push_back_read<math::matrix_4x4>( agent.bone_matrix + ( sdk::offsets::left_foot << 6 ), &agent.left_foot_matrix );
        frame_read.push_back_read<math::matrix_4x4>( agent.bone_matrix + ( sdk::offsets::right_shoulder << 6 ), &agent.right_shoulder_matrix );
        frame_read.push_back_read<math::matrix_4x4>( agent.bone_matrix + ( sdk::offsets::right_elbow << 6 ), &agent.right_elbow_matrix );
        frame_read.push_back_read<math::matrix_4x4>( agent.bone_matrix + ( sdk::offsets::right_hand << 6 ), &agent.right_hand_matrix );
        frame_read.push_back_read<math::matrix_4x4>( agent.bone_matrix + ( sdk::offsets::right_thigh << 6 ), &agent.right_thigh_matrix );
        frame_read.push_back_read<math::matrix_4x4>( agent.bone_matrix + ( sdk::offsets::right_knee << 6 ), &agent.right_knee_matrix );
        frame_read.push_back_read<math::matrix_4x4>( agent.bone_matrix + ( sdk::offsets::right_calf << 6 ), &agent.right_calf_matrix );
        frame_read.push_back_read<math::matrix_4x4>( agent.bone_matrix + ( sdk::offsets::right_foot << 6 ), &agent.right_foot_matrix );
    }

    for ( auto& gadget : actor_cache::gadgets )
    {
        frame_read.push_back_read<math::vec3>( gadget.addr + sdk::offsets::location, &gadget.location );
    };

    frame_read.execute_read( );

    sdk::ctx::crosshair_target = 0;
    for ( auto& agent : actor_cache::agents )
    {
        if ( agent.addr == sdk::ctx::local_player )
            continue;

        auto get_bone_pos = [ ]( math::matrix_4x4 bone_matrix ) -> math::vec3
        {
            return math::vec3( bone_matrix.m[ 3 ][ 0 ], bone_matrix.m[ 3 ][ 1 ], bone_matrix.m[ 3 ][ 2 ] );
        };

        agent.head_pos = get_bone_pos( agent.head_matrix );
        agent.neck_pos = get_bone_pos( agent.neck_matrix );
        agent.upper_spine_pos = get_bone_pos( agent.upper_spine_matrix );
        agent.lower_spine_pos = get_bone_pos( agent.lower_spine_matrix );
        agent.pelvis_pos = get_bone_pos( agent.pelvis_matrix );
        agent.left_shoulder_pos = get_bone_pos( agent.left_shoulder_matrix );
        agent.left_elbow_pos = get_bone_pos( agent.left_elbow_matrix );
        agent.left_hand_pos = get_bone_pos( agent.left_hand_matrix );
        agent.left_thigh_pos = get_bone_pos( agent.left_thigh_matrix );
        agent.left_knee_pos = get_bone_pos( agent.left_knee_matrix );
        agent.left_calf_pos = get_bone_pos( agent.left_calf_matrix );
        agent.left_foot_pos = get_bone_pos( agent.left_foot_matrix );
        agent.right_shoulder_pos = get_bone_pos( agent.right_shoulder_matrix );
        agent.right_elbow_pos = get_bone_pos( agent.right_elbow_matrix );
        agent.right_hand_pos = get_bone_pos( agent.right_hand_matrix );
        agent.right_thigh_pos = get_bone_pos( agent.right_thigh_matrix );
        agent.right_knee_pos = get_bone_pos( agent.right_knee_matrix );
        agent.right_calf_pos = get_bone_pos( agent.right_calf_matrix );
        agent.right_foot_pos = get_bone_pos( agent.right_foot_matrix );

        if ( agent.addr == crosshair_target && !( agent.current_health <= 0 ) && agent.addr != sdk::ctx::local_player && agent.team_id != sdk::ctx::local_team_id )
            sdk::ctx::crosshair_target = agent.addr;

        agent.is_sprinting = agent.is_sprinting_value & 2;
        agent.is_in_riot_shield = agent.is_in_riot_shield_value & 1;
        agent.is_crouched = agent.is_crouched_value == 1;

        //printf( "0x%llx | 0x%llx\n", crosshair_target, sdk::ctx::crosshair_target );
    }

    frame_read.close( );

    sdk::ctx::view = sdk::c_view( view_matrix.transpose( ), math::vec2( menu::cfg::screen_width.value, menu::cfg::screen_height.value ) );

    for ( auto& agent : actor_cache::agents )
    {
        //printf( "%.f, %.f, %.f\n", agent.location.x, agent.location.y, agent.location.z );

        if ( agent.addr == sdk::ctx::local_player || agent.current_health <= 0.f || agent.team_id == sdk::ctx::local_team_id )
           continue;

        auto dist = sdk::ctx::local_location.dist_to( agent.location );
        if ( dist > menu::vis::player::max_dist.value )
            continue;

        c_player_esp player_esp { agent };

        if( menu::vis::player::skeleton.value )
            player_esp.draw_skeleton( menu::vis::player::skeleton_col.value, menu::vis::player::skeleton_thickness.value );

        if ( !player_esp.update_box( ) )
            continue;

        if( menu::vis::player::bbox.value )
            player_esp.draw_box( menu::vis::player::bbox_type.value, menu::vis::player::bbox_col.value );

        if( menu::vis::player::name.value )
            player_esp.draw_name( menu::vis::player::name_col.value );

        //if ( menu::vis::player::weapon.value )
            //player_esp.draw_weapon( menu::vis::player::weapon_col.value );

        if ( menu::vis::player::health.value )
            player_esp.draw_health_bar( menu::vis::player::health_col_full.value, menu::vis::player::health_col_empty.value );

        if ( menu::vis::player::snaplines.value )
            player_esp.draw_snapline( menu::vis::player::snaplines.value, menu::vis::player::snaplines_col.value );

        player_esp.draw_flags( menu::vis::player::flags.value, menu::vis::player::health_col_full.value, menu::vis::player::health_col_empty.value );
    }

    for ( auto& gadget : actor_cache::gadgets )
    {
        auto dist = sdk::ctx::local_location.dist_to( gadget.location );
        if ( dist > menu::vis::gadgets::max_dist.value )
            continue;

        math::vec2 screen { };
        if ( !sdk::ctx::view.world_to_screen( gadget.location, &screen ) )
            continue;

        if ( menu::vis::gadgets::name.value )
        {
            auto gadget_name = std::string( gadget.name );
            if ( gadget_name.empty( ) )
                gadget.name = "unknown";

            if ( gadget_name.find( "Skill017" ) != std::string::npos )
                gadget.name = "ADS";

            draw_cmds->text( smallest_p7, 10.f, screen, menu::vis::gadgets::name_col.value, gadget.name, true, true );
        }

        if( menu::vis::gadgets::distance.value )
            draw_cmds->text( smallest_p7, 10.f, math::vec2( screen.x, screen.y + 8.f ), menu::vis::gadgets::distance_col.value, std::format( "{}m", static_cast< int >( dist ) ), true, true );
    }

    aimbot::run( );

    /*
    auto local_animation = sdk::decrypt_animation_component( utils::read<uintptr_t>( sdk::ctx::local_player + sdk::offsets::animation_component ) );
    printf( "local_animation: 0x%llx\n", local_animation );

    auto nigger = utils::read<uintptr_t>( local_animation + 0xf90 );
    printf( "nigger: 0x%llx\n", nigger );

    auto pitch = utils::read<float>( nigger + 0x24 );
    auto yaw = utils::read<float>( nigger + 0x20 );

    printf( "pitch: %.3f | yaw: %.3f\n", pitch, yaw );
    */

/*
    if ( utils::key( VK_XBUTTON1 ).is_held( ) )
    {
        utils::kmb.left_click( 0 );
    }
*/

    return true;
}