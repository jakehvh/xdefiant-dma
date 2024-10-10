#pragma once

#include "../utils/dependencies/kmbox/kmbox.hpp"

#include <cstdint>
#include <unordered_map>
#include <string>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <thread>
#include <mutex>
#include <array>
#include <vector>
#include <devguid.h>

#include <SetupAPI.h>
#pragma comment (lib, "Setupapi.lib")

using namespace std::chrono_literals;

#include "../utils/dependencies/leechcore/leechcore.hpp"
#include "../utils/dependencies/vmm/vmm.hpp"
#include "../utils/dependencies/json/json.hpp"

#define in_range( x, a, b ) ( x >= a && x <= b )
#define get_bits( x ) ( in_range( ( x & ( ~0x20 ) ), 'A', 'F' ) ? ( ( x & ( ~0x20 ) ) - 'A' + 0xA ) : ( in_range( x, '0', '9' ) ? x - '0' : 0 ) )
#define get_byte( x ) ( get_bits( x[ 0 ] ) << 4 | get_bits( x[ 1 ] ) )

enum class e_print_type : uint8_t
{
	normal = 0,
	good = 1,
	error = 2,
};

struct user_info
{
	char user_name[ 0x50 ];
	char password[ 0x50 ];
	bool moonlight_overlay;
};

namespace utils
{
	inline std::unordered_map<std::string, uint32_t>key_map
	{
		{ "m1", 0x01 }, { "m2", 0x02 }, { "m3", 0x04 }, { "m4", 0x05 }, { "m5", 0x06 },
		{ "Backspace", 0x08 }, { "Tab", 0x09 }, { "Enter", 0x0d }, { "CapsLock", 0x14 },
		{ "Escape", 0x1b }, { "Space", 0x20 }, { "PageUp", 0x21 }, { "PageDown", 0x22 },
		{ "End", 0x23 }, { "Home", 0x24 }, { "ArrowLeft", 0x25 }, { "ArrowUp", 0x26 },
		{ "ArrowRight", 0x27 }, { "ArrowDown", 0x28 }, { "printscreen", 0x2c },
		{ "Insert", 0x2d }, { "Delete", 0x2e }, { "Digit0", 0x30 }, { "Digit1", 0x31 },
		{ "Digit2", 0x32 }, { "Digit3", 0x33 }, { "Digit4", 0x34 }, { "Digit5", 0x35 },
		{ "Digit6", 0x36 }, { "Digit7", 0x37 }, { "Digit8", 0x38 }, { "Digit9", 0x39 },
		{ "KeyA", 0x41 }, { "KeyB", 0x42 }, { "KeyC", 0x43 }, { "KeyD", 0x44 },
		{ "KeyE", 0x45 }, { "KeyF", 0x46 }, { "KeyG", 0x47 }, { "KeyH", 0x48 },
		{ "KeyI", 0x49 }, { "KeyJ", 0x4a }, { "KeyK", 0x4b }, { "KeyL", 0x4c },
		{ "KeyM", 0x4d }, { "KeyN", 0x4e }, { "KeyO", 0x4f }, { "KeyP", 0x50 },
		{ "KeyQ", 0x51 }, { "KeyR", 0x52 }, { "KeyS", 0x53 }, { "KeyT", 0x54 },
		{ "KeyU", 0x55 }, { "KeyV", 0x56 }, { "KeyW", 0x57 }, { "KeyX", 0x58 },
		{ "KeyY", 0x59 }, { "KeyZ", 0x5a }, { "Numpad0", 0x60 }, { "Numpad1", 0x61 },
		{ "Numpad2", 0x62 }, { "Numpad3", 0x63 }, { "Numpad4", 0x64 }, { "Numpad5", 0x65 },
		{ "Numpad6", 0x66 }, { "Numpad7", 0x67 }, { "Numpad8", 0x68 }, { "Numpad9", 0x69 },
		{ "NumpadMultiply", 0x6a }, { "NumpadAdd", 0x6b }, { "NumpadSubtract", 0x6d },
		{ "NumpadDecimal", 0x6e }, { "NumpadDivide", 0x6f }, { "F1", 0x70 }, { "F2", 0x71 },
		{ "F3", 0x72 }, { "F4", 0x73 }, { "F5", 0x74 }, { "F6", 0x75 }, { "F7", 0x76 },
		{ "F8", 0x77 }, { "F9", 0x78 }, { "F10", 0x79 }, { "F11", 0x7a }, { "F12", 0x7b },
		{ "NumLock", 0x90 }, { "ScrollLock", 0x91 }, { "ShiftLeft", 0xa0 }, { "ShiftRight", 0xa1 },
		{ "ControlLeft", 0xa2 }, { "ControlRight", 0xa3 }, { "AltLeft", 0xa4 }, { "AltRight", 0xa5 },
		{ "Semicolon", 0xBa }, { "Equal", 0xbb }, { "Comma", 0xbc }, { "Minus", 0xbd },
		{ "Period", 0xbe },  { "Slash", 0xbf }, { "Backquote", 0xc0 }, { "BracketLeft", 0xdb },
		{ "Backslash", 0xdc }, { "BracketRight", 0xdd }, { "Quote", 0xde }
	};

	inline std::shared_ptr<user_info> info{ };

	inline void setup_console( )
	{
		HWND console = GetConsoleWindow( );
		HANDLE output_handle = GetStdHandle( STD_OUTPUT_HANDLE );

		DWORD console_mode{ }; GetConsoleMode( output_handle, &console_mode );
		RECT rect{ }; GetWindowRect( console, &rect );

		console_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		console_mode |= DISABLE_NEWLINE_AUTO_RETURN;

		SetConsoleMode( output_handle, console_mode );

		MoveWindow( console, rect.left, rect.top, 705, 400, TRUE );
		SetWindowLongA( console, GWL_STYLE, GetWindowLongA( console, GWL_STYLE ) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX );
		SetLayeredWindowAttributes( console, NULL, 230, LWA_ALPHA );

		SetConsoleTitleA( "[paionia.gg]: console" );
	}

	inline void print( e_print_type print_type, const char* fmt, ... )
	{
		char buffer[ 0x100 ]{ };

		va_list arg_list{ };
		va_start( arg_list, fmt );

		vsprintf_s( buffer, fmt, arg_list );

		va_end( arg_list );

		switch ( print_type )
		{
			case e_print_type::normal:
			{
				printf( "\33[38;2;255;255;0m[>]: \33[38;2;252;107;129m%s\33[m", buffer );
			}
			break;

			case e_print_type::good:
			{
				printf( "\33[38;2;0;255;0m[+]: \33[38;2;252;107;129m%s\33[m", buffer );
			}
			break;

			case e_print_type::error:
			{
				printf( "\33[38;2;255;0;0m[-]: \33[38;2;252;107;129m%s\33[m", buffer );
			}
			break;
		}
	}

	inline void throw_error( const char* fmt, ... )
	{
		char buffer[ 0x100 ]{ };

		va_list arg_list{ };
		va_start( arg_list, fmt );

		vsprintf_s( buffer, fmt, arg_list );

		va_end( arg_list );

		MessageBoxA( nullptr, buffer, "paionia.gg", MB_ICONERROR | MB_OK );
	}

	template <class _Rep, class _Period>
	inline void sleep( const std::chrono::duration<_Rep, _Period>& time )
	{
		std::this_thread::sleep_for( time );
	}

	inline VMM_HANDLE vmm_handle{ };

	inline bool load_leechcore_and_vmm( )
	{
#if defined(_DEBUG) || defined(RELEASE_EXE)
		auto leechcore = LoadLibraryA( "leechcore.dll" );
		if ( !leechcore )
			return false;

		auto vmm = LoadLibraryA( "vmm.dll" );
		if ( !vmm )
			return false;
#else

#endif

		return true;
	}

	inline bool generate_mmap( std::filesystem::path& mmap )
	{
		if ( !std::filesystem::exists( mmap.parent_path( ) ) )
			std::filesystem::create_directories( mmap.parent_path( ) );

		if ( !std::filesystem::exists( mmap ) )
		{
			LPCSTR args[ ]{ "", "-device", "fpga", "-waitinitialize" };
			vmm_handle = VMMDLL_Initialize( 4, args );

			if ( !vmm_handle )
			{
				args[ 2 ] = "fpga://algo=1"; // for idiots with slow firmware
				vmm_handle = VMMDLL_Initialize( 4, args );
				if ( !vmm_handle )
					return false;
			}

			PVMMDLL_MAP_PHYSMEM map_physmem = nullptr;
			if ( !VMMDLL_Map_GetPhysMem( vmm_handle, &map_physmem ) )
			{
				print( e_print_type::error, "failed to retrieve physical memory ranges!\n" );
				VMMDLL_Close( vmm_handle );
				return false;
			}

			if ( map_physmem->dwVersion != VMMDLL_MAP_PHYSMEM_VERSION )
			{
				print( e_print_type::error, "invalid physical memory map version!\n" );
				VMMDLL_MemFree( map_physmem );
				VMMDLL_Close( vmm_handle );
				return false;
			}

			if ( !map_physmem->cMap )
			{
				print( e_print_type::error, "failed to get physical memory map!\n" );
				VMMDLL_MemFree( map_physmem );
				VMMDLL_Close( vmm_handle );
				return false;
			}

			std::stringstream ss;
			for ( DWORD i = 0; i < map_physmem->cMap; i++ )
				ss << std::hex << map_physmem->pMap[ i ].pa << " " << ( map_physmem->pMap[ i ].pa + map_physmem->pMap[ i ].cb - 1 ) << std::endl;

			std::ofstream of( mmap );
			of << ss.str( );
			of.close( );

			VMMDLL_MemFree( map_physmem );
			VMMDLL_Close( vmm_handle );
		}

		return true;
	}

	inline bool initialize_memprocfs( )
	{
		auto mmap = std::filesystem::current_path( ) / "paionia\\memory_map.pi";
		if ( !generate_mmap( mmap ) )
			return false;

		auto path = mmap.generic_string( );
		LPCSTR args[ ]{ "", "-memmap", path.c_str( ), "-device", "fpga", "-waitinitialize" };
		vmm_handle = VMMDLL_Initialize( 6, args );
		if ( !vmm_handle )
		{
			args[ 4 ] = "fpga://algo=1"; // for idiots with slow firmware
			vmm_handle = VMMDLL_Initialize( 6, args );
		}

		return !!( vmm_handle );
	}

	class address
	{
	public:
		address( ) : addr( 0 ) { }
		address( void* address ) : addr( reinterpret_cast< uint64_t >( address ) ) { }
		address( uintptr_t address ) : addr( address ) { }

		template <typename t = void*>
		t as( ) const
		{
			if ( !addr )
				return ( t )0;

			return ( t )addr;
		}

		template <typename t = void*>
		t& as_ref( ) const
		{
			return ( t& )addr;
		}

		address add( size_t offset ) const
		{
			return address( addr + offset );
		}

		address sub( size_t offset ) const
		{
			return address( addr - offset );
		}

		address& operator+=( size_t offset )
		{
			addr += offset;
			return *this;
		}

		address& operator-=( size_t offset )
		{
			addr -= offset;
			return *this;
		}

		address operator+( size_t offset ) const
		{
			return address( addr + offset );
		}

		address operator-( size_t offset ) const
		{
			return address( addr - offset );
		}

		address& operator=( const address& address )
		{
			addr = address.addr;
			return *this;
		}

		address& operator=( uint64_t address )
		{
			addr = address;
			return *this;
		}
	private:
		uintptr_t addr;
	};

	class module
	{
	public:
		module( std::string module_name, DWORD parent_process_id )
		{
			VMMDLL_MAP_MODULEENTRY* module_entry{ };
			if ( !VMMDLL_Map_GetModuleFromNameU( vmm_handle, parent_process_id, module_name.c_str( ), &module_entry, VMMDLL_MODULE_FLAG_NORMAL ) )
			{
				print( e_print_type::error, "failed to get module entry for %s!\n", module_name.c_str( ) );
				return;
			}

			m_parent_pid = parent_process_id;
			m_name = module_name;
			m_base = address( module_entry->vaBase );
			m_size = module_entry->cbImageSize;

			VMMDLL_MemFree( module_entry );
		}

		address get_base( ) const
		{
			return m_base;
		}

		size_t get_size( ) const
		{
			return m_size;
		}

		address operator [ ]( std::string export_name ) const
		{
			return VMMDLL_ProcessGetProcAddressU( vmm_handle, m_parent_pid, m_name.c_str( ), export_name.c_str( ) );
		}

	private:
		DWORD m_parent_pid;
		std::string m_name;
		address m_base;
		size_t m_size;
	};

	inline size_t dtb_file_size = 0x80000;
	inline VOID add_file_callback( HANDLE handle, LPCSTR file_name, ULONG64 file_size, VMMDLL_VFS_FILELIST_EXINFO* ex_info )
	{
		if ( !strcmp( "dtb.txt", file_name ) )
			dtb_file_size = file_size;
	}

	class process
	{
	public:
		process( ) : m_name( std::string( ) ), m_pid( 0 ), m_peb( address( ) ), m_base( address( ) ), m_size( 0 ) { }

		process( DWORD pid )
		{
			m_pid = pid;

			VMMDLL_PROCESS_INFORMATION process_info{ };
			process_info.magic = VMMDLL_PROCESS_INFORMATION_MAGIC;
			process_info.wVersion = VMMDLL_PROCESS_INFORMATION_VERSION;

			size_t process_info_sz = sizeof( VMMDLL_PROCESS_INFORMATION );

			if ( !VMMDLL_ProcessGetInformation( vmm_handle, m_pid, &process_info, &process_info_sz ) )
			{
				print( e_print_type::error, "failed to get process information for %i!\n", m_pid );
				return;
			}

			m_peb = process_info.win.vaPEB;
			m_name = process_info.szNameLong;

			VMMDLL_MAP_MODULEENTRY* module_entry{ };
			if ( !VMMDLL_Map_GetModuleFromNameU( vmm_handle, m_pid, nullptr, &module_entry, VMMDLL_MODULE_FLAG_NORMAL ) )
			{
				print( e_print_type::error, "failed to get module entry for %i!\n", m_pid );
				return;
			}

			m_base = address( module_entry->vaBase );
			m_size = module_entry->cbImageSize;

			VMMDLL_MemFree( module_entry );
		};

		process( std::string process_name )
		{
			if ( !VMMDLL_PidGetFromName( vmm_handle, process_name.c_str( ), &m_pid ) )
			{
				print( e_print_type::error, "failed to get process id for %s!\n", process_name.c_str( ) );
				return;
			}

			m_name = process_name;

			VMMDLL_PROCESS_INFORMATION process_info{ };
			process_info.magic = VMMDLL_PROCESS_INFORMATION_MAGIC;
			process_info.wVersion = VMMDLL_PROCESS_INFORMATION_VERSION;

			size_t process_info_sz = sizeof( VMMDLL_PROCESS_INFORMATION );

			if ( !VMMDLL_ProcessGetInformation( vmm_handle, m_pid, &process_info, &process_info_sz ) )
			{
				print( e_print_type::error, "failed to get process information for %s!\n", process_name.c_str( ) );
				return;
			}

			m_peb = address( process_info.win.vaPEB );
			m_eprocess = address( process_info.win.vaEPROCESS );

			VMMDLL_MAP_MODULEENTRY* module_entry{ };
			if ( !VMMDLL_Map_GetModuleFromNameU( vmm_handle, m_pid, nullptr, &module_entry, VMMDLL_MODULE_FLAG_NORMAL ) )
			{
				print( e_print_type::error, "failed to get module entry for %s!\n", process_name.c_str( ) );
				return;
			}

			m_base = address( module_entry->vaBase );
			m_size = module_entry->cbImageSize;

			VMMDLL_MemFree( module_entry );
		};

		DWORD get_process_id( ) const
		{
			return m_pid;
		}

		address get_peb( ) const
		{
			return m_peb;
		}

		address get_base( ) const
		{
			return m_base;
		}

		address get_eprocess( ) const
		{
			return m_eprocess;
		}

		size_t get_size( ) const
		{
			return m_size;
		}

		utils::module operator [ ]( std::string module_name ) const //need to specify namespace??? weird - jakespenda
		{
			return utils::module( module_name, m_pid );
		}

		bool update_directory_table_base( )
		{
			VMMDLL_MAP_MODULEENTRY* module_entry{ };
			if ( VMMDLL_Map_GetModuleFromNameU( vmm_handle, m_pid, m_name.c_str( ), &module_entry, VMMDLL_MODULE_FLAG_NORMAL ) )
				return true;

			if ( !VMMDLL_InitializePlugins( vmm_handle ) )
			{
				print( e_print_type::error, "failed to initialize vmdll plugins!\n" );
				return false;
			}

			sleep( 1s );

			while ( true )
			{
				unsigned char pp_data[ 0x3 ]{ };
				DWORD i{ };

				auto status = VMMDLL_VfsReadU( vmm_handle, "\\misc\\procinfo\\progress_percent.txt", pp_data, 0x3, &i, 0 );
				if ( status == VMMDLL_STATUS_SUCCESS && std::atoi( reinterpret_cast< char* >( pp_data ) ) == 100 )
					break;

				system( "cls" );
				utils::print( e_print_type::normal, "initializing [3/4]\n" );
				utils::print( e_print_type::normal, "updating directory table base %s%%\n", reinterpret_cast< char* >( pp_data ) );

				sleep( 100ms );
			}

			system( "cls" );
			utils::print( e_print_type::normal, "initializing [3/4]\n" );
			utils::print( e_print_type::normal, "updating directory table base 100%%\n" );

			VMMDLL_VFS_FILELIST2 vfs_file_list{ };
			vfs_file_list.dwVersion = VMMDLL_VFS_FILELIST_VERSION;
			vfs_file_list.h = vmm_handle;
			vfs_file_list.pfnAddDirectory = nullptr;
			vfs_file_list.pfnAddFile = add_file_callback;

			if ( !VMMDLL_VfsListU( vmm_handle, "\\misc\\procinfo\\", &vfs_file_list ) )
				return false;

			auto dtb_data = new unsigned char[ dtb_file_size ];
			DWORD i{ };

			auto status = VMMDLL_VfsReadU( vmm_handle, "\\misc\\procinfo\\dtb.txt", dtb_data, dtb_file_size - 1, &i, NULL );
			if ( status != VMMDLL_STATUS_SUCCESS )
			{
				print( e_print_type::error, "failed to read dtb file! status: (0x%llx)\n", status );
				delete[ ] dtb_data;
				return false;
			}

			std::vector<dtb_info> possible_dtbs{ };
			std::istringstream dtb_data_ss( reinterpret_cast< char* >( dtb_data ) );
			std::string current_line{ };

			while ( std::getline( dtb_data_ss, current_line ) )
			{
				dtb_info info{ };

				std::istringstream info_ss( current_line );
				info_ss >> std::hex >> info.index >> std::dec >> info.pid >> std::hex >> info.dtb >> info.addr >> info.name;

				if ( m_name.find( info.name ) != std::string::npos || info.pid == 0 )
					possible_dtbs.push_back( info );
			}

			for ( size_t i = 0; i < possible_dtbs.size( ); i++ )
			{
				VMMDLL_ConfigSet( vmm_handle, VMMDLL_OPT_PROCESS_DTB | m_pid, possible_dtbs[ i ].dtb );

				if ( VMMDLL_Map_GetModuleFromNameU( vmm_handle, m_pid, m_name.c_str( ), &module_entry, VMMDLL_MODULE_FLAG_NORMAL ) )
				{
					m_base = address( module_entry->vaBase );
					m_size = module_entry->cbImageSize;

					VMMDLL_MemFree( module_entry );

					return true;
				}
			}

			return false;
		}
	private:
		struct dtb_info
		{
			uint32_t index;
			uint32_t pid;
			uintptr_t dtb;
			uintptr_t addr;
			std::string name;
		};

		std::string m_name;
		DWORD m_pid;
		address m_peb;
		address m_eprocess;
		address m_base;
		size_t m_size;
	};

	inline std::vector<DWORD> get_csrss_pids( )
	{
		std::vector<DWORD>possible_pids{ };

		VMMDLL_PROCESS_INFORMATION* process_info{ };
		DWORD process_count{ };

		if ( !VMMDLL_ProcessGetInformationAll( vmm_handle, &process_info, &process_count ) )
		{
			print( e_print_type::error, "failed to get process list!\n" );
			return possible_pids;
		}

		for ( DWORD i = 0; i < process_count; i++ )
			if ( strstr( process_info[ i ].szNameLong, "csrss.exe" ) )
				possible_pids.push_back( process_info[ i ].dwPID );

		if ( !possible_pids.size( ) )
		{
			print( e_print_type::error, "failed to find csrss processes!\n" );
			return possible_pids;
		}

		return possible_pids;
	}

	class module_export
	{
	public:
		module_export( std::string module_name )
		{
			auto crss_pids = get_csrss_pids( );
			for ( auto pid : crss_pids )
			{
				VMMDLL_MAP_EAT* module_export_table{ };
				if ( !VMMDLL_Map_GetEATU( vmm_handle, pid | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, module_name.c_str( ), &module_export_table ) )
					continue;

				if ( !module_export_table->cMap )
				{
					VMMDLL_MemFree( module_export_table );
					continue;
				}

				VMMDLL_MAP_EATENTRY* module_export_table_entry{ };
				for ( int i = 0; i < module_export_table->cMap; i++ )
				{
					module_export_table_entry = module_export_table->pMap + i;
					module_exports.emplace( std::make_pair( module_export_table_entry->uszFunction, module_export_table_entry->vaFunction ) );
				}

				VMMDLL_MemFree( module_export_table );
				break;
			}
		}

		address operator [ ]( std::string export_name ) const
		{
			auto pos = module_exports.find( export_name );
			if ( pos == module_exports.end( ) )
				return nullptr;

			return pos->second;
		}

	private:
		std::unordered_map<std::string, uintptr_t> module_exports;
	};

	inline process target_process{ };

	inline bool read( uintptr_t addr, void* buffer, size_t size, DWORD pid = target_process.get_process_id( ) )
	{
		DWORD read_size{ };
		if ( !VMMDLL_MemReadEx( vmm_handle, pid, addr, static_cast< unsigned char* >( buffer ), size, &read_size, VMMDLL_FLAG_NOCACHE ) )
		{
			print( e_print_type::error, "failed to read memory at 0x%llx!\n", addr );
			return false;
		}

		return ( read_size == size );
	}

	template <typename t>
	inline t read( uintptr_t addr, DWORD pid = target_process.get_process_id( ) )
	{
		t buffer{ };
		if ( !read( addr, reinterpret_cast< void* >( &buffer ), sizeof( t ), pid ) )
			return buffer;

		return buffer;
	}

	inline const char* read_string( uintptr_t addr )
	{
		char buffer[ 0x400 ]{ };
		read( addr, reinterpret_cast< void* >( &buffer ), 0x400 );

		return buffer;
	}

	inline const wchar_t* read_wstring( uintptr_t addr )
	{
		wchar_t buffer[ 0x400 ]{ };
		read( addr, reinterpret_cast< void* >( &buffer ), 0x400 );

		return buffer;
	}

	inline bool write( uintptr_t addr, void* buffer, size_t size )
	{
		if ( !VMMDLL_MemWrite( vmm_handle, target_process.get_process_id( ), addr, static_cast< unsigned char* >( buffer ), size ) )
		{
#if defined(_DEBUG) || defined(RELEASE_EXE)
			print( e_print_type::error, "failed to write memory at 0x%llx!\n", addr );
#endif
			return false;
		}

		return true;
	}

	template <typename t>
	inline void write( uintptr_t addr, t value )
	{
		write( addr, &value, sizeof( t ) );
	}

	class pattern
	{
	public:
		pattern( ) : m_addr( address( ) ) { }
		pattern( uintptr_t addr ) : m_addr( addr ) { }

		static pattern find( const char* module_name, const char* patt, utils::process process = target_process )
		{
			uintptr_t image_start{ };
			uintptr_t image_end{ };

			if ( !module_name )
			{
				image_start = process.get_base( ).as<uintptr_t>( );
				image_end = image_start + process.get_size( );
			}
			else
			{
				image_start = process[ module_name ].get_base( ).as<uintptr_t>( );
				image_end = image_start + process[ module_name ].get_size( );
			}

			size_t pattern_length = strlen( patt );
			char* pat1 = const_cast< char* >( patt );
			pattern first_match{ };

			std::vector<uint8_t> buffer( image_end - image_start );
			if ( !VMMDLL_MemReadEx( vmm_handle, target_process.get_process_id( ), image_start, buffer.data( ), buffer.size( ), 0, VMMDLL_FLAG_NOCACHE ) )
				return first_match;

			for ( uintptr_t current_address = image_start; current_address < image_end; current_address++ )
			{
				if ( !*pat1 )
					return first_match;

				if ( *reinterpret_cast< uint8_t* >( pat1 ) == '\?' || buffer[ current_address - image_start ] == get_byte( pat1 ) )
				{
					if ( !first_match.m_addr.as<uintptr_t>( ) )
						first_match.m_addr = current_address;

					if ( !pat1[ 2 ] )
						return first_match;

					if ( *reinterpret_cast< uint16_t* >( pat1 ) == '\?\?' || *reinterpret_cast< uint8_t* >( pat1 ) != '\?' )
						pat1 += 3;
					else
						pat1 += 2;
				}
				else
				{
					pat1 = const_cast< char* >( patt );
					first_match.m_addr = 0;
				}
			}

			return first_match;
		}

		address resolve_relative( ) const
		{
			if ( !m_addr.as<uintptr_t>( ) )
				return address( );

			int32_t offset = read<int32_t>( m_addr.add( 3 ).as<uintptr_t>( ) );
			return m_addr.add( offset ).add( 7 ).as<uintptr_t>( );
		}

		address resolve_call( ) const
		{
			if ( !m_addr.as<uintptr_t>( ) )
				return address( );

			int32_t offset = read<int32_t>( m_addr.add( 1 ).as<uintptr_t>( ) );
			return m_addr.add( offset ).add( 5 ).as<uintptr_t>( );
		}

		address get( ) const
		{
			return m_addr;
		}

	private:
		address m_addr;
	};

	class scatter
	{
	public:
		scatter( )
		{
			m_handle = VMMDLL_Scatter_Initialize( vmm_handle, target_process.get_process_id( ), VMMDLL_FLAG_NOCACHE | VMMDLL_FLAG_NOCACHEPUT );
			if ( !m_handle )
			{
#if defined(_DEBUG) || defined(RELEASE_EXE)
				print( e_print_type::error, "failed to create scatter handle!\n" );
#endif
			}
		}

		~scatter( )
		{
			close( );
		}

		bool push_back_read( uintptr_t addr, void* buffer, size_t size )
		{
			DWORD read_size{ };
			if ( !VMMDLL_Scatter_PrepareEx( m_handle, addr, size, static_cast< unsigned char* >( buffer ), &read_size ) )
			{
#if defined(_DEBUG) || defined(RELEASE_EXE)
				print( e_print_type::error, "failed to push scatter read at 0x%llx!\n", addr );
#endif
				return false;
			}

			return ( read_size == size );
		}

		template<typename T>
		bool push_back_read( uintptr_t addr, void* buffer )
		{
			DWORD read_size{ };
			if ( !VMMDLL_Scatter_PrepareEx( m_handle, addr, sizeof( T ), static_cast< unsigned char* >( buffer ), &read_size ) )
			{
#if defined(_DEBUG) || defined(RELEASE_EXE)
				print( e_print_type::error, "failed to push scatter read at 0x%llx!\n", addr );
#endif
				return false;
			}

			return ( read_size == sizeof( T ) );
		}

		bool push_back_write( uintptr_t addr, void* buffer, size_t size )
		{
			if ( !VMMDLL_Scatter_PrepareWrite( m_handle, addr, static_cast< unsigned char* >( buffer ), size ) )
			{
#if defined(_DEBUG) || defined(RELEASE_EXE)
				print( e_print_type::error, "failed to push scatter write at 0x%llx!\n", addr );
#endif
				return false;
			}

			return true;
		}

		template <typename T>
		bool push_back_write( uintptr_t addr, T value )
		{
			if ( !VMMDLL_Scatter_PrepareWrite( m_handle, addr, reinterpret_cast< unsigned char* >( &value ), sizeof( T ) ) )
			{
#if defined(_DEBUG) || defined(RELEASE_EXE)
				print( e_print_type::error, "failed to push scatter write at 0x%llx!\n", addr );
#endif
				return false;
			}

			return true;
		}

		void execute_read( )
		{
			if ( !m_handle )
				return;

			if ( !VMMDLL_Scatter_ExecuteRead( m_handle ) )
			{
#if defined(_DEBUG) || defined(RELEASE_EXE)
				print( e_print_type::error, "failed to execute scatter read!\n" );
#endif
			}

			if ( !VMMDLL_Scatter_Clear( m_handle, target_process.get_process_id( ), VMMDLL_FLAG_NOCACHE | VMMDLL_FLAG_NOCACHEPUT ) )
			{
#if defined(_DEBUG) || defined(RELEASE_EXE)
				print( e_print_type::error, "failed to clear scatter!\n" );
#endif
			}
		}

		void execute_write( )
		{
			if ( !VMMDLL_Scatter_Execute( m_handle ) )
			{
#if defined(_DEBUG) || defined(RELEASE_EXE)
				print( e_print_type::error, "failed to execute scatter write!\n" );
#endif
			}

			if ( !VMMDLL_Scatter_Clear( m_handle, target_process.get_process_id( ), VMMDLL_FLAG_NOCACHE | VMMDLL_FLAG_NOCACHEPUT ) )
			{
#if defined(_DEBUG) || defined(RELEASE_EXE)
				print( e_print_type::error, "failed to clear scatter!\n" );
#endif
			}
		}

		void close( ) const
		{
			VMMDLL_Scatter_CloseHandle( m_handle );
		}

	private:
		VMMDLL_SCATTER_HANDLE m_handle;
	};

	inline int get_windows_version( )
	{
		std::vector<int>windows_10_versions{ 10240, 10586, 14393, 15063, 16299, 17134, 17763, 18362, 18363, 19041, 19042, 19043, 19044, 19045, 22000 };
		//22000 is win11 but it uses the export still :/

		DWORD type = REG_SZ;
		BYTE buffer[ 0x100 ];
		DWORD size = sizeof( buffer );

		if ( !VMMDLL_WinReg_QueryValueExU( vmm_handle, "HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\CurrentBuild", &type, buffer, &size ) )
		{
			print( e_print_type::error, "failed to get windows version!\n" );
			return 0;
		}

		auto buffer_wide = std::wstring( reinterpret_cast< wchar_t* >( buffer ) );
		auto buffer_str = std::string( buffer_wide.begin( ), buffer_wide.end( ) );

		auto winver = std::atoi( buffer_str.c_str( ) );

		for ( auto win10_vers : windows_10_versions )
			if ( winver == win10_vers )
				return 1;

		return winver;
	}

	inline int get_windows_minor_version( )
	{
		DWORD type = REG_DWORD;
		DWORD buffer = 0;
		DWORD size = sizeof( buffer );

		if ( !VMMDLL_WinReg_QueryValueExU( vmm_handle, "HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\UBR", &type, ( LPBYTE )&buffer, &size ) )
		{
			print( e_print_type::error, "failed to get minor windows version!\n" );
			return 0;
		}

		return buffer;
	}

	class key
	{
	public:
		static bool init( )
		{
			winlogon_process_id = process( "winlogon.exe" ).get_process_id( );
			if ( !winlogon_process_id )
			{
				print( e_print_type::error, "failed to get winlogon process id!\n" );
				return false;
			}

			auto windows_version = get_windows_version( );
			auto windows_minor_version = get_windows_minor_version( );
			if ( !windows_version || !windows_minor_version )
			{
				print( e_print_type::error, "failed to get windows version!\n" );
				return false;
			}

			if ( windows_version == 1 )
			{
				async_key_state_export = module_export( "win32kbase.sys" )[ "gafAsyncKeyState" ].as<uintptr_t>( );
				if ( !async_key_state_export )
				{
					print( e_print_type::error, "failed to find module export for get async key state!\n" );
					return false;
				}
			}
			else
			{
				auto offset = 0x3690;
				switch ( windows_version )
				{
					case 22631:
					{
						if ( windows_minor_version >= 3810 )
							offset = 0x36a8;
					}
					break;
				}

				auto crss_pids = get_csrss_pids( );
				for ( auto pid : crss_pids )
				{
					auto win32ksgd_base = process( pid )[ "win32ksgd.sys" ].get_base( ).as<uintptr_t>( );
					auto session_global_slots = utils::read<uintptr_t>( win32ksgd_base + 0x3110, pid );

					uintptr_t user_session_state = 0;
					for ( int i = 0; i < 4; i++ )
					{
						user_session_state = utils::read<uintptr_t>( utils::read<uintptr_t>( session_global_slots + 8 * i, pid ), pid );
						if ( user_session_state > 0x7FFFFFFFFFFF )
							break;
					}

					async_key_state_export = user_session_state + offset;
					if ( async_key_state_export > 0x7FFFFFFFFFFF )
						break;
				}
			}

			return true;
		}

	public:
		key( int vkey ) : m_vkey( vkey ) { }

		bool is_held( ) const
		{
			if ( m_vkey == 0x0 )
				return false;

			return key_state_map[ ( m_vkey * 2 / 8 ) ] & 1 << m_vkey % 4 * 2;
		}

	public:
		static bool update( )
		{
			uint8_t key_state_map_backup[ 0x40 ]{ };
			memcpy( key_state_map_backup, key_state_map, 0x40 );

			DWORD read_size{ };
			VMMDLL_MemReadEx( vmm_handle, winlogon_process_id | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, async_key_state_export, reinterpret_cast< unsigned char* >( &key_state_map ), 0x40, &read_size, VMMDLL_FLAG_NOCACHE );

			for ( int vkey = 0; vkey < 0x100; vkey++ )
				if ( ( key_state_map[ ( vkey * 2 / 8 ) ] & 1 << vkey % 4 * 2 ) && !( key_state_map_backup[ ( vkey * 2 / 8 ) ] & 1 << vkey % 4 * 2 ) )
					key_state_map_prev[ vkey / 8 ] |= 1 << vkey % 8;

			return true;
		}

		static inline uintptr_t async_key_state_export;
		static inline DWORD winlogon_process_id;

		static inline uint8_t key_state_map[ 0x40 ];
		static inline uint8_t key_state_map_prev[ 0x40 ];

		int m_vkey;
	};

	class cursor
	{
	public:
		static bool init( )
		{
			winlogon_process_id = process( "winlogon.exe" ).get_process_id( );
			if ( !winlogon_process_id )
			{
				print( e_print_type::error, "failed to get winlogon process id!\n" );
				return false;
			}

			cursor_async_export = module_export( "win32kbase.sys" )[ "gptCursorAsync" ].as<uintptr_t>( );
			if ( !cursor_async_export )
			{
				print( e_print_type::error, "failed to find module export for cursor async!\n" );
				return false;
			}

			return true;
		}

		static POINT get_pos( )
		{
			POINT out_pos{ };

			DWORD read_size{ };
			VMMDLL_MemReadEx( vmm_handle, winlogon_process_id | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, cursor_async_export, reinterpret_cast< unsigned char* >( &out_pos ), sizeof( POINT ), &read_size, VMMDLL_FLAG_NOCACHE );

			return out_pos;
		}

		static void set_pos( long x, long y )
		{
			POINT pos{ x, y };
			VMMDLL_MemWrite( vmm_handle, winlogon_process_id | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, cursor_async_export, reinterpret_cast< unsigned char* >( &pos ), sizeof( POINT ) );
		}

	private:
		static inline uintptr_t cursor_async_export;
		static inline DWORD winlogon_process_id;
	};

	class kmbox
	{
		struct kmboxb
		{
			uint64_t baudrate = 115200;
			NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT( kmboxb, baudrate );
		} kmb;

		struct kmboxnet
		{
			std::string ip = "0.0.0.0";
			std::string port = "0000";
			std::string uuid = "00000000";
			NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT( kmboxnet, ip, port, uuid );
		} kmn;
	public:

		~kmbox( ) { close( ); }

		bool init( )
		{
			auto cfg_path = std::filesystem::current_path( ).string( ) + "\\paionia\\";
			if ( !std::filesystem::exists( cfg_path ) )
				std::filesystem::create_directories( cfg_path );

			auto cfg_file = std::filesystem::current_path( ).string( ) + "\\paionia\\kmbox.pi";
			if ( !std::filesystem::exists( cfg_file ) )
			{
				nlohmann::json cfg = { };
				cfg[ "kmboxb" ] = kmb;
				cfg[ "kmboxnet" ] = kmn;
				std::ofstream out( cfg_file );
				out << cfg.dump( 4 );
				out.close( );
			}

			std::ifstream cfg_stream( cfg_file );

			auto cfg = nlohmann::json::parse( cfg_stream );

			kmb = cfg[ "kmboxb" ].get<kmboxb>( );
			is_net = !open( kmb.baudrate );
			if ( is_net )
			{
				kmn = cfg[ "kmboxnet" ].get<kmboxnet>( );
				if ( kmNet_init( ( char* )kmn.ip.c_str( ), ( char* )kmn.port.c_str( ), ( char* )kmn.uuid.c_str( ) ) != 0 )
					return false;

				print( e_print_type::good, "connected to kmboxnet.\n" );

				//kmNet_lcd_picture_bottom( logo_lcd );
				return true;
			}

			return true;
		}

		void close( )
		{
			CloseHandle( com_handle );
			com_handle = INVALID_HANDLE_VALUE;
		}

		bool is_kmnet( ) const
		{
			return is_net;
		}

		void move_mouse( short x, short y, bool encrypt = false )
		{
			if ( !is_net )
				send_data( "km.move(" + std::to_string( x ) + "," + std::to_string( y ) + ",5)\r\n" );
			else
				encrypt ? kmNet_enc_mouse_move( x, y ) : kmNet_mouse_move( x, y );
		}

		void left_click( long long delay, bool encrypt = false )
		{
			if ( is_net )
			{
				utils::sleep( std::chrono::milliseconds( delay ) );
				encrypt ? kmNet_enc_mouse_left( 1 ) : kmNet_mouse_left( 1 );
				utils::sleep( std::chrono::milliseconds( 15 ) );
				encrypt ? kmNet_enc_mouse_left( 0 ) : kmNet_mouse_left( 0 );
			}
			else
			{
				utils::sleep( std::chrono::milliseconds( delay ) );
				send_data( "km.left(1)\r\n" );
				utils::sleep( std::chrono::milliseconds( 15 ) );
				send_data( "km.left(0)\r\n" );
			}
		}

	private:
		bool open( DWORD baudrate )
		{
			LPCSTR device_name = "USB-SERIAL CH340";

			char port[ 120 ]{};
			strcat_s( port, "\\\\.\\" );

			if ( !scan_devices( device_name, port ) )
				return false;

			if ( com_handle != INVALID_HANDLE_VALUE )
				close( );

			com_handle = CreateFileA( port,
									  GENERIC_READ | GENERIC_WRITE,
									  0,
									  NULL,
									  OPEN_EXISTING,
									  FILE_ATTRIBUTE_NORMAL,
									  NULL );

			if ( com_handle == INVALID_HANDLE_VALUE )
				return false;


			DCB dcb = { 0 };
			dcb.DCBlength = sizeof( dcb );
			GetCommState( com_handle, &dcb );

			dcb.BaudRate = baudrate;
			dcb.ByteSize = 8;
			dcb.StopBits = ONESTOPBIT;
			dcb.Parity = NOPARITY;

			SetCommState( com_handle, &dcb );

			COMMTIMEOUTS cto = { 0 };
			cto.ReadIntervalTimeout = 1;
			cto.ReadTotalTimeoutConstant = 0;
			cto.ReadTotalTimeoutMultiplier = 0;
			cto.WriteTotalTimeoutConstant = 0;
			cto.WriteTotalTimeoutMultiplier = 0;

			SetCommTimeouts( com_handle, &cto );

			print( e_print_type::good, "connected to kmbox b pro.\n" );
			return true;
		}

		bool scan_devices( LPCSTR device_name, LPSTR out )
		{
			bool status = false;
			char com[ ] = "COM";

			HDEVINFO device_info = SetupDiGetClassDevsA( &GUID_DEVCLASS_PORTS, NULL, NULL, DIGCF_PRESENT );
			if ( device_info == INVALID_HANDLE_VALUE )
				return false;

			SP_DEVINFO_DATA dev_info_data;
			dev_info_data.cbSize = sizeof( dev_info_data );

			DWORD count = 0;

			while ( SetupDiEnumDeviceInfo( device_info, count++, &dev_info_data ) )
			{
				BYTE buffer[ 256 ];
				if ( SetupDiGetDeviceRegistryPropertyA( device_info, &dev_info_data, SPDRP_FRIENDLYNAME, NULL, buffer, sizeof( buffer ), NULL ) )
				{
					DWORD i = ( DWORD )strlen( out );
					LPCSTR lp_pos = strstr( ( LPCSTR )buffer, com );
					if ( !lp_pos )
						continue;

					DWORD len = i + ( DWORD )strlen( lp_pos );

					if ( strstr( ( LPCSTR )buffer, device_name ) && lp_pos )
					{
						for ( DWORD j = 0; i < len; i++, j++ )
						{
							out[ i ] = lp_pos[ j ];
						}

						out[ i - 1 ] = '\0';
						status = true;
						break;
					}
				}
			}

			SetupDiDestroyDeviceInfoList( device_info );
			return status;
		}

		bool send_data( const std::string& data )
		{
			if ( com_handle == INVALID_HANDLE_VALUE )
				return false;

			DWORD bytes_written;
			return WriteFile( com_handle, data.c_str( ), data.size( ), &bytes_written, nullptr ) && bytes_written == data.size( );
		}

		bool receive_data( std::string& data, size_t size )
		{
			if ( com_handle == INVALID_HANDLE_VALUE || size == 0 )
				return false;

			auto buffer = std::make_unique<char>( 256 );
			DWORD bytes_read;
			if ( !ReadFile( com_handle, buffer.get( ), size, &bytes_read, nullptr ) )
				return false;

			data.assign( buffer.get( ), bytes_read );
			return true;
		}

	public:
		bool is_connected = false;

	private:
		bool is_net = false;
		HANDLE com_handle = INVALID_HANDLE_VALUE;
		DCB dcb{};
		COMMTIMEOUTS co{};
	};

	inline kmbox kmb{ };
}