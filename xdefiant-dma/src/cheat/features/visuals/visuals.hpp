#pragma once

#include "../aimbot/aimbot.hpp"

namespace visuals
{
	class c_player_esp
	{
	public:
		c_player_esp( actor_cache::f_agent player ) : m_player( player ) { }

	public:
		bool update_box( );
		void draw_box( const int box_type, const ImColor color );
		void draw_name( const ImColor color );
		void draw_weapon( const ImColor color );
		void draw_skeleton( const ImColor color, const float thickness );
		void draw_health_bar( const ImColor color_full, const ImColor color_empty );
		void draw_flags( const bool* flags, const ImColor color_full, const ImColor color_empty );
		void draw_snapline( int line_pos, const ImColor color );

	private:
		actor_cache::f_agent m_player;
		math::box m_bbox;
		float m_health_flag_offset = 3.f;
	};

	bool render( );
}