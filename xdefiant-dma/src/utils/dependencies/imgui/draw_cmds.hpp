#pragma once

#include "../../../sdk/math/math.hpp"

#include <d3d11.h>

#include <mutex>
#include <vector>
#include "imgui.h"

#include "../fonts/smallest_pixel.hpp"

extern ImFont* arial_small;
extern ImFont* arial_medium;
extern ImFont* smallest_p7;

class img
{
public:
	ID3D11ShaderResourceView* texture;
	int width;
	int height;
};


void outline_text_internal( ImDrawList* dl, const ImFont* font, float size,const ImVec2& pos, ImColor fill, ImColor outline, std::string_view txt ) noexcept;

class imgui_draw_cmds 
{
public:
	imgui_draw_cmds() noexcept = default;

	void init( ImGuiIO& io ) noexcept;

	void begin() noexcept;
	void end() noexcept;

	void clear() noexcept;

	void line( const math::vec2& start, const math::vec2& end, ImColor color, float thickness = 1.f ) noexcept;
	void rect_filled( const math::vec2& pos, const math::vec2& size, ImColor color, float rounding = 0.f, bool outline = false ) noexcept;
	void rect( const math::vec2& pos, const math::vec2& size, ImColor color, float rounding = 0.f, float thickness = 1.f, bool outline = false ) noexcept;
	void text( ImFont* font, float size, const math::vec2& pos, ImColor color, std::string_view txt, bool outline = false, bool center = false, bool dropshadow = false ) noexcept;
	void arc( const math::vec2& pos, float min_ang, float max_ang, float radius, ImColor color, float thickness = 1.f ) noexcept;
	void rect_gradient( const math::vec2& pos, const math::vec2& size, ImColor left_top, ImColor left_bot, ImColor right_top, ImColor right_bot ) noexcept;
	void circle_filled( const math::vec2& pos, float radius, ImColor color, int segments = 0 ) noexcept;
	void circle( const math::vec2& pos, float radius, ImColor color, int segments = 0, float thickness = 1.f ) noexcept;
	void image( img img_data, const math::vec2& pos, bool center = false ) noexcept;
	void triangle( const math::vec2& p1, const math::vec2& p2, const math::vec2& p3, ImColor color, float thickness = 1.f ) noexcept;
	void triangle_filled( const math::vec2& p1, const math::vec2& p2, const math::vec2& p3, ImColor color ) noexcept;
	void polygon( const std::vector<math::vec2> points, ImColor color, float thickness = 1.f ) noexcept;
	void polygon_filled( const std::vector<math::vec2> points, ImColor color ) noexcept;

	void draw( ImDrawList* dl ) noexcept;

	ID3D11Device* device { };
	img load_img_from_file( const char* path );
	img load_img_from_mem( unsigned char* data, unsigned int size );

private:
	enum class cmd_type : uint8_t
	{
		line = 0,
		rect_filled,
		rect,
		text,
		arc,
		rect_gradient,
		circle_filled,
		circle,
		image,
		triangle,
		triangle_filled,
		poly_line,
		poly_filled
	};

	struct draw_cmd
	{
		cmd_type m_type;

		struct line 
		{
			ImVec2 m_start;
			ImVec2 m_end;
			ImColor m_color;
			float m_thickness;
		} m_line;

		struct rect_filled
		{
			ImVec2 m_min;
			ImVec2 m_max;
			ImColor m_color;
			float m_rounding;
			bool m_outline;
		} m_rect_filled;

		struct rect
		{
			ImVec2 m_min;
			ImVec2 m_max;
			ImColor m_color;
			float m_rounding;
			float m_thickness;
			bool m_outline;
		} m_rect;

		struct text 
		{
			ImFont* m_font;
			float m_size;
			ImVec2 m_pos;
			ImColor m_color;
			std::string m_txt;
			bool m_outline;
			bool m_center;
			bool m_dropshadow;
		} m_text;

		struct arc
		{
			ImVec2 m_pos;
			float m_min;
			float m_max;
			float m_radius;
			ImColor m_color;
			float m_thickness;
		} m_arc;

		struct rect_gradient
		{
			ImVec2 m_min;
			ImVec2 m_max;
			ImColor m_left_top;
			ImColor m_left_bot;
			ImColor m_right_top;
			ImColor m_right_bot;
		} m_rect_gradient;

		struct circle_filled
		{
			ImVec2 m_pos;
			float m_radius;
			ImColor m_color;
			int m_segments;
		} m_circle_filled;

		struct circle
		{
			ImVec2 m_pos;
			float m_radius;
			ImColor m_color;
			int m_segments;
			float m_thickness;
		} m_circle;

		struct image
		{
			img m_img;
			ImVec2 m_pos;
			bool m_center;
		} m_image;

		struct triangle
		{
			ImVec2 m_p1;
			ImVec2 m_p2;
			ImVec2 m_p3;
			ImColor m_color;
			float m_thickness;
		} m_triangle;

		struct triangle_filled
		{
			ImVec2 m_p1;
			ImVec2 m_p2;
			ImVec2 m_p3;
			ImColor m_color;
		} m_triangle_filled;
		
		struct poly_line 
		{
			std::vector<ImVec2> m_points;
			ImColor m_color;
			float m_thickness;
		} m_poly_line;

		struct poly_filled 
		{
			std::vector<ImVec2> m_points;
			ImColor m_color;
		} m_poly_filled;
	};

	std::mutex m_mutex;
	std::vector<draw_cmd> m_cmds;
};

inline const auto draw_cmds = std::make_unique<imgui_draw_cmds>( );