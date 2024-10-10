#include "draw_cmds.hpp"

ImFont* arial_small;
ImFont* arial_medium;
ImFont* smallest_p7;

void imgui_draw_cmds::init( ImGuiIO& io ) noexcept
{
	arial_small = io.Fonts->AddFontFromFileTTF( ( "C:\\Windows\\Fonts\\Arial.ttf" ), 12.0f );
	arial_medium = io.Fonts->AddFontFromFileTTF( ( "C:\\Windows\\Fonts\\Arial.ttf" ), 14.f );
	smallest_p7 = io.Fonts->AddFontFromMemoryTTF( smallest_pixel_font, sizeof( smallest_pixel_font ), 10.0f, NULL, io.Fonts->GetGlyphRangesDefault( ) );
}

void imgui_draw_cmds::begin( ) noexcept
{
	std::unique_lock lock( m_mutex );
}

void imgui_draw_cmds::end( ) noexcept
{
	std::unique_lock lock( m_mutex );
	lock.unlock( );
}

void imgui_draw_cmds::clear( ) noexcept
{
	m_cmds.clear( );
}

void outline_text_internal( ImDrawList* dl, const ImFont* font, float size, const ImVec2& pos, ImColor fill, ImColor outline, std::string_view txt ) noexcept
{
	dl->AddText( font, size, { pos.x - 1.f, pos.y }, outline, txt.data( ) );
	dl->AddText( font, size, { pos.x - 1.f, pos.y - 1.f }, outline, txt.data( ) );
	dl->AddText( font, size, { pos.x, pos.y - 1.f }, outline, txt.data( ) );
	dl->AddText( font, size, { pos.x + 1.f, pos.y - 1.f }, outline, txt.data( ) );
	dl->AddText( font, size, { pos.x + 1.f, pos.y }, outline, txt.data( ) );
	dl->AddText( font, size, { pos.x + 1.f, pos.y + 1.f }, outline, txt.data( ) );
	dl->AddText( font, size, { pos.x, pos.y + 1.f }, outline, txt.data( ) );
	dl->AddText( font, size, { pos.x - 1.f, pos.y + 1.f }, outline, txt.data( ) );
	dl->AddText( font, size, pos, fill, txt.data( ) );
}

void imgui_draw_cmds::line( const math::vec2& start, const math::vec2& end, ImColor color, float thickness ) noexcept
{
	auto& cmd = m_cmds.emplace_back( );
	cmd.m_type = cmd_type::line;
	cmd.m_line.m_start = { static_cast< float >( start.x ), static_cast< float >( start.y ) };
	cmd.m_line.m_end = { static_cast< float >( end.x ), static_cast< float >( end.y ) };
	cmd.m_line.m_color = color;
	cmd.m_line.m_thickness = thickness;
}

void imgui_draw_cmds::rect_filled( const math::vec2& pos, const math::vec2& size, ImColor color, float rounding, bool outline ) noexcept
{
	auto& cmd = m_cmds.emplace_back( );
	cmd.m_type = cmd_type::rect_filled;
	cmd.m_rect_filled.m_min = { static_cast< float >( pos.x ), static_cast< float >( pos.y ) };
	cmd.m_rect_filled.m_max = { static_cast< float >( pos.x + size.x ), static_cast< float >( pos.y + size.y ) };
	cmd.m_rect_filled.m_color = color;
	cmd.m_rect_filled.m_rounding = rounding;
	cmd.m_rect_filled.m_outline = outline;
}

void imgui_draw_cmds::rect( const math::vec2& pos, const math::vec2& size, ImColor color, float rounding, float thickness, bool outline ) noexcept
{
	auto& cmd = m_cmds.emplace_back( );
	cmd.m_type = cmd_type::rect;
	cmd.m_rect.m_min = { static_cast< float >( pos.x ), static_cast< float >( pos.y ) };
	cmd.m_rect.m_max = { static_cast< float >( pos.x + size.x ), static_cast< float >( pos.y + size.y ) };
	cmd.m_rect.m_color = color;
	cmd.m_rect.m_rounding = rounding;
	cmd.m_rect.m_thickness = thickness;
	cmd.m_rect.m_outline = outline;
}

void imgui_draw_cmds::text( ImFont* font, float size, const math::vec2& pos, ImColor color, std::string_view txt, bool outline, bool center, bool dropshadow ) noexcept
{
	auto& cmd = m_cmds.emplace_back( );
	cmd.m_type = cmd_type::text;
	cmd.m_text.m_font = font;
	cmd.m_text.m_size = size;
	cmd.m_text.m_pos = { static_cast< float >( pos.x ), static_cast< float >( pos.y ) };
	cmd.m_text.m_color = color;
	cmd.m_text.m_txt = txt;
	cmd.m_text.m_outline = outline;
	cmd.m_text.m_center = center;
	cmd.m_text.m_dropshadow = dropshadow;
}

void imgui_draw_cmds::arc( const math::vec2& pos, float min_ang, float max_ang, float radius, ImColor color, float thickness ) noexcept
{
	auto& cmd = m_cmds.emplace_back( );
	cmd.m_type = cmd_type::arc;
	cmd.m_arc.m_pos = { static_cast< float >( pos.x ), static_cast< float >( pos.y ) };
	cmd.m_arc.m_min = min_ang;
	cmd.m_arc.m_max = max_ang;
	cmd.m_arc.m_radius = radius;
	cmd.m_arc.m_color = color;
	cmd.m_arc.m_thickness = thickness;
}

void imgui_draw_cmds::rect_gradient( const math::vec2& pos, const math::vec2& size, ImColor left_top, ImColor left_bot, ImColor right_top, ImColor right_bot ) noexcept
{
	auto& cmd = m_cmds.emplace_back( );
	cmd.m_type = cmd_type::rect_gradient;
	cmd.m_rect_gradient.m_min = { static_cast< float >( pos.x ), static_cast< float >( pos.y ) };
	cmd.m_rect_gradient.m_max = { static_cast< float >( pos.x + size.x ), static_cast< float >( pos.y + size.y ) };
	cmd.m_rect_gradient.m_left_top = left_top;
	cmd.m_rect_gradient.m_left_bot = left_bot;
	cmd.m_rect_gradient.m_right_top = right_top;
	cmd.m_rect_gradient.m_right_bot = right_bot;
}

void imgui_draw_cmds::circle_filled( const math::vec2& pos, float radius, ImColor color, int segments ) noexcept
{
	auto& cmd = m_cmds.emplace_back( );
	cmd.m_type = cmd_type::circle_filled;
	cmd.m_circle_filled.m_pos = { static_cast< float >( pos.x ), static_cast< float >( pos.y ) };
	cmd.m_circle_filled.m_radius = radius;
	cmd.m_circle_filled.m_color = color;
	cmd.m_circle_filled.m_segments = segments;
}

void imgui_draw_cmds::circle( const math::vec2& pos, float radius, ImColor color, int segments, float thickness ) noexcept
{
	auto& cmd = m_cmds.emplace_back( );
	cmd.m_type = cmd_type::circle;
	cmd.m_circle.m_pos = { static_cast< float >( pos.x ), static_cast< float >( pos.y ) };
	cmd.m_circle.m_radius = radius;
	cmd.m_circle.m_color = color;
	cmd.m_circle.m_segments = segments;
	cmd.m_circle.m_thickness = thickness;
}

void imgui_draw_cmds::image( img img_data, const math::vec2& pos, bool center ) noexcept
{
	auto& cmd = m_cmds.emplace_back( );
	cmd.m_type = cmd_type::image;
	cmd.m_image.m_img = img_data;
	cmd.m_image.m_pos = { static_cast< float >( pos.x ), static_cast< float >( pos.y ) };
	cmd.m_image.m_center = center;
}

void imgui_draw_cmds::triangle( const math::vec2& p1, const math::vec2& p2, const math::vec2& p3, ImColor color, float thickness ) noexcept
{
	auto& cmd = m_cmds.emplace_back( );
	cmd.m_type = cmd_type::triangle;
	cmd.m_triangle.m_p1 = { static_cast< float >( p1.x ), static_cast< float >( p1.y ) };
	cmd.m_triangle.m_p2 = { static_cast< float >( p2.x ), static_cast< float >( p2.y ) };
	cmd.m_triangle.m_p3 = { static_cast< float >( p3.x ), static_cast< float >( p3.y ) };
	cmd.m_triangle.m_color = color;
	cmd.m_triangle.m_thickness = thickness;
}

void imgui_draw_cmds::triangle_filled( const math::vec2& p1, const math::vec2& p2, const math::vec2& p3, ImColor color ) noexcept
{
	auto& cmd = m_cmds.emplace_back( );
	cmd.m_type = cmd_type::triangle_filled;
	cmd.m_triangle_filled.m_p1 = { static_cast< float >( p1.x ), static_cast< float >( p1.y ) };
	cmd.m_triangle_filled.m_p2 = { static_cast< float >( p2.x ), static_cast< float >( p2.y ) };
	cmd.m_triangle_filled.m_p3 = { static_cast< float >( p3.x ), static_cast< float >( p3.y ) };
	cmd.m_triangle_filled.m_color = color;
}

void imgui_draw_cmds::polygon( const std::vector<math::vec2> points, ImColor color, float thickness ) noexcept
{
	auto& cmd = m_cmds.emplace_back( );
	cmd.m_type = cmd_type::poly_line;

	std::vector<ImVec2>points_cache{ };
	for ( int i = 0; i < points.size( ); i++ )
		points_cache.push_back( ImVec2( points[ i ].x, points[ i ].y ) );

	cmd.m_poly_line.m_points = points_cache;
	cmd.m_poly_line.m_color = color;
	cmd.m_poly_line.m_thickness = thickness;
}

void imgui_draw_cmds::polygon_filled( const std::vector<math::vec2> points, ImColor color ) noexcept
{
	auto& cmd = m_cmds.emplace_back( );
	cmd.m_type = cmd_type::poly_filled;

	std::vector<ImVec2>points_cache{ };
	for ( int i = 0; i < points.size( ); i++ )
		points_cache.push_back( ImVec2( points[ i ].x, points[ i ].y ) );

	cmd.m_poly_filled.m_points = points_cache;
	cmd.m_poly_filled.m_color = color;
}

void imgui_draw_cmds::draw( ImDrawList* dl ) noexcept
{
	for ( const auto& c : m_cmds )
	{
		switch ( c.m_type )
		{
			case cmd_type::line:
			{
				const auto& data = c.m_line;
				dl->AddLine( data.m_start, data.m_end, data.m_color, data.m_thickness );
				break;
			}

			case cmd_type::rect_filled:
			{
				const auto& data = c.m_rect_filled;

				dl->AddRectFilled( data.m_min, data.m_max, data.m_color, data.m_rounding );

				if ( data.m_outline )
					dl->AddRect( ImVec2( data.m_min.x - 1, data.m_min.y - 1 ), ImVec2( data.m_max.x + 2 / 2, data.m_max.y + 2 / 2 ), ImColor( 30, 30, 30, 255 ), data.m_rounding, 0 );

				break;
			}

			case cmd_type::rect:
			{
				const auto& data = c.m_rect;

				dl->AddRect( data.m_min, data.m_max, data.m_color, data.m_rounding, 0, data.m_thickness );

				if ( data.m_outline )
				{
					dl->AddRect( ImVec2( data.m_min.x - 1, data.m_min.y - 1 ), ImVec2( data.m_max.x + 2 / 2, data.m_max.y + 2 / 2 ), ImColor( 25, 25, 25, 255 ), data.m_rounding, 0, data.m_thickness );
					dl->AddRect( ImVec2( data.m_min.x + 1, data.m_min.y + 1 ), ImVec2( data.m_max.x - 2 / 2, data.m_max.y - 2 / 2 ), ImColor( 25, 25, 25, 255 ), data.m_rounding, 0, data.m_thickness );
				}

				break;
			}

			case cmd_type::arc:
			{
				const auto& data = c.m_arc;
				dl->PathArcTo( ImVec2( data.m_pos.x, data.m_pos.y ), data.m_radius, DEG2RAD( data.m_min ), DEG2RAD( data.m_max ), 32 );
				dl->PathStroke( data.m_color, false, data.m_thickness );
				break;
			}

			case cmd_type::rect_gradient:
			{
				const auto& data = c.m_rect_gradient;
				dl->AddRectFilledMultiColor( data.m_min, data.m_max, data.m_left_top, data.m_right_top, data.m_right_bot, data.m_left_bot );
				break;
			}

			case cmd_type::circle_filled:
			{
				const auto& data = c.m_circle_filled;
				dl->AddCircleFilled( ImVec2( data.m_pos.x, data.m_pos.y ), data.m_radius, data.m_color, data.m_segments );
				break;
			}

			case cmd_type::circle:
			{
				const auto& data = c.m_circle;
				dl->AddCircle( ImVec2( data.m_pos.x, data.m_pos.y ), data.m_radius, data.m_color, data.m_segments, data.m_thickness );
				break;
			}

			case cmd_type::triangle:
			{
				const auto& data = c.m_triangle;
				dl->AddTriangle( data.m_p1, data.m_p2, data.m_p3, data.m_color, data.m_thickness );
				break;
			}

			case cmd_type::triangle_filled:
			{
				const auto& data = c.m_triangle_filled;
				dl->AddTriangleFilled( data.m_p1, data.m_p2, data.m_p3, data.m_color );
				break;
			}

			case cmd_type::poly_line:
			{
				const auto& data = c.m_poly_line;
				dl->AddPolyline( data.m_points.data( ), data.m_points.size( ), data.m_color, ImDrawFlags_None, data.m_thickness );
				break;
			}

			case cmd_type::poly_filled:
			{
				const auto& data = c.m_poly_filled;
				dl->AddConvexPolyFilled( data.m_points.data( ), data.m_points.size( ), data.m_color );
				break;
			}

			case cmd_type::image:
			{
				const auto& data = c.m_image;
				if ( data.m_center )
				{
					ImVec2 pos = { data.m_pos.x - ( data.m_img.width / 2.f ), data.m_pos.y - ( data.m_img.height / 2.f ) };
					dl->AddImage( data.m_img.texture, ImVec2( pos.x, pos.y ), ImVec2( data.m_img.width + pos.x, data.m_img.height + pos.y ) );
				}
				else
					dl->AddImage( data.m_img.texture, ImVec2( data.m_pos.x, data.m_pos.y ), ImVec2( data.m_img.width + data.m_pos.x, data.m_img.height + data.m_pos.y ) );

				break;
			}

			case cmd_type::text:
			{
				const auto& data = c.m_text;
				auto pos = data.m_pos;

				if ( data.m_center )
				{
					const auto size = data.m_font->CalcTextSizeA( data.m_size, FLT_MAX, 0.0f, data.m_txt.c_str( ) );

					pos.x -= ( size.x / 2.f );
					pos.y -= size.y;
				}

				if ( data.m_outline )
					outline_text_internal( dl, data.m_font, data.m_size, pos, data.m_color, ImColor( 25, 25, 25, 255 ), data.m_txt );

				else if ( data.m_dropshadow )
				{
					dl->AddText( data.m_font, data.m_size, ImVec2( pos.x + 1.f, pos.y + 1.f ), ImColor( 25, 25, 25, 255 ), data.m_txt.c_str( ) );
					dl->AddText( data.m_font, data.m_size, pos, data.m_color, data.m_txt.c_str( ) );
				}
				else
					dl->AddText( data.m_font, data.m_size, pos, data.m_color, data.m_txt.c_str( ) );

				break;
			}
		}
	}
}

#define STB_IMAGE_IMPLEMENTATION
#include "../d3d11_overlay/stb_image.hpp"

img imgui_draw_cmds::load_img_from_file( const char* path )
{
	img out_data{ };

	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load( path, &image_width, &image_height, nullptr, STBI_rgb_alpha );
	if ( !image_data )
		return out_data;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory( &desc, sizeof( desc ) );
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* texture = NULL;
	D3D11_SUBRESOURCE_DATA sub_resource;
	sub_resource.pSysMem = image_data;
	sub_resource.SysMemPitch = desc.Width * 4;
	sub_resource.SysMemSlicePitch = 0;
	draw_cmds->device->CreateTexture2D( &desc, &sub_resource, &texture );

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	ZeroMemory( &srv_desc, sizeof( srv_desc ) );
	srv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MipLevels = desc.MipLevels;
	srv_desc.Texture2D.MostDetailedMip = 0;
	draw_cmds->device->CreateShaderResourceView( texture, &srv_desc, &out_data.texture );
	texture->Release( );

	out_data.width = image_width;
	out_data.height = image_height;
	stbi_image_free( image_data );

	return out_data;
}

img imgui_draw_cmds::load_img_from_mem( unsigned char* data, unsigned int size )
{
	img out_data{ };

	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load_from_memory( static_cast< const stbi_uc* >( data ), size, &image_width, &image_height, nullptr, STBI_rgb_alpha );
	if ( !image_data )
		return out_data;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory( &desc, sizeof( desc ) );
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* texture = NULL;
	D3D11_SUBRESOURCE_DATA sub_resource;
	sub_resource.pSysMem = image_data;
	sub_resource.SysMemPitch = desc.Width * 4;
	sub_resource.SysMemSlicePitch = 0;
	draw_cmds->device->CreateTexture2D( &desc, &sub_resource, &texture );

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	ZeroMemory( &srv_desc, sizeof( srv_desc ) );
	srv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MipLevels = desc.MipLevels;
	srv_desc.Texture2D.MostDetailedMip = 0;
	draw_cmds->device->CreateShaderResourceView( texture, &srv_desc, &out_data.texture );
	texture->Release( );

	out_data.width = image_width;
	out_data.height = image_height;
	stbi_image_free( image_data );

	return out_data;
}