#pragma once

#include "../../../sdk/sdk.hpp"
#include "../../../utils/dependencies/imgui/draw_cmds.hpp"

//JAKEWORK RAHHHHHHHHHHHHHH

namespace jw
{
	inline const char* user_name = nullptr;

	inline bool drag = false;
	inline bool main_pc_control = true;
	inline bool pop_up_open = false;

	inline math::vec2 pos{ 200.f, 500.f };
	inline math::vec2 size{ 600.f, 450.f };

	inline ImColor accent_col = ImColor( 252, 107, 129, 255 );

	inline long long current_time = 0ll;
	inline long long last_time = 0ll;

	inline const char* keys[ ] = { "none", "m1", "m2", "cn", "m3", "m4", "m5", "-", "bac", "tab", "-", "-", "clr", "ret", "-", "-", "shi", "ct", "alt", "pau", "cap", "kan", "-", "jun", "fin", "kan", "-", "esc", "con", "nco", "acc", "mad", "spa", "pgu", "pgd", "end", "hom", "lef", "up", "rig", "dow", "se", "pri", "exe", "pri", "ins", "de", "he", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "-", "-", "-", "-", "-", "-", "-", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "win", "win", "app", "-", "sle", "num", "num", "num", "num", "num", "num", "num", "num", "num", "num", "mu", "add", "sep", "min", "dec", "div", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "-", "-", "-", "-", "-", "-", "-", "-", "num", "scr", "equ", "mas", "toy", "loya", "oya", "-", "-", "-", "-", "-", "-", "-", "-", "-", "shi", "shi", "ctr", "ctr", "alt", "alt" };
	inline const char* key_modes[ ] = { "hold", "toggle" };

	inline std::vector<int>key_codes{ 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0xbe };
	inline std::vector<char>key_names { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '.' };

	bool mouse_in_bounds( math::vec2 pos, math::vec2 size );
	bool is_key_held( int vkey );

	void watermark(  );

	void begin( );
	void end( );

	void tabs( const char* tabs[ 0x10 ], int tabs_size, int* selected_tab );
	void sub_tabs( const char* sub_tabs[ 0x10 ], int sub_tabs_size, int* selected_sub_tab );

	struct check_box_data_t
	{
		const char* label;
		bool value;
	};

	struct slider_data_t
	{
		const char* label;
		float min;
		float max;
		float value;
		const char* fmt;
		bool sliding;
	};

	struct combo_data_t
	{
		const char* label;
		const char* items[ 0x10 ];
		int items_count;
		int value;
		bool active;
	};

	struct multi_combo_data_t
	{
		const char* label;
		const char* items[ 0x10 ];
		bool value[ 0x10 ];
		int count;
		bool active;
	};

	struct text_box_data_t
	{
		const char* label;
		const char* preview_value;
		int size;
		int index;
		bool active;
	};

	struct key_bind_data_t
	{
		int key;
		int mode;
		int tick;
		bool in_bind;
		bool in_mode;
	};

	struct old_color_t
	{
		float r, g, b, a;
	};

	struct color_picker_data_t
	{
		ImColor value;
		bool second_col;
		bool active;
		old_color_t old_value;
	};

	class child
	{
	private:
		math::vec2 offset;

	public:
		child( const char* label, int offset_from_top, int size_y, bool right = false );

		void text( const char* label, bool should_render = true );

		void check_box( check_box_data_t* check_box_data, bool should_render = true );
		void slider( slider_data_t* slider_data, bool should_render = true );
		void combo( combo_data_t* combo_data, bool should_render = true );
		void multi_combo( multi_combo_data_t* multi_combo_data, bool should_render = true );
		bool button( const char* label, bool should_render = true );
		void text_box( text_box_data_t* text_box_data, char* value, bool should_render = true );

		void key_bind( key_bind_data_t* key_bind_data, bool should_render = true );
		void color_picker( color_picker_data_t* color_picker_data, bool should_render = true );
	};
}