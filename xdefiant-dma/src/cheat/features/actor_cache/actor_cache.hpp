#pragma once

#include "../../../sdk/sdk.hpp"

namespace actor_cache
{
	struct f_agent
	{
		uintptr_t addr;
		uintptr_t animation_1;
		uintptr_t animation_2;
		uintptr_t animation_component;
		uintptr_t health_component;
		uintptr_t team_component;
		uintptr_t weapon_component;
		uintptr_t bone_data_1;
		uintptr_t bone_data_2;
		uintptr_t bone_data;
		uintptr_t bone_matrix_1;
		uintptr_t bone_matrix_2;
		uintptr_t bone_matrix;
		uintptr_t current_weapon;
		uintptr_t skill_script_vars;
		math::vec3 location;
		math::matrix_4x4 head_matrix;
		math::matrix_4x4 neck_matrix;
		math::matrix_4x4 upper_spine_matrix;
		math::matrix_4x4 lower_spine_matrix;
		math::matrix_4x4 pelvis_matrix;
		math::matrix_4x4 left_shoulder_matrix;
		math::matrix_4x4 left_elbow_matrix;
		math::matrix_4x4 left_hand_matrix;
		math::matrix_4x4 left_thigh_matrix;
		math::matrix_4x4 left_knee_matrix;
		math::matrix_4x4 left_calf_matrix;
		math::matrix_4x4 left_foot_matrix;
		math::matrix_4x4 right_shoulder_matrix;
		math::matrix_4x4 right_elbow_matrix;
		math::matrix_4x4 right_hand_matrix;
		math::matrix_4x4 right_thigh_matrix;
		math::matrix_4x4 right_knee_matrix;
		math::matrix_4x4 right_calf_matrix;
		math::matrix_4x4 right_foot_matrix;
		math::vec3 head_pos;
		math::vec3 neck_pos;
		math::vec3 upper_spine_pos;
		math::vec3 lower_spine_pos;
		math::vec3 pelvis_pos;
		math::vec3 left_shoulder_pos;
		math::vec3 left_elbow_pos;
		math::vec3 left_hand_pos;
		math::vec3 left_thigh_pos;
		math::vec3 left_knee_pos;
		math::vec3 left_calf_pos;
		math::vec3 left_foot_pos;
		math::vec3 right_shoulder_pos;
		math::vec3 right_elbow_pos;
		math::vec3 right_hand_pos;
		math::vec3 right_thigh_pos;
		math::vec3 right_knee_pos;
		math::vec3 right_calf_pos;
		math::vec3 right_foot_pos;
		char player_name[ 0x400 ];
		int current_health;
		int max_health;
		int team_id;
		int level;
		BYTE is_sprinting_value;
		BYTE is_crouched_value;
		BYTE is_in_riot_shield_value;
		bool is_sprinting;
		bool is_crouched;
		bool is_in_riot_shield;
	};

	struct f_gadget
	{
		uintptr_t addr;
		std::string name;
		uintptr_t team_component;
		int team_id;
		math::vec3 location;
	};

	inline int actor_count = 0;
	inline bool debug_actors = false;

	inline std::vector<f_agent> agents;
	inline std::vector<f_gadget> gadgets;

	bool update( );
}