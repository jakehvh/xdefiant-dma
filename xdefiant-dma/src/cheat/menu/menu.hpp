#pragma once

#include "../../utils/utils.hpp"

#include "jakework/jakework.hpp"

namespace menu
{
    inline bool open = false;
    inline const char* tabs[ ] = { "Aim", "Visuals", "Misc", "Config" };
    inline int selected_tab = 0;

    void render( );


    namespace aim
    {
        inline const char* tabs[ ] = { "General" };
        inline int selected_tab = 0;

        inline jw::check_box_data_t enabled{ "Enabled", false };
        inline jw::key_bind_data_t key{ 0x0, 0, 0, false, false };
        inline jw::slider_data_t smooth_x{ "Smooth X", 1.f, 100.f, 0.f, "%", false };
        inline jw::slider_data_t smooth_y{ "Smooth Y", 1.f, 100.f, 0.f, "%", false };
        inline jw::slider_data_t fov{ "Fov", 0.f, 1000.f, 0.f, "px", false };
        inline jw::check_box_data_t draw_fov{ "Draw Fov", false };
        inline jw::color_picker_data_t fov_col{ { 1.f, 1.f, 1.f, 1.f }, false, false };
        inline jw::slider_data_t deadzone{ "Deadzone", 0.f, 1000.f, 0.f, "px", false };
        inline jw::check_box_data_t draw_deadzone{ "Draw Deadzone", false };
        inline jw::color_picker_data_t deadzone_col{ { 1.f, 0.f, 0.f, 1.f }, false, false };
        inline jw::multi_combo_data_t hitboxes{ "Hitboxes", { "Head", "Neck", "Chest", "Stomach", "Arms", "Hands", "Legs", "Feet" }, { false, false, false, false, false, false, false, false }, 8, false };
        inline jw::check_box_data_t cache_target{ "Lock Once", false };
        inline jw::check_box_data_t prioritize_dist{ "Prioritize Distance", false };
        inline jw::check_box_data_t psilent{ "PSilent", false };
        inline jw::check_box_data_t aim_line{ "Aim Line", false };
        inline jw::color_picker_data_t aim_line_col{ { 1.f, 1.f, 1.f, 1.f }, false, false };

        inline jw::check_box_data_t trigger_enabled{ "Enabled", false };
        inline jw::key_bind_data_t trigger_key{ 0x0, 0, 0, false, false };
        inline jw::slider_data_t trigger_delay{ "Delay", 0.f, 250.f, 0.f, "ms", false };
    }

    namespace vis
    {
        inline const char* tabs[ ] = { "General" };
        inline int selected_tab = 0;

        namespace player
        {
            inline jw::check_box_data_t bbox{ "Bounding Box", false };
            inline jw::color_picker_data_t bbox_col{ { 1.f, 1.f, 1.f, 1.f }, false, false };
            inline jw::combo_data_t bbox_type{ "Bounding Box Type", { "Full", "Cornered" }, 2, 0, false };
            inline jw::check_box_data_t name{ "Name", false };
			inline jw::color_picker_data_t name_col{ { 1.f, 1.f, 1.f, 1.f }, false, false };
            inline jw::check_box_data_t health{ "Health Bar", false };
            inline jw::color_picker_data_t health_col_full{ { 0.f, 1.f, 0.f, 1.f }, true, false };
            inline jw::color_picker_data_t health_col_empty{ { 1.f, 0.f, 0.f, 1.f }, false, false };
            inline jw::check_box_data_t skeleton{ "Skeleton", false };
            inline jw::color_picker_data_t skeleton_col{ { 1.f, 1.f, 1.f, 1.f }, false, false };
            inline jw::slider_data_t skeleton_thickness{ "Skeleton Thickness", 1.f, 5.f, 2.f, "px", false };
            inline jw::combo_data_t snaplines{ "Snaplines", { "Off", "Top", "Bottom" }, 3, 0, false };
            inline jw::color_picker_data_t snaplines_col{ { 1.f, 1.f, 1.f, 1.f }, false, false };
            inline jw::multi_combo_data_t flags{ "Flags", { "Distance", "Health", "Level", "Sprinting", "Crouched", "In Shield" }, { false, false, false, false, false, false }, 6, false };
            inline jw::slider_data_t max_dist{ "Max Distance", 10.f, 200.f, 10.f, "m", false };
        }

        namespace gadgets
        {
            inline jw::check_box_data_t name{ "Name", false };
            inline jw::color_picker_data_t name_col{ { 1.f, 1.f, 1.f, 1.f }, false, false };
            inline jw::check_box_data_t distance{ "Distance", false };
            inline jw::color_picker_data_t distance_col{ { 1.f, 1.f, 1.f, 1.f }, false, false };
            inline jw::slider_data_t max_dist{ "Max Distance", 10.f, 200.f, 10.f, "m", false };
        }

        namespace local
        {
            inline jw::check_box_data_t crosshair{ "Crosshair", false };
            inline jw::color_picker_data_t crosshair_col{ { 1.f, 1.f, 1.f, 1.f }, false, false };
            inline jw::combo_data_t viewmodel_fov{ "Viewmodel FOV", { "Off/Normal", "Super Short", "Short", "Long", "Super Long" }, 5, 0, false };
        }
    }

    namespace misc
    {
        inline const char* tabs[ ] = { "General" };
        inline int selected_tab = 0;

        inline jw::check_box_data_t no_recoil{ "Remove Recoil", false };
        inline jw::check_box_data_t no_sway{ "Remove Sway", false };
        inline jw::check_box_data_t no_spread{ "Remove Spread", false };
        inline jw::check_box_data_t remove_penalty{ "Remove Movement Penalty", false };
        inline jw::check_box_data_t instant_swap{ "Instant Swap", false };
        inline jw::check_box_data_t instant_reload{ "Instant Reload", false };
        inline jw::check_box_data_t instant_ads{ "Instant ADS", false };
        inline jw::check_box_data_t instant_fire{ "Instant Fire", false };
        inline jw::check_box_data_t modify_rpm{ "Modify RPM", false };
        inline jw::slider_data_t rpm_value{ "RPM Value", 1000.f, 5000.f, 1000.f, "rpm", false };
        inline jw::check_box_data_t modify_bullet_count{ "Modify Bullet Count", false };
        inline jw::slider_data_t bullet_count{ "Bullet Count", 1.f, 10.f, 1.f, nullptr, false };
        inline jw::combo_data_t jump_height{ "Aim Jump Height", { "Normal", "Double", "Triple", "Quadruple" }, 4, 0, false };
        inline jw::check_box_data_t omni_sprint{ "Sprint While Aiming", false };
        inline jw::check_box_data_t shoot_through_players{ "Shoot Through Players", false };
    }

    namespace cfg
    {
        inline const char* tabs[ ] = { "General" };
        inline int selected_tab = 0;

        inline jw::combo_data_t selected_cfg{ "Selected Config", { "Slot 1", "Slot 2", "Slot 3" }, 3, 0, false };

        inline jw::text_box_data_t kmbox_ip_ele { "Kmbox IP", "0.0.0.0", 0x10 };
        inline jw::text_box_data_t kmbox_port_ele{ "Kmbox Port", "0000", 0x6 };
        inline jw::text_box_data_t kmbox_uuid_ele{ "Kmbox UUID", "00000000", 0x9 };


        inline char kmbox_ip[ 0x10 ]{ };
        inline char kmbox_port[ 0x6 ]{ };
        inline char kmbox_uuid[ 0x9 ]{ };

        inline jw::slider_data_t cache_update_rate{ "Cache Update Rate", 100.f, 5000.f, 1000.f, "ms", false };
        inline jw::slider_data_t kmb_rate_limit{ "Kmbox B Update Rate", 1.f, 25.f, 1.f, "ms", false };

        inline jw::slider_data_t screen_width{ "Screen Width", 1280.f, 3840.f, 1280.f, "px", false };
        inline jw::slider_data_t screen_height{ "Screen Height", 720.f, 2160.f, 720.f, "px", false };
    }
}