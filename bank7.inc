
		ORG $C000, $00

; =============== S U B	R O U T	I N E =======================================
; be careful! dumb loop routine may take different amount of clocks depending
; on location in memory! normal operation is 5900 clocks, if jump cross page
; boundary, delay will be 7000 clocks!!!
;
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
; delay 18 clocks for 1 loop, total delay 13+5*X
;
_delayx:
		DEX
		BNE		_delayx
		RTS

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
		TXA
loc_60023:
		STA		$000,X
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
		LDX		#[_def_score_tbl_end-_def_score_tbl]
loc_60044:
		LDA		_def_score_tbl,X
		STA		_ram_score_tbl,X
		DEX
		BNE		loc_60044
		STX		_nmi_hndl_idx
		LDA		#APU_MUS_ENABLE|APU_SND_ENABLE
		STA		_apu_enable
		LDY		#5
		JSR		_mmc1_prg
		JSR		_ppu_reinit
		JSR		_attract_display						; copyrights, logos and title with hi-scores

; =============== S U B	R O U T	I N E =======================================
_MAIN_LOOP:
		JSR		_ppu_reinit
		LDA		#$23
		STA		_rand_seed								; setup rng
		LDA		#$4C
		STA		_rand_seed+1
		LDA		#$E6
		STA		_rand_seed+2
		LDA		#$B1
		STA		_rand_seed+3
		LDA		#0
;		STA		_hud_msg_idx
		STA		_lvl2_smart_bomb_flag					; init special livel folags
		STA		_lvl4_cur_pass_char_idx
		JSR		_hud_msg_set_request_ex					; actually init it for the first time 
		LDA		#2
;		STA		_hud_msg_expose_time+1					; msg expose time init for some unknown reason
														; any msg call reinit it for sure, seems no need here
		STA		_player_damage_blink_timer
		STA		_lvl4_pass_enter_timeout_hi				; for some reason only hi value is initialized
														; low param is 0 at start, so we have 512 frames
														; before the first pass char is entered and
														; 200 for every other, maybe this is a plan?
		LDA		_lvl_idx
		JSR		_bios_msg_display
		JSR		_ppu_reinit
_GAME_START:
		SEI
		LDA		_PPU_STATUS
		LDX		#$FF
		STX		_pad_block_mask
		INX												; write 0
		STX		_PPU_CTRL
		STX		_PPU_MASK
		STX		_ppu_ctrl_shadow
		STX		_ppu_mask_shadow
		STX		_APU_STATUS
		STX		_nmi_is_over_flag
		STX		_pause_flag
		INX												; write 1
		STX		_lvl4_minutes_timer						; set lvl4 timer to 01:15
		INX												; write 2
		STX		_game_delay_nmi_counter					; set the delay counter
		INX												; write 3
		STX		_nmi_hndl_idx							; set the nmi routine to game init
		LDA		#$F
		STA		_lvl4_seconds_timer						; set lvl4 timer to 01:15
		STA		_lvl4_clock_delay						; lvl4 load delay timer (60/4 = 15)
		LDA		#$E7
		STA		_ppu_mask_shadow_mask
;		LDX		_lvl_idx								; set the lvl data bank
;		LDY		_lvl_prg_bank_list,X
		JSR		_lvl_prg_init
		JSR		_lvl_vars_init							; read level data from external banks
.ingame_init_loop0:
		LDA		_nmi_is_over_flag						; wait for next nmi, first will clear screen
		BEQ		.ingame_init_loop0						; second will draw the level itself
		LDA		#0
		STA		_nmi_is_over_flag
		JSR		_main_ingame_handler					; run game handlers once in normal mode
		DEC		_game_delay_nmi_counter					; do 2 frames to be sure all is ok
		BNE		.ingame_init_loop0
		LDA		#1
		STA		_pause_flag								; set the pause flag
		LDA		#2
		STA		_game_delay_nmi_counter
.ingame_init_loop1:
		LDA		_nmi_is_over_flag						; wait for another 2 frames before game
		BEQ		.ingame_init_loop1						; start
		LDA		#0
		STA		_nmi_is_over_flag
		JSR		_main_ingame_handler
		DEC		_game_delay_nmi_counter
		BNE		.ingame_init_loop1
		LDA		#0
		STA		_pause_flag								; unpause again and start the actual gameplay
		STA		_nmi_is_over_flag
		LDY		_lvl_state_idx
		LDA		_lvl_bg_pal_list_lo,Y					; setup the palettes finally
		STA		_ptr0
		LDA		_lvl_bg_pal_list_hi,Y
		STA		_ptr0+1
		LDY		#$F
loc_60152:
		LDA		(_ptr0),Y
		STA		_ingame_bg_pal_buf,Y
		DEY
		BPL		loc_60152
		LDY		_lvl_state_idx
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
_MAIN_INGAME_LOOP:
		LDA		_nmi_is_over_flag						; wait for sync
		BEQ		_MAIN_INGAME_LOOP
		LDA		#0
		STA		_nmi_is_over_flag
		JSR		_main_ingame_handler					; do main game code
		JSR		_randomize								; forward random counters
		LDA		_game_over_flag							; test if game is over
		BEQ		_MAIN_INGAME_LOOP						; if > 0, then level is complete
		BPL		_LEVEL_COMPLETE							; if < 0, then game over
; --------------------------------
; game over branch begins here
		LDY		#5
		JSR		_mmc1_prg
		DEC		_continues								; decrease continues
		JSR		_ppu_reinit
		LDA		#0
		STA		_nmi_hndl_idx
		STA		_lvl_state_idx
		JSR		_game_over_display
		JSR		_player_percents_restore				; restore percents at last checkpoint (start game, repair lab, bonus)
		JMP		_MAIN_LOOP
; --------------------------------
_LEVEL_COMPLETE:
		LDY		#5
		JSR		_mmc1_prg
		INC		_lvl_idx
		LDA		_lvl_idx								; test if the game is complete
		CMP		#5
		BEQ		.game_complete
		LDA		#0
		STA		_nmi_hndl_idx
		STA		_lvl_state_idx
		JSR		_ppu_reinit								; prepare to enter the repair lab
		LDA		_lvl_idx
		CMP		#4										
		BNE		.do_repair								; there is no repair after the level 4, except a secret one
		JMP		_MAIN_LOOP
; --------------------------------
.do_repair:
		LDA		#6
		JSR		_bios_msg_display
		JSR		_repair_lab_display
		JMP		_MAIN_LOOP
; --------------------------------
.game_complete:
		JSR		_ppu_reinit
		LDA		#0
		STA		_nmi_hndl_idx
		LDA		#8
		JSR		_bios_msg_display
		JSR		_ending_display
		JMP		_MAIN_LOOP

; =============== S U B	R O U T	I N E =======================================
_player_percents_backup:
		LDA		_percents.head
		STA		_head_percent_backup
		LDA		_percents.body
		STA		_body_percent_backup
		LDA		_percents.arms
		STA		_arms_percent_backup
		LDA		_percents.legs
		STA		_legs_percent_backup
		RTS
		
; =============== S U B	R O U T	I N E =======================================
_player_percents_restore:
		LDA		_head_percent_backup
		STA		_percents.head
		LDA		_body_percent_backup
		STA		_percents.body
		LDA		_arms_percent_backup
		STA		_percents.arms
		LDA		_legs_percent_backup
		STA		_percents.legs

; =============== S U B	R O U T	I N E =======================================
_player_overall_armor_calc:
		LDA		#0
		STA		_overal_armor_sum+1
		LDA		_percents.head
		ORA		_percents.body
		ORA		_percents.arms
		ORA		_percents.legs
		BEQ		.death
		CMP		#4
		BCS		.full_calc
		LDA		#1
.death:
		STA		_overal_armor_sum
		RTS
.full_calc:
		CLC												; sum body + head
		LDA		_percents.head
		ADC		_percents.body
		BCC		.no_car0
		INC		_overal_armor_sum+1
.no_car0:
		ADC		_percents.arms
		BCC		.no_car1
		INC		_overal_armor_sum+1
.no_car1:
		ADC		_percents.legs
		BCC		.no_car2
		INC		_overal_armor_sum+1
.no_car2:
		STA		_overal_armor_sum
		ROR		_overal_armor_sum+1						; / 2
		ROR		_overal_armor_sum
		LDA		_overal_armor_sum						; return value
		RTS
		
; =============== S U B	R O U T	I N E =======================================
_player_vars_init:
		LDA		#0
		STA		_lvl_idx
		STA		_score_hex
		STA		_score_hex+1
		STA		_weapon0_idx
		STA		_weapon1_idx
		STA		_weapon_type
		LDA		#DEF_PERCENT_VALUE
		STA		_percents.head
		STA		_percents.body
		STA		_percents.arms
		STA		_percents.legs
		LDA		#5
		STA		_repair_tanks
		LDA		#3
		STA		_continues
		JSR		_player_overall_armor_calc
		JSR		_player_percents_backup
		JMP		_ppu_pal_clear

; =============== S U B	R O U T	I N E =======================================
_read_pads:
		LDY		#1
		STY		_PAD0_REG
		DEY
		STY		_PAD0_REG
		LDY		#8
loc_6125A:
		PHA
		LDA		_PAD0_REG
		STA		_pad_data
		LSR
		ORA		_pad_data
		LSR
		PLA
		ROL
		DEY
		BNE		loc_6125A
		LDY		_pad_last								; backup prev input
		STY		_pad_data
		STA		_pad_last								; save new input
		LDA		_pad_block_mask							; test for special mask case
		BNE		loc_61250								; if 0 then swap left and right
		LDA		_pad_last
		AND		#3
		TAX
		LDA		_pad_last
		AND		#$FC
		ORA		_dpad_swap_list,X
		JMP		loc_61251
loc_61250:
		AND		_pad_last								; mask last data with block value
loc_61251:
		STA		_pad_last								; save again filtered value
		EOR		_pad_data								; do edge calcs
		AND		_pad_last
		STA		_pad_edge
		RTS

_dpad_swap_list:
		.BYTE	  0,  2,  1,  3

; =============== S U B	R O U T	I N E =======================================
_lvl_vars_init:
		SEI
		LDX		#0
		STX		_lvl_bg_chr_treshold+1
		STX		_player_scroll_stop_flag
		STX		_obj_shuffle_mode
		STX		_game_over_flag
		STX		_pause_flag
		STX		_player_explode_timer_tmp
		STX		_player_sit_jump_flag
		STX		_special_boss_flood_object_flag
;		STX		_lvl_rooms_walked_count_unused
		STX		_lvl2_tank_weapon_position_switch
		STX		_lvl2_tank_weapon_disable_flag
		LDA		#3
		STA		_lvl2_tank_shot_delay
		STA		_lvl2_jetpack_fuel_tanks
		LDY		_lvl_state_idx
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
		LDY		#$F										; set black palette before init the level
		LDA		#$F										; to hide first 4 frames of level initialization
loc_603B6:
		STA		_ingame_bg_pal_buf,Y
		STA		_ingame_spr_pal_buf,Y
		DEY
		BPL		loc_603B6
loc_603BF:
		LDA		_PPU_STATUS
		BPL		loc_603BF
		LDY		_lvl_state_idx
		LDA		_lvl_obj_global_y_delta_list,Y
		STA		_obj_global_y_delta
		LDA		_lvl_stop_sctoll_room_idx_list,Y
		STA		_lvl_stop_sctoll_room_idx
		LDA		_lvl_spr_chr_bank_up_list,Y
		STA		_lvl_spr_chr_bank_up
		STA		_lvl_spr_chr_bank_up_backup
		LDA		_lvl_spr_chr_bank_down_list,Y
		STA		_lvl_spr_chr_bank_down
		STA		_lvl_spr_chr_bank_down_backup
		LDA		#$FF
		STA		_pal_blink_timer
		LDA		#$1E
		STA		_lvl_bonus_portal_delay
		LDA		#$14
		STA		_special_boss_flood_object_count
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
		LDA		#SND_SILENCE
		STA		_snd_idx
		JSR		_far_apu_mus_load_unsafe				; mute all current sounds
		LDY		_lvl_idx
		LDA		_lvl_mus_list,Y
		STA		_snd_idx
		JSR		_far_apu_mus_load_unsafe				; load level bg music
		JMP		_ppu_reinit
		
_lvl_mus_list:
		.BYTE	SND_MLEVEL0,SND_MLEVEL1,SND_MLEVEL2,SND_MLEVEL3,SND_MLEVEL4

; =============== S U B	R O U T	I N E =======================================
; used only from bank 5 routines, changes bank to 6 and restores it back to 5
;
_far_apu_mus_load_prg5_safe:
		STA		_snd_idx
		JSR		_far_apu_mus_load_unsafe
		LDY		#5
		JMP		_mmc1_prg

; =============== S U B	R O U T	I N E =======================================
; used only from bank 5 routines, changes bank to 6 and restores it back to 5
;
_far_apu_play_prg5_safe:
		LDY		#6
		JSR		_mmc1_prg
		JSR		_apu_play
		LDY		#5
		JMP		_mmc1_prg

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
		STA		_tmp_ptr
		LDA		_nmis_lib+1,Y
		STA		_tmp_ptr+1
		JMP		(_tmp_ptr)

_nmis_lib:
		.WORD _nmi0	; nmi routine for intro is in bank 5, it should be set before start!
		.WORD _nmi0
		.WORD _nmi0
		.WORD _nmi3
		.WORD _nmi4
		.WORD _nmi5

; =============== S U B	R O U T	I N E =======================================
; general ingame nmi routine
;
_nmi5:
		BIT		_PPU_STATUS								; from here
		LDX		#0
		STX		_PPU_MASK
		STX		_PPU_CTRL
		STX		_PPU_OAM_ADDR
		LDA		#2
		STA		_PPU_OAM_DMA
		LDA		_ppu_ctrl_shadow
		AND		#$78
		STA		_PPU_CTRL
		STA		_ppu_ctrl_shadow
		LDA		#$18
		STA		_PPU_MASK
		STA		_ppu_mask_shadow
		JSR		_ingame_pal_update
		JSR		_hud_ppu_data_send
		JSR		_lvl_ppu_data_dequeue
		LDX		#0
		STX		_PPU_SCROLL
		LDA		#5
		STA		_PPU_SCROLL
		LDA		_lvl_spr_chr_bank_up
		JSR		_mmc1_chr1
		LDA		#0
		JSR		_mmc1_chr0								; to here 2200-2300 clocks original
		JSR		_delay256								; should be 5900 clocks delay
		LDA		_lvl_bg_chr_bank						; timing based HUD back switch delay!
		JSR		_mmc1_chr0								; just wait for particular number of ticks...
		LDA		_ppu_mask_shadow						; set the game bg bank! 
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
;		LDX		_lvl_idx
;		LDY		_lvl_prg_bank_list,X
		JSR		_lvl_prg_init
		JSR		_hud_init
		JSR		_lvl_rooms_init_draw
		INC		_nmi_hndl_idx
		JMP		_nmi_on_exit

; =============== S U B	R O U T	I N E =======================================
_lvl_ppu_data_dequeue:
		LDA		_lvl_idx
		ASL
		TAY
		LDA		_ppu_dequeues_lib,Y
		STA		_tmp_ptr
		LDA		_ppu_dequeues_lib+1,Y
		STA		_tmp_ptr+1
		JMP		(_tmp_ptr)

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
		STA		_tptr1+1
		PLA
		LSR
		LSR
		LSR
		LSR
		LSR
		ORA		#$D0
		STA		_PPU_ADDR
		STA		_tptr1
		LDX		#0
loc_605E3:
		LDA		_tmp_buf1,X
		STA		_PPU_DATA
		LDA		_tptr1+1
		STA		_PPU_ADDR
		CLC
		LDA		_tptr1
		ADC		#8
		STA		_tptr1
		STA		_PPU_ADDR
		INX
		CPX		#6
		BCC		loc_605E3
		LDA		_ppu_ctrl_shadow
		AND		#$FB
		STA		_ppu_ctrl_shadow
		STA		_PPU_CTRL
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
		LDX		#$53
		JSR		_delayx
		RTS

; =============== S U B	R O U T	I N E =======================================
_lvl_scroll_handlers:
		LDA		_lvl_idx
		CMP		#4
		BEQ		.lvl4_special_handler
		JMP		.regular_lvl_handler
; --------------------------------
.lvl4_special_handler:
		LDY		#$E0
		LDA		_hud_msg_cursor_blink_timer
		BPL		loc_6070B
		LDY		#$F5
loc_6070B:
		STY		_tmp_buf0
		LDA		_hud_msg_cursor_blink_timer
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
		LDY		_lvl4_cur_pass_char_idx
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
		LDA		_lvl4_cur_pass_char_idx
		CMP		#$C
		BCC		loc_60757
		LDA		_hud_msg_cursor_blink_timer
		BMI		loc_607AA
loc_60757:
		LDX		#0
loc_60759:
		CPX		_lvl4_cur_pass_char_idx
		BEQ		loc_60769
		LDA		_lvl4_pass_chars_list,X
		STA		_tmp_buf0+2,X
		INX
		CPX		#$C
		BCC		loc_60759
loc_60769:
		LDA		_player_back_standing_flag
		BEQ		loc_607AA
		DEC		_lvl4_pass_enter_timeout_lo
		BNE		loc_607AA
		DEC		_lvl4_pass_enter_timeout_hi
		BMI		loc_607AA
		LDA		#2
		STA		_lvl4_pass_enter_timeout_hi
		LDA		#$64
		STA		_lvl4_pass_enter_timeout_lo
		INC		_lvl4_cur_pass_char_idx
		LDY		_lvl4_cur_pass_char_idx
		CPY		#$D
		BCS		loc_60797
		LDA		_lvl4_pass_snd_list,Y
		STA		_snd_idx
loc_60797:
		LDY		#$B
		LDA		_lvl4_cur_pass_char_idx
		CMP		#$C
		BCC		loc_607A2
		LDY		#$C
loc_607A2:
		TYA
		JSR		_hud_msg_set_request
loc_607AA:
		LDA		_lvl4_cur_pass_char_idx
		CMP		#$C
		BCC		locret_607D5
		DEC		_lvl4_pass_done_snd_delay
		BPL		locret_607D5
		LDA		#$A
		STA		_lvl4_pass_done_snd_delay
		INC		_lvl4_pass_done_snd_pos
		LDY		_lvl4_pass_done_snd_pos
		CPY		#$C
		BCC		loc_607C7
		LDY		#0
loc_607C7:
		STY		_lvl4_pass_done_snd_pos
		LDA		_lvl4_pass_snd_list,Y
		STA		_snd_idx
locret_607D5:
		RTS
_lvl4_pass_chars_list:
		.BYTE	$FF,$FA,$FB,$F7
		.BYTE	$F9,$F8,$FD,$FE
		.BYTE	$FC,$F6,$FF,$F9
_lvl4_pass_snd_list:
		.BYTE	SND_PASSBP9,SND_PASSBP4,SND_PASSBP5,SND_PASSBP1
		.BYTE	SND_PASSBP3,SND_PASSBP2,SND_PASSBP7,SND_PASSBP8
		.BYTE	SND_PASSBP6,SND_PASSBP0,SND_PASSBP9,SND_PASSBP3
; --------------------------------
.regular_lvl_handler:
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
;		INC		_lvl_rooms_walked_count_unused
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
		ADD16	_lvl_room_data_ptr, $20
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
;		DEC		_lvl_rooms_walked_count_unused
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
		ADD16	_lvl_room_data_ptr, $20
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
		STA		_lvl_bg_chr_bank
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
		STA		_tmp_ptr
		LDA		_hud_ppu_data_update_lib+1,Y
		STA		_tmp_ptr+1
		JMP		(_tmp_ptr)

_hud_ppu_data_update_lib:
		.WORD _hud_msg_ppu_data_update
		.WORD _hud_all_energy_ppu_data_update
		.WORD _hud_score_weapon_ppu_data_update
		.WORD _hud_large_weapon_ppu_data_update

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
.copy_msg_until_cur_pos:
		LDA		(_ptr0),Y
		BEQ		.write_over_char
		STA		_tmp_buf2,Y
		INY
		CPY		_hud_msg_type_pos
		BCC		.copy_msg_until_cur_pos
		BEQ		loc_6093B
.write_over_char:
		LDA		#_rq
		STA		_tmp_buf2,Y								; write right "quote" char
		INY
loc_6093B:
		CLC
		LDA		_hud_msg_cursor_blink_timer				; blink counters
		ADC		#$20
		STA		_hud_msg_cursor_blink_timer
		BMI		loc_6094A
		LDA		#_cur									; write cursor
		STA		_tmp_buf2,Y
		INY
loc_6094A:
		LDA		#0										; fill the rest with zeroes
loc_60950:
		CPY		#$1C
		BEQ		loc_6095B								; check for end of buffer first
		STA		_tmp_buf2,Y								; write if any
		INY
		BNE		loc_60950
loc_6095B:
		LDA		_hud_msg_type_pos						; increase msg position, up to $1B
		CMP		#$1B
		BCS		loc_60963
		INC		_hud_msg_type_pos						; this occurs every 4th frame, so does typing delay
loc_60963:
		DEC		_hud_msg_expose_time
		BNE		locret_6097A
		DEC		_hud_msg_expose_time+1
		BNE		locret_6097A
		LDA		_rand_seed+1							; message displayed, time to randomly select another one.
		AND		#3
		CMP		_hud_msg_idx							; if the same as current, force it to be different
		BNE		_hud_msg_set_request_ex
		EOR		#1
		BPL		_hud_msg_set_request_ex
locret_6097A:
		RTS

; =============== S U B	R O U T	I N E =======================================
_hud_msg_set_request:
		CMP		_hud_msg_idx
		BEQ		.no_override

; =============== S U B	R O U T	I N E =======================================
_hud_msg_set_request_ex:
		STA		_hud_msg_idx
		LDA		#$C8
		STA		_hud_msg_expose_time
		LDA		#2
		STA		_hud_msg_expose_time+1
		LDA		#0
		STA		_hud_msg_type_pos						; do not remove this 0, some code rely on it!
.no_override:
		RTS

_hud_msg_lib:
		.WORD	_hud_msg00
		.WORD	_hud_msg01
		.WORD	_hud_msg02
		.WORD	_hud_msg03
		.WORD	_hud_msg04
		.WORD	_hud_msg05_damage_alert
		.WORD	_hud_msg06_fire_upgrade
		.WORD	_hud_msg07
		.WORD	_hud_msg08
		.WORD	_hud_msg09_system_failure
		.WORD	_hud_msg0A_missile_jammed
		.WORD	_hud_msg0B_passcode_accepted
		.WORD	_hud_msg0C_otomo_destruction
		.WORD	_hud_msg0D
		.WORD	_hud_msg0E_smart_bomb_deployed
		.WORD	_hud_msg0F
		.WORD	_hud_msg10
		.WORD	_hud_msg11
		.WORD	_hud_msg12
		.WORD	_hud_msg13_jetpack_fuel_low
		.WORD	_hud_msg14_maximum_fuel
		.WORD	_hud_msg15_tank_weapon_disabled
		.WORD	_hud_msg16_head_failure
		.WORD	_hud_msg17_body_failure
		.WORD	_hud_msg18_arms_failure
		.WORD	_hud_msg19_legs_failure
		.WORD	_hud_msg1A_out_of_time
		
_hud_msg00:
		.BYTE	_p,_r,_o,_t,_e,_c,_t,__,_t,_h,_e,__,_i,_n,_n,_o,_c,_e,_n,_t,_dot,0
_hud_msg01:
		.BYTE	_u,_p,_h,_o,_l,_d,__,_t,_h,_e,__,_l,_a,_w,_dot,0
_hud_msg02:
		.BYTE	_c,_o,_m,_e,__,_a,_n,_d,__,_g,_e,_t,__,_i,_t,__,_s,_c,_u,_m,_dot,0
_hud_msg03:
		.BYTE	_s,_e,_r,_v,_e,__,_t,_h,_e,__,_p,_u,_b,_l,_i,_c,__,_t,_r,_u,_s,_t,_dot,0
_hud_msg04:
		.BYTE	_e,_f,_f,_i,_c,_i,_e,_n,_c,_y,__,_i,_n,_c,_r,_e,_a,_s,_e,_d,_dot,0
_hud_msg05_damage_alert:
		.BYTE	_s,_y,_s,_t,_e,_m,_s,__,_d,_a,_m,_a,_g,_e,__,_a,_l,_e,_r,_t,_dot,0
_hud_msg06_fire_upgrade:
		.BYTE	_f,_i,_r,_e,__,_p,_o,_w,_e,_r,__,_u,_p,_g,_r,_a,_d,_e,_d,_dot,0
_hud_msg07:
		.BYTE	_m,_i,_s,_s,_i,_l,_e,_s,__,_u,_p,_g,_r,_a,_d,_e,_d,_dot,0
_hud_msg08:
		.BYTE	_r,_e,_p,_a,_i,_r,__,_i,_t,_e,_m,__,_o,_b,_t,_a,_i,_n,_e,_d,_dot,0
_hud_msg09_system_failure:
		.BYTE	_s,_y,_s,_t,_e,_m,__,_f,_a,_i,_l,_u,_r,_e,_dot,0
_hud_msg0A_missile_jammed:
		.BYTE	_m,_i,_s,_s,_i,_l,_e,__,_j,_a,_m,_m,_e,_d,_dot,0
_hud_msg0B_passcode_accepted:
		.BYTE	_p,_a,_s,_s,_c,_o,_d,_e,__,_a,_c,_c,_e,_p,_t,_e,_d,_dot,0
_hud_msg0C_otomo_destruction:
		.BYTE	_o,_t,_o,_m,_o,__,_d,_e,_s,_t,_r,_u,_c,_t,__,_i,_n,_i,_t,_i,_a,_t,_e,_d,_dot,0
_hud_msg0D:
		.BYTE	_s,_m,_a,_r,_t,__,_b,_o,_m,_b,__,_i,_n,_i,_t,_i,_a,_t,_e,_d,_dot,0
_hud_msg0E_smart_bomb_deployed:
		.BYTE	_s,_m,_a,_r,_t,__,_b,_o,_m,_b,__,_d,_e,_p,_l,_o,_y,_e,_d,_dot,0
_hud_msg0F:
		.BYTE	_w,_a,_r,_n,_i,_n,_g,__,_h,_e,_a,_d,__,_u,_n,_i,_t,__,_d,_a,_m,_a,_g,_e,_d,_dot,0
_hud_msg10:
		.BYTE	_w,_a,_r,_n,_i,_n,_g,__,_b,_o,_d,_y,__,_u,_n,_i,_t,__,_d,_a,_m,_a,_g,_e,_d,_dot,0
_hud_msg11:
		.BYTE	_w,_a,_r,_n,_i,_n,_g,__,_a,_r,_m,__,_u,_n,_i,_t,_s,__,_d,_a,_m,_a,_g,_e,_d,_dot,0
_hud_msg12:
		.BYTE	_w,_a,_r,_n,_i,_n,_g,__,_l,_e,_g,__,_u,_n,_i,_t,_s,__,_d,_a,_m,_a,_g,_e,_d,_dot,0
_hud_msg13_jetpack_fuel_low:
		.BYTE	_w,_a,_r,_n,_i,_n,_g,__,_j,_e,_t,_p,_a,_c,_k,__,_f,_u,_e,_l,__,_l,_o,_w,_dot,0
_hud_msg14_maximum_fuel:
		.BYTE	_m,_a,_x,_i,_m,_u,_m,__,_f,_u,_e,_l,_dot,0
_hud_msg15_tank_weapon_disabled:
		.BYTE	_t,_a,_n,_k,__,_w,_e,_a,_p,_o,_n,_s,__,_d,_i,_s,_a,_b,_l,_e,_d,_dot,0
_hud_msg16_head_failure:
		.BYTE	_h,_e,_a,_d,__,_u,_n,_i,_t,__,_m,_a,_l,_f,_u,_n,_c,_t,_i,_o,_n,_dot,0
_hud_msg17_body_failure:
		.BYTE	_b,_o,_d,_y,__,_u,_n,_i,_t,__,_m,_a,_l,_f,_u,_n,_c,_t,_i,_o,_n,_dot,0
_hud_msg18_arms_failure:
		.BYTE	_a,_r,_m,__,_u,_n,_i,_t,_s,__,_m,_a,_l,_f,_u,_n,_c,_t,_i,_o,_n,_dot,0
_hud_msg19_legs_failure:
		.BYTE	_l,_e,_g,__,_u,_n,_i,_t,_s,__,_m,_a,_l,_f,_u,_n,_c,_t,_i,_o,_n,_dot,0
_hud_msg1A_out_of_time:
		.BYTE	_o,_u,_t,__,_o,_f,__,_t,_i,_m,_e,_dot,0

; =============== S U B	R O U T	I N E =======================================
_j_hud_lvl4_timer_ppu_data_update:
		JMP		_hud_lvl4_timer_ppu_data_update

; =============== S U B	R O U T	I N E =======================================
_hud_all_energy_ppu_data_update:
		LDA		#$36
		STA		_hex2str_tile_delta
		LDA		_overal_armor_sum
		U8TOS	_tmp_buf2								; draw armor number
		LDA		_lvl2_smart_bomb_flag					; ckeck if smartbomb taken
		BNE		.hud_draw_smartbomb
		LDY		#3										; if not, draw enemy energy
.look_for_active_enemy:
		LDA		_obj_flags,Y							; find active enemy
		BNE		.active_enemy_found						; Y loaded with enemy idx
		DEY
		BNE		.look_for_active_enemy
		LDA		#$40
		STA		_tmp_buf2+$12							; draw -- placeholder if no enemy
		STA		_tmp_buf2+$13
		BNE		loc_60CD4
.active_enemy_found:
;		LDA		#$63									; whatwhat? default 99 value? not used anymore
		LDX		_lvl_idx								; check for additional case
		CPX		#4										; timer in the last level instead of energy
		BEQ		_j_hud_lvl4_timer_ppu_data_update
		LDX		_last_enemy_hit							; first we found an active enemy, then use
		LDA		_obj_energy,X							; different index lol, maybe we should check it first?
		BNE		loc_60CCB
		LDA		_obj_energy,Y							; last hit enemy died, choose another one
loc_60CCB:
		U8TOS	_tmp_buf2+$F
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
.hud_draw_smartbomb:
		LDX		#7
loc_60CF5:
		LDA		_tlm_hud_rocket,X
		STA		_tmp_buf2+5,X
		LDA		_tlm_hud_rocket+8,X
		STA		_tmp_buf2+$D,X
		DEX
		BPL		loc_60CF5
		DEC		_smart_bomb_timer
		BNE		locret_60D49
		LDA		#$7F
		STA		_pal_blink_timer
		LDA		#0
		STA		_lvl2_smart_bomb_flag
		LDA		#$E
		JSR		_hud_msg_set_request_ex
		LDA		#SND_BIGBOOM
		STA		_snd_idx
		LDA		_obj_x_hi._player
		CMP		#$E
		BCC		locret_60D49
		CMP		#$F
		BCS		loc_60D30
		LDA		_obj_x_lo._player
		CMP		#$50
		BCC		locret_60D49
loc_60D30:
		INC		_lvl2_tank_weapon_disable_flag
		CLC
		LDA		_score_hex
		ADC		#$F4
		STA		_score_hex
		LDA		_score_hex+1
		ADC		#1
		STA		_score_hex+1
		LDA		#$15
		JMP		_hud_msg_set_request_ex
locret_60D49:
		RTS

; =============== S U B	R O U T	I N E =======================================
_hud_lvl4_timer_ppu_data_update:
		DEC		_lvl4_clock_delay						; this function is calling every 4th frame
		BNE		.do_lvl4_timer_draw						; so to count 1 second, skip 15 times on NTSC!
		LDA		#$F
		STA		_lvl4_clock_delay						; reload skip timer
		DEC		_lvl4_seconds_timer
		BNE		.do_lvl4_timer_draw
		LDA		_lvl4_minutes_timer
		BNE		.lvl4_dec_minutes
;		LDA		#$1A									; were unused, wrong msg index (max 19), nothing
;		JSR		_hud_msg_set_request_ex					; is displayed because of immediate game over
		LDA		#$FF
		STA		_game_over_flag
		RTS
.lvl4_dec_minutes:
		LDA		#$3B									; after first 15 sec is over, reload the rest with 59
		STA		_lvl4_seconds_timer
		DEC		_lvl4_minutes_timer
.do_lvl4_timer_draw:
;		LDA		#$36
;		STA		_hex2str_tile_delta						; actually, already loaded before
		LDA		_lvl4_seconds_timer
		U8TOS	_tmp_buf2+$10-2							; write seconds
		LDA		#$F6									; write : original tyle is empty!
		STA		[_tmp_buf2+$12-2]
		LDA		_lvl4_minutes_timer
		U8TOS	_tmp_buf2+$D-2							; write minutes
		LDX		#9										; erase the rest of the buffer
		LDA		#0
loc_60C5F:
		STA		_tmp_buf2+5,X
		DEX
		BPL		loc_60C5F
;		STA		_tmp_buf2+$12
		STA		_tmp_buf2+$13
		STA		_tmp_buf2+$14
		RTS

; =============== S U B	R O U T	I N E =======================================
_hud_score_weapon_ppu_data_update:
		LDA		#0
		STA		_hex2str_tile_delta
		U16TOS	_score_hex, _tmp_buf2					; write scores
		LDX		#4
loc_60D62:
		LDY		_tmp_buf2,X
		LDA		_hud_score_num_tile_upper_list,Y
		STA		_tmp_buf2,X
		LDA		_hud_score_num_tile_lower_list,Y
		STA		_tmp_buf2+5,X
		DEX
		BPL		loc_60D62
		LDY		_lvl_idx
		LDA		_hud_weap0_attr0_overlay_list,Y
		STA		_hud_weap0_attr_overlay
		LDA		_hud_weap0_attr1_overlay_list,Y
		STA		_hud_weap0_attr_overlay+1
		LDA		_hud_weap1_attr0_overlay_list,Y
		STA		_hud_weap1_attr_overlay
		LDA		_hud_weap1_attr1_overlay_list,Y
		STA		_hud_weap1_attr_overlay+1
		LDA		_lvl_idx
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
		STA		_tptr1
		ASL
		ASL
		ASL
		CLC
		ADC		_tptr1
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
		
; =============== S U B	R O U T	I N E =======================================
_hud_ppu_data_send:
		LDA		_hud_update_ppu_data_counter
		ASL
		TAY
		LDA		_hud_ppu_data_send_lib,Y
		STA		_tmp_ptr
		LDA		_hud_ppu_data_send_lib+1,Y
		STA		_tmp_ptr+1
		JMP		(_tmp_ptr)

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
		JSR		_delayx
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
		JSR		_delayx
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
		LDX		#9
		JSR		_delayx
		RTS

; =============== S U B	R O U T	I N E =======================================
_hex2str:
		STX		_ptr1
		STY		_ptr1+1
		LDY		#0
loc_6112A:
		LDX		#$FF
		SEC
loc_6112D:
		INX
		LDA		_str_ptr
		SBC		_dec_list_lo,Y
		STA		_str_ptr
		LDA		_str_ptr+1
		SBC		_dec_list_hi,Y
		STA		_str_ptr+1
		BCS		loc_6112D
		LDA		_str_ptr
		ADC		_dec_list_lo,Y
		STA		_str_ptr
		LDA		_str_ptr+1
		ADC		_dec_list_hi,Y
		STA		_str_ptr+1
		TXA
		CLC
		ADC		_hex2str_tile_delta
		STA		(_ptr1),Y
		INY
		CPY		#5
		BNE		loc_6112A
		RTS
		
_dec_list_lo:
		.BYTE	$10,$E8,$64, $A,  1
_dec_list_hi:
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
_failure_test_handler:
		DEC		_failure_duration_period
		BPL		locret_602D5
		LDA		#$64
		STA		_failure_duration_period
		LDA		_rand_seed
		AND		#3
		TAX
		LDA		_percents,X
		CMP		#FAILURE_TRESHOLD
		BCS		.no_failure
		LDA		_rand_seed
		CMP		#$14
		BCS		.no_failure
		LDA		_pad_block_masks_list,X
		STA		_pad_block_mask
		TXA
		CLC
		ADC		#$16
		JMP		_hud_msg_set_request
.no_failure:
		LDA		#$FF
		STA		_pad_block_mask
		LDA		_hud_msg_idx
		CMP		#$16
		BCC		locret_602D5
		LDA		#0
		JMP		_hud_msg_set_request_ex
locret_602D5:
		RTS

_pad_block_masks_list:
		.BYTE	  0,$7B,$BF,$78

; =============== S U B	R O U T	I N E =======================================
_main_ingame_handler:
		LDA		#$FF
		STA		_snd_idx
		STA		_ppu_mask_shadow_mask
		JSR		_failure_test_handler					; test failure event, set mask for pad block
		JSR		_read_pads								; start main frame, read pards first
		INC		_ingame_sync_counter					; increase frame counter
		LDA		_pause_flag								; skip pause handler
		BEQ		.no_paused_handler						; pause has its own branch
		LDA		_pad_edge
; --
		AND		#$20									; DEBUG level skip
		BEQ		loc_6118F								; dev leftover, hold Select on pause to skip this
		LDA		#1
		STA		_game_over_flag							
; --
loc_6118F:
		LDA		_pad_edge
		AND		#$10
		BEQ		.skip_ingame_handlers1					; unpause with Start
		LDA		#0
		STA		_pause_flag								; remove pause flag
		JMP		.skip_ingame_handlers1
.no_paused_handler:
		JSR		_lvl_obj_collision_test					; common ingame handlers, part 1 collisions, scroll/sprite/obj managers
		JSR		_far_obj_misc_collision_test_lvl_prg_safe
		JSR		_player_input_control
		LDA		_obj_y._player
		AND		#$F0
		STA		_player_y_meta16_tile_tmp
		JSR		_lvl_scroll_handlers
		JSR		_far_spr_manager_lvl_prg_safe		; set bank 6, return back lvl bank
		JSR		_lvl_obj_manager						; read far data from lvl prg bank
.skip_ingame_handlers1:
		LDA		_lvl_idx								; one more special check for level 4
		CMP		#4										; only level 4 has no sprite0hit chr data change
		BNE		.spr0hit_wait								; you don't need to wait for the spr0hit flag raised
		LDA		_PPU_STATUS
		JMP		.spr0hit_skip							; so just skip the loop
.spr0hit_wait:
		LDA		_PPU_STATUS								; in any other cases, wait for spr0hit
		AND		#$C0
		BEQ		.spr0hit_wait
.spr0hit_skip:
		LDA		_lvl_spr_chr_bank_down					; change bottom sprite chr bank to draw lower enemies
		JSR		_mmc1_chr1
		JSR		_obj_hidden_portal_test_and_bonus_animate	; one more any time test routine, working on pause either
		LDY		#6
		JSR		_mmc1_prg
		LDA		_pause_flag								; the rest o
		BNE		.skip_ingame_handlers2
		JSR		_hud_update_ppu_data					; graphics update routines, main ai handlers, misc objects handlers
		JSR		_obj_enemy_ai_manager
		JSR		_obj_position_adjust
		JSR		_player_bullets_animate
		JSR		_enemy_bullet_explosion_anim
		JSR		_obj_obj_collision_tests
		JSR		_lvl_bg_chr_switch
		JSR		_player_damage_blink
		JSR		_far_apu_snd_load_unsafe				; start queued in previous ai handlers sound effects
		JSR		_lvl_flood_boss_complete_test
		JSR		_bg_pal_blink_handler
.skip_ingame_handlers2:
		JSR		_apu_play								; jump directly to bank 6, it is already set here
;		LDX		_lvl_idx								; restore back current level data bank
;		LDY		_lvl_prg_bank_list,X
		JMP		_lvl_prg_init

; =============== S U B	R O U T	I N E =======================================
_lvl_flood_boss_complete_test:
		LDA		_special_boss_flood_object_count
		BNE		locret_6122C							; enemies not over yet
		LDA		_special_boss_flood_object_flag			; and they are used at all, means there is an end of
		BEQ		locret_6122C							; enemy list reached for thie level (FF)
		LDA		_obj_flags._enemy						; test for last enemies die
		ORA		_obj_flags._enemy+1
		ORA		_obj_flags._enemy+2
		BNE		locret_6122C
		LDA		_game_over_flag							; finally game over flag haven't set yet.
		BNE		locret_6122C
		LDA		_overal_armor_sum						; and player still alive
		BEQ		locret_6122C
		PLA												; do break and exit from the main loop
		PLA
		PLA												; we in the TWO jsr's under the main loop, so need two more PLA's
		PLA
		JMP		_LEVEL_COMPLETE
locret_6122C:
		RTS

; =============== S U B	R O U T	I N E =======================================
_bg_pal_blink_handler:
		LDA		_pal_blink_timer
		BPL		loc_6123A								; if timer counting, do full pal xoring
		LDA		_ingame_bg_pal_buf						; or just write the clamped max color
		AND		#$30
		BNE		loc_6123A								; if not a zero
		RTS
loc_6123A:
		DEC		_pal_blink_timer						; decrease timer
		LDY		#$F
loc_6123F:
		LDA		_ingame_bg_pal_buf,Y					; do palette data xoring
		EOR		#$30
		STA		_ingame_bg_pal_buf,Y
		DEY
		BPL		loc_6123F
		RTS

; =============== S U B	R O U T	I N E =======================================
_player_input_control:
		LDA		_obj_y._player							; test for instant death first in level pits
		CMP		#$F0
		BCS		.no_pit									; all Y positions between E0 and F0 are deadly, others don't
		CMP		#$E0
		BCC		.no_pit
		LDA		#$FF
		STA		_game_over_flag							; death flag, instant game over
		RTS
.no_pit:
		LDA		_lvl_idx
		CMP		#2
		BNE		.walk_control							; hardwired for level 3 only
		JMP		_jetpack_control
; --------------------------------
.walk_control:
		LDA		_lvl4_cur_pass_char_idx					; hack to keep robocop stay after password in level 5 is entered
		CMP		#$C
		BCC		loc_61290								; check for the numbers entered first
		LDA		#8
		STA		_pad_last								; if positive, force pads to Up
loc_61290:
		LDA		#0
		STA		_player_back_standing_flag				; disableturn back flag, it not needed on the regular levels
		LDA		_overal_armor_sum						; check for death animation sequence
		BNE		.lvl1_scroll_adjust_test
		LDA		#0
		STA		_pad_last								; disable input
		LDA		_player_sit_jump_flag					; test if in mid air, so wait for landing
		BNE		.lvl1_scroll_adjust_test
		JMP		_player_explosion_anim					; and BOOOOM
; ------------------------
.lvl1_scroll_adjust_test:
		LDA		_player_scroll_stop_flag				; used for special ninja case to stop scrolling
		CMP		#2
		BNE		.acid_test
		LDA		_obj_x_hi._player
		CMP		_lvl_stop_sctoll_room_idx				; regular stop scroll test
		BEQ		.stop_scroll
		LDX		_player_wnd_scroll_counter
		CPX		#$74
		BCC		.stop_scroll							; walk until X = $74 without scroll
		DEC		_player_wnd_scroll_counter
		LDA		_pad_last								; maskout left-right movement keys, and do scroll increase
		AND		#$FC
		STA		_pad_last
		JSR		_scroll_inc
		JMP		.acid_test
.stop_scroll:
		LDA		#0
		STA		_player_scroll_stop_flag
; ------------------------
.acid_test:
		LDY		#0										; other special level test case a lot of them!
		STY		_tptr1									; clear tmp flags
		LDX		#0
		LDA		_obj_flags._player						; remove special player flag
		AND		#$DF
		LDY		_lvl_idx								; check for particular level
		CPY		#1
		BNE		.no_acid_test							; special level 2 acid test
		LDY		_obj_y._player							; check for particular Y pos
		CPY		#$DC
		BCS		.no_acid_test							; acid is at Y $BC-$DC
		CPY		#$BC
		BCC		.no_acid_test
		ORA		#$20									; special player flag raise, 20
		LDY		_lvl1_player_pal_acid_lockup			; lock flag
		STY		_tptr1									; pal value
		BNE		.skip_acid_sound						; first pass here lock = 0, start sound
		LDY		#SND_ACIDHIS							; further passes lock isn't zero, no more sound start
		STY		_snd_idx
		STY		_tptr1
.skip_acid_sound:
		LDX		#$7A									; hide player's foot under the acid value
.no_acid_test:
		LDY		_tptr1									; write tmp, lock and pal values
		STY		_lvl1_player_pal_acid_lockup
		STA		_obj_flags._player						; write player flags
		STX		_player_spr_out_reload					; position of player sprite to remove
; ------------------------
		LDY		#1										; left/right movement starts here
		LDA		_pad_edge								; edge triggers increment #8
		AND		#3										; or 1 otherwise (actual movement test not here)
		BEQ		loc_6130E
		LDY		#8
loc_6130E:
		STY		_player_hor_move_anim_increment
; ------------------------
		LDA		_player_shoot_state_timer				; read shoot timer (init value 0)
		BEQ		.do_shot_state_change
		DEC		_player_shoot_state_timer				; clock every 16 frames (1/4 of sec)
		JMP		.mid_air_animations
.do_shot_state_change:
		JSR		_player_shot_anim_phase_change
; ------------------------
.mid_air_animations:
		LDA		_player_sit_jump_flag					; if mid-air (1), then adjust hor controls
		AND		#1										; (2) for landing or sit-down state, dosn't matter here
		BNE		.do_mid_air_anims
		JMP		.sit_down_state
.do_mid_air_anims:
		LDA		_player_spr_out_reload					; only in acid and while exploding
		BNE		.no_hor_move_lock
		LDA		_player_edge_fall_flag					; set to 1 only if slip from the obstacles
		BEQ		.no_hor_move_lock						; clear to 0 if jump
		LDA		_pad_last								; prevent the mid-air horizontal movement
		AND		#$FC									; control in that case, do only forced
		ORA		_player_edge_fall_flag					; left or right movenet according to fall direction
		STA		_pad_last
.no_hor_move_lock:
		LDA		#0
		STA		_player_diagonal_fire_delta
		LDY		_player_anim_phase_offset				; check if we fire in mid-air
		CPY		#$A
		BNE		loc_61352
		LDA		#1										; adjust mid-air frame
loc_61352:
		CLC
		ADC		#$23									; set mid-air sprite idx (base $23 + 0 to 1)
		STA		_obj_spr_idx._player
		LDA		_obj_y_velocity._player					; check the y movement direction
		BPL		.mid_air_landing
		JMP		.misc_controls						; do nothing more here
; ------------------------
.mid_air_landing:
		LDA		_lvl_idx								; special check for level 2 falling case
		CMP		#1
		BNE		.regular_landing						; test for Y $BC-$D2 range, stop player
		LDA		_obj_y._player							; at the $BC, do not let it fall down further
		CMP		#$D2
		BCS		.regular_landing
		CMP		#$BC
		BCC		.regular_landing
		LDA		#$BC
		STA		_obj_y._player							; lock Y position, update collision flags
		LDA		_obj_hcoll._player						; there are no collision data for acid
		ORA		#1										; it will threaded as pit otherwise
		STA		_obj_hcoll._player
		STA		_obj_vcoll._player
		LDA		#0
		STA		_obj_y_velocity._player					; clear Y velocity
		STA		_obj_y_velosity_target._player
		STA		_player_sit_jump_flag					; complete stop
		JMP		.sit_down_state_chenge_test
.regular_landing:
		LDA		#0
		STA		_player_spr_out_reload
		LDA		_obj_y._player							; test for general direction of Y movement
		AND		#$F0
		STA		_tptr1
		LDA		_player_y_meta16_tile_tmp				; last saven Y tile less than cur Y tile pos
		CMP		_tptr1									; then we going down
		BCC		.collision_landing_test
		JMP		.misc_controls						; do nothing more here
.collision_landing_test:
		LDA		_obj_vcoll._player						; test for collision with ground or obstacles
		ORA		_obj_hcoll._player
		AND		#1
		BNE		.do_landing
		JMP		.misc_controls						; if no, do nothing more
.do_landing:
		LDA		_obj_y._player							; stop at 16 px tile boundary
		AND		#$F0
		STA		_obj_y._player
		LDA		#0
		STA		_obj_y_velocity._player					; clear velocity
		STA		_obj_y_velosity_target._player
		LDA		#2
		STA		_player_sit_jump_flag					; set sit flag for landing animation
		LDA		#8
		STA		_obj_sit_state._player					; when landing set sit down anim counter to 8			
		LDA		#SND_RBLAND2							; do landing sound
		STA		_snd_idx
		JMP		.do_sit_anim_sprite
; ------------------------
.sit_down_state:
		JSR		_lvl1_player_transporter_pos_adjust
		LDA		_player_sit_jump_flag
		AND		#2
		BNE		.sit_down_already						; if we land, sit down state is forced here
; ------------------------
.sit_down_state_chenge_test:
		LDA		_pad_last
		CMP		#4
		BEQ		.do_sit_down							; sit down button test
		BNE		.start_fall_from_edge_test						
; ------------------------
.sit_down_already:
		LDA		_obj_hcoll._player
		ORA		_obj_vcoll._player
		AND		#1
		BEQ		.start_fall_from_edge
		LDA		_pad_last
		AND		#4
		BNE		.do_sit_down							; if down released
		DEC		_obj_sit_state._player					; stand up time, rewind sit down states backward
		BNE		.do_sit_anim_sprite
		LDA		_player_sit_jump_flag					; clear sitdown flag
		AND		#$FD
		STA		_player_sit_jump_flag
		JMP		.misc_controls							; do normal land anim
.do_sit_down:
		LDA		_player_sit_jump_flag					; if Down is still pressed here, keep sit down state on
		ORA		#2
		STA		_player_sit_jump_flag
		LDA		_obj_sit_state._player
		CMP		#9
		BCS		.do_sit_anim_sprite						; do increase sitdown anim counter until 9
		INC		_obj_sit_state._player
; ------------------------
.do_sit_anim_sprite:
		LDA		_player_anim_phase_offset				; here we actually draw the sit down sprite if detected
		CMP		#$A
		PHP												; keed compare state flags
		STY		_tptr1
		LDA		_obj_sit_state._player					; get sit down anim phase
		LSR
		LSR
		PLP
		ADC		#$26
		STA		_obj_spr_idx._player					; set sprite ($26 base + 0 to 2)
		LDA		#0
		STA		_player_diagonal_fire_delta				; can't fire diagonally when seated
		LDA		#$1A
		STA		_player_shot_pos_delta					; set Y delta for shot position adjust if any
		JMP		.misc_controls
; ------------------------
.start_fall_from_edge_test:
		LDA		_obj_hcoll._player
		ORA		_obj_vcoll._player
		AND		#1
		BNE		.stand_still							; in mid-air test for obstacle occurs
; ------------------------
.start_fall_from_edge:
		LDA		#8
		STA		_obj_y_velosity_target._player			
		LDA		_player_sit_jump_flag					; set mid-air flag
		ORA		#1
		STA		_player_sit_jump_flag
		LDA		#$F0
		STA		_player_y_meta16_tile_tmp				; init base tile to land
		LDY		#2
		LDA		_obj_flags._player						; get player face direction
		AND		#$40
		BEQ		loc_61440
		LDY		#1
loc_61440:
		STY		_player_edge_fall_flag					; set fall edge flag left or right accordingly
		JMP		.misc_controls							; leave the rest to the other logic
; ------------------------
.stand_still:
		LDA		#0
		STA		_player_sit_jump_flag					; clear mid-air flag
		LDA		#$A
		STA		_player_shot_pos_delta					; set base position for shooting ($A)
		LDA		#0
		STA		_player_diagonal_fire_delta				; default direction
		LDA		_pad_last
		AND		#$8F
		BNE		.up_press_test							; test if any d-pad button of jump pressed
		STA		_player_hor_move_anim_timer				; if no, then just stand
		CLC
		LDA		#$18
		ADC		_player_anim_phase_offset				; default stand sprite
		STA		_obj_spr_idx._player
		JMP		.misc_controls
.up_press_test:
		LDA		_pad_last
		AND		#$83
		BNE		loc_614DA								; check for jump and/or movement
		LDA		_pad_last								; if no, up test is possible
		AND		#8
		BEQ		.stand_ready_for_sit_down				; check if Up is pressed
		LDA		_lvl_hidden_portal_detected				; check if a hidden portal under the foot
		BEQ		.stand_ready_to_fire_up
		LDA		#$25
		STA		_obj_spr_idx._player					; change sprite idx to show player's back
		LDA		_lvl_idx
		CMP		#4
		BNE		.do_stand_delay							; do not raise this flag in any level except 5
		LDA		#1
		STA		_player_back_standing_flag				; level 5 pass entry flag
locret_6149B:
		RTS												; shorter to ret, than branch then ret in any cases
.do_stand_delay:
		DEC		_lvl_bonus_portal_delay					; any other level than 5 will count stand time
		BPL		locret_6149B
		INC		_lvl_state_idx							; then switch the next level part
		PLA
		PLA
		PLA												; two more needed to not to damage stack
		PLA
		LDA		_lvl_idx								; special check for level 4, do not switch level data
		CMP		#3										; but go to the repair lab
		BNE		loc_6149B
		LDY		#0
		STY		_nmi_hndl_idx
		INY		
		STY		_lvl_state_idx
		JSR		_ppu_reinit
		LDY		#5
		JSR		_mmc1_prg
		LDA		#6
		JSR		_bios_msg_display
		JSR		_repair_lab_display
loc_6149B:
		JMP		_GAME_START
; ------------------------
.stand_ready_to_fire_up:
		LDA		#$FC
		STA		_player_diagonal_fire_delta
		LDA		_player_anim_phase_offset
		CMP		#$A
		LDA		#0
		ADC		#$2A									; stand fire up base sprite $2A +0 to 1 according to shot mode
		STA		_obj_spr_idx._player					
		LDA		#$FD
		BNE		loc_614D0								; set player delta and block hor movements common branch
; ------------------------
.stand_ready_for_sit_down:
		LDA		_pad_last
		AND		#4
		BEQ		loc_614DA								; test for down is pressed
;		LDA		#4										; here we have 4 in reg already
		STA		_player_diagonal_fire_delta				; we got here if no jump or movement keys are pressed
		LDA		_player_anim_phase_offset				; this allows to shot diagonally down
		CMP		#$A
		LDA		#0
		ADC		#$2C
		STA		_obj_spr_idx._player					; base sprite $2C for diagonal down +0 to 1 anim phases
		LDA		#$20
loc_614D0:
		STA		_player_shot_pos_delta
		LDA		_pad_last								; block hor movements as well
		AND		#$FC
		STA		_pad_last
		JMP		.misc_controls
; ------------------------
loc_614DA:
		LDA		_player_hor_move_anim_timer				; walk or stand sprites set
		AND		#$1F
		LSR
		LSR
		LSR
		CLC
		ADC		#$14
		ADC		_player_anim_phase_offset				; base $14
		STA		_obj_spr_idx._player
		LDA		_pad_last
		AND		#$80
		BEQ		.misc_controls							; test for jump button
		LDA		#8
		STA		_obj_y_velosity_target._player			; target velocity is positive - go down
		LDA		#$FB
		STA		_obj_y_velocity._player					; start velocity negative - go up, so we can do up/down calcs
		LDA		#5
		STA		_obj_var3._player
		LDA		#4
		STA		_obj_var4._player
		LDA		_player_sit_jump_flag					; yep, jump flag raised
		ORA		#1
		STA		_player_sit_jump_flag
		LDA		#$27
		STA		_obj_spr_idx._player					; jump sprite fixed
		LDA		#$F0
		STA		_player_y_meta16_tile_tmp				; base for collision test
		LDA		#0
		STA		_player_edge_fall_flag					; we not falling from the edges, so no need to block hor movements
; ------------------------
.misc_controls:
		JSR		_non_movement_controls
		LDA		_player_spr_out_reload					; tmp var to indicate acid damage flag
		BEQ		.additional_special_lvl_ctrl_test		; can be non zero here if we in acid
		LDA		#3
		STA		_body_target_idx						; do damage to legs then
		LDA		#ACID_DAMAGE_VALUE
		JSR		_player_damage_apply
.additional_special_lvl_ctrl_test:
		LDA		_player_sit_jump_flag					; nothing to do here if we sit down, just skip
		AND		#2
		BEQ		.additional_special_lvl_controls
		RTS
; ------------------------
.additional_special_lvl_controls:
		LDA		_lvl_idx								
		CMP		#3
		BNE		loc_61570
		JMP		.lvl3_hor_move_controls
loc_61570:
		CMP		#4
		BNE		.common_hor_move_controls
		JMP		.lvl4_hor_move_controls
; ------------------------
.common_hor_move_controls:
		LDA		_pad_last
		AND		#2
		BEQ		.common_right_move_test					; move to the left
		LDA		_obj_flags._player
		AND		#$BF
		STA		_obj_flags._player						; clear right flip flag
		LDA		_obj_hcoll._player						; if no obstacle to the left
		AND		#2
		BNE		locret_615A4
		LDA		_player_wnd_scroll_counter				; and not the left side of screen
		BEQ		locret_615A4
		DEC		_player_wnd_scroll_counter
		JSR		_player_x_pos_dec						; move left
		JSR		_player_hor_animation					; do animation timers
locret_615A4:
		RTS
; ------------------------
.common_right_move_test:
		LDA		_pad_last
		AND		#1
		BEQ		locret_6160C
		LDA		_obj_flags._player						; set right flip flag
		ORA		#$40
		STA		_obj_flags._player
		LDA		_obj_hcoll._player						; if no obstacles to the right
		AND		#4
		BNE		locret_6160C
		JSR		_player_hor_animation					; do animation timers
		LDA		_player_scroll_stop_flag				; test if stop scroll set
		CMP		#1
		BEQ		loc_615D1
		LDA		_obj_x_hi._player						; check for last room number
		CMP		_lvl_stop_sctoll_room_idx
		BCC		loc_615E7								; not yet, go further without restrictions
		LDA		_obj_x_lo._player						; test for cure player x pos (center of the screen)
		CMP		#$74
		BCC		loc_615E7
loc_615D1:
		LDA		_player_wnd_scroll_counter				; special check for lvl 2 moving transporters
		CMP		#$E6									; which is forcing the stoll to move right
		BCS		locret_6160C							; if any transporters are go beyond the middle
		INC		_player_wnd_scroll_counter				; of the last scrollable screen
		JMP		_player_x_pos_inc
loc_615E7:
		JSR		_player_x_pos_inc						; otherwise, do pos adjust as well as scroll pos adjust
		LDA		_player_wnd_scroll_counter
		CMP		#$74
		BEQ		_scroll_inc
		INC		_player_wnd_scroll_counter
locret_6160C:
		RTS
; ------------------------
_scroll_inc:
		CLC
		LDA		_lvl_scroll_value
		ADC		#1
		STA		_lvl_scroll_value
		LDA		_lvl_nt_value
		ADC		#0
		AND		#1
		STA		_lvl_nt_value
		RTS
; ------------------------
_player_x_pos_inc:
		CLC
		LDA		_obj_x_lo._player
		ADC		#1
		STA		_obj_x_lo._player
		BCC		.player_x_pos_inc
		INC		_obj_x_hi._player
.player_x_pos_inc:
		RTS
; ------------------------
_player_x_pos_dec:
		SEC
		LDA		_obj_x_lo._player
		SBC		#1
		STA		_obj_x_lo._player
		BCS		.player_x_pos_dec
		DEC		_obj_x_hi._player
.player_x_pos_dec:
		RTS
; ------------------------
_player_hor_animation:
		CLC
		LDA		_player_hor_move_anim_timer
		ADC		_player_hor_move_anim_increment
		STA		_player_hor_move_anim_timer
		RTS
; ------------------------
.lvl3_hor_move_controls:
		LDA		_pad_last								; mostly the same as common controls, but
		AND		#2										; used to move in opposite direction
		BEQ		.lvl3_right_move_test					; all the same, except there is no special
		LDA		_obj_flags._player						; check for transporter
		AND		#$BF
		STA		_obj_flags._player
		LDA		_obj_hcoll._player
		AND		#2
		BNE		locret_619DB
		JSR		_player_hor_animation					; do animation timers
		LDA		_obj_x_hi._player
		CMP		_lvl_stop_sctoll_room_idx
		BNE		loc_619B6
		LDA		_obj_x_lo._player
		CMP		#$75
		BCS		loc_619B6
		LDA		_player_wnd_scroll_counter
		BEQ		locret_619DB
		DEC		_player_wnd_scroll_counter
		JMP		_player_x_pos_dec
loc_619B6:
		JSR		_player_x_pos_dec
		LDA		_player_wnd_scroll_counter
		CMP		#$75
		BCC		_scroll_dec
		DEC		_player_wnd_scroll_counter
locret_619DB:
		RTS
; ------------------------
_scroll_dec:
		SEC
		LDA		_lvl_scroll_value
		SBC		#1
		STA		_lvl_scroll_value
		LDA		_lvl_nt_value
		SBC		#0
		AND		#1
		STA		_lvl_nt_value
		RTS
; ------------------------
.lvl3_right_move_test:
		LDA		_pad_last
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
		JSR		_player_x_pos_inc
		JSR		_player_hor_animation					; do animation timers
locret_61A0B:
		RTS
; ------------------------
.lvl4_hor_move_controls:
		LDA		_pad_last								; level 5 is a small level with basic room scroll
		AND		#2										; but the rest is exactly the same as common
		BEQ		.lvl4_right_move_test
		LDA		_obj_flags._player
		AND		#$BF
		STA		_obj_flags._player
		LDA		_obj_hcoll._player
		AND		#2
		BNE		locret_61661
		LDA		_player_wnd_scroll_counter
		BEQ		locret_61661
		LDA		_obj_x_hi._player
		BNE		loc_61632
		LDA		_obj_x_lo._player
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
		JSR		_scroll_dec
loc_6164C:
		JSR		_player_x_pos_dec
		JSR		_player_hor_animation					; do animation timers
locret_61661:
		RTS
.lvl4_right_move_test:
		LDA		_pad_last
		AND		#1
		BEQ		locret_61661
		LDA		_obj_flags._player
		ORA		#$40
		STA		_obj_flags._player
		LDA		_obj_hcoll._player
		AND		#4
		BNE		locret_61661
		JSR		_player_hor_animation					; do animation timers
		LDA		_obj_x_hi._player
		CMP		#1
		BCC		loc_6169E
		LDA		_obj_x_lo._player
		CMP		#$74
		BCC		loc_6169E
		LDA		_player_wnd_scroll_counter
		CMP		#$E6
		BCS		locret_61661
		INC		_player_wnd_scroll_counter
		JMP		_player_x_pos_inc
loc_6169E:
		JSR		_player_x_pos_inc
		LDA		_player_wnd_scroll_counter
		CMP		#$74
		BEQ		loc_616B4
		INC		_player_wnd_scroll_counter
		RTS
loc_616B4:
		JMP		_scroll_inc

; =============== S U B	R O U T	I N E =======================================
_non_movement_controls:
		LDA		_pad_last
		AND		#$40
		BEQ		.loc_61530								; also we can fire while jumping
		JSR		_player_anim_fire_control
.loc_61530:
		LDA		_pad_edge								; test for pause is pressed
		AND		#$10
		BEQ		loc_6153A
		LDA		#1
		STA		_pause_flag
loc_6153A:
		LDA		_pad_edge								; or weapon is selecting
		AND		#$20
		BEQ		loc_61552
		LDA		_weapon_type
		EOR		#1
		AND		#1
		STA		_weapon_type
		LDA		#SND_WCHANGE							; weapon change sound
		STA		_snd_idx
loc_61552:
		RTS
		
; =============== S U B	R O U T	I N E =======================================
_jetpack_control:
		LDA		_overal_armor_sum
		BNE		.jetpack_inputs							; full set of controls for the jet pack stage
		LDA		#0
		STA		_pad_last								; block input if dead
		LDA		_player_sit_jump_flag					; and wait for landing if in flight mode
		BNE		.jetpack_inputs
		JMP		_player_explosion_anim
.jetpack_inputs:
		LDA		#$14
		STA		_player_spr_out_reload
		LDY		#1
		LDA		_pad_edge
		AND		#3
		BEQ		loc_61760
		LDY		#8
loc_61760:
		STY		_player_hor_move_anim_increment
		LDA		_player_shoot_state_timer
		BEQ		loc_6176E
		DEC		_player_shoot_state_timer
		JMP		loc_6177F
loc_6176E:
		JSR		_player_shot_anim_phase_change
loc_6177F:
		LDA		_player_sit_jump_flag
		AND		#1
		BNE		loc_61788
		JMP		loc_61834
loc_61788:
		LDA		#0
		STA		_player_diagonal_fire_delta
		LDY		_player_anim_phase_offset
		CPY		#$A
		BNE		loc_61794
		LDA		#1
loc_61794:
		STA		_tptr1
		LDA		_pad_last
		AND		#$80
		STA		_tptr1+1
		BEQ		loc_617CB
		LDA		_lvl2_jetpack_fuel_tanks
		BEQ		loc_617CB
		DEC		_lvl2_jetpack_fuel_count
		BNE		loc_617AB
		DEC		_lvl2_jetpack_fuel_tanks
loc_617AB:
		LDA		_lvl2_jetpack_fuel_tanks
		CMP		#2
		BCS		loc_617BB
		LDA		#$13
		JSR		_hud_msg_set_request
loc_617BB:
		JSR		_lvl2_obj_jet_update
		LDA		_obj_y_velosity_target._player
		CMP		#$FD
		BEQ		loc_617E1
		DEC		_obj_y_velosity_target._player
		JMP		loc_617E1
loc_617CB:
		LDA		_obj_flags._misc
		BEQ		loc_617D7
		LDA		#0
		STA		_obj_flags._misc
		LDA		#SND_SOUND0E
		STA		_snd_idx
loc_617D7:
		LDA		_obj_y_velosity_target._player
		CMP		#8
		BEQ		loc_617E1
		INC		_obj_y_velosity_target._player
loc_617E1:
		CLC
		LDA		#$94
		ADC		_tptr1
		STA		_obj_spr_idx._player
		LDA		#$C
		STA		_obj_var4._player
		LDA		_obj_y_velocity._player
		BPL		loc_61805
loc_617F2:
		LDA		#8
		STA		_obj_var4._player
		LDA		_obj_y._player
		CMP		#$3C
		BCS		loc_61802
		LDA		#0
		STA		_obj_y_velocity._player
loc_61802:
		JMP		loc_6193B
loc_61805:
		BNE		loc_6180B
		LDA		_tptr1+1
		BNE		loc_617F2
loc_6180B:
		LDA		_obj_vcoll._player
		ORA		_obj_hcoll._player
		AND		#1
		BNE		loc_61816
		JMP		loc_6193B
loc_61816:
		LDA		_obj_y._player
		AND		#$F0
		STA		_obj_y._player
		LDA		#0
		STA		_obj_y_velocity._player
		STA		_obj_y_velosity_target._player
		LDA		#2
		STA		_player_sit_jump_flag
		LDA		#8
		STA		_obj_sit_state._player
		LDA		#SND_RBLAND1
		STA		_snd_idx
		JMP		loc_6186B
loc_61834:
		LDA		#0
		STA		_obj_flags._misc
		LDA		_player_sit_jump_flag
		AND		#2
		BNE		loc_61847
		LDA		_pad_last
		CMP		#4
		BEQ		loc_6185B
		JMP		loc_61888
loc_61847:
		LDA		_pad_last
		AND		#4
		BNE		loc_6185B
		DEC		_obj_sit_state._player
		BNE		loc_6186B
		LDA		_player_sit_jump_flag
		AND		#$FD
		STA		_player_sit_jump_flag
		JMP		loc_6193B
loc_6185B:
		LDA		_player_sit_jump_flag
		ORA		#2
		STA		_player_sit_jump_flag
		LDA		_obj_sit_state._player
		CMP		#9
		BCS		loc_6186B
		INC		_obj_sit_state._player
loc_6186B:
		LDA		_player_anim_phase_offset
		CMP		#$A
		PHP
		STY		_tptr1
		LDA		_obj_sit_state._player
		LSR
		LSR
		PLP
		ADC		#$A2
		STA		_obj_spr_idx._player
		LDA		#0
		STA		_player_diagonal_fire_delta
		LDA		#$1A
		STA		_player_shot_pos_delta
		JMP		loc_6193B
loc_61888:
		LDA		_obj_hcoll._player
		ORA		_obj_vcoll._player
		AND		#1
		BNE		loc_618A2
		LDA		#8
		STA		_obj_y_velosity_target._player
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
		STA		_player_shot_pos_delta
		LDA		#0
		STA		_player_diagonal_fire_delta
		LDA		_pad_last
		AND		#$8F
		BNE		loc_618C8
		STA		_player_hor_move_anim_timer
		CLC
		LDA		#$97
		LDY		_player_anim_phase_offset
		CPY		#$A
		BNE		loc_618C3
		CLC
		ADC		#4
loc_618C3:
		STA		_obj_spr_idx._player
		JMP		loc_6193B
loc_618C8:
		LDA		_pad_last
		AND		#$83
		BNE		loc_61912
		LDA		_pad_last
		AND		#8
		BEQ		loc_618F0
		LDA		#$FC
		STA		_player_diagonal_fire_delta
		LDA		_player_anim_phase_offset
		CMP		#$A
		LDA		#0
		ADC		#$A6
		STA		_obj_spr_idx._player
		LDA		#$FD
		STA		_player_shot_pos_delta
		LDA		_pad_last
		AND		#$FC
		STA		_pad_last
		JMP		loc_6193B
loc_618F0:
		LDA		_pad_last
		AND		#4
		BEQ		loc_61912
		LDA		#4
		STA		_player_diagonal_fire_delta
		LDA		_player_anim_phase_offset
		CMP		#$A
		LDA		#0
		ADC		#$A8
		STA		_obj_spr_idx._player
		LDA		#$20
		STA		_player_shot_pos_delta
		LDA		_pad_last
		AND		#$FC
		STA		_pad_last
		JMP		loc_6193B
loc_61912:
		LDA		_player_hor_move_anim_timer
		AND		#$1F
		LSR
		LSR
		LSR
		CLC
		ADC		#$96
		LDY		_player_anim_phase_offset
		CPY		#$A
		BNE		loc_61925
		CLC
		ADC		#4
loc_61925:
		STA		_obj_spr_idx._player
		LDA		_pad_last
		AND		#$80
		BEQ		loc_6193B
		LDA		_player_sit_jump_flag
		ORA		#1
		STA		_player_sit_jump_flag
		LDA		#$F0
		STA		_player_y_meta16_tile_tmp
		LDA		#0
		STA		_player_edge_fall_flag
loc_6193B:
		JSR		_non_movement_controls
		JMP		.additional_special_lvl_ctrl_test

; =============== S U B	R O U T	I N E =======================================
_player_explosion_anim:
		LDA		_obj_y._player
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
		ADC		_obj_x_lo._player
		STA		_obj_x_lo,X
		LDA		_obj_x_hi._player
		ADC		#0
		STA		_obj_x_hi,X
		CLC
		LDA		_obj_y._player
		ADC		_player_explode_timer_tmp
		SBC		#4
		STA		_obj_y,X
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
		STA		_player_spr_out_reload
		CLC
		LDA		#0
		SBC		_player_spr_out_reload
		STA		_player_spr_out_reload
		CPX		#6
		BCS		loc_6171B
		LDA		_rand_seed
		AND		#$10
		BEQ		loc_6171B
		JSR		_enemy_proj_explode1_start
loc_6171B:
		LDA		_rand_seed+1
		AND		#3
		CLC
		ADC		#$10
		STA		_obj_sit_state,X
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

; =============== S U B	R O U T	I N E =======================================
_player_anim_fire_control:
		LDA		_player_anim_phase_offset
		CMP		#$A
		BEQ		.pafc_skip								; A phase here means we just start firing
		LDA		#5
		STA		_player_anim_phase_offset
		LDA		#$10
		STA		_player_shoot_state_timer				; reload shoot timer.
		JSR		_player_fire_control
.pafc_skip:
		RTS

; =============== S U B	R O U T	I N E =======================================
_player_shot_anim_phase_change:
		LDY		#0
		LDA		_player_anim_phase_offset				; when anim phase $A - hand raised upper, set when shoot start
		CMP		#$A										; change it to 5 after 16 frames (hand raised lower)
		BNE		loc_61326								; geves two-frame of arm firing
		LDY		#5
loc_61326:
		STY		_player_anim_phase_offset
		LDA		#$10
		STA		_player_shoot_state_timer				; reload timer
		RTS
		
; =============== S U B	R O U T	I N E =======================================
_lvl2_obj_jet_update:
		LDA		_obj_flags._misc
		BNE		loc_61A29
		LDA		#SND_JETPACK
		STA		_snd_idx
		LDA		#0
		STA		_obj_pal_idx._misc
		STA		_obj_y_velocity._misc
		STA		_obj_x_velocity._misc
		STA		_obj_x_velosity_target._misc
		STA		_obj_y_velosity_target._misc
		LDA		#$FF
		STA		_obj_hndl_idx._misc
		INC		_obj_flags._misc
loc_61A29:
		CLC
		LDA		_obj_y._player
		ADC		#$1C
		STA		_obj_y._misc
		LDY		#2
		LDA		_obj_flags._player
		AND		#$40
		BNE		loc_61A3A
		LDY		#$16
loc_61A3A:
		TYA
		CLC
		ADC		_obj_x_lo._player
		STA		_obj_x_lo._misc
		LDA		_obj_x_hi._player
		ADC		#0
		STA		_obj_x_hi._misc
		INC		_obj_sit_state._misc
		LDA		_obj_sit_state._misc
		AND		#$F
		LSR
		LSR
		CLC
		ADC		#$9E
		STA		_obj_spr_idx._misc
		RTS

; =============== S U B	R O U T	I N E =======================================
_lvl1_player_transporter_pos_adjust:
		LDA		_lvl_idx
		CMP		#1
		BEQ		loc_61A5D
		RTS
loc_61A5D:
		LDA		#0
		STA		_tptr1
		LDA		_obj_vcoll._player
		LSR
		ROL		_tptr1
		LDA		_obj_hcoll._player
		LSR
		ROL		_tptr1
		LDA		_tptr1
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
		JSR		_player_x_pos_dec
locret_61A8F:
		RTS
loc_61A90:
		LDA		_obj_hcoll._player
		AND		#4
		BNE		locret_61A8F
		LDA		_obj_x_hi._player
		CMP		_lvl_stop_sctoll_room_idx
		BCC		loc_61AB8
		LDA		_obj_x_lo._player
		CMP		#$74
		BCC		loc_61AB8
		LDA		_player_wnd_scroll_counter
		CMP		#$E6
		BCS		locret_61A8F
		INC		_player_wnd_scroll_counter
		JMP		_player_x_pos_inc
loc_61AB8:
		JSR		_player_x_pos_inc
		LDA		_player_wnd_scroll_counter
		CMP		#$74
		BEQ		loc_61ACE
		INC		_player_wnd_scroll_counter
		RTS
loc_61ACE:
		JMP		_scroll_inc

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
		LDA		_obj_x_hi,X
		STA		_ptr0+1
		LDA		_obj_x_lo,X
		LSR		_ptr0+1
		ROR
		LSR		_ptr0+1
		ROR
		LSR		_ptr0+1
		ROR
		STA		_player_tile_x
		CLC
		STA		_tptr1
		ADC		_lvl_collision_data0_ptr
		STA		_ptr0
		LDA		_ptr0+1
		STA		_tptr1+1
		ADC		_lvl_collision_data0_ptr+1
		STA		_ptr0+1
		CLC
		LDA		_tptr1
		ADC		_lvl_collision_data1_ptr
		STA		_ptr1
		LDA		_tptr1+1
		ADC		_lvl_collision_data1_ptr+1
		STA		_ptr1+1
		CLC
		LDA		_tptr1
		ADC		_lvl_collision_data2_ptr
		STA		_tptr2
		LDA		_tptr1+1
		ADC		_lvl_collision_data2_ptr+1
		STA		_tptr2+1
		LDA		_obj_y,X
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
		STX		_tptr1+1
		TAX
		LDA		_bitmask,X
		STA		_tptr1
		LDX		_tptr1+1
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
		AND		_tptr1
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
		AND		_tptr1
		BEQ		loc_61B8D
		LDA		_obj_vcoll,X
		ORA		#1
		STA		_obj_vcoll,X
loc_61B8D:
		CPX		#0
		BNE		loc_61B9D
		LDA		_obj_y._player
		CMP		#$BC
		BCC		loc_61B9D
		LDA		#$80
		STA		_tptr1
		BNE		loc_61B9F
loc_61B9D:
		LSR		_tptr1
loc_61B9F:
		LDY		#0
		LDA		(_tptr2),Y
		AND		_tptr1
		BEQ		loc_61BAD
		LDA		_obj_hcoll,X
		ORA		#2
		STA		_obj_hcoll,X
loc_61BAD:
		LDY		#3
		LDA		(_tptr2),Y
		AND		_tptr1
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
		LDA		#SND_ROCKET1
		STA		_snd_idx
		LDX		#$F
		LDA		#$1E
		STA		_player_fire_delay_reload
loc_61C2E:
		STX		_tptr1
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
		STA		_obj_sit_state,X
		LDA		byte_65A
		STA		_obj_hndl_idx,X
		JSR		_player_bullet_insert
		LDA		_player_fire_delay_reload
		STA		_player_fire_delay_timer
		LDA		byte_65A
		CMP		#3
		BCS		locret_61C58
loc_61C54:
		LDA		#SND_SHOTGUN
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
		LDA		_obj_x_lo._pbullet+1
		STA		_obj_x_lo._pbullet
		STA		_obj_x_lo._pbullet+2
		LDA		_obj_x_hi._pbullet+1
		STA		_obj_x_hi._pbullet
		STA		_obj_x_hi._pbullet+2
		LDA		_obj_y._pbullet+1
		STA		_obj_y._pbullet
		STA		_obj_y._pbullet+2
		LDA		_obj_x_velosity_target._pbullet+1
		STA		_obj_x_velosity_target._pbullet
		STA		_obj_x_velosity_target._pbullet+2
		STA		_obj_x_velocity._pbullet
		STA		_obj_x_velocity._pbullet+2
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
		STA		_obj_y_velosity_target._pbullet
		STA		_obj_y_velocity._pbullet
		LDA		byte_61CF1+1,Y
		STA		_obj_y_velosity_target._pbullet+1
		STA		_obj_y_velocity._pbullet+1
		LDA		byte_61CF1+2,Y
		STA		_obj_y_velosity_target._pbullet+2
		STA		_obj_y_velocity._pbullet+2
		LDA		_obj_x_velocity._pbullet+1
		AND		byte_61CE8,Y
		STA		_obj_x_velocity._pbullet
		STA		_obj_x_velosity_target._pbullet
		LDA		_obj_x_velocity._pbullet+1
		AND		byte_61CE8+1,Y
		STA		_obj_x_velocity._pbullet+1
		STA		_obj_x_velosity_target._pbullet+1
		LDA		_obj_x_velocity._pbullet+1
		AND		byte_61CE8+2,Y
		STA		_obj_x_velocity._pbullet+2
		STA		_obj_x_velosity_target._pbullet+2
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
		STA		_obj_energy._enemy,X
		LDA		#8
		STA		_obj_x_velosity_target._enemy,X
		STA		_obj_x_velocity._enemy,X
		LDA		#0
		STA		_obj_energy._enemy,X
loc_61D11:
		DEX
		BPL		loc_61CFC
		DEC		byte_663
		BNE		loc_61D1B
		DEC		_weapon1_idx
loc_61D1B:
		LDA		#SND_ROCKET2
		STA		_snd_idx
		LDA		#$32
		STA		_pal_blink_timer
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
		STA		_player_anim_phase_offset
		LDA		#4
		STA		_player_shoot_state_timer
		LDA		#0
		STA		_obj_pal_idx,X
		CLC
		LDA		_obj_y._player
		ADC		_player_shot_pos_delta
		STA		_obj_y,X
		LDA		_player_diagonal_fire_delta
		STA		_obj_y_velosity_target,X
		STA		_obj_y_velocity,X
		LDA		_tptr1
		STA		_obj_spr_idx,X
		LDA		_obj_flags._player
		AND		#$40
		PHP
		ORA		#1
		STA		_obj_flags,X
		PLP
		BEQ		loc_61D90
		LDA		#4
		STA		_obj_x_velosity_target,X
		STA		_obj_x_velocity,X
		CLC
		LDA		_obj_x_lo._player
		ADC		#$1E
		STA		_obj_x_lo,X
		LDA		_obj_x_hi._player
		ADC		#0
		STA		_obj_x_hi,X
		RTS
loc_61D90:
		LDA		#$FC
		STA		_obj_x_velosity_target,X
		STA		_obj_x_velocity,X
		SEC
		LDA		_obj_x_lo._player
		SBC		#6
		STA		_obj_x_lo,X
		LDA		_obj_x_hi._player
		SBC		#0
		STA		_obj_x_hi,X
		RTS

; =============== S U B	R O U T	I N E =======================================
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

; =============== S U B	R O U T	I N E =======================================
_player_bullets_animate:
		LDA		_player_fire_delay_timer				; bullet animation delay
		BEQ		.pbullet_do_work
		DEC		_player_fire_delay_timer
.pbullet_do_work:
		LDX		#4
.pbullet_do_loop:
		LDA		_obj_flags,X							; check if bullet object active
		BEQ		.skip_to_next_pbullet
		LDA		_obj_y,X								; if positive, check for max Y position
		CMP		#$D8									; on the ground
		BCS		.pbullet_turn_explosion					; if positive, switch the explosion animation
		CMP		#$3C
		BCS		.pbullet_regular_animate				; between 3C and D8 pos Y do regular animate
		JSR		_player_bullet_remove					; remove silently if reach upper side of the screen
		LDA		#0
		STA		_obj_flags,X							; disable object, skip to the next one
		BEQ		.skip_to_next_pbullet
.pbullet_regular_animate:
		LDY		_obj_hndl_idx,X
		BMI		.skip_to_next_pbullet					; do regular anim handlers
		BEQ		.skip_to_next_pbullet					; 0,1,2 gun bullets, has no special animations
		DEY												; FF - handler for jet of the missile
		BEQ		.skip_to_next_pbullet
		DEY
		BEQ		.skip_to_next_pbullet
		DEY
		BEQ		.pbullet_regular_missile_animate		; 3 - regular missile animation
		DEY
		BEQ		.pbullet_homing_missile_animate			; 4 - homing missile animation
		DEY
		BEQ		.skip_to_next_pbullet					; 5,6 has no special handlers (and no bullets?)
		DEY
		BEQ		.skip_to_next_pbullet
.pbullet_handler7_animate:
		JSR		_explosion_spr_anim						; 7 is an explosion animation
.skip_to_next_pbullet:
		INX
		CPX		#7
		BNE		.pbullet_do_loop
		RTS
; --------------------------------
.pbullet_turn_explosion:
		LDA		_obj_hndl_idx,X							; check if active object is not explosion already
		CMP		#7
		BEQ		.pbullet_regular_animate				; if positive, just jump to animation
		JSR		_player_bullet_remove					; otherwise remove bullet, start explode
		JSR		_enemy_proj_explode0_start				; skip to the next one
		JMP		.pbullet_handler7_animate
; --------------------------------
.pbullet_regular_missile_animate:
		LDY		 _obj_sit_state._pbullet
		JMP		.pbullet_common_missile_animate
; --------------------------------
.pbullet_homing_missile_animate:
		LDY		 _obj_sit_state._pbullet
		BEQ		loc_61E29
		LDA		_obj_flags,Y
		BEQ		loc_61E29
		LDA		_obj_var5,Y
		BNE		loc_61E38
loc_61E29:
		LDY		#3
loc_61E2B:
		LDA		_obj_flags,Y
		BEQ		loc_61E35
		LDA		_obj_var5,Y
		BNE		loc_61E38
loc_61E35:
		DEY
		BNE		loc_61E2B
loc_61E38:
		STY		 _obj_sit_state._pbullet
		LDA		#4
		STA		_tptr1
		LDA		#3
		STA		_tptr1+1
		SEC
		LDA		_obj_x_lo,Y
		SBC		_obj_x_lo._pbullet
		LDA		_obj_x_hi,Y
		SBC		_obj_x_hi._pbullet
		BCS		loc_61E54
		LDA		#$FC
		STA		_tptr1
loc_61E54:
		CLC
		LDA		_obj_y,Y
		ADC		#$1E
		SEC
		SBC		_obj_y._pbullet
		PHP
		LDA		_obj_y,Y
		AND		#$F8
		STA		_tmpE4
		LDA		_obj_y._pbullet
		AND		#$F8
		CMP		_tmpE4
		BNE		loc_61E74
		PLP
		LDA		#0
		STA		_tptr1+1
		BEQ		loc_61E7B
loc_61E74:
		PLP
		BCS		loc_61E7B
		LDA		#$FD
		STA		_tptr1+1
loc_61E7B:
		LDA		_tptr1
		STA		_obj_x_velosity_target._pbullet
		LDA		_tptr1+1
		STA		_obj_y_velosity_target._pbullet
		LDY		#0
		LDA		_obj_x_velocity._pbullet
		BPL		loc_61E8D
		LDY		#9
loc_61E8D:
		STY		_tptr1
		CLC
		LDA		_obj_y_velocity._pbullet
		ADC		#4
		CLC
		ADC		_tptr1
		TAY
; --------------------------------
.pbullet_common_missile_animate:
		LDA		_pmissile_spr_list,Y
		STA		_obj_spr_idx._pbullet
		LDA		_pmissile_flags_list,Y
		STA		_obj_flags._pbullet
		LDA		_pmissile_x_delta_list,Y
		BMI		loc_61EB6
		CLC
		ADC		_obj_x_lo._pbullet
		STA		_obj_x_lo._pbullet+1
		LDA		_obj_x_hi._pbullet
		ADC		#0
		STA		_obj_x_hi._pbullet+1
		JMP		loc_61ECA
loc_61EB6:
		EOR		#$FF
		CLC
		ADC		#1
		STA		_tptr1
		SEC
		LDA		_obj_x_lo._pbullet
		SBC		_tptr1
		STA		_obj_x_lo._pbullet+1
		LDA		_obj_x_hi._pbullet
		SBC		#0
		STA		_obj_x_hi._pbullet+1
loc_61ECA:
		CLC
		LDA		_pmissile_y_delta_list,Y
		ADC		_obj_y._pbullet
		STA		_obj_y._pbullet+1
		DEC		_obj_var7._pbullet
		LDA		_obj_var7._pbullet
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
		LDA		_obj_x_velocity._pbullet
		STA		_obj_x_velocity._pbullet+1
		LDA		#3
		STA		_obj_pal_idx._pbullet
		JMP		.skip_to_next_pbullet
		
_pmissile_spr_list:
		.BYTE	 $B, $C, $D, $E, $F, $E, $D, $C, $B, $B, $C, $D, $E, $F, $E, $D, $C, $B
_pmissile_flags_list:
		.BYTE	  1,  1,  1,  1,  1,$81,$81,$81,$81,  1,$41,$41,$41,$C1,$C1,$C1,$81,$81
_pmissile_x_delta_list:
		.BYTE	  0,$FE,$FC,$FB,$FA,$FB,$FC,$FE,  0,  0,  2,  4,  5,  6,  5,  4,  2,  0
_pmissile_y_delta_list:
		.BYTE	  6,  5,  4,  2,  0,$FE,$FC,$FB,$FA,  6,  5,  4,  2,  0,$FE,$FC,$FB,$FA
byte_61F40:
		.BYTE	  2,  4,  6, $B, $D, $F

; =============== S U B	R O U T	I N E =======================================
; general level objects manager, read lists sequentialy from first to last,
; never return back
;
_lvl_obj_manager:
		LDA		_special_boss_flood_object_flag			; test if we already reach the last level object
		BNE		.do_wnd_level_enemy_flood
		LDY		#0
		LDA		(_lvl_obj_list_ptr),Y					; +0 read appear X tile idx
		CMP		#$FF									; test for the last level object record
		BEQ		.set_end_level_flag						; jump if true
		CMP		_player_tile_x							; player X tile to appera test, do not add
		BEQ		_obj_add								; until player is reach this tile
		RTS
.special_transition_objects:
		CMP		#$FF
		BNE		.pal_object								; there are two types of such objects
		LDY		#2										; object type FF start here (bank object)
		LDA		(_lvl_obj_list_ptr),Y
		STA		_lvl_phase_bg_chr_banks_list			; change the bg chr data bank number for long levels
		STA		_lvl_phase_bg_chr_banks_list+1
		STA		_lvl_phase_bg_chr_banks_list+2
		JMP		.advance_obj_ptr_and_break
.pal_object:											; object type FE start here
		LDY		#2
		LDA		(_lvl_obj_list_ptr),Y
		ASL
		ASL
		ASL
		ASL
		TAY
		LDX		#0
loc_61F75:
		LDA		_lvl2_transition_pal_lib,Y
		STA		_ingame_bg_pal_buf,X
		INY
		INX
		CPX		#$10
		BCC		loc_61F75
		JMP		.advance_obj_ptr_and_break
.set_end_level_flag:
		INC		_special_boss_flood_object_flag			; when end of the enemy list, set flag
.do_wnd_level_enemy_flood:
		LDA		_special_boss_flood_object_count		; and wait for all enemies are disappear
		BNE		loc_61F8D
		RTS												; this means the end of the level
loc_61F8D:
		LDX		#2										; only two enemies at a time on the screen
loc_61F8F:
		LDA		_obj_flags,X							; wait for free slot
		BEQ		loc_61F97
		DEX
		BNE		loc_61F8F
		RTS
loc_61F97:
		LDA		_lvl_idx							; enemy type is selecting by the level number
		ASL												; from the table, one of two randomly selected at a time
		STA		_tmpE4
		LDA		_rand_seed
		AND		#1
		ORA		_tmpE4
		TAY
		LDA		_end_of_level_enemy_table,Y
		STA		_tmpE4
		LDY		_lvl_idx
		LDA		#$70									; base position of the screen to appear
		STA		_tptr1+1								; y
		LDA		#$50
		STA		_tmpE6									; low x
		LDA		_end_of_level_enemy_appear_pos_x,Y		; hi x is different for every level
		STA		_tmpE7
		LDY		#$A
		LDA		_rand_seed								; select appearance side delta
		BMI		loc_61FBF								; $0A - left side, $88 - right side
		LDY		#$88
loc_61FBF:
		STY		_tptr1
		DEC		_special_boss_flood_object_count		; decrease enemies counter, do insert
		JMP		.insert_object

; =============== S U B	R O U T	I N E =======================================
; common enemy insert routine, used my the enemy lvl manager or by the
; lvl init routine to fill all enemies that should be already on the screen
;
_obj_add:
		LDY		#1
		LDA		(_lvl_obj_list_ptr),Y					; test for a special object for transition
		BMI		.special_transition_objects				; between different parts of the level
		LDA		_obj_x_lo._player						; x position is relative to player's pos
		STA		_tmpE6
		LDA		_obj_x_hi._player
		STA		_tmpE7
		LDY		#3
		LDA		(_lvl_obj_list_ptr),Y					; +3 read x delta
		STA		_tptr1
		DEY
		LDA		(_lvl_obj_list_ptr),Y					; +2 read y
		STA		_tptr1+1
		DEY
		LDA		(_lvl_obj_list_ptr),Y					; +1 read object handler
		STA		_tmpE4
		CMP		#$21
		BCS		loc_61FF5
		LDX		#3										; object handlers from $00 to $20 are enemies
loc_61FEB:
		LDA		_obj_flags,X							; look for free slot to add
		BEQ		.insert_object
		DEX
		BNE		loc_61FEB
		JMP		.advance_obj_ptr_and_break				; if no empty slots, skip enemy and do next one
loc_61FF5:
		CMP		#$28
		BCC		loc_62001								; object 28 is a special spr0hit object
		LDX		#$C
		STA		_obj_hndl_idx,X
		TAY
		JMP		.obj_pos_x_calc
loc_62001:
		CMP		#$24
		BNE		loc_62009
		LDX		#$A										; $24 is misc object slot
		BNE		.insert_object
loc_62009:
		LDX		#$B										; $21, $22, $23, $25, $26 and $27 are bonuses
.insert_object:
		LDA		_tmpE4									; write object handle
		STA		_obj_hndl_idx,X
		TAY
		LDA		_tptr1+1
		CLC
		ADC		#$40
		STA		_obj_y,X								; write object Y pos
		LDA		_obj_pal_lib,Y							; palette and sprite indexes are in the global object table
		STA		_obj_pal_idx,X
		LDA		_obj_spr_lib,Y
		STA		_obj_spr_idx,X
		LDA		byte_63BD0,Y
		STA		_obj_energy,X
		LDA		_obj_init_flags_lib,Y					; as well as sprites
		STA		_obj_flags,X
.obj_pos_x_calc:
		LDA		_lvl_idx								; level object appears on the right side in all levels
		CMP		#3										; except fourth, wich is reversed, then from the right side
		BNE		.obj_gen_pos_x_calc
		SEC
		LDA		_tmpE6
		SBC		#$84									; adjust appear position
		STA		_obj_x_lo,X
		LDA		_tmpE7
		SBC		#0
		STA		_obj_x_hi,X
		JMP		.obj_pos_delta_aplly
.obj_gen_pos_x_calc:
		CLC
		LDA		_tmpE6
		ADC		#$8E									; adjust appear position
		STA		_obj_x_lo,X
		LDA		_tmpE7
		ADC		#0
		STA		_obj_x_hi,X
.obj_pos_delta_aplly:
		LDA		_tptr1									; extra delta shift for the object
		BMI		loc_62062								; can be positive or negative
		ASL
		CLC
		ADC		_obj_x_lo,X
		STA		_obj_x_lo,X
		LDA		_obj_x_hi,X
		ADC		#0
		STA		_obj_x_hi,X
		JMP		loc_62077
loc_62062:
		EOR		#$FF
		CLC
		ADC		#1
		ASL
		STA		_tptr1
		SEC
		LDA		_obj_x_lo,X
		SBC		_tptr1
		STA		_obj_x_lo,X
		LDA		_obj_x_hi,X
		SBC		#0
		STA		_obj_x_hi,X
loc_62077:
		LDA		#0										; init misc vars
		STA		_obj_x_velosity_target,X
		STA		_obj_x_velocity,X
		STA		_obj_y_velosity_target,X
		STA		_obj_y_velocity,X
		STA		_obj_sit_state,X
		STA		_obj_var7,X
		LDA		#1
		STA		_obj_var5,X
.advance_obj_ptr_and_break:
		ADD16	_lvl_obj_list_ptr, 4
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
		STA		_tmp_ptr
		LDA		_obj_enemy_ai_handlers_lib+1,Y
		STA		_tmp_ptr+1
		JMP		(_tmp_ptr)

_obj_next_enemy_ai:
		DEX
		BNE		_obj_enemy_ai_loop
		JMP		_obj_tank_weapon_handler

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
		.WORD 0	; _obj_ehndl0C_unused
		.WORD _obj_ehndl0D_regular_gunman2
		.WORD _obj_ehndl0E_regular_gunman3
		.WORD _obj_ehndl0F_grenader2
		.WORD _obj_ehndl10_hanging_gunman0
		.WORD 0 ; _obj_ehndl13_hanging_gunman1
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
		STY		_tptr1+1
		INY
		INY
		INY
		INY
		STY		_tmpE4
		INY
		STY		_tmpE5
		LDA		_obj_energy,X
		BNE		loc_62122
		LDA		_tmpE6
		STA		_obj_spr_idx,X
		JMP		loc_62228
loc_62122:
		LDA		_obj_hcoll,X
		AND		#1
		BNE		loc_62130
		LDA		_obj_y_velosity_target,X
		BEQ		loc_6217E
		JMP		loc_62228
loc_62130:
		LDA		_obj_y,X
		CMP		#$A0
		BCS		loc_62139
		JMP		loc_62228
loc_62139:
		LDA		#0
		STA		_obj_y_velosity_target,X
		STA		_obj_y_velocity,X
		LDA		_obj_y,X
		AND		#$F0
		STA		_obj_y,X
		LDA		_obj_x_velocity,X
		BNE		loc_62168
		DEC		_obj_var7,X
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
		STA		_obj_var7,X
		JMP		loc_6217E
loc_62168:
		INC		_obj_sit_state,X
		LDA		_obj_sit_state,X
		AND		#$1F
		LSR
		LSR
		LSR
		CLC
		ADC		_tptr1+1
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
		STA		_obj_x_velosity_target,X
		STA		_obj_x_velocity,X
		JMP		loc_62228
loc_62195:
		LDA		_obj_var7,X
		BPL		loc_621C4
		LDA		_rand_seed,X
		AND		#$1F
		ADC		#$1E
		STA		_obj_var7,X
		JSR		_enemy_projectile_collision_test_ex
		LDA		#$FF
		STA		_tptr1
		LDA		_obj_flags,X
		AND		#$BF
		BCC		loc_621B8
		ORA		#$40
		PHA
		LDA		#1
		STA		_tptr1
		PLA
loc_621B8:
		STA		_obj_flags,X
		LDA		_tptr1
		STA		_obj_x_velosity_target,X
		STA		_obj_x_velocity,X
		JMP		loc_62228
loc_621C4:
		DEC		_obj_var7,X
		LDA		_obj_var7,X
		CMP		#$A
		BCS		loc_62228
		JSR		_enemy_projectile_collision_test_ex
		TYA
		BNE		loc_62228
		LDA		_obj_flags,X
		AND		#2
		BEQ		loc_621F9
		LDA		_tptr1
		CMP		#$16
		BCS		loc_62228
		CMP		#$12
		BCC		loc_62228
		JSR		_enemy_projectile_collision_test
		LDA		#3
		STA		_body_target_idx
		LDA		#BACK_DAMAGE_VALUE
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
		LDA		#SND_SOUND0F
		STA		_snd_idx
		LDA		_tmpE4
		STA		_obj_spr_idx,X
		LDA		#$F
loc_6221E:
		STA		_obj_var7,X
		LDA		#0
		STA		_obj_x_velosity_target,X
		STA		_obj_x_velocity,X
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
		STY		_tptr1
		INY
		STY		_tptr1+1
		INY
		STY		_tmpE4
		INY
		STY		_tmpE5
		LDA		_obj_energy,X
		BNE		loc_62252
		LDA		#$2E
		STA		_obj_spr_idx,X
		JMP		loc_6235C
loc_62252:
		DEC		_obj_var7,X
		BMI		loc_6225A
		JMP		loc_6235C
loc_6225A:
		LDA		#5
		STA		_obj_var7,X
		LDA		_obj_sit_state,X
		BNE		loc_62280
		LDA		_obj_spr_idx,X
		CMP		_tptr1
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
		STA		_obj_sit_state,X
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
		STA		_obj_sit_state,X
		JMP		loc_6235C
loc_622A0:
		JSR		_enemy_projectile_collision_test_ex
		BCC		loc_622DC
		LDA		_lvl_idx
		CMP		#2
		BCS		loc_622DC
		LDA		#0
		STA		_tptr1
		LDY		#3
loc_622B1:
		LDA		_obj_flags,Y
		BEQ		loc_622BF
		LDA		_obj_y,Y
		CMP		#$8C
		BCC		loc_622BF
		INC		_tptr1
loc_622BF:
		DEY
		BNE		loc_622B1
		LDA		_tptr1
		BNE		loc_622DC
		LDA		#6
		STA		_obj_y_velosity_target,X
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
		LDA		#SND_SOUND0F
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
		STA		_tptr1
		LDA		#$FC
		STA		_tptr1+1
		LDA		_obj_flags,X
		AND		#$40
		BEQ		loc_6233A
		LDA		#2
		STA		_tptr1
		LDA		#4
		STA		_tptr1+1
loc_6233A:
		LDA		_tptr1
		STA		_obj_x_velosity_target,Y
		LDA		_tptr1+1
		STA		_obj_x_velocity,Y
		LDA		#$FD
		STA		_obj_y_velocity,Y
		LDA		#3
		STA		_obj_pal_idx,Y
		LDA		#3
		STA		_obj_var4,Y
		LDA		_tmpE5
		STA		_obj_spr_idx,X
		LDA		#$1E
		STA		_obj_var7,X
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
;_obj_ehndl0C_unused:
;		LDA		#$67
;		LDY		#$6D
;		BNE		_obj_ehndl_hiding_barrel

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
		STY		_tptr1
		INY
		STY		_tptr1+1
		INY
		STY		_tmpE4
		INY
		STY		_tmpE5
		LDA		_obj_energy,X
		BNE		loc_62394
		LDA		_tmpE6
		STA		_obj_spr_idx,X
loc_62391:
		JMP		loc_62427
loc_62394:
		DEC		_obj_var7,X
		BPL		loc_62391
		LDA		#4
		STA		_obj_var7,X
		LDA		_obj_sit_state,X
		BNE		loc_623D0
		LDA		#3
		STA		_obj_var8,X
		LDA		_obj_spr_idx,X
		CMP		_tptr1
		BEQ		loc_623BE
		DEC		_obj_spr_idx,X
		LDA		_obj_spr_idx,X
		CMP		_tptr1
		BNE		loc_62427
		LDA		#0
		STA		_obj_var5,X
		JMP		loc_62427
loc_623BE:
		LDA		#0
		STA		_obj_var5,X
		LDA		_rand_seed,X
		CMP		#6
		BCS		loc_62427
		LDA		#1
		STA		_obj_sit_state,X
		BNE		loc_62427
loc_623D0:
		LDA		#1
		STA		_obj_var5,X
		LDA		_obj_spr_idx,X
		CMP		_tmpE4
		BCS		loc_623DF
		INC		_obj_spr_idx,X
		BNE		loc_62427
loc_623DF:
		LDA		_tmpE4
		STA		_obj_spr_idx,X
		LDA		_obj_var8,X
		BNE		loc_623EF
loc_623E8:
		LDA		#0
		STA		_obj_sit_state,X
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
		LDA		#SND_SOUND10
		STA		_snd_idx
		LDA		_tmpE5
		STA		_obj_spr_idx,X
		DEC		_obj_var8,X
		LDA		#2
		STA		_pal_blink_timer
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
		STY		_tptr1
		INY
		STY		_tptr1+1
		INY
		INY
		INY
		STY		_tmpE4
		LDA		_obj_energy,X
		BNE		loc_62448
		JMP		loc_624EB
loc_62448:
		LDA		_obj_hcoll,X
		AND		#3
		BNE		loc_62463
		LDA		_obj_x_velocity,X
		EOR		#$FF
		CLC
		ADC		#1
		STA		_obj_x_velosity_target,X
		STA		_obj_x_velocity,X
		LDA		_obj_flags,X
		EOR		#$40
		STA		_obj_flags,X
		JMP		loc_624EB
loc_62463:
		LDA		_obj_x_velocity,X
		BEQ		loc_62477
		INC		_obj_sit_state,X
		LDA		_obj_sit_state,X
		AND		#$1F
		LSR
		LSR
		LSR
		CLC
		ADC		_tptr1
		STA		_obj_spr_idx,X
loc_62477:
		LDA		_rand_seed,X
		CMP		#$A
		BCS		loc_624EB
		JSR		_enemy_projectile_collision_test_ex
		LDA		#$FF
		STA		_tptr1
		LDA		_obj_flags,X
		AND		#$BF
		BCC		loc_62492
		ORA		#$40
		PHA
		LDA		#1
		STA		_tptr1
		PLA
loc_62492:
		STA		_obj_flags,X
		LDA		_tptr1
		STA		_obj_x_velosity_target,X
		STA		_obj_x_velocity,X
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
		LDA		#SND_SOUND10
		STA		_snd_idx
		LDA		_tmpE4
		STA		_obj_spr_idx,X
		LDA		#0
		STA		_obj_x_velosity_target,X
		STA		_obj_x_velocity,X
		JMP		_obj_next_enemy_ai
loc_624C8:
		LDA		_obj_spr_idx,X
		CMP		_tptr1+1
		BNE		loc_624EB
		LDY		#5
		LDA		#$19
		STA		_enemy_bullet_y_pos_global_arg
		LDA		#2
		STA		_enemy_bullet_body_target_select_arg
		LDA		#5
		JSR		_enemy_bullet_insert
		BCS		loc_624EB
		LDA		#SND_SOUND10
		STA		_snd_idx
		LDA		#0
		STA		_obj_x_velosity_target,X
		STA		_obj_x_velocity,X
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
		STA		_tptr1+1
		STY		_tmpE4
		INY
		STY		_tmpE5
		LDA		_obj_energy,X
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
		LDA		_obj_y,X
		EOR		_tmpE6
		STA		_obj_y,X
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
		STA		_obj_y_velosity_target,X
		LDA		_tptr1+1
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
		LDA		#SND_SOUND0F
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
		STY		_tptr1
		INY
		INY
		INY
		INY
		STY		_tptr1+1
		INY
		STY		_tmpE4
		LDA		_obj_energy,X
		BNE		loc_62581
		JMP		loc_6262C
loc_62581:
		LDA		_obj_hcoll,X
		AND		#3
		BNE		loc_6259C
		LDA		_obj_x_velocity,X
		EOR		#$FF
		CLC
		ADC		#1
		STA		_obj_x_velosity_target,X
		STA		_obj_x_velocity,X
		LDA		_obj_flags,X
		EOR		#$40
		STA		_obj_flags,X
		JMP		loc_6262C
loc_6259C:
		LDA		_obj_x_velocity,X
		BEQ		loc_625B0
		INC		_obj_sit_state,X
		LDA		_obj_sit_state,X
		AND		#$1F
		LSR
		LSR
		LSR
		CLC
		ADC		_tptr1
		STA		_obj_spr_idx,X
loc_625B0:
		LDA		_rand_seed,X
		CMP		#$A
		BCS		loc_6262C
		JSR		_enemy_projectile_collision_test_ex
		LDA		#$FF
		STA		_tptr1
		LDA		_obj_flags,X
		AND		#$BF
		BCC		loc_625CB
		ORA		#$40
		PHA
		LDA		#1
		STA		_tptr1
		PLA
loc_625CB:
		STA		_obj_flags,X
		LDA		_tptr1
		STA		_obj_x_velosity_target,X
		STA		_obj_x_velocity,X
		LDA		_rand_seed,X
		CMP		#5
		BCS		loc_6262C
		LDA		_obj_spr_idx,X
		CMP		_tptr1+1
		BEQ		loc_625E6
		LDA		_tptr1+1
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
		STA		_obj_x_velosity_target,Y
		LDA		#$FC
		STA		_obj_x_velocity,Y
		LDA		#$FD
		STA		_obj_y_velocity,Y
		LDA		#2
		STA		_obj_pal_idx,Y
		LDA		#3
		STA		_obj_var4,Y
		LDA		_tmpE4
		STA		_obj_spr_idx,X
		LDA		_obj_flags,X
		AND		#$40
		BEQ		loc_62625
		LDA		#2
		STA		_obj_x_velosity_target,Y
		LDA		#4
		STA		_obj_x_velocity,Y
loc_62625:
		LDA		#0
		STA		_obj_x_velosity_target,X
		STA		_obj_x_velocity,X
loc_6262C:
		JMP		_obj_next_enemy_ai

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl18_rifle_man_lvl2:
		LDA		_obj_energy,X
		BNE		loc_6263B
		LDA		#$B1
		STA		_obj_spr_idx,X
		JMP		loc_626B8
loc_6263B:
		LDA		_obj_hcoll,X
		AND		#3
		BNE		loc_62656
		LDA		_obj_x_velocity,X
		EOR		#$FF
		CLC
		ADC		#1
		STA		_obj_x_velosity_target,X
		STA		_obj_x_velocity,X
		LDA		_obj_flags,X
		EOR		#$40
		STA		_obj_flags,X
		JMP		loc_626B8
loc_62656:
		LDA		_obj_x_velocity,X
		BEQ		loc_6266A
		INC		_obj_sit_state,X
		LDA		_obj_sit_state,X
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
		STA		_tptr1
		LDA		_obj_flags,X
		AND		#$BF
		BCC		loc_62685
		ORA		#$40
		PHA
		LDA		#1
		STA		_tptr1
		PLA
loc_62685:
		STA		_obj_flags,X
		LDA		_tptr1
		STA		_obj_x_velosity_target,X
		STA		_obj_x_velocity,X
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
		LDA		#SND_SOUND10
		STA		_snd_idx
		LDA		#$B6
		STA		_obj_spr_idx,X
		LDA		#0
		STA		_obj_x_velosity_target,X
		STA		_obj_x_velocity,X
loc_626B8:
		JMP		_obj_next_enemy_ai

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl0A_ninja:
		LDA		#8
		CMP		_lvl_spr_chr_bank_up
		BEQ		loc_626CC
		STA		_lvl_spr_chr_bank_up					; special case, change bg chr bank and music
		STA		_lvl_spr_chr_bank_down
		LDA		#SND_MLEVEL4
		STA		_snd_idx
		JSR		_far_apu_mus_load_unsafe
loc_626CC:
		LDA		_obj_energy,X
		BNE		loc_6271B
		LDA		_player_scroll_stop_flag
		BEQ		loc_626D9
		LDA		#2
		STA		_player_scroll_stop_flag
loc_626D9:
		LDA		#$61
		STA		_obj_spr_idx,X
		LDA		_obj_flags,X
		AND		#8
		BEQ		loc_626F8
		LDA		_lvl_spr_chr_bank_up_backup
		STA		_lvl_spr_chr_bank_up
		LDA		_lvl_spr_chr_bank_down_backup
		STA		_lvl_spr_chr_bank_down
		LDA		#0										; hacky-hacky!
		STA		_obj_flags,X							; if ninja is dead, restore music for level 2
		LDA		#SND_MLEVEL1							; if set ninjas in other levels, will cause problems
		STA		_snd_idx
		JSR		_far_apu_mus_load_unsafe
loc_626F8:
		JSR		_enemy_bullet_get_free_slot_idx
		BCC		loc_62700
		JMP		loc_628D6
loc_62700:
		LDA		_obj_x_lo,X
		STA		_obj_x_lo,Y
		LDA		_obj_x_hi,X
		STA		_obj_x_hi,Y
		LDA		_obj_y,X
		STA		_obj_y,Y
		TXA
		PHA
		TYA
		TAX
		JSR		_enemy_proj_explode0_start
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
		STA		_player_scroll_stop_flag
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
		STA		_obj_var7,X
		TYA
		STA		_obj_x_velosity_target,X
		STA		_obj_x_velocity,X
		LDA		#$FB
		STA		_obj_y_velocity,X
		LDA		#5
		STA		_obj_y_velosity_target,X
		LDA		#0
		STA		_obj_sit_state,X
		LDA		#5
		STA		_obj_var4,X
		LDA		_obj_flags,X
		ORA		#4
		STA		_obj_flags,X
		JMP		loc_628D6
loc_6277A:
		LDA		_obj_y,X
		CMP		#$B4
		BCS		loc_62742
		INC		_obj_sit_state,X
		LDA		_obj_sit_state,X
		LSR
		LSR
		LSR
		AND		#3
		LDY		_obj_var7,X
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
		LDA		_obj_y_velocity,X
		BPL		loc_627B9
		JMP		loc_628D6
loc_627B9:
		LDA		#0
		STA		_obj_y_velosity_target,X
		STA		_obj_y_velocity,X
		LDA		_obj_y,X
		AND		#$F0
		STA		_obj_y,X
		LDA		_obj_flags,X
		AND		#4
		BEQ		loc_627DA
		LDA		_obj_flags,X
		AND		#$3B
		LDY		_obj_var7,X
		BEQ		loc_627D8
		ORA		#$40
loc_627D8:
		STA		_obj_flags,X
loc_627DA:
		LDA		_lvl_idx
		CMP		#4
		BNE		loc_627FB
		LDA		_obj_energy,X
		CMP		#$50
		BCS		loc_627FB
		LDA		#$63
		STA		_obj_energy,X
		LDA		#0
		STA		_obj_pal_idx,X
		LDA		#3
		STA		_tmpE5
		LDA		#$FD
		STA		_tmpE6
		JMP		loc_6274A
loc_627FB:
		LDA		_obj_x_velocity,X
		BNE		loc_62841
		INC		_obj_sit_state,X
		LDA		_obj_sit_state,X
		LSR
		LSR
		AND		#7
		CMP		#7
		BCC		loc_62822
		LDA		#SND_RBLAND2
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
		LDA		_lvl4_cur_pass_char_idx
		CMP		#$C
		BCC		loc_6283E
		LDA		_obj_y,X
		CMP		#$64
		BCC		loc_6286E
loc_6283E:
		JMP		loc_62742
loc_62841:
		LDA		_obj_flags,X
		AND		#8
		BNE		loc_6286E
		INC		_obj_sit_state,X
		LDA		_obj_sit_state,X
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
		LDA		_obj_var7,X
		BPL		loc_6288F
		LDA		_rand_seed,X
		AND		#$1F
		ADC		#$1E
		STA		_obj_var7,X
loc_6286E:
		JSR		_obj_ninja_self_destruct_animate
		LDA		#$FF
		STA		_tptr1
		LDA		_obj_flags,X
		AND		#$BF
		BCC		loc_62883
		ORA		#$40
		PHA
		LDA		#1
		STA		_tptr1
		PLA
loc_62883:
		STA		_obj_flags,X
		LDA		_tptr1
		STA		_obj_x_velosity_target,X
		STA		_obj_x_velocity,X
		JMP		loc_628D6
loc_6288F:
		DEC		_obj_var7,X
		JSR		_enemy_projectile_collision_test_ex
		TYA
		BNE		loc_6289E
		LDA		_obj_flags,X
		AND		#2
		BNE		loc_628A8
loc_6289E:
		LDA		_obj_var7,X
		CMP		#$A
		BCS		loc_628D6
		JMP		loc_6282B
loc_628A8:
		LDA		_lvl4_cur_pass_char_idx
		CMP		#$C
		BCS		loc_6289E
		JSR		_enemy_projectile_collision_test
		LDA		#1
		STA		_body_target_idx
		LDA		#NINJA_DAMAGE_VALUE
		JSR		_player_damage_apply
		LDA		#$5F
		STA		_obj_spr_idx,X
		LDA		#$A
		STA		_obj_var7,X
		LDA		#0
		STA		_obj_x_velosity_target,X
		STA		_obj_x_velocity,X
		STA		_obj_sit_state,X
		LDA		#SND_SOUND11
		STA		_snd_idx
		JMP		loc_628D6
loc_628D6:
		JMP		_obj_next_enemy_ai
loc_628D9:
		LDA		#0
		STA		_obj_x_velocity,X
		STA		_obj_x_velosity_target,X
		STA		_obj_y_velosity_target,X
		STA		_obj_y_velocity,X
		LDA		_obj_pal_idx._enemy+2
		AND		_obj_pal_idx._enemy+1
		BMI		loc_628EF
		JMP		_obj_next_enemy_ai
loc_628EF:
		LDA		_obj_spr_idx._enemy+2
		CMP		#$60
		BEQ		loc_62908
		DEC		_obj_var7._enemy+2
		BPL		loc_62903
		INC		_obj_spr_idx._enemy+2
		INC		_obj_spr_idx._enemy+1
		LDA		#$14
		STA		_obj_var7._enemy+2
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
		STA		_obj_y._ebullet
		STA		_obj_y._ebullet+1
		LDA		#$88
		STA		_obj_x_lo._ebullet
		LDA		#1
		STA		_obj_x_hi._ebullet
		LDA		#$91
		STA		_obj_x_lo._ebullet+1
		LDA		#1
		STA		_obj_x_hi._ebullet+1
		LDX		#7
		JSR		_enemy_proj_explode0_start
		LDX		#8
		JSR		_enemy_proj_explode0_start
loc_62939:
		LDA		_ingame_sync_counter
		BNE		locret_62946
;		LDA		#5
;		STA		_lvl_idx								; and here too
		LDA		#1
		STA		_game_over_flag
locret_62946:
		RTS
byte_62947:
		.BYTE	$5D,$5E,$5E,$5F,$60,$60,$5D,$5D

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl_ninja_animate:
		LDA		_lvl_idx
		CMP		#4
		BNE		loc_629C5
		LDA		_obj_x_hi,X
		BNE		loc_62983
		LDA		_obj_x_velocity,X
		BPL		loc_6298D
		LDA		_obj_x_lo,X
		CMP		#8
		BCS		loc_6298D
loc_62963:
		LDA		_obj_x_velocity,X
		EOR		#$FF
		CLC
		ADC		#1
		STA		_obj_x_velocity,X
		LDA		_obj_x_velosity_target,X
		EOR		#$FF
		CLC
		ADC		#1
		STA		_obj_x_velosity_target,X
		LDA		_obj_var7,X
		EOR		#$FF
		CLC
		ADC		#1
		STA		_obj_var7,X
		RTS
loc_62983:
		LDA		_obj_x_velocity,X
		BMI		loc_6298D
		LDA		_obj_x_lo,X
		CMP		#$F2
		BCS		loc_62963
loc_6298D:
		LDA		_lvl4_cur_pass_char_idx
		CMP		#$C
		BCC		locret_629BC
		LDA		_obj_hcoll,X
		BEQ		locret_629BC
		LDA		_obj_y,X
		BMI		locret_629BC
		LDA		_obj_x_hi,X
		BEQ		locret_629BC
		LDA		_obj_flags,X
		AND		#4
		BNE		locret_629BC
		LDA		_obj_x_lo,X
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
		LDA		_obj_x_velosity_target,X
		BPL		locret_629EC
		LDA		#1
		STA		_obj_x_velosity_target,X
		LDA		#$40
		STA		_obj_var7,X
		ORA		_obj_flags,X
		STA		_obj_flags,X
		RTS
loc_629E4:
		LDA		_obj_x_velosity_target,X
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
		JSR		_enemy_proj_explode0_start
		PLA
		TAX
		LDA		#$A
		JSR		_hud_msg_set_request
locret_62A0D:
		RTS

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl15_walker_droid:
		LDA		#$B
		STA		_lvl_spr_chr_bank_up
		STA		_lvl_spr_chr_bank_down
		LDA		_obj_energy,X
		BNE		.droid_active
		JMP		.droid_explode
.droid_active:
		JSR		_enemy_projectile_collision_test_ex
		LDA		_obj_flags,X
		AND		#$BF
		BCS		loc_62A27
		ORA		#$40
loc_62A27:
		STA		_obj_flags,X
		CLC
		LDA		_obj_sit_state,X
		ADC		_obj_x_velocity,X
		AND		#$1F
		STA		_obj_sit_state,X
		LSR
		LSR
		LSR
		TAY
		CLC
		ADC		#$88
		STA		_obj_spr_idx,X
		LDA		byte_62B44,Y
		STA		_obj_y,X
		LDA		_obj_x_lo,X
		BPL		loc_62A4A
		LDY		#1
		BNE		loc_62A6B
loc_62A4A:
		LDA		_obj_x_lo,X
		CMP		#$50
		BCC		loc_62A54
		LDY		#$FF
		BNE		loc_62A6B
loc_62A54:
		DEC		_obj_var7,X
		BPL		loc_62A71
		LDA		_rand_seed,X
		AND		#$1F
		ORA		#$20
		STA		_obj_var7,X
		LDY		#1
		LDA		_obj_x_velosity_target,X
		BMI		loc_62A6B
		LDY		#$FF
loc_62A6B:
		TYA
		STA		_obj_x_velosity_target,X
		STA		_obj_x_velocity,X
loc_62A71:
		LDA		#0
		STA		_obj_x_velosity_target._enemy+1
		STA		_obj_x_velocity._enemy+1
		STA		_obj_y_velosity_target._enemy+1
		STA		_obj_y_velocity._enemy+1
		STA		_obj_y_velosity_target,X
		STA		_obj_y_velocity,X
		LDA		#3
		STA		_obj_pal_idx._enemy+1
		LDA		_obj_flags,X
		STA		_obj_flags._enemy+1
		AND		#$40
		BNE		loc_62AA0
		CLC
		LDA		_obj_x_lo,X
		ADC		#$28
		STA		_obj_x_lo._enemy+1
		LDA		_obj_x_hi,X
		ADC		#0
		STA		_obj_x_hi._enemy+1
		JMP		loc_62AA8
loc_62AA0:
		LDA		_obj_x_lo,X
		STA		_obj_x_lo._enemy+1
		LDA		_obj_x_hi,X
		STA		_obj_x_hi._enemy+1
loc_62AA8:
		CLC
		LDA		_obj_y,X
		ADC		#$10
		STA		_obj_y._enemy+1
		LDA		_obj_spr_idx._enemy+1
		CMP		#$8C
		BCC		loc_62ACF
		DEC		_obj_sit_state._enemy
		BPL		loc_62AD3
		LDA		#3
		STA		_obj_sit_state._enemy
		LDA		_obj_var7._enemy
		BMI		loc_62AD3
		DEC		_obj_var7._enemy
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
		LDA		_obj_y._player
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
		STA		_obj_var7._enemy
		LDA		#SND_SHOTGUN
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
.droid_explode:
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
		ADC		_obj_x_lo,X
		STA		_obj_x_lo,Y
		LDA		_obj_x_hi,X
		ADC		#0
		STA		_obj_x_hi,Y
		LDA		_rand_seed+1
		AND		#$1F
		ADC		#8
		ADC		_obj_y,X
		STA		_obj_y,Y
		TXA
		PHA
		TYA
		TAX
		JSR		_enemy_proj_explode0_start
		PLA
		TAX
loc_62B7C:
		LDA		#0
		STA		_obj_x_velosity_target,X
		STA		_obj_x_velocity,X
		STA		_obj_y_velosity_target,X
		STA		_obj_y_velocity,X
		LDA		_obj_pal_idx,X
		EOR		#3
		STA		_obj_pal_idx,X
		INC		_obj_pal_idx._enemy
		BNE		locret_62B9C
		LDA		#1
		STA		_game_over_flag
;		LDA		#4										; no need anymore, autoincrementing
;		STA		_lvl_idx
locret_62B9C:
		RTS

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl16_ground_timed_bomb:
		INC		_obj_sit_state,X
		LDA		_obj_sit_state,X
		CMP		#$64
		BCC		loc_62C0B

; =============== S U B	R O U T	I N E =======================================
_obj_ehndl_ground_bomb:
		LDY		#7
loc_62BA9:
		LDA		_obj_x_lo,X
		STA		_obj_x_lo,Y
		LDA		_obj_x_hi,X
		STA		_obj_x_hi,Y
		SEC
		LDA		_obj_y,X
		SBC		#4
		STA		_obj_y,Y
		LDA		#4
		STA		_obj_spr_idx,Y
		LDA		#$FC
		STA		_obj_y_velocity,Y
		LDA		#2
		STA		_obj_y_velosity_target,Y
		LDA		#0
		STA		_obj_hndl_idx,Y
		STA		_obj_x_velosity_target,Y
		STA		_obj_x_velocity,Y
		STA		_obj_pal_idx,Y
		LDA		#1
		STA		_obj_flags,Y
		INY
		CPY		#$A
		BCC		loc_62BA9
		LDA		#$F8
		STA		_obj_x_velocity._ebullet
		LDA		#$FC
		STA		_obj_x_velosity_target._ebullet
		LDA		#8
		STA		_obj_x_velocity._ebullet+2
		LDA		#4
		STA		_obj_x_velosity_target._ebullet+2
		LDA		#$F8
		STA		_obj_y_velocity._ebullet+1
		LDA		#6
		STA		_obj_y_velosity_target._ebullet+1
		LDA		#0
		STA		_obj_flags,X
		LDA		#SND_EXPLDLA
		STA		_snd_idx
		LDA		#5
		STA		_pal_blink_timer
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
_enemy_proj_explode0_start:
		LDA		#SND_EXPLDSM
		STA		_snd_idx
		JMP		_enemy_projectile_explode_start

; =============== S U B	R O U T	I N E =======================================
_enemy_proj_explode1_start:
		LDA		#SND_EXPLDMD
		STA		_snd_idx

; =============== S U B	R O U T	I N E =======================================
_enemy_projectile_explode_start:
		LDA		#$13
		STA		_obj_sit_state,X
		LDA		#7
		STA		_obj_hndl_idx,X
		STA		_obj_spr_idx,X
		LDA		#1
		STA		_obj_flags,X
		RTS

; =============== S U B	R O U T	I N E =======================================
_explosion_spr_anim:
		LDA		#0
		STA		_obj_y_velosity_target,X
		STA		_obj_x_velosity_target,X
		STA		_obj_x_velocity,X
		STA		_obj_y_velocity,X
		STA		_obj_pal_idx,X
		DEC		_obj_sit_state,X
		BEQ		loc_62C62
		LDA		_obj_sit_state,X
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
		STA		_tptr1
		STY		_tptr1+1
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
		LDA		_obj_y,X
		ADC		_enemy_bullet_y_pos_global_arg
		STA		_obj_y,Y
		LDA		_tptr1
		STA		_obj_y_velosity_target,Y
		STA		_obj_y_velocity,Y
		LDA		_tptr1+1
		STA		_obj_spr_idx,Y
		LDA		_obj_flags,X
		AND		#$40
		BEQ		loc_62CC1
		LDA		#5
		STA		_obj_x_velosity_target,Y
		STA		_obj_x_velocity,Y
		CLC
		LDA		_obj_x_lo,X
		ADC		#$18
		STA		_obj_x_lo,Y
		LDA		_obj_x_hi,X
		ADC		#0
		STA		_obj_x_hi,Y
		CLC
		RTS
loc_62CC1:
		LDA		#$FB
		STA		_obj_x_velosity_target,Y
		STA		_obj_x_velocity,Y
		LDA		_obj_x_lo,X
		STA		_obj_x_lo,Y
		LDA		_obj_x_hi,X
		STA		_obj_x_hi,Y
		CLC
		RTS

; =============== S U B	R O U T	I N E =======================================
; enemy bullets explosion simple animation manager. check if bullet is out of
; range. bullets going up should just disappear, bullets goind down shold explode
; above the ground
;
_enemy_bullet_explosion_anim:
		LDX		#9
loc_62CD7:
		LDA		_obj_y,X								; all the same explosion animate for enemy bullets
		CMP		#$D4									; but this time they don't check for the active flag
		BCS		.ebullet_explode_start					; at all, so the animation and explosions do a lot
		CMP		#$3C									; of trash calculations
		BCS		.ebullet_explode_anim
		LDA		#0										; remove object out of the screen (Y is less than 3C)
		STA		_obj_flags,X
		BEQ		.ebullet_skip_to_next
.ebullet_explode_anim:
		LDA		_obj_hndl_idx,X
		CMP		#7
		BNE		.ebullet_skip_to_next					; here check for only the explosion to animate
		TXA
		JSR		_explosion_spr_anim						; do animate
.ebullet_skip_to_next:
		DEX
		CPX		#6
		BNE		loc_62CD7
		RTS
.ebullet_explode_start:
		LDA		_obj_hndl_idx,X							; for objects lower than Y=D4 check if it is already explode
		CMP		#7
		BEQ		.ebullet_explode_anim					; and do the regular anim roll
		JSR		_enemy_proj_explode0_start				; or start the anim 
		JMP		.ebullet_explode_anim

; =============== S U B	R O U T	I N E =======================================
_obj_tank_weapon_handler:
		LDA		_lvl_idx
		CMP		#2
		BEQ		loc_62D0A
locret_62D09:
		RTS
loc_62D0A:
		LDA		_lvl2_tank_weapon_disable_flag
		BNE		locret_62D09
		LDA		_obj_x_hi._player
		CMP		#$E
		BCS		.do_lvl2_tank_boss_fire
		JMP		.do_lvl2_shot_delay
.do_lvl2_tank_boss_fire:
		LDA		_special_boss_flood_object_flag
		BEQ		.do_lvl2_tank_boss_shot
		LDY		#3
loc_62D1F:
		LDA		_obj_flags,Y							; shot from tank while there are three people in it
		BEQ		loc_62D33								; when all pilots are shot, cease tank fire
		LDA		_obj_hndl_idx,Y
		CMP		#4
		BEQ		.do_lvl2_tank_boss_shot
		CMP		#6
		BEQ		.do_lvl2_tank_boss_shot
		CMP		#9
		BEQ		.do_lvl2_tank_boss_shot
loc_62D33:
		DEY
		BPL		loc_62D1F
		LDA		#1
		STA		_lvl2_tank_weapon_disable_flag			; force tank disable flag
.do_lvl2_tank_boss_shot:
		LDA		#$C
		STA		_lvl_spr_chr_bank_down				; select the fire shot bank
		JSR		_enemy_bullet_get_free_slot_idx
		BCC		loc_62D45								; get shot if any bullets slots available
		RTS
loc_62D45:
		LDA		_rand_seed								; randomly select bullet type to shoot
		AND		#$F
		TAX
		LDA		_lvl2_tank_boss_fire_x_lo_list,X
		STA		_obj_x_lo,Y
		LDA		#$F
		STA		_obj_x_hi,Y
		LDA		_lvl2_tank_boss_fire_y_list,X
		STA		_obj_y,Y
		LDA		_lvl2_tank_boss_fire_pal_list,X
		STA		_obj_pal_idx,Y
		LDA		_lvl2_tank_boss_fire_vel_list,X
		STA		_obj_x_velosity_target,Y
		STA		_obj_x_velocity,Y
		LDA		_lvl2_tank_boss_fire_var2_list,X
		STA		_obj_y_velosity_target,Y
		LDA		_lvl2_tank_boss_fire_var1_list,X
		STA		_obj_y_velocity,Y
		LDA		#2
		STA		_obj_hndl_idx,Y
		LDA		#1
		STA		_obj_flags,Y
		LDA		_lvl2_tank_boss_fire_spr_list,X
		STA		_obj_spr_idx,Y
		CMP		#5
		BEQ		loc_62D8F								; additional blink only if main cannon shot (!=5)
		LDA		#2
		STA		_pal_blink_timer
loc_62D8F:
		LDA		#SND_EXPLDLA
		STA		_snd_idx
		RTS
.do_lvl2_shot_delay:
		LDA		_lvl2_tank_shot_delay					; do delay between shots
		BEQ		.do_lvl2_tank_shot_counter				; when delay is over, jump to bullet counters
		DEC		_lvl2_tank_shot_delay
		BEQ		.do_lvl2_shot_sound						; the moment of timer is out, do shot sound
		JMP		locret_62E28
.do_lvl2_shot_sound:
		SEC												; shot fired, wait for bullet to appear
		LDA		#$E
		SBC		_obj_x_hi._player
		STA		_lvl2_tank_shot_timer_hi				; the closer to the end of level, the faster appears
		LDA		#3
		STA		_pal_blink_timer							; do sound and blink
		LDA		#SND_EXPLDLA
		STA		_snd_idx
		RTS
.do_lvl2_tank_shot_counter:
		LDA		_lvl2_tank_shot_timer_lo				; set small delay (not initialized causes random small delays)
		ORA		_lvl2_tank_shot_timer_hi				; total telay between shot is fired and appears
		BEQ		.do_lvl2_tank_shot						; on the screen 14 times number of rooms
		DEC		_lvl2_tank_shot_timer_lo
		BNE		locret_62E28
		LDA		#$14
		STA		_lvl2_tank_shot_timer_lo
		DEC		_lvl2_tank_shot_timer_hi
		BNE		locret_62E28
.do_lvl2_tank_shot:
		JSR		_enemy_bullet_get_free_slot_idx			; get free enemy bullet shot
		BCS		locret_62E28
		LDX		_lvl2_tank_weapon_position_switch		; get the cur bullet position (0-3)
		SEC												; calc x position of the projectile
		LDA		_obj_x_lo._player
		SBC		_player_wnd_scroll_counter
		STA		_tptr1
		LDA		_obj_x_hi._player
		SBC		#0
		STA		_tptr1+1
		CLC
		LDA		_tptr1
		ADC		#$FA
		STA		_obj_x_lo,Y								; adjust x pos
		LDA		_tptr1+1
		ADC		#0
		STA		_obj_x_hi,Y
		LDA		_lvl2_tank_y_pos_list,X					; initialize the object
		STA		_obj_y,Y
		LDA		#0
		STA		_obj_y_velosity_target,Y
		STA		_obj_y_velocity,Y
		LDA		#$FA
		STA		_obj_x_velosity_target,Y
		STA		_obj_x_velocity,Y						; velicity
		LDA		#$AB
		STA		_obj_spr_idx,Y							; sprite
		LDA		#3
		STA		_obj_hndl_idx,Y							; handler, etc.
		STA		_obj_pal_idx,Y
		LDA		#1
		STA		_obj_flags,Y							; enable object
		INC		_lvl2_tank_weapon_position_switch		; advanse to the next position
		LDA		_lvl2_tank_weapon_position_switch
		AND		#3
		STA		_lvl2_tank_weapon_position_switch		; wrap around 3
		LDA		_rand_seed								; get the random delay to appear next time
		ORA		#$40
		STA		_lvl2_tank_shot_delay					; but no less than 40 iterations
locret_62E28:
		RTS

_lvl2_tank_y_pos_list:
		.BYTE	$50,$78,$A0,$C8
_lvl2_tank_boss_fire_x_lo_list:
		.BYTE	$4E,$AE,$AD,$B0,$B7,$E3,$E7,$EB,$4E,$AE,$AD,$B0,$B7,$E3,$E7,$EB
_lvl2_tank_boss_fire_y_list:
		.BYTE	$A0,$A6,$A0,$9C,$99,$9C,$9A,$98,$A0,$A6,$A0,$9C,$99,$9C,$9A,$98
_lvl2_tank_boss_fire_spr_list:
		.BYTE	$AB,  5,  5,  5,  5,  5,  5,  5,$AB,  5,  5,  5,  5,  5,  5,  5
_lvl2_tank_boss_fire_vel_list:
		.BYTE	$FA,$FA,$FA,$FB,$FE,$FE,$FE,$FE,$FA,$FA,$FA,$FB,$FE,$FE,$FE,$FE
_lvl2_tank_boss_fire_var1_list:
		.BYTE	  0,  0,$FE,$FD,$FA,$FC,$FC,$FC,  0,  0,$FE,$FD,$FA,$FC,$FC,$FC
_lvl2_tank_boss_fire_var2_list:
		.BYTE	  0,  0,$FE,$FD,$FA,$FC,$FC,$FC,  0,  0,  6,  6,  6,  6,  6,  6
_lvl2_tank_boss_fire_pal_list:
		.BYTE	  3,  0,  0,  0,  0,  0,  0,  0,  3,  0,  0,  0,  0,  0,  0,  0

; =============== S U B	R O U T	I N E =======================================
_enemy_projectile_collision_test_ex:
		LDY		#0
		LDA		_obj_y._player
		AND		#$80
		STA		_tptr1
		LDA		_obj_y,X
		AND		#$80
		CMP		_tptr1
		BEQ		loc_62EAE
		INY
loc_62EAE:
		SEC
		LDA		_obj_x_lo._player
		SBC		_obj_x_lo,X
		STA		_tptr1
		LDA		_obj_x_hi._player
		SBC		_obj_x_hi,X
		PHP
		BCS		loc_62ECF
		SEC
		LDA		_obj_x_lo,X
		SBC		_obj_x_lo._player
		STA		_tptr1
		LDA		_obj_x_hi,X
		SBC		_obj_x_hi._player
		LSR
		LDA		_tptr1
		ROR
		LSR
		LSR
		PLP
		RTS
loc_62ECF:
		LSR
		LDA		_tptr1
		ROR
		LSR
		LSR
		PLP
		RTS

; =============== S U B	R O U T	I N E =======================================
_obj_ninja_self_destruct_animate:
		LDA		_lvl4_cur_pass_char_idx
		CMP		#$C
		BCC		_enemy_projectile_collision_test_ex
		LDY		#0
		SEC
		LDA		#$60
		SBC		_obj_x_lo,X
		STA		_tptr1
		LDA		#1
		SBC		_obj_x_hi,X
		PHP
		BCS		loc_62F01
		SEC
		LDA		_obj_x_lo,X
		SBC		#$60
		STA		_tptr1
		LDA		_obj_x_hi,X
		SBC		#1
		LSR
		LDA		_tptr1
		ROR
		LSR
		LSR
		PLP
		RTS
loc_62F01:
		LSR
		LDA		_tptr1
		ROR
		LSR
		LSR
		PLP
		RTS

; =============== S U B	R O U T	I N E =======================================
_obj_hidden_portal_test_and_bonus_animate:
		LDA		#0
		STA		_lvl_hidden_portal_detected
		LDY		_lvl_state_idx
		SEC
		LDA		_obj_x_lo._player
		SBC		_lvl_hidden_passage_x_lo_list,Y
		TAX
		LDA		_obj_x_hi._player
		SBC		_lvl_hidden_passage_x_hi_list,Y
		BNE		loc_62F2E
		CPX		#8
		BCS		loc_62F2E
		LDA		_obj_y._player
		AND		#$F0
		CMP		_lvl_hidden_passage_y_list,Y
		BNE		loc_62F2E
		INC		_lvl_hidden_portal_detected
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
		STA		_tptr1
		ASL
		ORA		_tptr1
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
		STA		_tmp_ptr
		LDA		_collision_tests_lib+1,Y
		STA		_tmp_ptr+1
		JMP		(_tmp_ptr)

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
		LDA		_pad_last
		AND		#4
		BEQ		locret_62F9A
		LDA		_obj_flags._bonus
		BEQ		locret_62F9A
		LDX		#0
		LDY		#$B
		JSR		_collision_test
		BEQ		locret_62F9A
		LDA		_obj_hndl_idx._bonus
		CMP		#$27
		BEQ		.get_missile_power_item
		CMP		#$26
		BEQ		.get_power_restore_item
		CMP		#$25
		BEQ		.get_fire_power_item
		CMP		#$22
		BEQ		.get_smart_bomb_item
		CMP		#$21
		BEQ		.get_jetpack_fuel_item
		BNE		.get_repair_tank_items
; --------------------------------
.get_fire_power_item:
		LDX		#0
		LDA		_weapon0_idx
		CMP		#2
		BCS		.weapon_ammo_full_reload
		INC		_weapon0_idx
		LDX		_weapon0_idx
		LDA		_upgrade_bullets_list,X
		STA		_weapon0_ammo,X
		LDA		#6
		BNE		.regular_item_snd
; --------------------------------
.get_missile_power_item:
		LDX		#3
		LDA		_weapon1_idx
		CMP		#2
		BCS		.weapon_ammo_full_reload
		INC		_weapon1_idx
		LDX		_weapon1_idx
		LDA		_upgrade_missile_list,X
		STA		_weapon1_ammo,X
		LDA		#7
.regular_item_snd:
		JSR		_hud_msg_set_request_ex
		STA		_obj_flags._bonus						; after exit ser_request_ex A=0 by default
		LDA		_apu_enable
		AND		#APU_MUS_ENABLE
		BNE		.short_pickup1
		LDA		#SND_PCKUP1A
		.BYTE	$2C
.short_pickup1:
		LDA		#SND_PICKUP1
		STA		_snd_idx
		RTS
; --------------------------------
.weapon_ammo_full_reload:								; when maximum fore power are obtained
		LDY		#2										; any power item works as ammo reload
loc_62FFB:												; never occurs in the game!
		LDA		_upgrade_bullets_list,X
		STA		_weapon0_ammo,X
		INX
		DEY
		BNE		loc_62FFB
		LDA		#0										; the only case you need to force A=0
		BEQ		.regular_item_snd
; --------------------------------
.get_repair_tank_items:
		INC		_repair_tanks
		LDA		#8
		BNE		.powerup_item_snd
; --------------------------------
.get_smart_bomb_item:
		INC		_lvl2_smart_bomb_flag
		LDA		#$FF
		STA		_smart_bomb_timer
		LDA		#$D
		BNE		.regular_item_snd
; --------------------------------
.get_jetpack_fuel_item:
		LDA		#3
		STA		_lvl2_jetpack_fuel_tanks
		LDA		#$14
		BNE		.regular_item_snd
; --------------------------------
.get_power_restore_item:
		LDX		#3
loc_63009:
		CLC
		LDA		_percents,X
		ADC		#DEF_PERCENT_POWERUP					; tweakable
		STA		_percents,X
		DEX
		BPL		loc_63009
		JSR		_player_overall_armor_calc				; since we don't recalc it every time
		LDA		#4
.powerup_item_snd:
		JSR		_hud_msg_set_request_ex
		STA		_obj_flags._bonus						; again after exit ser_request_ex A=0 by default
		LDA		_apu_enable
		AND		#APU_MUS_ENABLE
		BNE		.short_pickup2
		LDA		#SND_PCKUP2A
		.BYTE	$2C
.short_pickup2:
		LDA		#SND_PICKUP2
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
		STX		_tptr1
		LDA		_obj_hndl_idx,X
		STA		_tptr1+1
		CMP		#3
		BCS		loc_6309C
		LDA		#0
		STA		_obj_flags,X
		BEQ		loc_6309F
loc_6309C:
		JSR		_enemy_proj_explode0_start
loc_6309F:
		LDA		_obj_hndl_idx,Y
		CMP		#$A
		BEQ		loc_630AA
		CMP		#$15
		BNE		loc_630B0
loc_630AA:
		LDA		_tptr1+1
		ORA		#8
		STA		_tptr1+1
loc_630B0:
		LDX		_tptr1+1
		SEC
		LDA		_obj_energy,Y
		SBC		byte_63C22,X
		STA		_obj_energy,Y
		STY		_last_enemy_hit
		LDX		_tptr1
		BCC		loc_630C6
		AND		#$FF
		BNE		loc_6310F
loc_630C6:
		LDA		#1
		STA		_tptr1
		LDA		#$F9
		STA		_tptr1+1
		LDA		_obj_y,Y
		BPL		loc_630D3
loc_630D3:
		LDA		#4
		STA		_tptr1
		LDA		#$FD
		STA		_tptr1+1
		LDA		_tptr1
		STA		_obj_y_velosity_target,Y
		LDA		_tptr1+1
		STA		_obj_y_velocity,Y
		LDA		#0
		STA		_obj_energy,Y
		LDA		#5
		STA		_obj_var4,Y
		LDA		#6
		STA		_tptr1
		LDA		_obj_x_velocity,X
		BPL		loc_630FB
		LDA		#$FA
		STA		_tptr1
loc_630FB:
		LDA		_tptr1
		STA		_obj_x_velocity,Y
		STA		_obj_x_velosity_target,Y
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
		JSR		_enemy_proj_explode1_start
loc_63144:
		LDY		_enemy_bullet_body_target_select_arg
		LDA		_body_targets_list,Y
		STA		_body_target_idx
		LDA		_projectiles_damage_list,Y
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
		STA		_tptr1									; tmp damage value
		STX		_tptr1+1								; backup X register
		LDA		#3
		STA		_player_damage_blink_timer				; pal blink
		LDA		_hud_msg_idx							
		CMP		#9
		BEQ		_exit									; do not damage if already dead
		LDA		_obj_shuffle_mode						; also check for shuffle mode as well
		AND		#2										; to be sure we still exploding lol
		BNE		_exit									
		LDX		_body_target_idx						; get the target body part index
		LDA		_percents,X								; check for critical value
		CMP		#UPPER_DAMAGE_ALERT
		BCS		.write_damage							; if not so critical, just decrease
		CMP		#LOWER_DAMAGE_ALERT
		BCC		loc_63188
		TXA												; current power betwen 10 and 7 percents
		CLC												; display warning damage message for particular part
		ADC		#$F
		JSR		_hud_msg_set_request
loc_63188:
		LDA		_rand_seed								; if damage already less than critical, select
loc_6318A:												; pickup other random body part to damage
		AND		#3										; with non-zero percentage
		TAX
		LDA		_percents,X
		BNE		.write_damage
		INX
		TXA
		JMP		loc_6318A								; roll until non-zero, may cause hang if using cheats!
.write_damage:
		SEC
		LDA		_percents,X
; --
;		SBC		_tptr1									; DEBUG invulnerability
; --
		BCS		loc_631A1
		LDA		#0										; clamp to 0
loc_631A1:
		STA		_percents,X
		JSR		_player_overall_armor_calc				; recalc overall percentage
		BNE		.still_alive							; test if drop to zero
		STA		_player_explode_timer_tmp				; init explode timer (zero-1 - 255 frames)
		LDA		#2
		STA		_obj_shuffle_mode						; special shuffle mode with explosion proirity
		LDA		#9
		JSR		_hud_msg_set_request_ex					; final message.
		BEQ		_exit									; DEATH
.still_alive:
		CMP		#MIN_PERCENT_VALUE
		BCS		_exit									; final warning test
		LDA		#5
		JSR		_hud_msg_set_request					; less than default value means common alert
_exit:
		LDX		_tptr1+1								; restore X back
		RTS

; =============== S U B	R O U T	I N E =======================================
_far_obj_misc_collision_test_lvl_prg_safe:
		LDA		_obj_flags._misc
		BEQ		loc_63220
		CMP		#$FF
		BEQ		loc_63220
		LDA		_obj_y_velocity._misc
		BNE		loc_63216
		LDY		#6
		JSR		_mmc1_prg
		LDX		#0
		LDY		#$A
		JSR		_collision_test
		BEQ		loc_63220
		SEC
		LDA		_obj_y._misc
		SBC		_obj_y._player
		BCC		loc_63220
		CMP		#$28
		BCC		loc_63220
		CMP		#$30
		BCS		loc_63220
		LDA		#1
		STA		_obj_hcoll._player
		STA		_obj_vcoll._player
		INC		_obj_sit_state._misc
		LDA		_obj_sit_state._misc
		CMP		#$1E
		BCC		loc_63220
		LDA		#5
		STA		_obj_y_velosity_target._misc
		RTS
loc_63216:
		LDA		_obj_y._misc
		CMP		#$E6
		BCC		loc_63220
		LDA		#0
		STA		_obj_flags._misc
loc_63220:
;		LDX		_lvl_idx
;		LDY		_lvl_prg_bank_list,X
		JMP		_lvl_prg_init

; =============== S U B	R O U T	I N E =======================================
_obj_add_score:
		STX		_tptr1
		LDX		_obj_hndl_idx,Y
		CLC
		LDA		_scores_table,X
		ADC		_score_hex
		STA		_score_hex
		LDA		_score_hex+1
		ADC		#0
		STA		_score_hex+1
		LDX		_tptr1
		RTS

; =============== S U B	R O U T	I N E =======================================
_collision_test:
		STY		_tptr1
		STX		_tptr1+1
		LDY		_obj_spr_idx,X
		LDA		_spr_bbox_height_list,Y
		BNE		loc_63250
		JMP		loc_632D0
loc_63250:
		LDA		_spr_bbox_y_list,Y
		CLC
		ADC		_obj_y,X
		STA		_tmpE4
		ADC		_spr_bbox_width_list,Y
		STA		_tmpE5
		LDX		_tptr1
		LDY		_obj_spr_idx,X
		LDA		_spr_bbox_y_list,Y
		CLC
		ADC		_obj_y,X
		CMP		_tmpE5
		BCS		loc_632D0
		ADC		_spr_bbox_width_list,Y
		CMP		_tmpE4
		BCC		loc_632D0
		CLC
		LDA		_obj_x_lo,X
		ADC		_spr_bbox_x_list,Y
		STA		_tmpE6
		LDA		_obj_x_hi,X
		ADC		#0
		STA		_tmpE7
		LDX		_tptr1+1
		LDY		_obj_spr_idx,X
		CLC
		LDA		_obj_x_lo,X
		ADC		_spr_bbox_x_list,Y
		STA		_tmpE8
		LDA		_obj_x_hi,X
		ADC		#0
		STA		_tmpE9
		LDX		_tptr1+1
		LDY		_tptr1
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
		LDY		_tptr1
		LDX		_tptr1+1
		LDA		#1
		RTS
loc_632D0:
		LDY		_tptr1
		LDX		_tptr1+1
		LDA		#0
		RTS

; =============== S U B	R O U T	I N E =======================================
_lvl_obj_vars_init:
		LDX		#$B
loc_632D9:
		LDA		#0
		STA		_obj_flags,X
		LDA		#3
		STA		_obj_var4,X
		STA		_obj_var3,X
		DEX
		BPL		loc_632D9
		LDY		_lvl_state_idx
		LDA		_lvl_player_start_y_list,Y
		STA		_obj_y._player
		LDA		_lvl_player_start_x_lo_list,Y
		STA		_obj_x_lo._player
		LDA		_lvl_player_start_x_hi_list,Y
		STA		_obj_x_hi._player
		LDA		#0
		STA		_obj_y_velosity_target._player
		STA		_obj_y_velocity._player
		STA		_obj_x_velosity_target._player
		STA		_obj_x_velocity._player
		LDA		#3
		STA		_obj_pal_idx._player
		LDA		#$41
		STA		_obj_flags._player
		LDA		#1
		STA		_obj_var5._player
		LDA		_lvl_spr_hit0_x_lo_list,Y
		STA		_obj_x_lo._spr0hit
		LDA		_lvl_spr_hit0_x_hi_list,Y
		STA		_obj_x_hi._spr0hit
		LDA		_lvl_spr_hit0_y_list,Y
		STA		_obj_y._spr0hit
		LDA		#$21
		STA		_obj_flags._spr0hit
		LDA		#0
		STA		_obj_spr_idx._spr0hit
		STA		_obj_pal_idx._spr0hit
		RTS

; =============== S U B	R O U T	I N E =======================================
_far_spr_manager_lvl_prg_safe:
		LDY		#6
		JSR		_mmc1_prg
		SEC
		LDA		_obj_x_lo._player
		SBC		_player_wnd_scroll_counter
		STA		_tmpE5
		LDA		_obj_x_hi._player
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
		STA		_player_spr_out_cnt
		LDY		_obj_manager_tmp_cnt
		LDA		(_ptr1),Y
		BNE		loc_63368
		LDY		_player_spr_out_reload
		STY		_player_spr_out_cnt
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
;		LDX		_lvl_idx
;		LDY		_lvl_prg_bank_list,X
		JMP		_lvl_prg_init

_obj_shuffle_tables:
		.BYTE	 $C,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, $A, $B,  0,  0,  0	; player sprite priority, never blink
		.BYTE	 $C,  0,  4,  5,  6,  7,  8,  9, $B,  1,  2,  3, $A,  0,  0,  0
		.BYTE	 $C,  4,  5,  6,  0,  1,  2,  3,  7,  8,  9, $A, $B,  0,  0,  0	; explosion sprites priority, when dead
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
		LDA		_obj_x_lo,Y
		SBC		_tmpE5
		STA		_obj_scr_x_pos_tmp
		LDA		_obj_x_hi,Y
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
		LDA		_obj_y,Y
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
		STA		_obj_spr_width_tmp
		INY
		LDA		(_ptr0),Y
		STA		_obj_spr_height_tmp
		INY
		PLP
		BEQ		.spr_left_flip_draw_loop
		BNE		.spr_right_flip_draw_loop
; --------------------------------
.spr_left_flip_draw_loop:
		INC		_player_spr_out_cnt
		BPL		loc_63473
		TYA
		CLC
		ADC		_obj_spr_width_tmp
		TAY
		JMP		loc_634CB
loc_63473:
		LDA		_obj_spr_width_tmp
		STA		_tptr1
		LDA		(_ptr0),Y
		STA		_tmpE7
		CLC
		ADC		_obj_scr_x_pos_tmp
		STA		_tptr1+1
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
		LDA		_tptr1+1
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
		DEC		_tptr1
		BEQ		loc_634CB
		CLC
		LDA		_tptr1+1
		ADC		#8
		STA		_tptr1+1
		BCC		loc_63495
		LDA		_tmpE7
		EOR		#1
		STA		_tmpE7
		BPL		loc_63495
loc_634CB:
		DEC		_obj_spr_height_tmp
		BEQ		locret_634DA
		CLC
		LDA		_obj_scr_y_pos_tmp
		ADC		#8
		STA		_obj_scr_y_pos_tmp
		INY
		JMP		.spr_left_flip_draw_loop
locret_634DA:
		RTS
; --------------------------------
.spr_right_flip_draw_loop:
		INC		_player_spr_out_cnt
		BPL		loc_634E3
		INY
		JMP		loc_63545
loc_634E3:
		LDA		_obj_spr_width_tmp
		STA		_tptr1
		LDA		(_ptr0),Y
		EOR		#$FF
		CLC
		ADC		#1
		STA		_tmpE7
		CLC
		ADC		_obj_scr_x_pos_tmp
		STA		_tptr1+1
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
		ADC		_obj_spr_width_tmp
		TAY
loc_6350F:
		DEY
		LDA		_tmpE7
		BEQ		loc_63530
		LDA		(_ptr0),Y
		BEQ		loc_63530
		STA		_spr_buf+1,X
		LDA		_tptr1+1
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
		DEC		_tptr1
		BEQ		loc_63545
		CLC
		LDA		_tptr1+1
		ADC		#8
		STA		_tptr1+1
		BCC		loc_6350F
		LDA		_tmpE7
		EOR		#1
		STA		_tmpE7
		BPL		loc_6350F
loc_63545:
		DEC		_obj_spr_height_tmp
		BEQ		locret_63558
		CLC
		LDA		_obj_scr_y_pos_tmp
		ADC		#8
		STA		_obj_scr_y_pos_tmp
		CLC
		TYA
		ADC		_obj_spr_width_tmp
		TAY
		JMP		.spr_right_flip_draw_loop
locret_63558:
		RTS

; =============== S U B	R O U T	I N E =======================================
_obj_position_adjust:
		LDX		#$A
loc_6355B:
		LDA		_obj_flags,X
		BEQ		loc_635AB
		CLC
		LDA		_obj_x_velocity,X
		TAY
		ADC		_obj_x_lo,X
		STA		_obj_x_lo,X
		TYA
		BMI		loc_63571
		BCC		loc_63575
		INC		_obj_x_hi,X
		JMP		loc_63575
loc_63571:
		BCS		loc_63575
		DEC		_obj_x_hi,X
loc_63575:
		CLC
		LDA		_obj_y_velocity,X
		ADC		_obj_y,X
		STA		_obj_y,X
		DEC		_obj_var3,X
		BNE		loc_635AB
		LDA		_obj_var4,X
		STA		_obj_var3,X
		LDA		_obj_x_velosity_target,X
		CMP		_obj_x_velocity,X
		BEQ		loc_63598
		BMI		loc_63596
		INC		_obj_x_velocity,X
		JMP		loc_63598
loc_63596:
		DEC		_obj_x_velocity,X
loc_63598:
		LDA		_obj_y_velosity_target,X
		CMP		_obj_y_velocity,X
		BEQ		loc_635AB
		BMI		loc_635A8
		INC		_obj_y_velocity,X
		JMP		loc_635AB
loc_635A8:
		DEC		_obj_y_velocity,X
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
_hud_init:
		LDA		_PPU_STATUS
		LDA		#<[_tlm_default_hud]
		STA		_tptr0
		LDA		#>[_tlm_default_hud]
		STA		_tptr0+1
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
		LDA		(_tptr0),Y
		STA		_PPU_DATA
		INY
		BNE		loc_63654
		LDA		_lvl_idx
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
		LDY		_lvl_state_idx
		LDA		_lvl_room_init_draw_ptr_list_lo,Y
		STA		_tptr0
		STA		_tptr1
		LDA		_lvl_room_init_draw_ptr_list_hi,Y
		STA		_tptr0+1
		STA		_tptr1+1
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
		LDA		(_tptr0),Y
		STA		_PPU_DATA
		INY
		BNE		loc_636A4
		INC		_tptr0+1
loc_636AE:
		LDA		(_tptr0),Y
		STA		_PPU_DATA
		INY
		BNE		loc_636AE
		INC		_tptr0+1
loc_636B8:
		LDA		(_tptr0),Y
		STA		_PPU_DATA
		INY
		CPY		#$A0
		BNE		loc_636B8
		CLC
		LDA		_tptr1
		ADC		#$A0
		STA		_tptr0
		LDA		_tptr1+1
		ADC		#2
		STA		_tptr0+1
		LDA		#$23
		STA		_PPU_ADDR
		LDA		#$D0
		STA		_PPU_ADDR
		LDY		#0
loc_636DB:
		LDA		(_tptr0),Y
		STA		_PPU_DATA
		INY
		CPY		#$30
		BNE		loc_636DB
		CLC
		LDA		_tptr1
		ADC		#$D0
		STA		_tptr0
		STA		_tptr1
		LDA		_tptr1+1
		ADC		#2
		STA		_tptr0+1
		STA		_tptr1+1
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
		LDA		(_tptr0),Y
		STA		_PPU_DATA
		INY
		BNE		loc_6370E
		INC		_tptr0+1
loc_63718:
		LDA		(_tptr0),Y
		STA		_PPU_DATA
		INY
		BNE		loc_63718
		INC		_tptr0+1
loc_63722:
		LDA		(_tptr0),Y
		STA		_PPU_DATA
		INY
		CPY		#$A0
		BNE		loc_63722
		CLC
		LDA		_tptr1
		ADC		#$A0
		STA		_tptr0
		LDA		_tptr1+1
		ADC		#2
		STA		_tptr0+1
		LDA		#$27
		STA		_PPU_ADDR
		LDA		#$D0
		STA		_PPU_ADDR
		LDY		#0
loc_63745:
		LDA		(_tptr0),Y
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
		TAX
		LDY		#8
loc_63769:
		STA		_PPU_DATA
		DEX
		BNE		loc_63769
		DEY
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
; text display routine, print out a text resource at once on the screen
; resource format:
; 0,1	- ppu offset to printf
; 2		- line size N
; ...	- data of size N
; ...	- more chunks
; FF	- eof
;
_printf:
		STA		_printf_ofs_tmp
		STY		_printf_ofs_tmp+1
		LDY		#0
.printf_line:
		LDA		(_printf_ofs_tmp),Y
		BMI		.printf_eof
		BIT		_PPU_STATUS
		STA		_PPU_ADDR
		INY
		LDA		(_printf_ofs_tmp),Y
		STA		_PPU_ADDR
		INY
		LDA		(_printf_ofs_tmp),Y
		STA		_printf_len_tmp
		INY
.printf_loop:
		LDA		(_printf_ofs_tmp),Y
		STA		_PPU_DATA
		INCY	_printf_ofs_tmp
		DEC		_printf_len_tmp
		BNE		.printf_loop
		BEQ		.printf_line
.printf_eof:
		RTS

; =============== S U B	R O U T	I N E =======================================
_far_apu_mus_load_unsafe:
		LDA		_snd_idx
		BEQ		_far_apu_load
		LDA		_apu_enable
		AND		#APU_MUS_ENABLE
		BNE		_far_apu_load
loc_637C0:
		LDA		#$FF									; if music or sounds disabled, force idx to FF
		STA		_snd_idx								; to prevent accidental music start
		RTS

; =============== S U B	R O U T	I N E =======================================
_far_apu_snd_load_unsafe:
		LDA		_apu_enable
		AND		#APU_SND_ENABLE
		BEQ		loc_637C0

; =============== S U B	R O U T	I N E =======================================
_far_apu_load:
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
		ADC		#<[_sound_lib]
		STA		_ptr0
		LDA		_ptr0+1
		ADC		#>[_sound_lib]
		STA		_ptr0+1
		LDY		#0
loc_637B5:
		STY		_apu_load_tmp_Y
		LDA		(_ptr0),Y
		BMI		locret_637CC
		TAX
		INY
		LDA		(_ptr0),Y
		JSR		_apu_load_chnl
		LDY		_apu_load_tmp_Y
		INY
		INY
		CPY		#8
		BNE		loc_637B5
locret_637CC:
		RTS

_sound_lib:
		SOUND	SILENCE,  0,  0,  1,  0,  2,  0,  3,  0
		SOUND	SHOTGUN,  0,  1,  3,  2,$FF,$FF,$FF,$FF	; $01
		SOUND	ROCKET1,  3,  3,$FF,$FF,$FF,$FF,$FF,$FF	; $02
		SOUND	ROCKET2,  3,  4,$FF,$FF,$FF,$FF,$FF,$FF	; $03
		SOUND	BIGBOOM,  2,  5,  3,  6,$FF,$FF,$FF,$FF	; $04
		SOUND	EXPLDSM,  2,  7,  3,  8,$FF,$FF,$FF,$FF	; $05
		SOUND	EXPLDMD,  2,  9,  3, $A,$FF,$FF,$FF,$FF	; $06
		SOUND	EXPLDLA,  2, $B,  3, $C,$FF,$FF,$FF,$FF	; $07
		SOUND	PICKUP1,  0, $D,$FF,$FF,$FF,$FF,$FF,$FF	; $08
		SOUND	PCKUP1A,  0, $D,  1, $E,$FF,$FF,$FF,$FF	; original version for no-music mode
		SOUND	PICKUP2,  0, $F,$FF,$FF,$FF,$FF,$FF,$FF	; $09
		SOUND	PCKUP2A,  0, $F,  1,$10,$FF,$FF,$FF,$FF	; original version for no-music mode
		SOUND	JETPACK,  3,$11,$FF,$FF,$FF,$FF,$FF,$FF	; $0A
		SOUND	RBLAND1,  2,$13,  3,$14,$FF,$FF,$FF,$FF	; $0B
		SOUND	RBLAND2,  0,$12,  2,$13,  3,$14,$FF,$FF	; $0C
		SOUND	ACIDHIS,  3,$3B,$FF,$FF,$FF,$FF,$FF,$FF
		SOUND	SOUND0E,  3,  0,$FF,$FF,$FF,$FF,$FF,$FF
		SOUND	SOUND0F,  0,$26,  3,$27,$FF,$FF,$FF,$FF
		SOUND	SOUND10,  0,$28,  3,$29,$FF,$FF,$FF,$FF
		SOUND	SOUND11,  3,$2A,$FF,$FF,$FF,$FF,$FF,$FF
		SOUND	WCHANGE,  0,$2B,$FF,$FF,$FF,$FF,$FF,$FF	; $21
;		SOUND	WCHANG_,  0,$2B,  2,$2C,$FF,$FF,$FF,$FF	; $12	unused, conflict with explosions
		SOUND	PASSBP0,  0,$30,  2,$32,$FF,$FF,$FF,$FF
		SOUND	PASSBP1,  0,$2D,  2,$31,$FF,$FF,$FF,$FF
		SOUND	PASSBP2,  0,$2D,  2,$32,$FF,$FF,$FF,$FF
		SOUND	PASSBP3,  0,$2D,  2,$33,$FF,$FF,$FF,$FF
		SOUND	PASSBP4,  0,$2E,  2,$31,$FF,$FF,$FF,$FF
		SOUND	PASSBP5,  0,$2E,  2,$32,$FF,$FF,$FF,$FF
		SOUND	PASSBP6,  0,$2E,  2,$33,$FF,$FF,$FF,$FF
		SOUND	PASSBP7,  0,$2F,  2,$31,$FF,$FF,$FF,$FF
		SOUND	PASSBP8,  0,$2F,  2,$32,$FF,$FF,$FF,$FF
		SOUND	PASSBP9,  0,$2F,  2,$33,$FF,$FF,$FF,$FF
		SOUND	MTITLE_,  0,$15,  1,$16,  2,$17,  3,$18	; $1D
		SOUND	MREPAIR,  0,$1A,  1,$1B,  2,$1C,  3,$1D	; $1E
		SOUND	MENDING,  0,$1E,  1,$1F,  2,$20,  3,$21	; $1F
		SOUND	MGMOVER,  0,$22,  1,$23,  2,$24,  3,$25	; $20
		SOUND	MLEVEL0,  1,$19,$FF,$FF,$FF,$FF,$FF,$FF	; $22
		SOUND	MLEVEL1,  1,$37,$FF,$FF,$FF,$FF,$FF,$FF	; $23
		SOUND	MLEVEL2,  1,$38,$FF,$FF,$FF,$FF,$FF,$FF	; $24
		SOUND	MLEVEL3,  1,$39,$FF,$FF,$FF,$FF,$FF,$FF	; $25
		SOUND	MLEVEL4,  1,$3A,$FF,$FF,$FF,$FF,$FF,$FF	; $26

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
_lvl_prg_init:
		LDX		_lvl_idx								; set the lvl data bank
		LDY		_lvl_prg_bank_list,X

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

_lvl_prg_bank_list:
		.BYTE	  0,  1,  2,  3,  4

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

; hud attributes fo every single level, mostly the same except levels 3-5
_attr_hud_def_lib:
		.BYTE	$AF,$AF,$3F, $F,$CF,$FF,$EF,$FF
		.BYTE	$A5,$A5,$A3,$A8,$A2,$AC,$A5,$A5
		.BYTE	 $F, $F,$3F, $F,$CF,$FF,$EF,$FF
		.BYTE	$A5,$A5,$A3,$A8,$A2,$AC,$A5,$A5
		.BYTE	$AF,$AF,$3F, $F,$CF,$FF,$EF,$FF
		.BYTE	$A5,$A5,$A3,$A8,$A2,$AC,$A5,$A5
		.BYTE	$AF,$AF,$3F, $F,$CF,$FF,$EF,$FF
		.BYTE	$A5,$A5,$A3,$A8,$A2,$AC,$A5,$A5
		.BYTE	$AF,$AF,$3F, $F,$CF,$FF,$EF,$FF
		.BYTE	$A5,$A5,$A3,$A8,$A2,$AC,$A5,$A5
_obj_spr_lib:
		.BYTE	$3A,$44,$30,$54,$48,$3E,$4C,$34,$54,$50,$5A,$69,$6D,$71,$77,$83
		.BYTE	$65,$67,$65,$67,$7D,$88,$12,$B7,$B1,$AD,$12,  0,  0,  0,  0,  0
		.BYTE	  0,$AC,  1,  2,$59,  1,  2,  1,  0
_obj_init_flags_lib:
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
_projectiles_damage_list:
		.BYTE	PROJ0_FAMAGE_VALUE
		.BYTE	PROJ1_FAMAGE_VALUE
		.BYTE	PROJ2_FAMAGE_VALUE
		.BYTE	PROJ3_FAMAGE_VALUE
_body_targets_list:
		.BYTE	  2,  0,  1
_end_of_level_enemy_appear_pos_x:
		.BYTE	  9,  5, $E,  5,  5
_end_of_level_enemy_table:
		.BYTE	  0,  5,  0,  0,  3,  8,  0,  0,  0,  0
_lvl2_transition_pal_lib:
		.BYTE	 $F,$18,$1B,$2B, $F,  6,$16,$26, $F,$18,$28,$38, $F,$12,$22,$32
		.BYTE	 $F, $C,$1C,$2C, $F,  6,$16,$26, $F,$18,$28,$38, $F,$12,$22,$32
		.BYTE	 $F,$22,$18,$29, $F,$18,$29,$39, $F,$18,$28,$38, $F,$12,$22,$32
; --
; original version of hi-score table
_def_score_tbl:
		.BYTE	0,0,2,0,0
		.BYTE	0,0,1,0,0
		.BYTE	0,0,0,8,0
		.BYTE	0,0,0,6,0
		.BYTE	0,0,0,4,0
		.BYTE	0,0,0,1,0
		.BYTE	$21,$0B
		STRING	_H,_I,_G,_H,_S,_C,_O,_R,_E,_S
		.BYTE	$21,$44
		STRING	_1,_DT
		.BYTE	$21,$51
		STRING	_G,_DT,_H,_A,_R,_R,_I,_S,_O,_N
		.BYTE	$21,$84
		STRING	_2,_DT
		.BYTE	$21,$91
		STRING	_S,_DT,_N,_I,_C,_O,_L,_,_,_
		.BYTE	$21,$C4
		STRING	_3,_DT
		.BYTE	$21,$D1
		STRING	_S,_DT,_C,_R,_O,_W,_,_,_,_
		.BYTE	$22,$04
		STRING	_4,_DT
		.BYTE	$22,$11
		STRING	_J,_DT,_B,_O,_N,_A,_R,_,_,_
		.BYTE	$22,$44
		STRING	_5,_DT
		.BYTE	$22,$51
		STRING	_J,_DT,_T,_E,_L,_,_,_,_,_
		.BYTE	$22,$84
		STRING	_6,_DT
		.BYTE	$22,$91
		STRING	_H,_DT,_B,_I,_N,_N,_S,_,_,_
		.BYTE	$FF
_def_score_tbl_end:

		ECHO "bank 7 free space", [SYS_STUB_ORG-.]
		ECHO " zero ram free", [$100 - _end_of_zero_ram]
		ECHO " 300 ram free", [$100 - _end_of_300_ram]
		ECHO " 500 ram free", [$100 - _end_of_500_ram]
		ECHO " 600-700 ram free", [$200 - _end_of_600_700_ram]

		ORG	SYS_STUB_ORG, $00
RESET:
		SAFE_STUB0

		ORG	SYS_VECTOR_ORG, $00

		.WORD NMI
		.WORD RESET
		.WORD IRQ
