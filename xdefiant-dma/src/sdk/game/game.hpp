#pragma once

#include "../../utils/utils.hpp"
#include "../math/math.hpp"

#define __ROL8__ _rotl64
#define __ROR8__ _rotr64

namespace sdk::offsets
{
    //player
    constexpr int32_t location = 0x80;
    constexpr int32_t entity_type = 0x530;
    constexpr int32_t entity_name = 0x4a0;
    constexpr int32_t level = 0x354;
    constexpr int32_t skill_script_vars = 0x280;
    constexpr int32_t animation_1 = 0x0;
    constexpr int32_t animation_2 = 0x0;
    constexpr int32_t bone_data_1 = 0x2D8;
    constexpr int32_t bone_data_2 = 0x2E0;
    constexpr int32_t bone_matrix_1 = 0x0;
    constexpr int32_t bone_matrix_2 = 0x0;

    //skill script vars
    constexpr int32_t is_sprinting_value = 0x19F;
    constexpr int32_t is_in_riot_shield_value = 0x19B;
    constexpr int32_t is_crouched_value = 0x3BC;

    //team component
    constexpr int32_t team_id = 0x10;

    //health component
    constexpr int32_t current_health = 0x1C8;
    constexpr int32_t max_health = 0x1C4;

    //weapon data 2
    constexpr int32_t my_bullets_per_round = 0xec;  //myBulletsPerRound

    //weapon data
    constexpr int32_t my_reload_time_ms = 0x2a0; //myReloadTimeMS
    constexpr int32_t my_empty_clip_reload_time_ms = 0x2b0; //myEmptyClipFirstRoundReloadTimeMS
    constexpr int32_t my_sprint_shoot_delay_ms = 0x2d8; //mySprintShootDelayMS
    constexpr int32_t my_spread_min = 0x2f8; //mySpreadMin
    constexpr int32_t my_spread_max = 0x2fc; //mySpreadMax
    constexpr int32_t my_crouch_spread_min = 0x328; //myCrouchSpreadMin
    constexpr int32_t my_crouch_spread_max = 0x32c; //myCrouchSpreadMax
    constexpr int32_t my_crouch_slide_spread_min = 0x340; //myCrouchSlideSpreadMin
    constexpr int32_t my_crouch_slide_spread_max = 0x344; //myCrouchSlideSpreadMax
    constexpr int32_t my_in_air_spread_min = 0x358; //myInAirSpreadMin
    constexpr int32_t my_in_air_spread_max = 0x35c; //myInAirSpreadMax
    constexpr int32_t my_movement_modifier = 0x414; //myMovementModifier
    constexpr int32_t my_sway_movement_speed = 0x44c; //mySwayMovementSpeed
    constexpr int32_t my_first_person_fov_chance = 0x488; //myFirstPersonFovChange
    constexpr int32_t my_first_person_zoom_fov_change = 0x48c; //myFirstPersonZoomFovChange
    constexpr int32_t my_zoom_in_duration = 0x4b8; //myZoomInDuration
    constexpr int32_t my_aiming_move_speed_modifier = 0x4d4; //myAimingMoveSpeedModifier
    constexpr int32_t my_rpm = 0x4ec; //myRPM
    constexpr int32_t my_agent_bullet_penetration_count = 0x608; //myAgentBulletPenetrationCount
    constexpr int32_t my_jump_max_height_aiming_factor = 0x151C; //myJumpMaxHeightAimingFactor

    //bone positions
    inline uintptr_t head = 3;
    inline uintptr_t neck = 10;
    inline uintptr_t upper_spine = 9;
    inline uintptr_t lower_spine = 6;
    inline uintptr_t pelvis = 5;

    inline uintptr_t left_shoulder = 17;
    inline uintptr_t left_elbow = 18;
    inline uintptr_t left_hand = 81;

    inline uintptr_t left_thigh = 84;
    inline uintptr_t left_knee = 85;
    inline uintptr_t left_calf = 89;
    inline uintptr_t left_foot = 86;

    inline uintptr_t right_shoulder = 47;
    inline uintptr_t right_elbow = 48;
    inline uintptr_t right_hand = 75;

    inline uintptr_t right_thigh = 92;
    inline uintptr_t right_knee = 93;
    inline uintptr_t right_calf = 97;
    inline uintptr_t right_foot = 94;
}

#define BYTEn(x, n)   (*((_BYTE*)&(x)+n))

#define BYTE2(x)   BYTEn(x,  2)

namespace sdk
{
    inline uintptr_t get_world( ) //E8 ? ? ? ? 33 F6 48 85 C0 0F 84
    {
        __int64 v6; // r8
        unsigned __int8 v19; // al
        __int64 v20; // r9
        uintptr_t v21; // rdx
        unsigned __int64 v22; // rdx
        unsigned __int8 v23; // cl
        __int64 v24; // r10
        uintptr_t v25; // r9
        __int64 v26; // rsi
        unsigned __int8 v27; // cl
        __int64 v28; // r9

        v6 = utils::read<uintptr_t>( utils::target_process.get_base( ).add( 0x75f7fd8 ).as<uintptr_t>( ) );;

        v19 = utils::read<BYTE>( utils::target_process.get_base( ).add( 0x75f7fa8 ).as<uintptr_t>( ) + 2 ) & 3;
        if ( ( utils::read<uintptr_t>( utils::target_process.get_base( ).add( 0x75f7fa8 ).as<uintptr_t>( ) ) & 0x30000 ) == 0 )
            v19 = 1;

        v20 = v19 - 1LL;
        if ( v20 >= 0 )
        {
            uintptr_t r13 = utils::target_process.get_base( ).add( 0x75f7fb0 ).as<uintptr_t>( );
            uintptr_t rdx = v20 << 4;
            rdx += r13;

            v21 = rdx;
            do
            {
                if ( ( utils::read<BYTE>( v21 + 12 ) & 3 ) != 0 )
                {
                    if ( ( utils::read<BYTE>( v21 + 12 ) & 3 ) == 1 )
                    {
                        v6 = __ROR8__( v6, utils::read<BYTE>( v21 ) & 0x3F ) ^ 0x1BF7E721FDA87395LL;
                    }
                    else if ( ( utils::read<BYTE>( v21 + 12 ) & 3 ) == 2 )
                    {
                        v6 = __ROR8__( v6, utils::read<BYTE>( v21 ) & 0x3F ) ^ 0x7F7CD68D1529C655LL;
                    }
                }
                else
                {
                    v6 = __ROR8__( v6, utils::read<BYTE>( v21 ) & 0x3F ) - 0x33EFE7E7C0B9622LL;
                }
                v21 -= 0x10;
                --v20;
            }
            while ( v20 >= 0 );
        }

        v22 = utils::read<QWORD>( v6 + 0x7C8 );
        v23 = utils::read<BYTE>( v6 + 0x78D ) & 3;
        if ( !v23 )
            v23 = 1;
        v24 = v23 - 1LL;
        if ( v24 >= 0 )
        {
            v25 = ( v6 + 24 * ( v24 + 0x51 ) );
            do
            {
                if ( ( utils::read<BYTE>( v25 - 6 ) & 3 ) != 0 )
                {
                    if ( ( utils::read<BYTE>( v25 - 6 ) & 3 ) == 1 )
                    {
                        v22 = __ROR8__( __ROR8__( v22, utils::read<QWORD>( v25 ) & 0x3F ), 17 );
                    }
                    else if ( ( utils::read<BYTE>( v25 - 6 ) & 3 ) == 2 )
                    {
                        v22 = __ROR8__( __ROR8__( v22, utils::read<QWORD>( v25 ) & 0x3F ), 10 );
                    }
                }
                else
                {
                    v22 = __ROR8__( utils::read<QWORD>( v25 ) ^ v22, 40 );
                }
                v25 -= 24;
                --v24;
            }
            while ( v24 >= 0 );
        }

        v26 = utils::read<QWORD>( v22 + 0xD8 );
        v27 =  utils::read<BYTE>( v22 + 0x92 ) & 3;
        if ( !v27 )
            v27 = 1;

        v28 = v27 - 1LL;
        if ( v28 >= 0 )
        {
            v6 = v22 + 8 * ( v28 + 2 * ( v28 + 10 ) );
            v22 = 0xC0FBE27FEDDAC602uLL;
            do
            {
                if ( ( utils::read<BYTE>( v6 - 7 ) & 3 ) != 0 )
                {
                    if ( ( utils::read<BYTE>( v6 - 7 ) & 3 ) == 1 )
                        v26 = __ROR8__( v26, utils::read<BYTE>( v6 ) & 0x3F ) ^ 0x6946AA58328F7C95LL;
                }
                else
                {
                    v26 = ( utils::read<QWORD>( v6 ) ^ v26 ) - 0x3F041D80122539FELL;
                }
                v6 -= 24LL;
                --v28;
            }
            while ( v28 >= 0 );
        }

        return v26;
    }

    inline uintptr_t get_player_list( uintptr_t a1 ) //E8 ? ? ? ? 48 83 C4 48 C3 8B 41 08
    {
        a1 = a1 + 0x898;

        __int64 v1; // rax
        unsigned __int8 v2; // r8
        __int64 v3; // r9
        uintptr_t v4; // r8
        __int64 v5; // rcx

        v1 = utils::read<QWORD>( a1 );
        v2 = utils::read<BYTE>( a1 + 20 ) & 3;
        if ( !v2 )
            v2 = 1;
        v3 = v2 - 1LL;
        if ( v3 >= 0 )
        {
            v4 = ( a1 + 8 * ( v3 + 2 * ( v3 + 2 ) ) );
            do
            {
                if ( ( utils::read<BYTE>( v4 - 5 ) & 3 ) != 0 )
                {
                    if ( ( utils::read<BYTE>( v4 - 5 ) & 3 ) == 1 )
                    {
                        v1 = __ROR8__( __ROR8__( v1, utils::read<QWORD>( v4 ) & 0x3F ), 14 );
                    }
                    else if ( ( utils::read<BYTE>( v4 - 5 ) & 3 ) == 2 )
                    {
                        v1 = __ROR8__( v1, utils::read<QWORD>( v4 ) & 0x3F ) - 0x11DC4D0D706A444ALL;
                    }
                }
                else
                {
                    v1 = __ROR8__( v1, utils::read<QWORD>( v4 ) & 0x3F ) ^ 0x6804C6C72BB97258LL;
                }
                v4 -= 24;
                --v3;
            }
            while ( v3 >= 0 );
        }

        return v1;
    }

    inline uintptr_t get_view_data( uintptr_t world ) //str: No UI Camera
    {
        __int64 v35; // rdx
        __int64 v36; // rbx
        unsigned __int8 v37; // cl
        __int64 v38; // r8
        uintptr_t v39; // rdx
        __int64 result; // rax
        unsigned __int8 v2; // dl
        __int64 v3; // r9
        uintptr_t v4; // r8

        v35 = world + 0xC48;
        v36 = utils::read<QWORD>( world + 0xC38 );

        v37 = utils::read<BYTE>( v35 + 0x3A ) & 3;
        if ( !v37 )
            v37 = 1;

        v38 = v37 - 1LL;
        if ( v38 >= 0 )
        {
            v39 = ( v35 + 24 * v38 + 16 );
            do
            {
                if ( ( utils::read<BYTE>( v39 - 7 ) & 3 ) != 0 )
                {
                    if ( ( utils::read<BYTE>( v39 - 7 ) & 3 ) == 1 )
                        v36 ^= utils::read<QWORD>( v39 ) ^ 0x6CA6AB607EDD30EBLL;
                }
                else
                {
                    v36 = __ROR8__( utils::read<QWORD>( v39 ) ^ v36, 3 );
                }
                v39 -= 0x18;
                --v38;
            }
            while ( v38 >= 0 );
        }

        auto a1 = v36 + 0x478; //str: Game Camera Position 2nd xref

        result = utils::read<QWORD>( a1 + 64 );
        v2 = utils::read<BYTE>( a1 + 61 ) & 3;
        if ( !v2 )
            v2 = 1;
        v3 = v2 - 1LL;
        if ( v3 >= 0 )
        {
            v4 = ( a1 + 8 + 24 * v3 + 8 );
            do
            {
                if ( ( utils::read<BYTE>( v4 - 6 ) & 3 ) != 0 )
                {
                    if ( ( utils::read<BYTE>( v4 - 6 ) & 3 ) == 1 )
                        result ^= utils::read<QWORD>( v4 ) ^ 0xB14F91BE5EFFA6ALL;
                }
                else
                {
                    result = __ROR8__( result, utils::read<BYTE>( v4 ) & 0x3F ) ^ 0x5B4DAC68B1A698E8LL;
                }
                v4 -= 0x18;
                --v3;
            }
            while ( v3 >= 0 );
        }

        return result;
    }

    inline uintptr_t decrypt_crosshair_component( uintptr_t a1 ) //E8 ? ? ? ? 48 8B 47 ? 83 B8 ? ? ? ? ? 4C 8B BC 24
    {
        __int64 result; // rax
        unsigned __int8 v2; // dl
        __int64 v3; // r9
        uintptr_t v4; // r8

        result = utils::read<QWORD>( a1 + 0xCE0 );
        v2 = utils::read<BYTE>( a1 + 0xC99 ) & 3;
        if ( !v2 )
            v2 = 1;

        v3 = v2 - 1LL;
        if ( v3 >= 0 )
        {
            v4 = a1 + 0xC98 + 24 * v3 + 16;
            do
            {
                if ( ( utils::read<BYTE>( v4 + 8 ) & 3 ) != 0 )
                {
                    if ( ( utils::read<BYTE>( v4 + 8 ) & 3 ) == 1 )
                    {
                        result = __ROR8__( result, utils::read<BYTE>( v4 ) & 0x3F ) - 0x523685A68DB9EA42LL;
                    }
                    else if ( ( utils::read<BYTE>( v4 + 8 ) & 3 ) == 2 )
                    {
                        result = __ROR8__( result, utils::read<BYTE>( v4 ) & 0x3F ) - 0x6448DECEA34E84BFLL;
                    }
                }
                else
                {
                    result = __ROR8__( __ROR8__( result, utils::read<BYTE>( v4 ) & 0x3F ), 14 );
                }

                v4 -= 24LL;
                --v3;
            }
            while ( v3 >= 0 );
        }

        return result;
    }

    inline uintptr_t decrypt_team_component( uintptr_t v3 ) //40 53 56 57 41 55 48 83 EC ? 48 8B F9
    {
        __int64 v6; // rax
        unsigned __int8 v7; // dl
        __int64 v8; // r9
        __int64 v9; // r8

        v9 = v3 + 0x1088;
        v6 = utils::read<QWORD>( v3 + 0x1078 );
        v7 = utils::read<BYTE>( v3 + 0x10C0 ) & 3;
        if ( !v7 )
            v7 = 1;
        v8 = v7 - 1LL;
        if ( v8 >= 0 )
        {
            v9 += 24 * v8 + 16;
            do
            {
                if ( ( utils::read<BYTE>( v9 - 8 ) & 3 ) != 0 )
                {
                    if ( ( utils::read<BYTE>( v9 - 8 ) & 3 ) == 1 )
                    {
                        v6 = ( v6 - utils::read<QWORD>( v9 ) ) ^ 0x5936D100663B6A79LL;
                    }
                    else if ( ( utils::read<BYTE>( v9 - 8 ) & 3 ) == 2 )
                    {
                        v6 = ( v6 - utils::read<QWORD>( v9 ) ) ^ 0x216F7726813EF1B5LL;
                    }
                }
                else
                {
                    v6 = __ROR8__( v6, utils::read<BYTE>( v9 ) & 0x3F ) ^ 0x512237AC7C9616C4LL;
                }
                v9 -= 24ll;
                --v8;
            }
            while ( v8 >= 0 );
        }

        return v6;
    }

    inline uintptr_t decrypt_health_component( uintptr_t a1 ) //40 53 48 83 EC ? 41 C7 00 ? ? ? ? 49 8B D8 49 8B 09
    {
        return __ROR8__( utils::read<QWORD>( a1 + 0xAF8 ), utils::read<DWORD>( a1 + 0xB00 ) & 0x3F ) ^ 0xB93D7A4A0A46251i64;
    }

    inline uintptr_t decrypt_animation_component( uintptr_t a1 )
    {
        __int64 result; // rax
        unsigned __int8 v2; // r8
        __int64 v3; // r9
        uintptr_t v4; // r8

        result = utils::read<QWORD>( a1 + 0x808 );
        v2 = utils::read<BYTE>( a1 + 0x811 ) & 3;
        if ( !v2 )
            v2 = 1;
        v3 = v2 - 1LL;
        if ( v3 >= 0 )
        {
            v4 = ( a1 + 8 * ( v3 + 2 * ( v3 + 0x82 ) ) );
            do
            {
                if ( ( utils::read<BYTE>( v4 - 6 ) & 3 ) != 0 )
                {
                    if ( ( utils::read<BYTE>( v4 - 6 ) & 3 ) == 1 )
                    {
                        result ^= utils::read<QWORD>( v4 ) ^ 0x5CFE84A2AFC53794LL;
                    }
                    else if ( ( utils::read<BYTE>( v4 - 6 ) & 3 ) == 2 )
                    {
                        result = __ROR8__( result, utils::read<BYTE>( v4 ) & 0x3F ) ^ 0x915C280C43BE82ALL;
                    }
                }
                else
                {
                    result = __ROR8__( result, utils::read<BYTE>( v4 ) & 0x3F ) ^ 0x518CE5462B617529LL;
                }
                v4 -= 0x18;
                --v3;
            }
            while ( v3 >= 0 );
        }

        return result;
    }

    inline uintptr_t decrypt_bone_data( uintptr_t bone_data1, uintptr_t bone_data2 ) //40 57 48 83 EC ? 48 83 7A ? ? 48 8B FA 0F 84 ? ? ? ? 48 8B CA E8
    {
        return __ROR8__( bone_data1 ^ bone_data2, 0x35 );
    }

    inline uintptr_t decrypt_bone_matrix( uintptr_t a1 )
    {
        __int64 v2; // rax
        unsigned __int8 v3; // r9
        __int64 v4; // r9
        __int64 v5; // r8

        v2 = utils::read<QWORD>( a1 + 0x50 );
        v3 = utils::read<BYTE>( a1 + 0x98 ) & 3;
        if ( !v3 )
            v3 = 1;

        v4 = v3 - 1LL;
        if ( v4 >= 0 )
        {
            v5 = a1 + 8 * ( v4 + 2 * ( v4 + 7 ) );
            do
            {
                if ( ( utils::read<BYTE>( v5 + 9 ) & 3 ) != 0 )
                {
                    if ( ( utils::read<BYTE>( v5 + 9 ) & 3 ) == 1 )
                    {
                        v2 = __ROR8__( v2 - utils::read<QWORD>( v5 ), 33 );
                    }
                    else if ( ( utils::read<BYTE>( v5 + 9 ) & 3 ) == 2 )
                    {
                        v2 = __ROR8__( __ROR8__( v2, utils::read<BYTE>( v5 ) & 0x3F ), 26 );
                    }
                }
                else
                {
                    v2 = __ROR8__( v2 - utils::read<QWORD>( v5 ), 41 );
                }
                v5 -= 24LL;
                --v4;
            }
            while ( v4 >= 0 );
        }

        return v2;
    }

    class c_indexer
    {
    public:
        c_indexer( uintptr_t addr ) : m_addr( addr ) { }

    public:
        unsigned int get_index( ) const
        {
            return utils::read<unsigned int>( m_addr + 0x50 );
        }

        bool is_ptr_name( ) const
        {
            auto flags = utils::read<unsigned int>( m_addr + 0x64 );

            return ( flags & 0x1000000 ) != 0;
        }

        std::string get_name( )
        {
            bool ptr_name = is_ptr_name( );

            std::string name = "null";

            if ( ptr_name )
            {
                auto ptr = utils::read<uintptr_t>( m_addr + 0x58 );

                name = utils::read_string( ptr );
            }
            else
                name = utils::read_string( m_addr + 0x58 );

            return name;
        }

    private:
        uintptr_t m_addr;
    };

    class c_index_module
    {
    public:
        uintptr_t m_index_module;
        uintptr_t m_index_info;

    public:
        c_indexer get_indexer( int index ) const
        {
            auto indexer_addr = utils::read<uintptr_t>( m_index_info + ( index * 0x8 ) );

            return c_indexer( indexer_addr );
        }
    };

    inline c_index_module get_index_module( ) //E8 ? ? ? ? 48 8B 3F 48 8B F0
    {
        c_index_module out_info{ };

        __int64 index_module; // rax
        unsigned __int8 v1; // dl
        __int64 v2; // r9
        uintptr_t v3; // r8


        auto qword_7FF697D11B28 = utils::read<uintptr_t>( utils::target_process.get_base( ).add( 0x7191b28 ).as<uintptr_t>( ) );
        auto qword_7FF697D11B40 = utils::read<uintptr_t>( utils::target_process.get_base( ).add( 0x7191b40 ).as<uintptr_t>( ) );
        auto qword_7FF697D11B68 = utils::read<BYTE>( utils::target_process.get_base( ).add( 0x7191b68 ).as<uintptr_t>( ) );

        index_module = qword_7FF697D11B28;
        v1 = qword_7FF697D11B68 & 3;
        if ( ( qword_7FF697D11B68 & 0x300 ) == 0 )
            v1 = 1;
        v2 = v1 - 1LL;
        if ( v2 >= 0 )
        {
            v3 = utils::read<uintptr_t>( qword_7FF697D11B40 + ( ( 3 * v2 ) * 0x8 ) );
            do
            {
                if ( ( utils::read<BYTE>( v3 - 6 ) & 3 ) != 0 )
                {
                    if ( ( utils::read<BYTE>( v3 - 6 ) & 3 ) == 1 )
                        index_module = ( utils::read<uintptr_t>( v3 ) ^ index_module ) - 0x4DD7EB8E72DC8C5DLL;
                }
                else
                {
                    index_module = __ROR8__( __ROR8__( index_module, utils::read<BYTE>( v3 ) & 0x3F ), 10 );
                }
                v3 -= 3;
                --v2;
            }
            while ( v2 >= 0 );
        }

        printf( "index_module: 0x%llx\n", index_module );

        out_info.m_index_module = index_module;
        out_info.m_index_info = 0x0;

        return out_info;
    }

    class c_attribute_manager
    {
    public:
        uintptr_t get_attribute( int index ) //E8 ? ? ? ? 84 C0 74 0E F3 0F 10 44 24 ?
        {
            if ( !m_attributes_max_offset )
                return 0;

            uintptr_t index_key = 0xFFFFFFFF | ( ( uint64_t )index << 0x20 );
            uintptr_t attribute_key_addr = m_attributes + 4 * ( 5 * m_attributes_max + ( ( 0x7F4A7C19 * ( int )index_key ) ^ ( int )( ( 0x9E3779B97F4A7C19 * index_key ) >> 0x20 ) ) % m_attributes_max_offset );

            auto attribute_key = utils::read<uint32_t>( attribute_key_addr );
            if ( attribute_key > m_attributes_count )
            {
#if defined(_DEBUG) || defined(RELEASE_EXE)
                utils::print( e_print_type::error, "attribute key is invalid!\n" );
#endif
                return 0;
            }

            while ( index_key != utils::read<uintptr_t>( m_attributes + 0x10 * attribute_key ) )
            {
                attribute_key = utils::read<uint32_t>( m_attributes + 0x10 * m_attributes_max + 4 * attribute_key );
                if ( attribute_key >= m_attributes_count )
                {
#if defined(_DEBUG) || defined(RELEASE_EXE)
                    utils::print( e_print_type::error, "attribute key not found!\n" );
#endif
                    return 0;
                }
            }

            if ( attribute_key == -1 )
            {
                utils::print( e_print_type::error, "attribute key not found!\n" );
                return 0;
            }

            auto attribute_value = ( 0x10 * ( int )attribute_key + m_attributes + 8 );
            if ( !attribute_value )
                return 0;

            return attribute_value;
        }

    public:
        uintptr_t m_attribute_manager;
        uintptr_t m_attribute_info;
        uintptr_t m_attributes;
        uint32_t m_attributes_count;
        uint32_t m_attributes_max;
        uint32_t m_attributes_max_offset;
    };

    inline c_attribute_manager get_attribute_manager( uintptr_t local_player ) //E8 ? ? ? ? 48 83 C3 08 48 3B DE 75 E7 48 8D 4D 0F
    {
        c_attribute_manager out_info{ };

        auto attribute_manager = utils::read<uintptr_t>( 0x62AB9169A7A75E6ELL );
        if ( !attribute_manager )
            return out_info;

        auto attribute_info = utils::read<uintptr_t>( attribute_manager + 0x28 );
        if ( !attribute_info )
            return out_info;

        uintptr_t attributes;
        uint32_t attributes_count;
        uint32_t attributes_max;

        utils::scatter attribute_read{ };

        attribute_read.push_back_read<uintptr_t>( attribute_info + 0x20, &attributes );
        attribute_read.push_back_read<uint32_t>( attribute_info + 0x28, &attributes_count );
        attribute_read.push_back_read<uint32_t>( attribute_info + 0x2c, &attributes_max );

        attribute_read.execute_read( );
        attribute_read.close( );

        auto attributes_max_offset = attributes_max * 2 - 1;

        if ( !attributes_count )
            return out_info;

        out_info.m_attribute_manager = attribute_manager;
        out_info.m_attribute_info = attribute_info;
        out_info.m_attributes = attributes;
        out_info.m_attributes_count = attributes_count;
        out_info.m_attributes_max = attributes_max;
        out_info.m_attributes_max_offset = attributes_max_offset;

        /*
        printf( "attribute_manager: 0x%llx\n", attribute_manager );
        printf( "attribute_info: 0x%llx\n", attribute_info );
        printf( "attributes: 0x%llx\n", attributes );
        printf( "attributes_count: %d\n", attributes_count );
        printf( "attributes_max: %d\n", attributes_max );
        printf( "attributes_max_offset: 0x%x\n", attributes_max_offset );
        */

        return out_info;
    }

    class c_view
    {
    public:
        c_view( ) : m_matrix( ), m_screen_size( ) { }
        c_view( math::matrix_4x4 matrix, math::vec2 screen_size ) : m_matrix( matrix ), m_screen_size( screen_size ) { }

    public:
        bool world_to_screen( math::vec3& world_pos, math::vec2* screen_pos ) const
        {
            math::vec3 trans = { m_matrix.m[ 3 ][ 0 ], m_matrix.m[ 3 ][ 1 ], m_matrix.m[ 3 ][ 2 ] };
            math::vec3 up = { m_matrix.m[ 1 ][ 0 ], m_matrix.m[ 1 ][ 1 ], m_matrix.m[ 1 ][ 2 ] };
            math::vec3 right = { m_matrix.m[ 0 ][ 0 ], m_matrix.m[ 0 ][ 1 ], m_matrix.m[ 0 ][ 2 ] };

            float w = trans.dot_product( world_pos ) + m_matrix.m[ 3 ][ 3 ];
            if ( w < 0.098f )
                return false;

            float x = right.dot_product( world_pos ) + m_matrix.m[ 0 ][ 3 ];
            float y = up.dot_product( world_pos ) + m_matrix.m[ 1 ][ 3 ];

            screen_pos->x = ( m_screen_size.x * 0.5f ) * ( 1.f + x / w );
            screen_pos->y = ( m_screen_size.y * 0.5f ) * ( 1.f - y / w );

            if ( screen_pos->x < 0 || screen_pos->x > m_screen_size.x || screen_pos->y < 0 || screen_pos->y > m_screen_size.y )
                return false;

            return true;
        }
    private:
        math::matrix_4x4 m_matrix;
        math::vec2 m_screen_size;
    };

    enum e_attributes
    {
        AimChangeAccuracyRecoveryTimeMSFinal,
        AimChangeModifierFinal,
        AimChangeSensitivityFinal,
        AimSpreadMaxFinal,
        AimSpreadMinFinal,
        AirSupplyFinal,
        AirSupplyDepletionRateFinal,
        AirSupplyRegenRateFinal,
        AirSupplyRegenTickIntervalFinal,
        AirSupplyDepletionTickIntervalFinal,
        AirSupplyDamageRateFinal,
        AirSupplyDamageIntervalFinal,
        AirSupplyEmptyDamageDelayFinal,
        FallDamageModFinal,
        FallDamageLethalHeightFinal,
        FallDamageStartHeightFinal,
        FallDamageOverrideModFinal,
        BleedingOutHealthFinal,
        BleedingOutHealthPvPFinal,
        BleedingOutHealthDegenRateFinal,
        BleedingOutHealthDegenRatePvPFinal,
        BleedingOutHealthDegenTickIntervalFinal,
        BleedingOutHealthDegenTickIntervalPvPFinal,
        BleedingOutDieQuickMultiplierFinal,
        BleedingOutReviveHealthPercentageFinal,
        BleedingOutReviveHealthPercentagePvPFinal,
        BleedingOutReviveArmorPercentagePvPFinal,
        ClipSizeFinal,
        ReserveAmmoClipCountFinal,
        AdditionalLootReserveAmmoClipCountFinal,
        CoverAimSpreadMaxFinal,
        CoverAimSpreadMinFinal,
        CoverSpreadMaxFinal,
        CoverSpreadMinFinal,
        CrouchSpreadMinFinal,
        CrouchSpreadMaxFinal,
        CrouchAimSpreadMinFinal,
        CrouchAimSpreadMaxFinal,
        CrouchSlideSpreadMinFinal,
        CrouchSlideSpreadMaxFinal,
        CrouchSlideAimSpreadMinFinal,
        CrouchSlideaimSpreadMaxFinal,
        InAirSpreadMinFinal,
        InAirSpreadMaxFinal,
        InAirAimSpreadMinFinal,
        InAirAimSpreadMaxFinal,
        MantleSpreadMinFinal,
        MantleSpreadMaxFinal,
        MantleAimSpreadMinFinal,
        MantleAimSpreadMaxFinal,
        CriticalHitChanceResilienceFinal,
        DamageDoneToSuppressedTargetModFinal,
        DamageDoneToElitesModFinal,
        DamageMultiplierArmsFinal,
        DamageMultiplierUpperArmsFinal,
        DamageMultiplierLowerArmsFinal,
        DamageMultiplierBodyFinal,
        DamageMultiplierUpperBodyFinal,
        DamageMultiplierMiddleBodyFinal,
        DamageMultiplierLowerBodyFinal,
        DamageMultiplierHeadFinal,
        DamageMultiplierLegsFinal,
        DamageMultiplierUpperLegsFinal,
        DamageMultiplierLowerLegsFinal,
        DamageMultiplierHandsFinal,
        DamageMultiplierFeetFinal,
        DamageTakenModFinal,
        DamageTakenFromElitesModFinal,
        DamageToPlayerArmorModFinal,
        DamageToHealthModFinal,
        DownedDamageMaxFinal,
        DownedDamageMaxPVPFinal,
        DownedDamageModFinal,
        DownedDamageModPVPFinal,
        DownedHealthFinal,
        DownedHealthPvPFinal,
        DownedHealthDegenRateFinal,
        DownedHealthDegenRatePvPFinal,
        DownedHealthDegenTickIntervalFinal,
        DownedHealthDegenTickIntervalPvPFinal,
        DownedInARowMaxFinal,
        DownedInARowMaxTimePeriodFinal,
        DownedDieQuickMultiplierFinal,
        DownedReviveHealthPercentageFinal,
        DownedReviveHealthPercentagePvPFinal,
        DownedReviveArmorPercentagePvPFinal,
        UnconsciousReviveHealthPercentageFinal,
        UnconsciousReviveArmorPercentageFinal,
        TalentDownedAssistSpeedModFinal,
        TalentSecondChanceHealModFinal,
        TalentSecondChanceCooldownFinal,
        HealthFinal,
        HealthRegenDelayFinal,
        HealthRegenRateFinal,
        ArmorCombatRegenRateFinal,
        HealthRegenTickIntervalFinal,
        HealthSegmentsFinal,
        ArmorPipCapacity,
        IncomingHealingModFinal,
        OutgoingHealingModFinal,
        DarkZoneInventoryFinal,
        DefaultInventoryFinal,
        ResourceInventoryFinal,
        SkillInventoryFinal,
        BundleInventoryFinal,
        ModInventoryFinal,
        VanityInventoryFinal,
        BackupSpaceInventoryFinal,
        StashInventoryFinal,
        DarkZoneStashInventoryFinal,
        MailboxInventoryFinal,
        ClanCacheStashInventoryFinal,
        DarkZoneInventoryFlatBonus,
        LifeOnKillFinal,
        ArmorOnKillFinal,
        LootChanceModFinal,
        LootedAmmoModFinal,
        LootedGoldModFinal,
        LootMagicFindFinal,
        ScavengingExtraDropChancesFinal,
        MeleeDamagePvPFinal,
        MeleeDamageFinal,
        MeleeDamageMitigationPercentageFinal,
        MeleeDamageArmorModifierFinal,
        MeleeDamageTakenFromBackModifierFinal,
        MeleeRangeFinal,
        MovementSpeedModFinal,
        AimingMovementSpeedModFinal,
        PulseCriticalHitChanceFinal,
        PulseCriticalHitDamageFinal,
        RPMFinal,
        BulletsPerRoundOverrideFinal,
        AdditionalBulletsPerRoundFinal,
        AdditionalRoundsInBurstFinal,
        RangeLongFinal,
        RangeMaxFinal,
        RangeOptimalFinal,
        ShortRangeFinal,
        MediumRangeFinal,
        LongRangeFinal,
        PvPRangeLongFinal,
        PvPRangeOptimalFinal,
        ReloadTimeMSFinal,
        EmptyClipFirstRoundReloadTimeMSFinal,
        StripperClipReloadTimeMSFinal,
        InitialShellReloadTimeMSFinal,
        ReloadEndTimeMSFinal,
        ReloadInterruptTimeMSFinal,
        RespawnTimeMSFinal,
        RespawnMinimumTimeFinal,
        ReviveDurationMSFinal,
        ReviveDurationMSPvPFinal,
        ReviveFromUnconsciousDurationMSFinal,
        LootDeadBodyDurationMSFinal,
        ReviveImmunityDurationFinal,
        ReviveImmunityRangeFinal,
        ReviveServerNotifyTickIntervalMSFinal,
        SafetyBeltCooldownFinal,
        SkillCriticalHitChanceFinal,
        SkillCriticalHitDamageModFinal,
        SpreadIncreaseSpeedFinal,
        SpreadIncreaseTimeMSFinal,
        SpreadMaxFinal,
        SpreadMinFinal,
        SpreadMovementModFinal,
        SpreadReductionSpeedFinal,
        StaggerDamageModifierFinal,
        StaminaConsumptionRateFinal,
        StaminaConsumptionTickIntervalFinal,
        StaminaDamageTakenModFinal,
        StaminaFinal,
        StaminaRegenDelayFinal,
        StaminaRegenRateFinal,
        StaminaRegenTickIntervalFinal,
        SuppressionFinal,
        TargetOutOfCoverDamageModFinal,
        ThrowRangeModFinal,
        TimeToMaxAccuracyMSFinal,
        TimeToMinAccuracyMSFinal,
        WeaponCriticalHitChanceFinal,
        WeaponCriticalHitChanceDistanceMinFinal,
        WeaponCriticalHitChanceDistanceMaxFinal,
        WeaponCriticalHitDamageModFinal,
        WeaponCriticalHitChanceVsOOCFinal,
        WeaponCriticalHitChanceCap,
        WeaponDrawSpeedModFinal,
        WeaponHolsterSpeedModFinal,
        XPModFinal,
        ZoneDiscoveryXPFinal,
        FastTravelDamageBlockTimeMSFinal,
        LevelRequirementForSignatureWeaponSlot,
        WeaponRPMModBonus,
        WeaponClipSizeFlatBonus,
        WeaponReserveAmmoClipCountFlatBonus,
        CameraShakeOnDamageADSModFinal,
        CameraShakeOnDamageHipFireModFinal,
        PhysicalDamageMinFinal,
        PhysicalDamageMaxFinal,
        PhysicalDamageVariationFinal,
        BlastDamageMinFinal,
        BlastDamageMaxFinal,
        BlastDamageVariationFinal,
        BleedDamageMinFinal,
        BleedDamageMaxFinal,
        BleedDamageVariationFinal,
        BurnDamageMinFinal,
        BurnDamageMaxFinal,
        BurnDamageVariationFinal,
        ToxinDamageMinFinal,
        ToxinDamageMaxFinal,
        ToxinDamageVariationFinal,
        ElectricDamageMinFinal,
        ElectricDamageMaxFinal,
        ElectricDamageVariationFinal,
        NeutralDamageMinFinal,
        NeutralDamageMaxFinal,
        NeutralDamageVariationFinal,
        EMPDamageMinFinal,
        EMPDamageMaxFinal,
        EMPDamageVariationFinal,
        StaminaDamageMinFinal,
        StaminaDamageMaxFinal,
        StaminaDamageVariationFinal,
        DiseaseDamageMinFinal,
        DiseaseDamageMaxFinal,
        DiseaseDamageVariationFinal,
        ColdDamageMinFinal,
        ColdDamageMaxFinal,
        ColdDamageVariationFinal,
        BulletPenetrationDamageModifierFinal,
        BulletPenetrationDistanceModifierFinal,
        PhysicalDamageShortRangeFinal,
        PhysicalDamageMediumRangeFinal,
        PhysicalDamageLongRangeFinal,
        TestDamageScaling,
        PrimaryOffensiveCalc,
        WeaponDamageModBonus,
        WeaponTypeDamageModCalc,
        SpecialAmmoArmorPiercingDamageModFinal,
        SpecialAmmoArmorPiercingDamageResistancePercentageFinal,
        NPCArmorFinal,
        NPCArmorBypassRatioHeadFinal,
        NPCArmorBypassRatioBodyFinal,
        NPCArmorDamageModifierFinal,
        NPCArmorBypassModifierFinal,
        PhysicalDamageMitigationPercentageFinal,
        BlastDamageMitigationPercentageFinal,
        BlastDamageFromOpponentSourceMitigationPercentageFinal,
        BleedDamageMitigationPercentageFinal,
        BurnDamageMitigationPercentageFinal,
        ToxinDamageMitigationPercentageFinal,
        ElectricDamageMitigationPercentageFinal,
        NeutralDamageMitigationPercentageFinal,
        EMPDamageMitigationPercentageFinal,
        StaminaDamageMitigationPercentageFinal,
        GuardianDroneInterceptionChanceFinal,
        GuardianDroneInterceptionChancePvPFinal,
        SelfSourceDamageMitigationPercentageFinal,
        FriendlySourceDamageMitigationPercentageFinal,
        NPCSourceDamageMitigationPercentageFinal,
        OpponentSourceDamageMitigationPercentageFinal,
        NullSourceDamageMitigationPercentageFinal,
        WeaponMethodDamageMitigationPercentageFinal,
        TrapMethodDamageMitigationPercentageFinal,
        SkillMethodDamageMitigationPercentageFinal,
        StatusEffectMethodDamageMitigationPercentageFinal,
        MeleeMethodDamageMitigationPercentageFinal,
        EnvironmentMethodDamageMitigationPercentageFinal,
        BleedingStatusResistancePercentageFinal,
        BlindDeafStatusResistancePercentageFinal,
        BurningStatusResistancePercentageFinal,
        ConfusedStatusResistancePercentageFinal,
        DisruptedStatusResistancePercentageFinal,
        ShockedStatusResistancePercentageFinal,
        StaggeredStatusResistancePercentageFinal,
        SuppressedStatusResistancePercentageFinal,
        AimSwayMinHorizontalFinal,
        AimSwayMaxHorizontalFinal,
        AimSwayMinVerticalFinal,
        AimSwayMaxVerticalFinal,
        AimSwaySpeedModifierFinal,
        VisualDetectionModFinal,
        VisualStealthModFinal,
        AudialDetectionModFinal,
        AudialStealthModFinal,
        DarkZoneConvertXPModFinal,
        DarkZoneXPModFinal,
        DarkZoneXPLossModFinal,
        DarkZoneXPLossRogueModFinal,
        DarkZoneXPLossFinal,
        DarkZoneXPLossRogueFinal,
        DarkZoneXPLossBlackoutFinal,
        DarkZoneLandmarkXPModFinal,
        DarkZoneAgentKillXPModFinal,
        DarkZoneRogueActionTimeModFinal,
        DarkZoneRogueCooldownModFinal,
        DarkZoneContaminatedLootDropChanceModFinal,
        DarkZoneAutoSecureLootChanceFinal,
        DarkZoneAutoSecureLootItemsFinal,
        DarkZoneReviveModFinal,
        DarkZoneExtractionArrivalTimeModFinal,
        DarkZoneRogueTimeModFinal,
        DarkZoneManhuntBountyModFinal,
        ThreatPerBulletFinal,
        ThreatTotalGeneratedModFinal,
        ThreatPerBulletDamageModFinal,
        SuppressionTriggerScoreFinal,
        SuppressionDefenseModifierFinal,
        SuppressionDepletionRateFinal,
        SuppressionDurationFinal,
        SuppressionBufferScoreFinal,
        SuppressionBufferResetTimeFinal,
        StaggerGaugeFinal,
        StaggerAnimationDurationModFinal,
        XPRepeatableEventsModFinal,
        CreditsRepeatableEventsModFinal,
        SecurityRepeatableEventsModFinal,
        PersonnelRescueMissionsModFinal,
        SupplyDropMissionsModFinal,
        XPHeadshotKillsModFinal,
        XPTacticalKillsModFinal,
        XPSpecialKillsModFinal,
        XPEliteKillsModFinal,
        XPMultiKillsModFinal,
        XPWeakpointKillsModFinal,
        XPKillsModFinal,
        XPAccoladeModFinal,
        XPSurvivorModFinal,
        XPTeamSurvivorModFinal,
        ExtractionTimeModFinal,
        CraftingCostModFinal,
        SkillXPModFinal,
        DZRewardedCredsModFinal,
        VendorCostModFinal,
        AmmoDroppedModFinal,
        CreditsDroppedModFinal,
        LootedCreditsModFinal,
        LootedDZCreditsModFinal,
        UnlockedSkillSlotsFinal,
        SecondaryWeaponAllowedBoolFinal,
        RewardedCredsModFinal,
        PerkAutoDiscoverFinal,
        LastBulletDamageModFinal,
        SkillPowerModBonus,
        BlastDamageModBonus,
        TalentGunslingerFinal,
        TalentGunslingerBonusIncFinal,
        TalentGunslingerBonusDecFinal,
        TalentGunslingerMaxBonusFinal,
        TalentNomadFinal,
        TalentNomadCooldownFinal,
        TalentWardenFinal,
        TalentWardenDamageFinal,
        TalentWardenBonusIncFinal,
        TalentWardenBonusDecFinal,
        TalentWardenMaxBonusFinal,
        TalentWeaponSwapReload,
        TalentSoloFinal,
        TalentRiotShieldWithSMG,
        DeconstructionExtraResourceDropFinal,
        DeconstructionExtraQuantityChanceFinal,
        DeconstructionExtraQuantityModFinal,
        AutoRefillMedkitsInSafeHouseFinal,
        AutoRefillGrenadesInSafeHouseFinal,
        MaximumLoadoutCountFinal,
        LootAutoPickupRadiusFinal,
        TalentEvasiveActiveFinal,
        TalentEvasivePercentToReloadFinal,
        TalentBoomerangActiveFinal,
        TalentBoomerangPercentChanceFinal,
        TalentBoomerangBonusDamageModFinal,
        TalentBoomerangAugmentedRealityActiveFinal,
        TalentLuckyShotActiveFinal,
        TalentLuckyShotPercentChanceFinal,
        TalentLuckyShotAugmentedRealityActiveFinal,
        TalentEverlastingActiveFinal,
        TalentEverlastingAmmoCooldownFinal,
        TalentEverlastingAmmoAmountFinal,
        TalentChatterboxActiveFinal,
        TalentChatterboxRateOfFireIncreasePerHitFinal,
        TalentChatterboxMagazineSizeIncreasePerHitFinal,
        TalentChatterboxMaxRateOfFireFinal,
        TalentChatterboxMaxMagazineSizeFinal,
        TalentChatterboxRefillPercentOnKillFinal,
        TalentChatterboxMagazineBuffDurationFinal,
        TalentChatterboxRangeFinal,
        TalentChatterboxMagazineSizeAugmentedRealityFinal,
        TalentChatterboxShowMagazineSizeAugmentedRealityFinal,
        TalentChatterboxAugmentedRealityIconActiveFinal,
        TalentGuerillaWarfareActiveFinal,
        TalentGuerillaWarfareDamagePerExplosionFinal,
        TalentGuerillaWarfareDamageModPerPrimerRoundFinal,
        TalentGuerillaWarfarePrimerRoundPhysicalDamageModFinal,
        TalentGuerillaWarfareMaxPrimerRoundCountFinal,
        TalentGuerillaWarfarePrimerStackCountFinal,
        TalentHolsteredGuerillaWarfareActiveFinal,
        TalentHolsteredGuerillaWarfarePercentChanceFinal,
        TalentHolsteredGuerillaWarfareDamageModFinal,
        TalentPlagueOfTheOutcastsActiveFinal,
        TalentPlagueOfTheOutcastsHealthDamagePercentFinal,
        TalentPlagueOfTheOutcastsDamageDurationFinal,
        TalentPlagueOfTheOutcastsDamageTickRateFinal,
        TalentPlagueOfTheOutcastsMaxStackCountFinal,
        TalentPlagueOfTheOutcastsStackCountFinal,
        TalentPlagueOfTheOutcastsShowIconFinal,
        TalentLibertyActiveFinal,
        TalentLibertyPercentDamageIncreaseAgainstSkillProxiesFinal,
        TalentIndependenceActiveFinal,
        TalentIndependencePercentMagazineRefillFinal,
        CraftingQualityFinal,
        CraftingTierRangeMinFinal,
        CraftingTierRangeMaxFinal,
        PrimaryDefensiveFinal,
        PrimaryOffensiveFinal,
        PrimarySupportFinal,
        SkillPowerFinal,
        SkillPowerChemLauncherFinal,
        SkillPowerDroneFinal,
        SkillPowerFireflyFinal,
        SkillPowerHiveFinal,
        SkillPowerPulseFinal,
        SkillPowerSeekerFinal,
        SkillPowerShieldFinal,
        SkillPowerTurretFinal,
        SkillPowerRequirement,
        OverHealFinal,
        OverHealDepletionDelayFinal,
        OverHealDepletionRateFinal,
        SignatureSkillResourceRewardModFinal,
        CombatBuff_Cooldown_Final,
        DefenceBuff_Cooldown_Final,
        GroupHeal_Cooldown_Final,
        ModSocketAttributeBonus,
        ArmorMitigationDefaultRatingFlatBonus,
        ContaminationZoneGasMaskLevelDefaultFlatBonus,
        DefaultInventoryFlatBonus,
        WeaponDamageBaseModBonus,
        WeaponDamageAssaultRifleBaseModBonus,
        WeaponDamageLMGBaseModBonus,
        WeaponDamageMarksmanRifleBaseModBonus,
        WeaponDamagePistolBaseModBonus,
        WeaponDamageShotgunBaseModBonus,
        WeaponDamageSMGBaseModBonus,
        PrimaryOffensiveFlatBonus,
        PrimaryDefensiveFlatBonus,
        PrimarySupportFlatBonus,
        PrimaryDefensiveDefaultBaseFlatBonus,
        PrimaryOffensiveDefaultBaseFlatBonus,
        PrimarySupportDefaultBaseFlatBonus,
        PrimaryDefensiveDefaultFlatBonus,
        PrimaryOffensiveDefaultFlatBonus,
        PrimarySupportDefaultFlatBonus,
        ArmorRatingUI,
        ToughnessUI,
        DefaultInventoryUI,
        ContaminationProtectionUI,
        SkillPowerUI,
        HealthMaxUI,
        WeaponDamageCompareUI,
        WeaponRPMCompareUI,
        WeaponMagazineCompareUI,
        WeaponAccuracyCompareUI,
        WeaponRangeCompareUI,
        WeaponReloadTimeCompareUI,
        WeaponStabilityCompareUI,
        MedkitStackFinal,
        PvPDamageMultiplierArmsFinal,
        PvPDamageMultiplierBodyFinal,
        PvPDamageMultiplierHeadFinal,
        PvPDamageMultiplierLegsFinal,
        GrenadeMaxCookTimeMS,
        GrenadeEMPStackFinal,
        GrenadeFlashbangStackFinal,
        GrenadeHEStackFinal,
        GrenadeIncendiaryStackFinal,
        GrenadeShockStackFinal,
        GrenadeTearGasStackFinal,
        StaggerMediumTierModFinal,
        StaggerHeavyTierModFinal,
        StaggerMeleeDamageModFinal,
        CombatSprintMovementSpeedModFinal,
        CoverToCoverMovementSpeedModFinal,
        SpecialMovementSpeedModFinal,
        CombatRollCooldownMSFinal,
        MeleeAnimationSpeedFinal,
        PlayerArmorFinal,
        PlayerArmorOverflowFinal,
        BulletPenetrationFinal,
        SkillScalingFinal,
        ThrowDistanceModifierFinal,
        CoopBuff,
        WeaponZoomInDurationFinal,
        LookSpeedModFinal,
        WeaponSwitchSpeedModFinal,
        WeaponRecoilPatternModFinal,
        WeaponRecoilPatternHorizontalModFinal,
        WeaponRecoilPatternVerticalModFinal,
        SprintShootDelayMSFinal,
        WeaponHolsterTimeFinal,
        WeaponHolsterTimeToSidearmFinal,
        WeaponHolsterTimeToSkillFinal,
        WeaponDrawTimeFinal,
        WeaponRecoilRecoveryTimeModModFinal,
        MatchEndRankPointsFinal,
        MatchEndMaxWinnableRankPointsFinal,
        MatchEndMaxLosableRankPointsFinal,
        LossForgivenessUnevenTeamsRatioFinal,
        LossForgivenessUnevenTeamsTimeFinal,
        LossForgivenessJoinedMidGameTimeFinal,
        RankedMatchQuitPenaltyFinal,
        CanBePinged,
        IsRevealedWhenWeaponFired
    };
}