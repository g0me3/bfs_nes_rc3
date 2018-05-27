
		ORG $C000, $00

; =============== S U B	R O U T	I N E =======================================
_j_RESET:
		JMP		_RESET

; =============== S U B	R O U T	I N E =======================================
_j_apu_mus_start:
		JMP		_apu_mus_start

; =============== S U B	R O U T	I N E =======================================
_j_far_apu_play:
		JMP		_far_apu_play

; =============== S U B	R O U T	I N E =======================================
_j_MAIN_LOOP:
		JMP		_MAIN_LOOP

; =============== S U B	R O U T	I N E =======================================
_RESET:
		SEI
		LDA		_PPU_STATUS
		LDX		#0
		STX		_PPU_CTRL
		STX		_PPU_MASK
		STX		_ppu_ctrl_shadow
		STX		_ppu_mask_shadow
		STX		_APU_STATUS
		LDY		_unused1B3
		TXA
loc_60023:
		STA		0,X
		STA		$100,X
		STA		$200,X
		STA		$300,X
		STA		$400,X
		STA		$500,X
		STA		$600,X
		STA		$700,X
		INX
		BNE		loc_60023
		DEX
		TXS
		STY		_unused1B3
		LDX		#$59
loc_60044:
		LDA		_def_score_tbl,X
		STA		_hi0_score,X
		DEX
		BPL		loc_60044
		JSR		_all_player_vars_init
		LDA		#1
		STA		_apu_sound_enable
		STA		_apu_music_enable
		LDY		#5
		JSR		_mmc1_prg
		JSR		_ppu_reinit
		LDA		#0
		STA		_nmi_hndl_idx
		LDA		#0
		JSR		_j_exec_sub

; =============== S U B	R O U T	I N E =======================================
_MAIN_LOOP:
		JSR		_ppu_reinit
		LDA		#$23
		STA		_rand_seed
		LDA		#$4C
		STA		_rand_seed+1
		LDA		#$E6
		STA		_rand_seed+2
		LDA		#$B1
		STA		_rand_seed+3
		LDA		#0
		STA		_hud_msg_idx
		STA		_lvl2_smart_bomb_flag
		STA		_lvl4_destruction_counter
		LDA		#2
		STA		_hud_random_msg_timeout+1
		STA		_player_damage_blink_timer
		STA		byte_685
		LDA		_cur_lvl_idx
		JSR		_asci_bios_msg_display_with_skip
		JSR		_ppu_reinit
		LDA		#0
		STA		_weapon0_idx
		STA		_weapon1_idx
		STA		_weapon_type
_MAIN_GAME_START:
		LDX		_player_number
		LDA		_pl_bckp_lvl_idx,X
		STA		_cur_lvl_idx
		LDA		_pl_bckp_head_percent,X
		STA		_cur_head_percent
		LDA		_pl_bckp_body_percent,X
		STA		_cur_body_percent
		LDA		_pl_bckp_arms_percent,X
		STA		_cur_arms_percent
		LDA		_pl_bckp_legs_percent,X
		STA		_cur_legs_percent
		LDA		_pl_bckp_repair_tanks,X
		STA		_cur_repair_tanks
		LDA		_pl_bckp_score_hex_lo,X
		STA		_cur_score_hex
		LDA		_pl_bckp_score_hex_hi,X
		STA		_cur_score_hex+1
		JSR		_calc_failure_threshold
loc_600D6:
		SEI
		LDA		_PPU_STATUS
		LDX		#0
		STX		_PPU_CTRL
		STX		_PPU_MASK
		STX		_ppu_ctrl_shadow
		STX		_ppu_mask_shadow
		STX		_APU_STATUS
		LDA		#$E7
		STA		_ppu_mask_shadow_mask
		LDA		#3
		STA		_nmi_hndl_idx
		LDX		_cur_lvl_idx
		LDY		_lvl_prg_bank_list,X
		JSR		_mmc1_prg
		JSR		_lvl_vars_init
		LDA		#0
		STA		_nmi_is_over_flag
		STA		_pause_flag
		LDA		#2
		STA		byte_6F5
		LDA		#1
		STA		_star_field_data1
		LDA		#$F
		STA		_star_field_data0+$27
		STA		_star_field_data0+$26
_main_ingame_loop2:
		LDA		_nmi_is_over_flag
		BEQ		_main_ingame_loop2
		LDA		#0
		STA		_nmi_is_over_flag
		JSR		_main_ingame_handler
		DEC		byte_6F5
		BNE		_main_ingame_loop2
		LDA		#1
		STA		_pause_flag
		LDA		#2
		STA		byte_6F5
_main_ingame_loop0:
		LDA		_nmi_is_over_flag
		BEQ		_main_ingame_loop0
		LDA		#0
		STA		_nmi_is_over_flag
		JSR		_main_ingame_handler
		DEC		byte_6F5
		BNE		_main_ingame_loop0
		LDA		#0
		STA		_pause_flag
		STA		_nmi_is_over_flag
		LDY		_cur_lvl_state_idx
		LDA		_lvl_bg_pal_list_lo,Y
		STA		_ptr0
		LDA		_lvl_bg_pal_list_hi,Y
		STA		_ptr0+1
		LDY		#$F
loc_60152:
		LDA		(_ptr0),Y
		STA		_ingame_bg_pal_buf,Y
		DEY
		BPL		loc_60152
		LDY		_cur_lvl_state_idx
		LDA		_lvl_spr_pal_list_lo,Y
		STA		_ptr0
		LDA		_lvl_spr_pal_list_hi,Y
		STA		_ptr0+1
		LDY		#$F
loc_60168:
		LDA		(_ptr0),Y
		STA		_ingame_spr_pal_buf,Y
		DEY
		BPL		loc_60168
_main_ingame_loop1:
		LDA		_nmi_is_over_flag
		BEQ		_main_ingame_loop1
		LDA		#0
		STA		_nmi_is_over_flag
		JSR		_main_ingame_handler
		JSR		_randomize
		LDA		_game_over_flag
		BEQ		_main_ingame_loop1
		BPL		_level_complete
		LDX		_player_number
		DEC		_pl_bkcp_continues,X
		LDA		_cur_score_hex
		STA		_pl_bckp_score_hex_lo,X
		LDA		_cur_score_hex+1
		STA		_pl_bckp_score_hex_hi,X
		LDA		_cur_lvl_idx
		STA		_pl_bckp_lvl_idx,X
		LDY		#5
		JSR		_mmc1_prg
		JSR		_ppu_reinit
		LDA		#0
		STA		_nmi_hndl_idx
		STA		_cur_lvl_state_idx
		LDA		#2
		JSR		_j_exec_sub
		LDA		_new_game_start_flag
		BNE		loc_601BA
		JSR		_all_player_vars_init
		JMP		_MAIN_LOOP
loc_601BA:
		LDX		_player_number
		LDA		#0
		STA		_pl_bckp_score_hex_lo,X
		STA		_pl_bckp_score_hex_hi,X
		JMP		_MAIN_LOOP
_level_complete:
		LDA		_cur_lvl_idx
		CMP		#5
		BEQ		_game_complete
		LDX		_player_number
		LDA		_cur_lvl_idx
		STA		_pl_bckp_lvl_idx,X
		LDA		#0
		STA		_nmi_hndl_idx
		STA		_cur_lvl_state_idx
		JSR		_ppu_reinit
		LDY		#5
		JSR		_mmc1_prg
		LDA		_cur_lvl_idx
		CMP		#4
		BNE		_player_backup
		JMP		_MAIN_LOOP
_player_backup:
		LDX		_player_number
		LDA		_cur_score_hex
		STA		_pl_bckp_score_hex_lo,X
		LDA		_cur_score_hex+1
		STA		_pl_bckp_score_hex_hi,X
		LDA		_cur_head_percent
		STA		_pl_bckp_head_percent,X
		LDA		_cur_body_percent
		STA		_pl_bckp_body_percent,X
		LDA		_cur_arms_percent
		STA		_pl_bckp_arms_percent,X
		LDA		_cur_legs_percent
		STA		_pl_bckp_legs_percent,X
		LDA		#6
		JSR		_asci_bios_msg_display_with_skip
		LDA		#1
		JSR		_j_exec_sub
		JMP		_MAIN_LOOP
_game_complete:
		LDY		#5
		JSR		_mmc1_prg
		JSR		_ppu_reinit
		LDA		#0
		STA		_nmi_hndl_idx
		LDA		#3
		JSR		_j_exec_sub
		JSR		_player_vars_init_ex
		JMP		_MAIN_LOOP

; =============== S U B	R O U T	I N E =======================================
_repair_lab_handler:
		LDX		_player_number
		LDA		_cur_lvl_idx
		STA		_pl_bckp_lvl_idx,X
		LDA		#0
		STA		_nmi_hndl_idx
		LDA		#1
		STA		_cur_lvl_state_idx
		JSR		_ppu_reinit
		LDY		#5
		JSR		_mmc1_prg
		LDA		#6
		JSR		_asci_bios_msg_display_with_skip
		LDA		#1
		JSR		_j_exec_sub
		JMP		_MAIN_GAME_START

; =============== S U B	R O U T	I N E =======================================
_all_player_vars_init:
		LDX		#1

; =============== S U B	R O U T	I N E =======================================
_player_vars_init_ex:
		LDA		#0
		STA		_pl_bckp_lvl_idx,X
		LDA		#$19
		STA		_pl_bckp_head_percent,X
		STA		_pl_bckp_body_percent,X
		STA		_pl_bckp_arms_percent,X
		STA		_pl_bckp_legs_percent,X
		LDA		#5
		STA		_pl_bckp_repair_tanks,X
		LDA		#3
		STA		_pl_bkcp_continues,X
		LDA		#0
		STA		_pl_bckp_score_hex_lo,X
		STA		_pl_bckp_score_hex_hi,X
		DEX
		BPL		_player_vars_init_ex
		JSR		_ppu_pal_clear
		RTS

; =============== S U B	R O U T	I N E =======================================
_calc_failure_threshold:
		SEC
		LDA		#$FF
		SBC		_cur_head_percent
		STA		_failure_treshold_list
		SEC
		LDA		#$FF
		SBC		_cur_body_percent
		STA		_failure_treshold_list+1
		SEC
		LDA		#$FF
		SBC		_cur_arms_percent
		STA		_failure_treshold_list+2
		SEC
		LDA		#$FF
		SBC		_cur_legs_percent
		STA		_failure_treshold_list+3
		RTS

; =============== S U B	R O U T	I N E =======================================
_failure_test_handler:
		DEC		_failure_duration_period
		BPL		loc_602E2
		LDA		#$64
		STA		_failure_duration_period
		LDA		#0
		STA		_failure_mode
		LDA		_rand_seed
		AND		#3
		TAX
		LDA		_failure_treshold_list,X
		CMP		#$EE
		BCC		loc_602D6
		LDA		_rand_seed
		CMP		#$14
		BCS		loc_602D6
		INX
		STX		_failure_mode
locret_602D5:
		RTS
loc_602D6:
		LDA		_hud_msg_idx
		CMP		#$16
		BCC		locret_602D5
		LDA		#0
		JSR		_hud_msg_set_request
		RTS
loc_602E2:
		LDA		_failure_mode
		BNE		loc_602E8
		RTS
loc_602E8:
		CMP		#4
		BNE		loc_602F5
		LDA		_pads
		AND		#$78
		STA		_pads
		JMP		loc_6031D
loc_602F5:
		CMP		#3
		BNE		loc_60302
		LDA		_pads
		AND		#$BF
		STA		_pads
		JMP		loc_6031D
loc_60302:
		CMP		#2
		BNE		loc_6030F
		LDA		_pads
		AND		#$7B
		STA		_pads
		JMP		loc_6031D
loc_6030F:
		LDA		_pads
		AND		#3
		TAX
		LDA		_pads
		AND		#$FC
		ORA		_failure_dpad_swap_table,X
		STA		_pads
loc_6031D:
		LDA		_failure_mode
		CLC
		ADC		#$15
		CMP		_hud_msg_idx
		BEQ		locret_6032A
		JSR		_hud_msg_set_request
locret_6032A:
		RTS
_failure_dpad_swap_table:
		.BYTE	  0,  2,  1,  3

; =============== S U B	R O U T	I N E =======================================
_lvl_vars_init:
		SEI
		LDX		#0
		STX		_lvl_bg_chr_treshold+1
		STX		_player_scroll_stop_override_tmp
		STX		_obj_shuffle_mode
		STX		_game_over_flag
		STX		_pause_flag
		STX		_player_explode_timer_tmp
		STX		_player_sit_jump_flag
		STX		_end_of_the_level_flag
		STX		_lvl_rooms_walked_count_unused
		STX		byte_69D
		STX		_lvl3_tank_weapon_disable_flag
		LDA		#3
		STA		_nmi_hndl_idx
		STA		byte_69C
		LDA		#3
		STA		_lvl3_jetpack_fuel_tanks
		LDY		_cur_lvl_state_idx
		LDA		_lvl_room_start_ptr_list_lo,Y
		STA		_lvl_room_data_ptr
		LDA		_lvl_room_start_ptr_list_hi,Y
		STA		_lvl_room_data_ptr+1
		LDA		_lvl_obj_list_lib_lo,Y
		STA		_lvl_obj_list_ptr
		LDA		_lvl_obj_list_lib_hi,Y
		STA		_lvl_obj_list_ptr+1
		LDA		_lvl_room_scroll_initial_values_list,Y
		STA		_player_wnd_scroll_counter
		LDA		_lvl_room_initial_tile_values_list,Y
		STA		_player_wnd_tile_number
		LDA		_lvl_collision_data0_lib_lo,Y
		STA		_lvl_collision_data0_ptr
		LDA		_lvl_collision_data0_lib_hi,Y
		STA		_lvl_collision_data0_ptr+1
		LDA		_lvl_collision_data1_lib_lo,Y
		STA		_lvl_collision_data1_ptr
		LDA		_lvl_collision_data1_lib_hi,Y
		STA		_lvl_collision_data1_ptr+1
		LDA		_lvl_collision_data2_lib_lo,Y
		STA		_lvl_collision_data2_ptr
		LDA		_lvl_collision_data2_lib_hi,Y
		STA		_lvl_collision_data2_ptr+1
		LDA		_lvl_bg0_chr_bank_list,Y
		STA		_lvl_phase_bg_chr_banks_list
		LDA		_lvl_bg1_chr_bank_list,Y
		STA		_lvl_phase_bg_chr_banks_list+1
		LDA		_lvl_bg2_chr_bank_list,Y
		STA		_lvl_phase_bg_chr_banks_list+2
		LDA		_lvl_initial_scroll_list,Y
		STA		_lvl_scroll_value
		STA		_lvl_scroll_tile_hit_flag
		LDA		_lvl_initial_nt_list,Y
		STA		_lvl_nt_value
		LDY		#$F
		LDA		#$F
loc_603B6:
		STA		_ingame_bg_pal_buf,Y
		STA		_ingame_spr_pal_buf,Y
		DEY
		BPL		loc_603B6
loc_603BF:
		LDA		_PPU_STATUS
		BPL		loc_603BF
		LDY		_cur_lvl_state_idx
		LDA		_lvl_obj_global_y_delta_list,Y
		STA		_obj_global_y_delta
		LDA		_lvl_stop_sctoll_room_idx_list,Y
		STA		_lvl_stop_sctoll_room_idx
		LDA		_lvl_cur_spr_chr_bank_up_list,Y
		STA		_lvl_cur_spr_chr_bank_up
		STA		_lvl_cur_spr_chr_bank_up_backup
		LDA		_lvl_cur_spr_chr_bank_down_list,Y
		STA		_lvl_cur_spr_chr_bank_down
		STA		_lvl_cur_spr_chr_bank_down_backup
		LDA		#$FF
		STA		_pal_blink_flag
		LDA		#$1E
		STA		_lvl_door_enrty_delay
		LDA		#$14
		STA		_end_of_the_level_delay
		LDA		_PPU_STATUS
		JSR		_lvl_obj_vars_init
		LDA		_lvl_obj_num_at_start,Y
		STA		_tmpE5
		BEQ		loc_60402
loc_603FB:
		JSR		_obj_add
		DEC		_tmpE5
		BNE		loc_603FB
loc_60402:
		LDA		#0
		STA		_snd_idx
		JSR		_apu_snd_load
		CLC
		LDA		_cur_lvl_idx
		ADC		#$22
		STA		_snd_idx
		JSR		_apu_snd_load
		JSR		_ppu_reinit
		RTS

; =============== S U B	R O U T	I N E =======================================
_apu_mus_start:
		STA		_snd_idx
		JSR		_apu_snd_load
		LDY		#5
		JSR		_mmc1_prg
		RTS

; =============== S U B	R O U T	I N E =======================================
_far_apu_play:
		LDY		#6
		JSR		_mmc1_prg
		JSR		_apu_play_prg_safe
		LDY		#5
		JSR		_mmc1_prg
		RTS

; =============== S U B	R O U T	I N E =======================================
_ppu_reinit:
		SEI
		LDX		#0
		STX		_PPU_CTRL
		STX		_PPU_MASK
		STX		_PPU_STATUS
loc_6043C:
		BIT		_PPU_STATUS
		BPL		loc_6043C
loc_60441:
		BIT		_PPU_STATUS
		BMI		loc_60441
loc_60446:
		BIT		_PPU_STATUS
		BPL		loc_60446
loc_6044B:
		BIT		_PPU_STATUS
		BMI		loc_6044B
		LDX		#0
		LDY		#$88
		STX		_PPU_MASK
		STY		_PPU_CTRL
		STX		_ppu_mask_shadow
		STY		_ppu_ctrl_shadow
		RTS

; =============== S U B	R O U T	I N E =======================================
_delayX:
		DEX
		BNE		_delayX
		RTS

; =============== S U B	R O U T	I N E =======================================
_unused_9:
		RTS

; =============== S U B	R O U T	I N E =======================================
IRQ:
		CLI
		RTI

; =============== S U B	R O U T	I N E =======================================
NMI:
		STA		_irq_regs_backup
		STY		_irq_regs_backup+2
		STX		_irq_regs_backup+1
		LDA		_nmi_hndl_idx
		ASL
		TAY
		LDA		_nmis_lib,Y
		STA		_tmp_ptr0
		LDA		_nmis_lib+1,Y
		STA		_tmp_ptr0+1
		JMP		(_tmp_ptr0)

; =============== S U B	R O U T	I N E =======================================
_nmi5:
		BIT		_PPU_STATUS
		LDX		#0
		STX		_PPU_MASK
		STX		_ppu_mask_shadow
		STX		_unused1B3
		STX		_PPU_CTRL
		STX		_PPU_OAM_ADDR
		LDA		#2
		STA		_PPU_OAM_DMA
		LDA		_ppu_ctrl_shadow
		AND		#$78
		STA		_PPU_CTRL
		STA		_ppu_ctrl_shadow
		LDA		_ppu_mask_shadow
		AND		#0
		ORA		#$18
		STA		_ppu_mask_shadow
		STA		_PPU_MASK
		JSR		_ingame_pal_update
		JSR		_hud_ppu_data_send
		JSR		_lvl_ppu_data_dequeue
		LDX		#0
		STX		_PPU_SCROLL
		LDA		#5
		STA		_PPU_SCROLL
		LDA		_lvl_cur_spr_chr_bank_up
		JSR		_mmc1_chr1
		LDA		#0
		JSR		_mmc1_chr0
		JSR		_delay256
		LDA		_lvl_cur_bg_chr_bank
		JSR		_mmc1_chr0
		LDA		_ppu_mask_shadow
		ORA		#$18
		AND		_ppu_mask_shadow_mask
		AND		#$FE
		ORA		#1
		STA		_ppu_mask_shadow
		LDA		_ppu_ctrl_shadow
		AND		#$F8
		ORA		_lvl_nt_value
		ORA		#$80
		STA		_ppu_ctrl_shadow
		BIT		_unused1B3
		BIT		_unused1B3
		BIT		_unused1B3
		BIT		_unused1B3
		BIT		_unused1B3
		BIT		_unused1B3
		BIT		_unused1B3
		BIT		_unused1B3
		BIT		_unused1B3
		BIT		_unused1B3
		BIT		_unused1B3
		BIT		_unused1B3
		BIT		_unused1B3
		BIT		_unused1B3
		BIT		_unused4
		LDX		_lvl_scroll_value
		LDY		#5
		STA		_PPU_CTRL
		BIT		_PPU_SCROLL
		STX		_PPU_SCROLL
		STY		_PPU_SCROLL
		LDA		#1
		STA		_nmi_is_over_flag
		JMP		_irq_exit

; =============== S U B	R O U T	I N E =======================================
_nmi_on_exit:
		LDA		_ppu_ctrl_shadow
		ORA		#$80
		STA		_PPU_CTRL
		STA		_ppu_ctrl_shadow

; =============== S U B	R O U T	I N E =======================================
_irq_exit:
		LDY		_irq_regs_backup+2
		LDX		_irq_regs_backup+1
		LDA		_irq_regs_backup
		RTI

; =============== S U B	R O U T	I N E =======================================
_nmi3:
		LDA		#$E7
		STA		_ppu_mask_shadow_mask
		JSR		_spr_clear
		JSR		_ppu_nt_clear
		INC		_nmi_hndl_idx
		JMP		_nmi_on_exit

; =============== S U B	R O U T	I N E =======================================
_nmi4:
		LDX		_cur_lvl_idx
		LDY		_lvl_prg_bank_list,X
		JSR		_mmc1_prg
		JSR		_hud_init
		JSR		_lvl_rooms_init_draw
		INC		_nmi_hndl_idx
		JMP		_nmi_on_exit
_nmis_lib:
		.WORD _j_nmi0
		.WORD _j_nmi0
		.WORD _j_nmi0
		.WORD _nmi3
		.WORD _nmi4
		.WORD _nmi5

; =============== S U B	R O U T	I N E =======================================
_lvl_ppu_data_dequeue:
		LDA		_cur_lvl_idx
		ASL
		TAY
		LDA		_ppu_dequeues_lib,Y
		STA		_tmp_ptr0
		LDA		_ppu_dequeues_lib+1,Y
		STA		_tmp_ptr0+1
		JMP		(_tmp_ptr0)
_ppu_dequeues_lib:
		.WORD _lvl0_ppu_data_dequeue
		.WORD _lvl0_ppu_data_dequeue
		.WORD _lvl0_ppu_data_dequeue
		.WORD _lvl0_ppu_data_dequeue
		.WORD _lvl4_ppu_data_dequeue

; =============== S U B	R O U T	I N E =======================================
_lvl0_ppu_data_dequeue:
		CLC
		LDA		_lvl_scroll_value
		ADC		#$80
		PHA
		LDA		_lvl_nt_value
		ADC		#1
		AND		#1
		ASL
		ASL
		ADC		#$21
		STA		_PPU_ADDR
		PLA
		LSR
		LSR
		LSR
		STA		_PPU_ADDR
		LDA		_ppu_ctrl_shadow
		ORA		#4
		STA		_ppu_ctrl_shadow
		STA		_PPU_CTRL
		LDY		#0
loc_605AB:
		LDA		_tmp_buf0,Y
		STA		_PPU_DATA
		INY
		CPY		#$15
		BCC		loc_605AB
		CLC
		LDA		_lvl_scroll_value
		ADC		#$80
		PHA
		LDA		_lvl_nt_value
		ADC		#1
		AND		#1
		ASL
		ASL
		ORA		#$23
		STA		_PPU_ADDR
		STA		_tmpE2+1
		PLA
		LSR
		LSR
		LSR
		LSR
		LSR
		ORA		#$D0
		STA		_PPU_ADDR
		STA		_tmpE2
		LDA		_ppu_ctrl_shadow
		AND		#$FB
		STA		_ppu_ctrl_shadow
		STA		_PPU_CTRL
		LDX		#0
loc_605E3:
		LDA		_tmp_buf1,X
		STA		_PPU_DATA
		LDA		_tmpE2+1
		STA		_PPU_ADDR
		CLC
		LDA		_tmpE2
		ADC		#8
		STA		_tmpE2
		STA		_PPU_ADDR
		LDA		_ppu_ctrl_shadow
		AND		#$FB
		STA		_ppu_ctrl_shadow
		STA		_PPU_CTRL
		INX
		CPX		#6
		BCC		loc_605E3
		RTS

; =============== S U B	R O U T	I N E =======================================
_lvl4_ppu_data_dequeue:
		LDA		_ppu_ctrl_shadow
		ORA		#4
		STA		_ppu_ctrl_shadow
		STA		_PPU_CTRL
		LDA		#$27
		STA		_PPU_ADDR
		LDA		#$1D
		STA		_PPU_ADDR
		LDA		_tmp_buf0
		STA		_PPU_DATA
		LDA		_tmp_buf0+1
		STA		_PPU_DATA
		LDA		#$26
		STA		_PPU_ADDR
		LDA		#$2D
		STA		_PPU_ADDR
		LDA		_tmp_buf0+2
		STA		_PPU_DATA
		LDA		#$26
		STA		_PPU_ADDR
		LDA		#$30
		STA		_PPU_ADDR
		LDA		_tmp_buf0+3
		STA		_PPU_DATA
		LDA		#$26
		STA		_PPU_ADDR
		LDA		#$33
		STA		_PPU_ADDR
		LDA		_tmp_buf0+4
		STA		_PPU_DATA
		LDA		#$26
		STA		_PPU_ADDR
		LDA		#$36
		STA		_PPU_ADDR
		LDA		_tmp_buf0+5
		STA		_PPU_DATA
		LDA		#$26
		STA		_PPU_ADDR
		LDA		#$8D
		STA		_PPU_ADDR
		LDA		_tmp_buf0+6
		STA		_PPU_DATA
		LDA		#$26
		STA		_PPU_ADDR
		LDA		#$90
		STA		_PPU_ADDR
		LDA		_tmp_buf0+7
		STA		_PPU_DATA
		LDA		#$26
		STA		_PPU_ADDR
		LDA		#$93
		STA		_PPU_ADDR
		LDA		_tmp_buf0+8
		STA		_PPU_DATA
		LDA		#$26
		STA		_PPU_ADDR
		LDA		#$96
		STA		_PPU_ADDR
		LDA		_tmp_buf0+9
		STA		_PPU_DATA
		LDA		#$26
		STA		_PPU_ADDR
		LDA		#$ED
		STA		_PPU_ADDR
		LDA		_tmp_buf0+$A
		STA		_PPU_DATA
		LDA		#$26
		STA		_PPU_ADDR
		LDA		#$F0
		STA		_PPU_ADDR
		LDA		_tmp_buf0+$B
		STA		_PPU_DATA
		LDA		#$26
		STA		_PPU_ADDR
		LDA		#$F3
		STA		_PPU_ADDR
		LDA		_tmp_buf0+$C
		STA		_PPU_DATA
		LDA		#$26
		STA		_PPU_ADDR
		LDA		#$F6
		STA		_PPU_ADDR
		LDA		_tmp_buf0+$D
		STA		_PPU_DATA
		LDA		_ppu_ctrl_shadow
		AND		#$FB
		STA		_ppu_ctrl_shadow
		STA		_PPU_CTRL
		LDX		#$52
		JSR		_delayX
		NOP
		NOP
		NOP
		NOP
		RTS

; =============== S U B	R O U T	I N E =======================================
_lvl_scroll_handlers:
		LDA		_cur_lvl_idx
		CMP		#4
		BEQ		_lvl4_special_handler
		JMP		loc_607E5
_lvl4_special_handler:
		LDY		#$E0
		LDA		_hud_msg_cur_blink_timer
		BPL		loc_6070B
		LDY		#$F5
loc_6070B:
		STY		_tmp_buf0
		LDA		_hud_msg_cur_blink_timer
		CMP		#$20
		BCS		loc_60720
		LDY		#$E9
		LDA		_rand_seed
		BMI		loc_6071D
		LDY		#$F4
loc_6071D:
		STY		_tmp_buf0+1
loc_60720:
		INC		byte_682
		LDA		byte_682
		CMP		#$28
		BCC		loc_6072F
		LDA		#0
		STA		byte_682
loc_6072F:
		LSR
		LSR
		CLC
		ADC		#$F6
		LDY		_lvl4_destruction_counter
		CPY		#$C
		BCC		loc_6073D
		LDA		#$49
loc_6073D:
		LDX		#3
loc_6073F:
		STA		_tmp_buf0+2,X
		STA		_tmp_buf0+6,X
		STA		_tmp_buf0+$A,X
		DEX
		BPL		loc_6073F
		LDA		_lvl4_destruction_counter
		CMP		#$C
		BCC		loc_60757
		LDA		_hud_msg_cur_blink_timer
		BMI		loc_607AA
loc_60757:
		LDX		#0
loc_60759:
		CPX		_lvl4_destruction_counter
		BEQ		loc_60769
		LDA		_lvl4_pass_chars_list,X
		STA		_tmp_buf0+2,X
		INX
		CPX		#$C
		BCC		loc_60759
loc_60769:
		LDA		byte_686
		BEQ		loc_607AA
		DEC		byte_684
		BNE		loc_607AA
		DEC		byte_685
		BMI		loc_607AA
		LDA		#2
		STA		byte_685
		LDA		#$64
		STA		byte_684
		INC		_lvl4_destruction_counter
		LDY		_lvl4_destruction_counter
		CPY		#$D
		BCS		loc_60797
		LDA		_lvl4_pass_chars_list,Y
		SEC
		SBC		#$F6
		CLC
		ADC		#$13
		STA		_snd_idx
loc_60797:
		LDY		#$B
		LDA		_lvl4_destruction_counter
		CMP		#$C
		BCC		loc_607A2
		LDY		#$C
loc_607A2:
		CPY		_hud_msg_idx
		BEQ		loc_607AA
		TYA
		JSR		_hud_msg_set_request
loc_607AA:
		LDA		_lvl4_destruction_counter
		CMP		#$C
		BCC		locret_607D5
		DEC		byte_697
		BPL		locret_607D5
		LDA		#$A
		STA		byte_697
		INC		byte_698
		LDY		byte_698
		CPY		#$C
		BCC		loc_607C7
		LDY		#0
loc_607C7:
		STY		byte_698
		LDA		_lvl4_pass_chars_list,Y
		SEC
		SBC		#$F6
		CLC
		ADC		#$13
		STA		_snd_idx
locret_607D5:
		RTS
_lvl4_pass_chars_list:
		.BYTE	$FF,$FA,$FB,$F7,$F9,$F8,$FD,$FE,$FC,$F6,$FF,$F9,$F6,$F6,$F6
loc_607E5:
		CMP		#3
		BNE		_lvl_right_scroll_handler
		JMP		_lvl_left_scroll_handler
_lvl_right_scroll_handler:
		LDA		_lvl_scroll_value
		AND		#8
		CMP		_lvl_scroll_tile_hit_flag
		BEQ		loc_60811
		STA		_lvl_scroll_tile_hit_flag
		CLC
		LDA		_player_wnd_tile_number
		ADC		#1
		AND		#$1F
		STA		_player_wnd_tile_number
		BNE		loc_60811
		CLC
		LDA		_lvl_room_data_ptr
		ADC		#$D0
		STA		_lvl_room_data_ptr
		LDA		_lvl_room_data_ptr+1
		ADC		#2
		STA		_lvl_room_data_ptr+1
		INC		_lvl_rooms_walked_count_unused
loc_60811:
		LDA		_lvl_room_data_ptr
		PHA
		LDA		_lvl_room_data_ptr+1
		PHA
		LDX		#0
		LDY		_player_wnd_tile_number
loc_6081B:
		LDA		(_lvl_room_data_ptr),Y
		STA		_tmp_buf0,X
		CLC
		LDA		_lvl_room_data_ptr
		ADC		#$20
		STA		_lvl_room_data_ptr
		LDA		_lvl_room_data_ptr+1
		ADC		#0
		STA		_lvl_room_data_ptr+1
		INX
		CPX		#$15
		BCC		loc_6081B
		PLA
		STA		_lvl_room_data_ptr+1
		PLA
		STA		_lvl_room_data_ptr
		CLC
		LDA		_lvl_room_data_ptr
		PHA
		ADC		#$A0
		STA		_lvl_room_data_ptr
		LDA		_lvl_room_data_ptr+1
		PHA
		ADC		#2
		STA		_lvl_room_data_ptr+1
		LDA		_player_wnd_tile_number
		LSR
		LSR
		TAY
		LDX		#0
loc_6084E:
		LDA		(_lvl_room_data_ptr),Y
		STA		_tmp_buf1,X
		TYA
		CLC
		ADC		#8
		TAY
		INX
		CPX		#6
		BCC		loc_6084E
		PLA
		STA		_lvl_room_data_ptr+1
		PLA
		STA		_lvl_room_data_ptr
		RTS
_lvl_left_scroll_handler:
		LDA		_lvl_scroll_value
		AND		#8
		CMP		_lvl_scroll_tile_hit_flag
		BEQ		loc_6088B
		STA		_lvl_scroll_tile_hit_flag
		SEC
		LDA		_player_wnd_tile_number
		SBC		#1
		AND		#$1F
		STA		_player_wnd_tile_number
		CMP		#$1F
		BNE		loc_6088B
		SEC
		LDA		_lvl_room_data_ptr
		SBC		#$D0
		STA		_lvl_room_data_ptr
		LDA		_lvl_room_data_ptr+1
		SBC		#2
		STA		_lvl_room_data_ptr+1
		DEC		_lvl_rooms_walked_count_unused
loc_6088B:
		LDA		_lvl_room_data_ptr
		PHA
		LDA		_lvl_room_data_ptr+1
		PHA
		LDX		#0
		LDY		_player_wnd_tile_number
loc_60895:
		LDA		(_lvl_room_data_ptr),Y
		STA		_tmp_buf0,X
		CLC
		LDA		_lvl_room_data_ptr
		ADC		#$20
		STA		_lvl_room_data_ptr
		LDA		_lvl_room_data_ptr+1
		ADC		#0
		STA		_lvl_room_data_ptr+1
		INX
		CPX		#$15
		BCC		loc_60895
		PLA
		STA		_lvl_room_data_ptr+1
		PLA
		STA		_lvl_room_data_ptr
		CLC
		LDA		_lvl_room_data_ptr
		PHA
		ADC		#$A0
		STA		_lvl_room_data_ptr
		LDA		_lvl_room_data_ptr+1
		PHA
		ADC		#2
		STA		_lvl_room_data_ptr+1
		LDA		_player_wnd_tile_number
		LSR
		LSR
		TAY
		LDX		#0
loc_608C8:
		LDA		(_lvl_room_data_ptr),Y
		STA		_tmp_buf1,X
		TYA
		CLC
		ADC		#8
		TAY
		INX
		CPX		#6
		BCC		loc_608C8
		PLA
		STA		_lvl_room_data_ptr+1
		PLA
		STA		_lvl_room_data_ptr
		RTS

; =============== S U B	R O U T	I N E =======================================
_lvl_bg_chr_switch:
		DEC		_lvl_bg_chr_treshold
		BPL		loc_608F1
		LDA		#6
		STA		_lvl_bg_chr_treshold
		LDY		_lvl_bg_chr_treshold+1
		INY
		CPY		#3
		BCC		loc_608EF
		LDY		#0
loc_608EF:
		STY		_lvl_bg_chr_treshold+1
loc_608F1:
		LDY		_lvl_bg_chr_treshold+1
		LDA		_lvl_phase_bg_chr_banks_list,Y
		STA		_lvl_cur_bg_chr_bank
		RTS

; =============== S U B	R O U T	I N E =======================================
_hud_update_ppu_data:
		CLC
		LDA		_hud_update_ppu_data_counter
		ADC		#1
		AND		#3
		STA		_hud_update_ppu_data_counter
		ASL
		TAY
		LDA		_hud_ppu_data_update_lib,Y
		STA		_tmp_ptr0
		LDA		_hud_ppu_data_update_lib+1,Y
		STA		_tmp_ptr0+1
		JMP		(_tmp_ptr0)

; =============== S U B	R O U T	I N E =======================================
_hud_msg_ppu_data_update:
		LDA		_hud_msg_idx
		ASL
		TAY
		LDA		_hud_msg_lib,Y
		STA		_ptr0
		LDA		_hud_msg_lib+1,Y
		STA		_ptr0+1
		LDY		#0
loc_60923:
		LDA		(_ptr0),Y
		BEQ		loc_60935
		SEC
		SBC		#$35
		STA		_tmp_buf2,Y
		INY
		CPY		_hud_msg_type_pos
		BCC		loc_60923
		JMP		loc_6093B
loc_60935:
		LDA		#$28
		STA		_tmp_buf2,Y
		INY
loc_6093B:
		LDX		#$2B
		CLC
		LDA		_hud_msg_cur_blink_timer
		ADC		#$20
		STA		_hud_msg_cur_blink_timer
		BPL		loc_6094A
		LDX		#0
loc_6094A:
		TXA
		STA		_tmp_buf2,Y
		LDA		#0
loc_60950:
		INY
		CPY		#$1C
		BEQ		loc_6095B
		STA		_tmp_buf2,Y
		JMP		loc_60950
loc_6095B:
		LDA		_hud_msg_type_pos
		CMP		#$1B
		BCS		loc_60963
		INC		_hud_msg_type_pos
loc_60963:
		DEC		_hud_random_msg_timeout
		BNE		locret_6097A
		DEC		_hud_random_msg_timeout+1
		BNE		locret_6097A
		LDA		_rand_seed+1
		AND		#3
		CMP		_hud_msg_idx
		BNE		loc_60977
		EOR		#1
loc_60977:
		JSR		_hud_msg_set_request
locret_6097A:
		RTS

; =============== S U B	R O U T	I N E =======================================
_hud_msg_set_request:
		STA		_hud_msg_idx
		LDA		#$C8
		STA		_hud_random_msg_timeout
		LDA		#2
		STA		_hud_random_msg_timeout+1
		LDA		#0
		STA		_hud_msg_type_pos
		RTS
_hud_msg_lib:
		.WORD _hud_msg00
		.WORD _hud_msg01
		.WORD _hud_msg02
		.WORD _hud_msg03
		.WORD _hud_msg04
		.WORD _hud_msg05_damage_alert
		.WORD _hud_msg06_fire_upgrade
		.WORD _hud_msg07
		.WORD _hud_msg08
		.WORD _hud_msg09_system_failure
		.WORD _hud_msg0A_missile_jammed
		.WORD _hud_msg0B_passcode_accepted
		.WORD _hud_msg0C_otomo_destruction
		.WORD _hud_msg0D
		.WORD _hud_msg0E_smart_bomb_deployed
		.WORD _hud_msg0F
		.WORD _hud_msg10
		.WORD _hud_msg11
		.WORD _hud_msg12
		.WORD _hud_msg13_jetpack_fuel_low
		.WORD _hud_msg14_maximum_fuel
		.WORD _hud_msg15_tank_weapon_disabled
		.WORD _hud_msg16_head_failure
		.WORD _hud_msg17_body_failure
		.WORD _hud_msg18_arms_failure
		.WORD _hud_msg19_legs_failure
_hud_msg00:
		.BYTE	"PROTECT@THE@INNOCENT_",0
_hud_msg01:
		.BYTE	"UPHOLD@THE@LAW_",0
_hud_msg02:
		.BYTE	"COME@AND@GET@IT@SCUM_",0
_hud_msg03:
		.BYTE	"SERVE@THE@PUBLIC@TRUST_",0
_hud_msg04:
		.BYTE	"EFFICIENCY@INCREASED_",0
_hud_msg05_damage_alert:
		.BYTE	"SYSTEMS@DAMAGE@ALERT_",0
_hud_msg06_fire_upgrade:
		.BYTE	"FIRE@POWER@UPGRADED_",0
_hud_msg07:
		.BYTE	"MISSILES@UPGRADED_",0
_hud_msg08:
		.BYTE	"REPAIR@ITEM@OBTAINED_",0
_hud_msg09_system_failure:
		.BYTE	"SYSTEM@FAILURE_",0
_hud_msg0A_missile_jammed:
		.BYTE	"MISSILE@JAMMED_",0
_hud_msg0B_passcode_accepted:
		.BYTE	"PASSCODE@ACCEPTED_",0
_hud_msg0C_otomo_destruction:
		.BYTE	"OTOMO@DESTRUCT@INITIATED_",0
_hud_msg0D:
		.BYTE	"SMART@BOMB@INITIATED_",0
_hud_msg0E_smart_bomb_deployed:
		.BYTE	"SMART@BOMB@DEPLOYED_",0
_hud_msg0F:
		.BYTE	"WARNING@HEAD@UNIT@DAMAGED_",0
_hud_msg10:
		.BYTE	"WARNING@BODY@UNIT@DAMAGED_",0
_hud_msg11:
		.BYTE	"WARNING@ARM@UNITS@DAMAGED_",0
_hud_msg12:
		.BYTE	"WARNING@LEG@UNITS@DAMAGED_",0
_hud_msg13_jetpack_fuel_low:
		.BYTE	"WARNING@JETPACK@FUEL@LOW_",0
_hud_msg14_maximum_fuel:
		.BYTE	"MAXIMUM@FUEL_",0
_hud_msg15_tank_weapon_disabled:
		.BYTE	"TANK@WEAPONS@DISABLED_",0
_hud_msg16_head_failure:
		.BYTE	"HEAD@UNIT@MALFUNCTION_",0
_hud_msg17_body_failure:
		.BYTE	"BODY@UNIT@MALFUNCTION_",0
_hud_msg18_arms_failure:
		.BYTE	"ARM@UNITS@MALFUNCTION_",0
_hud_msg19_legs_failure:
		.BYTE	"LEG@UNITS@MALFUNCTION_",0
_hud_msg1A_out_of_time:
		.BYTE	"OUT@OF@TIME_",0

; =============== S U B	R O U T	I N E =======================================
_hud_lvl4_timer_ppu_data_update:
		DEC		_star_field_data0+$26
		BNE		loc_60C31
		LDA		#$F
		STA		_star_field_data0+$26
		DEC		_star_field_data0+$27
		BNE		loc_60C31
		LDA		_star_field_data1
		BNE		loc_60C29
		LDA		#$1A
		JSR		_hud_msg_set_request
		LDA		#$FF
		STA		_game_over_flag
		RTS
loc_60C29:
		LDA		#$3B
		STA		_star_field_data0+$27
		DEC		_star_field_data1
loc_60C31:
		LDA		#$36
		STA		_dec_word_1E_chr_ofs_delta
		LDA		_star_field_data0+$27
		STA		_tmp1E
		LDA		#0
		STA		_tmp1E+1
		LDX		#<[_tmp_buf2+$10]
		LDY		#>[_tmp_buf2+$10]
		JSR		_hex2str_tmp1E
		LDA		#$F6
		STA		_tmp_buf2+$12
		LDA		_star_field_data1
		STA		_tmp1E
		LDA		#0
		STA		_tmp1E+1
		LDX		#<[_tmp_buf2+$D]
		LDY		#>[_tmp_buf2+$D]
		JSR		_hex2str_tmp1E
		LDX		#7
		LDA		#0
loc_60C5F:
		STA		_tmp_buf2+5,X
		DEX
		BPL		loc_60C5F
		STA		_tmp_buf2+$D
		STA		_tmp_buf2+$E
		STA		_tmp_buf2+$F
		STA		_tmp_buf2+$10
		RTS

; =============== S U B	R O U T	I N E =======================================
_j_hud_lvl4_timer_ppu_data_update:
		JMP		_hud_lvl4_timer_ppu_data_update

; =============== S U B	R O U T	I N E =======================================
_hud_all_energy_ppu_data_update:
		LDA		#$36
		STA		_dec_word_1E_chr_ofs_delta
		LDA		#0
		STA		_tmp1E+1
		CLC
		LDA		_cur_head_percent
		ADC		_cur_body_percent
		ADC		_cur_arms_percent
		ADC		_cur_legs_percent
		STA		_cur_overal_armor_sum
		STA		_tmp1E
		LDX		#<[_tmp_buf2]
		LDY		#>[_tmp_buf2]
		JSR		_hex2str_tmp1E
		LDA		_lvl2_smart_bomb_flag
		BNE		_hud_draw_smartbomb
		LDY		#3
loc_60C9E:
		LDA		_obj_flags,Y
		BNE		loc_60CB5
		DEY
		BNE		loc_60C9E
		LDA		#$40
		STA		_tmp_buf2+$12
		STA		_tmp_buf2+$13
		LDA		#0
		STA		_pal_update_redundant
		JMP		loc_60CD4
loc_60CB5:
		LDA		#$10
		STA		_pal_update_redundant
		LDA		#$63
		LDX		_cur_lvl_idx
		CPX		#4
		BEQ		_j_hud_lvl4_timer_ppu_data_update
		LDX		_last_enemy_hit
		LDA		_tmp3C3,X
		BNE		loc_60CCB
		LDA		_tmp3C3,Y
loc_60CCB:
		STA		_tmp1E
		LDX		#<[_tmp_buf2+$F]
		LDY		#>[_tmp_buf2+$F]
		JSR		_hex2str_tmp1E
loc_60CD4:
		LDX		#4
loc_60CD6:
		LDA		#0
		STA		_tmp_buf2+5,X
		LDA		_tlm_enemy_text,X
		STA		_tmp_buf2+$D,X
		DEX
		BPL		loc_60CD6
		LDA		#0
		STA		_tmp_buf2+$A
		STA		_tmp_buf2+$B
		STA		_tmp_buf2+$C
		STA		_tmp_buf2+$14
		RTS
_hud_draw_smartbomb:
		LDX		#7
loc_60CF5:
		LDA		_tlm_hud_rocket,X
		STA		_tmp_buf2+5,X
		LDA		_tlm_hud_rocket+8,X
		STA		_tmp_buf2+$D,X
		DEX
		BPL		loc_60CF5
		LDA		#$20
		STA		_pal_update_redundant
		DEC		_smart_bomb_timer
		BNE		locret_60D49
		LDA		#$7F
		STA		_pal_blink_flag
		LDA		#0
		STA		_lvl2_smart_bomb_flag
		LDA		#$E
		JSR		_hud_msg_set_request
		LDA		#4
		STA		_snd_idx
		LDA		_obj_lvl_x_hi._player
		CMP		#$E
		BCC		locret_60D49
		CMP		#$F
		BCS		loc_60D30
		LDA		_obj_lvl_x_lo._player
		CMP		#$50
		BCC		locret_60D49
loc_60D30:
		INC		_lvl3_tank_weapon_disable_flag
		CLC
		LDA		_cur_score_hex
		ADC		#$F4
		STA		_cur_score_hex
		LDA		_cur_score_hex+1
		ADC		#1
		STA		_cur_score_hex+1
		LDA		#$15
		JSR		_hud_msg_set_request
locret_60D49:
		RTS

; =============== S U B	R O U T	I N E =======================================
_hud_score_weapon_ppu_data_update:
		LDA		#0
		STA		_dec_word_1E_chr_ofs_delta
		LDA		_cur_score_hex+1
		STA		_tmp1E+1
		LDA		_cur_score_hex
		STA		_tmp1E
		LDX		#<[_tmp_buf2]
		LDY		#>[_tmp_buf2]
		JSR		_hex2str_tmp1E
		LDX		#4
loc_60D62:
		LDY		_tmp_buf2,X
		LDA		_hud_score_num_tile_upper_list,Y
		STA		_tmp_buf2,X
		LDA		_hud_score_num_tile_lower_list,Y
		STA		_tmp_buf2+5,X
		DEX
		BPL		loc_60D62
		LDY		_cur_lvl_idx
		LDA		_hud_weap0_attr0_overlay_list,Y
		STA		_hud_weap0_attr_overlay
		LDA		_hud_weap0_attr1_overlay_list,Y
		STA		_hud_weap0_attr_overlay+1
		LDA		_hud_weap1_attr0_overlay_list,Y
		STA		_hud_weap1_attr_overlay
		LDA		_hud_weap1_attr1_overlay_list,Y
		STA		_hud_weap1_attr_overlay+1
		LDA		_cur_lvl_idx
		ASL
		ASL
		ASL
		TAY
		LDX		#0
loc_60D96:
		LDA		_hud_weap_detault_attrs_lib,Y
		STA		_tmp_buf2+$A,X
		INY
		INX
		CPX		#8
		BCC		loc_60D96
		DEC		_hud_weapon_blink_timer
		LDA		_hud_weapon_blink_timer
		AND		#7
		STA		_hud_weapon_blink_timer
		LDA		_weapon_type
		BNE		loc_60DB9
		LDA		_hud_weapon_blink_timer
		AND		#4
		BEQ		loc_60DD4
loc_60DB9:
		LDY		_weapon0_idx
		LDX		byte_60DFC,Y
		LDA		_tmp_buf2+$A,X
		AND		#$3F
		ORA		_hud_weap0_attr_overlay
		STA		_tmp_buf2+$A,X
		LDA		_tmp_buf2+$B,X
		AND		#$CF
		ORA		_hud_weap0_attr_overlay+1
		STA		_tmp_buf2+$B,X
loc_60DD4:
		LDA		_weapon_type
		BEQ		loc_60DE0
		LDA		_hud_weapon_blink_timer
		AND		#4
		BEQ		locret_60DFB
loc_60DE0:
		LDY		_weapon1_idx
		LDX		byte_60DFF,Y
		LDA		_tmp_buf2+$A,X
		AND		#$F3
		ORA		_hud_weap1_attr_overlay
		STA		_tmp_buf2+$A,X
		LDA		_tmp_buf2+$B,X
		AND		#$FC
		ORA		_hud_weap1_attr_overlay+1
		STA		_tmp_buf2+$B,X
locret_60DFB:
		RTS
byte_60DFC:
		.BYTE	  0,  1,  2
byte_60DFF:
		.BYTE	  4,  5,  6
_hud_weap0_attr0_overlay_list:
		.BYTE	  0,  0,$80,$80,  0
_hud_weap0_attr1_overlay_list:
		.BYTE	  0,  0,$20,$20,  0
_hud_weap1_attr0_overlay_list:
		.BYTE	  0,  0,  8,  8,  0
_hud_weap1_attr1_overlay_list:
		.BYTE	  0,  0,  2,  2,  0
_hud_weap_detault_attrs_lib:
		.BYTE	$F0,$F0,$F0,$F0,$AF,$AF,$AF,$AF
		.BYTE	$F0,$F0,$F0,$F0,$AF,$AF,$AF,$AF
		.BYTE	$F0,$F0,$F0,$F0,$AF,$AF,$AF,$AF
		.BYTE	$F0,$F0,$F0,$F0,$AF,$AF,$AF,$AF
		.BYTE	$F0,$F0,$F0,$F0,$AF,$AF,$AF,$AF

; =============== S U B	R O U T	I N E =======================================
_hud_large_weapon_ppu_data_update:
		LDA		_weapon_type
		BNE		loc_60E48
		LDA		_weapon0_idx
		JMP		loc_60E4D
loc_60E48:
		CLC
		LDA		_weapon1_idx
		ADC		#3
loc_60E4D:
		ASL
		STA		_tmpE2
		ASL
		ASL
		ASL
		CLC
		ADC		_tmpE2
		TAX
		LDY		#0
loc_60E59:
		LDA		_hud_weapon_lagre_icons_lib,X
		STA		_tmp_buf2,Y
		INX
		INY
		CPY		#$12
		BNE		loc_60E59
		RTS
_hud_ppu_data_update_lib:
		.WORD _hud_msg_ppu_data_update
		.WORD _hud_all_energy_ppu_data_update
		.WORD _hud_score_weapon_ppu_data_update
		.WORD _hud_large_weapon_ppu_data_update

; =============== S U B	R O U T	I N E =======================================
_hud_ppu_data_send:
		LDA		_hud_update_ppu_data_counter
		ASL
		TAY
		LDA		_hud_ppu_data_send_lib,Y
		STA		_tmp_ptr0
		LDA		_hud_ppu_data_send_lib+1,Y
		STA		_tmp_ptr0+1
		JMP		(_tmp_ptr0)
_hud_ppu_data_send_lib:
		.WORD _hud_msg_ppu_data_send
		.WORD _hud_all_energy_ppu_data_send
		.WORD _hud_score_weapon_ppu_data_send
		.WORD _hud_large_weapon_ppu_data_send

; =============== S U B	R O U T	I N E =======================================
_hud_all_energy_ppu_data_send:
		LDA		_ppu_ctrl_shadow
		AND		#$FB
		STA		_ppu_ctrl_shadow
		STA		_PPU_CTRL
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$A3
		STA		_PPU_ADDR
		LDA		_tmp_buf2+2
		STA		_PPU_DATA
		LDA		_tmp_buf2+3
		STA		_PPU_DATA
		LDA		_tmp_buf2+4
		STA		_PPU_DATA
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$98
		STA		_PPU_ADDR
		LDA		_tmp_buf2+5
		STA		_PPU_DATA
		LDA		_tmp_buf2+6
		STA		_PPU_DATA
		LDA		_tmp_buf2+7
		STA		_PPU_DATA
		LDA		_tmp_buf2+8
		STA		_PPU_DATA
		LDA		_tmp_buf2+9
		STA		_PPU_DATA
		LDA		_tmp_buf2+$A
		STA		_PPU_DATA
		LDA		_tmp_buf2+$B
		STA		_PPU_DATA
		LDA		_tmp_buf2+$C
		STA		_PPU_DATA
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$B8
		STA		_PPU_ADDR
		LDA		_tmp_buf2+$D
		STA		_PPU_DATA
		LDA		_tmp_buf2+$E
		STA		_PPU_DATA
		LDA		_tmp_buf2+$F
		STA		_PPU_DATA
		LDA		_tmp_buf2+$10
		STA		_PPU_DATA
		LDA		_tmp_buf2+$11
		STA		_PPU_DATA
		LDA		_tmp_buf2+$12
		STA		_PPU_DATA
		LDA		_tmp_buf2+$13
		STA		_PPU_DATA
		LDA		_tmp_buf2+$14
		STA		_PPU_DATA
		LDX		#7
		JSR		_delayX
		NOP
		RTS

; =============== S U B	R O U T	I N E =======================================
_hud_score_weapon_ppu_data_send:
		LDA		_ppu_ctrl_shadow
		AND		#$FB
		STA		_ppu_ctrl_shadow
		STA		_PPU_CTRL
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$41
		STA		_PPU_ADDR
		LDA		_tmp_buf2
		STA		_PPU_DATA
		LDA		_tmp_buf2+1
		STA		_PPU_DATA
		LDA		_tmp_buf2+2
		STA		_PPU_DATA
		LDA		_tmp_buf2+3
		STA		_PPU_DATA
		LDA		_tmp_buf2+4
		STA		_PPU_DATA
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$61
		STA		_PPU_ADDR
		LDA		_tmp_buf2+5
		STA		_PPU_DATA
		LDA		_tmp_buf2+6
		STA		_PPU_DATA
		LDA		_tmp_buf2+7
		STA		_PPU_DATA
		LDA		_tmp_buf2+8
		STA		_PPU_DATA
		LDA		_tmp_buf2+9
		STA		_PPU_DATA
		LDA		#$23
		STA		_PPU_ADDR
		LDA		#$C2
		STA		_PPU_ADDR
		LDA		_tmp_buf2+$A
		STA		_PPU_DATA
		LDA		_tmp_buf2+$B
		STA		_PPU_DATA
		LDA		_tmp_buf2+$C
		STA		_PPU_DATA
		LDA		_tmp_buf2+$D
		STA		_PPU_DATA
		LDA		#$23
		STA		_PPU_ADDR
		LDA		#$CA
		STA		_PPU_ADDR
		LDA		_tmp_buf2+$E
		STA		_PPU_DATA
		LDA		_tmp_buf2+$F
		STA		_PPU_DATA
		LDA		_tmp_buf2+$10
		STA		_PPU_DATA
		LDA		_tmp_buf2+$11
		STA		_PPU_DATA
		LDX		#6
		JSR		_delayX
		NOP
		RTS

; =============== S U B	R O U T	I N E =======================================
_hud_msg_ppu_data_send:
		LDA		_ppu_ctrl_shadow
		AND		#$FB
		STA		_ppu_ctrl_shadow
		STA		_PPU_CTRL
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$C3
		STA		_PPU_ADDR
		LDA		_tmp_buf2
		STA		_PPU_DATA
		LDA		_tmp_buf2+1
		STA		_PPU_DATA
		LDA		_tmp_buf2+2
		STA		_PPU_DATA
		LDA		_tmp_buf2+3
		STA		_PPU_DATA
		LDA		_tmp_buf2+4
		STA		_PPU_DATA
		LDA		_tmp_buf2+5
		STA		_PPU_DATA
		LDA		_tmp_buf2+6
		STA		_PPU_DATA
		LDA		_tmp_buf2+7
		STA		_PPU_DATA
		LDA		_tmp_buf2+8
		STA		_PPU_DATA
		LDA		_tmp_buf2+9
		STA		_PPU_DATA
		LDA		_tmp_buf2+$A
		STA		_PPU_DATA
		LDA		_tmp_buf2+$B
		STA		_PPU_DATA
		LDA		_tmp_buf2+$C
		STA		_PPU_DATA
		LDA		_tmp_buf2+$D
		STA		_PPU_DATA
		LDA		_tmp_buf2+$E
		STA		_PPU_DATA
		LDA		_tmp_buf2+$F
		STA		_PPU_DATA
		LDA		_tmp_buf2+$10
		STA		_PPU_DATA
		LDA		_tmp_buf2+$11
		STA		_PPU_DATA
		LDA		_tmp_buf2+$12
		STA		_PPU_DATA
		LDA		_tmp_buf2+$13
		STA		_PPU_DATA
		LDA		_tmp_buf2+$14
		STA		_PPU_DATA
		LDA		_tmp_buf2+$15
		STA		_PPU_DATA
		LDA		_tmp_buf2+$16
		STA		_PPU_DATA
		LDA		_tmp_buf2+$17
		STA		_PPU_DATA
		LDA		_tmp_buf2+$18
		STA		_PPU_DATA
		LDA		_tmp_buf2+$19
		STA		_PPU_DATA
		LDA		_tmp_buf2+$1A
		STA		_PPU_DATA
		LDA		_tmp_buf2+$1B
		STA		_PPU_DATA
		RTS

; =============== S U B	R O U T	I N E =======================================
_hud_large_weapon_ppu_data_send:
		LDA		_ppu_ctrl_shadow
		AND		#$FB
		STA		_ppu_ctrl_shadow
		STA		_PPU_CTRL
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$58
		STA		_PPU_ADDR
		LDA		_tmp_buf2
		STA		_PPU_DATA
		LDA		_tmp_buf2+1
		STA		_PPU_DATA
		LDA		_tmp_buf2+2
		STA		_PPU_DATA
		LDA		_tmp_buf2+3
		STA		_PPU_DATA
		LDA		_tmp_buf2+4
		STA		_PPU_DATA
		LDA		_tmp_buf2+5
		STA		_PPU_DATA
		LDA		_tmp_buf2+6
		STA		_PPU_DATA
		LDA		_tmp_buf2+7
		STA		_PPU_DATA
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$78
		STA		_PPU_ADDR
		LDA		_tmp_buf2+8
		STA		_PPU_DATA
		LDA		_tmp_buf2+9
		STA		_PPU_DATA
		LDA		_tmp_buf2+$A
		STA		_PPU_DATA
		LDA		_tmp_buf2+$B
		STA		_PPU_DATA
		LDA		_tmp_buf2+$C
		STA		_PPU_DATA
		LDA		_tmp_buf2+$D
		STA		_PPU_DATA
		LDA		_tmp_buf2+$E
		STA		_PPU_DATA
		LDA		_tmp_buf2+$F
		STA		_PPU_DATA
		LDA		#$23
		STA		_PPU_ADDR
		LDA		#$C6
		STA		_PPU_ADDR
		LDA		_tmp_buf2+$10
		STA		_PPU_DATA
		LDA		_tmp_buf2+$11
		STA		_PPU_DATA
		LDX		#8
		JSR		_delayX
		NOP
		NOP
		RTS

; =============== S U B	R O U T	I N E =======================================
_hex2str_tmp1E:
		STX		_ptr1
		STY		_ptr1+1
		LDY		#0
loc_6112A:
		LDX		#$FF
		SEC
loc_6112D:
		INX
		LDA		_tmp1E
		SBC		_dec_list_lo_0,Y
		STA		_tmp1E
		LDA		_tmp1E+1
		SBC		_dec_list_lo_hi,Y
		STA		_tmp1E+1
		BCS		loc_6112D
		LDA		_tmp1E
		ADC		_dec_list_lo_0,Y
		STA		_tmp1E
		LDA		_tmp1E+1
		ADC		_dec_list_lo_hi,Y
		STA		_tmp1E+1
		TXA
		CLC
		ADC		_dec_word_1E_chr_ofs_delta
		STA		(_ptr1),Y
		INY
		CPY		#5
		BNE		loc_6112A
		RTS
_dec_list_lo_0:
		.BYTE	$10,$E8,$64, $A,  1
_dec_list_lo_hi:
		.BYTE	$27,  3,  0,  0,  0

; =============== S U B	R O U T	I N E =======================================
_randomize:
		LDX		#3
		LDA		_rand_seed
loc_61167:
		ADC		_rand_seed,X
		STA		_rand_seed,X
		DEX
		BPL		loc_61167
		RTS

; =============== S U B	R O U T	I N E =======================================
_level_skip_unused:
		INC		_cur_lvl_idx
		PLA
		PLA
		JMP		_level_complete

; =============== S U B	R O U T	I N E =======================================
_main_ingame_handler:
		LDA		#$FF
		STA		_snd_idx
		STA		_ppu_mask_shadow_mask
		JSR		_read_pads
		JSR		_failure_test_handler
		INC		_ingame_sync_counter
		LDA		_pause_flag
		BEQ		loc_611A0
		LDA		_pads
		AND		#$20
		BNE		loc_6118F
loc_6118F:
		LDA		_pads
		AND		#$10
		BEQ		loc_611B8
		AND		_pads+2
		BNE		loc_611B8
		LDA		#0
		STA		_pause_flag
		JMP		loc_611B8
loc_611A0:
		JSR		_lvl_obj_collision_test
		JSR		_obj_misc_collision_handler
		JSR		_lvl_generic_input_control
		LDA		_obj_lvl_y._player
		AND		#$F0
		STA		_player_y_meta16_tile_tmp
		JSR		_lvl_scroll_handlers
		JSR		_obj_spr_manager
		JSR		_lvl_obj_manager
loc_611B8:
		LDA		_cur_lvl_idx
		CMP		#4
		BNE		loc_611C4
		LDA		_PPU_STATUS
		JMP		loc_611CB
loc_611C4:
		LDA		_PPU_STATUS
		AND		#$C0
		BEQ		loc_611C4
loc_611CB:
		LDA		_lvl_cur_spr_chr_bank_down
		JSR		_mmc1_chr1
		JSR		_obj_hidden_passage_test_and_bonus_animate
		LDY		#6
		JSR		_mmc1_prg
		LDA		_pause_flag
		BNE		loc_611FD
		JSR		_hud_update_ppu_data
		JSR		_obj_enemy_ai_manager
		JSR		_obj_proj_position_adjust
		JSR		_player_bullets_animate
		JSR		_enemy_bullet_animate
		JSR		_obj_obj_collision_tests
		JSR		_lvl_bg_chr_switch
		JSR		_player_damage_blink
		JSR		_apu_snd_reload
		JSR		_lvl_complete_test
		JSR		_bg_pal_blink_handler
loc_611FD:
		JSR		_apu_play_prg_safe
		LDX		_cur_lvl_idx
		LDY		_lvl_prg_bank_list,X
		JSR		_mmc1_prg
		RTS

; =============== S U B	R O U T	I N E =======================================
_lvl_complete_test:
		LDA		_end_of_the_level_delay
		BNE		locret_6122C
		LDA		_end_of_the_level_flag
		BEQ		locret_6122C
		LDA		_obj_flags._enemy
		ORA		_obj_flags._enemy+1
		ORA		_obj_flags._enemy+2
		BNE		locret_6122C
		LDA		_game_over_flag
		BNE		locret_6122C
		LDA		_cur_overal_armor_sum
		BEQ		locret_6122C
		INC		_cur_lvl_idx
		PLA
		PLA
		JMP		_level_complete
locret_6122C:
		RTS

; =============== S U B	R O U T	I N E =======================================
_bg_pal_blink_handler:
		LDA		_pal_blink_flag
		BPL		loc_6123A
		LDA		_ingame_bg_pal_buf
		AND		#$30
		BNE		loc_6123A
		RTS
loc_6123A:
		DEC		_pal_blink_flag
		LDY		#$F
loc_6123F:
		LDA		_ingame_bg_pal_buf,Y
		EOR		#$30
		STA		_ingame_bg_pal_buf,Y
		DEY
		BPL		loc_6123F
		RTS

; =============== S U B	R O U T	I N E =======================================
_read_pads:
		LDA		_pads
		STA		_pads+2
		LDY		#1
		STY		_PAD0_REG
		DEY
		STY		_PAD0_REG
		LDY		#8
loc_6125A:
		PHA
		LDA		_PAD0_REG
		STA		_pad_tmp
		LSR
		ORA		_pad_tmp
		LSR
		PLA
		ROL
		DEY
		BNE		loc_6125A
		STA		_pads
		RTS

; =============== S U B	R O U T	I N E =======================================
_lvl_generic_input_control:
		LDA		_obj_lvl_y._player
		CMP		#$F0
		BCS		loc_6127C
		CMP		#$E0
		BCC		loc_6127C
		LDA		#$FF
		STA		_game_over_flag
		RTS
loc_6127C:
		LDA		_cur_lvl_idx
		CMP		#2
		BNE		loc_61285
		JMP		_jetpack_input_control
loc_61285:
		LDA		_lvl4_destruction_counter
		CMP		#$C
		BCC		loc_61290
		LDA		#8
		STA		_pads
loc_61290:
		LDA		#0
		STA		byte_686
		LDA		_cur_overal_armor_sum
		BNE		loc_612A6
		LDA		#0
		STA		_pads
		LDA		_player_sit_jump_flag
		BNE		loc_612A6
		JSR		_player_explosion_anim
		RTS
loc_612A6:
		LDA		_player_scroll_stop_override_tmp
		CMP		#2
		BNE		loc_612CA
		LDA		_obj_lvl_x_hi._player
		CMP		_lvl_stop_sctoll_room_idx
		BEQ		loc_612C6
		LDX		_player_wnd_scroll_counter
		CPX		#$74
		BCC		loc_612C6
		DEC		_player_wnd_scroll_counter
		LDA		_pads
		AND		#$FC
		STA		_pads
		JSR		loc_615FD
		JMP		loc_612CA
loc_612C6:
		LDA		#0
		STA		_player_scroll_stop_override_tmp
loc_612CA:
		LDY		#0
		STY		_tmpE2
		LDX		#0
		LDA		_obj_flags._player
		AND		#$DF
		LDY		_cur_lvl_idx
		CPY		#1
		BNE		loc_612F4
		LDY		_obj_lvl_y._player
		CPY		#$DC
		BCS		loc_612F4
		CPY		#$BC
		BCC		loc_612F4
		ORA		#$20
		LDY		_lvl1_player_pal_acid_lockup_redundant
		STY		_tmpE2
		BNE		loc_612F2
		LDY		#$D
		STY		_snd_idx
		STY		_tmpE2
loc_612F2:
		LDX		#$7A
loc_612F4:
		LDY		_tmpE2
		STY		_lvl1_player_pal_acid_lockup_redundant
		STA		_obj_flags._player
		STX		_player_explode_spr_remove_reload
		LDY		#1
		LDA		_pads
		AND		#3
		STA		_tmpE2
		LDA		_pads+2
		AND		#3
		CMP		_tmpE2
		BEQ		loc_6130E
		LDY		#8
loc_6130E:
		STY		_player_hor_move_anim_increment
		LDA		byte_656
		BEQ		loc_6131C
		DEC		byte_656
		JMP		loc_6132D
loc_6131C:
		LDY		#0
		LDA		_player_cur_bullet_shot_state
		CMP		#$A
		BNE		loc_61326
		LDY		#5
loc_61326:
		STY		_player_cur_bullet_shot_state
		LDA		#$10
		STA		byte_656
loc_6132D:
		LDA		_player_sit_jump_flag
		AND		#1
		BNE		loc_61336
		JMP		loc_613C4
loc_61336:
		LDA		_player_explode_spr_remove_reload
		BNE		loc_61346
		LDA		_player_jump_or_fall_flag
		BEQ		loc_61346
		LDA		_pads
		AND		#$FC
		ORA		_player_jump_or_fall_flag
		STA		_pads
loc_61346:
		LDA		#0
		STA		_player_diagonal_fire_delta
		LDY		_player_cur_bullet_shot_state
		CPY		#$A
		BNE		loc_61352
		LDA		#1
loc_61352:
		CLC
		ADC		#$23
		STA		_obj_spr_idx._player
		LDA		_star_field_data1+$18
		BPL		loc_6135F
		JMP		loc_61514
loc_6135F:
		LDA		_cur_lvl_idx
		CMP		#1
		BNE		loc_61388
		LDA		_obj_lvl_y._player
		CMP		#$D2
		BCS		loc_61388
		CMP		#$BC
		BCC		loc_61388
		LDA		#$BC
		STA		_obj_lvl_y._player
		LDA		_obj_hcoll._player
		ORA		#1
		STA		_obj_hcoll._player
		STA		_obj_vcoll._player
		LDA		#0
		STA		_star_field_data1+$18
		STA		_star_field_data1+$25
		STA		_player_sit_jump_flag
		JMP		loc_613CD
loc_61388:
		LDA		#0
		STA		_player_explode_spr_remove_reload
		LDA		_obj_lvl_y._player
		AND		#$F0
		STA		_tmpE2
		LDA		_player_y_meta16_tile_tmp
		CMP		_tmpE2
		BCC		loc_6139B
		JMP		loc_61514
loc_6139B:
		LDA		_obj_vcoll._player
		ORA		_obj_hcoll._player
		AND		#1
		BNE		loc_613A6
		JMP		loc_61514
loc_613A6:
		LDA		_obj_lvl_y._player
		AND		#$F0
		STA		_obj_lvl_y._player
		LDA		#0
		STA		_star_field_data1+$18
		STA		_star_field_data1+$25
		LDA		#2
		STA		_player_sit_jump_flag
		LDA		#8
		STA		_star_field_data2+$24
		LDA		#$C
		STA		_snd_idx
		JMP		loc_61402
loc_613C4:
		JSR		_lvl1_player_transporter_pos_adjust
		LDA		_player_sit_jump_flag
		AND		#2
		BNE		loc_613D6
loc_613CD:
		LDA		_pads
		CMP		#4
		BEQ		loc_613F2
		JMP		loc_6141F
loc_613D6:
		LDA		_obj_hcoll._player
		ORA		_obj_vcoll._player
		AND		#1
		BEQ		loc_61427
		LDA		_pads
		AND		#4
		BNE		loc_613F2
		DEC		_star_field_data2+$24
		BNE		loc_61402
		LDA		_player_sit_jump_flag
		AND		#$FD
		STA		_player_sit_jump_flag
		JMP		loc_61514
loc_613F2:
		LDA		_player_sit_jump_flag
		ORA		#2
		STA		_player_sit_jump_flag
		LDA		_star_field_data2+$24
		CMP		#9
		BCS		loc_61402
		INC		_star_field_data2+$24
loc_61402:
		LDA		_player_cur_bullet_shot_state
		CMP		#$A
		PHP
		STY		_tmpE2
		LDA		_star_field_data2+$24
		LSR
		LSR
		PLP
		ADC		#$26
		STA		_obj_spr_idx._player
		LDA		#0
		STA		_player_diagonal_fire_delta
		LDA		#$1A
		STA		byte_657
		JMP		loc_61514
loc_6141F:
		LDA		_obj_hcoll._player
		ORA		_obj_vcoll._player
		AND		#1
		BNE		loc_61445
loc_61427:
		LDA		#8
		STA		_star_field_data1+$25
		LDA		_player_sit_jump_flag
		ORA		#1
		STA		_player_sit_jump_flag
		LDA		#$F0
		STA		_player_y_meta16_tile_tmp
		LDY		#2
		LDA		_obj_flags._player
		AND		#$40
		BEQ		loc_61440
		LDY		#1
loc_61440:
		STY		_player_jump_or_fall_flag
		JMP		loc_61514
loc_61445:
		LDA		#0
		STA		_player_sit_jump_flag
		LDA		#$A
		STA		byte_657
		LDA		#0
		STA		_player_diagonal_fire_delta
		LDA		_pads
		AND		#$8F
		BNE		loc_61464
		STA		_player_hor_move_anim_timer
		CLC
		LDA		#$18
		ADC		_player_cur_bullet_shot_state
		STA		_obj_spr_idx._player
		JMP		loc_61514
loc_61464:
		LDA		_pads
		AND		#$83
		BNE		loc_614DA
		LDA		_pads
		AND		#8
		BEQ		loc_614B8
		LDA		_player_hit_bonus_portal_flag
		BEQ		loc_6149C
		LDA		#$25
		STA		_obj_spr_idx._player
		LDA		_cur_lvl_idx
		CMP		#4
		BNE		loc_61487
		LDA		#1
		STA		byte_686
		JMP		locret_6149B
loc_61487:
		DEC		_lvl_door_enrty_delay
		BPL		locret_6149B
		INC		_cur_lvl_state_idx
		PLA
		PLA
		LDA		_cur_lvl_idx
		CMP		#3
		BEQ		loc_61498
		JMP		loc_600D6
loc_61498:
		JMP		_repair_lab_handler
locret_6149B:
		RTS
loc_6149C:
		LDA		#$FC
		STA		_player_diagonal_fire_delta
		LDA		_player_cur_bullet_shot_state
		CMP		#$A
		LDA		#0
		ADC		#$2A
		STA		_obj_spr_idx._player
		LDA		#$FD
		STA		byte_657
		LDA		_pads
		AND		#$FC
		STA		_pads
		JMP		loc_61514
loc_614B8:
		LDA		_pads
		AND		#4
		BEQ		loc_614DA
		LDA		#4
		STA		_player_diagonal_fire_delta
		LDA		_player_cur_bullet_shot_state
		CMP		#$A
		LDA		#0
		ADC		#$2C
		STA		_obj_spr_idx._player
		LDA		#$20
		STA		byte_657
		LDA		_pads
		AND		#$FC
		STA		_pads
		JMP		loc_61514
loc_614DA:
		LDA		_player_hor_move_anim_timer
		AND		#$1F
		LSR
		LSR
		LSR
		CLC
		ADC		#$14
		ADC		_player_cur_bullet_shot_state
		STA		_obj_spr_idx._player
		LDA		_pads
		AND		#$80
		BEQ		loc_61514
		LDA		#8
		STA		_star_field_data1+$25
		LDA		#$FB
		STA		_star_field_data1+$18
		LDA		#5
		STA		_star_field_data2+$A
		LDA		#4
		STA		_star_field_data2+$17
		LDA		_player_sit_jump_flag
		ORA		#1
		STA		_player_sit_jump_flag
		LDA		#$27
		STA		_obj_spr_idx._player
		LDA		#$F0
		STA		_player_y_meta16_tile_tmp
		LDA		#0
		STA		_player_jump_or_fall_flag
loc_61514:
		LDA		_pads
		AND		#$40
		BEQ		loc_6152C
		LDA		_player_cur_bullet_shot_state
		CMP		#$A
		BEQ		loc_6152C
		LDA		#5
		STA		_player_cur_bullet_shot_state
		LDA		#$10
		STA		byte_656
		JSR		_player_fire_control
loc_6152C:
		LDA		_pads
		AND		#$10
		BEQ		loc_6153A
		AND		_pads+2
		BNE		loc_6153A
		LDA		#1
		STA		_pause_flag
loc_6153A:
		LDA		_pads
		AND		#$20
		BEQ		loc_61552
		AND		_pads+2
		BNE		loc_61552
		LDA		_weapon_type
		EOR		#1
		AND		#1
		STA		_weapon_type
		LDA		#$21
		STA		_snd_idx
loc_61552:
		LDA		_player_explode_spr_remove_reload
		BEQ		loc_61560
		LDA		#3
		STA		_body_target_idx
		LDA		#2
		JSR		_player_damage_apply
loc_61560:
		LDA		_player_sit_jump_flag
		AND		#2
		BEQ		loc_61567
		RTS
loc_61567:
		LDA		_cur_lvl_idx
		CMP		#4
		BNE		loc_61570
		JMP		loc_6160D
loc_61570:
		CMP		#3
		BNE		loc_61577
		JMP		_left_move_test
loc_61577:
		LDA		_pads
		AND		#2
		BEQ		loc_615A5
		LDA		_obj_flags._player
		AND		#$BF
		STA		_obj_flags._player
		LDA		_obj_hcoll._player
		AND		#2
		BNE		locret_615A4
		LDA		_player_wnd_scroll_counter
		BEQ		locret_615A4
		DEC		_player_wnd_scroll_counter
		SEC
		LDA		_obj_lvl_x_lo._player
		SBC		#1
		STA		_obj_lvl_x_lo._player
		LDA		_obj_lvl_x_hi._player
		SBC		#0
		STA		_obj_lvl_x_hi._player
		CLC
		LDA		_player_hor_move_anim_timer
		ADC		_player_hor_move_anim_increment
		STA		_player_hor_move_anim_timer
locret_615A4:
		RTS
loc_615A5:
		LDA		_pads
		AND		#1
		BEQ		locret_6160C
		LDA		_obj_flags._player
		ORA		#$40
		STA		_obj_flags._player
		LDA		_obj_hcoll._player
		AND		#4
		BNE		locret_6160C
		CLC
		LDA		_player_hor_move_anim_timer
		ADC		_player_hor_move_anim_increment
		STA		_player_hor_move_anim_timer
		LDA		_player_scroll_stop_override_tmp
		CMP		#1
		BEQ		loc_615D1
		LDA		_obj_lvl_x_hi._player
		CMP		_lvl_stop_sctoll_room_idx
		BCC		loc_615E7
		LDA		_obj_lvl_x_lo._player
		CMP		#$74
		BCC		loc_615E7
loc_615D1:
		LDA		_player_wnd_scroll_counter
		CMP		#$E6
		BCS		locret_6160C
		INC		_player_wnd_scroll_counter
		CLC
		LDA		_obj_lvl_x_lo._player
		ADC		#1
		STA		_obj_lvl_x_lo._player
		LDA		_obj_lvl_x_hi._player
		ADC		#0
		STA		_obj_lvl_x_hi._player
		RTS
loc_615E7:
		CLC
		LDA		_obj_lvl_x_lo._player
		ADC		#1
		STA		_obj_lvl_x_lo._player
		LDA		_obj_lvl_x_hi._player
		ADC		#0
		STA		_obj_lvl_x_hi._player
		LDA		_player_wnd_scroll_counter
		CMP		#$74
		BEQ		loc_615FD
		INC		_player_wnd_scroll_counter
		RTS
loc_615FD:
		CLC
		LDA		_lvl_scroll_value
		ADC		#1
		STA		_lvl_scroll_value
		LDA		_lvl_nt_value
		ADC		#0
		AND		#1
		STA		_lvl_nt_value
locret_6160C:
		RTS
loc_6160D:
		LDA		_pads
		AND		#2
		BEQ		loc_61662
		LDA		_obj_flags._player
		AND		#$BF
		STA		_obj_flags._player
		LDA		_obj_hcoll._player
		AND		#2
		BNE		locret_61661
		LDA		_player_wnd_scroll_counter
		BEQ		locret_61661
		LDA		_obj_lvl_x_hi._player
		BNE		loc_61632
		LDA		_obj_lvl_x_lo._player
		CMP		#$74
		BCS		loc_61632
		DEC		_player_wnd_scroll_counter
		JMP		loc_6164C
loc_61632:
		LDA		_player_wnd_scroll_counter
		CMP		#$74
		BCC		loc_6163D
		DEC		_player_wnd_scroll_counter
		JMP		loc_6164C
loc_6163D:
		SEC
		LDA		_lvl_scroll_value
		SBC		#1
		STA		_lvl_scroll_value
		LDA		_lvl_nt_value
		SBC		#0
		AND		#1
		STA		_lvl_nt_value
loc_6164C:
		SEC
		LDA		_obj_lvl_x_lo._player
		SBC		#1
		STA		_obj_lvl_x_lo._player
		LDA		_obj_lvl_x_hi._player
		SBC		#0
		STA		_obj_lvl_x_hi._player
		CLC
		LDA		_player_hor_move_anim_timer
		ADC		_player_hor_move_anim_increment
		STA		_player_hor_move_anim_timer
locret_61661:
		RTS
loc_61662:
		LDA		_pads
		AND		#1
		BEQ		locret_616C3
		LDA		_obj_flags._player
		ORA		#$40
		STA		_obj_flags._player
		LDA		_obj_hcoll._player
		AND		#4
		BNE		locret_616C3
		CLC
		LDA		_player_hor_move_anim_timer
		ADC		_player_hor_move_anim_increment
		STA		_player_hor_move_anim_timer
		LDA		_obj_lvl_x_hi._player
		CMP		#1
		BCC		loc_6169E
		LDA		_obj_lvl_x_lo._player
		CMP		#$74
		BCC		loc_6169E
		LDA		_player_wnd_scroll_counter
		CMP		#$E6
		BCS		locret_616C3
		INC		_player_wnd_scroll_counter
		CLC
		LDA		_obj_lvl_x_lo._player
		ADC		#1
		STA		_obj_lvl_x_lo._player
		LDA		_obj_lvl_x_hi._player
		ADC		#0
		STA		_obj_lvl_x_hi._player
		RTS
loc_6169E:
		CLC
		LDA		_obj_lvl_x_lo._player
		ADC		#1
		STA		_obj_lvl_x_lo._player
		LDA		_obj_lvl_x_hi._player
		ADC		#0
		STA		_obj_lvl_x_hi._player
		LDA		_player_wnd_scroll_counter
		CMP		#$74
		BEQ		loc_616B4
		INC		_player_wnd_scroll_counter
		RTS
loc_616B4:
		CLC
		LDA		_lvl_scroll_value
		ADC		#1
		STA		_lvl_scroll_value
		LDA		_lvl_nt_value
		ADC		#0
		AND		#1
		STA		_lvl_nt_value
locret_616C3:
		RTS
_player_explosion_anim:
		LDA		_obj_lvl_y._player
		CMP		#$BC
		BCC		loc_616CE
		LDA		#$AA
		STA		_obj_spr_idx._player
loc_616CE:
		LDX		#4
loc_616D0:
		LDA		_obj_flags,X
		BEQ		loc_616DA
		INX
		CPX		#$A
		BCC		loc_616D0
		RTS
loc_616DA:
		LDA		_rand_seed
		AND		#$F
		CLC
		ADC		#4
		ADC		_obj_lvl_x_lo._player
		STA		_obj_lvl_x_lo,X
		LDA		_obj_lvl_x_hi._player
		ADC		#0
		STA		_obj_lvl_x_hi,X
		CLC
		LDA		_obj_lvl_y._player
		ADC		_player_explode_timer_tmp
		SBC		#4
		STA		_obj_lvl_y,X
		LDA		#7
		STA		_obj_hndl_idx,X
		LDA		_player_explode_timer_tmp
		CMP		#$32
		BCS		loc_6172E
		INC		_player_explode_timer_tmp
		LDA		_player_explode_timer_tmp
		LSR
		LSR
		LSR
		STA		_player_explode_spr_remove_reload
		CLC
		LDA		#0
		SBC		_player_explode_spr_remove_reload
		STA		_player_explode_spr_remove_reload
		CPX		#6
		BCS		loc_6171B
		LDA		_rand_seed
		AND		#$10
		BEQ		loc_6171B
		JSR		_enemy_proj1_explode_start
loc_6171B:
		LDA		_rand_seed+1
		AND		#3
		CLC
		ADC		#$10
		STA		_star_field_data2+$24,X
		LDA		#7
		STA		_obj_hndl_idx,X
		STA		_obj_flags,X
		STA		_obj_spr_idx,X
		RTS
loc_6172E:
		DEC		_weapon0_ammo
		BNE		locret_61738
		LDA		#$FF
		STA		_game_over_flag
locret_61738:
		RTS
_jetpack_input_control:
		LDA		_cur_overal_armor_sum
		BNE		loc_6174A
		LDA		#0
		STA		_pads
		LDA		_player_sit_jump_flag
		BNE		loc_6174A
		JSR		_player_explosion_anim
		RTS
loc_6174A:
		LDA		#$14
		STA		_player_explode_spr_remove_reload
		LDY		#1
		LDA		_pads
		AND		#3
		STA		_tmpE2
		LDA		_pads+2
		AND		#3
		CMP		_tmpE2
		BEQ		loc_61760
		LDY		#8
loc_61760:
		STY		_player_hor_move_anim_increment
		LDA		byte_656
		BEQ		loc_6176E
		DEC		byte_656
		JMP		loc_6177F
loc_6176E:
		LDY		#0
		LDA		_player_cur_bullet_shot_state
		CMP		#$A
		BNE		loc_61778
		LDY		#5
loc_61778:
		STY		_player_cur_bullet_shot_state
		LDA		#$10
		STA		byte_656
loc_6177F:
		LDA		_player_sit_jump_flag
		AND		#1
		BNE		loc_61788
		JMP		loc_61834
loc_61788:
		LDA		#0
		STA		_player_diagonal_fire_delta
		LDY		_player_cur_bullet_shot_state
		CPY		#$A
		BNE		loc_61794
		LDA		#1
loc_61794:
		STA		_tmpE2
		LDA		_pads
		AND		#$80
		STA		_tmpE2+1
		BEQ		loc_617CB
		LDA		_lvl3_jetpack_fuel_tanks
		BEQ		loc_617CB
		DEC		_lvl3_jetpack_fuel_count
		BNE		loc_617AB
		DEC		_lvl3_jetpack_fuel_tanks
loc_617AB:
		LDA		_lvl3_jetpack_fuel_tanks
		CMP		#2
		BCS		loc_617BB
		LDA		#$13
		CMP		_hud_msg_idx
		BEQ		loc_617BB
		JSR		_hud_msg_set_request
loc_617BB:
		JSR		_lvl2_obj_jet_update
		LDA		_star_field_data1+$25
		CMP		#$FD
		BEQ		loc_617E1
		DEC		_star_field_data1+$25
		JMP		loc_617E1
loc_617CB:
		LDA		_obj_flags._misc
		BEQ		loc_617D7
		LDA		#0
		STA		_obj_flags._misc
		LDA		#$E
		STA		_snd_idx
loc_617D7:
		LDA		_star_field_data1+$25
		CMP		#8
		BEQ		loc_617E1
		INC		_star_field_data1+$25
loc_617E1:
		CLC
		LDA		#$94
		ADC		_tmpE2
		STA		_obj_spr_idx._player
		LDA		#$C
		STA		_star_field_data2+$17
		LDA		_star_field_data1+$18
		BPL		loc_61805
loc_617F2:
		LDA		#8
		STA		_star_field_data2+$17
		LDA		_obj_lvl_y._player
		CMP		#$3C
		BCS		loc_61802
		LDA		#0
		STA		_star_field_data1+$18
loc_61802:
		JMP		loc_6193B
loc_61805:
		BNE		loc_6180B
		LDA		_tmpE2+1
		BNE		loc_617F2
loc_6180B:
		LDA		_obj_vcoll._player
		ORA		_obj_hcoll._player
		AND		#1
		BNE		loc_61816
		JMP		loc_6193B
loc_61816:
		LDA		_obj_lvl_y._player
		AND		#$F0
		STA		_obj_lvl_y._player
		LDA		#0
		STA		_star_field_data1+$18
		STA		_star_field_data1+$25
		LDA		#2
		STA		_player_sit_jump_flag
		LDA		#8
		STA		_star_field_data2+$24
		LDA		#$B
		STA		_snd_idx
		JMP		loc_6186B
loc_61834:
		LDA		#0
		STA		_obj_flags._misc
		LDA		_player_sit_jump_flag
		AND		#2
		BNE		loc_61847
		LDA		_pads
		CMP		#4
		BEQ		loc_6185B
		JMP		loc_61888
loc_61847:
		LDA		_pads
		AND		#4
		BNE		loc_6185B
		DEC		_star_field_data2+$24
		BNE		loc_6186B
		LDA		_player_sit_jump_flag
		AND		#$FD
		STA		_player_sit_jump_flag
		JMP		loc_6193B
loc_6185B:
		LDA		_player_sit_jump_flag
		ORA		#2
		STA		_player_sit_jump_flag
		LDA		_star_field_data2+$24
		CMP		#9
		BCS		loc_6186B
		INC		_star_field_data2+$24
loc_6186B:
		LDA		_player_cur_bullet_shot_state
		CMP		#$A
		PHP
		STY		_tmpE2
		LDA		_star_field_data2+$24
		LSR
		LSR
		PLP
		ADC		#$A2
		STA		_obj_spr_idx._player
		LDA		#0
		STA		_player_diagonal_fire_delta
		LDA		#$1A
		STA		byte_657
		JMP		loc_6193B
loc_61888:
		LDA		_obj_hcoll._player
		ORA		_obj_vcoll._player
		AND		#1
		BNE		loc_618A2
		LDA		#8
		STA		_star_field_data1+$25
		LDA		_player_sit_jump_flag
		ORA		#1
		STA		_player_sit_jump_flag
		LDA		#$F0
		STA		_player_y_meta16_tile_tmp
		JMP		loc_6193B
loc_618A2:
		LDA		#0
		STA		_player_sit_jump_flag
		LDA		#$A
		STA		byte_657
		LDA		#0
		STA		_player_diagonal_fire_delta
		LDA		_pads
		AND		#$8F
		BNE		loc_618C8
		STA		_player_hor_move_anim_timer
		CLC
		LDA		#$97
		LDY		_player_cur_bullet_shot_state
		CPY		#$A
		BNE		loc_618C3
		CLC
		ADC		#4
loc_618C3:
		STA		_obj_spr_idx._player
		JMP		loc_6193B
loc_618C8:
		LDA		_pads
		AND		#$83
		BNE		loc_61912
		LDA		_pads
		AND		#8
		BEQ		loc_618F0
		LDA		#$FC
		STA		_player_diagonal_fire_delta
		LDA		_player_cur_bullet_shot_state
		CMP		#$A
		LDA		#0
		ADC		#$A6
		STA		_obj_spr_idx._player
		LDA		#$FD
		STA		byte_657
		LDA		_pads
		AND		#$FC
		STA		_pads
		JMP		loc_6193B
loc_618F0:
		LDA		_pads
		AND		#4
		BEQ		loc_61912
		LDA		#4
		STA		_player_diagonal_fire_delta
		LDA		_player_cur_bullet_shot_state
		CMP		#$A
		LDA		#0
		ADC		#$A8
		STA		_obj_spr_idx._player
		LDA		#$20
		STA		byte_657
		LDA		_pads
		AND		#$FC
		STA		_pads
		JMP		loc_6193B
loc_61912:
		LDA		_player_hor_move_anim_timer
		AND		#$1F
		LSR
		LSR
		LSR
		CLC
		ADC		#$96
		LDY		_player_cur_bullet_shot_state
		CPY		#$A
		BNE		loc_61925
		CLC
		ADC		#4
loc_61925:
		STA		_obj_spr_idx._player
		LDA		_pads
		AND		#$80
		BEQ		loc_6193B
		LDA		_player_sit_jump_flag
		ORA		#1
		STA		_player_sit_jump_flag
		LDA		#$F0
		STA		_player_y_meta16_tile_tmp
		LDA		#0
		STA		_player_jump_or_fall_flag
loc_6193B:
		LDA		_pads
		AND		#$40
		BEQ		loc_61953
		LDA		_player_cur_bullet_shot_state
		CMP		#$A
		BEQ		loc_61953
		LDA		#5
		STA		_player_cur_bullet_shot_state
		LDA		#$10
		STA		byte_656
		JSR		_player_fire_control
loc_61953:
		LDA		_pads
		AND		#$10
		BEQ		loc_61961
		AND		_pads+2
		BNE		loc_61961
		LDA		#1
		STA		_pause_flag
loc_61961:
		LDA		_pads
		AND		#$20
		BEQ		loc_61979
		AND		_pads+2
		BNE		loc_61979
		LDA		_weapon_type
		EOR		#1
		AND		#1
		STA		_weapon_type
		LDA		#$21
		STA		_snd_idx
loc_61979:
		JMP		loc_61560
_left_move_test:
		LDA		_pads
		AND		#2
		BEQ		loc_619DC
		LDA		_obj_flags._player
		AND		#$BF
		STA		_obj_flags._player
		LDA		_obj_hcoll._player
		AND		#2
		BNE		locret_619DB
		CLC
		LDA		_player_hor_move_anim_timer
		ADC		_player_hor_move_anim_increment
		STA		_player_hor_move_anim_timer
		LDA		_obj_lvl_x_hi._player
		CMP		_lvl_stop_sctoll_room_idx
		BNE		loc_619B6
		LDA		_obj_lvl_x_lo._player
		CMP		#$75
		BCS		loc_619B6
		LDA		_player_wnd_scroll_counter
		BEQ		locret_619DB
		DEC		_player_wnd_scroll_counter
		SEC
		LDA		_obj_lvl_x_lo._player
		SBC		#1
		STA		_obj_lvl_x_lo._player
		LDA		_obj_lvl_x_hi._player
		SBC		#0
		STA		_obj_lvl_x_hi._player
		RTS
loc_619B6:
		SEC
		LDA		_obj_lvl_x_lo._player
		SBC		#1
		STA		_obj_lvl_x_lo._player
		LDA		_obj_lvl_x_hi._player
		SBC		#0
		STA		_obj_lvl_x_hi._player
		LDA		_player_wnd_scroll_counter
		CMP		#$75
		BCC		loc_619CC
		DEC		_player_wnd_scroll_counter
		RTS
loc_619CC:
		SEC
		LDA		_lvl_scroll_value
		SBC		#1
		STA		_lvl_scroll_value
		LDA		_lvl_nt_value
		SBC		#0
		AND		#1
		STA		_lvl_nt_value
locret_619DB:
		RTS
loc_619DC:
		LDA		_pads
		AND		#1
		BEQ		locret_61A0B
		LDA		_obj_flags._player
		ORA		#$40
		STA		_obj_flags._player
		LDA		_obj_hcoll._player
		AND		#4
		BNE		locret_61A0B
		LDA		_player_wnd_scroll_counter
		CMP		#$E6
		BEQ		locret_61A0B
		INC		_player_wnd_scroll_counter
		CLC
		LDA		_obj_lvl_x_lo._player
		ADC		#1
		STA		_obj_lvl_x_lo._player
		LDA		_obj_lvl_x_hi._player
		ADC		#0
		STA		_obj_lvl_x_hi._player
		CLC
		LDA		_player_hor_move_anim_timer
		ADC		_player_hor_move_anim_increment
		STA		_player_hor_move_anim_timer
locret_61A0B:
		RTS

; =============== S U B	R O U T	I N E =======================================
_lvl2_obj_jet_update:
		LDA		_obj_flags._misc
		BNE		loc_61A29
		LDA		#$A
		STA		_snd_idx
		LDA		#0
		STA		_obj_pal_idx._misc
		STA		_star_field_data1+$22
		STA		_obj_unk._misc
		STA		_star_field_data1+$15
		STA		_star_field_data2+7
		LDA		#$FF
		STA		_obj_hndl_idx._misc
		INC		_obj_flags._misc
loc_61A29:
		CLC
		LDA		_obj_lvl_y._player
		ADC		#$1C
		STA		_obj_lvl_y._misc
		LDY		#2
		LDA		_obj_flags._player
		AND		#$40
		BNE		loc_61A3A
		LDY		#$16
loc_61A3A:
		TYA
		CLC
		ADC		_obj_lvl_x_lo._player
		STA		_obj_lvl_x_lo._misc
		LDA		_obj_lvl_x_hi._player
		ADC		#0
		STA		_obj_lvl_x_hi._misc
		INC		_delay_counter+1
		LDA		_delay_counter+1
		AND		#$F
		LSR
		LSR
		CLC
		ADC		#$9E
		STA		_obj_spr_idx._misc
		RTS

; =============== S U B	R O U T	I N E =======================================
_lvl1_player_transporter_pos_adjust:
		LDA		_cur_lvl_idx
		CMP		#1
		BEQ		loc_61A5D
		RTS
loc_61A5D:
		LDA		#0
		STA		_tmpE2
		LDA		_obj_vcoll._player
		LSR
		ROL		_tmpE2
		LDA		_obj_hcoll._player
		LSR
		ROL		_tmpE2
		LDA		_tmpE2
		CMP		#1
		BEQ		loc_61A76
		CMP		#2
		BEQ		loc_61A90
		RTS
loc_61A76:
		LDA		_obj_hcoll._player
		AND		#2
		BNE		locret_61A8F
		LDA		_player_wnd_scroll_counter
		BEQ		locret_61A8F
		DEC		_player_wnd_scroll_counter
		SEC
		LDA		_obj_lvl_x_lo._player
		SBC		#1
		STA		_obj_lvl_x_lo._player
		LDA		_obj_lvl_x_hi._player
		SBC		#0
		STA		_obj_lvl_x_hi._player
locret_61A8F:
		RTS
loc_61A90:
		LDA		_obj_hcoll._player
		AND		#4
		BNE		locret_61ADD
		LDA		_obj_lvl_x_hi._player
		CMP		_lvl_stop_sctoll_room_idx
		BCC		loc_61AB8
		LDA		_obj_lvl_x_lo._player
		CMP		#$74
		BCC		loc_61AB8
		LDA		_player_wnd_scroll_counter
		CMP		#$E6
		BCS		locret_61ADD
		INC		_player_wnd_scroll_counter
		CLC
		LDA		_obj_lvl_x_lo._player
		ADC		#1
		STA		_obj_lvl_x_lo._player
		LDA		_obj_lvl_x_hi._player
		ADC		#0
		STA		_obj_lvl_x_hi._player
		RTS
loc_61AB8:
		CLC
		LDA		_obj_lvl_x_lo._player
		ADC		#1
		STA		_obj_lvl_x_lo._player
		LDA		_obj_lvl_x_hi._player
		ADC		#0
		STA		_obj_lvl_x_hi._player
		LDA		_player_wnd_scroll_counter
		CMP		#$74
		BEQ		loc_61ACE
		INC		_player_wnd_scroll_counter
		RTS
loc_61ACE:
		CLC
		LDA		_lvl_scroll_value
		ADC		#1
		STA		_lvl_scroll_value
		LDA		_lvl_nt_value
		ADC		#0
		AND		#1
		STA		_lvl_nt_value
locret_61ADD:
		RTS

; =============== S U B	R O U T	I N E =======================================
_player_damage_blink:
		LDA		#3
		LDY		_player_damage_blink_timer
		BEQ		loc_61AE8
		DEC		_player_damage_blink_timer
		LDA		#0
loc_61AE8:
		STA		_obj_pal_idx._player
		RTS

; =============== S U B	R O U T	I N E =======================================
_lvl_obj_collision_test:
		LDX		#3
loc_61AED:
		LDA		#0
		STA		_obj_hcoll,X
		STA		_obj_vcoll,X
		LDA		_obj_lvl_x_hi,X
		STA		_ptr0+1
		LDA		_obj_lvl_x_lo,X
		LSR		_ptr0+1
		ROR
		LSR		_ptr0+1
		ROR
		LSR		_ptr0+1
		ROR
		STA		_player_tile_x
		CLC
		STA		_tmpE2
		ADC		_lvl_collision_data0_ptr
		STA		_ptr0
		LDA		_ptr0+1
		STA		_tmpE2+1
		ADC		_lvl_collision_data0_ptr+1
		STA		_ptr0+1
		CLC
		LDA		_tmpE2
		ADC		_lvl_collision_data1_ptr
		STA		_ptr1
		LDA		_tmpE2+1
		ADC		_lvl_collision_data1_ptr+1
		STA		_ptr1+1
		CLC
		LDA		_tmpE2
		ADC		_lvl_collision_data2_ptr
		STA		_ptr2
		LDA		_tmpE2+1
		ADC		_lvl_collision_data2_ptr+1
		STA		_ptr2+1
		LDA		_obj_lvl_y,X
		CMP		#$40
		BCS		loc_61B37
		JMP		loc_61BBB
loc_61B37:
		CMP		#$BE
		BCC		loc_61B3E
		JMP		loc_61BBB
loc_61B3E:
		SEC
		SBC		#$40
		LSR
		LSR
		LSR
		LSR
		STX		_tmpE2+1
		TAX
		LDA		_bitmask,X
		STA		_tmpE2
		LDX		_tmpE2+1
		LDA		#0
		CPX		#0
		BNE		loc_61B60
		LDY		#3
		ORA		(_ptr0),Y
		DEY
		ORA		(_ptr0),Y
		LDY		#0
		ORA		(_ptr0),Y
loc_61B60:
		LDY		#1
		ORA		(_ptr0),Y
		AND		_tmpE2
		BEQ		loc_61B6E
		LDA		_obj_hcoll,X
		ORA		#1
		STA		_obj_hcoll,X
loc_61B6E:
		LDA		#0
		CPX		#0
		BNE		loc_61B7F
		LDY		#3
		ORA		(_ptr1),Y
		DEY
		ORA		(_ptr1),Y
		LDY		#0
		ORA		(_ptr1),Y
loc_61B7F:
		LDY		#1
		ORA		(_ptr1),Y
		AND		_tmpE2
		BEQ		loc_61B8D
		LDA		_obj_vcoll,X
		ORA		#1
		STA		_obj_vcoll,X
loc_61B8D:
		CPX		#0
		BNE		loc_61B9D
		LDA		_obj_lvl_y._player
		CMP		#$BC
		BCC		loc_61B9D
		LDA		#$80
		STA		_tmpE2
		BNE		loc_61B9F
loc_61B9D:
		LSR		_tmpE2
loc_61B9F:
		LDY		#0
		LDA		(_ptr2),Y
		AND		_tmpE2
		BEQ		loc_61BAD
		LDA		_obj_hcoll,X
		ORA		#2
		STA		_obj_hcoll,X
loc_61BAD:
		LDY		#3
		LDA		(_ptr2),Y
		AND		_tmpE2
		BEQ		loc_61BBB
		LDA		_obj_hcoll,X
		ORA		#4
		STA		_obj_hcoll,X
loc_61BBB:
		DEX
		BMI		locret_61BC1
		JMP		loc_61AED
locret_61BC1:
		RTS
_bitmask:
		.BYTE	  1,  2,  4,  8,$10,$20,$40,$80

; =============== S U B	R O U T	I N E =======================================
_player_fire_control:
		LDA		_player_fire_delay_timer
		BEQ		loc_61BD0
		RTS
loc_61BD0:
		LDA		_weapon0_idx
		LDY		_weapon_type
		BEQ		loc_61BDC
		LDA		_weapon1_idx
		CLC
		ADC		#3
loc_61BDC:
		STA		byte_65A
		TAY
		BEQ		loc_61BF4
		DEY
		BEQ		loc_61BFC
		DEY
		BNE		loc_61BEB
		JMP		loc_61C59
loc_61BEB:
		DEY
		BEQ		loc_61C04
		DEY
		BEQ		loc_61C1F
		JMP		loc_61CFA
loc_61BF4:
		LDX		#3
		LDA		#$F
		STA		_player_fire_delay_reload
		BNE		loc_61C2E
loc_61BFC:
		LDX		#3
		LDA		#6
		STA		_player_fire_delay_reload
		BNE		loc_61C2E
loc_61C04:
		LDY		#0
		LDA		_player_diagonal_fire_delta
		BMI		loc_61C10
		PHP
		INY
		PLP
		BEQ		loc_61C10
		INY
loc_61C10:
		LDA		_obj_flags._player
		AND		#$40
		BNE		loc_61C1B
		TYA
		CLC
		ADC		#3
		TAY
loc_61C1B:
		LDA		byte_61F40,Y
		TAY
loc_61C1F:
		LDA		_obj_flags._pbullet
		BEQ		loc_61C24
		RTS
loc_61C24:
		LDA		#2
		STA		_snd_idx
		LDX		#$F
		LDA		#$1E
		STA		_player_fire_delay_reload
loc_61C2E:
		STX		_tmpE2
		LDX		#4
loc_61C32:
		LDA		_obj_flags,X
		BEQ		loc_61C3C
		INX
		CPX		#7
		BCC		loc_61C32
		RTS
loc_61C3C:
		TYA
		STA		_star_field_data2+$24,X
		LDA		byte_65A
		STA		_obj_hndl_idx,X
		JSR		_player_bullet_insert
		LDA		_player_fire_delay_reload
		STA		_player_fire_delay_timer
		LDA		byte_65A
		CMP		#3
		BCS		locret_61C58
loc_61C54:
		LDA		#1
		STA		_snd_idx
locret_61C58:
		RTS
loc_61C59:
		LDX		#5
		LDA		_obj_flags,X
		BEQ		loc_61C60
		RTS
loc_61C60:
		JSR		_player_bullet_insert
		LDA		#2
		STA		_obj_hndl_idx._pbullet
		STA		_obj_hndl_idx._pbullet+1
		STA		_obj_hndl_idx._pbullet+2
		LDA		_obj_lvl_x_lo._pbullet+1
		STA		_obj_lvl_x_lo._pbullet
		STA		_obj_lvl_x_lo._pbullet+2
		LDA		_obj_lvl_x_hi._pbullet+1
		STA		_obj_lvl_x_hi._pbullet
		STA		_obj_lvl_x_hi._pbullet+2
		LDA		_obj_lvl_y._pbullet+1
		STA		_obj_lvl_y._pbullet
		STA		_obj_lvl_y._pbullet+2
		LDA		_star_field_data1+$10
		STA		_star_field_data1+$F
		STA		_star_field_data1+$11
		STA		_obj_unk._pbullet
		STA		_obj_unk._pbullet+2
		LDA		#3
		STA		_obj_spr_idx._pbullet
		STA		_obj_spr_idx._pbullet+1
		STA		_obj_spr_idx._pbullet+2
		LDY		#0
		LDA		_player_diagonal_fire_delta
		BMI		loc_61CA0
		LDY		#3
		LDA		_player_diagonal_fire_delta
		BEQ		loc_61CA0
		LDY		#6
loc_61CA0:
		LDA		byte_61CF1,Y
		STA		_star_field_data2+1
		STA		_star_field_data1+$1C
		LDA		byte_61CF1+1,Y
		STA		_star_field_data2+2
		STA		_star_field_data1+$1D
		LDA		byte_61CF1+2,Y
		STA		_star_field_data2+3
		STA		_star_field_data1+$1E
		LDA		_obj_unk._pbullet+1
		AND		byte_61CE8,Y
		STA		_obj_unk._pbullet
		STA		_star_field_data1+$F
		LDA		_obj_unk._pbullet+1
		AND		byte_61CE8+1,Y
		STA		_obj_unk._pbullet+1
		STA		_star_field_data1+$10
		LDA		_obj_unk._pbullet+1
		AND		byte_61CE8+2,Y
		STA		_obj_unk._pbullet+2
		STA		_star_field_data1+$11
		LDA		#0
		STA		_obj_pal_idx._pbullet
		STA		_obj_pal_idx._pbullet+2
		LDA		#1
		STA		_obj_flags._pbullet
		STA		_obj_flags._pbullet+2
		JMP		loc_61C54
byte_61CE8:
		.BYTE	  0,$FF,$FF
		.BYTE	$FF,$FF,$FF
		.BYTE	$FF,$FF,  0
byte_61CF1:
		.BYTE	$FC,$FC,  0
		.BYTE	$FC,  0,  4
		.BYTE	  0,  4,  4
loc_61CFA:
		LDX		#2
loc_61CFC:
		LDA		_obj_flags._enemy,X
		BEQ		loc_61D11
		LDA		#0
		STA		_tmp3C3+1,X
		LDA		#8
		STA		_star_field_data1+$C,X
		STA		_obj_unk._enemy,X
		LDA		#0
		STA		_tmp3C3+1,X
loc_61D11:
		DEX
		BPL		loc_61CFC
		DEC		byte_663
		BNE		loc_61D1B
		DEC		_weapon1_idx
loc_61D1B:
		LDA		#3
		STA		_snd_idx
		LDA		#$32
		STA		_pal_blink_flag
		LDA		#$28
		STA		_player_fire_delay_reload
		STA		_player_fire_delay_timer
		RTS

; =============== S U B	R O U T	I N E =======================================
_player_bullet_insert:
		STX		_tmpE7
		LDA		_weapon_type
		BNE		loc_61D41
		LDX		_weapon0_idx
		BEQ		loc_61D4C
		DEC		_weapon0_ammo,X
		BNE		loc_61D4C
		DEC		_weapon0_idx
		JMP		loc_61D4C
loc_61D41:
		LDX		_weapon1_idx
		BEQ		loc_61D4C
		DEC		_weapon1_ammo,X
		BNE		loc_61D4C
		DEC		_weapon1_idx
loc_61D4C:
		LDX		_tmpE7
		LDA		#$A
		STA		_player_cur_bullet_shot_state
		LDA		#4
		STA		byte_656
		LDA		#0
		STA		_obj_pal_idx,X
		CLC
		LDA		_obj_lvl_y._player
		ADC		byte_657
		STA		_obj_lvl_y,X
		LDA		_player_diagonal_fire_delta
		STA		_star_field_data1+$25,X
		STA		_star_field_data1+$18,X
		LDA		_tmpE2
		STA		_obj_spr_idx,X
		LDA		_obj_flags._player
		AND		#$40
		PHP
		ORA		#1
		STA		_obj_flags,X
		PLP
		BEQ		loc_61D90
		LDA		#4
		STA		_star_field_data1+$B,X
		STA		_obj_unk,X
		CLC
		LDA		_obj_lvl_x_lo._player
		ADC		#$1E
		STA		_obj_lvl_x_lo,X
		LDA		_obj_lvl_x_hi._player
		ADC		#0
		STA		_obj_lvl_x_hi,X
		RTS
loc_61D90:
		LDA		#$FC
		STA		_star_field_data1+$B,X
		STA		_obj_unk,X
		SEC
		LDA		_obj_lvl_x_lo._player
		SBC		#6
		STA		_obj_lvl_x_lo,X
		LDA		_obj_lvl_x_hi._player
		SBC		#0
		STA		_obj_lvl_x_hi,X
		RTS

; =============== S U B	R O U T	I N E =======================================
_player_bullets_animate:
		LDA		_player_fire_delay_timer
		BEQ		loc_61DAD
		DEC		_player_fire_delay_timer
loc_61DAD:
		LDX		#4
loc_61DAF:
		LDA		_obj_flags,X
		BEQ		_player_next_bullet
		LDA		_obj_lvl_y,X
		CMP		#$D8
		BCS		loc_61DE7
		CMP		#$3C
		BCS		loc_61DC6
		JSR		_player_bullet_remove
		LDA		#0
		STA		_obj_flags,X
		BEQ		_player_next_bullet
loc_61DC6:
		LDY		_obj_hndl_idx,X
		BMI		_player_next_bullet
		BEQ		loc_61E05
		DEY
		BEQ		loc_61E08
		DEY
		BEQ		loc_61E0B
		DEY
		BEQ		loc_61E0E
		DEY
		BEQ		_player_bullet_anim_hit_test
		DEY
		BEQ		loc_61E14
		DEY
		BEQ		loc_61E17
loc_61DDE:
		JSR		_player_death_anim_timers
_player_next_bullet:
		INX
		CPX		#7
		BNE		loc_61DAF
		RTS
loc_61DE7:
		LDA		_obj_hndl_idx,X
		CMP		#7
		BEQ		loc_61DC6
		JSR		_player_bullet_remove
		JSR		_enemy_proj0_explode_start
		JMP		loc_61DDE
_player_bullet_remove:
		LDA		_obj_hndl_idx,X
		CMP		#3
		BCC		locret_61E04
		CMP		#5
		BCS		locret_61E04
		LDA		#0
		STA		_obj_flags._pbullet+1
locret_61E04:
		RTS
loc_61E05:
		JMP		_player_next_bullet
loc_61E08:
		JMP		_player_next_bullet
loc_61E0B:
		JMP		_player_next_bullet
loc_61E0E:
		LDY		_star_field_center_pos_x
		JMP		loc_61E99
loc_61E14:
		JMP		_player_next_bullet
loc_61E17:
		JMP		_player_next_bullet
_player_bullet_anim_hit_test:
		LDY		_star_field_center_pos_x
		BEQ		loc_61E29
		LDA		_obj_flags,Y
		BEQ		loc_61E29
		LDA		_star_field_data0+$19,Y
		BNE		loc_61E38
loc_61E29:
		LDY		#3
loc_61E2B:
		LDA		_obj_flags,Y
		BEQ		loc_61E35
		LDA		_star_field_data0+$19,Y
		BNE		loc_61E38
loc_61E35:
		DEY
		BNE		loc_61E2B
loc_61E38:
		STY		_star_field_center_pos_x
		LDA		#<[_pal_buf+4]
		STA		_tmpE2
		LDA		#>[_pal_buf+4]
		STA		_tmpE2+1
		SEC
		LDA		_obj_lvl_x_lo,Y
		SBC		_obj_lvl_x_lo._pbullet
		LDA		_obj_lvl_x_hi,Y
		SBC		_obj_lvl_x_hi._pbullet
		BCS		loc_61E54
		LDA		#$FC
		STA		_tmpE2
loc_61E54:
		CLC
		LDA		_obj_lvl_y,Y
		ADC		#$1E
		SEC
		SBC		_obj_lvl_y._pbullet
		PHP
		LDA		_obj_lvl_y,Y
		AND		#$F8
		STA		_tmpE4
		LDA		_obj_lvl_y._pbullet
		AND		#$F8
		CMP		_tmpE4
		BNE		loc_61E74
		PLP
		LDA		#0
		STA		_tmpE2+1
		BEQ		loc_61E7B
loc_61E74:
		PLP
		BCS		loc_61E7B
		LDA		#$FD
		STA		_tmpE2+1
loc_61E7B:
		LDA		_tmpE2
		STA		_star_field_data1+$F
		LDA		_tmpE2+1
		STA		_star_field_data2+1
		LDY		#0
		LDA		_obj_unk._pbullet
		BPL		loc_61E8D
		LDY		#9
loc_61E8D:
		STY		_tmpE2
		CLC
		LDA		_star_field_data1+$1C
		ADC		#4
		CLC
		ADC		_tmpE2
		TAY
loc_61E99:
		LDA		byte_61EF8,Y
		STA		_obj_spr_idx._pbullet
		LDA		byte_61F0A,Y
		STA		_obj_flags._pbullet
		LDA		byte_61F1C,Y
		BMI		loc_61EB6
		CLC
		ADC		_obj_lvl_x_lo._pbullet
		STA		_obj_lvl_x_lo._pbullet+1
		LDA		_obj_lvl_x_hi._pbullet
		ADC		#0
		STA		_obj_lvl_x_hi._pbullet+1
		JMP		loc_61ECA
loc_61EB6:
		EOR		#$FF
		CLC
		ADC		#1
		STA		_tmpE2
		SEC
		LDA		_obj_lvl_x_lo._pbullet
		SBC		_tmpE2
		STA		_obj_lvl_x_lo._pbullet+1
		LDA		_obj_lvl_x_hi._pbullet
		SBC		#0
		STA		_obj_lvl_x_hi._pbullet+1
loc_61ECA:
		CLC
		LDA		byte_61F2E,Y
		ADC		_obj_lvl_y._pbullet
		STA		_obj_lvl_y._pbullet+1
		DEC		_mul_argB
		LDA		_mul_argB
		LSR
		LSR
		AND		#1
		CLC
		ADC		#4
		STA		_obj_spr_idx._pbullet+1
		LDA		#1
		STA		_obj_flags._pbullet+1
		LDA		#0
		STA		_obj_pal_idx._pbullet+1
		LDA		#$FF
		STA		_obj_hndl_idx._pbullet+1
		LDA		_obj_unk._pbullet
		STA		_obj_unk._pbullet+1
		LDA		#3
		STA		_obj_pal_idx._pbullet
		JMP		_player_next_bullet
byte_61EF8:
		.BYTE	 $B, $C, $D, $E, $F, $E, $D, $C, $B, $B, $C, $D, $E, $F, $E, $D
		.BYTE	 $C, $B
byte_61F0A:
		.BYTE	  1,  1,  1,  1,  1,$81,$81,$81,$81,  1,$41,$41,$41,$C1,$C1,$C1
		.BYTE	$81,$81
byte_61F1C:
		.BYTE	  0,$FE,$FC,$FB,$FA,$FB,$FC,$FE,  0,  0,  2,  4,  5,  6,  5,  4
		.BYTE	  2,  0
byte_61F2E:
		.BYTE	  6,  5,  4,  2,  0,$FE,$FC,$FB,$FA,  6,  5,  4,  2,  0,$FE,$FC
		.BYTE	$FB,$FA
byte_61F40:
		.BYTE	  2,  4,  6, $B, $D, $F

; =============== S U B	R O U T	I N E =======================================
_lvl_obj_manager:
		LDA		_end_of_the_level_flag
		BNE		loc_61F87
		LDY		#0
		LDA		(_lvl_obj_list_ptr),Y
		CMP		#$FF
		BEQ		loc_61F84
		CMP		_player_tile_x
		BEQ		_obj_add
		RTS
loc_61F59:
		CMP		#$FF
		BNE		loc_61F6A
		LDY		#2
		LDA		(_lvl_obj_list_ptr),Y
		STA		_lvl_phase_bg_chr_banks_list
		STA		_lvl_phase_bg_chr_banks_list+1
		STA		_lvl_phase_bg_chr_banks_list+2
		JMP		loc_6208F
loc_61F6A:
		LDY		#2
		LDA		(_lvl_obj_list_ptr),Y
		ASL
		ASL
		ASL
		ASL
		TAY
		LDX		#0
loc_61F75:
		LDA		byte_63C4D,Y
		STA		_ingame_bg_pal_buf,X
		INY
		INX
		CPX		#$10
		BCC		loc_61F75
		JMP		loc_6208F
loc_61F84:
		INC		_end_of_the_level_flag
loc_61F87:
		LDA		_end_of_the_level_delay
		BNE		loc_61F8D
		RTS
loc_61F8D:
		LDX		#2
loc_61F8F:
		LDA		_obj_flags,X
		BEQ		loc_61F97
		DEX
		BNE		loc_61F8F
		RTS
loc_61F97:
		LDA		_cur_lvl_idx
		ASL
		STA		_tmpE4
		LDA		_rand_seed
		AND		#1
		ORA		_tmpE4
		TAY
		LDA		byte_63C43,Y
		STA		_tmpE4
		LDY		_cur_lvl_idx
		LDA		#$70
		STA		_tmpE2+1
		LDA		#$50
		STA		_tmpE6
		LDA		byte_63C3E,Y
		STA		_tmpE7
		LDY		#$A
		LDA		_rand_seed
		BMI		loc_61FBF
		LDY		#$88
loc_61FBF:
		STY		_tmpE2
		DEC		_end_of_the_level_delay
		JMP		loc_6200B

; =============== S U B	R O U T	I N E =======================================
_obj_add:
		LDY		#1
		LDA		(_lvl_obj_list_ptr),Y
		BMI		loc_61F59
		LDA		_obj_lvl_x_lo._player
		STA		_tmpE6
		LDA		_obj_lvl_x_hi._player
		STA		_tmpE7
		LDY		#3
		LDA		(_lvl_obj_list_ptr),Y
		STA		_tmpE2
		DEY
		LDA		(_lvl_obj_list_ptr),Y
		STA		_tmpE2+1
		DEY
		LDA		(_lvl_obj_list_ptr),Y
		STA		_tmpE4
		CMP		#$21
		BCS		loc_61FF5
		LDX		#3
loc_61FEB:
		LDA		_obj_flags,X
		BEQ		loc_6200B
		DEX
		BNE		loc_61FEB
		JMP		loc_6208F
loc_61FF5:
		CMP		#$28
		BCC		loc_62001
		LDX		#$C
		STA		_obj_hndl_idx,X
		TAY
		JMP		loc_6202C
loc_62001:
		CMP		#$24
		BNE		loc_62009
		LDX		#$A
		BNE		loc_6200B
loc_62009:
		LDX		#$B
loc_6200B:
		LDA		_tmpE4
		STA		_obj_hndl_idx,X
		TAY
		LDA		_tmpE2+1
		CLC
		ADC		#$40
		STA		_obj_lvl_y,X
		LDA		_obj_pal_lib,Y
		STA		_obj_pal_idx,X
		LDA		_obj_spr_lib,Y
		STA		_obj_spr_idx,X
		LDA		byte_63BD0,Y
		STA		_tmp3C3,X
		LDA		_obj_initi_flags_lib,Y
		STA		_obj_flags,X
loc_6202C:
		LDA		_cur_lvl_idx
		CMP		#3
		BNE		loc_62042
		SEC
		LDA		_tmpE6
		SBC		#$84
		STA		_obj_lvl_x_lo,X
		LDA		_tmpE7
		SBC		#0
		STA		_obj_lvl_x_hi,X
		JMP		loc_6204F
loc_62042:
		CLC
		LDA		_tmpE6
		ADC		#$8E
		STA		_obj_lvl_x_lo,X
		LDA		_tmpE7
		ADC		#0
		STA		_obj_lvl_x_hi,X
loc_6204F:
		LDA		_tmpE2
		BMI		loc_62062
		ASL
		CLC
		ADC		_obj_lvl_x_lo,X
		STA		_obj_lvl_x_lo,X
		LDA		_obj_lvl_x_hi,X
		ADC		#0
		STA		_obj_lvl_x_hi,X
		JMP		loc_62077
loc_62062:
		EOR		#$FF
		CLC
		ADC		#1
		ASL
		STA		_tmpE2
		SEC
		LDA		_obj_lvl_x_lo,X
		SBC		_tmpE2
		STA		_obj_lvl_x_lo,X
		LDA		_obj_lvl_x_hi,X
		SBC		#0
		STA		_obj_lvl_x_hi,X
loc_62077:
		LDA		#0
		STA		_star_field_data1+$B,X
		STA		_obj_unk,X
		STA		_star_field_data1+$25,X
		STA		_star_field_data1+$18,X
		STA		_star_field_data2+$24,X
		STA		_chr1_bank_shadow,X
		LDA		#1
		STA		_star_field_data0+$19,X
loc_6208F:
		CLC
		LDA		_lvl_obj_list_ptr
		ADC		#4
		STA		_lvl_obj_list_ptr
		LDA		_lvl_obj_list_ptr+1
		ADC		#0
		STA		_lvl_obj_list_ptr+1
		RTS

; =============== S U B	R O U T	I N E =======================================
_obj_enemy_ai_manager:
		LDX		#3
_obj_enemy_ai_loop:
		LDA		_obj_flags,X
		BEQ		_obj_next_enemy_ai
		LDA		_obj_hndl_idx,X
		ASL
		TAY
		LDA		_obj_enemy_ai_handlers_lib,Y
		STA		_tmp_ptr0
		LDA		_obj_enemy_ai_handlers_lib+1,Y
		STA		_tmp_ptr0+1
		JMP		(_tmp_ptr0)
_obj_next_enemy_ai:
		DEX
		BNE		_obj_enemy_ai_loop
		JSR		_obj_tank_weapon_handler
		RTS
_obj_enemy_ai_handlers_lib:
		.WORD _obj_ehndl00_regular_gunman0
		.WORD _obj_ehndl01_bomb_thrower0
		.WORD _obj_ehndl02_hiding_barrel_lvl0
		.WORD _obj_ehndl03_grenader0
		.WORD _obj_ehndl04_bomb_thrower1
		.WORD _obj_ehndl05_regular_gunman1
		.WORD _obj_ehndl06_bomb_thrower1
		.WORD _obj_ehndl07_hiding_barrel_lvl0_bonus
		.WORD _obj_ehndl08_grenader1
		.WORD _obj_ehndl09_bomb_thrower2
		.WORD _obj_ehndl0A_ninja
		.WORD _obj_ehndl0B_hiding_barrel_lvl1
		.WORD _obj_ehndl0C_unused
		.WORD _obj_ehndl0D_regular_gunman2
		.WORD _obj_ehndl0E_regular_gunman3
		.WORD _obj_ehndl0F_grenader2
		.WORD _obj_ehndl10_hanging_gunman0
		.WORD _obj_ehndl13_hanging_gunman1
		.WORD _obj_ehndl10_hanging_gunman0
		.WORD _obj_ehndl13_hanging_gunman1
		.WORD _obj_ehndl14_walking_grenader0
		.WORD _obj_ehndl15_walker_droid
		.WORD _obj_ehndl16_ground_timed_bomb
		.WORD _obj_ehndl17_walking_grenader1
		.WORD _obj_ehndl18_rifle_man_lvl2
		.WORD _obj_ehndl19_hiding_barrel_lvl2
		.WORD _obj_ehndl1A_ground_still_bomb0

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl0D_regular_gunman2:
		LDA		#$71
		LDY		#$71
		BNE		_obj_ehndl_regular_gunman

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl0E_regular_gunman3:
		LDA		#$77
		LDY		#$77
		BNE		_obj_ehndl_regular_gunman

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl00_regular_gunman0:
		LDA		#$2E
		LDY		#$38
		BNE		_obj_ehndl_regular_gunman

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl05_regular_gunman1:
		LDA		#$2F
		LDY		#$3E

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl_regular_gunman:
		STA		_tmpE6
		STY		_tmpE2+1
		INY
		INY
		INY
		INY
		STY		_tmpE4
		INY
		STY		_tmpE5
		LDA		_tmp3C3,X
		BNE		loc_62122
		LDA		_tmpE6
		STA		_obj_spr_idx,X
		JMP		loc_62228
loc_62122:
		LDA		_obj_hcoll,X
		AND		#1
		BNE		loc_62130
		LDA		_star_field_data1+$25,X
		BEQ		loc_6217E
		JMP		loc_62228
loc_62130:
		LDA		_obj_lvl_y,X
		CMP		#$A0
		BCS		loc_62139
		JMP		loc_62228
loc_62139:
		LDA		#0
		STA		_star_field_data1+$25,X
		STA		_star_field_data1+$18,X
		LDA		_obj_lvl_y,X
		AND		#$F0
		STA		_obj_lvl_y,X
		LDA		_obj_unk,X
		BNE		loc_62168
		DEC		_chr1_bank_shadow,X
		BMI		loc_62153
		JMP		loc_62228
loc_62153:
		LDA		_obj_spr_idx,X
		CMP		_tmpE5
		BNE		loc_62195
		JSR		_enemy_projectile_collision_test
		LDA		_rand_seed,X
		AND		#$F
		ORA		#$20
		STA		_chr1_bank_shadow,X
		JMP		loc_6217E
loc_62168:
		INC		_star_field_data2+$24,X
		LDA		_star_field_data2+$24,X
		AND		#$1F
		LSR
		LSR
		LSR
		CLC
		ADC		_tmpE2+1
		STA		_obj_spr_idx,X
		LDA		_obj_hcoll,X
		AND		#6
		BEQ		loc_62195
loc_6217E:
		LDY		#1
		LDA		_obj_flags,X
		EOR		#$40
		STA		_obj_flags,X
		AND		#$40
		BNE		loc_6218C
		LDY		#$FF
loc_6218C:
		TYA
		STA		_star_field_data1+$B,X
		STA		_obj_unk,X
		JMP		loc_62228
loc_62195:
		LDA		_chr1_bank_shadow,X
		BPL		loc_621C4
		LDA		_rand_seed,X
		AND		#$1F
		ADC		#$1E
		STA		_chr1_bank_shadow,X
		JSR		_enemy_projectile_collision_test_ex
		LDA		#$FF
		STA		_tmpE2
		LDA		_obj_flags,X
		AND		#$BF
		BCC		loc_621B8
		ORA		#$40
		PHA
		LDA		#1
		STA		_tmpE2
		PLA
loc_621B8:
		STA		_obj_flags,X
		LDA		_tmpE2
		STA		_star_field_data1+$B,X
		STA		_obj_unk,X
		JMP		loc_62228
loc_621C4:
		DEC		_chr1_bank_shadow,X
		LDA		_chr1_bank_shadow,X
		CMP		#$A
		BCS		loc_62228
		JSR		_enemy_projectile_collision_test_ex
		TYA
		BNE		loc_62228
		LDA		_obj_flags,X
		AND		#2
		BEQ		loc_621F9
		LDA		_tmpE2
		CMP		#$16
		BCS		loc_62228
		CMP		#$12
		BCC		loc_62228
		JSR		_enemy_projectile_collision_test
		LDA		#3
		STA		_body_target_idx
		LDA		#1
		JSR		_player_damage_apply
		LDA		_tmpE5
		STA		_obj_spr_idx,X
		LDA		#$A
		BNE		loc_6221E
loc_621F9:
		LDA		_rand_seed,X
		CMP		#$50
		BCS		loc_62228
		JSR		_enemy_projectile_collision_test
		LDY		#$11
		LDA		#$10
		STA		_enemy_bullet_y_pos_global_arg
		LDA		#0
		STA		_enemy_bullet_body_target_select_arg
		LDA		#0
		JSR		_enemy_bullet_insert
		BCS		loc_62228
		LDA		#$F
		STA		_snd_idx
		LDA		_tmpE4
		STA		_obj_spr_idx,X
		LDA		#$F
loc_6221E:
		STA		_chr1_bank_shadow,X
		LDA		#0
		STA		_star_field_data1+$B,X
		STA		_obj_unk,X
loc_62228:
		JMP		_obj_next_enemy_ai

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl01_bomb_thrower0:
		LDY		#$44
		BNE		_obj_ehndl_bomb_thrower

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl04_bomb_thrower1:
		LDY		#$48
		BNE		_obj_ehndl_bomb_thrower

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl06_bomb_thrower1:
		LDY		#$4C
		BNE		_obj_ehndl_bomb_thrower

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl09_bomb_thrower2:
		LDY		#$50
		BNE		_obj_ehndl_bomb_thrower

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl_bomb_thrower:
		STY		_tmpE2
		INY
		STY		_tmpE2+1
		INY
		STY		_tmpE4
		INY
		STY		_tmpE5
		LDA		_tmp3C3,X
		BNE		loc_62252
		LDA		#$2E
		STA		_obj_spr_idx,X
		JMP		loc_6235C
loc_62252:
		DEC		_chr1_bank_shadow,X
		BMI		loc_6225A
		JMP		loc_6235C
loc_6225A:
		LDA		#5
		STA		_chr1_bank_shadow,X
		LDA		_star_field_data2+$24,X
		BNE		loc_62280
		LDA		_obj_spr_idx,X
		CMP		_tmpE2
		BEQ		loc_6226F
		DEC		_obj_spr_idx,X
		JMP		loc_6235C
loc_6226F:
		LDA		_rand_seed,X
		CMP		#$A
		BCC		loc_62278
		JMP		loc_6235C
loc_62278:
		LDA		#1
		STA		_star_field_data2+$24,X
		JMP		loc_6235C
loc_62280:
		LDA		_obj_spr_idx,X
		CMP		_tmpE4
		BCS		loc_6228B
		INC		_obj_spr_idx,X
		JMP		loc_6235C
loc_6228B:
		JSR		_enemy_projectile_collision_test
		LDA		_tmpE4
		STA		_obj_spr_idx,X
		LDA		_rand_seed,X
		CMP		#7
		BCS		loc_622A0
		LDA		#0
		STA		_star_field_data2+$24,X
		JMP		loc_6235C
loc_622A0:
		JSR		_enemy_projectile_collision_test_ex
		BCC		loc_622DC
		LDA		_cur_lvl_idx
		CMP		#2
		BCS		loc_622DC
		LDA		#0
		STA		_tmpE2
		LDY		#3
loc_622B1:
		LDA		_obj_flags,Y
		BEQ		loc_622BF
		LDA		_obj_lvl_y,Y
		CMP		#$8C
		BCC		loc_622BF
		INC		_tmpE2
loc_622BF:
		DEY
		BNE		loc_622B1
		LDA		_tmpE2
		BNE		loc_622DC
		LDA		#6
		STA		_star_field_data1+$25,X
		LDA		#0
		STA		_obj_hndl_idx,X
		LDA		#$2E
		STA		_obj_spr_idx,X
		LDA		_obj_flags,X
		ORA		#$40
		STA		_obj_flags,X
		JMP		_obj_next_enemy_ai
loc_622DC:
		LDA		_obj_hndl_idx,X
		CMP		#4
		BEQ		loc_6230C
		CMP		#9
		BEQ		loc_6230C
		LDA		_rand_seed,X
		CMP		#$32
		BCS		loc_6235C
		TYA
		BEQ		loc_6235C
		LDY		#$11
		LDA		#$16
		STA		_enemy_bullet_y_pos_global_arg
		LDA		#0
		STA		_enemy_bullet_body_target_select_arg
		LDA		#5
		JSR		_enemy_bullet_insert
		BCS		loc_6235C
		LDA		#$F
		STA		_snd_idx
		LDA		_tmpE5
		STA		_obj_spr_idx,X
		JMP		loc_6235C
loc_6230C:
		LDA		_rand_seed,X
		CMP		#$1E
		BCS		loc_6235C
		LDY		#$10
		LDA		#0
		STA		_enemy_bullet_y_pos_global_arg
		LDA		#1
		STA		_enemy_bullet_body_target_select_arg
		LDA		#$C
		JSR		_enemy_bullet_insert
		BCS		loc_6235C
		LDA		#$FE
		STA		_tmpE2
		LDA		#$FC
		STA		_tmpE2+1
		LDA		_obj_flags,X
		AND		#$40
		BEQ		loc_6233A
		LDA		#2
		STA		_tmpE2
		LDA		#4
		STA		_tmpE2+1
loc_6233A:
		LDA		_tmpE2
		STA		_star_field_data1+$B,Y
		LDA		_tmpE2+1
		STA		_obj_unk,Y
		LDA		#$FD
		STA		_star_field_data1+$18,Y
		LDA		#3
		STA		_obj_pal_idx,Y
		LDA		#3
		STA		_star_field_data2+$17,Y
		LDA		_tmpE5
		STA		_obj_spr_idx,X
		LDA		#$1E
		STA		_chr1_bank_shadow,X
loc_6235C:
		JMP		_obj_next_enemy_ai

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl19_hiding_barrel_lvl2:
		LDA		#$B7
		LDY		#$AD
		BNE		_obj_ehndl_hiding_barrel

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl0B_hiding_barrel_lvl1:
		LDA		#$65
		LDY		#$69
		BNE		_obj_ehndl_hiding_barrel

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl0C_unused:
		LDA		#$67
		LDY		#$6D
		BNE		_obj_ehndl_hiding_barrel

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl02_hiding_barrel_lvl0:
		LDA		#$2E
		LDY		#$30
		BNE		_obj_ehndl_hiding_barrel

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl07_hiding_barrel_lvl0_bonus:
		LDA		#$2E
		LDY		#$34

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl_hiding_barrel:
		STA		_tmpE6
		STY		_tmpE2
		INY
		STY		_tmpE2+1
		INY
		STY		_tmpE4
		INY
		STY		_tmpE5
		LDA		_tmp3C3,X
		BNE		loc_62394
		LDA		_tmpE6
		STA		_obj_spr_idx,X
loc_62391:
		JMP		loc_62427
loc_62394:
		DEC		_chr1_bank_shadow,X
		BPL		loc_62391
		LDA		#4
		STA		_chr1_bank_shadow,X
		LDA		_star_field_data2+$24,X
		BNE		loc_623D0
		LDA		#3
		STA		byte_3B6,X
		LDA		_obj_spr_idx,X
		CMP		_tmpE2
		BEQ		loc_623BE
		DEC		_obj_spr_idx,X
		LDA		_obj_spr_idx,X
		CMP		_tmpE2
		BNE		loc_62427
		LDA		#0
		STA		_star_field_data0+$19,X
		JMP		loc_62427
loc_623BE:
		LDA		#0
		STA		_star_field_data0+$19,X
		LDA		_rand_seed,X
		CMP		#6
		BCS		loc_62427
		LDA		#1
		STA		_star_field_data2+$24,X
		BNE		loc_62427
loc_623D0:
		LDA		#1
		STA		_star_field_data0+$19,X
		LDA		_obj_spr_idx,X
		CMP		_tmpE4
		BCS		loc_623DF
		INC		_obj_spr_idx,X
		BNE		loc_62427
loc_623DF:
		LDA		_tmpE4
		STA		_obj_spr_idx,X
		LDA		byte_3B6,X
		BNE		loc_623EF
loc_623E8:
		LDA		#0
		STA		_star_field_data2+$24,X
		BEQ		loc_62427
loc_623EF:
		JSR		_enemy_projectile_collision_test_ex
		LDA		_obj_flags,X
		AND		#$BF
		BCC		loc_623FA
		ORA		#$40
loc_623FA:
		STA		_obj_flags,X
		LDA		_rand_seed,X
		CMP		#$4B
		BCS		loc_62427
		TYA
		BNE		loc_623E8
		LDY		#$12
		LDA		#$E
		STA		_enemy_bullet_y_pos_global_arg
		LDA		#2
		STA		_enemy_bullet_body_target_select_arg
		LDA		#0
		JSR		_enemy_bullet_insert
		BCS		loc_62427
		LDA		#$10
		STA		_snd_idx
		LDA		_tmpE5
		STA		_obj_spr_idx,X
		DEC		byte_3B6,X
		LDA		#2
		STA		_pal_blink_flag
loc_62427:
		JMP		_obj_next_enemy_ai

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl0F_grenader2:
		LDY		#$83
		BNE		_obj_ehndl_grenader

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl03_grenader0:
		LDY		#$54
		BNE		_obj_ehndl_grenader

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl08_grenader1:
		LDY		#$54
		BNE		_obj_ehndl_grenader

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl_grenader:
		STY		_tmpE2
		INY
		STY		_tmpE2+1
		INY
		INY
		INY
		STY		_tmpE4
		LDA		_tmp3C3,X
		BNE		loc_62448
		JMP		loc_624EB
loc_62448:
		LDA		_obj_hcoll,X
		AND		#3
		BNE		loc_62463
		LDA		_obj_unk,X
		EOR		#$FF
		CLC
		ADC		#1
		STA		_star_field_data1+$B,X
		STA		_obj_unk,X
		LDA		_obj_flags,X
		EOR		#$40
		STA		_obj_flags,X
		JMP		loc_624EB
loc_62463:
		LDA		_obj_unk,X
		BEQ		loc_62477
		INC		_star_field_data2+$24,X
		LDA		_star_field_data2+$24,X
		AND		#$1F
		LSR
		LSR
		LSR
		CLC
		ADC		_tmpE2
		STA		_obj_spr_idx,X
loc_62477:
		LDA		_rand_seed,X
		CMP		#$A
		BCS		loc_624EB
		JSR		_enemy_projectile_collision_test_ex
		LDA		#$FF
		STA		_tmpE2
		LDA		_obj_flags,X
		AND		#$BF
		BCC		loc_62492
		ORA		#$40
		PHA
		LDA		#1
		STA		_tmpE2
		PLA
loc_62492:
		STA		_obj_flags,X
		LDA		_tmpE2
		STA		_star_field_data1+$B,X
		STA		_obj_unk,X
		LDA		_rand_seed,X
		CMP		#5
		BCS		loc_624EB
		TYA
		BNE		loc_624C8
		LDY		#5
		LDA		#$17
		STA		_enemy_bullet_y_pos_global_arg
		LDA		#2
		STA		_enemy_bullet_body_target_select_arg
		LDA		#0
		JSR		_enemy_bullet_insert
		BCS		loc_624EB
		LDA		#$10
		STA		_snd_idx
		LDA		_tmpE4
		STA		_obj_spr_idx,X
		LDA		#0
		STA		_star_field_data1+$B,X
		STA		_obj_unk,X
		JMP		_obj_next_enemy_ai
loc_624C8:
		LDA		_obj_spr_idx,X
		CMP		_tmpE2+1
		BNE		loc_624EB
		LDY		#5
		LDA		#$19
		STA		_enemy_bullet_y_pos_global_arg
		LDA		#2
		STA		_enemy_bullet_body_target_select_arg
		LDA		#5
		JSR		_enemy_bullet_insert
		BCS		loc_624EB
		LDA		#$10
		STA		_snd_idx
		LDA		#0
		STA		_star_field_data1+$B,X
		STA		_obj_unk,X
loc_624EB:
		JMP		_obj_next_enemy_ai

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl10_hanging_gunman0:
		LDY		#$65
		LDA		#$D
		BNE		_obj_ehndl_hanging_gunman

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl13_hanging_gunman1:
		LDY		#$67
		LDA		#$E
		BNE		_obj_ehndl_hanging_gunman

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl_hanging_gunman:
		STA		_tmpE2+1
		STY		_tmpE4
		INY
		STY		_tmpE5
		LDA		_tmp3C3,X
		BEQ		loc_62565
		LDA		_rand_seed
		CMP		#$A
		BCS		loc_6251B
		LDA		_ingame_sync_counter
		LSR
		LSR
		LSR
		AND		#1
		STA		_tmpE6
		LDA		_obj_lvl_y,X
		EOR		_tmpE6
		STA		_obj_lvl_y,X
loc_6251B:
		JSR		_enemy_projectile_collision_test_ex
		LDA		_obj_flags,X
		AND		#$BF
		BCC		loc_62526
		ORA		#$40
loc_62526:
		STA		_obj_flags,X
		AND		#$40
		BEQ		loc_6253E
		LDA		_obj_hndl_idx,X
		CMP		#$12
		BCS		loc_6253E
		LDA		#6
		STA		_star_field_data1+$25,X
		LDA		_tmpE2+1
		STA		_obj_hndl_idx,X
		JMP		loc_62565
loc_6253E:
		LDA		_rand_seed,X
		CMP		#$14
		BCS		loc_62561
		LDY		#$11
		LDA		#$1A
		STA		_enemy_bullet_y_pos_global_arg
		LDA		#0
		STA		_enemy_bullet_body_target_select_arg
		LDA		#5
		JSR		_enemy_bullet_insert
		BCS		loc_62561
		LDA		#$F
		STA		_snd_idx
		LDA		_tmpE5
		STA		_obj_spr_idx,X
		JMP		loc_62565
loc_62561:
		LDA		_tmpE4
		STA		_obj_spr_idx,X
loc_62565:
		JMP		_obj_next_enemy_ai

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl17_walking_grenader1:
		LDY		#$B7
		BNE		_obj_ehndl_walking_grenader

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl14_walking_grenader0:
		LDY		#$7D

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl_walking_grenader:
		STY		_tmpE2
		INY
		INY
		INY
		INY
		STY		_tmpE2+1
		INY
		STY		_tmpE4
		LDA		_tmp3C3,X
		BNE		loc_62581
		JMP		loc_6262C
loc_62581:
		LDA		_obj_hcoll,X
		AND		#3
		BNE		loc_6259C
		LDA		_obj_unk,X
		EOR		#$FF
		CLC
		ADC		#1
		STA		_star_field_data1+$B,X
		STA		_obj_unk,X
		LDA		_obj_flags,X
		EOR		#$40
		STA		_obj_flags,X
		JMP		loc_6262C
loc_6259C:
		LDA		_obj_unk,X
		BEQ		loc_625B0
		INC		_star_field_data2+$24,X
		LDA		_star_field_data2+$24,X
		AND		#$1F
		LSR
		LSR
		LSR
		CLC
		ADC		_tmpE2
		STA		_obj_spr_idx,X
loc_625B0:
		LDA		_rand_seed,X
		CMP		#$A
		BCS		loc_6262C
		JSR		_enemy_projectile_collision_test_ex
		LDA		#$FF
		STA		_tmpE2
		LDA		_obj_flags,X
		AND		#$BF
		BCC		loc_625CB
		ORA		#$40
		PHA
		LDA		#1
		STA		_tmpE2
		PLA
loc_625CB:
		STA		_obj_flags,X
		LDA		_tmpE2
		STA		_star_field_data1+$B,X
		STA		_obj_unk,X
		LDA		_rand_seed,X
		CMP		#5
		BCS		loc_6262C
		LDA		_obj_spr_idx,X
		CMP		_tmpE2+1
		BEQ		loc_625E6
		LDA		_tmpE2+1
		STA		_obj_spr_idx,X
		BNE		loc_62625
loc_625E6:
		LDY		#5
		LDA		#$A
		STA		_enemy_bullet_y_pos_global_arg
		LDA		#2
		STA		_enemy_bullet_body_target_select_arg
		LDA		#$C
		JSR		_enemy_bullet_insert
		BCS		loc_6262C
		LDA		#$FE
		STA		_star_field_data1+$B,Y
		LDA		#$FC
		STA		_obj_unk,Y
		LDA		#$FD
		STA		_star_field_data1+$18,Y
		LDA		#2
		STA		_obj_pal_idx,Y
		LDA		#3
		STA		_star_field_data2+$17,Y
		LDA		_tmpE4
		STA		_obj_spr_idx,X
		LDA		_obj_flags,X
		AND		#$40
		BEQ		loc_62625
		LDA		#2
		STA		_star_field_data1+$B,Y
		LDA		#4
		STA		_obj_unk,Y
loc_62625:
		LDA		#0
		STA		_star_field_data1+$B,X
		STA		_obj_unk,X
loc_6262C:
		JMP		_obj_next_enemy_ai

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl18_rifle_man_lvl2:
		LDA		_tmp3C3,X
		BNE		loc_6263B
		LDA		#$B1
		STA		_obj_spr_idx,X
		JMP		loc_626B8
loc_6263B:
		LDA		_obj_hcoll,X
		AND		#3
		BNE		loc_62656
		LDA		_obj_unk,X
		EOR		#$FF
		CLC
		ADC		#1
		STA		_star_field_data1+$B,X
		STA		_obj_unk,X
		LDA		_obj_flags,X
		EOR		#$40
		STA		_obj_flags,X
		JMP		loc_626B8
loc_62656:
		LDA		_obj_unk,X
		BEQ		loc_6266A
		INC		_star_field_data2+$24,X
		LDA		_star_field_data2+$24,X
		AND		#$1F
		LSR
		LSR
		LSR
		CLC
		ADC		#$B1
		STA		_obj_spr_idx,X
loc_6266A:
		LDA		_rand_seed,X
		CMP		#$A
		BCS		loc_626B8
		JSR		_enemy_projectile_collision_test_ex
		LDA		#$FF
		STA		_tmpE2
		LDA		_obj_flags,X
		AND		#$BF
		BCC		loc_62685
		ORA		#$40
		PHA
		LDA		#1
		STA		_tmpE2
		PLA
loc_62685:
		STA		_obj_flags,X
		LDA		_tmpE2
		STA		_star_field_data1+$B,X
		STA		_obj_unk,X
		LDA		_rand_seed,X
		CMP		#5
		BCS		loc_626B8
		TYA
		BNE		loc_626B8
		LDY		#$11
		LDA		#$17
		STA		_enemy_bullet_y_pos_global_arg
		LDA		#0
		STA		_enemy_bullet_body_target_select_arg
		LDA		#0
		JSR		_enemy_bullet_insert
		BCS		loc_626B8
		LDA		#$10
		STA		_snd_idx
		LDA		#$B6
		STA		_obj_spr_idx,X
		LDA		#0
		STA		_star_field_data1+$B,X
		STA		_obj_unk,X
loc_626B8:
		JMP		_obj_next_enemy_ai

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl0A_ninja:
		LDA		#8
		CMP		_lvl_cur_spr_chr_bank_up
		BEQ		loc_626CC
		STA		_lvl_cur_spr_chr_bank_up
		STA		_lvl_cur_spr_chr_bank_down
		LDA		#$26
		STA		_snd_idx
		JSR		_apu_snd_load
loc_626CC:
		LDA		_tmp3C3,X
		BNE		loc_6271B
		LDA		_player_scroll_stop_override_tmp
		BEQ		loc_626D9
		LDA		#2
		STA		_player_scroll_stop_override_tmp
loc_626D9:
		LDA		#$61
		STA		_obj_spr_idx,X
		LDA		_obj_flags,X
		AND		#8
		BEQ		loc_626F8
		LDA		_lvl_cur_spr_chr_bank_up_backup
		STA		_lvl_cur_spr_chr_bank_up
		LDA		_lvl_cur_spr_chr_bank_down_backup
		STA		_lvl_cur_spr_chr_bank_down
		LDA		#0
		STA		_obj_flags,X
		LDA		#$23
		STA		_snd_idx
		JSR		_apu_snd_load
loc_626F8:
		JSR		_enemy_bullet_get_free_slot_idx
		BCC		loc_62700
		JMP		loc_628D6
loc_62700:
		LDA		_obj_lvl_x_lo,X
		STA		_obj_lvl_x_lo,Y
		LDA		_obj_lvl_x_hi,X
		STA		_obj_lvl_x_hi,Y
		LDA		_obj_lvl_y,X
		STA		_obj_lvl_y,Y
		TXA
		PHA
		TYA
		TAX
		JSR		_enemy_proj0_explode_start
		PLA
		TAX
		JMP		loc_628D6
loc_6271B:
		LDA		_obj_pal_idx,X
		BPL		loc_62722
loc_6271F:
		JMP		loc_628D9
loc_62722:
		TXA
		SEC
		SBC		#1
		STA		_obj_pal_idx,X
		JSR		_obj_ehndl_ninja_animate
		JSR		_obj_ehndl_ninja_bullte_hit
		LDA		_obj_pal_idx,X
		BMI		loc_6271F
		LDA		#1
		STA		_player_scroll_stop_override_tmp
		LDA		_obj_hcoll,X
		AND		#1
		BNE		loc_627B1
		LDA		_obj_flags,X
		AND		#4
		BNE		loc_6277A
loc_62742:
		LDA		#1
		STA		_tmpE5
		LDA		#$FF
		STA		_tmpE6
loc_6274A:
		LDY		_tmpE5
		LDA		_obj_flags,X
		AND		#$40
		BNE		loc_62754
		LDY		_tmpE6
loc_62754:
		STA		_chr1_bank_shadow,X
		TYA
		STA		_star_field_data1+$B,X
		STA		_obj_unk,X
		LDA		#$FB
		STA		_star_field_data1+$18,X
		LDA		#5
		STA		_star_field_data1+$25,X
		LDA		#0
		STA		_star_field_data2+$24,X
		LDA		#5
		STA		_star_field_data2+$17,X
		LDA		_obj_flags,X
		ORA		#4
		STA		_obj_flags,X
		JMP		loc_628D6
loc_6277A:
		LDA		_obj_lvl_y,X
		CMP		#$B4
		BCS		loc_62742
		INC		_star_field_data2+$24,X
		LDA		_star_field_data2+$24,X
		LSR
		LSR
		LSR
		AND		#3
		LDY		_chr1_bank_shadow,X
		BEQ		loc_62793
		CLC
		ADC		#4
loc_62793:
		TAY
		LDA		byte_627A1,Y
		STA		_obj_spr_idx,X
		LDA		byte_627A9,Y
		STA		_obj_flags,X
		JMP		loc_628D6
byte_627A1:
		.BYTE	$61,$62,$63,$64,$61,$62,$63,$64
byte_627A9:
		.BYTE	  5,  5,$C5,$C5,$45,$45,$85,$85
loc_627B1:
		LDA		_star_field_data1+$18,X
		BPL		loc_627B9
		JMP		loc_628D6
loc_627B9:
		LDA		#0
		STA		_star_field_data1+$25,X
		STA		_star_field_data1+$18,X
		LDA		_obj_lvl_y,X
		AND		#$F0
		STA		_obj_lvl_y,X
		LDA		_obj_flags,X
		AND		#4
		BEQ		loc_627DA
		LDA		_obj_flags,X
		AND		#$3B
		LDY		_chr1_bank_shadow,X
		BEQ		loc_627D8
		ORA		#$40
loc_627D8:
		STA		_obj_flags,X
loc_627DA:
		LDA		_cur_lvl_idx
		CMP		#4
		BNE		loc_627FB
		LDA		_tmp3C3,X
		CMP		#$50
		BCS		loc_627FB
		LDA		#$63
		STA		_tmp3C3,X
		LDA		#0
		STA		_obj_pal_idx,X
		LDA		#3
		STA		_tmpE5
		LDA		#$FD
		STA		_tmpE6
		JMP		loc_6274A
loc_627FB:
		LDA		_obj_unk,X
		BNE		loc_62841
		INC		_star_field_data2+$24,X
		LDA		_star_field_data2+$24,X
		LSR
		LSR
		AND		#7
		CMP		#7
		BCC		loc_62822
		LDA		#$C
		STA		_snd_idx
		LDA		#2
		STA		_tmpE5
		LDA		#$FE
		STA		_tmpE6
		LDA		_rand_seed+1
		CMP		#$32
		BCS		loc_6282B
		JMP		loc_6274A
loc_62822:
		TAY
		LDA		byte_62947,Y
		STA		_obj_spr_idx,X
		JMP		loc_628D6
loc_6282B:
		LDA		_rand_seed
		CMP		#$1E
		BCS		loc_6286E
		LDA		_lvl4_destruction_counter
		CMP		#$C
		BCC		loc_6283E
		LDA		_obj_lvl_y,X
		CMP		#$64
		BCC		loc_6286E
loc_6283E:
		JMP		loc_62742
loc_62841:
		LDA		_obj_flags,X
		AND		#8
		BNE		loc_6286E
		INC		_star_field_data2+$24,X
		LDA		_star_field_data2+$24,X
		AND		#$1F
		LSR
		LSR
		LSR
		CLC
		ADC		#$5A
		STA		_obj_spr_idx,X
		LDA		_obj_hcoll,X
		AND		#6
		BEQ		loc_62860
		JMP		loc_62742
loc_62860:
		LDA		_chr1_bank_shadow,X
		BPL		loc_6288F
		LDA		_rand_seed,X
		AND		#$1F
		ADC		#$1E
		STA		_chr1_bank_shadow,X
loc_6286E:
		JSR		_obj_ninja_self_destruct_animate
		LDA		#$FF
		STA		_tmpE2
		LDA		_obj_flags,X
		AND		#$BF
		BCC		loc_62883
		ORA		#$40
		PHA
		LDA		#1
		STA		_tmpE2
		PLA
loc_62883:
		STA		_obj_flags,X
		LDA		_tmpE2
		STA		_star_field_data1+$B,X
		STA		_obj_unk,X
		JMP		loc_628D6
loc_6288F:
		DEC		_chr1_bank_shadow,X
		JSR		_enemy_projectile_collision_test_ex
		TYA
		BNE		loc_6289E
		LDA		_obj_flags,X
		AND		#2
		BNE		loc_628A8
loc_6289E:
		LDA		_chr1_bank_shadow,X
		CMP		#$A
		BCS		loc_628D6
		JMP		loc_6282B
loc_628A8:
		LDA		_lvl4_destruction_counter
		CMP		#$C
		BCS		loc_6289E
		JSR		_enemy_projectile_collision_test
		LDA		#1
		STA		_body_target_idx
		LDA		#7
		JSR		_player_damage_apply
		LDA		#$5F
		STA		_obj_spr_idx,X
		LDA		#$A
		STA		_chr1_bank_shadow,X
		LDA		#0
		STA		_star_field_data1+$B,X
		STA		_obj_unk,X
		STA		_star_field_data2+$24,X
		LDA		#$11
		STA		_snd_idx
		JMP		loc_628D6
loc_628D6:
		JMP		_obj_next_enemy_ai
loc_628D9:
		LDA		#0
		STA		_obj_unk,X
		STA		_star_field_data1+$B,X
		STA		_star_field_data1+$25,X
		STA		_star_field_data1+$18,X
		LDA		_obj_pal_idx._enemy+2
		AND		_obj_pal_idx._enemy+1
		BMI		loc_628EF
		JMP		_obj_next_enemy_ai
loc_628EF:
		LDA		_obj_spr_idx._enemy+2
		CMP		#$60
		BEQ		loc_62908
		DEC		_mul_argA
		BPL		loc_62903
		INC		_obj_spr_idx._enemy+2
		INC		_obj_spr_idx._enemy+1
		LDA		#$14
		STA		_mul_argA
loc_62903:
		LDA		#1
		STA		_ingame_sync_counter
		RTS
loc_62908:
		CLC
		LDA		_obj_pal_idx._enemy+2
		ADC		#1
		ORA		#$80
		STA		_obj_pal_idx._enemy+2
		STA		_obj_pal_idx._enemy+1
		LDA		_obj_flags._ebullet
		ORA		_obj_flags._ebullet+1
		BNE		loc_62939
		LDA		#$6C
		STA		_obj_lvl_y._ebullet
		STA		_obj_lvl_y._ebullet+1
		LDA		#$88
		STA		_obj_lvl_x_lo._ebullet
		LDA		#1
		STA		_obj_lvl_x_hi._ebullet
		LDA		#$91
		STA		_obj_lvl_x_lo._ebullet+1
		LDA		#1
		STA		_obj_lvl_x_hi._ebullet+1
		LDX		#7
		JSR		_enemy_proj0_explode_start
		LDX		#8
		JSR		_enemy_proj0_explode_start
loc_62939:
		LDA		_ingame_sync_counter
		BNE		locret_62946
		LDA		#5
		STA		_cur_lvl_idx
		LDA		#1
		STA		_game_over_flag
locret_62946:
		RTS
byte_62947:
		.BYTE	$5D,$5E,$5E,$5F,$60,$60,$5D,$5D

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl_ninja_animate:
		LDA		_cur_lvl_idx
		CMP		#4
		BNE		loc_629C5
		LDA		_obj_lvl_x_hi,X
		BNE		loc_62983
		LDA		_obj_unk,X
		BPL		loc_6298D
		LDA		_obj_lvl_x_lo,X
		CMP		#8
		BCS		loc_6298D
loc_62963:
		LDA		_obj_unk,X
		EOR		#$FF
		CLC
		ADC		#1
		STA		_obj_unk,X
		LDA		_star_field_data1+$B,X
		EOR		#$FF
		CLC
		ADC		#1
		STA		_star_field_data1+$B,X
		LDA		_chr1_bank_shadow,X
		EOR		#$FF
		CLC
		ADC		#1
		STA		_chr1_bank_shadow,X
		RTS
loc_62983:
		LDA		_obj_unk,X
		BMI		loc_6298D
		LDA		_obj_lvl_x_lo,X
		CMP		#$F2
		BCS		loc_62963
loc_6298D:
		LDA		_lvl4_destruction_counter
		CMP		#$C
		BCC		locret_629BC
		LDA		_obj_hcoll,X
		BEQ		locret_629BC
		LDA		_obj_lvl_y,X
		BMI		locret_629BC
		LDA		_obj_lvl_x_hi,X
		BEQ		locret_629BC
		LDA		_obj_flags,X
		AND		#4
		BNE		locret_629BC
		LDA		_obj_lvl_x_lo,X
		CMP		byte_629BD,X
		BNE		locret_629BC
		LDA		_obj_pal_idx,X
		ORA		#$80
		STA		_obj_pal_idx,X
		LDA		byte_629C1,X
		STA		_obj_flags,X
		LDA		#$5E
		STA		_obj_spr_idx,X
locret_629BC:
		RTS
byte_629BD:
		.BYTE	  0,$76,$76,$8B
byte_629C1:
		.BYTE	  0,$41,$41,  1
loc_629C5:
		LDA		_obj_flags,X
		AND		#8
		BEQ		locret_629EC
		JSR		_enemy_projectile_collision_test_ex
		BCC		loc_629E4
		LDA		_star_field_data1+$B,X
		BPL		locret_629EC
		LDA		#1
		STA		_star_field_data1+$B,X
		LDA		#$40
		STA		_chr1_bank_shadow,X
		ORA		_obj_flags,X
		STA		_obj_flags,X
		RTS
loc_629E4:
		LDA		_star_field_data1+$B,X
		BMI		locret_629EC
		JMP		loc_62963
locret_629EC:
		RTS

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl_ninja_bullte_hit:
		LDA		_obj_flags._pbullet
		BEQ		locret_62A0D
		LDA		_obj_hndl_idx._pbullet
		CMP		#3
		BCC		locret_62A0D
		CMP		#5
		BCS		locret_62A0D
		TXA
		PHA
		LDX		#4
		JSR		_enemy_proj0_explode_start
		PLA
		TAX
		LDA		#$A
		CMP		_hud_msg_idx
		BEQ		locret_62A0D
		JSR		_hud_msg_set_request
locret_62A0D:
		RTS

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl15_walker_droid:
		LDA		#$B
		STA		_lvl_cur_spr_chr_bank_up
		STA		_lvl_cur_spr_chr_bank_down
		LDA		_tmp3C3,X
		BNE		loc_62A1C
		JMP		loc_62B48
loc_62A1C:
		JSR		_enemy_projectile_collision_test_ex
		LDA		_obj_flags,X
		AND		#$BF
		BCS		loc_62A27
		ORA		#$40
loc_62A27:
		STA		_obj_flags,X
		CLC
		LDA		_star_field_data2+$24,X
		ADC		_obj_unk,X
		AND		#$1F
		STA		_star_field_data2+$24,X
		LSR
		LSR
		LSR
		TAY
		CLC
		ADC		#$88
		STA		_obj_spr_idx,X
		LDA		byte_62B44,Y
		STA		_obj_lvl_y,X
		LDA		_obj_lvl_x_lo,X
		BPL		loc_62A4A
		LDY		#1
		BNE		loc_62A6B
loc_62A4A:
		LDA		_obj_lvl_x_lo,X
		CMP		#$50
		BCC		loc_62A54
		LDY		#$FF
		BNE		loc_62A6B
loc_62A54:
		DEC		_chr1_bank_shadow,X
		BPL		loc_62A71
		LDA		_rand_seed,X
		AND		#$1F
		ORA		#$20
		STA		_chr1_bank_shadow,X
		LDY		#1
		LDA		_star_field_data1+$B,X
		BMI		loc_62A6B
		LDY		#$FF
loc_62A6B:
		TYA
		STA		_star_field_data1+$B,X
		STA		_obj_unk,X
loc_62A71:
		LDA		#0
		STA		_star_field_data1+$D
		STA		_obj_unk._enemy+1
		STA		_star_field_data1+$27
		STA		_star_field_data1+$1A
		STA		_star_field_data1+$25,X
		STA		_star_field_data1+$18,X
		LDA		#3
		STA		_obj_pal_idx._enemy+1
		LDA		_obj_flags,X
		STA		_obj_flags._enemy+1
		AND		#$40
		BNE		loc_62AA0
		CLC
		LDA		_obj_lvl_x_lo,X
		ADC		#$28
		STA		_obj_lvl_x_lo._enemy+1
		LDA		_obj_lvl_x_hi,X
		ADC		#0
		STA		_obj_lvl_x_hi._enemy+1
		JMP		loc_62AA8
loc_62AA0:
		LDA		_obj_lvl_x_lo,X
		STA		_obj_lvl_x_lo._enemy+1
		LDA		_obj_lvl_x_hi,X
		STA		_obj_lvl_x_hi._enemy+1
loc_62AA8:
		CLC
		LDA		_obj_lvl_y,X
		ADC		#$10
		STA		_obj_lvl_y._enemy+1
		LDA		_obj_spr_idx._enemy+1
		CMP		#$8C
		BCC		loc_62ACF
		DEC		_star_field_data2+$25
		BPL		loc_62AD3
		LDA		#3
		STA		_star_field_data2+$25
		LDA		_disp_spr0hit_use
		BMI		loc_62AD3
		DEC		_disp_spr0hit_use
		INC		_obj_spr_idx._enemy+1
		LDA		_obj_spr_idx._enemy+1
		CMP		#$91
		BCC		loc_62AD1
loc_62ACF:
		LDA		#$8C
loc_62AD1:
		STA		_obj_spr_idx._enemy+1
loc_62AD3:
		LDA		_obj_flags,X
		EOR		#$40
		STA		_obj_flags,X
		LDA		_rand_seed
		CMP		#$14
		BCS		loc_62AF2
		LDY		#$91
		LDA		#$12
		STA		_enemy_bullet_y_pos_global_arg
		LDA		#1
		STA		_enemy_bullet_body_target_select_arg
		LDA		#0
		JSR		_enemy_bullet_insert
		JMP		loc_62B2B
loc_62AF2:
		LDA		_rand_seed+1
		CMP		#$A
		BCS		loc_62B0B
		LDY		#$92
		LDA		#$19
		STA		_enemy_bullet_y_pos_global_arg
		LDA		#1
		STA		_enemy_bullet_body_target_select_arg
		LDA		#0
		JSR		_enemy_bullet_insert
		JMP		loc_62B2B
loc_62B0B:
		LDA		_rand_seed+2
		CMP		#5
		BCS		loc_62B39
		LDY		#$93
		LDA		#$16
		STA		_enemy_bullet_y_pos_global_arg
		LDA		#2
		STA		_enemy_bullet_body_target_select_arg
		LDA		#1
		PHA
		LDA		_obj_lvl_y._player
		BMI		loc_62B27
		PLA
		LDA		#$FE
		PHA
loc_62B27:
		PLA
		JSR		_enemy_bullet_insert
loc_62B2B:
		BCS		loc_62B39
		LDA		#3
		STA		_obj_pal_idx,Y
		STA		_disp_spr0hit_use
		LDA		#1
		STA		_snd_idx
loc_62B39:
		LDA		#1
		STA		_obj_pal_idx._enemy
		LDA		_obj_flags,X
		EOR		#$40
		STA		_obj_flags,X
		RTS
byte_62B44:
		.BYTE	$99,$98,$99,$98
loc_62B48:
		LDY		#4
loc_62B4A:
		LDA		_obj_flags,Y
		BEQ		loc_62B56
		INY
		CPY		#7
		BCC		loc_62B4A
		BCS		loc_62B7C
loc_62B56:
		LDA		_rand_seed
		AND		#$1F
		ADC		#8
		ADC		_obj_lvl_x_lo,X
		STA		_obj_lvl_x_lo,Y
		LDA		_obj_lvl_x_hi,X
		ADC		#0
		STA		_obj_lvl_x_hi,Y
		LDA		_rand_seed+1
		AND		#$1F
		ADC		#8
		ADC		_obj_lvl_y,X
		STA		_obj_lvl_y,Y
		TXA
		PHA
		TYA
		TAX
		JSR		_enemy_proj0_explode_start
		PLA
		TAX
loc_62B7C:
		LDA		#0
		STA		_star_field_data1+$B,X
		STA		_obj_unk,X
		STA		_star_field_data1+$25,X
		STA		_star_field_data1+$18,X
		LDA		_obj_pal_idx,X
		EOR		#3
		STA		_obj_pal_idx,X
		INC		_obj_pal_idx._enemy
		BNE		locret_62B9C
		LDA		#1
		STA		_game_over_flag
		LDA		#4
		STA		_cur_lvl_idx
locret_62B9C:
		RTS

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl16_ground_timed_bomb:
		INC		_star_field_data2+$24,X
		LDA		_star_field_data2+$24,X
		CMP		#$64
		BCC		loc_62C0B

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl_ground_bomb:
		LDY		#7
loc_62BA9:
		LDA		_obj_lvl_x_lo,X
		STA		_obj_lvl_x_lo,Y
		LDA		_obj_lvl_x_hi,X
		STA		_obj_lvl_x_hi,Y
		SEC
		LDA		_obj_lvl_y,X
		SBC		#4
		STA		_obj_lvl_y,Y
		LDA		#4
		STA		_obj_spr_idx,Y
		LDA		#$FC
		STA		_star_field_data1+$18,Y
		LDA		#2
		STA		_star_field_data1+$25,Y
		LDA		#0
		STA		_obj_hndl_idx,Y
		STA		_star_field_data1+$B,Y
		STA		_obj_unk,Y
		STA		_obj_pal_idx,Y
		LDA		#1
		STA		_obj_flags,Y
		INY
		CPY		#$A
		BCC		loc_62BA9
		LDA		#$F8
		STA		_obj_unk._ebullet
		LDA		#$FC
		STA		_star_field_data1+$12
		LDA		#8
		STA		_obj_unk._ebullet+2
		LDA		#4
		STA		_star_field_data1+$14
		LDA		#$F8
		STA		_star_field_data1+$20
		LDA		#6
		STA		_star_field_data2+5
		LDA		#0
		STA		_obj_flags,X
		LDA		#7
		STA		_snd_idx
		LDA		#5
		STA		_pal_blink_flag
loc_62C0B:
		JMP		_obj_next_enemy_ai

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl1A_ground_still_bomb0:
		LDA		_obj_flags,X
		AND		#2
		BEQ		loc_62C17
		JMP		_obj_ehndl_ground_bomb
loc_62C17:
		JMP		_obj_next_enemy_ai

; =============== S U B	R O U T	I N E =======================================
_enemy_projectile_collision_test:
		JSR		_enemy_projectile_collision_test_ex
		LDA		_obj_flags,X
		AND		#$BF
		BCC		loc_62C25
		ORA		#$40
loc_62C25:
		STA		_obj_flags,X
		RTS

; =============== S U B	R O U T	I N E =======================================
_enemy_proj0_explode_start:
		LDA		#5
		STA		_snd_idx
		JMP		_enemy_projectile_explode_start

; =============== S U B	R O U T	I N E =======================================
_enemy_proj1_explode_start:
		LDA		#6
		STA		_snd_idx

; =============== S U B	R O U T	I N E =======================================
_enemy_projectile_explode_start:
		LDA		#$13
		STA		_star_field_data2+$24,X
		LDA		#7
		STA		_obj_hndl_idx,X
		STA		_obj_spr_idx,X
		LDA		#1
		STA		_obj_flags,X
		RTS

; =============== S U B	R O U T	I N E =======================================
_player_death_anim_timers:
		LDA		#0
		STA		_star_field_data1+$25,X
		STA		_star_field_data1+$B,X
		STA		_obj_unk,X
		STA		_star_field_data1+$18,X
		STA		_obj_pal_idx,X
		DEC		_star_field_data2+$24,X
		BEQ		loc_62C62
		LDA		_star_field_data2+$24,X
		LSR
		LSR
		CLC
		ADC		#6
		STA		_obj_spr_idx,X
		RTS
loc_62C62:
		STA		_obj_flags,X
		RTS

; =============== S U B	R O U T	I N E =======================================
_enemy_bullet_get_free_slot_idx:
		CLC
		LDY		#7
loc_62C68:
		LDA		_obj_flags,Y
		BEQ		locret_62C72
		INY
		CPY		#$A
		BCC		loc_62C68
locret_62C72:
		RTS

; =============== S U B	R O U T	I N E =======================================
_enemy_bullet_insert:
		STA		_tmpE2
		STY		_tmpE2+1
		JSR		_enemy_bullet_get_free_slot_idx
		BCC		loc_62C7D
		RTS
loc_62C7D:
		LDA		#0
		STA		_obj_pal_idx,Y
		LDA		#1
		STA		_obj_flags,Y
		LDA		_enemy_bullet_body_target_select_arg
		STA		_obj_hndl_idx,Y
		CLC
		LDA		_obj_lvl_y,X
		ADC		_enemy_bullet_y_pos_global_arg
		STA		_obj_lvl_y,Y
		LDA		_tmpE2
		STA		_star_field_data1+$25,Y
		STA		_star_field_data1+$18,Y
		LDA		_tmpE2+1
		STA		_obj_spr_idx,Y
		LDA		_obj_flags,X
		AND		#$40
		BEQ		loc_62CC1
		LDA		#5
		STA		_star_field_data1+$B,Y
		STA		_obj_unk,Y
		CLC
		LDA		_obj_lvl_x_lo,X
		ADC		#$18
		STA		_obj_lvl_x_lo,Y
		LDA		_obj_lvl_x_hi,X
		ADC		#0
		STA		_obj_lvl_x_hi,Y
		CLC
		RTS
loc_62CC1:
		LDA		#$FB
		STA		_star_field_data1+$B,Y
		STA		_obj_unk,Y
		LDA		_obj_lvl_x_lo,X
		STA		_obj_lvl_x_lo,Y
		LDA		_obj_lvl_x_hi,X
		STA		_obj_lvl_x_hi,Y
		CLC
		RTS

; =============== S U B	R O U T	I N E =======================================
_enemy_bullet_animate:
		LDX		#9
loc_62CD7:
		LDA		_obj_lvl_y,X
		CMP		#$D4
		BCS		loc_62CF7
		CMP		#$3C
		BCS		loc_62CE7
		LDA		#0
		STA		_obj_flags,X
		BEQ		loc_62CF1
loc_62CE7:
		LDA		_obj_hndl_idx,X
		CMP		#7
		BNE		loc_62CF1
		TXA
		JSR		_player_death_anim_timers
loc_62CF1:
		DEX
		CPX		#6
		BNE		loc_62CD7
		RTS
loc_62CF7:
		LDA		_obj_hndl_idx,X
		CMP		#7
		BEQ		loc_62CE7
		JSR		_enemy_proj0_explode_start
		JMP		loc_62CE7

; =============== S U B	R O U T	I N E =======================================
_obj_tank_weapon_handler:
		LDA		_cur_lvl_idx
		CMP		#2
		BEQ		loc_62D0A
locret_62D09:
		RTS
loc_62D0A:
		LDA		_lvl3_tank_weapon_disable_flag
		BNE		locret_62D09
		LDA		_obj_lvl_x_hi._player
		CMP		#$E
		BCS		loc_62D18
		JMP		loc_62D94
loc_62D18:
		LDA		_end_of_the_level_flag
		BEQ		loc_62D3B
		LDY		#3
loc_62D1F:
		LDA		_obj_flags,Y
		BEQ		loc_62D33
		LDA		_obj_hndl_idx,Y
		CMP		#4
		BEQ		loc_62D3B
		CMP		#6
		BEQ		loc_62D3B
		CMP		#9
		BEQ		loc_62D3B
loc_62D33:
		DEY
		BPL		loc_62D1F
		LDA		#1
		STA		_lvl3_tank_weapon_disable_flag
loc_62D3B:
		LDA		#$C
		STA		_lvl_cur_spr_chr_bank_down
		JSR		_enemy_bullet_get_free_slot_idx
		BCC		loc_62D45
		RTS
loc_62D45:
		LDA		_rand_seed
		AND		#$F
		TAX
		LDA		byte_62E2D,X
		STA		_obj_lvl_x_lo,Y
		LDA		#$F
		STA		_obj_lvl_x_hi,Y
		LDA		byte_62E3D,X
		STA		_obj_lvl_y,Y
		LDA		byte_62E8D,X
		STA		_obj_pal_idx,Y
		LDA		byte_62E5D,X
		STA		_star_field_data1+$B,Y
		STA		_obj_unk,Y
		LDA		byte_62E7D,X
		STA		_star_field_data1+$25,Y
		LDA		byte_62E6D,X
		STA		_star_field_data1+$18,Y
		LDA		#2
		STA		_obj_hndl_idx,Y
		LDA		#1
		STA		_obj_flags,Y
		LDA		byte_62E4D,X
		STA		_obj_spr_idx,Y
		CMP		#5
		BEQ		loc_62D8F
		LDA		#2
		STA		_pal_blink_flag
loc_62D8F:
		LDA		#7
		STA		_snd_idx
		RTS
loc_62D94:
		LDA		byte_69C
		BEQ		loc_62DB3
		DEC		byte_69C
		BEQ		loc_62DA1
		JMP		locret_62E28
loc_62DA1:
		SEC
		LDA		#$E
		SBC		_obj_lvl_x_hi._player
		STA		byte_69B
		LDA		#3
		STA		_pal_blink_flag
		LDA		#7
		STA		_snd_idx
		RTS
loc_62DB3:
		LDA		byte_69A
		ORA		byte_69B
		BEQ		loc_62DCA
		DEC		byte_69A
		BNE		locret_62E28
		LDA		#$14
		STA		byte_69A
		DEC		byte_69B
		BNE		locret_62E28
loc_62DCA:
		JSR		_enemy_bullet_get_free_slot_idx
		BCS		locret_62E28
		LDX		byte_69D
		SEC
		LDA		_obj_lvl_x_lo._player
		SBC		_player_wnd_scroll_counter
		STA		_tmpE2
		LDA		_obj_lvl_x_hi._player
		SBC		#0
		STA		_tmpE2+1
		CLC
		LDA		_tmpE2
		ADC		#$FA
		STA		_obj_lvl_x_lo,Y
		LDA		_tmpE2+1
		ADC		#0
		STA		_obj_lvl_x_hi,Y
		LDA		byte_62E29,X
		STA		_obj_lvl_y,Y
		LDA		#0
		STA		_star_field_data1+$25,Y
		STA		_star_field_data1+$18,Y
		LDA		#$FA
		STA		_star_field_data1+$B,Y
		STA		_obj_unk,Y
		LDA		#$AB
		STA		_obj_spr_idx,Y
		LDA		#3
		STA		_obj_hndl_idx,Y
		STA		_obj_pal_idx,Y
		LDA		#1
		STA		_obj_flags,Y
		INC		byte_69D
		LDA		byte_69D
		AND		#3
		STA		byte_69D
		LDA		_rand_seed
		ORA		#$40
		STA		byte_69C
locret_62E28:
		RTS
byte_62E29:
		.BYTE	$50,$78,$A0,$C8
byte_62E2D:
		.BYTE	$4E,$AE,$AD,$B0,$B7,$E3,$E7,$EB,$4E,$AE,$AD,$B0,$B7,$E3,$E7,$EB
byte_62E3D:
		.BYTE	$A0,$A6,$A0,$9C,$99,$9C,$9A,$98,$A0,$A6,$A0,$9C,$99,$9C,$9A,$98
byte_62E4D:
		.BYTE	$AB,  5,  5,  5,  5,  5,  5,  5,$AB,  5,  5,  5,  5,  5,  5,  5
byte_62E5D:
		.BYTE	$FA,$FA,$FA,$FB,$FE,$FE,$FE,$FE,$FA,$FA,$FA,$FB,$FE,$FE,$FE,$FE
byte_62E6D:
		.BYTE	  0,  0,$FE,$FD,$FA,$FC,$FC,$FC,  0,  0,$FE,$FD,$FA,$FC,$FC,$FC
byte_62E7D:
		.BYTE	  0,  0,$FE,$FD,$FA,$FC,$FC,$FC,  0,  0,  6,  6,  6,  6,  6,  6
byte_62E8D:
		.BYTE	  3,  0,  0,  0,  0,  0,  0,  0,  3,  0,  0,  0,  0,  0,  0,  0

; =============== S U B	R O U T	I N E =======================================
_enemy_projectile_collision_test_ex:
		LDY		#0
		LDA		_obj_lvl_y._player
		AND		#$80
		STA		_tmpE2
		LDA		_obj_lvl_y,X
		AND		#$80
		CMP		_tmpE2
		BEQ		loc_62EAE
		INY
loc_62EAE:
		SEC
		LDA		_obj_lvl_x_lo._player
		SBC		_obj_lvl_x_lo,X
		STA		_tmpE2
		LDA		_obj_lvl_x_hi._player
		SBC		_obj_lvl_x_hi,X
		PHP
		BCS		loc_62ECF
		SEC
		LDA		_obj_lvl_x_lo,X
		SBC		_obj_lvl_x_lo._player
		STA		_tmpE2
		LDA		_obj_lvl_x_hi,X
		SBC		_obj_lvl_x_hi._player
		LSR
		LDA		_tmpE2
		ROR
		LSR
		LSR
		PLP
		RTS
loc_62ECF:
		LSR
		LDA		_tmpE2
		ROR
		LSR
		LSR
		PLP
		RTS

; =============== S U B	R O U T	I N E =======================================
_obj_ninja_self_destruct_animate:
		LDA		_lvl4_destruction_counter
		CMP		#$C
		BCC		_enemy_projectile_collision_test_ex
		LDY		#0
		SEC
		LDA		#$60
		SBC		_obj_lvl_x_lo,X
		STA		_tmpE2
		LDA		#1
		SBC		_obj_lvl_x_hi,X
		PHP
		BCS		loc_62F01
		SEC
		LDA		_obj_lvl_x_lo,X
		SBC		#$60
		STA		_tmpE2
		LDA		_obj_lvl_x_hi,X
		SBC		#1
		LSR
		LDA		_tmpE2
		ROR
		LSR
		LSR
		PLP
		RTS
loc_62F01:
		LSR
		LDA		_tmpE2
		ROR
		LSR
		LSR
		PLP
		RTS

; =============== S U B	R O U T	I N E =======================================
_obj_hidden_passage_test_and_bonus_animate:
		LDA		#0
		STA		_player_hit_bonus_portal_flag
		LDY		_cur_lvl_state_idx
		SEC
		LDA		_obj_lvl_x_lo._player
		SBC		_lvl_hidden_passage_x_lo_list,Y
		TAX
		LDA		_obj_lvl_x_hi._player
		SBC		_lvl_hidden_passage_x_hi_list,Y
		BNE		loc_62F2E
		CPX		#8
		BCS		loc_62F2E
		LDA		_obj_lvl_y._player
		AND		#$F0
		CMP		_lvl_hidden_passage_y_list,Y
		BNE		loc_62F2E
		INC		_player_hit_bonus_portal_flag
loc_62F2E:
		LDA		_obj_hndl_idx._bonus
		CMP		#$23
		BEQ		loc_62F38
		CMP		#$22
		BNE		locret_62F45
loc_62F38:
		LDA		_ingame_sync_counter
		LSR
		LSR
		AND		#1
		STA		_tmpE2
		ASL
		ORA		_tmpE2
		STA		_obj_pal_idx._bonus
locret_62F45:
		RTS

; =============== S U B	R O U T	I N E =======================================
_obj_obj_collision_tests:
		LDA		_game_over_flag
		BEQ		loc_62F4C
		RTS
loc_62F4C:
		INC		_collision_test_idx
		LDA		_collision_test_idx
		AND		#3
		ASL
		TAY
		LDA		_collision_tests_lib,Y
		STA		_tmp_ptr0
		LDA		_collision_tests_lib+1,Y
		STA		_tmp_ptr0+1
		JMP		(_tmp_ptr0)
_collision_tests_lib:
		.WORD _player_to_enemy_collision_test
		.WORD _enemy_to_player_shot_collision_test
		.WORD _player_to_enemy_shot_collision_test
		.WORD _player_to_item_collision_test

; =============== S U B	R O U T	I N E =======================================
_player_to_enemy_collision_test:
		LDX		#0
		STX		_player_to_enemy_overlap_flag
		LDY		#3
loc_62F74:
		LDA		_obj_flags,Y
		AND		#$FD
		STA		_obj_flags,Y
		BEQ		loc_62F90
		JSR		_collision_test
		BEQ		loc_62F90
		LDA		_obj_flags,Y
		ORA		#2
		STA		_obj_flags,Y
		LDA		#1
		STA		_player_to_enemy_overlap_flag
loc_62F90:
		DEY
		BNE		loc_62F74
		RTS

; =============== S U B	R O U T	I N E =======================================
_player_to_item_collision_test:
		LDA		_player_sit_jump_flag
		AND		#1
		BEQ		loc_62F9B
locret_62F9A:
		RTS
loc_62F9B:
		LDA		_pads
		AND		#4
		BEQ		locret_62F9A
		LDA		_obj_flags._bonus
		BEQ		locret_62F9A
		LDX		#0
		LDY		#$B
		JSR		_collision_test
		BEQ		locret_62F9A
		LDA		_obj_hndl_idx._bonus
		CMP		#$26
		BEQ		_get_power_restore_item
		CMP		#$27
		BEQ		_get_missile_power_item
		CMP		#$25
		BEQ		_get_fire_power_item
		JMP		_get_repair_tank_item
_get_fire_power_item:
		LDX		#0
		LDA		_weapon0_idx
		CMP		#2
		BCS		_weapon0_ammo_full_reload
		INC		_weapon0_idx
		LDX		_weapon0_idx
		LDA		_upgrade_bullets_list,X
		STA		_weapon0_ammo,X
		LDA		#6
		JSR		_hud_msg_set_request
		JMP		_wepon_item_snd
_get_missile_power_item:
		LDX		#3
		LDA		_weapon1_idx
		CMP		#2
		BCS		_weapon0_ammo_full_reload
		INC		_weapon1_idx
		LDX		_weapon1_idx
		LDA		_upgrade_missile_list,X
		STA		_weapon1_ammo,X
		LDA		#7
		JSR		_hud_msg_set_request
_wepon_item_snd:
		LDA		#0
		STA		_obj_flags._bonus
		LDA		#8
		STA		_snd_idx
		RTS
_weapon0_ammo_full_reload:
		LDY		#2
loc_62FFB:
		LDA		_upgrade_bullets_list,X
		STA		_weapon0_ammo,X
		INX
		DEY
		BNE		loc_62FFB
		BEQ		_wepon_item_snd
_get_power_restore_item:
		LDX		#3
loc_63009:
		CLC
		LDA		_cur_head_percent,X
		ADC		#3
		STA		_cur_head_percent,X
		DEX
		BPL		loc_63009
		LDA		#0
		STA		_obj_flags._bonus
		LDA		#4
		JSR		_hud_msg_set_request
		LDA		#9
		STA		_snd_idx
		RTS
_get_repair_tank_item:
		CMP		#$22
		BEQ		_get_smart_bomb_item
		CMP		#$21
		BEQ		_get_jetpack_fuel_item
		INC		_cur_repair_tanks
		LDA		#0
		STA		_obj_flags._bonus
		LDA		#8
		JSR		_hud_msg_set_request
		LDA		#9
		STA		_snd_idx
		RTS
_get_smart_bomb_item:
		INC		_lvl2_smart_bomb_flag
		LDA		#0
		STA		_obj_flags._bonus
		LDA		#$D
		JSR		_hud_msg_set_request
		LDA		#$FF
		STA		_smart_bomb_timer
		LDA		#8
		STA		_snd_idx
		RTS
_get_jetpack_fuel_item:
		LDA		#3
		STA		_lvl3_jetpack_fuel_tanks
		LDA		#0
		STA		_obj_flags._bonus
		LDA		#$14
		JSR		_hud_msg_set_request
		LDA		#8
		STA		_snd_idx
		RTS
_upgrade_bullets_list:
		.BYTE	$FF,$C8,$64
_upgrade_missile_list:
		.BYTE	$FF,  5,  2

; =============== S U B	R O U T	I N E =======================================
_enemy_to_player_shot_collision_test:
		LDX		#4
loc_6306D:
		LDA		_obj_flags,X
		BNE		loc_63077
loc_63071:
		JMP		loc_6310F
loc_63074:
		JMP		loc_63109
loc_63077:
		LDA		_obj_hndl_idx,X
		CMP		#7
		BCS		loc_63071
		LDY		#3
loc_6307F:
		LDA		_obj_flags,Y
		BEQ		loc_63074
		JSR		_collision_test
		BEQ		loc_63074
		JSR		_player_bullet_remove
		STX		_tmpE2
		LDA		_obj_hndl_idx,X
		STA		_tmpE2+1
		CMP		#3
		BCS		loc_6309C
		LDA		#0
		STA		_obj_flags,X
		BEQ		loc_6309F
loc_6309C:
		JSR		_enemy_proj0_explode_start
loc_6309F:
		LDA		_obj_hndl_idx,Y
		CMP		#$A
		BEQ		loc_630AA
		CMP		#$15
		BNE		loc_630B0
loc_630AA:
		LDA		_tmpE2+1
		ORA		#8
		STA		_tmpE2+1
loc_630B0:
		LDX		_tmpE2+1
		SEC
		LDA		_tmp3C3,Y
		SBC		byte_63C22,X
		STA		_tmp3C3,Y
		STY		_last_enemy_hit
		LDX		_tmpE2
		BCC		loc_630C6
		AND		#$FF
		BNE		loc_6310F
loc_630C6:
		LDA		#1
		STA		_tmpE2
		LDA		#$F9
		STA		_tmpE2+1
		LDA		_obj_lvl_y,Y
		BPL		loc_630D3
loc_630D3:
		LDA		#4
		STA		_tmpE2
		LDA		#$FD
		STA		_tmpE2+1
		LDA		_tmpE2
		STA		_star_field_data1+$25,Y
		LDA		_tmpE2+1
		STA		_star_field_data1+$18,Y
		LDA		#0
		STA		_tmp3C3,Y
		LDA		#5
		STA		_star_field_data2+$17,Y
		LDA		#6
		STA		_tmpE2
		LDA		_obj_unk,X
		BPL		loc_630FB
		LDA		#$FA
		STA		_tmpE2
loc_630FB:
		LDA		_tmpE2
		STA		_obj_unk,Y
		STA		_star_field_data1+$B,Y
		JSR		_obj_add_score
		JMP		loc_6310F
loc_63109:
		DEY
		BEQ		loc_6310F
		JMP		loc_6307F
loc_6310F:
		INX
		CPX		#7
		BCS		locret_63117
		JMP		loc_6306D
locret_63117:
		RTS

; =============== S U B	R O U T	I N E =======================================
_player_to_enemy_shot_collision_test:
		LDA		_hud_msg_idx
		CMP		#9
		BEQ		locret_6315A
		LDY		#0
		LDX		#7
loc_63122:
		LDA		_obj_flags,X
		BEQ		loc_63155
		JSR		_collision_test
		BEQ		loc_63155
		LDA		#1
		STA		_player_to_enemy_overlap_flag
		LDA		_obj_hndl_idx,X
		STA		_enemy_bullet_body_target_select_arg
		BNE		loc_6313D
		LDA		#0
		STA		_obj_flags,X
		BEQ		loc_63144
loc_6313D:
		CMP		#7
		BEQ		loc_63155
		JSR		_enemy_proj1_explode_start
loc_63144:
		LDY		_enemy_bullet_body_target_select_arg
		LDA		_body_targets_list,Y
		STA		_body_target_idx
		LDA		_player_damage_list,Y
		JSR		_player_damage_apply
		LDY		#0
loc_63155:
		INX
		CPX		#$A
		BCC		loc_63122
locret_6315A:
		RTS

; =============== S U B	R O U T	I N E =======================================
_player_damage_apply:
		STA		_tmpE2
		STX		_tmpE2+1
		LDA		#3
		STA		_player_damage_blink_timer
		LDA		_hud_msg_idx
		CMP		#9
		BEQ		_exit
		LDA		_obj_shuffle_mode
		AND		#2
		BNE		_exit
		LDX		_body_target_idx
		LDA		_cur_head_percent,X
		CMP		#$A
		BCS		loc_63197
		CMP		#7
		BCC		loc_63188
		TXA
		CLC
		ADC		#$F
		CMP		_hud_msg_idx
		BEQ		loc_63188
		JSR		_hud_msg_set_request
loc_63188:
		LDA		_rand_seed
loc_6318A:
		AND		#3
		TAX
		LDA		_cur_head_percent,X
		BNE		loc_63197
		INX
		TXA
		JMP		loc_6318A
loc_63197:
		SEC
		LDA		_cur_head_percent,X
		SBC		_tmpE2
		BCS		loc_631A1
		LDA		#0
loc_631A1:
		STA		_cur_head_percent,X
		CLC
		LDA		_cur_head_percent
		ADC		_cur_body_percent
		ADC		_cur_arms_percent
		ADC		_cur_legs_percent
		BNE		loc_631C4
		STA		_cur_overal_armor_sum
		STA		_player_explode_timer_tmp
		LDA		#2
		STA		_obj_shuffle_mode
		LDA		#9
		JSR		_hud_msg_set_request
		JMP		_exit
loc_631C4:
		CMP		#$19
		BCS		_exit
		LDA		#5
		CMP		_hud_msg_idx
		BEQ		_exit
		JSR		_hud_msg_set_request
_exit:
		LDX		_tmpE2+1
		RTS

; =============== S U B	R O U T	I N E =======================================
_obj_misc_collision_handler:
		LDA		_obj_flags._misc
		BEQ		loc_63220
		LDA		_obj_flags._misc
		CMP		#$FF
		BEQ		loc_63220
		LDA		_star_field_data1+$22
		BNE		loc_63216
		LDY		#6
		JSR		_mmc1_prg
		LDX		#0
		LDY		#$A
		JSR		_collision_test
		BEQ		loc_63220
		SEC
		LDA		_obj_lvl_y._misc
		SBC		_obj_lvl_y._player
		BCC		loc_63220
		CMP		#$28
		BCC		loc_63220
		CMP		#$30
		BCS		loc_63220
		LDA		#1
		STA		_obj_hcoll._player
		STA		_obj_vcoll._player
		INC		_delay_counter+1
		LDA		_delay_counter+1
		CMP		#$1E
		BCC		loc_63220
		LDA		#5
		STA		_star_field_data2+7
		RTS
loc_63216:
		LDA		_obj_lvl_y._misc
		CMP		#$E6
		BCC		loc_63220
		LDA		#0
		STA		_obj_flags._misc
loc_63220:
		LDX		_cur_lvl_idx
		LDY		_lvl_prg_bank_list,X
		JSR		_mmc1_prg
		RTS

; =============== S U B	R O U T	I N E =======================================
_obj_add_score:
		STX		_tmpE2
		LDX		_obj_hndl_idx,Y
		CLC
		LDA		_scores_table,X
		ADC		_cur_score_hex
		STA		_cur_score_hex
		LDA		_cur_score_hex+1
		ADC		#0
		STA		_cur_score_hex+1
		LDX		_tmpE2
		RTS

; =============== S U B	R O U T	I N E =======================================
_collision_test:
		STY		_tmpE2
		STX		_tmpE2+1
		LDY		_obj_spr_idx,X
		LDA		_spr_bbox_height_list,Y
		BNE		loc_63250
		JMP		loc_632D0
loc_63250:
		LDA		_spr_bbox_y_list,Y
		CLC
		ADC		_obj_lvl_y,X
		STA		_tmpE4
		ADC		_spr_bbox_width_list,Y
		STA		_tmpE5
		LDX		_tmpE2
		LDY		_obj_spr_idx,X
		LDA		_spr_bbox_y_list,Y
		CLC
		ADC		_obj_lvl_y,X
		CMP		_tmpE5
		BCS		loc_632D0
		ADC		_spr_bbox_width_list,Y
		CMP		_tmpE4
		BCC		loc_632D0
		CLC
		LDA		_obj_lvl_x_lo,X
		ADC		_spr_bbox_x_list,Y
		STA		_tmpE6
		LDA		_obj_lvl_x_hi,X
		ADC		#0
		STA		_tmpE7
		LDX		_tmpE2+1
		LDY		_obj_spr_idx,X
		CLC
		LDA		_obj_lvl_x_lo,X
		ADC		_spr_bbox_x_list,Y
		STA		_tmpE8
		LDA		_obj_lvl_x_hi,X
		ADC		#0
		STA		_tmpE9
		LDX		_tmpE2+1
		LDY		_tmpE2
		SEC
		LDA		_tmpE8
		SBC		_tmpE6
		STA		_tmpE4
		LDA		_tmpE9
		SBC		_tmpE7
		BCC		loc_632B1
		BNE		loc_632D0
		LDX		_obj_spr_idx,Y
		LDA		_spr_bbox_height_list,X
		CMP		_tmpE4
		BCC		loc_632D0
		JMP		loc_632C9
loc_632B1:
		SEC
		LDA		_tmpE6
		SBC		_tmpE8
		STA		_tmpE4
		LDA		_tmpE7
		SBC		_tmpE9
		BNE		loc_632D0
		LDY		_obj_spr_idx,X
		LDA		_spr_bbox_height_list,Y
		BEQ		loc_632D0
		CMP		_tmpE4
		BCC		loc_632D0
loc_632C9:
		LDY		_tmpE2
		LDX		_tmpE2+1
		LDA		#1
		RTS
loc_632D0:
		LDY		_tmpE2
		LDX		_tmpE2+1
		LDA		#0
		RTS

; =============== S U B	R O U T	I N E =======================================
_lvl_obj_vars_init:
		LDX		#$B
loc_632D9:
		LDA		#0
		STA		_obj_flags,X
		LDA		#3
		STA		_star_field_data2+$17,X
		STA		_star_field_data2+$A,X
		DEX
		BPL		loc_632D9
		LDY		_cur_lvl_state_idx
		LDA		_lvl_player_start_y_list,Y
		STA		_obj_lvl_y._player
		LDA		_lvl_player_start_x_lo_list,Y
		STA		_obj_lvl_x_lo._player
		LDA		_lvl_player_start_x_hi_list,Y
		STA		_obj_lvl_x_hi._player
		LDA		#0
		STA		_star_field_data1+$25
		STA		_star_field_data1+$18
		STA		_star_field_data1+$B
		STA		_obj_unk._player
		LDA		#3
		STA		_obj_pal_idx._player
		LDA		#$41
		STA		_obj_flags._player
		LDA		#1
		STA		_star_field_data0+$19
		LDA		_lvl_spr_hit0_x_lo_list,Y
		STA		_obj_lvl_x_lo._spr0hit
		LDA		_lvl_spr_hit0_x_hi_list,Y
		STA		_obj_lvl_x_hi._spr0hit
		LDA		_lvl_spr_hit0_y_list,Y
		STA		_obj_lvl_y._spr0hit
		LDA		#$21
		STA		_obj_flags._spr0hit
		LDA		#0
		STA		_obj_spr_idx._spr0hit
		STA		_obj_pal_idx._spr0hit
		RTS

; =============== S U B	R O U T	I N E =======================================
_obj_spr_manager:
		LDY		#6
		JSR		_mmc1_prg
		SEC
		LDA		_obj_lvl_x_lo._player
		SBC		_player_wnd_scroll_counter
		STA		_tmpE5
		LDA		_obj_lvl_x_hi._player
		SBC		#0
		STA		_tmpE6
		LDA		_obj_shuffle_mode
		EOR		#1
		STA		_obj_shuffle_mode
		AND		#3
		ASL
		ASL
		ASL
		ASL
		CLC
		ADC		#<[_obj_shuffle_tables]
		STA		_ptr1
		LDA		#>[_obj_shuffle_tables]
		ADC		#0
		STA		_ptr1+1
		LDX		#0
		STX		_obj_manager_tmp_cnt
loc_6335A:
		LDA		#0
		STA		_player_explode_spr_remove_cnt
		LDY		_obj_manager_tmp_cnt
		LDA		(_ptr1),Y
		BNE		loc_63368
		LDY		_player_explode_spr_remove_reload
		STY		_player_explode_spr_remove_cnt
loc_63368:
		TAY
		JSR		_obj_spr_insert
		CPX		#0
		BNE		loc_63378
		LDA		_obj_manager_tmp_cnt
		BNE		loc_6338F
		INX
		INX
		INX
		INX
loc_63378:
		INC		_obj_manager_tmp_cnt
		LDY		_obj_manager_tmp_cnt
		CPY		#$D
		BCC		loc_6335A
loc_63380:
		CPX		#0
		BEQ		loc_6338F
		LDA		#$F8
		STA		_spr_buf,X
		INX
		INX
		INX
		INX
		BNE		loc_63380
loc_6338F:
		LDX		_cur_lvl_idx
		LDY		_lvl_prg_bank_list,X
		JSR		_mmc1_prg
		RTS
_obj_shuffle_tables:
		.BYTE	 $C,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, $A, $B,  0,  0,  0
		.BYTE	 $C,  0,  4,  5,  6,  7,  8,  9, $B,  1,  2,  3, $A,  0,  0,  0
		.BYTE	 $C,  4,  5,  6,  0,  1,  2,  3,  7,  8,  9, $A, $B,  0,  0,  0
		.BYTE	 $C,  7,  8,  9,  0,  1,  2,  3,  4,  5,  6, $A, $B,  0,  0,  0

; =============== S U B	R O U T	I N E =======================================
_obj_spr_insert:
		LDA		_obj_flags,Y
		BNE		loc_633DE
		RTS
loc_633DE:
		AND		#$F7
		STA		_obj_flags,Y
		LDA		#1
		STA		_obj_offscreen_flag_tmp
		SEC
		LDA		_obj_lvl_x_lo,Y
		SBC		_tmpE5
		STA		_obj_scr_x_pos_tmp
		LDA		_obj_lvl_x_hi,Y
		SBC		_tmpE6
		BEQ		loc_63429
		CMP		#1
		BNE		loc_63402
		LDA		_obj_scr_x_pos_tmp
		CMP		#$32
		BCS		loc_6340C
		BCC		loc_63425
loc_63402:
		CMP		#$FF
		BNE		loc_6340C
		LDA		_obj_scr_x_pos_tmp
		CMP		#$CD
		BCS		loc_63425
loc_6340C:
		LDA		_obj_flags,Y
		ORA		#8
		STA		_obj_flags,Y
		LDA		_obj_hndl_idx,Y
		CMP		#$A
		BEQ		locret_63424
		CMP		#$15
		BEQ		locret_63424
		LDA		#0
		STA		_obj_flags,Y
locret_63424:
		RTS
loc_63425:
		LDA		#0
		STA		_obj_offscreen_flag_tmp
loc_63429:
		LDA		_obj_lvl_y,Y
		CLC
		ADC		_obj_global_y_delta
		SEC
		SBC		#9
		STA		_obj_scr_y_pos_tmp
		LDA		_obj_flags,Y
		AND		#$E0
		STA		_tmpE4
		LDA		_obj_pal_idx,Y
		AND		#3
		ORA		_tmpE4
		STA		_tmpE4
		AND		#$40
		PHP
		LDA		_obj_spr_idx,Y
		TAY
		LDA		_srp_lib_lo,Y
		STA		_ptr0
		LDA		_srp_lib_hi,Y
		STA		_ptr0+1
		LDY		#0
		LDA		(_ptr0),Y
		STA		_obj_spr_data_ptr_tmp
		INY
		LDA		(_ptr0),Y
		STA		_obj_spr_data_ptr_tmp+1
		INY
		PLP
		BEQ		loc_63467
		JMP		loc_634DB
loc_63467:
		INC		_player_explode_spr_remove_cnt
		BPL		loc_63473
		TYA
		CLC
		ADC		_obj_spr_data_ptr_tmp
		TAY
		JMP		loc_634CB
loc_63473:
		LDA		_obj_spr_data_ptr_tmp
		STA		_tmpE2
		LDA		(_ptr0),Y
		STA		_tmpE7
		CLC
		ADC		_obj_scr_x_pos_tmp
		STA		_tmpE2+1
		LDA		_tmpE7
		BMI		loc_6348D
		LDA		_obj_offscreen_flag_tmp
		BCC		loc_63493
		EOR		#1
		JMP		loc_63493
loc_6348D:
		LDA		_obj_offscreen_flag_tmp
		BCS		loc_63493
		EOR		#1
loc_63493:
		STA		_tmpE7
loc_63495:
		INY
		LDA		_tmpE7
		BEQ		loc_634B6
		LDA		(_ptr0),Y
		BEQ		loc_634B6
		STA		_spr_buf+1,X
		LDA		_tmpE2+1
		STA		_spr_buf+3,X
		LDA		_obj_scr_y_pos_tmp
		STA		_spr_buf,X
		LDA		_tmpE4
		STA		_spr_buf+2,X
		INX
		INX
		INX
		INX
		BEQ		locret_634DA
loc_634B6:
		DEC		_tmpE2
		BEQ		loc_634CB
		CLC
		LDA		_tmpE2+1
		ADC		#8
		STA		_tmpE2+1
		BCC		loc_63495
		LDA		_tmpE7
		EOR		#1
		STA		_tmpE7
		BPL		loc_63495
loc_634CB:
		DEC		_obj_spr_data_ptr_tmp+1
		BEQ		locret_634DA
		CLC
		LDA		_obj_scr_y_pos_tmp
		ADC		#8
		STA		_obj_scr_y_pos_tmp
		INY
		JMP		loc_63467
locret_634DA:
		RTS
loc_634DB:
		INC		_player_explode_spr_remove_cnt
		BPL		loc_634E3
		INY
		JMP		loc_63545
loc_634E3:
		LDA		_obj_spr_data_ptr_tmp
		STA		_tmpE2
		LDA		(_ptr0),Y
		EOR		#$FF
		CLC
		ADC		#1
		STA		_tmpE7
		CLC
		ADC		_obj_scr_x_pos_tmp
		STA		_tmpE2+1
		LDA		_tmpE7
		BMI		loc_63502
		LDA		_obj_offscreen_flag_tmp
		BCC		loc_63508
		EOR		#1
		JMP		loc_63508
loc_63502:
		LDA		_obj_offscreen_flag_tmp
		BCS		loc_63508
		EOR		#1
loc_63508:
		STA		_tmpE7
		TYA
		SEC
		ADC		_obj_spr_data_ptr_tmp
		TAY
loc_6350F:
		DEY
		LDA		_tmpE7
		BEQ		loc_63530
		LDA		(_ptr0),Y
		BEQ		loc_63530
		STA		_spr_buf+1,X
		LDA		_tmpE2+1
		STA		_spr_buf+3,X
		LDA		_obj_scr_y_pos_tmp
		STA		_spr_buf,X
		LDA		_tmpE4
		STA		_spr_buf+2,X
		INX
		INX
		INX
		INX
		BEQ		locret_63558
loc_63530:
		DEC		_tmpE2
		BEQ		loc_63545
		CLC
		LDA		_tmpE2+1
		ADC		#8
		STA		_tmpE2+1
		BCC		loc_6350F
		LDA		_tmpE7
		EOR		#1
		STA		_tmpE7
		BPL		loc_6350F
loc_63545:
		DEC		_obj_spr_data_ptr_tmp+1
		BEQ		locret_63558
		CLC
		LDA		_obj_scr_y_pos_tmp
		ADC		#8
		STA		_obj_scr_y_pos_tmp
		CLC
		TYA
		ADC		_obj_spr_data_ptr_tmp
		TAY
		JMP		loc_634DB
locret_63558:
		RTS

; =============== S U B	R O U T	I N E =======================================
_obj_proj_position_adjust:
		LDX		#$A
loc_6355B:
		LDA		_obj_flags,X
		BEQ		loc_635AB
		CLC
		LDA		_obj_unk,X
		TAY
		ADC		_obj_lvl_x_lo,X
		STA		_obj_lvl_x_lo,X
		TYA
		BMI		loc_63571
		BCC		loc_63575
		INC		_obj_lvl_x_hi,X
		JMP		loc_63575
loc_63571:
		BCS		loc_63575
		DEC		_obj_lvl_x_hi,X
loc_63575:
		CLC
		LDA		_star_field_data1+$18,X
		ADC		_obj_lvl_y,X
		STA		_obj_lvl_y,X
		DEC		_star_field_data2+$A,X
		BNE		loc_635AB
		LDA		_star_field_data2+$17,X
		STA		_star_field_data2+$A,X
		LDA		_star_field_data1+$B,X
		CMP		_obj_unk,X
		BEQ		loc_63598
		BMI		loc_63596
		INC		_obj_unk,X
		JMP		loc_63598
loc_63596:
		DEC		_obj_unk,X
loc_63598:
		LDA		_star_field_data1+$25,X
		CMP		_star_field_data1+$18,X
		BEQ		loc_635AB
		BMI		loc_635A8
		INC		_star_field_data1+$18,X
		JMP		loc_635AB
loc_635A8:
		DEC		_star_field_data1+$18,X
loc_635AB:
		DEX
		BPL		loc_6355B
		RTS

; =============== S U B	R O U T	I N E =======================================
_ingame_spr_pal_update_unused:
		LDA		_ppu_ctrl_shadow
		AND		#$FB
		STA		_ppu_ctrl_shadow
		STA		_PPU_CTRL
		LDA		_PPU_STATUS
		LDA		#$3F
		STA		_PPU_ADDR
		LDA		#$10
		STA		_PPU_ADDR
		LDY		#0
loc_635C7:
		LDA		_ingame_spr_pal_buf,Y
		STA		_PPU_DATA
		INY
		CPY		#$10
		BNE		loc_635C7
		RTS

; =============== S U B	R O U T	I N E =======================================
_ingame_pal_update:
		LDA		_ppu_ctrl_shadow
		AND		#$FB
		STA		_ppu_ctrl_shadow
		STA		_PPU_CTRL
		LDA		_PPU_STATUS
		LDA		#$3F
		STA		_PPU_ADDR
		LDA		#0
		STA		_PPU_ADDR
		LDX		_pal_update_redundant
		LDX		#0
		LDY		#$10
loc_635EF:
		LDA		_ingame_bg_pal_buf,X
		STA		_PPU_DATA
		INX
		DEY
		BNE		loc_635EF
		LDY		#0
loc_635FB:
		LDA		_ingame_spr_pal_buf,Y
		STA		_PPU_DATA
		INY
		CPY		#$10
		BNE		loc_635FB
		RTS

; =============== S U B	R O U T	I N E =======================================
_ppu_pal_clear:
		LDA		_ppu_ctrl_shadow
		AND		#$FB
		STA		_ppu_ctrl_shadow
		STA		_PPU_CTRL
		LDA		_PPU_STATUS
		LDA		#$3F
		STA		_PPU_ADDR
		LDA		#0
		STA		_PPU_ADDR
		LDY		#$20
		LDA		#$F
loc_63621:
		STA		_PPU_DATA
		INX
		DEY
		BNE		loc_63621
		RTS

; =============== S U B	R O U T	I N E =======================================
_delay256:
		LDY		#5
		LDX		#$99
loc_6362D:
		DEX
		BNE		loc_6362D
		DEY
		BNE		loc_6362D
		RTS

; =============== S U B	R O U T	I N E =======================================
_hud_init:
		LDA		_PPU_STATUS
		LDA		#<[_tlm_default_hud]
		STA		_tmp_ptr
		LDA		#>[_tlm_default_hud]
		STA		_tmp_ptr+1
		LDA		_ppu_ctrl_shadow
		AND		#$FB
		STA		_ppu_ctrl_shadow
		STA		_PPU_CTRL
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#0
		STA		_PPU_ADDR
		LDY		#0
loc_63654:
		LDA		(_tmp_ptr),Y
		STA		_PPU_DATA
		INY
		BNE		loc_63654
		LDA		_cur_lvl_idx
		ASL
		ASL
		ASL
		ASL
		TAX
		LDA		#$23
		STA		_PPU_ADDR
		LDA		#$C0
		STA		_PPU_ADDR
		LDY		#0
loc_6366F:
		LDA		_attr_hud_def_lib,X
		STA		_PPU_DATA
		INX
		INY
		CPY		#$10
		BNE		loc_6366F
		RTS

; =============== S U B	R O U T	I N E =======================================
_lvl_rooms_init_draw:
		LDY		_cur_lvl_state_idx
		LDA		_lvl_room_init_draw_ptr_list_lo,Y
		STA		_tmp_ptr
		STA		_tmpE2
		LDA		_lvl_room_init_draw_ptr_list_hi,Y
		STA		_tmp_ptr+1
		STA		_tmpE2+1
		LDA		_PPU_STATUS
		LDA		_ppu_ctrl_shadow
		AND		#$FB
		STA		_ppu_ctrl_shadow
		STA		_PPU_CTRL
		LDA		#$21
		STA		_PPU_ADDR
		LDA		#0
		STA		_PPU_ADDR
		LDY		#0
loc_636A4:
		LDA		(_tmp_ptr),Y
		STA		_PPU_DATA
		INY
		BNE		loc_636A4
		INC		_tmp_ptr+1
loc_636AE:
		LDA		(_tmp_ptr),Y
		STA		_PPU_DATA
		INY
		BNE		loc_636AE
		INC		_tmp_ptr+1
loc_636B8:
		LDA		(_tmp_ptr),Y
		STA		_PPU_DATA
		INY
		CPY		#$A0
		BNE		loc_636B8
		CLC
		LDA		_tmpE2
		ADC		#$A0
		STA		_tmp_ptr
		LDA		_tmpE2+1
		ADC		#2
		STA		_tmp_ptr+1
		LDA		#$23
		STA		_PPU_ADDR
		LDA		#$D0
		STA		_PPU_ADDR
		LDY		#0
loc_636DB:
		LDA		(_tmp_ptr),Y
		STA		_PPU_DATA
		INY
		CPY		#$30
		BNE		loc_636DB
		CLC
		LDA		_tmpE2
		ADC		#$D0
		STA		_tmp_ptr
		STA		_tmpE2
		LDA		_tmpE2+1
		ADC		#2
		STA		_tmp_ptr+1
		STA		_tmpE2+1
		LDA		_PPU_STATUS
		LDA		_ppu_ctrl_shadow
		AND		#$FB
		STA		_ppu_ctrl_shadow
		STA		_PPU_CTRL
		LDA		#$25
		STA		_PPU_ADDR
		LDA		#0
		STA		_PPU_ADDR
		LDY		#0
loc_6370E:
		LDA		(_tmp_ptr),Y
		STA		_PPU_DATA
		INY
		BNE		loc_6370E
		INC		_tmp_ptr+1
loc_63718:
		LDA		(_tmp_ptr),Y
		STA		_PPU_DATA
		INY
		BNE		loc_63718
		INC		_tmp_ptr+1
loc_63722:
		LDA		(_tmp_ptr),Y
		STA		_PPU_DATA
		INY
		CPY		#$A0
		BNE		loc_63722
		CLC
		LDA		_tmpE2
		ADC		#$A0
		STA		_tmp_ptr
		LDA		_tmpE2+1
		ADC		#2
		STA		_tmp_ptr+1
		LDA		#$27
		STA		_PPU_ADDR
		LDA		#$D0
		STA		_PPU_ADDR
		LDY		#0
loc_63745:
		LDA		(_tmp_ptr),Y
		STA		_PPU_DATA
		INY
		CPY		#$30
		BNE		loc_63745
		RTS

; =============== S U B	R O U T	I N E =======================================
_ppu_nt_clear:
		LDA		_ppu_ctrl_shadow
		AND		#$FB
		STA		_ppu_ctrl_shadow
		STA		_PPU_CTRL
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#0
		STA		_PPU_ADDR
		LDX		#0
		LDY		#0
		LDA		#0
loc_63769:
		STA		_PPU_DATA
		DEX
		BNE		loc_63769
		INY
		CPY		#8
		BNE		loc_63769
		RTS

; =============== S U B	R O U T	I N E =======================================
_spr_clear:
		LDA		#$F8
		LDY		#0
loc_63779:
		STA		_spr_buf,Y
		BNE		loc_63779
		RTS

; =============== S U B	R O U T	I N E =======================================
_apu_snd_load:
		LDA		_snd_idx
		BEQ		_apu_load
		LDA		_apu_sound_enable
		BEQ		locret_637CC
		JMP		_apu_load

; =============== S U B	R O U T	I N E =======================================
_apu_snd_reload:
		LDA		_apu_music_enable
		BEQ		locret_637CC

; =============== S U B	R O U T	I N E =======================================
_apu_load:
		LDA		_snd_idx
		BMI		locret_637CC
		LDY		#6
		JSR		_mmc1_prg
		LDA		#0
		STA		_ptr0+1
		LDA		_snd_idx
		ASL
		ROL		_ptr0+1
		ASL
		ROL		_ptr0+1
		ASL
		ROL		_ptr0+1
		CLC
		ADC		#<[_snt00_title_mus]
		STA		_ptr0
		LDA		_ptr0+1
		ADC		#>[_snt00_title_mus]
		STA		_ptr0+1
		LDY		#0
loc_637B5:
		STY		_apu_load_tmp_Y
		LDA		(_ptr0),Y
		BMI		locret_637CC
		TAX
		INY
		LDA		(_ptr0),Y
		JSR		_j_apu_load_chnl
		LDY		_apu_load_tmp_Y
		INY
		INY
		CPY		#8
		BNE		loc_637B5
locret_637CC:
		RTS
_snt00_title_mus:
		.BYTE	  0,  0,  1,  0,  2,  0,  3,  0
		.BYTE	  0,  1,  3,  2,$FF,$FF,$FF,$FF
		.BYTE	  3,  3,$FF,$FF,$FF,$FF,$FF,$FF
		.BYTE	  3,  4,$FF,$FF,$FF,$FF,$FF,$FF
		.BYTE	  2,  5,  3,  6,$FF,$FF,$FF,$FF
		.BYTE	  2,  7,  3,  8,$FF,$FF,$FF,$FF
		.BYTE	  2,  9,  3, $A,$FF,$FF,$FF,$FF
		.BYTE	  2, $B,  3, $C,$FF,$FF,$FF,$FF
		.BYTE	  0, $D,$FF,$FF,$FF,$FF,$FF,$FF
		.BYTE	  0, $F,$FF,$FF,$FF,$FF,$FF,$FF
		.BYTE	  3,$11,$FF,$FF,$FF,$FF,$FF,$FF
		.BYTE	  2,$13,  3,$14,$FF,$FF,$FF,$FF
		.BYTE	  0,$12,  2,$13,  3,$14,$FF,$FF
		.BYTE	  3,$3B,$FF,$FF,$FF,$FF,$FF,$FF
		.BYTE	  3,  0,$FF,$FF,$FF,$FF,$FF,$FF
		.BYTE	  0,$26,  3,$27,$FF,$FF,$FF,$FF
		.BYTE	  0,$28,  3,$29,$FF,$FF,$FF,$FF
		.BYTE	  3,$2A,$FF,$FF,$FF,$FF,$FF,$FF
_snd12_weap_change_unused:
		.BYTE	  0,$2B,  2,$2C,$FF,$FF,$FF,$FF
		.BYTE	  0,$30,  2,$32,$FF,$FF,$FF,$FF
		.BYTE	  0,$2D,  2,$31,$FF,$FF,$FF,$FF
		.BYTE	  0,$2D,  2,$32,$FF,$FF,$FF,$FF
		.BYTE	  0,$2D,  2,$33,$FF,$FF,$FF,$FF
		.BYTE	  0,$2E,  2,$31,$FF,$FF,$FF,$FF
		.BYTE	  0,$2E,  2,$32,$FF,$FF,$FF,$FF
		.BYTE	  0,$2E,  2,$33,$FF,$FF,$FF,$FF
		.BYTE	  0,$2F,  2,$31,$FF,$FF,$FF,$FF
		.BYTE	  0,$2F,  2,$32,$FF,$FF,$FF,$FF
		.BYTE	  0,$2F,  2,$33,$FF,$FF,$FF,$FF
		.BYTE	  0,$15,  1,$16,  2,$17,  3,$18
		.BYTE	  0,$1A,  1,$1B,  2,$1C,  3,$1D
		.BYTE	  0,$1E,  1,$1F,  2,$20,  3,$21
		.BYTE	  0,$22,  1,$23,  2,$24,  3,$25
_snd21_weap_change:
		.BYTE	  0,$2B,$FF,$FF,$FF,$FF,$FF,$FF
_snd22_lvl0_mus:
		.BYTE	  1,$19,$FF,$FF,$FF,$FF,$FF,$FF
_snd23_lvl1_mus:
		.BYTE	  1,$37,$FF,$FF,$FF,$FF,$FF,$FF
_snd24_lvl2_mus:
		.BYTE	  1,$38,$FF,$FF,$FF,$FF,$FF,$FF
_snd25_lvl3_mus:
		.BYTE	  1,$39,$FF,$FF,$FF,$FF,$FF,$FF
_snd26_lvl4_mus:
		.BYTE	  1,$3A,$FF,$FF,$FF,$FF,$FF,$FF

; =============== S U B	R O U T	I N E =======================================
_apu_play_prg_safe:
		LDX		#0
		JSR		_j_apu_play_chnl
		LDX		#1
		JSR		_j_apu_play_chnl
		LDX		#2
		JSR		_j_apu_play_chnl
		LDX		#3
		JSR		_j_apu_play_chnl
		LDX		_cur_lvl_idx
		LDY		_lvl_prg_bank_list,X
		JSR		_mmc1_prg
		RTS

; =============== S U B	R O U T	I N E =======================================
_mmc1_chr0:
		LDY		#$80
		STY		$BFFF
		STA		$BFFF
		LSR
		STA		$BFFF
		LSR
		STA		$BFFF
		LSR
		STA		$BFFF
		LSR
		STA		$BFFF
		RTS

; =============== S U B	R O U T	I N E =======================================
_mmc1_chr1:
		LDY		#$80
		STY		$DFFF
		STA		$DFFF
		LSR
		STA		$DFFF
		LSR
		STA		$DFFF
		LSR
		STA		$DFFF
		LSR
		STA		$DFFF
		RTS

; =============== S U B	R O U T	I N E =======================================
_mmc1_prg:
		LDA		#$80
		STA		$FFFF
		TYA
		STA		$FFFF
		LSR
		STA		$FFFF
		LSR
		STA		$FFFF
		LSR
		STA		$FFFF
		LSR
		STA		$FFFF
		RTS
_hud_score_num_tile_upper_list:
		.BYTE	$41,$42,$43,$44,$45,$46,$47,$48,$49,$4A
_hud_score_num_tile_lower_list:
		.BYTE	$4B,$4C,$4D,$4E,$4C,$4B,$4B,$4F,$4B,$50
_hud_weapon_lagre_icons_lib:
		.BYTE	$51,$52,$53,$54,$55,$56,$57,  0
		.BYTE	$58,$59,$5A,$5B,$5C,$5D,$5E,  0
		.BYTE	$F5,$F5
		.BYTE	$53,$54,$5F,$60,$61,$62,$63,  0
		.BYTE	$5A,$5B,  0,$64,$65,$66,$67,  0
		.BYTE	$F5,$F5
		.BYTE	$68,$69,$6A,$6B,$6C,$6D,$6E,  0
		.BYTE	$6F,$70,$71,$72,$73,$74,$75,  0
		.BYTE	$F5,$F5
		.BYTE	$76,$77,$78,$79,$7A,$7B,$7C,  0
		.BYTE	  0,  0,$7D,$7E,$7F,$80,$81,  0
		.BYTE	$F5,$F5
		.BYTE	$82,$83,$84,$85,$86, $B,$87,  0
		.BYTE	  0,$88,$89,$8A,$8B,$8C,$8D,  0
		.BYTE	$F5,$F5
		.BYTE	$8E,$8F,$90,$91,$92,$93,$94,  0
		.BYTE	$95,$96,$97,$98,$99,$9A,$9B,  0
		.BYTE	$F5,$F5
_tlm_hud_rocket:
		.BYTE	$9C,$9D,$9E,$9F,$A0,$A1,$A2,  0
		.BYTE	$A3,$A4,$A5,$A6,$A7,$A8,$A9,  0
_tlm_enemy_text:
		.BYTE	$AA,$AB,$AC,$AD,$AE,$36,$36
_tlm_default_hud:
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,$41,$41,$41,$41,$41,$41,$41,$AF,$B0,$B1,$B2,$B3,$B4,$B5,$B6,$B7,$B8,$B9,$BA,$BB,$BC,$BD,$BE,$68,$69,$6A,$6B,$6C,$6D,$6E,  0
		.BYTE	  0,$4B,$4B,$4B,$4B,$4B,$4B,$4B,$BF,$C0,$C1,$C2,$C3,$C4,$C5,$C6,$C7,$C8,$C9,$CA,$CB,$CC,$CD,$CE,$6F,$70,$71,$72,$73,$74,$75,  0
		.BYTE	  0,$CF,$D0,$D1,$D2,$D3,$D4,$AE,$D5,$D6,$D7,$D8,$D9,$DA,$DB,$DC,$DD,$DE,$DF,$E0,$E1,$E2,$E3,$E4,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,$E5,$E5,$E5,  0,  0,$E6,$E7,$E8,$E9,$EA,$EB,$EC,$ED,$EE,$EF,$F0,$F1,$F2,$F3,$F4,$F5,$AA,$AB,$AC,$AD,$AE,$36,$36,  0
		.BYTE	  0,$27,$29, $B, $C, $B,$1E, $C,$18,$1B,$17,$10, $B,$18,$10,$1E,$1E, $C,$12,$10,$2A,$28,  0,$2B,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
_attr_hud_def_lib:
		.BYTE	$AF,$AF,$3F, $F,$CF,$FF,$EF,$FF,$A5,$A5,$A3,$A8,$A2,$AC,$A5,$A5
		.BYTE	 $F, $F,$3F, $F,$CF,$FF,$EF,$FF,$A5,$A5,$A3,$A8,$A2,$AC,$A5,$A5
		.BYTE	$AF,$AF,$3F, $F,$CF,$FF,$EF,$FF,$A5,$A5,$A3,$A8,$A2,$AC,$A5,$A5
		.BYTE	$AF,$AF,$3F, $F,$CF,$FF,$EF,$FF,$A5,$A5,$A3,$A8,$A2,$AC,$A5,$A5
		.BYTE	$AF,$AF,$3F, $F,$CF,$FF,$EF,$FF,$A5,$A5,$A3,$A8,$A2,$AC,$A5,$A5
_obj_spr_lib:
		.BYTE	$3A,$44,$30,$54,$48,$3E,$4C,$34,$54,$50,$5A,$69,$6D,$71,$77,$83
		.BYTE	$65,$67,$65,$67,$7D,$88,$12,$B7,$B1,$AD,$12,  0,  0,  0,  0,  0
		.BYTE	  0,$AC,  1,  2,$59,  1,  2,  1,  0
_obj_initi_flags_lib:
		.BYTE	  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1
		.BYTE	  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0
		.BYTE	  0,  1,  1,  1,  1,  1,  1,  1,$21
_obj_pal_lib:
		.BYTE	  3,  2,  1,  3,  2,  2,  3,  1,  1,  1,  1,  1,  2,  1,  1,  1
		.BYTE	  1,  1,  1,  1,  1,  3,  2,  1,  1,  2,  2,  0,  0,  0,  0,  0
		.BYTE	  0,  3,  0,  0,  2,  0,  3,  3,  0
byte_63BD0:
		.BYTE	$63,$63,$63,$63,$63,$63,$63,$63,$63,$63,$63,$63,$63,$63,$63,$63
		.BYTE	$63,$63,$63,$63,$63,$63,  1,$63,$63,$63,  1,$63,$63,$63,$63,$63
		.BYTE	$63,$63,$63,$63,$63,$63,$63,$63,$63
_scores_table:
		.BYTE	  7,  5, $A, $A,  7,  7,  5, $A, $A,  7,$64, $A, $A, $A, $A, $A
		.BYTE	 $A, $A, $A, $A, $A,$C8,  5, $A, $A, $A,  5,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,$14,$14,$14,  0
byte_63C22:
		.BYTE	$1E,$32,$3C,$32,$64,$C8,  0,  0,  3,  3,  3, $F, $F,$32,  0,  0
_player_damage_list:
		.BYTE	  1,  2,  3, $A
_body_targets_list:
		.BYTE	  2,  0,  1
_lvl_prg_bank_list:
		.BYTE	  0,  1,  2,  3,  4
byte_63C3E:
		.BYTE	  9,  5, $E,  5,  5
byte_63C43:
		.BYTE	  0,  5,  0,  0,  3,  8,  0,  0,  0,  0
byte_63C4D:
		.BYTE	 $F,$18,$1B,$2B, $F,  6,$16,$26, $F,$18,$28,$38, $F,$12,$22,$32
		.BYTE	 $F, $C,$1C,$2C, $F,  6,$16,$26, $F,$18,$28,$38, $F,$12,$22,$32
		.BYTE	 $F,$22,$18,$29, $F,$18,$29,$39, $F,$18,$28,$38, $F,$12,$22,$32
		.BYTE	  0,  1,  1,  0,  0,  0
_def_score_tbl:
		.BYTE	  0,  0,  2,  0,  0,  7,$24,  8,  1,$12,$12,  9,$13, $F, $E
		.BYTE	  0,  0,  1,  0,  0,$13,$24, $E,  9,  3, $F, $C,  0,  0,  0
		.BYTE	  0,  0,  0,  8,  0,$13,$24,  3,$12, $F,$17,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  6,  0, $A,$24,  2, $F, $E,  1,$12,  0,  0,  0
		.BYTE	  0,  0,  0,  4,  0, $A,$24,$14,  5, $C,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  1,  0,  8,$24,  2,  9, $E, $E,$13,  0,  0,  0

		ORG	SYS_STUB_ORG, $00
RESET:
		SAFE_STUB0
		SAFE_STUB1
		SAFE_STUB2

		ORG	SYS_VECTOR_ORG, $00

		.WORD NMI
		.WORD RESET
		.WORD IRQ
