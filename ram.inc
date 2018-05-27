	ORG $0000

_pads:
		.BYTE	0,0,0
_cur_lvl_idx:
		.BYTE	0
_unused4:
		.BYTE	0,0
_irq_regs_backup:
		.BYTE	0,0,0
_nmi_is_over_flag:
		.BYTE	0
_nmi_hndl_idx:
		.BYTE	0
_unused_33:
		.BYTE	0
_lvl_obj_list_ptr:
		.BYTE	0,0
_tmp_ptr:
		.BYTE	0,0
_ppu_ctrl_shadow:
		.BYTE	0
_ppu_mask_shadow:
		.BYTE	0
_lvl_scroll_value:
		.BYTE	0
_lvl_scroll_tile_hit_flag:
		.BYTE	0
_lvl_nt_value:
		.BYTE	0
_player_wnd_scroll_counter:
		.BYTE	0
_player_wnd_tile_number:
		.BYTE	0
_player_sit_jump_flag:
		.BYTE	0
_hud_update_ppu_data_counter:
		.BYTE	0
_hud_msg_type_pos:
		.BYTE	0
_pal_update_redundant:
		.BYTE	0
_cur_lvl_state_idx:
		.BYTE	0
_weapon0_idx:
		.BYTE	0
_weapon1_idx:
		.BYTE	0
_tmp1E:
		.BYTE	0,0
_logo_skip_forbid_flag:
		.BYTE	0
_unused_34:
		.BYTE	0
_obj_manager_tmp_cnt:
		.BYTE	0
_obj_scr_x_pos_tmp:
		.BYTE	0
_obj_scr_y_pos_tmp:
		.BYTE	0
_obj_spr_data_ptr_tmp:
		.BYTE	0,0
_unused_35:
		.BYTE	0
_obj_offscreen_flag_tmp:
		.BYTE	0
_lvl_cur_spr_chr_bank_up:
		.BYTE	0
_lvl_cur_spr_chr_bank_down:
		.BYTE	0
_lvl_cur_bg_chr_bank:
		.BYTE	0
_lvl_phase_bg_chr_banks_list:
		.BYTE	0,0,0
_lvl_bg_chr_treshold:
		.BYTE	0,0
_obj_global_y_delta:
		.BYTE	0
_lvl_stop_sctoll_room_idx:
		.BYTE	0
_pause_flag:
		.BYTE	0
_player_y_meta16_tile_tmp:
		.BYTE	0
_player_explode_spr_remove_reload:
		.BYTE	0
_player_scroll_stop_override_tmp:
		.BYTE	0
_rand_seed:
		.BYTE	0,0,0,0
_player_damage_blink_timer:
		.BYTE	0
_player_explode_timer_tmp:
		.BYTE	0
_obj_shuffle_mode:
		.BYTE	0
_player_fire_delay_reload:
		.BYTE	0
_player_explode_spr_remove_cnt:
		.BYTE	0
_player_jump_or_fall_flag:
		.BYTE	0
_lvl_door_enrty_delay:
		.BYTE	0
_ingame_sync_counter:
		.BYTE	0
_lvl1_player_pal_acid_lockup_redundant:
		.BYTE	0
_pad_tmp:
		.BYTE	0
_unused_8:
		.BYTE	0
_hud_msg_idx:
		.BYTE	0
_snd_idx:
		.BYTE	0
_last_enemy_hit:
		.BYTE	0
_player_diagonal_fire_delta:
		.BYTE	0
_player_hor_move_anim_timer:
		.BYTE	0
_enemy_bullet_y_pos_global_arg:
		.BYTE	0
_player_cur_bullet_shot_state:
		.BYTE	0
_unused_36:
		.BYTE	0,0,0

		TOBJ	_obj_flags
		TOBJ	_obj_spr_idx
		TOBJ	_obj_pal_idx
		TOBJ	_obj_lvl_x_lo
		TOBJ	_obj_lvl_x_hi
		TOBJ	_obj_lvl_y
		TOBJ	_obj_hcoll
		TOBJ	_obj_vcoll
		TOBJ	_obj_hndl_idx
		TOBJ	_obj_unk

_unused?:
		.BYTE	0,0,0,0,0,0,0,0
_lvl_room_data_ptr:
		.BYTE	0,0
_lvl_collision_data0_ptr:
		.BYTE	0,0
_lvl_collision_data1_ptr:
		.BYTE	0,0
_lvl_collision_data2_ptr:
		.BYTE	0,0
_tmpE2:
		.BYTE	0,0
_tmpE4:
		.BYTE	0
_tmpE5:
		.BYTE	0
_tmpE6:
		.BYTE	0
_tmpE7:
		.BYTE	0
_tmpE8:
		.BYTE	0
_tmpE9:
		.BYTE	0
_ptr0:
		.BYTE	0,0
_ptr1:
		.BYTE	0,0
_ptr2:
		.BYTE	0,0
_unused_37:
		.BYTE	0,0,0,0
_text_data_tmp_ptr:
		.BYTE	0,0
_intro_txt_ppu_ofs_tmp:
		.BYTE	0,0
_unused_38:
		.BYTE	0,0,0,0,0,0
_apu_tmp0:
		.BYTE	0,0
_unused_39:
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0
_unused1B3:
		.BYTE	0
_unused_40:
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0
_spr_buf:
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
_pal_buf:
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
_nmi_ppu_ctrl_override_redundant:
		.BYTE	0
_ppu_scroll_x_override_redundant:
		.BYTE	0
_nmi0_scroll_reload_counter_redundant:
		.BYTE	0
_star_field_rand_seed:
		.BYTE	0,0,0,0,0
_star_field_data0:
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0
_star_field_data1:
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0
_star_field_data2:
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0
_star_field_center_pos_x:
		.BYTE	0
_star_field_center_pos_y:
		.BYTE	0
_event_idx:
		.BYTE	0
_unused_41:
		.BYTE	0,0
_delay_counter:
		.BYTE	0,0
_screen_idx:
		.BYTE	0
_chr0_bank_shadow:
		.BYTE	0
_chr1_bank_shadow:
		.BYTE	0
_disp_spr0hit_use:
		.BYTE	0
_mul_result_low:
		.BYTE	0
_mul_argA:
		.BYTE	0
_mul_argB:
		.BYTE	0
_star_filed_sign_tmp:
		.BYTE	0
_generic_nmi_over_counter:
		.BYTE	0
_pad:
		.BYTE	0
byte_3B1:
		.BYTE	0
byte_3B2:
		.BYTE	0
_event_reload_idx:
		.BYTE	0
byte_3B4:
		.BYTE	0
byte_3B5:
		.BYTE	0
byte_3B6:
		.BYTE	0
		.BYTE	0
byte_3B8:
		.BYTE	0
		.BYTE	0
word_3BA:
		.BYTE	0,0
_repair_cur_body_index:
		.BYTE	0
_repair_body_index_request:
		.BYTE	0
_dpad:
		.BYTE	0
byte_3BF:
		.BYTE	0
		.BYTE	0
byte_3C1:
		.BYTE	0
byte_3C2:
		.BYTE	0
_tmp3C3:
		.BYTE	0,0,0
_tmp_score_hex_value:
		.BYTE	0,0
byte_3C8:
		.BYTE	0
		.BYTE	0
byte_3CA:
		.BYTE	0
byte_3CB:
		.BYTE	0
byte_3CC:
		.BYTE	0
		.BYTE	0
byte_3CE:
		.BYTE	0
byte_3CF:
		.BYTE	0
_disp_extra_ctrl:
		.BYTE	0
_disp_extra_shift:
		.BYTE	0
_title_menu_idx:
		.BYTE	0
byte_3D3:
		.BYTE	0
byte_3D4:
		.BYTE	0
byte_3D5:
		.BYTE	0
byte_3D6:
		.BYTE	0
byte_3D7:
		.BYTE	0
_dec_score_chr_ofs_delta:
		.BYTE	0
_asci_msg_skip_forbid:
		.BYTE	0
_cur_head_percent_backup:
		.BYTE	0
_cur_arms_percent_backup:
		.BYTE	0
_cur_body_percent_backup:
		.BYTE	0
_cur_legs_percent_backup:
		.BYTE	0
_cur_repair_tanks_backup:
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
_tmp_ptr0:
		.BYTE	0,0
_tmp_buf0:
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0
_tmp_buf1:
		.BYTE	0,0,0,0,0,0
_tmp_buf2:
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0
_lvl_rooms_walked_count_unused:
		.BYTE	0
		.BYTE	0
_dec_word_1E_chr_ofs_delta:
		.BYTE	0
_cur_score_hex:
		.BYTE	0,0
_cur_overal_armor_sum:
		.BYTE	0
		.BYTE	0
_lvl3_jetpack_fuel_tanks:
		.BYTE	0
_lvl3_jetpack_fuel_count:
		.BYTE	0
_collision_test_idx:
		.BYTE	0
		.BYTE	0,0
_pal_blink_flag:
		.BYTE	0
_lvl2_smart_bomb_flag:
		.BYTE	0
_player_fire_delay_timer:
		.BYTE	0
_smart_bomb_timer:
		.BYTE	0
_player_tile_x:
		.BYTE	0,0
_hud_msg_cur_blink_timer:
		.BYTE	0
_lvl_cur_spr_chr_bank_up_backup:
		.BYTE	0
_lvl_cur_spr_chr_bank_down_backup:
		.BYTE	0
		.BYTE	0,0,0
_hud_random_msg_timeout:
		.BYTE	0,0
		.BYTE	0
_player_hor_move_anim_increment:
		.BYTE	0
		.BYTE	0
byte_656:
		.BYTE	0
byte_657:
		.BYTE	0
		.BYTE	0
_weapon_type:
		.BYTE	0
byte_65A:
		.BYTE	0
_player_to_enemy_overlap_flag:
		.BYTE	0
		.BYTE	0
_ppu_mask_shadow_mask:
		.BYTE	0
_weapon0_ammo:
		.BYTE	0,0,0
_weapon1_ammo:
		.BYTE	0
		.BYTE	0
byte_663:
		.BYTE	0
_player_hit_bonus_portal_flag:
		.BYTE	0
_ingame_bg_pal_buf:
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0
_enemy_bullet_body_target_select_arg:
		.BYTE	0
		.BYTE	0,0,0
_apu_load_tmp_Y:
		.BYTE	0
		.BYTE	0,0,0
_game_over_flag:
		.BYTE	0
_end_of_the_level_flag:
		.BYTE	0
_end_of_the_level_delay:
		.BYTE	0
		.BYTE	0
byte_682:
		.BYTE	0
_lvl4_destruction_counter:
		.BYTE	0
byte_684:
		.BYTE	0
byte_685:
		.BYTE	0
byte_686:
		.BYTE	0
_ingame_spr_pal_buf:
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
byte_697:
		.BYTE	0
byte_698:
		.BYTE	0
_body_target_idx:
		.BYTE	0
byte_69A:
		.BYTE	0
byte_69B:
		.BYTE	0
byte_69C:
		.BYTE	0
byte_69D:
		.BYTE	0
_lvl3_tank_weapon_disable_flag:
		.BYTE	0
		.BYTE	0
_cur_head_percent:
		.BYTE	0
_cur_body_percent:
		.BYTE	0
_cur_arms_percent:
		.BYTE	0
_cur_legs_percent:
		.BYTE	0
_cur_repair_tanks:
		.BYTE	0
_new_game_start_flag:
		.BYTE	0
_unused_32:
		.BYTE	0
_apu_sound_enable:
		.BYTE	0
_apu_music_enable:
		.BYTE	0
_player_number:
		.BYTE	0
_unused_31:
		.BYTE	0,0,0,0,0,0
_failure_treshold_list:
		.BYTE	0,0,0,0
_failure_duration_period:
		.BYTE	0
_failure_mode:
		.BYTE	0
_unused_30:
		.BYTE	0,0,0,0,0,0,0,0,0,0
_pl_bckp_head_percent:
		.BYTE	0,0
_pl_bckp_body_percent:
		.BYTE	0,0
_pl_bckp_arms_percent:
		.BYTE	0,0
_pl_bckp_legs_percent:
		.BYTE	0,0
_pl_bckp_repair_tanks:
		.BYTE	0,0
_pl_bckp_lvl_idx:
		.BYTE	0,0
_unused_29:
		.BYTE	0,0,0,0,0,0
_pl_bckp_score_hex_lo:
		.BYTE	0,0
_pl_bckp_score_hex_hi:
		.BYTE	0,0
_pl_bkcp_continues:
		.BYTE	0,0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
_hud_weap0_attr_overlay:
		.BYTE	0,0
_hud_weap1_attr_overlay:
		.BYTE	0,0
_hud_weapon_blink_timer:
		.BYTE	0
byte_6F5:
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
		.BYTE	0
_log_msg_buf:
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
_hi0_score:
		.BYTE	0,0,0,0,0
_hi0_name:
		.BYTE	0,0,0,0,0,0,0,0,0,0
_hi1_score:
		.BYTE	0,0,0,0,0
_hi1_name:
		.BYTE	0,0,0,0,0,0,0,0,0,0
_hi2_score:
		.BYTE	0,0,0,0,0
_hi2_name:
		.BYTE	0,0,0,0,0,0,0,0,0,0
_hi3_score:
		.BYTE	0,0,0,0,0
_hi3_name:
		.BYTE	0,0,0,0,0,0,0,0,0,0
_hi4_score:
		.BYTE	0,0,0,0,0
_hi4_name:
		.BYTE	0,0,0,0,0,0,0,0,0,0
_hi5_score:
		.BYTE	0,0,0,0,0
_hi5_name:
		.BYTE	0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0
_score_cmp_buf:
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
_hi_score_line_idx:
		.BYTE	0
_hi_score_pos_count:
		.BYTE	0
_hi_dec_tmp_buf:
		.BYTE	0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0

	ORG	$1400
byte_1400:
		.BYTE	0,0,0,0,0
byte_1405:
		.BYTE	0,0,0,0,0
byte_140A:
		.BYTE	0,0,0,0,0
byte_140F:
		.BYTE	0,0,0,0,0
byte_1414:
		.BYTE	0,0,0,0,0
byte_1419:
		.BYTE	0,0,0,0,0
byte_141E:
		.BYTE	0,0,0,0,0
byte_1423:
		.BYTE	0,0,0,0,0
byte_1428:
		.BYTE	0,0,0,0,0
byte_142D:
		.BYTE	0,0,0,0,0
byte_1432:
		.BYTE	0,0,0,0,0
byte_1437:
		.BYTE	0,0,0,0,0
byte_143C:
		.BYTE	0,0,0,0,0
		.BYTE	0,0,0,0,0
byte_1446:
		.BYTE	0,0,0,0,0
byte_144B:
		.BYTE	0,0,0,0,0
byte_1450:
		.BYTE	0,0,0,0,0
byte_1455:
		.BYTE	0,0,0,0,0
byte_145A:
		.BYTE	0,0,0,0,0
byte_145F:
		.BYTE	0,0,0,0,0
byte_1464:
		.BYTE	0,0,0,0,0
byte_1469:
		.BYTE	0,0,0,0,0
byte_146E:
		.BYTE	0,0,0,0,0
byte_1473:
		.BYTE	0,0,0,0,0
		.BYTE	0,0,0,0,0
byte_147D:
		.BYTE	0,0,0,0,0
byte_1482:
		.BYTE	0,0,0,0,0
byte_1487:
		.BYTE	0,0,0,0,0
byte_148C:
		.BYTE	0,0,0,0,0
byte_1491:
		.BYTE	0,0,0,0,0
byte_1496:
		.BYTE	0,0,0,0,0
byte_149B:
		.BYTE	0,0,0,0,0
byte_14A0:
		.BYTE	0,0,0,0,0
byte_14A5:
		.BYTE	0,0,0,0,0
byte_14AA:
		.BYTE	0,0,0,0,0
byte_14AF:
		.BYTE	0,0,0,0,0
byte_14B4:
		.BYTE	0,0,0,0,0
		.BYTE	0,0,0,0,0
		.BYTE	0,0,0,0,0
byte_14C3:
		.BYTE	0,0,0,0,0
byte_14C8:
		.BYTE	0,0,0,0,0

	ORG $2000

_PPU_CTRL:
		.BYTE 0
_PPU_MASK:
		.BYTE 0
_PPU_STATUS:
		.BYTE 0
_PPU_OAM_ADDR:
		.BYTE 0
_PPU_OAM_DATA:
		.BYTE 0
_PPU_SCROLL:
		.BYTE 0
_PPU_ADDR:
		.BYTE 0
_PPU_DATA:
		.BYTE 0

	ORG $4000

_APU_PULSE1_ENV:
		.BYTE 0
_APU_PULSE1_SWEEP:
		.BYTE	0
_APU_PULSE1_TIME:
		.BYTE 0
_APU_PULSE1_LEN:
		.BYTE 0
_APU_PULSE2_ENV:
		.BYTE 0
_APU_PULSE2_SWEEP:
		.BYTE	0
_APU_PULSE2_TIME:
		.BYTE 0
_APU_PULSE2_LEN:
		.BYTE 0
_APU_TRI_COUNT:
		.BYTE 0
_APU_TRI_UNUSED:
		.BYTE 0
_APU_TRI_TIME:
		.BYTE 0
_APU_TRI_LEN:
		.BYTE 0
_APU_NOISE_ENV:
		.BYTE 0
_APU_NOISE_UNUSED:
		.BYTE 0
_APU_NOISE_LOOP:
		.BYTE 0
_APU_NOISE_LEN:
		.BYTE 0
_APU_DMC_FREQ:
		.BYTE 0
_APU_DMC_LOAD:
		.BYTE 0
_APU_DMC_ADDR:
		.BYTE 0
_APU_DMC_LEN:
		.BYTE 0
_PPU_OAM_DMA:
		.BYTE 0
_APU_STATUS:
		.BYTE 0
_PAD0_REG:
		.BYTE 0
_PAD1_REG:
		.BYTE 0
