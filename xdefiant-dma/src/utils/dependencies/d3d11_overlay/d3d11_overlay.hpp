#pragma once

#include "utils/utils.hpp"

#include <vector>
#include <string_view>
#include <functional>

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include "../../../cheat/menu/menu.hpp"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/draw_cmds.hpp"

inline unsigned int g_resize_width = 0, g_resize_height = 0;

extern IMGUI_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

inline LRESULT CALLBACK window_proc( HWND window_handle, UINT message, WPARAM param_1, LPARAM param_2 )
{
    switch ( message )
    {
        case WM_DESTROY:
        {
            PostQuitMessage( 0 );
            break;
        }

        case WM_SIZE:
        {
            if ( param_1 == SIZE_MINIMIZED )
                return 0;
            g_resize_width = ( UINT )LOWORD( param_2 ); // Queue resize
            g_resize_height = ( UINT )HIWORD( param_2 );
            return 0;
        }

        case WM_SYSCOMMAND:
        {
            if ( ( param_1 & 0xfff0 ) == SC_KEYMENU ) // Disable ALT application menu
                return 0;
            break;
        }

        default:
        break;
    }

    return DefWindowProcA( window_handle, message, param_1, param_2 );
}

inline BOOL CALLBACK monitor_enum_proc( HMONITOR monitor, HDC monitor_handle, LPRECT lprcMonitor, LPARAM data )
{
    auto& monitor_info_array = *reinterpret_cast< std::vector<MONITORINFO>* >( data );

    MONITORINFO monitor_info { };
    monitor_info.cbSize = sizeof( MONITORINFO );
    GetMonitorInfoA( monitor, &monitor_info );

    monitor_info_array.push_back( monitor_info );

    return TRUE;
}

inline void throw_error( const char* msg )
{
    MessageBoxA( nullptr, msg, "[paionia.gg]: error", MB_ICONERROR | MB_OK );
}

class d3d11_overlay
{
public:
    d3d11_overlay( LPCSTR window_name, math::vec2* screen_size, bool transparent = false )
    {
        m_monitors.reserve( GetSystemMetrics( SM_CMONITORS ) );
        EnumDisplayMonitors( nullptr, nullptr, monitor_enum_proc, reinterpret_cast< LPARAM >( &m_monitors ) );

        auto monitor = m_monitors[ m_monitors.size( ) - 1 ];
        if ( transparent )
        {
            for ( const auto display : m_monitors )
            {
                if ( display.dwFlags & MONITORINFOF_PRIMARY ) // cope but wont break
                {
                    monitor = display;
                    break;
                }
            }
        }
        else
        {
            if ( monitor.dwFlags & MONITORINFOF_PRIMARY )
                monitor = m_monitors[ 0 ];
        }

        m_window_size.x = monitor.rcMonitor.right - monitor.rcMonitor.left;
        m_window_size.y = monitor.rcMonitor.bottom - monitor.rcMonitor.top;

        screen_size->x = m_window_size.x;
        screen_size->y = m_window_size.y;

        m_window_class = { sizeof( WNDCLASSEXA ), CS_CLASSDC, window_proc, 0l, 0l, GetModuleHandleA( nullptr ), nullptr, nullptr, nullptr, nullptr, "paionia_window_cls", nullptr };
        if ( !RegisterClassExA( &m_window_class ) )
        {
            throw_error( "Failed to Register Window Class!" );
            return;
        }

        m_window = CreateWindowExA( transparent ? WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED : 0, m_window_class.lpszClassName, window_name, WS_POPUP, monitor.rcMonitor.left, monitor.rcMonitor.top, m_window_size.x, m_window_size.y, nullptr, nullptr, m_window_class.hInstance, nullptr );
        if ( !m_window )
        {
            throw_error( "Failed to Create Window!" );
            return;
        }

        ShowWindow( m_window, SW_SHOW );
        UpdateWindow( m_window );
        if ( transparent )
            SetLayeredWindowAttributes(
            m_window,
            0,
            255,
            ULW_COLORKEY | LWA_ALPHA );

        auto get_adapters = [ ]( )->std::vector<IDXGIAdapter*>
        {
            std::vector<IDXGIAdapter*> adapters { };
            IDXGIFactory* factory = nullptr;

            if ( !SUCCEEDED( CreateDXGIFactory( __uuidof( IDXGIFactory ), reinterpret_cast< void** >( &factory ) ) ) )
            {
                throw_error( "Failed to Create DXGI Factory!" );
                return adapters;
            }

            IDXGIAdapter* adapter = nullptr;
            UINT adapter_count { };

            while ( SUCCEEDED( factory->EnumAdapters( adapter_count, &adapter ) ) )
            {
                adapters.push_back( adapter );
                adapter_count++;
            }

            return adapters;
        };

        auto adapters = get_adapters( );
        if ( !adapters.size( ) )
        {
            throw_error( "Failed to Create DXGI Adapters!" );
            return;
        }

        DXGI_SWAP_CHAIN_DESC swap_chain_desc;
        ZeroMemory( &swap_chain_desc, sizeof( DXGI_SWAP_CHAIN_DESC ) );

        swap_chain_desc.BufferCount = 2;
        swap_chain_desc.BufferDesc.Width = 0;
        swap_chain_desc.BufferDesc.Height = 0;
        swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swap_chain_desc.BufferDesc.RefreshRate.Numerator = 144;
        swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
        swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swap_chain_desc.OutputWindow = m_window;
        swap_chain_desc.SampleDesc.Count = 1;
        swap_chain_desc.SampleDesc.Quality = 0;
        swap_chain_desc.Windowed = TRUE;
        swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        UINT createDeviceFlags = 0;
        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevelArray[ 2 ] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

        HRESULT res = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &swap_chain_desc, &m_swap_chain, &m_device, &featureLevel, &m_device_context );
        if ( res == DXGI_ERROR_UNSUPPORTED ) // Try high-performance WARP software driver if hardware is not available.
            res = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &swap_chain_desc, &m_swap_chain, &m_device, &featureLevel, &m_device_context );
        if ( res != S_OK )
        {
            throw_error( "Failed to Create Device and Swap Chain!" );
            return;
        }


        if ( !SUCCEEDED( D3D11CreateDeviceAndSwapChain( adapters[ 0 ], D3D_DRIVER_TYPE_UNKNOWN, nullptr, NULL, nullptr, NULL, D3D11_SDK_VERSION, &swap_chain_desc, &m_swap_chain, &m_device, nullptr, &m_device_context ) ) )
        {
            throw_error( "Failed to Create Device and Swapchain!" );
            return;
        }

        ID3D11Texture2D* back_buffer = nullptr;
        if ( !SUCCEEDED( m_swap_chain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast< void** >( &back_buffer ) ) ) )
        {
            throw_error( "Failed to Get Back Buffer!" );
            return;
        }

        if ( !SUCCEEDED( m_device->CreateRenderTargetView( back_buffer, nullptr, &m_render_target_view ) ) )
        {
            throw_error( "Failed to Create Render Target View!" );
            return;
        }

        back_buffer->Release( );

        ImGui::CreateContext( );
        auto& io = ImGui::GetIO( );
        io.IniFilename = nullptr;

        ImGui_ImplWin32_Init( m_window );
        ImGui_ImplDX11_Init( m_device, m_device_context );

        draw_cmds->init( io );
        io.Fonts->Build( );

        draw_cmds->device = std::move( m_device );
    }

    void run( std::function<void( )> func );

private:
    std::vector<MONITORINFO> m_monitors;

    HWND m_window;
    WNDCLASSEXA m_window_class;
    POINTS m_window_size;

    IDXGISwapChain* m_swap_chain;
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_device_context;
    ID3D11RenderTargetView* m_render_target_view;
};