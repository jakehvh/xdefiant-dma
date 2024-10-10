#include "menu.hpp"

#include "jakefig/jakefig.hpp"

std::once_flag config_init{ };

void menu::render( )
{
    std::call_once( config_init, [&]
    {
        jakefig::initialize( );
    } );

    auto insert_held = utils::key( VK_INSERT ).is_held( );
    static bool insert_updated = false;

    auto end_held = utils::key( VK_END ).is_held( );
    static bool end_updated = false;

    if ( end_held && !end_updated )
    {
        jw::main_pc_control = !jw::main_pc_control;
		end_updated = true;
	}

    if ( !end_held && end_updated )
		end_updated = false;

    if ( insert_held && !insert_updated )
    {
		open = !open;
		insert_updated = true;
	}

    if ( !insert_held && insert_updated )
        insert_updated = false;

    if ( !open )
        return;

    jw::begin( );
    jw::tabs( tabs, 4, &selected_tab );

    switch ( selected_tab )
    {
        case 0:
        {
            jw::sub_tabs( aim::tabs, 1, &aim::selected_tab );
            switch ( aim::selected_tab )
            {
                case 0:
                {
                    jw::child trigger( "Triggerbot", 0.f, 80.f, true );

                    trigger.check_box( &aim::trigger_enabled );
                    trigger.key_bind( &aim::trigger_key );
                    trigger.slider( &aim::trigger_delay );

                    jw::child general( "General", 0.f, 350.f );

                    general.check_box( &aim::enabled );
                    general.key_bind( &aim::key );
                    general.slider( &aim::smooth_x );
                    general.slider( &aim::smooth_y );
                    general.slider( &aim::fov );
                    general.check_box( &aim::draw_fov );
                    general.color_picker( &aim::fov_col );
                    general.slider( &aim::deadzone );
                    general.check_box( &aim::draw_deadzone );
                    general.color_picker( &aim::deadzone_col );
                    general.multi_combo( &aim::hitboxes );
                    general.check_box( &aim::cache_target, !aim::hitboxes.active );
                    general.check_box( &aim::prioritize_dist, !aim::hitboxes.active );
                    general.check_box( &aim::psilent, !aim::hitboxes.active );
                    general.check_box( &aim::aim_line, !aim::hitboxes.active );
                    general.color_picker( &aim::aim_line_col, !aim::hitboxes.active );

                    break;
                }
            }
            break;
        }

        case 1:
        {
            jw::sub_tabs( vis::tabs, 1, &vis::selected_tab );
            switch ( vis::selected_tab )
            {
                case 0:
                {
                    jw::child local( "Local", 110.f, 75.f, true );

                    local.check_box( &vis::local::crosshair );
                    local.color_picker( &vis::local::crosshair_col );
                    local.combo( &vis::local::viewmodel_fov );

                    jw::child gadgets( "Gadgets", 0.f, 90.f, true );

                    gadgets.check_box( &vis::gadgets::name );
                    gadgets.color_picker( &vis::gadgets::name_col );
                    gadgets.check_box( &vis::gadgets::distance );
                    gadgets.color_picker( &vis::gadgets::distance_col );
                    gadgets.slider( &vis::gadgets::max_dist );

                    jw::child players( "Players", 0.f, 320.f );

                    players.check_box( &vis::player::bbox );
                    players.color_picker( &vis::player::bbox_col );
                    players.combo( &vis::player::bbox_type );
                    players.check_box( &vis::player::name, !vis::player::bbox_type.active );
                    players.color_picker( &vis::player::name_col, !vis::player::bbox_type.active );
                    players.check_box( &vis::player::health, !vis::player::bbox_type.active );
                    players.color_picker( &vis::player::health_col_full, !vis::player::bbox_type.active );
                    players.color_picker( &vis::player::health_col_empty, !vis::player::bbox_type.active );
                    players.check_box( &vis::player::skeleton );
                    players.color_picker( &vis::player::skeleton_col );
                    players.slider( &vis::player::skeleton_thickness );
                    players.combo( &vis::player::snaplines );
                    players.text( "Snaplines Color", !vis::player::snaplines.active );
                    players.color_picker( &vis::player::snaplines_col, !vis::player::snaplines.active );
                    players.multi_combo( &vis::player::flags, !vis::player::snaplines.active );
                    players.slider( &vis::player::max_dist, !vis::player::flags.active );

                    break;
                }
            }
            break;
        }

        case 2:
        {
            jw::sub_tabs( misc::tabs, 1, &misc::selected_tab );
            switch ( misc::selected_tab )
            {
                case 0:
                {
                    jw::child general( "General", 0.f, 330.f ); //370.f

                    //general.check_box( &misc::no_recoil );
                    //general.check_box( &misc::no_sway );
                    general.check_box( &misc::no_spread );
                    general.check_box( &misc::remove_penalty );
                    general.check_box( &misc::instant_swap );
                    general.check_box( &misc::instant_reload );
                    general.check_box( &misc::instant_ads );
                    general.check_box( &misc::instant_fire );
                    general.check_box( &misc::modify_rpm );
                    general.slider( &misc::rpm_value );
                    general.check_box( &misc::modify_bullet_count );
                    general.slider( &misc::bullet_count );
                    general.combo( &misc::jump_height );
                    general.check_box( &misc::omni_sprint, !misc::jump_height.active );
                    general.check_box( &misc::shoot_through_players, !misc::jump_height.active );

                    break;
                }
            }
            break;
        }

        case 3:
        {
            jw::sub_tabs( cfg::tabs, 1, &cfg::selected_tab );
            switch ( cfg::selected_tab )
            {
                case 0:
                {
                    jw::child performance( "Performance", 0.f, 160.f, true );

                    performance.slider( &cfg::cache_update_rate );
                    performance.slider( &cfg::kmb_rate_limit );

                    performance.slider( &cfg::screen_width );
                    performance.slider( &cfg::screen_height );

                    jw::child general( "General", 0.f, 295.f );

                    general.combo( &cfg::selected_cfg );

                    if ( general.button( "Save", !cfg::selected_cfg.active ) )
                    {
                        auto path = std::filesystem::current_path( ).string( ) + "\\paionia\\" + "slot" + std::to_string( cfg::selected_cfg.value + 1 ) + ".pi";
                        jakefig::save( path );
                    }

                    if ( general.button( "Load", !cfg::selected_cfg.active ) )
                    {
                        auto path = std::filesystem::current_path( ).string( ) + "\\paionia\\" + "slot" + std::to_string( cfg::selected_cfg.value + 1 ) + ".pi";
                        jakefig::load( path );
					}

                    general.text_box( &cfg::kmbox_ip_ele, cfg::kmbox_ip );
                    general.text_box( &cfg::kmbox_port_ele, cfg::kmbox_port );
                    general.text_box( &cfg::kmbox_uuid_ele, cfg::kmbox_uuid );

                    if ( general.button( "Connect" ) )
                    {
                        auto path = std::filesystem::current_path( ).string( ) + "\\paionia\\kmbox.pi";
                        
                        nlohmann::json cfg = { };
                        cfg[ "kmboxb" ][ "baudrate" ] = 115200;
                        cfg[ "kmboxnet" ][ "ip" ] = cfg::kmbox_ip;
                        cfg[ "kmboxnet" ][ "port" ] = cfg::kmbox_port;
                        cfg[ "kmboxnet" ][ "uuid" ] = cfg::kmbox_uuid;
                        std::ofstream out( path );
                        out << cfg.dump( 4 );
                        out.close( );

                        utils::kmb.is_connected = utils::kmb.init( );
                    }

                    break;
                }
            }
            break;
        }
    }

    jw::end( );
}