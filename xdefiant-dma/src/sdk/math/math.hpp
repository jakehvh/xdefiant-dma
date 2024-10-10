#pragma once
#include <cmath>
#include <intrin.h>
#define PI			3.14159265358979323846
#define DEG2RAD(x) ((float)(x) * (float)((float)(PI) / 180.0f))
#define RAD2DEG(x) ((float)(x) * (float)(180.0f / (float)(PI)))

namespace math
{
	class vec2
	{
	public:
		float x, y;

		vec2( float _x = 0.0, float _y = 0.0 ) : x{ _x }, y{ _y } { };

		vec2 operator-( const vec2& sub ) const
		{
			return vec2( x - sub.x, y - sub.y );
		}

		vec2 operator-( const float& sub ) const
		{
			return vec2( x - sub, y - sub );
		}

		vec2 operator+( const vec2& add ) const
		{
			return vec2( x + add.x, y + add.y );
		}

		vec2 operator+( const float& add ) const
		{
			return vec2( x + add, y + add );
		}

		vec2 operator*( const float& mult ) const
		{
			return vec2( x * mult, y * mult );
		}

		vec2 operator*( const vec2& mult ) const
		{
			return vec2( x * mult.x, y * mult.y );
		}

		vec2 operator/( const float& base ) const
		{
			return vec2( x / base, y / base );
		}

		vec2 operator/( const vec2& base ) const
		{
			return vec2( x / base.x, y / base.y );
		}

		bool operator==( const vec2& other ) const
		{
			return x == other.x && y == other.y;
		}

		bool operator!=( const vec2& other ) const
		{
			return x != other.x || y != other.y;
		}

		constexpr vec2& operator-=( const vec2& sub )
		{
			x -= sub.x;
			y -= sub.y;
			return *this;
		}

		constexpr vec2& operator+=( const vec2& add )
		{
			x += add.x;
			y += add.y;
			return *this;
		}
		
		constexpr vec2& operator*=( const float& mult )
		{
			x *= mult;
			y *= mult;
			return *this;
		}

		constexpr vec2& operator/=( const vec2& base )
		{
			x /= base.x;
			y /= base.y;
			return *this;
		}

		constexpr vec2& operator/=( const float& base )
		{
			x /= base;
			y /= base;
			return *this;
		}

		void clamp( )
		{
			x = static_cast< int >( x );
			y = static_cast< int >( y );
		}
	};

	class vec3
	{
	public:
		float x, y, z;

		constexpr vec3( float _x = 0, float _y = 0, float _z = 0 ) : x( _x ), y( _y ), z( _z ) { };
		constexpr vec3( float* _fp ) : x( _fp[ 0 ] ), y( _fp[ 1 ] ), z( _fp[ 2 ] ) { };

		[[nodiscard]] float* data( )
		{
			return reinterpret_cast< float* >( this );
		}

		[[nodiscard]] void clamp( )
		{
			this->x = static_cast< int >( this->x );
			this->y = static_cast< int >( this->y );
			this->z = static_cast< int >( this->z );
		}

		[[nodiscard]] constexpr float dot_product( const vec3& dot ) const
		{
			return ( this->x * dot.x + this->y * dot.y + this->z * dot.z );
		}

		[[nodiscard]] constexpr float length_sqr( ) const
		{
			return dot_product( *this );
		}

		[[nodiscard]] float length( ) const
		{
			return std::sqrt( this->length_sqr( ) );
		}

		[[nodiscard]] constexpr float length_2d_sqr( ) const
		{
			return ( this->x * this->x + this->y * this->y );
		}

		[[nodiscard]] float length_2d( ) const
		{
			return std::sqrt( this->length_2d_sqr( ) );
		}

		[[nodiscard]] float dist_to( const vec3& end ) const
		{
			return ( *this - end ).length( );
		}

		[[nodiscard]] constexpr float dist_to_sqr( const vec3& end ) const
		{
			return ( *this - end ).length_sqr( );
		}

		bool is_zero( ) const
		{
			return this->x == 0.0 && this->y == 0.0 && this->z == 0.0;
		}

		math::vec3 unit( ) const
		{
			float mult = ( 1.f / length( ) );
			return math::vec3 { x * mult, y * mult, z * mult };
		}

		constexpr vec3 cross_product( const vec3& cross ) const
		{
			return vec3( this->y * cross.z - this->z * cross.y, this->z * cross.x - this->x * cross.z, this->x * cross.y - this->y * cross.x );
		}

		float& operator[]( const size_t i )
		{
			return this->data( )[ i ];
		}

		constexpr vec3& operator=( const vec3& base )
		{
			this->x = base.x;
			this->y = base.y;
			this->z = base.z;
			return *this;
		}

		constexpr vec3& operator+=( const vec3& base )
		{
			this->x += base.x;
			this->y += base.y;
			this->z += base.z;
			return *this;
		}

		constexpr vec3& operator-=( const vec3& base )
		{
			this->x -= base.x;
			this->y -= base.y;
			this->z -= base.z;
			return *this;
		}

		constexpr vec3& operator*=( const vec3& base )
		{
			this->x *= base.x;
			this->y *= base.y;
			this->z *= base.z;
			return *this;
		}

		constexpr vec3& operator/=( const vec3& base )
		{
			this->x /= base.x;
			this->y /= base.y;
			this->z /= base.z;
			return *this;
		}

		constexpr vec3& operator+=( const float add )
		{
			this->x += add;
			this->y += add;
			this->z += add;
			return *this;
		}

		constexpr vec3& operator-=( const float sub )
		{
			this->x -= sub;
			this->y -= sub;
			this->z -= sub;
			return *this;
		}

		constexpr vec3& operator*=( const float mult )
		{
			this->x *= mult;
			this->y *= mult;
			this->z *= mult;
			return *this;
		}

		constexpr vec3& operator/=( const float div )
		{
			this->x /= div;
			this->y /= div;
			this->z /= div;
			return *this;
		}

		vec3 operator+( const vec3& add ) const
		{
			return vec3( this->x + add.x, this->y + add.y, this->z + add.z );
		}

		vec3 operator-( const vec3& sub ) const
		{
			return vec3( this->x - sub.x, this->y - sub.y, this->z - sub.z );
		}

		vec3 operator*( const vec3& mult ) const
		{
			return vec3( this->x * mult.x, this->y * mult.y, this->z * mult.z );
		}

		vec3 operator/( const vec3& div ) const
		{
			return vec3( this->x / div.x, this->y / div.y, this->z / div.z );
		}

		vec3 operator+( const float add ) const
		{
			return vec3( this->x + add, this->y + add, this->z + add );
		}

		vec3 operator-( const float sub ) const
		{
			return vec3( this->x - sub, this->y - sub, this->z - sub );
		}

		vec3 operator*( const float mult ) const
		{
			return vec3( this->x * mult, this->y * mult, this->z * mult );
		}

		vec3 operator/( const float div ) const
		{
			return vec3( this->x / div, this->y / div, this->z / div );
		}

		float operator|( const vec3& v ) const
		{
			return x * v.x + y * v.y + z * v.z;
		}
	};

	class box
	{
	public:
		float x, y, w, h;
	};

	struct matrix_4x4
	{
		union
		{
			struct
			{
				float m[ 4 ][ 4 ];
			};
			struct
			{
				float mm[ 16 ];
			};
			struct
			{
				__m128 m128[ 4 ];
			};
		};

		matrix_4x4( )
		{
			for ( int i = 0; i < 16; i++ )
				mm[ i ] = 0.f;
		}

		matrix_4x4( const matrix_4x4& ref )
		{
			for ( int i = 0; i < 16; i++ )
				mm[ i ] = ref.mm[ i ];
		}

		matrix_4x4 transpose( )
		{
			matrix_4x4 out;

			auto t0 = _mm_unpacklo_ps( this->m128[ 0 ], this->m128[ 1 ] );
			auto t1 = _mm_unpacklo_ps( this->m128[ 2 ], this->m128[ 3 ] );
			auto t2 = _mm_unpackhi_ps( this->m128[ 0 ], this->m128[ 1 ] );
			auto t3 = _mm_unpackhi_ps( this->m128[ 2 ], this->m128[ 3 ] );

			out.m128[ 0 ] = _mm_movelh_ps( t0, t1 );
			out.m128[ 1 ] = _mm_movehl_ps( t1, t0 );
			out.m128[ 2 ] = _mm_movelh_ps( t2, t3 );
			out.m128[ 3 ] = _mm_movehl_ps( t3, t2 );

			return out;
		}
	};
}