#include "jakework.hpp"																		\

POINT get_cursor_pos( )
{
	POINT p;
	GetCursorPos( &p );
	return p;
}

bool jw::mouse_in_bounds( math::vec2 pos, math::vec2 size )
{
	auto mouse_pos = main_pc_control ? utils::cursor::get_pos( ) : get_cursor_pos( );
	if ( mouse_pos.x > pos.x && mouse_pos.y > pos.y && mouse_pos.x < pos.x + size.x && mouse_pos.y < pos.y + size.y )
		return true;

	return false;
}

bool jw::is_key_held( int vkey )
{
	return main_pc_control ? utils::key( vkey ).is_held( ) : GetAsyncKeyState( vkey ) & 0x8000;
}

void jw::watermark( )
{
	//draw_cmds->text( arial_small, 12.f, utils::user_info.is_reseller_build ? L"SoTCheat | SoT | " __DATE__ : L"undetected.ac | SoT | " __DATE__, math::vec2( 5.f, 5.f ), accent_col, false, true );
}

void jw::begin( )
{
	current_time = std::chrono::duration_cast< std::chrono::milliseconds >( std::chrono::system_clock::now( ).time_since_epoch( ) ).count( );

	auto mouse_pos = main_pc_control ? utils::cursor::get_pos( ) : get_cursor_pos( );
	if ( mouse_in_bounds( pos, math::vec2( size.x - 200.f, 30.f ) ) && is_key_held( 0x01 ) )
		drag = true;

	static math::vec2 offset{ };
	if ( drag )
	{
		if ( offset.x == 0.f )
		{
			offset.x = mouse_pos.x - pos.x;
			offset.y = mouse_pos.y - pos.y;
		}

		if ( is_key_held( 0x01 ) )
		{
			pos.x = mouse_pos.x - offset.x;
			pos.y = mouse_pos.y - offset.y;
		}
		else
			drag = false;
	}
	else
		offset = { 0.f, 0.f };


	draw_cmds->rect( pos, size, ImColor( 10, 10, 10, 255 ) );
	draw_cmds->rect_filled( pos, size, ImColor( 20, 20, 20, 255 ) );

	draw_cmds->rect_filled( pos, math::vec2( size.x, 25.f ), ImColor( 35, 35, 35, 255 ) );
	draw_cmds->line( math::vec2( pos.x, pos.y + 25.f ), math::vec2( pos.x + size.x, pos.y + 25.f ), ImColor( 10, 10, 10, 255 ) );
	draw_cmds->text( arial_medium, 14.f, math::vec2( pos.x + 5.f, pos.y + 5.f ), accent_col, "PAIONIA.GG" );

	draw_cmds->rect_filled( math::vec2( pos.x, pos.y + size.y - 25.f ), math::vec2( size.x, 25.f ), ImColor( 35, 35, 35, 255 ) );
	draw_cmds->line( math::vec2( pos.x, pos.y + size.y - 25.f ), math::vec2( pos.x + size.x, pos.y + size.y - 25.f ), ImColor( 10, 10, 10, 255 ) );


	auto text = utils::kmb.is_connected ? "Connected" : "Not Connected";
	auto text_sz = arial_medium->CalcTextSizeA( 14.f, FLT_MAX, 0.f, text );
	auto text_sz2 = arial_medium->CalcTextSizeA( 14.f, FLT_MAX, 0.f, "Kmbox: " );

	draw_cmds->text( arial_medium, 14.f, math::vec2( pos.x + size.x - text_sz.x - 5.f, pos.y + size.y - 20.f ), utils::kmb.is_connected ? ImColor( 0.f, 1.f, 0.f, 1.f ) : ImColor( 1.f, 0.f, 0.f, 1.f ), text );
	draw_cmds->text( arial_medium, 14.f, math::vec2( pos.x + size.x - text_sz2.x - text_sz.x - 5.f, pos.y + size.y - 20.f ), accent_col, "Kmbox: " );
}

void jw::end( )
{
	auto mouse_pos = main_pc_control ? utils::cursor::get_pos( ) : get_cursor_pos( );

	draw_cmds->rect_filled( math::vec2( mouse_pos.x, mouse_pos.y ), math::vec2( 7.f, 7.f ), ImColor( 10, 10, 10, 255 ) );
	draw_cmds->rect_filled( math::vec2( mouse_pos.x + 1.f, mouse_pos.y + 1.f ), math::vec2( 5.f, 5.f ), accent_col );
}

void jw::tabs( const char* tabs[ 0x10 ], int tabs_size, int* selected_tab )
{
	float x_offset = 15.f;
	for ( int i = 0; i < tabs_size; i++ )
	{
		auto im_sz = arial_medium->CalcTextSizeA( 14.f, FLT_MAX, 0.f, tabs[ i ] );
		auto text_sz = math::vec2( im_sz.x, im_sz.y );
		text_sz += math::vec2( 10.f, 5.f );

		x_offset += text_sz.x;
	}

	for ( int i = 0; i < tabs_size; i++ )
	{
		auto im_sz = arial_medium->CalcTextSizeA( 14.f, FLT_MAX, 0.f, tabs[ i ] );
		auto text_sz = math::vec2( im_sz.x, im_sz.y );
		text_sz += math::vec2( 10.f, 5.f );

		float pos_x = pos.x + ( size.x - x_offset ) + ( i * 50.f ) - ( text_sz.x / 2.f );
		float pos_y = pos.y + 15.f - ( text_sz.y / 2.f );

		if ( mouse_in_bounds( math::vec2( pos_x, pos_y ), text_sz ) && is_key_held( 0x01 ) )
			*selected_tab = i;

		if ( i == *selected_tab )
		{
			draw_cmds->rect_filled( math::vec2( pos_x, pos_y ), math::vec2( text_sz.x, text_sz.y + 3.f ), ImColor( 20, 20, 20, 255 ) );
			draw_cmds->rect_filled( math::vec2( pos_x, pos_y ), math::vec2( text_sz.x, 1.f ), accent_col );
			draw_cmds->line( math::vec2( pos_x, pos_y ), math::vec2( pos_x, pos_y + text_sz.y ), ImColor( 10, 10, 10, 255 ) );
			draw_cmds->line( math::vec2( pos_x + text_sz.x, pos_y ), math::vec2( pos_x + text_sz.x, pos_y + text_sz.y ), ImColor( 10, 10, 10, 255 ) );
		}

		draw_cmds->text( arial_medium, 14.f, math::vec2( pos.x + ( size.x - x_offset ) + ( i * 50.f ), pos.y + 20.f ), i == *selected_tab ? accent_col : ImColor( 100, 100, 100, 255 ), tabs[ i ], false, true );
	}
}

void jw::sub_tabs( const char* sub_tabs[ 0x10 ], int sub_tabs_size, int* selected_sub_tab )
{
	for ( int i = 0; i < sub_tabs_size; i++ )
	{
		auto im_sz = arial_medium->CalcTextSizeA( 14.f, FLT_MAX, 0.f, sub_tabs[ i ] );
		auto text_sz = math::vec2( im_sz.x, im_sz.y );
		text_sz += math::vec2( 10.f, 5.f );

		float pos_x = pos.x + 30.f + ( i * 50.f ) - ( text_sz.x / 2.f );
		float pos_y = pos.y + size.y - 15.f - ( text_sz.y / 2.f );

		if ( mouse_in_bounds( math::vec2( pos_x, pos_y ), text_sz ) && is_key_held( 0x01 ) )
			*selected_sub_tab = i;

		if ( i == *selected_sub_tab )
		{
			draw_cmds->rect_filled( math::vec2( pos_x, pos_y ), text_sz, ImColor( 20, 20, 20, 255 ) );
			draw_cmds->rect_filled( math::vec2( pos_x, pos_y + text_sz.y - 1.f ), math::vec2( text_sz.x, 1.f ), accent_col );
			draw_cmds->line( math::vec2( pos_x, pos_y ), math::vec2( pos_x, pos_y + text_sz.y ), ImColor( 10, 10, 10, 255 ) );
			draw_cmds->line( math::vec2( pos_x + text_sz.x, pos_y ), math::vec2( pos_x + text_sz.x, pos_y + text_sz.y ), ImColor( 10, 10, 10, 255 ) );
		}

		draw_cmds->text( arial_medium, 14.f, math::vec2( pos.x + 30.f + ( i * 50.f ), pos.y + size.y - 10.f ), i == *selected_sub_tab ? accent_col : ImColor( 100, 100, 100, 255 ), sub_tabs[ i ], false, true );
	}
}

jw::child::child( const char* label, int offset_from_top, int size_y, bool right )
{
	int y = pos.y + 45.f + offset_from_top;

	if ( right )
	{
		auto child_pos = math::vec2( pos.x + 310.f, y );
		auto child_size = math::vec2( 270.f, size_y );

		draw_cmds->rect( child_pos, child_size, ImColor( 10, 10, 10, 255 ) );
		auto text_sz = arial_medium->CalcTextSizeA( 14.f, FLT_MAX, 0.f, label );
		draw_cmds->text( arial_medium, 14.f, math::vec2( child_pos.x + 15.f, child_pos.y - static_cast< int >( ( text_sz.y / 2.f ) ) ), ImColor( 100, 100, 100, 255 ), label );
	}
	else
	{
		auto child_pos = math::vec2( pos.x + 20.f, y );
		auto child_size = math::vec2( 270.f, size_y );

		draw_cmds->rect( child_pos, child_size, ImColor( 10, 10, 10, 255 ) );
		auto text_sz = arial_medium->CalcTextSizeA( 14.f, FLT_MAX, 0.f, label );
		draw_cmds->text( arial_medium, 14.f, math::vec2( child_pos.x + 15.f, child_pos.y - static_cast< int >( ( text_sz.y / 2.f ) ) ), ImColor( 100, 100, 100, 255 ), label );
	}

	offset = math::vec2( right ? 320.f : 30.f, y + 16.f );
}

void jw::child::text( const char* label, bool should_render )
{
	if ( should_render )
		draw_cmds->text( arial_small, 12.f, math::vec2( pos.x + offset.x, offset.y - 5.f ), ImColor( 255, 255, 255, 255 ), label );

	offset.y += 15.f;
}

void jw::child::check_box( check_box_data_t* check_box_data, bool should_render )
{
	if ( !should_render )
	{
		offset.y += 20.f;
		return;
	}

	draw_cmds->rect_filled( math::vec2( pos.x + offset.x, offset.y ), math::vec2( 10.f, 10.f ), ImColor( 10, 10, 10, 255 ) );
	draw_cmds->rect_filled( math::vec2( pos.x + offset.x + 1.f, offset.y + 1.f ), math::vec2( 8.f, 8.f ), check_box_data->value ? accent_col : ImColor( 35, 35, 35, 255 ) );
	draw_cmds->text( arial_small, 12.f, math::vec2( pos.x + offset.x + 15.f, offset.y - 1.f ), check_box_data->value ? ImColor( 255, 255, 255, 255 ) : ImColor( 100, 100, 100, 255 ), check_box_data->label );

	if ( mouse_in_bounds( math::vec2( pos.x + offset.x, offset.y ), math::vec2( 10.f, 10.f ) ) && is_key_held( VK_LBUTTON ) && !pop_up_open && current_time > last_time )
	{
		check_box_data->value = !check_box_data->value;
		last_time = current_time + 100;
	}

	offset.y += 20.f;
}

void jw::child::slider( slider_data_t* slider_data, bool should_render )
{
	if ( !should_render )
	{
		offset.y += 35.f;
		return;
	}

	draw_cmds->text( arial_small, 12.f, math::vec2( pos.x + offset.x, offset.y - 3.f ), slider_data->value == slider_data->min ? ImColor( 100, 100, 100, 255 ) : ImColor( 255, 255, 255, 255 ), slider_data->label );
	draw_cmds->rect_filled( math::vec2( pos.x + offset.x, offset.y + 15.f ), math::vec2( 250.f, 10.f ), ImColor( 10, 10, 10, 255 ) );
	draw_cmds->rect_filled( math::vec2( pos.x + offset.x + 1.f, offset.y + 16.f ), math::vec2( 248.f, 8.f ), ImColor( 35, 35, 35, 255 ) );

	float d_x = 0.f;
	auto mouse_pos = main_pc_control ? utils::cursor::get_pos( ) : get_cursor_pos( );

	if ( mouse_pos.x >= ( pos.x + offset.x + 1.f ) )
		d_x = static_cast< float >( mouse_pos.x - ( pos.x + offset.x + 1.f ) );

	if ( d_x > 248.f )
		d_x = 248.f;

	bool in_bounds = mouse_in_bounds( math::vec2( pos.x + offset.x, offset.y + 12.f ), math::vec2( 250.f, 10.f ) ) && !pop_up_open;
	if ( in_bounds && is_key_held( 0x01 ) )
		slider_data->sliding = true;

	if ( slider_data->sliding )
	{
		slider_data->value = slider_data->max * ( d_x / 248.f );

		if ( !is_key_held( 0x01 ) )
			slider_data->sliding = false;
	}

	if ( in_bounds && is_key_held( 0x25 ) && current_time > last_time )
	{
		slider_data->value = slider_data->value - 1.f;
		last_time = current_time + 100;
	}

	if ( in_bounds && is_key_held( 0x27 ) && current_time > last_time )
	{
		slider_data->value = slider_data->value + 1.f;
		last_time = current_time + 100;
	}

	if ( slider_data->value > slider_data->max )
		slider_data->value = slider_data->max;

	if ( slider_data->value < slider_data->min )
		slider_data->value = slider_data->min;

	auto value_txt = std::format( "{}{}", static_cast< int >( slider_data->value ), slider_data->fmt != nullptr ? slider_data->fmt : "" );
	auto value_txt_sz = arial_small->CalcTextSizeA( 12.f, FLT_MAX, 0.f, value_txt.c_str( ) );

	draw_cmds->text( arial_small, 12.f, math::vec2( ( pos.x + offset.x + 248.f ) - value_txt_sz.x, offset.y - 3.f ), slider_data->value == slider_data->min ? ImColor( 100, 100, 100, 255 ) : ImColor( 255, 255, 255, 255 ), value_txt );
	draw_cmds->rect_filled( math::vec2( pos.x + offset.x + 1.f, offset.y + 16.f ), math::vec2( static_cast< double >( ( slider_data->value / slider_data->max ) ) * 248.f, 8.f ), accent_col );

	offset.y += 35.f;
}

void jw::child::combo( combo_data_t* combo_data, bool should_render )
{
	if ( !should_render )
	{
		offset.y += 45.f;
		return;
	}

	draw_cmds->text( arial_small, 12.f, math::vec2( pos.x + offset.x, offset.y - 3.f ), ImColor( 255, 255, 255, 255 ), combo_data->label );
	draw_cmds->rect_filled( math::vec2( pos.x + offset.x, offset.y + 15.f ), math::vec2( 250.f, 20.f ), ImColor( 10, 10, 10, 255 ) );
	draw_cmds->rect_filled( math::vec2( pos.x + offset.x + 1.f, offset.y + 16.f ), math::vec2( 248.f, 18.f ), ImColor( 35, 35, 35, 255 ) );

	draw_cmds->text( arial_small, 12.f, math::vec2( pos.x + offset.x + 3.f, offset.y + 19.f ), ImColor( 255, 255, 255, 255 ), combo_data->items[ combo_data->value ] );
	draw_cmds->text( arial_small, 12.f, math::vec2( pos.x + offset.x + 238.f, offset.y + 19.f ), ImColor( 255, 255, 255, 255 ), combo_data->active ? "^" : "<" );

	bool in_bounds = mouse_in_bounds( math::vec2( pos.x + offset.x, offset.y + 15.f ), math::vec2( 250.f, 20.f ) ) && !pop_up_open;
	if ( in_bounds && is_key_held( 0x01 ) )
		combo_data->active = true;

	if ( combo_data->active )
	{
		pop_up_open = true;
		draw_cmds->rect_filled( math::vec2( pos.x + offset.x, offset.y + 35.f ), math::vec2( 250.f, ( combo_data->items_count * 15.f ) + 5.f ), ImColor( 10, 10, 10, 255 ) );
		draw_cmds->rect_filled( math::vec2( pos.x + offset.x + 1.f, offset.y + 36.f ), math::vec2( 248.f, ( combo_data->items_count * 15.f ) + 3.f ), ImColor( 35, 35, 35, 255 ) );

		for ( int i = 0; i < combo_data->items_count; i++ )
		{
			auto text_pos = math::vec2( pos.x + offset.x + 3.f, offset.y + 37.f + ( i * 15.f ) );
			auto text_sz = arial_small->CalcTextSizeA( 12.f, FLT_MAX, 0.f, combo_data->items[ i ] );

			draw_cmds->text( arial_small, 12.f, text_pos, i == combo_data->value ? accent_col : ImColor( 100, 100, 100, 255 ), combo_data->items[ i ] );

			if ( mouse_in_bounds( text_pos, math::vec2( text_sz.x, text_sz.y ) ) && is_key_held( 0x01 ) )
			{
				combo_data->value = i;
				combo_data->active = false;
				pop_up_open = false;
			}
		}
	}

	offset.y += 45.f;
}

void jw::child::multi_combo( multi_combo_data_t* multi_combo_data, bool should_render )
{
	int total_selected = 0;

	for ( int i = 0; i < multi_combo_data->count; i++ )
		if ( multi_combo_data->value[ i ] )
			total_selected++;

	auto total_selected_txt = std::format( "{} Selected", total_selected );

	if ( !should_render )
	{
		offset.y += 45.f;
		return;
	}

	draw_cmds->text( arial_small, 12.f, math::vec2( pos.x + offset.x, offset.y - 3.f ), total_selected == 0 ? ImColor( 100, 100, 100, 255 ) : ImColor( 255, 255, 255, 255 ), multi_combo_data->label );
	draw_cmds->rect_filled( math::vec2( pos.x + offset.x, offset.y + 15.f ), math::vec2( 250.f, 20.f ), ImColor( 10, 10, 10, 255 ) );
	draw_cmds->rect_filled( math::vec2( pos.x + offset.x + 1.f, offset.y + 16.f ), math::vec2( 248.f, 18.f ), ImColor( 35, 35, 35, 255 ) );

	draw_cmds->text( arial_small, 12.f, math::vec2( pos.x + offset.x + 3.f, offset.y + 19.f ), total_selected == 0 ? ImColor( 100, 100, 100, 255 ) : ImColor( 255, 255, 255, 255 ), total_selected_txt );
	draw_cmds->text( arial_small, 12.f, math::vec2( pos.x + offset.x + 238.f, offset.y + 19.f ), total_selected == 0 ? ImColor( 100, 100, 100, 255 ) : ImColor( 255, 255, 255, 255 ), multi_combo_data->active ? "^" : "<" );

	bool in_bounds = mouse_in_bounds( math::vec2( pos.x + offset.x, offset.y + 15.f ), math::vec2( 250.f, 20.f ) );
	if ( in_bounds && is_key_held( 0x01 ) && !pop_up_open )
		multi_combo_data->active = true;

	if ( multi_combo_data->active )
	{
		pop_up_open = true;
		draw_cmds->rect_filled( math::vec2( pos.x + offset.x, offset.y + 35.f ), math::vec2( 250.f, ( multi_combo_data->count * 15.f ) + 5.f ), ImColor( 10, 10, 10, 255 ) );
		draw_cmds->rect_filled( math::vec2( pos.x + offset.x + 1.f, offset.y + 36.f ), math::vec2( 248.f, ( multi_combo_data->count * 15.f ) + 3.f ), ImColor( 35, 35, 35, 255 ) );

		for ( int i = 0; i < multi_combo_data->count; i++ )
		{
			auto text_pos = math::vec2( pos.x + offset.x + 3.f, offset.y + 37.f + ( i * 15.f ) );
			auto text_sz = arial_small->CalcTextSizeA( 12.f, FLT_MAX, 0.f, multi_combo_data->items[ i ] );

			draw_cmds->text( arial_small, 12.f, text_pos, multi_combo_data->value[ i ] ? accent_col : ImColor( 100, 100, 100, 255 ), multi_combo_data->items[ i ] );

			if ( mouse_in_bounds( text_pos, math::vec2( text_sz.x, text_sz.y ) ) && is_key_held( 0x01 ) && current_time > last_time )
			{
				multi_combo_data->value[ i ] = !multi_combo_data->value[ i ];
				last_time = current_time + 100;
			}
		}
	}

	if ( !mouse_in_bounds( math::vec2( pos.x + offset.x, offset.y + 15.f ), math::vec2( 250.f, ( multi_combo_data->count * 15.f ) + 25.f ) ) && is_key_held( 0x01 ) )
	{
		multi_combo_data->active = false;
		pop_up_open = false;
	}

	offset.y += 45.f;
}

bool jw::child::button( const char* label, bool should_render )
{
	if ( !should_render )
	{
		offset.y += 30.f;
		return false;
	}

	bool pressed = false;

	if ( mouse_in_bounds( math::vec2( pos.x + offset.x, offset.y ), math::vec2( 250.f, 20.f ) ) && is_key_held( 0x01 ) && !pop_up_open && current_time > last_time )
	{
		pressed = true;
		last_time = current_time + 100;
	}

	draw_cmds->rect_filled( math::vec2( pos.x + offset.x, offset.y ), math::vec2( 250.f, 20.f ), pressed ? accent_col : ImColor( 10, 10, 10, 255 ) );
	draw_cmds->rect_filled( math::vec2( pos.x + offset.x + 1.f, offset.y + 1.f ), math::vec2( 248.f, 18.f ), ImColor( 35, 35, 35, 255 ) );
	draw_cmds->text( arial_small, 12.f, math::vec2( pos.x + offset.x + 125.f, offset.y + 16.f ), ImColor( 100, 100, 100, 255 ), label, false, true );

	offset.y += 30.f;

	return pressed;
}

void jw::child::text_box( text_box_data_t* text_box_data, char* value, bool should_render )
{
	if ( !should_render )
	{
		offset.y += 45.f;
		return;
	}

	draw_cmds->text( arial_small, 12.f, math::vec2( pos.x + offset.x, offset.y - 3.f ), ImColor( 255, 255, 255, 255 ), text_box_data->label );
	draw_cmds->rect_filled( math::vec2( pos.x + offset.x, offset.y + 15.f ), math::vec2( 250.f, 20.f ), text_box_data->active ? accent_col : ImColor( 10, 10, 10, 255 ) );
	draw_cmds->rect_filled( math::vec2( pos.x + offset.x + 1.f, offset.y + 16.f ), math::vec2( 248.f, 18.f ), ImColor( 35, 35, 35, 255 ) );

	draw_cmds->text( arial_small, 12.f, math::vec2( pos.x + offset.x + 3.f, offset.y + 19.f ), value[ 0 ] != NULL ? ImColor( 255, 255, 255, 255 ) : ImColor( 100, 100, 100, 255 ), value[ 0 ] != NULL ? value : text_box_data->preview_value );

	bool in_bounds = mouse_in_bounds( math::vec2( pos.x + offset.x, offset.y + 15.f ), math::vec2( 250.f, 20.f ) ) && !pop_up_open;
	if ( in_bounds && is_key_held( 0x01 ) )
	{
		text_box_data->active = true;
		last_time = current_time + 100;
	}

	if ( text_box_data->active )
	{
		pop_up_open = true;

		for ( auto i = 0; i < key_codes.size( ); i++ )
		{
			if ( text_box_data->index >= text_box_data->size )
				continue;

			if ( is_key_held( key_codes[ i ] ) && current_time > last_time )
			{
				value[ text_box_data->index ] = key_names[ i ];
				text_box_data->index++;

				last_time = current_time + 300;
			}
		}

		if ( is_key_held( VK_BACK ) && current_time > last_time && text_box_data->index >= 0 )
		{
			value[ text_box_data->index ] = NULL;
			text_box_data->index--;

			last_time = current_time + 300;
		}

		if ( ( !mouse_in_bounds( math::vec2( pos.x + offset.x, offset.y + 15.f ), math::vec2( 250.f, 20.f ) ) && is_key_held( 0x01 ) ) || is_key_held( VK_RETURN ) )
		{
			text_box_data->active = false;
			pop_up_open = false;
		}
	}

	offset.y += 45.f;
}

void jw::child::key_bind( key_bind_data_t* key_bind_data, bool should_render )
{
	auto display_txt = std::format( "[{}]", key_bind_data->in_bind ? "press key" : keys[ key_bind_data->key ] );
	auto display_txt_sz = arial_small->CalcTextSizeA( 12.f, FLT_MAX, 0.f, display_txt.c_str( ) );

	bool in_bounds = mouse_in_bounds( math::vec2( ( pos.x + offset.x + 250.f ) - display_txt_sz.x, offset.y - 23.f ), math::vec2( display_txt_sz.x, display_txt_sz.y ) );
	if ( in_bounds && is_key_held( 0x01 ) && !pop_up_open )
	{
		key_bind_data->in_bind = true;
		last_time = current_time + 300;
	}

	if ( key_bind_data->in_bind && current_time > last_time )
	{
		for ( auto i = 0x01; i <= 0xa5; i++ )
		{
			if ( is_key_held( i ) )
			{
				if ( i == 0x1b )
					key_bind_data->key = 0;
				else
					key_bind_data->key = i;

				key_bind_data->in_bind = false;
			}
		}
	}

	if ( should_render )
		draw_cmds->text( arial_small, 12.f, math::vec2( ( pos.x + offset.x + 250.f ) - display_txt_sz.x, offset.y - 21.f ), key_bind_data->in_bind ? accent_col : key_bind_data->key != 0 ? ImColor( 255, 255, 255, 255 ) : ImColor( 100, 100, 100, 255 ), display_txt );

	/*
		if ( key_bind_data->in_mode )
	{
		pop_up_open = true;
		canvas->rect_filled( math::vec2( pos.x + offset.x + 175.f, offset.y - 17.f ), math::vec2( 50.f, 35.f ), ImColor( 30.f, 30.f, 30.f, 255.f ) );
		canvas->rect_filled( math::vec2( pos.x + offset.x + 176.f, offset.y - 16.f ), math::vec2( 48.f, 33.f ), ImColor( 5.f, 5.f, 5.f, 255.f ) );

		for ( int i = 0; i < 2; i++ )
		{
			auto text_pos = math::vec2( pos.x + offset.x + 180.f, ( offset.y - 12.f ) + ( i * 12.f ) );
			auto text_sz = canvas->text_size( sdk::fonts[ sdk::roboto_tiny ], 1.f, key_modes[ i ] );

			canvas->text( sdk::fonts[ sdk::roboto_tiny ], 1.f, key_modes[ i ], text_pos, key_bind_data->mode == i ? accent_col : ImColor( 100, 100, 100, 255 ) );

			if ( mouse_in_bounds( text_pos, text_sz ) && is_key_held( 0x01 ).is_pressed( ) )
			{
				key_bind_data->mode = i;
				key_bind_data->in_mode = false;
				pop_up_open = false;
			}
		}
	}
	*/
}

void jw::child::color_picker( color_picker_data_t* color_picker_data, bool should_render )
{
	auto color_picker_slider = [ ]( const char* label, math::vec2 widget_pos, float* value, double x_offset )->float
	{
		draw_cmds->text( arial_small, 12.f, math::vec2( widget_pos.x + x_offset + 1.f, widget_pos.y + 2.f ), ImColor( 1.f, 1.f, 1.f, 1.f ), label );
		draw_cmds->rect_filled( math::vec2( widget_pos.x + x_offset, widget_pos.y + 15.f ), math::vec2( 10.f, 100.f ), ImColor( 10, 10, 10, 255 ) );
		draw_cmds->rect_filled( math::vec2( widget_pos.x + x_offset + 1.f, widget_pos.y + 16.f ), math::vec2( 8.f, 98.f ), ImColor( 50, 50, 50, 255 ) );

		float d_y = 0.f;
		auto mouse_widget_pos = main_pc_control ? utils::cursor::get_pos( ) : get_cursor_pos( );

		if ( mouse_widget_pos.y >= ( widget_pos.y + 16.f ) )
			d_y = static_cast< float >( mouse_widget_pos.y - ( widget_pos.y + 16.f ) );

		if ( d_y > 98.f )
			d_y = 98.f;

		bool in_bounds = mouse_in_bounds( math::vec2( widget_pos.x + x_offset, widget_pos.y + 15.f ), math::vec2( 10.f, 100.f ) );
		if ( in_bounds && is_key_held( 0x01 ) )
			*value = 255.f * ( d_y / 98.f );

		if ( in_bounds && is_key_held( 0x26 ) )
			*value = *value - 1.f;

		if ( in_bounds && is_key_held( 0x28 ) )
			*value = *value + 1.f;

		if ( *value > 255.f )
			*value = 255.f;

		if ( *value < 0.f )
			*value = 0.f;

		draw_cmds->rect_filled( math::vec2( widget_pos.x + x_offset + 1.f, widget_pos.y + 16.f ), math::vec2( 8.f, static_cast< double >( ( *value / 255.f ) ) * 98.f ), accent_col );
		draw_cmds->text( arial_small, 12.f, math::vec2( ( widget_pos.x + x_offset + 5.f ), widget_pos.y + 126.f ), ImColor( 1.f, 1.f, 1.f, 1.f ), std::format( "{}", static_cast< int >( *value ) ), false, true );

		return *value;
	};

	auto widget_pos = math::vec2( pos.x + offset.x + ( color_picker_data->second_col ? 205.f : 230.f ), offset.y - 20.f );
	auto widget_sz = math::vec2( 20.f, 10.f );

	if ( should_render )
	{
		draw_cmds->rect_filled( widget_pos, widget_sz, ImColor( 10, 10, 10, 255 ) );
		draw_cmds->rect_filled( math::vec2( widget_pos.x + 1.f, widget_pos.y + 1.f ), math::vec2( 18.f, 8.f ), color_picker_data->value );
	}

	bool in_bounds = mouse_in_bounds( widget_pos, widget_sz );
	if ( in_bounds && is_key_held( 0x01 ) && !pop_up_open )
	{
		auto col = color_picker_data->value;

		color_picker_data->old_value = { col.Value.x * 255.f, col.Value.y * 255.f, col.Value.z * 255.f, col.Value.w * 255.f };
		color_picker_data->active = true;
	}

	if ( color_picker_data->active )
	{
		pop_up_open = true;
		draw_cmds->rect_filled( math::vec2( widget_pos.x + 25.f, widget_pos.y ), math::vec2( 82.f, 130.f ), ImColor( 10, 10, 10, 255 ) );
		draw_cmds->rect_filled( math::vec2( widget_pos.x + 26.f, widget_pos.y + 1.f ), math::vec2( 80.f, 128.f ), ImColor( 35, 35, 35, 255 ) );

		float r = color_picker_slider( "R", widget_pos, &color_picker_data->old_value.r, 31.f );
		float g = color_picker_slider( "G", widget_pos, &color_picker_data->old_value.g, 51.f );
		float b = color_picker_slider( "B", widget_pos, &color_picker_data->old_value.b, 71.f );
		float a = color_picker_slider( "A", widget_pos, &color_picker_data->old_value.a, 91.f );

		color_picker_data->value.Value.x = r / 255.f;
		color_picker_data->value.Value.y = g / 255.f;
		color_picker_data->value.Value.z = b / 255.f;
		color_picker_data->value.Value.w = a / 255.f;

		if ( !mouse_in_bounds( math::vec2( widget_pos.x + 5.f, widget_pos.y ), math::vec2( 95.f, 130.f ) ) && is_key_held( 0x01 ) )
		{
			color_picker_data->active = false;
			pop_up_open = false;
		}
	}
}