#pragma once

#include <cstdint>

namespace auth
{
	enum e_heart_beat_status : int32_t
	{
		HEART_BEAT_INVALID_TOKEN = 0,
		HEART_BEAT_INVALID_SUBSCRIPTION = 1,
		HEART_BEAT_INVALID_GAME = 2,
		HEART_BEAT_OK = 3,
	};

	struct f_ffi_vec
	{
	public:
		uint8_t* data;
		size_t len;
	};

	struct f_ffi_heart_beat_response
	{
	public:
		int32_t status;
		int64_t interval;
		int64_t timestamp;
		f_ffi_vec challenge_data;
		int64_t challenge_type;
	};

	struct f_ffi_heart_beat_request
	{
	public:
		const char* token;
		const char* game;
		const char* version;
	};

	inline f_ffi_heart_beat_request heart_beat_request{};
	inline bool heart_beat_thread_check = true;

	extern "C" f_ffi_heart_beat_response send_heartbeat( f_ffi_heart_beat_request request );
}