#include "d3d11_overlay.hpp"

void d3d11_overlay::run( std::function<void( )> func )
{
    MSG msg;
    ZeroMemory( &msg, sizeof( MSG ) );

    while ( true )
    {
        if ( PeekMessageA( &msg, m_window, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessageA( &msg );
            continue;
        }

        if ( msg.message == WM_QUIT )
            break;

        if ( g_resize_width != 0 && g_resize_height != 0 )
        {
            if ( m_render_target_view )
            {
                m_render_target_view->Release( );
                m_render_target_view = nullptr;
            }

            m_swap_chain->ResizeBuffers( 0, g_resize_width, g_resize_height, DXGI_FORMAT_UNKNOWN, 0 );
            g_resize_width = g_resize_height = 0;

            ID3D11Texture2D* back_buffer;
            m_swap_chain->GetBuffer( 0, IID_PPV_ARGS( &back_buffer ) );
            m_device->CreateRenderTargetView( back_buffer, nullptr, &m_render_target_view );
            back_buffer->Release( );
        }

        bool pressed = utils::key( VK_HOME ).is_held( );
        static bool updated = false;

        if ( pressed && !updated )
        {
            static int monitor_index = 0;
            if ( monitor_index > m_monitors.size( ) - 1 )
                monitor_index = 0;

            auto monitor = m_monitors[ monitor_index ];

            utils::print( e_print_type::good, "swapping to monitor index: %i\n", monitor_index + 1 ); // doing plus one for end user so it will map to the display order of their os

            m_window_size.x = monitor.rcMonitor.right - monitor.rcMonitor.left;
            m_window_size.y = monitor.rcMonitor.bottom - monitor.rcMonitor.top;

            SetWindowPos( m_window, 0, monitor.rcMonitor.left, monitor.rcMonitor.top, m_window_size.x, m_window_size.y, 0 );
            ShowWindow( m_window, SW_SHOW );
            UpdateWindow( m_window );

            monitor_index++;
            updated = true;
        }

        if ( !pressed && updated )
            updated = false;

        ImGui_ImplDX11_NewFrame( );
        ImGui_ImplWin32_NewFrame( );
        ImGui::NewFrame( );

        const auto draw_list = ImGui::GetBackgroundDrawList( );

        draw_cmds->begin( );
        draw_cmds->draw( draw_list );
        draw_cmds->clear( );
        draw_cmds->end( );

        menu::render( );
        func( );

        ImGui::Render( );

        static float bgcolor[ ] = { 0.f, 0.f, 0.f, 1.f };
        m_device_context->OMSetRenderTargets( 1, &m_render_target_view, nullptr );
        m_device_context->ClearRenderTargetView( m_render_target_view, bgcolor );
        ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );

        m_swap_chain->Present( 1, 0 );
    }
}