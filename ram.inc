	ORG $0000

_apu_tmp0:							.BYTE	0,0
_cur_lvl_idx:						.BYTE	0
_cur_lvl_state_idx:					.BYTE	0
_enemy_bullet_y_pos_global_arg:		.BYTE	0
_hud_msg_idx:						.BYTE	0
_hud_msg_type_pos:					.BYTE	0
_hud_update_ppu_data_counter:		.BYTE	0
_ingame_sync_counter:				.BYTE	0
_printf_ofs_tmp:					.WORD	0
_printf_len_tmp:					.BYTE	0
_irq_regs_backup:					.BYTE	0,0,0
_last_enemy_hit:					.BYTE	0
_logo_skip_forbid_flag:				.BYTE	0
_lvl1_player_pal_acid_lockup:		.BYTE	0
_lvl_bg_chr_treshold:				.BYTE	0,0
_lvl_collision_data0_ptr:			.BYTE	0,0
_lvl_collision_data1_ptr:			.BYTE	0,0
_lvl_collision_data2_ptr:			.BYTE	0,0
_lvl_cur_bg_chr_bank:				.BYTE	0
_lvl_cur_spr_chr_bank_down:			.BYTE	0
_lvl_cur_spr_chr_bank_up:			.BYTE	0
_lvl_bonus_portal_delay:			.BYTE	0
_lvl_nt_value:						.BYTE	0
_lvl_obj_list_ptr:					.BYTE	0,0
_lvl_phase_bg_chr_banks_list:		.BYTE	0,0,0
_lvl_room_data_ptr:					.BYTE	0,0
_lvl_scroll_tile_hit_flag:			.BYTE	0
_lvl_scroll_value:					.BYTE	0
_lvl_stop_sctoll_room_idx:			.BYTE	0
_nmi_hndl_idx:						.BYTE	0
_nmi_is_over_flag:					.BYTE	0
_obj_global_y_delta:				.BYTE	0
_obj_manager_tmp_cnt:				.BYTE	0
_obj_offscreen_flag_tmp:			.BYTE	0
_obj_scr_x_pos_tmp:					.BYTE	0
_obj_scr_y_pos_tmp:					.BYTE	0
_obj_shuffle_mode:					.BYTE	0
_obj_spr_data_ptr_tmp:				.BYTE	0,0
_pad_tmp:							.BYTE	0
_pads:								.BYTE	0,0,0
_pal_update_redundant:				.BYTE	0
_pause_flag:						.BYTE	0
_player_cur_bullet_shot_state:		.BYTE	0
_player_damage_blink_timer:			.BYTE	0
_player_diagonal_fire_delta:		.BYTE	0
_player_explode_spr_remove_cnt:		.BYTE	0
_player_explode_spr_remove_reload:	.BYTE	0
_player_explode_timer_tmp:			.BYTE	0
_player_fire_delay_reload:			.BYTE	0
_player_hor_move_anim_timer:		.BYTE	0
_player_jump_or_fall_flag:			.BYTE	0
_player_scroll_stop_override_tmp:	.BYTE	0
_player_sit_jump_flag:				.BYTE	0
_player_wnd_scroll_counter:			.BYTE	0
_player_wnd_tile_number:			.BYTE	0
_player_y_meta16_tile_tmp:			.BYTE	0
_ppu_ctrl_shadow:					.BYTE	0
_ppu_mask_shadow:					.BYTE	0
_ptr0:								.BYTE	0,0
_ptr1:								.BYTE	0,0
_rand_seed:							.BYTE	0,0,0,0
_snd_idx:							.BYTE	0
_str_ptr:							.BYTE	0,0
_tmpE4:								.BYTE	0
_tmpE5:								.BYTE	0
_tmpE6:								.BYTE	0
_tmpE7:								.BYTE	0
_tmpE8:								.BYTE	0
_tmpE9:								.BYTE	0
_tptr0:								.BYTE	0,0
_tptr1:								.BYTE	0,0
_tptr2:								.BYTE	0,0
_txt_ptr:							.BYTE	0,0
_weapon0_idx:						.BYTE	0
_weapon1_idx:						.BYTE	0

		TOBJ	_obj_flags
		TOBJ	_obj_spr_idx
		TOBJ	_obj_pal_idx
		TOBJ	_obj_x_lo
		TOBJ	_obj_x_hi
		TOBJ	_obj_y
		TOBJ	_obj_hcoll
		TOBJ	_obj_vcoll

; --------------------------------
; 0x00-0x20 - enemies (0x1A max is used)
; 0x21 - fuel tank
; 0x22 - smart bomb
; 0x23 - repair item A
; 0x24 - repair item B (for slot A)
; 0x25 - fire power upgrade
; 0x26 - powerup
; 0x27 - missile upgrade
;
		TOBJ	_obj_hndl_idx
		TOBJ	_obj_x_velocity
		
_end_of_zero_ram	EQU	.

		ORG	$200

_spr_buf:
		.BYTE	0

		ORG	$300

_pal_buf:
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.BYTE	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
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
		
_chr0_bank_shadow:					.BYTE	0
_chr1_bank_shadow:					.BYTE	0
_cur_arms_percent_backup:			.BYTE	0
_cur_body_percent_backup:			.BYTE	0
_cur_head_percent_backup:			.BYTE	0
_cur_legs_percent_backup:			.BYTE	0
_cur_repair_tanks_backup:			.BYTE	0
_cur_repair_tanks_number_tile_tmp:	.BYTE	0
_delay_counter:						.BYTE	0,0
_disp_extra_shift:					.BYTE	0
_disp_spr0hit_use:					.BYTE	0
_disp_spr_send_tmp:					.BYTE	0
_dpad:								.BYTE	0
_event_idx:							.BYTE	0
_event_reload_idx:					.BYTE	0
_generic_nmi_over_counter:			.BYTE	0
_hi_score_entry_display_lock:		.BYTE	0
_log_msg_skip_forbid:				.BYTE	0
_mul_argA:							.BYTE	0
_mul_argB:							.BYTE	0
_mul_result_low:					.BYTE	0
_pad:								.BYTE	0
_pad_prev0:							.BYTE	0
_pad_prev1:							.BYTE	0
_repair_body_index_request:			.BYTE	0
_repair_cur_body_index:				.BYTE	0
_repair_cur_percent_disp_buf_tmp:	.BYTE	0,0,0,0,0,0
_repair_cursor_animate_in_progress:	.BYTE	0
_repair_cursor_animate_tmp:			.BYTE	0
_repair_cursor_pos_dest:			.BYTE	0,0
_repair_cursor_spr_x_pos:			.BYTE	0
_repair_cursor_spr_y_pos:			.BYTE	0
_screen_idx:						.BYTE	0
_special_controls_flag:				.BYTE	0
_star_field_center_pos_x:			.BYTE	0
_star_field_center_pos_y:			.BYTE	0
_star_filed_sign_tmp:				.BYTE	0
_title_menu_idx:					.BYTE	0
_title_menu_spr_num_tmp:			.BYTE	0
_tmp_idx0:							.BYTE	0
_tmp_idx1:							.BYTE	0
_tmp_idx2:							.BYTE	0

		ORG	$400

_apu_voictest:		.BYTE	0,0,0,0
_apu_stpindex:		.BYTE	0,0,0,0
_apu_seqindex:		.BYTE	0,0,0,0
_apu_seqposit:		.BYTE	0,0,0,0
_apu_repstcnt:		.BYTE	0,0,0,0
_apu_stepnumb:		.BYTE	0,0,0,0
_apu_spdcount:		.BYTE	0,0,0,0
_apu_spdcheck:		.BYTE	0,0,0,0
_apu_realplay:		.BYTE	0,0,0,0
_apu_noteinit:		.BYTE	0,0,0,0
_apu_realsoun:		.BYTE	0,0,0,0
_apu_noteleng:		.BYTE	0,0,0,0
_apu_realleng:		.BYTE	0,0,0,0
_apu_realnote:		.BYTE	0,0,0,0
_apu_lofrqwrd:		.BYTE	0,0,0,0
_apu_hifrqwrd:		.BYTE	0,0,0,0
_apu_tranbyte:		.BYTE	0,0,0,0
_apu_sfxtrans:		.BYTE	0,0,0,0
_apu_arptrans:		.BYTE	0,0,0,0
_apu_arpindex:		.BYTE	0,0,0,0
_apu_arpnumbo:		.BYTE	0,0,0,0
_apu_arpspeed:		.BYTE	0,0,0,0
_apu_arponoto:		.BYTE	0,0,0,0
_apu_gldspeed:		.BYTE	0,0,0,0
_apu_glddirec:		.BYTE	0,0,0,0
_apu_glddelay:		.BYTE	0,0,0,0
_apu_destnote:		.BYTE	0,0,0,0
_apu_vibdelay:		.BYTE	0,0,0,0
_apu_vibindex:		.BYTE	0,0,0,0
_apu_continos:		.BYTE	0,0,0,0
_apu_amplitud:		.BYTE	0,0,0,0
_apu_ampindex:		.BYTE	0,0,0,0
_apu_fx01:			.BYTE	0,0,0,0
_apu_fx02:			.BYTE	0,0,0,0
_apu_fx03:			.BYTE	0,0,0,0
_apu_pulindex:		.BYTE	0,0,0,0
_apu_pulsbyt1:		.BYTE	0,0,0,0

		ORG	$500

_tmp500:			.BYTE	0
_tmp501:			.BYTE	0
_tmp502:			.BYTE	0

		TOBJ	_obj_var0
		TOBJ	_obj_var1
		TOBJ	_obj_var2
		TOBJ	_obj_var3
		TOBJ	_obj_var4
		TOBJ	_obj_var5
		TOBJ	_obj_var6
		TOBJ	_obj_var7
		TOBJ	_obj_var8
		TOBJ	_obj_var9

		ORG	$600

_tmp_ptr:
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
_hex2dec_tile_delta:
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
_lvl_bonus_portal_detected:
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
_special_boss_flood_object_flag:
		.BYTE	0
_special_boss_flood_object_count:
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
_apu_music_enable:
		.BYTE	0
_apu_sound_enable:
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
