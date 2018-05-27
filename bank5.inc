
		ORG $8000, $00

; =============== S U B	R O U T	I N E =======================================
_j_nmi0:
		JMP		_nmi0

; =============== S U B	R O U T	I N E =======================================
_j_exec_sub:
		JMP		_exec_sub

; =============== S U B	R O U T	I N E =======================================
_asci_bios_msg_display_with_skip:
		LDY		#0
		STY		_asci_msg_skip_forbid
		JMP		_asci_bios_msg_display

; =============== S U B	R O U T	I N E =======================================
_exec_sub:
		ASL
		TAX
		LDA		_subs_lib,X
		STA		_tmp_buf0
		LDA		_subs_lib+1,X
		STA		_tmp_buf0+1
		LDA		#$78
		STA		_star_field_center_pos_x
		LDA		#$70
		STA		_star_field_center_pos_y
		JMP		(_tmp_buf0)
_subs_lib:
		.WORD _sub0_intro_title
		.WORD _sub1_repair_lab
		.WORD _sub2_game_over
		.WORD _sub3_ending

; =============== S U B	R O U T	I N E =======================================
_sub0_intro_title:
		LDA		#9
		JSR		_asci_bios_msg_display
		LDY		#0
		STY		_PPU_CTRL
		STY		_PPU_MASK
		BIT		_PPU_STATUS
		LDA		#$21
		STA		_intro_txt_ppu_ofs_tmp+1
		STA		_PPU_ADDR
		LDA		#$22
		STA		_intro_txt_ppu_ofs_tmp
		STA		_PPU_ADDR
		LDA		_asci_copyrights_ofs,X
		STA		_text_data_tmp_ptr
		LDA		_asci_copyrights_ofs+1,X
		STA		_text_data_tmp_ptr+1
loc_44059:
		LDA		(_text_data_tmp_ptr),Y
		CMP		#$FE
		BEQ		loc_44076
		CMP		#$FA
		BEQ		loc_440A0
		CMP		#$C0
		BCS		loc_4406A
		SEC
		SBC		#$40
loc_4406A:
		STA		_PPU_DATA
		INC		_text_data_tmp_ptr
		BNE		loc_44059
		INC		_text_data_tmp_ptr+1
		JMP		loc_44059
loc_44076:
		LDA		_intro_txt_ppu_ofs_tmp
		CLC
		ADC		#$20
		STA		_intro_txt_ppu_ofs_tmp
		LDA		_intro_txt_ppu_ofs_tmp+1
		ADC		#0
		STA		_intro_txt_ppu_ofs_tmp+1
		LDA		_text_data_tmp_ptr
		CLC
		ADC		#1
		STA		_text_data_tmp_ptr
		LDA		_text_data_tmp_ptr+1
		ADC		#0
		STA		_text_data_tmp_ptr+1
		BIT		_PPU_STATUS
		LDA		_intro_txt_ppu_ofs_tmp+1
		STA		_PPU_ADDR
		LDA		_intro_txt_ppu_ofs_tmp
		STA		_PPU_ADDR
		JMP		loc_44059
loc_440A0:
		LDA		#$81
		STA		_PPU_CTRL
		LDA		#$1D
		JSR		_j_apu_mus_start
		LDA		#1
		STA		_event_reload_idx
		LDX		#$FF
loc_440B1:
		LDA		_event_idx
		BEQ		loc_440B1
		LDA		#0
		STA		_event_idx
		TXA
		PHA
		LDX		#3
		LDY		#0
loc_440C1:
		DEY
		BNE		loc_440C1
		DEX
		BNE		loc_440C1
		JSR		_call_j_far_apu_play
		PLA
		TAX
		DEX
		BNE		loc_440B1
		LDA		#0
		STA		_event_idx
		STA		_delay_counter
		STA		_delay_counter+1
		STA		_screen_idx
		LDA		#2
		STA		_event_reload_idx
		LDA		#$FF
		STA		byte_3B1
		LDA		#8
		STA		_ppu_scroll_x_override_redundant
		LDA		#$80
		STA		_PPU_CTRL
		JMP		_event_exec

; =============== S U B	R O U T	I N E =======================================
_hiscore_display:
		LDA		#0
		STA		_event_idx
		STA		byte_3B4
		STA		byte_3B5
		STA		byte_3D7
		STA		_PPU_CTRL
		STA		_PPU_MASK
		STA		_delay_counter
		LDA		#8
		STA		byte_3B2
		LDA		#1
		STA		byte_3D3
		LDA		#3
		STA		_event_reload_idx
		LDA		#$FF
		STA		byte_3B1
		JSR		_spr_buf_clear
		JSR		_log_msg_buf_clear
		LDA		#4
		STA		_screen_idx
		JSR		_screen_display
		LDA		#$81
		STA		_PPU_CTRL
		JMP		_event_exec

; =============== S U B	R O U T	I N E =======================================
_sub3_ending:
		LDA		#0
		STA		_event_idx
		STA		_PPU_CTRL
		STA		_PPU_MASK
		STA		byte_3D4
		STA		_delay_counter
		STA		_delay_counter+1
		STA		_pl_bkcp_continues
		STA		byte_3B2
		LDA		#4
		STA		_event_reload_idx
		LDA		#$FF
		STA		byte_3B1
		LDA		#1
		STA		byte_3D3
		LDA		#7
		STA		_screen_idx
		JSR		_screen_display
		JSR		_spr_buf_clear
		LDA		#$7F
		STA		_spr_buf+4
		STA		_spr_buf+8
		STA		_spr_buf+$C
		STA		_spr_buf+$10
		STA		_spr_buf+$14
		LDA		#$87
		STA		_spr_buf+$18
		STA		_spr_buf+$1C
		STA		_spr_buf+$20
		STA		_spr_buf+$24
		LDA		#2
		STA		_spr_buf+6
		STA		_spr_buf+$A
		STA		_spr_buf+$E
		STA		_spr_buf+$12
		STA		_spr_buf+$16
		STA		_spr_buf+$1A
		STA		_spr_buf+$1E
		STA		_spr_buf+$22
		STA		_spr_buf+$26
		LDX		#$F7
		STX		_spr_buf+5
		LDA		#$80
		STA		_spr_buf+7
		STA		_spr_buf+$1B
		LDA		#$43
		STA		_spr_buf+9
		LDA		#$88
		STA		_spr_buf+$B
		STA		_spr_buf+$1F
		INX
		STX		_spr_buf+$D
		LDA		#$90
		STA		_spr_buf+$F
		STA		_spr_buf+$23
		INX
		STX		_spr_buf+$11
		LDA		#$98
		STA		_spr_buf+$13
		STA		_spr_buf+$27
		INX
		STX		_spr_buf+$15
		LDA		#$A0
		STA		_spr_buf+$17
		LDX		#$FB
		STX		_spr_buf+$19
		INX
		STX		_spr_buf+$1D
		INX
		STX		_spr_buf+$21
		INX
		STX		_spr_buf+$25
		LDA		#$1F
		JSR		_j_apu_mus_start
		LDA		#$81
		STA		_PPU_CTRL
		JMP		_event_exec

; =============== S U B	R O U T	I N E =======================================
_sub1_repair_lab:
		LDA		#0
		STA		_event_idx
		LDA		#5
		STA		_event_reload_idx
		LDA		#$FF
		STA		byte_3B1
		LDA		#8
		STA		_screen_idx
		JSR		_screen_display
		JSR		_repair_ppu_head_upgrade_display
		JSR		_repair_ppu_body_upgrade_display
		JSR		_repair_ppu_arms_upgrade_display
		JSR		_repair_ppu_legs_upgrade_display
		LDA		#$A8
		STA		byte_3B8
		LDA		#$40
		STA		_tmp3C3
		LDA		#2
		STA		_repair_cur_body_index
		STA		_repair_body_index_request
		LDA		#$1E
		JSR		_j_apu_mus_start
		LDA		_cur_head_percent
		STA		_cur_head_percent_backup
		LDA		_cur_body_percent
		STA		_cur_body_percent_backup
		LDA		_cur_arms_percent
		STA		_cur_arms_percent_backup
		LDA		_cur_legs_percent
		STA		_cur_legs_percent_backup
		LDA		_cur_repair_tanks
		STA		_cur_repair_tanks_backup
		LDA		#6
		STA		_PPU_MASK
		LDA		#$80
		STA		_PPU_CTRL
		JMP		_event_exec

; =============== S U B	R O U T	I N E =======================================
_sub2_game_over:
		LDA		#0
		STA		_event_idx
		STA		byte_3B4
		STA		byte_3B5
		STA		byte_3D7
		STA		_PPU_CTRL
		STA		_PPU_MASK
		STA		_delay_counter
		STA		_delay_counter+1
		LDA		#8
		STA		byte_3B2
		LDA		#1
		STA		byte_3D3
		LDA		#6
		STA		_event_reload_idx
		LDA		#$FF
		STA		byte_3B1
		LDA		_pl_bkcp_continues
		BEQ		loc_4429E
		LDA		#1
		STA		_title_menu_idx
		JMP		loc_442AB
loc_4429E:
		LDA		#0
		STA		_title_menu_idx
		STA		_new_game_start_flag
		LDA		_cur_lvl_idx
		STA		_pl_bckp_lvl_idx
loc_442AB:
		JSR		_spr_buf_clear
		JSR		_log_msg_buf_clear
		LDA		#4
		STA		_screen_idx
		JSR		_screen_display
		LDX		#$27
loc_442BB:
		JSR		_star_field_randomize
		AND		#$3F
		STA		_star_field_data0,X
		JSR		_star_field_randomize
		AND		#$3F
		STA		_star_field_data1,X
		LDA		#0
		STA		_star_field_data2,X
		DEX
		BPL		loc_442BB
		BIT		_PPU_STATUS
		LDA		#$21
		STA		_PPU_ADDR
		LDA		#$CB
		STA		_PPU_ADDR
		LDX		#0
loc_442E2:
		LDA		_txt_game_over,X
		BMI		loc_442ED
		STA		_PPU_DATA
		INX
		BNE		loc_442E2
loc_442ED:
		LDA		#$20
		JSR		_j_apu_mus_start
		LDA		#$81
		STA		_PPU_CTRL
		JMP		_event_exec
_txt_game_over:
		.BYTE	  7, 1, $D,  5, 0, 0, $F, $16, 5, $12, $FF

; =============== S U B	R O U T	I N E =======================================
_spr_buf_clear:
		LDX		#0
		LDA		#$F0
loc_44309:
		STA		_spr_buf,X
		INX
		BNE		loc_44309
		RTS

; =============== S U B	R O U T	I N E =======================================
_log_msg_buf_clear:
		LDX		#$6F
		LDA		#0
loc_44314:
		STA		_log_msg_buf,X
		DEX
		BPL		loc_44314
		RTS

; =============== S U B	R O U T	I N E =======================================
_log_msg_buf_scroll_up:
		LDX		#$1B
loc_4431D:
		LDA		_log_msg_buf+$1C,X
		STA		_log_msg_buf,X
		DEX
		BPL		loc_4431D
		LDX		#$1B
loc_44328:
		LDA		_log_msg_buf+$38,X
		STA		_log_msg_buf+$1C,X
		DEX
		BPL		loc_44328
		LDX		#$1B
loc_44333:
		LDA		_log_msg_buf+$54,X
		STA		_log_msg_buf+$38,X
		DEX
		BPL		loc_44333
		LDX		#$1B
		LDA		#0
loc_44340:
		STA		_log_msg_buf+$54,X
		DEX
		BPL		loc_44340
		RTS

; =============== S U B	R O U T	I N E =======================================
_event_exec:
		LDA		_event_idx
		BEQ		_event_exec
		ASL
		TAX
		LDA		_events_lib,X
		STA		_obj_hcoll._enemy+1
		INX
		LDA		_events_lib,X
		STA		_obj_hcoll._enemy+2
		LDA		#0
		STA		_event_idx
		JMP		(_obj_hcoll._enemy+1)
_events_lib:
		.WORD _event0
		.WORD _event1
		.WORD _event2_title
		.WORD _event3_hiscore
		.WORD _event4_ending
		.WORD _event5_repair
		.WORD _event6_game_over
		.WORD _event7_asci_msg_print

; =============== S U B	R O U T	I N E =======================================
_event0:
		JMP		_event_exec

; =============== S U B	R O U T	I N E =======================================
_event1:
		JMP		_event_exec
_spr_presents:
		.BYTE	$10, $12,  5, $13,  5, $E, $14, $13
_spr_oduction:
		.BYTE	 $F, 4, $15, 3, $14, 9, $F, $E

; =============== S U B	R O U T	I N E =======================================
_event2_title:
		LDX		_screen_idx
		LDA		_delays_list_lo,X
		CMP		_delay_counter+1
		BNE		loc_4439A
		LDA		_delays_list_hi,X
		CMP		_delay_counter
		BEQ		_title_screen_change
loc_4439A:
		JMP		loc_4445A
_title_screen_change:
		LDX		_screen_idx
		CPX		#4
		BCC		loc_443B5
		DEX
		DEX
		STX		_screen_idx
		LDA		_delays_list_lo,X
		STA		_delay_counter+1
		LDA		_delays_list_hi,X
		STA		_delay_counter
loc_443B5:
		JSR		_spr_buf_clear
		LDA		_screen_idx
		JSR		_screen_display
		LDA		_screen_idx
		CMP		#0
		BNE		loc_443EC
		LDA		#$68
		PHA
		LDX		#0
loc_443CA:
		TXA
		ASL
		ASL
		TAY
		PLA
		STA		_spr_buf+3,Y
		CLC
		ADC		#8
		PHA
		LDA		#0
		STA		_spr_buf+2,Y
		LDA		#$80
		STA		_spr_buf,Y
		LDA		_spr_presents,X
		STA		_spr_buf+1,Y
		INX
		CPX		#8
		BNE		loc_443CA
		PLA
loc_443EC:
		LDA		_screen_idx
		CMP		#1
		BNE		loc_44445
		BIT		_PPU_STATUS
		LDA		#$22
		STA		_PPU_ADDR
		LDA		#$C
		STA		_PPU_ADDR
		LDA		#$92
		STA		_PPU_DATA
		LDA		#$93
		STA		_PPU_DATA
		LDA		#$84
		STA		_spr_buf+$C3
		LDA		#$34
		STA		_spr_buf+$C0
		LDA		#1
		STA		_spr_buf+$C1
		LDA		#0
		STA		_spr_buf+$C2
		LDA		#$70
		PHA
		LDX		#0
loc_44423:
		TXA
		ASL
		ASL
		TAY
		PLA
		STA		_spr_buf+3,Y
		CLC
		ADC		#8
		PHA
		LDA		#0
		STA		_spr_buf+2,Y
		LDA		#$7F
		STA		_spr_buf,Y
		LDA		_spr_oduction,X
		STA		_spr_buf+1,Y
		INX
		CPX		#8
		BNE		loc_44423
		PLA
loc_44445:
		LDA		_screen_idx
		CMP		#3
		BNE		loc_4444F
		JSR		_hi_score_display
loc_4444F:
		LDA		#$81
		STA		_PPU_CTRL
		JSR		_read_pad
		INC		_screen_idx
loc_4445A:
		LDA		_delay_counter
		CLC
		ADC		#1
		STA		_delay_counter
		LDA		_delay_counter+1
		ADC		#0
		STA		_delay_counter+1
		LDA		_screen_idx
		CMP		#4
		BNE		loc_44491
		JSR		_star_field_animate
		LDA		_pad
		BEQ		loc_4448B
		LDX		#4
		STX		_screen_idx
		LDA		_delays_list_lo,X
		STA		_delay_counter+1
		LDA		_delays_list_hi,X
		STA		_delay_counter
loc_4448B:
		JSR		_call_j_far_apu_play
		JMP		_event_exec
loc_44491:
		LDA		_disp_spr0hit_use
		BNE		loc_444AD
		LDX		#3
		LDY		#0
loc_4449A:
		DEY
		BNE		loc_4449A
		DEX
		BNE		loc_4449A
		JSR		_read_pad
		JSR		_call_j_far_apu_play
		LDA		#1
		STA		_logo_skip_forbid_flag
		JMP		_event_exec
loc_444AD:
		LDX		#1
		LDY		#0
loc_444B1:
		DEY
		BNE		loc_444B1
		DEX
		BNE		loc_444B1
		JSR		_call_j_far_apu_play
		JSR		_read_pad
		LDY		_title_menu_idx
		LDX		byte_44651,Y
		LDA		_star_field_params1,X
		TAX
		LDA		byte_44655,Y
		STA		_tmp3C3+2
		LDA		_generic_nmi_over_counter
		AND		#$1F
		CMP		#$15
		BCC		loc_444DA
		LDA		#$F0
		BNE		loc_44543
loc_444DA:
		LDA		_logo_skip_forbid_flag
		BEQ		loc_444EA
		LDA		_pad
		BEQ		loc_444E6
		JMP		_wait_for_midscreen_change
loc_444E6:
		LDA		#0
		STA		_logo_skip_forbid_flag
loc_444EA:
		LDA		_pad
		CMP		byte_3B2
		BNE		loc_444FC
		STA		byte_3B2
		LDA		_generic_nmi_over_counter
		AND		#$1F
		BNE		loc_44540
loc_444FC:
		LDA		_pad
		AND		#4
		STA		byte_3B2
		BNE		loc_44510
		LDA		_pad
		STA		byte_3B2
		AND		#$20
		BEQ		loc_44526
loc_44510:
		LDA		_title_menu_idx
		CLC
		ADC		#1
		AND		#3
		STA		_title_menu_idx
		JSR		_reload_delay_counter_2
		LDA		#0
		STA		_generic_nmi_over_counter
		JMP		loc_44540
loc_44526:
		LDA		_pad
		AND		#$10
		BEQ		loc_44540
		LDA		_title_menu_idx
		SEC
		SBC		#1
		AND		#3
		STA		_title_menu_idx
		JSR		_reload_delay_counter_2
		LDA		#0
		STA		_generic_nmi_over_counter
loc_44540:
		LDA		byte_44659,Y
loc_44543:
		STA		_spr_buf,X
		INX
		INX
		INX
		INX
		DEC		_tmp3C3+2
		BNE		loc_44543
		LDA		_apu_music_enable
		BEQ		loc_44561
		LDA		#$4F
		STA		_spr_buf+$A9
		LDA		#1
		STA		_spr_buf+$AD
		JMP		loc_44569
loc_44561:
		LDA		#$3F
		STA		_spr_buf+$A9
		STA		_spr_buf+$AD
loc_44569:
		LDA		_apu_sound_enable
		BEQ		loc_4457B
		LDA		#$4F
		STA		_spr_buf+$C9
		LDA		#1
		STA		_spr_buf+$CD
		JMP		loc_44583
loc_4457B:
		LDA		#$3F
		STA		_spr_buf+$C9
		STA		_spr_buf+$CD
loc_44583:
		LDA		_title_menu_idx
		BNE		loc_4459F
		LDA		_pad
		AND		#9
		BEQ		loc_4459F
		STA		byte_3B2
		LDA		#0
		STA		_new_game_start_flag
		STA		_cur_lvl_idx
		STA		_pl_bckp_lvl_idx
		JMP		_hiscore_display
loc_4459F:
		LDA		_title_menu_idx
		CMP		#1
		BNE		loc_445C3
		LDA		_pad
		AND		#9
		BEQ		loc_445C3
		LDA		_pl_bkcp_continues
		BNE		loc_445BD
		LDA		#0
		STA		_new_game_start_flag
		STA		byte_3B4
		JMP		_wait_for_midscreen_change
loc_445BD:
		LDA		#1
		STA		_new_game_start_flag
		RTS
loc_445C3:
		LDA		_title_menu_idx
		CMP		#2
		BNE		loc_445E7
		LDA		_pad
		CMP		byte_3D3
		BEQ		loc_445E7
		STA		byte_3D3
		AND		#9
		BEQ		loc_445E7
		JSR		_reload_delay_counter_2
		LDA		_apu_music_enable
		CLC
		ADC		#1
		AND		#1
		STA		_apu_music_enable
loc_445E7:
		LDA		_title_menu_idx
		CMP		#3
		BNE		_wait_for_midscreen_change
		LDA		_pad
		CMP		byte_3D3
		BEQ		_wait_for_midscreen_change
		STA		byte_3D3
		AND		#9
		BEQ		_wait_for_midscreen_change
		JSR		_reload_delay_counter_2
		LDA		_apu_sound_enable
		CLC
		ADC		#1
		AND		#1
		STA		_apu_sound_enable
		BEQ		loc_44615
		LDA		#$1D
		JSR		_j_apu_mus_start
		JMP		_wait_for_midscreen_change
loc_44615:
		LDA		#0
		JSR		_j_apu_mus_start
_wait_for_midscreen_change:
		BIT		_PPU_STATUS
		BMI		loc_44623
		BVC		_wait_for_midscreen_change
		LDX		#$13
loc_44623:
		DEX
		BNE		loc_44623
		LDA		#$1A
		STA		$BFFF
		LSR
		STA		$BFFF
		LSR
		STA		$BFFF
		LSR
		STA		$BFFF
		LSR
		STA		$BFFF
		JMP		_event_exec

; =============== S U B	R O U T	I N E =======================================
_reload_delay_counter_2:
		TXA
		PHA
		LDX		#2
		LDA		_delays_list_lo,X
		STA		_delay_counter+1
		LDA		_delays_list_hi,X
		STA		_delay_counter
		PLA
		TAX
		RTS
byte_44651:
		.BYTE	$15,$1C,$24,$2C
byte_44655:
		.BYTE	  5,  8,  8,  8
byte_44659:
		.BYTE	$8F,$9F,$AF,$BF

; =============== S U B	R O U T	I N E =======================================
_event3_hiscore:
		LDX		#4
		LDY		#0
loc_44661:
		DEY
		BNE		loc_44661
		DEX
		BNE		loc_44661
		JSR		_call_j_far_apu_play
		JSR		_read_pad
		LDA		_pad
		CMP		byte_3B2
		BEQ		loc_44684
		STA		byte_3B2
		AND		#9
		BEQ		loc_44684
		LDA		byte_3B4
		CMP		#7
		BEQ		loc_44684
locret_44683:
		RTS
loc_44684:
		LDA		byte_3B4
		ASL
		TAX
		LDA		_lvl0_intro_log_lib,X
		STA		_text_data_tmp_ptr
		INX
		LDA		_lvl0_intro_log_lib,X
		STA		_text_data_tmp_ptr+1
		LDY		byte_3B5
		LDA		(_text_data_tmp_ptr),Y
		CMP		#$FF
		BEQ		loc_446CA
		CMP		#$FE
		BEQ		loc_446E6
		CMP		#$FD
		BNE		loc_446A8
		JMP		loc_4472B
loc_446A8:
		CMP		#$FC
		BEQ		loc_44710
		CMP		#$FB
		BEQ		_log_msg_buf_prepare
		CMP		#$FA
		BEQ		locret_44683
		LDX		byte_3D7
		LDA		(_text_data_tmp_ptr),Y
		STA		_log_msg_buf+$54,X
		INC		byte_3B5
		INC		byte_3D7
		LDA		#0
		STA		_generic_nmi_over_counter
loc_446C7:
		JMP		_event_exec
loc_446CA:
		INC		byte_3B4
		LDA		#0
		STA		byte_3D7
		STA		byte_3B5
		INC		_screen_idx
		LDA		_screen_idx
		JSR		_screen_display
		LDA		#$81
		STA		_PPU_CTRL
		JMP		loc_446C7
loc_446E6:
		LDA		_generic_nmi_over_counter
		AND		#$F
		BNE		loc_446C7
		JSR		_log_msg_buf_scroll_up
		INC		byte_3B5
		LDA		#0
		STA		byte_3D7
		JMP		loc_446C7
_log_msg_buf_prepare:
		LDX		#$6F
		LDA		#0
loc_446FF:
		STA		_log_msg_buf,X
		DEX
		BPL		loc_446FF
		INC		byte_3B5
		LDA		#0
		STA		byte_3D7
		JMP		loc_446C7
loc_44710:
		LDA		_delay_counter
		CLC
		ADC		#1
		STA		_delay_counter
		CMP		#$3F
		BCC		loc_446C7
		INC		byte_3B5
		LDA		#0
		STA		byte_3D7
		STA		_delay_counter
		JMP		loc_446C7
loc_4472B:
		LDA		_delay_counter
		CLC
		ADC		#1
		STA		_delay_counter
		CMP		#7
		BCC		loc_446C7
		INC		byte_3B5
		LDA		#0
		STA		byte_3D7
		STA		_delay_counter
		JMP		loc_446C7
_lvl0_intro_log_lib:
		.WORD _lvl0_intro_log0
		.WORD _lvl0_intro_log1
		.WORD _lvl0_intro_log2
_lvl0_intro_log0:
		.BYTE	  3, $F,  $D, $D,  1, $E,  4,$24,  3, $F,  $D,$FE,  $C, $F,  1,  4
		.BYTE	  0,  2,  9, $F,$13,$FE, $D,  5, $D, $F,$12,$19,  0,$13,  5,$14
		.BYTE	$FE,$13,$19,$13,$14,  5, $D,  0,$13,$14,  1,$14,$15,$13,$FE, $F
		.BYTE	 $B,$24,$FE,$FC,$FB,  3,$12,  9, $D,  5,  0,  9, $E,  0,$10,$12
		.BYTE	 $F,  7,$12,  5,$13,$13,$24,$FE,$10,  5, $E,  1, $C,  0,  3, $F
		.BYTE	  4,  5,  0,$1B,$1D,$23,$24,$1B,$FE,  9, $E,  0,$10,$15,$12,$13
		.BYTE	$15,  9,$14,$FE,$FC,$FB,$FF
_lvl0_intro_log1:
		.BYTE	$16, $F,  9,  3,  5,  0,  1, $E,  1, $C,$19,$13,  9,$13,$FE, $F
		.BYTE	  6,  6,  9,  3,  5,$12,  0, $C,  5,$17,  9,$13,$26,  1, $E, $E
		.BYTE	  5,$FE,$FC,$FE,  3,$15,$12,$12,  5, $E,$14,  0,$10, $F,$13,  9
		.BYTE	$14,  9, $F, $E,$24,$FE,  5,$13,$10, $F,$13,  9,$14, $F,  0,  1
		.BYTE	 $E,  4,  0,  3,  8,  1, $C, $D,  5,$12,$13,  0,  2, $C,$16,  4
		.BYTE	$24,$FE,$FC,$FB,$FF
_lvl0_intro_log2:
		.BYTE	  9,  0,  1, $D,  0, $F,  $E,  0,  $D,$19,  0,$17,  1,$19,  0, $C
		.BYTE	  5,$17,  9,$13,$24,$24,$24,$FE,$FC,$FA

; =============== S U B	R O U T	I N E =======================================
_event4_ending:
		LDA		#$7D
		STA		_spr_buf
		LDA		#$30
		STA		_spr_buf+1
		LDA		#$FF
		STA		_spr_buf+2
		LDA		#0
		STA		_spr_buf+3
		LDA		_delay_counter
		CLC
		ADC		#1
		STA		_delay_counter
		LDA		_delay_counter+1
		ADC		#0
		STA		_delay_counter+1
		LDA		_delay_counter+1
		CMP		#3
		BCC		loc_44858
		CMP		#$D
		BCS		loc_44855
		JMP		loc_44914
loc_44855:
		JMP		loc_448BE
loc_44858:
		JSR		_read_pad
		LDA		_pad
		AND		#1
		CMP		byte_3D3
		BEQ		loc_44871
		STA		byte_3D3
		CMP		#1
		BNE		loc_44871
		LDA		#3
		STA		_delay_counter+1
loc_44871:
		LDX		#3
		LDY		#0
loc_44875:
		DEY
		BNE		loc_44875
		DEX
		BNE		loc_44875
loc_4487B:
		BIT		_PPU_STATUS
		BMI		loc_448B8
		BVC		loc_4487B
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		LDA		#$1F
		STA		$BFFF
		LSR
		STA		$BFFF
		LSR
		STA		$BFFF
		LSR
		STA		$BFFF
		LSR
		STA		$BFFF
loc_448B8:
		JSR		_call_j_far_apu_play
		JMP		_event_exec
loc_448BE:
		LDA		#0
		STA		_PPU_CTRL
		STA		_PPU_MASK
		STA		_event_idx
		STA		_delay_counter
		STA		_delay_counter+1
		STA		_screen_idx
		LDA		#2
		STA		_event_reload_idx
		LDA		#$FF
		STA		byte_3B1
		LDA		#8
		STA		_ppu_scroll_x_override_redundant
		LDA		#2
		STA		_screen_idx
		JSR		_screen_display
		LDA		#1
		STA		byte_3D3
		LDX		#2
		LDA		_delays_list_lo+2
		STA		_delay_counter+1
		LDA		_delays_list_hi+2
		STA		_delay_counter
		LDA		#$80
		STA		_PPU_CTRL
		LDA		#$1D
		JSR		_j_apu_mus_start
		LDA		#1
		STA		byte_3B2
		STA		byte_3D3
		JMP		_event_exec
loc_44911:
		JMP		loc_448BE
loc_44914:
		LDA		byte_3D4
		BNE		loc_44965
		JSR		_hi_score_sort_test
		LDA		_hi_score_line_idx
		CMP		#6
		BCS		loc_44911
		ASL
		TAX
		LDA		_hi_score_ppu_ofs_list+1,X
		STA		byte_3D5
		LDA		_hi_score_ppu_ofs_list,X
		STA		byte_3D6
		LDA		#0
		STA		byte_3D7
		LDA		#3
		STA		_screen_idx
		JSR		_screen_display
		JSR		_hi_score_display
		LDX		#$27
loc_44943:
		JSR		_star_field_randomize
		AND		#$3F
		STA		_star_field_data0,X
		JSR		_star_field_randomize
		AND		#$3F
		STA		_star_field_data1,X
		LDA		#0
		STA		_star_field_data2,X
		DEX
		BPL		loc_44943
		LDA		#$FF
		STA		byte_3D4
		LDA		#$81
		STA		_PPU_CTRL
loc_44965:
		JSR		_star_field_animate
		JSR		_call_j_far_apu_play
		JSR		_read_pad
		LDX		_hi_score_line_idx
		LDA		_hi_score_ram_ofs_list,X
		CLC
		ADC		byte_3D7
		TAX
		LDA		_pad
		CMP		byte_3B2
		BEQ		loc_44987
		STA		byte_3B2
		JMP		loc_44992
loc_44987:
		LDA		_generic_nmi_over_counter
		AND		#$1F
		BEQ		loc_44992
		CMP		#$14
		BNE		loc_44A0F
loc_44992:
		LDA		_pad
		AND		#$20
		BEQ		loc_449B3
		LDA		_log_msg_buf,X
		CLC
		ADC		#1
		CMP		#$1B
		BCC		loc_449A5
		SBC		#$1B
loc_449A5:
		STA		_log_msg_buf,X
		LDA		#3
		STA		_delay_counter+1
		STA		_generic_nmi_over_counter
		JMP		loc_44A05
loc_449B3:
		LDA		_pad
		AND		#$10
		BEQ		loc_449D2
		LDA		_log_msg_buf,X
		SEC
		SBC		#1
		BCS		loc_449C4
		ADC		#$1B
loc_449C4:
		STA		_log_msg_buf,X
		LDA		#3
		STA		_delay_counter+1
		STA		_generic_nmi_over_counter
		JMP		loc_44A05
loc_449D2:
		LDA		_pad
		AND		#$80
		BEQ		loc_449EE
		LDA		byte_3D7
		CMP		#9
		BCS		loc_449EE
		INC		byte_3D7
		LDA		#3
		STA		_delay_counter+1
		STA		_generic_nmi_over_counter
		JMP		loc_44A05
loc_449EE:
		LDA		_pad
		AND		#$40
		BEQ		loc_44A05
		LDA		byte_3D7
		BEQ		loc_44A05
		DEC		byte_3D7
		LDA		#3
		STA		_delay_counter+1
		STA		_generic_nmi_over_counter
loc_44A05:
		LDA		_pad
		AND		#9
		BEQ		loc_44A0F
		JMP		loc_448BE
loc_44A0F:
		JMP		_event_exec
_hi_score_ram_ofs_list:
		.BYTE	$75,$84,$93,$A2,$B1,$C0

; =============== S U B	R O U T	I N E =======================================
_event5_repair:
		LDX		#1
		LDY		#0
loc_44A1C:
		DEY
		BNE		loc_44A1C
		DEX
		BNE		loc_44A1C
		LDA		#$66
		STA		_spr_buf
		LDA		#$30
		STA		_spr_buf+1
		LDA		#$FF
		STA		_spr_buf+2
		LDA		#$30
		STA		_spr_buf+3
		LDA		_tmp3C3
		STA		_spr_buf+4
		CLC
		ADC		#8
		STA		_spr_buf+8
		LDA		byte_3B8
		STA		_spr_buf+7
		STA		_spr_buf+$B
		LDA		#$F6
		STA		_spr_buf+5
		LDA		#$F7
		STA		_spr_buf+9
		LDA		#0
		STA		_spr_buf+6
		STA		_spr_buf+$A
		LDA		#0
		STA		_tmp_score_hex_value
		STA		_tmp_score_hex_value+1
		LDX		_repair_body_index_request
		LDY		_repair_update_flag_list,X
		LDA		_cur_head_percent,Y
		STA		_tmp_score_hex_value
		LDA		#$E6
		STA		_dec_score_chr_ofs_delta
		LDX		#<[byte_3C8]
		LDY		#>[byte_3C8]
		JSR		_hex2str_hiscore
		LDA		#$9A
		STA		_spr_buf+$E0
		LDA		#$F8
		STA		_spr_buf+$E1
		LDA		#0
		STA		_spr_buf+$E2
		LDA		#$17
		STA		_spr_buf+$E3
		STA		_spr_buf+$E7
		LDA		#$A2
		STA		_spr_buf+$E4
		LDA		#$F9
		STA		_spr_buf+$E5
		LDA		#0
		STA		_spr_buf+$E6
		JSR		_read_pad
		LDA		_pad
		AND		#8
		BEQ		loc_44AB8
		LDA		#0
		STA		_event_reload_idx
		LDA		#7
		JMP		_asci_bios_msg_display
		RTS
loc_44AB8:
		LDA		_cur_repair_tanks
		CMP		#$A
		BCC		loc_44AC1
		LDA		#$A
loc_44AC1:
		CLC
		ADC		#$E6
		STA		byte_3CE
		LDA		_pad
		AND		#1
		CMP		byte_3B2
		BEQ		loc_44AF1
		STA		byte_3B2
		CMP		#1
		BNE		loc_44AF1
		LDA		_cur_repair_tanks
		BEQ		loc_44AF1
		LDX		_repair_body_index_request
		LDY		_repair_update_flag_list,X
		LDA		_cur_head_percent,Y
		CLC
		ADC		#$A
		BCS		loc_44AF1
		STA		_cur_head_percent,Y
		DEC		_cur_repair_tanks
loc_44AF1:
		BIT		_PPU_STATUS
		BMI		loc_44B2B
		BVC		loc_44AF1
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		LDA		#$17
		STA		$BFFF
		LSR
		STA		$BFFF
		LSR
		STA		$BFFF
		LSR
		STA		$BFFF
		LSR
		STA		$BFFF
loc_44B2B:
		JSR		_repair_cursor_input
		JSR		_repair_cursor_animate
		JSR		_dpad_convert
		JSR		_call_j_far_apu_play
		JMP		_event_exec

; =============== S U B	R O U T	I N E =======================================
_event6_game_over:
		LDX		#3
		LDY		#0
loc_44B3E:
		DEY
		BNE		loc_44B3E
		DEX
		BNE		loc_44B3E
		JSR		_call_j_far_apu_play
		JSR		_star_field_animate
		JSR		_read_pad
		LDA		_pad
		BEQ		loc_44B5A
		LDA		byte_3B2
		BEQ		loc_44B62
		JMP		loc_44B6F
loc_44B5A:
		LDA		#0
		STA		byte_3B2
		JMP		loc_44B6F
loc_44B62:
		LDA		#2
		STA		_delay_counter+1
		LDA		#$30
		STA		_delay_counter
		JMP		loc_44B80
loc_44B6F:
		LDA		_delay_counter
		CLC
		ADC		#1
		STA		_delay_counter
		LDA		_delay_counter+1
		ADC		#0
		STA		_delay_counter+1
loc_44B80:
		LDA		_delay_counter+1
		CMP		#2
		BNE		loc_44BA3
		LDA		_delay_counter
		CMP		#$30
		BCC		loc_44BA3
		LDA		#4
		STA		_delay_counter+1
		STA		_event_reload_idx
		LDA		#0
		STA		byte_3D4
		LDA		#3
		STA		_screen_idx
		JMP		loc_44914
loc_44BA3:
		JMP		_event_exec

; =============== S U B	R O U T	I N E =======================================
_event7_asci_msg_print:
		LDX		#3
		LDY		#0
loc_44BAA:
		DEY
		BNE		loc_44BAA
		DEX
		BNE		loc_44BAA
		LDA		_asci_msg_skip_forbid
		BNE		loc_44BCA
		JSR		_read_pad
		LDA		_pad
		AND		#1
		CMP		byte_3D3
		BEQ		loc_44BCA
		STA		byte_3D3
		CMP		#1
		BNE		loc_44BCA
		RTS
loc_44BCA:
		JSR		_call_j_far_apu_play
		LDY		byte_3B5
		LDA		(_text_data_tmp_ptr),Y
		CMP		#$FD
		BNE		loc_44BE1
		LDA		#$FF
		STA		_asci_msg_skip_forbid
		INC		byte_3B5
		JMP		_j_even_exec
loc_44BE1:
		CMP		#$F9
		BNE		loc_44BE8
		JMP		_asci_msg_repair_menu_select_input
loc_44BE8:
		CMP		#$FE
		BEQ		_asci_msg_scroll
		CMP		#$FC
		BEQ		_asci_msg_delay
		CMP		#$FB
		BEQ		loc_44BF9
		CMP		#$FA
		BNE		loc_44BFC
		RTS
loc_44BF9:
		JMP		_asci_msg_buf_clear
loc_44BFC:
		LDX		byte_3D7
		LDA		(_text_data_tmp_ptr),Y
		SEC
		SBC		#$40
		STA		_log_msg_buf+$54,X
		INC		byte_3B5
		INC		byte_3D7
		LDA		#0
		STA		_generic_nmi_over_counter
_j_even_exec:
		JMP		_event_exec

; =============== S U B	R O U T	I N E =======================================
_asci_msg_next_screen_cmd_unused:
		INC		byte_3B4
		LDA		#0
		STA		byte_3D7
		STA		byte_3B5
		INC		_screen_idx
		LDA		_screen_idx
		JSR		_screen_display
		LDA		#$81
		STA		_PPU_CTRL
		JMP		_j_even_exec

; =============== S U B	R O U T	I N E =======================================
_asci_msg_scroll:
		LDA		_generic_nmi_over_counter
		AND		#$F
		BNE		_j_even_exec
		JSR		_log_msg_buf_scroll_up
		INC		byte_3B5
		LDA		#0
		STA		byte_3D7
		JMP		_j_even_exec

; =============== S U B	R O U T	I N E =======================================
_asci_msg_delay:
		LDA		_delay_counter
		CLC
		ADC		#1
		STA		_delay_counter
		CMP		#$3F
		BCC		_j_even_exec
		INC		byte_3B5
		LDA		#0
		STA		byte_3D7
		STA		_delay_counter
		JMP		_j_even_exec

; =============== S U B	R O U T	I N E =======================================
_asci_msg_buf_clear:
		LDX		#$6F
		LDA		#0
loc_44C65:
		STA		_log_msg_buf,X
		DEX
		BPL		loc_44C65
		INC		byte_3B5
		LDA		#0
		STA		byte_3D7
		JMP		_j_even_exec

; =============== S U B	R O U T	I N E =======================================
_asci_msg_repair_menu_select_input:
		JSR		_read_pad
		LDA		_pad
		AND		#1
		BEQ		loc_44CA9
		LDX		#0
		STX		_new_game_start_flag
		LDA		_cur_head_percent
		STA		_pl_bckp_head_percent,X
		LDA		_cur_body_percent
		STA		_pl_bckp_body_percent,X
		LDA		_cur_arms_percent
		STA		_pl_bckp_arms_percent,X
		LDA		_cur_legs_percent
		STA		_pl_bckp_legs_percent,X
		LDA		_cur_repair_tanks
		STA		_pl_bckp_repair_tanks,X
		INC		byte_3B5
		JMP		_j_even_exec
loc_44CA9:
		LDA		_pad
		AND		#2
		BNE		loc_44CB3
		JMP		_j_even_exec
loc_44CB3:
		LDA		_cur_head_percent_backup
		STA		_cur_head_percent
		LDA		_cur_body_percent_backup
		STA		_cur_body_percent
		LDA		_cur_arms_percent_backup
		STA		_cur_arms_percent
		LDA		_cur_legs_percent_backup
		STA		_cur_legs_percent
		LDA		_cur_repair_tanks_backup
		STA		_cur_repair_tanks
		JMP		_sub1_repair_lab

; =============== S U B	R O U T	I N E =======================================
_nmi0:
		LDA		#0
		STA		_PPU_MASK
		STA		_PPU_OAM_ADDR
		BIT		_PPU_STATUS
		STA		_PPU_SCROLL
		STA		_PPU_SCROLL
		LDA		#2
		STA		_PPU_OAM_DMA
		JSR		_ppu_pal_send
		LDA		_chr0_bank_shadow
		STA		$BFFF
		LSR
		STA		$BFFF
		LSR
		STA		$BFFF
		LSR
		STA		$BFFF
		LSR
		STA		$BFFF
		LDA		_chr1_bank_shadow
		STA		$DFFF
		LSR
		STA		$DFFF
		LSR
		STA		$DFFF
		LSR
		STA		$DFFF
		LSR
		STA		$DFFF
		LDA		_event_reload_idx
		CMP		#3
		BEQ		_log_msg_dequeue
		CMP		#7
		BEQ		_log_msg_dequeue
		JMP		_no_log_screen
_log_msg_dequeue:
		BIT		_PPU_STATUS
		LDA		#$22
		STA		_PPU_ADDR
		LDA		#$43
		STA		_PPU_ADDR
		LDA		_log_msg_buf
		STA		_PPU_DATA
		LDA		_log_msg_buf+1
		STA		_PPU_DATA
		LDA		_log_msg_buf+2
		STA		_PPU_DATA
		LDA		_log_msg_buf+3
		STA		_PPU_DATA
		LDA		_log_msg_buf+4
		STA		_PPU_DATA
		LDA		_log_msg_buf+5
		STA		_PPU_DATA
		LDA		_log_msg_buf+6
		STA		_PPU_DATA
		LDA		_log_msg_buf+7
		STA		_PPU_DATA
		LDA		_log_msg_buf+8
		STA		_PPU_DATA
		LDA		_log_msg_buf+9
		STA		_PPU_DATA
		LDA		_log_msg_buf+$A
		STA		_PPU_DATA
		LDA		_log_msg_buf+$B
		STA		_PPU_DATA
		LDA		_log_msg_buf+$C
		STA		_PPU_DATA
		LDA		_log_msg_buf+$D
		STA		_PPU_DATA
		LDA		_log_msg_buf+$E
		STA		_PPU_DATA
		LDA		_log_msg_buf+$F
		STA		_PPU_DATA
		LDA		_log_msg_buf+$10
		STA		_PPU_DATA
		LDA		_log_msg_buf+$11
		STA		_PPU_DATA
		LDA		_log_msg_buf+$12
		STA		_PPU_DATA
		LDA		_log_msg_buf+$13
		STA		_PPU_DATA
		LDA		_log_msg_buf+$14
		STA		_PPU_DATA
		LDA		_log_msg_buf+$15
		STA		_PPU_DATA
		LDA		_log_msg_buf+$16
		STA		_PPU_DATA
		LDA		_log_msg_buf+$17
		STA		_PPU_DATA
		LDA		_log_msg_buf+$18
		STA		_PPU_DATA
		LDA		_log_msg_buf+$19
		STA		_PPU_DATA
		LDA		_log_msg_buf+$1A
		STA		_PPU_DATA
		LDA		#$22
		STA		_PPU_ADDR
		LDA		#$63
		STA		_PPU_ADDR
		LDA		_log_msg_buf+$1C
		STA		_PPU_DATA
		LDA		_log_msg_buf+$1D
		STA		_PPU_DATA
		LDA		_log_msg_buf+$1E
		STA		_PPU_DATA
		LDA		_log_msg_buf+$1F
		STA		_PPU_DATA
		LDA		_log_msg_buf+$20
		STA		_PPU_DATA
		LDA		_log_msg_buf+$21
		STA		_PPU_DATA
		LDA		_log_msg_buf+$22
		STA		_PPU_DATA
		LDA		_log_msg_buf+$23
		STA		_PPU_DATA
		LDA		_log_msg_buf+$24
		STA		_PPU_DATA
		LDA		_log_msg_buf+$25
		STA		_PPU_DATA
		LDA		_log_msg_buf+$26
		STA		_PPU_DATA
		LDA		_log_msg_buf+$27
		STA		_PPU_DATA
		LDA		_log_msg_buf+$28
		STA		_PPU_DATA
		LDA		_log_msg_buf+$29
		STA		_PPU_DATA
		LDA		_log_msg_buf+$2A
		STA		_PPU_DATA
		LDA		_log_msg_buf+$2B
		STA		_PPU_DATA
		LDA		_log_msg_buf+$2C
		STA		_PPU_DATA
		LDA		_log_msg_buf+$2D
		STA		_PPU_DATA
		LDA		_log_msg_buf+$2E
		STA		_PPU_DATA
		LDA		_log_msg_buf+$2F
		STA		_PPU_DATA
		LDA		_log_msg_buf+$30
		STA		_PPU_DATA
		LDA		_log_msg_buf+$31
		STA		_PPU_DATA
		LDA		_log_msg_buf+$32
		STA		_PPU_DATA
		LDA		_log_msg_buf+$33
		STA		_PPU_DATA
		LDA		_log_msg_buf+$34
		STA		_PPU_DATA
		LDA		_log_msg_buf+$35
		STA		_PPU_DATA
		LDA		_log_msg_buf+$36
		STA		_PPU_DATA
		LDA		#$22
		STA		_PPU_ADDR
		LDA		#$83
		STA		_PPU_ADDR
		LDA		_log_msg_buf+$38
		STA		_PPU_DATA
		LDA		_log_msg_buf+$39
		STA		_PPU_DATA
		LDA		_log_msg_buf+$3A
		STA		_PPU_DATA
		LDA		_log_msg_buf+$3B
		STA		_PPU_DATA
		LDA		_log_msg_buf+$3C
		STA		_PPU_DATA
		LDA		_log_msg_buf+$3D
		STA		_PPU_DATA
		LDA		_log_msg_buf+$3E
		STA		_PPU_DATA
		LDA		_log_msg_buf+$3F
		STA		_PPU_DATA
		LDA		_log_msg_buf+$40
		STA		_PPU_DATA
		LDA		_log_msg_buf+$41
		STA		_PPU_DATA
		LDA		_log_msg_buf+$42
		STA		_PPU_DATA
		LDA		_log_msg_buf+$43
		STA		_PPU_DATA
		LDA		_log_msg_buf+$44
		STA		_PPU_DATA
		LDA		_log_msg_buf+$45
		STA		_PPU_DATA
		LDA		_log_msg_buf+$46
		STA		_PPU_DATA
		LDA		_log_msg_buf+$47
		STA		_PPU_DATA
		LDA		_log_msg_buf+$48
		STA		_PPU_DATA
		LDA		_log_msg_buf+$49
		STA		_PPU_DATA
		LDA		_log_msg_buf+$4A
		STA		_PPU_DATA
		LDA		_log_msg_buf+$4B
		STA		_PPU_DATA
		LDA		_log_msg_buf+$4C
		STA		_PPU_DATA
		LDA		_log_msg_buf+$4D
		STA		_PPU_DATA
		LDA		_log_msg_buf+$4E
		STA		_PPU_DATA
		LDA		_log_msg_buf+$4F
		STA		_PPU_DATA
		LDA		_log_msg_buf+$50
		STA		_PPU_DATA
		LDA		_log_msg_buf+$51
		STA		_PPU_DATA
		LDA		_log_msg_buf+$52
		STA		_PPU_DATA
		LDA		#$22
		STA		_PPU_ADDR
		LDA		#$A3
		STA		_PPU_ADDR
		LDX		#0
loc_44F3A:
		LDA		_log_msg_buf+$54,X
		STA		_PPU_DATA
		INX
		CPX		#$1B
		BCC		loc_44F3A
_no_log_screen:
		LDA		_event_reload_idx
		CMP		#5
		BEQ		_repair_mode_dequeue
		JMP		_nmi0_common
_repair_mode_dequeue:
		BIT		_PPU_STATUS
		LDA		#$22
		STA		_PPU_ADDR
		LDA		#5
		STA		_PPU_ADDR
		LDA		_repair_body_index_request
		ASL
		TAX
		LDA		off_45404,X
		STA		_obj_flags._player
		INX
		LDA		off_45404,X
		STA		_obj_flags._enemy
		LDY		#0
loc_44F6E:
		LDA		(_obj_flags),Y
		STA		_PPU_DATA
		INY
		CPY		#4
		BCC		loc_44F6E
		LDA		#$22
		STA		_PPU_ADDR
		LDA		#$25
		STA		_PPU_ADDR
		LDA		byte_3CA
		STA		_PPU_DATA
		LDA		byte_3CB
		STA		_PPU_DATA
		LDA		byte_3CC
		STA		_PPU_DATA
		LDA		#$F0
		STA		_PPU_DATA
		LDA		#$22
		STA		_PPU_ADDR
		LDA		#$24
		STA		_PPU_ADDR
		LDA		_PPU_DATA
		LDA		_PPU_DATA
		LDX		#$22
		STX		_PPU_ADDR
		LDX		#$44
		STX		_PPU_ADDR
		STA		_PPU_DATA
		LDA		byte_3CE
		STA		_PPU_DATA
		LDA		_generic_nmi_over_counter
		AND		#1
		BNE		loc_44FC6
		JMP		loc_4505F
loc_44FC6:
		LDX		_repair_body_index_request
		LDY		byte_476DC,X
		BIT		_PPU_STATUS
		LDA		#$22
		STA		_PPU_ADDR
		LDA		#$80
		STA		_PPU_ADDR
		LDX		#0
loc_44FDB:
		LDA		_repair_tlm0,Y
		STA		_PPU_DATA
		INY
		INX
		CPX		#8
		BCC		loc_44FDB
		LDA		#$22
		STA		_PPU_ADDR
		LDA		#$A0
		STA		_PPU_ADDR
		LDX		#0
loc_44FF3:
		LDA		_repair_tlm0,Y
		STA		_PPU_DATA
		INY
		INX
		CPX		#8
		BCC		loc_44FF3
		LDA		#$22
		STA		_PPU_ADDR
		LDA		#$C0
		STA		_PPU_ADDR
		LDX		#0
loc_4500B:
		LDA		_repair_tlm0,Y
		STA		_PPU_DATA
		INY
		INX
		CPX		#8
		BCC		loc_4500B
		LDA		#$22
		STA		_PPU_ADDR
		LDA		#$E0
		STA		_PPU_ADDR
		LDX		#0
loc_45023:
		LDA		_repair_tlm0,Y
		STA		_PPU_DATA
		INY
		INX
		CPX		#8
		BCC		loc_45023
		LDA		#$23
		STA		_PPU_ADDR
		LDA		#0
		STA		_PPU_ADDR
		LDX		#0
loc_4503B:
		LDA		_repair_tlm0,Y
		STA		_PPU_DATA
		INY
		INX
		CPX		#8
		BCC		loc_4503B
		LDA		#$23
		STA		_PPU_ADDR
		LDA		#$20
		STA		_PPU_ADDR
		LDX		#0
loc_45053:
		LDA		_repair_tlm0,Y
		STA		_PPU_DATA
		INY
		INX
		CPX		#8
		BCC		loc_45053
loc_4505F:
		LDA		_generic_nmi_over_counter
		AND		#1
		BEQ		loc_45069
		JMP		_nmi0_common
loc_45069:
		LDX		_repair_body_index_request
		LDA		_repair_update_flag_list,X
		CMP		#0
		BEQ		loc_45081
		CMP		#1
		BEQ		loc_45087
		CMP		#2
		BEQ		loc_4508D
		JSR		_repair_ppu_legs_upgrade_display
		JMP		_nmi0_common
loc_45081:
		JSR		_repair_ppu_head_upgrade_display
		JMP		_nmi0_common
loc_45087:
		JSR		_repair_ppu_body_upgrade_display
		JMP		_nmi0_common
loc_4508D:
		JSR		_repair_ppu_arms_upgrade_display
		JMP		_nmi0_common

; =============== S U B	R O U T	I N E =======================================
_repair_ppu_head_upgrade_display:
		LDA		_cur_head_percent
		CMP		#$B
		BCC		loc_450A2
		CMP		#$15
		BCC		loc_450A6
		LDY		#0
		BEQ		loc_450A8
loc_450A2:
		LDY		#$20
		BNE		loc_450A8
loc_450A6:
		LDY		#$10
loc_450A8:
		BIT		_PPU_STATUS
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$12
		STA		_PPU_ADDR
		LDX		#4
loc_450B7:
		LDA		_tlm_repair_head_lib,Y
		STA		_PPU_DATA
		INY
		DEX
		BNE		loc_450B7
		BIT		_PPU_STATUS
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$32
		STA		_PPU_ADDR
		LDX		#4
loc_450D0:
		LDA		_tlm_repair_head_lib,Y
		STA		_PPU_DATA
		INY
		DEX
		BNE		loc_450D0
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$52
		STA		_PPU_ADDR
		LDX		#4
loc_450E6:
		LDA		_tlm_repair_head_lib,Y
		STA		_PPU_DATA
		INY
		DEX
		BNE		loc_450E6
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$72
		STA		_PPU_ADDR
		LDX		#4
loc_450FC:
		LDA		_tlm_repair_head_lib,Y
		STA		_PPU_DATA
		INY
		DEX
		BNE		loc_450FC
		RTS

; =============== S U B	R O U T	I N E =======================================
_repair_ppu_arms_upgrade_display:
		LDA		_cur_arms_percent
		CMP		#$B
		BCC		loc_45116
		CMP		#$15
		BCC		loc_4511A
		LDY		#0
		BEQ		loc_4511C
loc_45116:
		LDY		#$24
		BNE		loc_4511C
loc_4511A:
		LDY		#$12
loc_4511C:
		BIT		_PPU_STATUS
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$B0
		STA		_PPU_ADDR
		LDX		#3
loc_4512B:
		LDA		_tlm_repair_larm_lib0,Y
		STA		_PPU_DATA
		INY
		DEX
		BNE		loc_4512B
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$D0
		STA		_PPU_ADDR
		LDX		#3
loc_45141:
		LDA		_tlm_repair_larm_lib0,Y
		STA		_PPU_DATA
		INY
		DEX
		BNE		loc_45141
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$F0
		STA		_PPU_ADDR
		LDX		#3
loc_45157:
		LDA		_tlm_repair_larm_lib0,Y
		STA		_PPU_DATA
		INY
		DEX
		BNE		loc_45157
		LDA		#$21
		STA		_PPU_ADDR
		LDA		#$10
		STA		_PPU_ADDR
		LDX		#3
loc_4516D:
		LDA		_tlm_repair_larm_lib0,Y
		STA		_PPU_DATA
		INY
		DEX
		BNE		loc_4516D
		LDA		#$21
		STA		_PPU_ADDR
		LDA		#$30
		STA		_PPU_ADDR
		LDX		#3
loc_45183:
		LDA		_tlm_repair_larm_lib0,Y
		STA		_PPU_DATA
		INY
		DEX
		BNE		loc_45183
		LDA		#$21
		STA		_PPU_ADDR
		LDA		#$50
		STA		_PPU_ADDR
		LDX		#3
loc_45199:
		LDA		_tlm_repair_larm_lib0,Y
		STA		_PPU_DATA
		INY
		DEX
		BNE		loc_45199
		LDA		_cur_arms_percent
		CMP		#$B
		BCC		loc_451B6
		CMP		#$15
		BCC		loc_451B2
		LDY		#0
		BEQ		loc_451B8
loc_451B2:
		LDY		#6
		BNE		loc_451B8
loc_451B6:
		LDY		#$C
loc_451B8:
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$77
		STA		_PPU_ADDR
		LDA		_tlm_repair_larm_lib1,Y
		STA		_PPU_DATA
		INY
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$97
		STA		_PPU_ADDR
		LDA		_tlm_repair_larm_lib1,Y
		STA		_PPU_DATA
		INY
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$B7
		STA		_PPU_ADDR
		LDA		_tlm_repair_larm_lib1,Y
		STA		_PPU_DATA
		INY
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$D7
		STA		_PPU_ADDR
		LDA		_tlm_repair_larm_lib1,Y
		STA		_PPU_DATA
		INY
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$F7
		STA		_PPU_ADDR
		LDA		_tlm_repair_larm_lib1,Y
		STA		_PPU_DATA
		INY
		LDA		#$21
		STA		_PPU_ADDR
		LDA		#$17
		STA		_PPU_ADDR
		LDA		_tlm_repair_larm_lib1,Y
		STA		_PPU_DATA
		RTS

; =============== S U B	R O U T	I N E =======================================
_repair_ppu_body_upgrade_display:
		LDA		_cur_body_percent
		CMP		#$B
		BCC		loc_45231
		CMP		#$15
		BCC		loc_4522D
		LDY		#0
		BEQ		loc_45233
loc_4522D:
		LDY		#$10
		BNE		loc_45233
loc_45231:
		LDY		#$20
loc_45233:
		BIT		_PPU_STATUS
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$B3
		STA		_PPU_ADDR
		LDX		#4
loc_45242:
		LDA		_tlm_repair_body_lib,Y
		STA		_PPU_DATA
		INY
		DEX
		BNE		loc_45242
		BIT		_PPU_STATUS
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$D3
		STA		_PPU_ADDR
		LDX		#4
loc_4525B:
		LDA		_tlm_repair_body_lib,Y
		STA		_PPU_DATA
		INY
		DEX
		BNE		loc_4525B
		BIT		_PPU_STATUS
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$F3
		STA		_PPU_ADDR
		LDX		#4
loc_45274:
		LDA		_tlm_repair_body_lib,Y
		STA		_PPU_DATA
		INY
		DEX
		BNE		loc_45274
		BIT		_PPU_STATUS
		LDA		#$21
		STA		_PPU_ADDR
		LDA		#$13
		STA		_PPU_ADDR
		LDX		#4
loc_4528D:
		LDA		_tlm_repair_body_lib,Y
		STA		_PPU_DATA
		INY
		DEX
		BNE		loc_4528D
		RTS

; =============== S U B	R O U T	I N E =======================================
_repair_ppu_legs_upgrade_display:
		LDA		_cur_legs_percent
		CMP		#$B
		BCC		loc_452AB
		CMP		#$15
		BCC		loc_452A7
		LDY		#0
		BEQ		loc_452AD
loc_452A7:
		LDY		#$30
		BNE		loc_452AD
loc_452AB:
		LDY		#$60
loc_452AD:
		BIT		_PPU_STATUS
		LDA		#$21
		STA		_PPU_ADDR
		LDA		#$72
		STA		_PPU_ADDR
		LDX		#8
loc_452BC:
		LDA		_tlm_repair_legs_lib,Y
		STA		_PPU_DATA
		INY
		DEX
		BNE		loc_452BC
		LDA		#$21
		STA		_PPU_ADDR
		LDA		#$92
		STA		_PPU_ADDR
		LDX		#8
loc_452D2:
		LDA		_tlm_repair_legs_lib,Y
		STA		_PPU_DATA
		INY
		DEX
		BNE		loc_452D2
		LDA		#$21
		STA		_PPU_ADDR
		LDA		#$B2
		STA		_PPU_ADDR
		LDX		#8
loc_452E8:
		LDA		_tlm_repair_legs_lib,Y
		STA		_PPU_DATA
		INY
		DEX
		BNE		loc_452E8
		LDA		#$21
		STA		_PPU_ADDR
		LDA		#$D2
		STA		_PPU_ADDR
		LDX		#8
loc_452FE:
		LDA		_tlm_repair_legs_lib,Y
		STA		_PPU_DATA
		INY
		DEX
		BNE		loc_452FE
		LDA		#$21
		STA		_PPU_ADDR
		LDA		#$F2
		STA		_PPU_ADDR
		LDX		#8
loc_45314:
		LDA		_tlm_repair_legs_lib,Y
		STA		_PPU_DATA
		INY
		DEX
		BNE		loc_45314
		RTS
_unused_0:
		LDA		#$22
		STA		_PPU_ADDR
		LDA		#$12
		STA		_PPU_ADDR
		LDX		#8
loc_4532B:
		LDA		_tlm_repair_legs_lib,Y
		STA		_PPU_DATA
		INY
		DEX
		BNE		loc_4532B

; =============== S U B	R O U T	I N E =======================================
_nmi0_common:
		JSR		_nmi0_scroll_reload
		LDA		_event_reload_idx
		STA		_event_idx
		LDA		#$1E
		STA		_PPU_MASK
		LDA		_disp_extra_ctrl
		BEQ		loc_4535A
		LDA		#$A8
		ORA		_nmi_ppu_ctrl_override_redundant
		STA		_PPU_CTRL
		INC		_generic_nmi_over_counter
		LDA		_irq_regs_backup
		LDX		_irq_regs_backup+1
		LDY		_irq_regs_backup+2
		RTI
loc_4535A:
		LDA		_event_reload_idx
		CMP		#4
		BNE		loc_45368
		LDA		_screen_idx
		CMP		#3
		BEQ		loc_4536B
loc_45368:
		JMP		loc_453F2
loc_4536B:
		LDA		#0
		CLC
		LDX		_hi_score_line_idx
loc_45371:
		CPX		#0
		BEQ		loc_4537B
		ADC		#$F
		DEX
		JMP		loc_45371
loc_4537B:
		TAY
		BIT		_PPU_STATUS
		LDA		_hi_score_line_idx
		ASL
		TAX
		LDA		_hi_score_ppu_ofs_list+1,X
		STA		_PPU_ADDR
		STA		_tmpE6
		LDA		_hi_score_ppu_ofs_list,X
		STA		_PPU_ADDR
		STA		_tmpE7
		LDX		_hi_score_line_idx
		LDY		_hi_score_ram_ofs_list,X
		LDX		#0
loc_4539C:
		LDA		_log_msg_buf,Y
		STA		_PPU_DATA
		INY
		INX
		CPX		#$A
		BCC		loc_4539C
		LDA		_tmpE7
		CLC
		ADC		#$1F
		STA		_tmpE7
		LDA		_tmpE6
		ADC		#0
		STA		_tmpE6
		BIT		_PPU_STATUS
		LDA		_tmpE6
		STA		_PPU_ADDR
		LDA		_tmpE7
		CLC
		ADC		byte_3D7
		STA		_PPU_ADDR
		LDA		_generic_nmi_over_counter
		AND		#$1F
		CMP		#$13
		BCS		loc_453E0
		LDA		#0
		STA		_PPU_DATA
		LDA		#$DE
		STA		_PPU_DATA
		LDA		#0
		STA		_PPU_DATA
		BNE		loc_453E8
loc_453E0:
		LDA		#0
		STA		_PPU_DATA
		STA		_PPU_DATA
loc_453E8:
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#0
		STA		_PPU_ADDR
loc_453F2:
		LDA		#$88
		ORA		_nmi_ppu_ctrl_override_redundant
		STA		_PPU_CTRL
		INC		_generic_nmi_over_counter
		LDA		_irq_regs_backup
		LDX		_irq_regs_backup+1
		LDY		_irq_regs_backup+2
		RTI
off_45404:
		.WORD byte_477A1
		.WORD byte_477A5
		.WORD byte_477AD
		.WORD byte_477A5
		.WORD byte_477A9
_hi_score_ppu_ofs_list:
		.WORD $2151
		.WORD $2191
		.WORD $21D1
		.WORD $2211
		.WORD $2251
		.WORD $2291

; =============== S U B	R O U T	I N E =======================================
_ppu_pal_send:
		BIT		_PPU_STATUS
		LDA		#$3F
		STA		_PPU_ADDR
		LDX		#0
		STX		_PPU_ADDR
loc_45427:
		LDA		_pal_buf,X
		STA		_PPU_DATA
		LDA		_pal_buf+1,X
		STA		_PPU_DATA
		INX
		INX
		CPX		#$20
		BNE		loc_45427
		RTS

; =============== S U B	R O U T	I N E =======================================
_nmi0_scroll_reload:
		BIT		_PPU_STATUS
		LDA		_ppu_scroll_x_override_redundant
		LDA		#0
		STA		_PPU_SCROLL
		LDA		_disp_extra_shift
		BEQ		loc_4544C
		LDA		#$E0
loc_4544C:
		STA		_PPU_SCROLL
		INC		_nmi0_scroll_reload_counter_redundant
		RTS

; =============== S U B	R O U T	I N E =======================================
_dpad_convert:
		LDA		_pad
		AND		#$F0
		LSR
		LSR
		LSR
		TAX
		LDA		off_4546B,X
		STA		_tmp_buf0
		LDA		off_4546B+1,X
		STA		_tmp_buf0+1
		JMP		(_tmp_buf0)
off_4546B:
		.WORD _dpad_00
		.WORD _dpad_01
		.WORD _dpad_02
		.WORD _dpad_00
		.WORD _dpad_04
		.WORD _dpad_05
		.WORD _dpad_06
		.WORD _dpad_00
		.WORD _dpad_08
		.WORD _dpad_09
		.WORD _dpad_0A
		.WORD _dpad_00
		.WORD _dpad_00
		.WORD _dpad_00
		.WORD _dpad_00
		.WORD _dpad_00

; =============== S U B	R O U T	I N E =======================================
_dpad_00:
		LDA		#0
		STA		_dpad
		RTS

; =============== S U B	R O U T	I N E =======================================
_dpad_01:
		LDA		#1
		STA		_dpad
		RTS

; =============== S U B	R O U T	I N E =======================================
_dpad_02:
		LDA		#3
		STA		_dpad
		RTS

; =============== S U B	R O U T	I N E =======================================
_dpad_04:
		LDA		#4
		STA		_dpad
		RTS

; =============== S U B	R O U T	I N E =======================================
_dpad_08:
		LDA		#2
		STA		_dpad
		RTS

; =============== S U B	R O U T	I N E =======================================
_dpad_05:
		LDA		#0
		STA		_dpad
		RTS

; =============== S U B	R O U T	I N E =======================================
_dpad_06:
		LDA		#0
		STA		_dpad
		RTS

; =============== S U B	R O U T	I N E =======================================
_dpad_09:
		LDA		#0
		STA		_dpad
		RTS

; =============== S U B	R O U T	I N E =======================================
_dpad_0A:
		LDA		#0
		STA		_dpad
		RTS

; =============== S U B	R O U T	I N E =======================================
_read_pad:
		LDX		#1
		STX		_PAD0_REG
		DEX
		STX		_PAD0_REG
		LDY		#8
loc_454CC:
		LDA		_PAD0_REG
		LSR
		ROR		_pad
		DEY
		BNE		loc_454CC
		RTS

; =============== S U B	R O U T	I N E =======================================
_star_field_animate:
		LDX		#$14
_star_field_loop:
		LDY		_star_field_data0,X
		LDA		_star_field_data1,X
		STA		_mul_argB
		LDA		_star_field_params0,Y
		STA		_star_filed_sign_tmp
		AND		#$7F
		STA		_mul_argA
		JSR		_A_mul_B_mul_2
		LDY		_star_field_params1,X
		BIT		_star_filed_sign_tmp
		BMI		loc_45502
		ADC		_star_field_center_pos_x
		CMP		#$E9
		BCS		loc_4550B
		JMP		loc_4550E
loc_45502:
		SBC		_star_field_center_pos_x
		EOR		#$FF
		CMP		#$10
		BCS		loc_4550E
loc_4550B:
		JMP		loc_4556B
loc_4550E:
		STA		_spr_buf+3,Y
		LDY		_star_field_data0,X
		LDA		_star_field_params2,Y
		STA		_star_filed_sign_tmp
		AND		#$7F
		STA		_mul_argA
		JSR		_A_mul_B_mul_2
		LDY		_star_field_params1,X
		BIT		_star_filed_sign_tmp
		BMI		loc_45530
		ADC		_star_field_center_pos_y
		JMP		loc_45535
loc_45530:
		SBC		_star_field_center_pos_y
		EOR		#$FF
loc_45535:
		CMP		#$40
		BCC		loc_4556B
		CMP		#$AA
		BCS		loc_4556B
		STA		_spr_buf,Y
		LDY		_star_field_data2,X
		LDA		_star_field_data1,X
		CLC
		ADC		_star_field_params3,Y
		STA		_star_field_data1,X
		INC		_star_field_data2,X
		LDA		_star_field_params4,Y
		LDY		_star_field_params1,X
		CLC
		ADC		#$8B
		STA		_spr_buf+1,Y
		TXA
		AND		#3
		ORA		#$20
		STA		_spr_buf+2,Y
		DEX
		BMI		locret_4556A
		JMP		_star_field_loop
locret_4556A:
		RTS
loc_4556B:
		JSR		_star_field_randomize
		AND		#$3F
		STA		_star_field_data0,X
		JSR		_star_field_randomize
		AND		#$1F
		STA		_star_field_data1,X
		LDA		#0
		STA		_star_field_data2,X
		JMP		_star_field_loop

; =============== S U B	R O U T	I N E =======================================
_star_field_randomize:
		SEC
		LDA		_star_field_rand_seed+1
		ADC		_star_field_rand_seed+4
		ADC		_star_field_data0
		STA		_star_field_rand_seed
		LDA		_star_field_rand_seed+4
		STA		_star_field_data0
		LDA		_star_field_rand_seed+3
		STA		_star_field_rand_seed+4
		LDA		_star_field_rand_seed+2
		STA		_star_field_rand_seed+3
		LDA		_star_field_rand_seed+1
		STA		_star_field_rand_seed+2
		LDA		_star_field_rand_seed
		STA		_star_field_rand_seed+1
		RTS

; =============== S U B	R O U T	I N E =======================================
_A_mul_B_mul_2:
		LDA		#0
		STA		_mul_result_low
		LDY		#8
loc_455B6:
		LSR		_mul_argA
		BCC		loc_455BF
		CLC
		ADC		_mul_argB
loc_455BF:
		ROR
		ROR		_mul_result_low
		DEY
		BNE		loc_455B6
		ASL		_mul_result_low
		ROL
		RTS
_star_field_params1:
		.BYTE	  0,  4,  8, $C,$10,$14,$18,$1C,$20,$24,$28,$2C,$30,$34,$38,$3C
		.BYTE	$40,$44,$48,$4C,$50,$54,$58,$5C,$60,$64,$68,$6C,$70,$74,$78,$7C
		.BYTE	$80,$84,$88,$8C,$90,$94,$98,$9C,$A0,$A4,$A8,$AC,$B0,$B4,$B8,$BC
		.BYTE	$C0,$C4,$C8,$CC,$D0,$D4,$D8,$DC,$E0,$E4,$E8,$EC,$F0,$F4,$F8,$FC
_star_field_params4:
		.BYTE	  6,  6,  6,  6,  6,  6,  6,  6,  5,  5,  5,  5,  5,  5,  5,  5
		.BYTE	  4,  4,  4,  4,  4,  4,  4,  4,  3,  3,  3,  3,  2,  2,  2,  2
		.BYTE	  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1
		.BYTE	  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1
		.BYTE	  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1
		.BYTE	  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1
		.BYTE	  1,  1,  1,  1,  1,  1,  1,  1
_star_field_params3:
		.BYTE	  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1
		.BYTE	  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  3,  3
		.BYTE	  3,  3,  3,  4,  4,  4,  5,  5,  6,  6,  7,  8,  9, $B, $C, $E
		.BYTE	$10,$12,$14,$17,$1A,$1E,$22,$26,$2B,$34,$38,$42,$4A,$4A,$4A,$4A
		.BYTE	$4A,$4A,$4A,$4A,$4A,$4A
_star_field_params0:
		.BYTE	  0, $B,$18,$25,$30,$3C,$47,$51,$5A,$63,$6B,$71,$77,$7A,$7D,$7E
		.BYTE	$7E,$7E,$7C,$78,$73,$6E,$66,$5E,$55,$4B,$41,$35,$2A,$1D,$11,  5
		.BYTE	$87,$94,$A1,$AE,$B9,$C3,$CF,$D8,$E1,$E8,$F0,$F5,$FA,$FD,$FF,$FF
		.BYTE	$FF,$FE,$FA,$F7,$F1,$EB,$E3,$DA,$D1,$C7,$BC,$B0,$A3,  0, $B,$18
_star_field_params2:
		.BYTE	$7D,$79,$75,$6F,$68,$61,$58,$4F,$45,$39,$2E,$21,$14,  8,$83,$90
		.BYTE	$9D,$AA,$B5,$C1,$CB,$D5,$DE,$E7,$EE,$F4,$F9,$FD,$FE,$FF,$FF,$FE
		.BYTE	$FC,$F8,$F3,$EC,$E6,$DD,$D4,$CA,$C0,$B4,$A7,$9C,$8F,$82, $A,$17
		.BYTE	$23,$2F,$3A,$46,$50,$59,$62,$6A,$70,$75,$7A,$7D,$7E,$7D,$79,$74
		.BYTE	$A9,$1E,$8D,$FF,$9F,$4A,$8D,$FF,$9F,$4A,$8D,$FF,$9F,$4A,$8D,$FF
		.BYTE	$9F,$4A,$8D,$FF,$9F,$A9,$18,$8D,$FF,$BF,$4A,$8D,$FF,$BF,$4A,$8D
		.BYTE	$FF,$BF,$4A,$8D,$FF,$BF,$4A,$8D,$FF,$BF,$A9,$17,$8D,$FF,$DF,$4A
		.BYTE	$8D,$FF,$DF,$4A,$8D,$FF,$DF,$4A,$8D,$FF,$DF,$4A,$8D,$FF,$DF,$A9
		.BYTE	 $F,$8D,$FF,$FF,$4A,$8D,$FF,$FF,$4A,$8D,$FF,$FF,$4A,$8D,$FF,$FF
		.BYTE	$4A,$8D,$FF,$FF,$60
_disp_tlm_lib:
		.WORD _tlm0_ocean
		.WORD _tlm1_probe
		.WORD _tlm2_title
		.WORD _tlm3_hiscore
		.WORD _tlm4_frame
		.WORD _tlm5_lewis
		.WORD _tlm6_robocop
		.WORD _tlm7_ending
		.WORD _tlm8_repair
_disp_atr_lib:
		.WORD _atr0_ocean
		.WORD _atr1_probe
		.WORD _atr2_title
		.WORD _atr3_hiscore
		.WORD _atr4_frame
		.WORD _atr5_lewis
		.WORD _atr6_robocop
		.WORD _atr7_ending
		.WORD _atr8_repair
_disp_spr_lib:
		.WORD 0
		.WORD 0
		.WORD _spr2_title
		.WORD 0
		.WORD 0
		.WORD 0
		.WORD 0
		.WORD 0
		.WORD _spr8_repair
_disp_pal_lib:
		.WORD _pal0_ocean
		.WORD _pal1_probe
		.WORD _pal2_title
		.WORD _pal3_hiscore
		.WORD _pal3_hiscore
		.WORD _pal3_hiscore
		.WORD _pal3_hiscore
		.WORD _pal7_ending
		.WORD _pal8_repair
byte_457D6:
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,$BE
_disp_chr1_list:
		.BYTE	$18,$18,$19,$1C,$1C,$1C,$1D,$1E,$16
_disp_chr0_list:
		.BYTE	$1C,$1C,$1B,$18,$18,$18,$18,$1E,$16
_disp_spr0hit_use_flags:
		.BYTE	  0,  0,$FF,  0,  0,  0,  0,$FF,$FF
_disp_spr0hit_x:
		.BYTE	  0,  0,$BD,  0,  0,  0,  0,$F8,$F8
_disp_spr0hit_y:
		.BYTE	  0,  0,$65,  0,  0,  0,  0,$7C,$7C
_disp_spr0hit_tile:
		.BYTE	  0,  0,$FF,  0,  0,  0,  0,$30,$30
_disp_extra_ctrl_flags_list:
		.BYTE	  0,  0,$FF,  0,  0,  0,  0,  0,  0
_disp_extra_shifts_list:
		.BYTE	  0,  0,$10,  0,  0,  0,  0,$10,$10
_delays_list_lo:
		.BYTE	  0,  1,  2,  6, $A, $A, $B, $C,$11
_delays_list_hi:
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0

; =============== S U B	R O U T	I N E =======================================
_screen_display:
		TAX
		LDY		#0
		STY		_PPU_CTRL
		STY		_PPU_MASK
		BIT		_PPU_STATUS
		BPL		_screen_display
		BIT		_PPU_STATUS
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#0
		STA		_PPU_ADDR
		TAY
loc_45855:
		STA		_PPU_DATA
		STA		_PPU_DATA
		STA		_PPU_DATA
		STA		_PPU_DATA
		INY
		BNE		loc_45855
		BIT		_PPU_STATUS
		LDA		#$24
		STA		_PPU_ADDR
		LDA		#0
		STA		_PPU_ADDR
		TAY
loc_45872:
		STA		_PPU_DATA
		STA		_PPU_DATA
		STA		_PPU_DATA
		STA		_PPU_DATA
		INY
		BNE		loc_45872
		LDY		#0
		LDA		#$F0
loc_45885:
		STA		_spr_buf,Y
		INY
		BNE		loc_45885
		LDA		_disp_chr0_list,X
		STA		_chr1_bank_shadow
		LDA		_disp_chr1_list,X
		STA		_chr0_bank_shadow
		LDA		_disp_extra_ctrl_flags_list,X
		STA		_disp_extra_ctrl
		LDA		_disp_extra_shifts_list,X
		STA		_disp_extra_shift
		LDA		_disp_spr0hit_use_flags,X
		BEQ		loc_458C2
		STA		_disp_spr0hit_use
		LDA		_disp_spr0hit_x,X
		STA		_spr_buf+3
		LDA		_disp_spr0hit_y,X
		STA		_spr_buf
		LDA		_disp_spr0hit_tile,X
		STA		_spr_buf+1
		LDA		#$FF
		STA		_spr_buf+2
loc_458C2:
		TXA
		ASL
		TAX
		LDA		_disp_tlm_lib,X
		STA		_obj_flags._player
		LDA		_disp_spr_lib,X
		STA		_obj_flags._enemy+1
		LDA		_disp_pal_lib,X
		STA		_obj_flags._pbullet
		LDA		_disp_atr_lib,X
		STA		_obj_flags._pbullet+2
		INX
		LDA		_disp_tlm_lib,X
		STA		_obj_flags._enemy
		LDA		_disp_spr_lib,X
		STA		_obj_flags._enemy+2
		LDA		_disp_pal_lib,X
		STA		_obj_flags._pbullet+1
		LDA		_disp_atr_lib,X
		STA		_obj_flags._ebullet
		LDA		#$24
		STA		_PPU_ADDR
		STY		_PPU_ADDR
		TYA
loc_458F7:
		STA		_PPU_DATA
		STA		_PPU_DATA
		STA		_PPU_DATA
		STA		_PPU_DATA
		INY
		BNE		loc_458F7
		LDA		#$20
		STA		_PPU_ADDR
		STY		_PPU_ADDR
_disp_tlm_send:
		LDY		#0
		LDA		(_obj_flags),Y
		BPL		loc_4592B
		CMP		#$FF
		BEQ		_disp_atr_send
		AND		#$7F
		TAX
		JSR		_ptr5_inc
loc_4591E:
		LDA		(_obj_flags),Y
		STA		_PPU_DATA
		JSR		_ptr5_inc
		DEX
		BNE		loc_4591E
		BEQ		_disp_tlm_send
loc_4592B:
		TAX
		JSR		_ptr5_inc
		LDA		(_obj_flags),Y
loc_45931:
		STA		_PPU_DATA
		DEX
		BNE		loc_45931
		JSR		_ptr5_inc
		JMP		_disp_tlm_send
_ptr5_inc:
		INC		_obj_flags._player
		BNE		locret_45943
		INC		_obj_flags._enemy
locret_45943:
		RTS
_disp_atr_send:
		LDA		#$23
		STA		_PPU_ADDR
		LDX		#$C0
		STX		_PPU_ADDR
loc_4594E:
		LDA		(_obj_flags._pbullet+2),Y
		STA		_PPU_DATA
		INY
		INX
		BNE		loc_4594E
		LDY		#$1F
loc_45959:
		LDA		(_obj_flags._pbullet),Y
		STA		_pal_buf,Y
		DEY
		BPL		loc_45959
		LDA		_obj_flags._enemy+1
		BNE		_disp_spr_send
		LDA		_obj_flags._enemy+2
		BNE		_disp_spr_send
		JMP		locret_459F1
_disp_spr_send:
		LDA		#$C
		STA		_obj_flags._player
		LDA		#$FF
		STA		_obj_flags._enemy
		LDA		#0
		STA		byte_3CF
loc_45979:
		LDX		#0
loc_4597B:
		LDY		#0
		LDA		(_obj_flags._enemy+1),Y
		BEQ		loc_459D4
		STA		_player_jump_or_fall_flag
		CMP		#$FF
		BNE		loc_45992
		LDA		byte_3CF
		EOR		#$80
		STA		byte_3CF
		JMP		loc_459D4
loc_45992:
		LDA		_player_jump_or_fall_flag
		AND		#$3F
		LDY		_obj_flags._player
		TXA
		PHA
		LDX		_screen_idx
		LDA		_player_jump_or_fall_flag
		AND		#$3F
		CLC
		ADC		byte_457D6,X
		BIT		byte_3CF
		BPL		loc_459AC
		ADC		#$34
loc_459AC:
		STA		_spr_buf+1,Y
		PLA
		TAX
		ASL
		ASL
		ASL
		STA		_spr_buf+3,Y
		LDA		_obj_flags._enemy
		CLC
		ADC		_disp_extra_shift
		STA		_spr_buf,Y
		LDA		_player_jump_or_fall_flag
		LSR
		LSR
		LSR
		LSR
		LSR
		LSR
		STA		_spr_buf+2,Y
		LDA		_obj_flags._player
		CLC
		ADC		#4
		STA		_obj_flags._player
		BCS		locret_459F1
loc_459D4:
		LDA		_obj_flags._enemy+1
		CLC
		ADC		#1
		STA		_obj_flags._enemy+1
		LDA		_obj_flags._enemy+2
		ADC		#0
		STA		_obj_flags._enemy+2
		INX
		CPX		#$20
		BCC		loc_4597B
		LDA		_obj_flags._enemy
		CLC
		ADC		#8
		STA		_obj_flags._enemy
		CMP		#$C8
		BCC		loc_45979
locret_459F1:
		RTS

; =============== S U B	R O U T	I N E =======================================
_hi_score_sort_test:
		LDX		#0
		LDA		_cur_score_hex
		STA		_pl_bckp_score_hex_lo,X
		STA		_tmp_score_hex_value
		LDA		_cur_score_hex+1
		STA		_pl_bckp_score_hex_hi,X
		STA		_tmp_score_hex_value+1
		LDA		#0
		STA		_dec_score_chr_ofs_delta
		LDX		#<[_hi_dec_tmp_buf]
		LDY		#>[_hi_dec_tmp_buf]
		JSR		_hex2str_hiscore
		LDX		#0
		LDY		#0
loc_45A16:
		LDA		_hi_dec_tmp_buf,X
		STA		_score_cmp_buf,X
		INX
		CPX		#5
		BNE		loc_45A16
loc_45A21:
		LDA		#0
		STA		_score_cmp_buf,X
		INX
		CPX		#$F
		BCC		loc_45A21
		LDA		#0
		STA		_hi_score_pos_count
		STA		_hi_score_line_idx
		LDA		#<[_hi0_score]
		STA		_tmpE2
		LDA		#>[_hi0_score]
		STA		_tmpE2+1
loc_45A3B:
		LDY		#0
		LDA		_hi_score_pos_count
		CMP		#6
		BCS		locret_45A7A
loc_45A44:
		LDA		_score_cmp_buf,Y
		CMP		(_tmpE2),Y
		BCC		loc_45A52
		BNE		_hiscore_swap
		INY
		CPY		#5
		BNE		loc_45A44
loc_45A52:
		INC		_hi_score_line_idx
loc_45A55:
		INC		_hi_score_pos_count
		LDA		_tmpE2
		CLC
		ADC		#$F
		STA		_tmpE2
		LDA		_tmpE2+1
		ADC		#0
		STA		_tmpE2+1
		BNE		loc_45A3B
_hiscore_swap:
		LDY		#$E
loc_45A69:
		LDA		(_tmpE2),Y
		TAX
		LDA		_score_cmp_buf,Y
		STA		(_tmpE2),Y
		TXA
		STA		_score_cmp_buf,Y
		DEY
		BPL		loc_45A69
		BMI		loc_45A55
locret_45A7A:
		RTS
_chars_table:
		.BYTE	 $F,$1B,$1C,$1D,$1E,$1F,$20,$21,$22,$23
_unused_hi_score:
		.BYTE	  1,  0,  2,  0,  0,  7,$24,  8,  1,$12,$12,  9,$13, $F,  $E
		.BYTE	  0,  7,  5,  3,  0,$13,$24, $E,  9,  3, $F, $C,  0,  0,  0
		.BYTE	  0,  7,  0,  0,  0,$13,$24,  3,$12, $F,$17,  0,  0,  0,  0
		.BYTE	  0,  4,  0,  6,  5,  8,$24,  2,  9, $E, $E,$13,  0,  0,  0
		.BYTE	  0,  0,  8,  0,  0, $A,$24,$14,  5, $C,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  4,  5, $A,$24,  2, $F, $E,  1,$12,  0,  0,  0

; =============== S U B	R O U T	I N E =======================================
_hi_score_display:
		BIT		_PPU_STATUS
		LDA		#$21
		STA		_PPU_ADDR
		LDA		#$47
		STA		_PPU_ADDR
		LDX		#0
loc_45AEE:
		LDY		_hi0_score,X
		LDA		_chars_table,Y
		STA		_PPU_DATA
		INX
		CPX		#5
		BCC		loc_45AEE
		LDA		#$F
		STA		_PPU_DATA
		STA		_PPU_DATA
		LDA		#0
		STA		_PPU_DATA
		STA		_PPU_DATA
		STA		_PPU_DATA
loc_45B0F:
		LDA		_hi0_score,X
		STA		_PPU_DATA
		INX
		CPX		#$F
		BCC		loc_45B0F
		LDA		#$21
		STA		_PPU_ADDR
		LDA		#$87
		STA		_PPU_ADDR
		LDX		#0
loc_45B26:
		LDY		_hi1_score,X
		LDA		_chars_table,Y
		STA		_PPU_DATA
		INX
		CPX		#5
		BCC		loc_45B26
		LDA		#$F
		STA		_PPU_DATA
		STA		_PPU_DATA
		LDA		#0
		STA		_PPU_DATA
		STA		_PPU_DATA
		STA		_PPU_DATA
loc_45B47:
		LDA		_hi1_score,X
		STA		_PPU_DATA
		INX
		CPX		#$F
		BCC		loc_45B47
		LDA		#$21
		STA		_PPU_ADDR
		LDA		#$C7
		STA		_PPU_ADDR
		LDX		#0
loc_45B5E:
		LDY		_hi2_score,X
		LDA		_chars_table,Y
		STA		_PPU_DATA
		INX
		CPX		#5
		BCC		loc_45B5E
		LDA		#$F
		STA		_PPU_DATA
		STA		_PPU_DATA
		LDA		#0
		STA		_PPU_DATA
		STA		_PPU_DATA
		STA		_PPU_DATA
loc_45B7F:
		LDA		_hi2_score,X
		STA		_PPU_DATA
		INX
		CPX		#$F
		BCC		loc_45B7F
		LDA		#$22
		STA		_PPU_ADDR
		LDA		#7
		STA		_PPU_ADDR
		LDX		#0
loc_45B96:
		LDY		_hi3_score,X
		LDA		_chars_table,Y
		STA		_PPU_DATA
		INX
		CPX		#5
		BCC		loc_45B96
		LDA		#$F
		STA		_PPU_DATA
		STA		_PPU_DATA
		LDA		#0
		STA		_PPU_DATA
		STA		_PPU_DATA
		STA		_PPU_DATA
loc_45BB7:
		LDA		_hi3_score,X
		STA		_PPU_DATA
		INX
		CPX		#$F
		BCC		loc_45BB7
		LDA		#$22
		STA		_PPU_ADDR
		LDA		#$47
		STA		_PPU_ADDR
		LDX		#0
loc_45BCE:
		LDY		_hi4_score,X
		LDA		_chars_table,Y
		STA		_PPU_DATA
		INX
		CPX		#5
		BCC		loc_45BCE
		LDA		#$F
		STA		_PPU_DATA
		STA		_PPU_DATA
		LDA		#0
		STA		_PPU_DATA
		STA		_PPU_DATA
		STA		_PPU_DATA
loc_45BEF:
		LDA		_hi4_score,X
		STA		_PPU_DATA
		INX
		CPX		#$F
		BCC		loc_45BEF
		LDA		#$22
		STA		_PPU_ADDR
		LDA		#$87
		STA		_PPU_ADDR
		LDX		#0
loc_45C06:
		LDY		_hi5_score,X
		LDA		_chars_table,Y
		STA		_PPU_DATA
		INX
		CPX		#5
		BCC		loc_45C06
		LDA		#$F
		STA		_PPU_DATA
		STA		_PPU_DATA
		LDA		#0
		STA		_PPU_DATA
		STA		_PPU_DATA
		STA		_PPU_DATA
loc_45C27:
		LDA		_hi5_score,X
		STA		_PPU_DATA
		INX
		CPX		#$F
		BCC		loc_45C27
		RTS

; =============== S U B	R O U T	I N E =======================================
_repair_cursor_input:
		LDA		_dpad
		BNE		loc_45C39
		RTS
loc_45C39:
		CMP		#1
		BEQ		loc_45C48
		CMP		#2
		BEQ		loc_45C4B
		CMP		#3
		BEQ		loc_45C4E
		JMP		loc_45C51
loc_45C48:
		JMP		loc_45C54
loc_45C4B:
		JMP		loc_45C6B
loc_45C4E:
		JMP		loc_45C82
loc_45C51:
		JMP		loc_45C99
loc_45C54:
		JSR		_get_cursor_allow_list_ptr
		LDY		#0
		LDA		(_obj_hcoll._enemy+1),Y
		BMI		locret_45C6A
		CMP		_repair_body_index_request
		BEQ		locret_45C6A
		STA		_repair_body_index_request
		LDA		#1
		STA		byte_3BF
locret_45C6A:
		RTS
loc_45C6B:
		JSR		_get_cursor_allow_list_ptr
		LDY		#1
		LDA		(_obj_hcoll._enemy+1),Y
		BMI		locret_45C81
		CMP		_repair_body_index_request
		BEQ		locret_45C81
		STA		_repair_body_index_request
		LDA		#1
		STA		byte_3BF
locret_45C81:
		RTS
loc_45C82:
		JSR		_get_cursor_allow_list_ptr
		LDY		#2
		LDA		(_obj_hcoll._enemy+1),Y
		BMI		locret_45C98
		CMP		_repair_body_index_request
		BEQ		locret_45C98
		STA		_repair_body_index_request
		LDA		#1
		STA		byte_3BF
locret_45C98:
		RTS
loc_45C99:
		JSR		_get_cursor_allow_list_ptr
		LDY		#3
		LDA		(_obj_hcoll._enemy+1),Y
		BMI		locret_45CAF
		CMP		_repair_body_index_request
		BEQ		locret_45CAF
		STA		_repair_body_index_request
		LDA		#1
		STA		byte_3BF
locret_45CAF:
		RTS

; =============== S U B	R O U T	I N E =======================================
_get_cursor_allow_list_ptr:
		LDA		#0
		STA		_obj_hcoll._enemy+2
		LDX		byte_3C2
		LDA		byte_45D7E,X
		CLC
		LDA		_repair_cur_body_index
		ASL
		ASL
		ROL		_obj_hcoll._enemy+2
		CLC
		ADC		#<[_repair_cursor_allow_table]
		STA		_obj_hcoll._enemy+1
		LDA		_obj_hcoll._enemy+2
		ADC		#>[_repair_cursor_allow_table]
		STA		_obj_hcoll._enemy+2
		RTS

; =============== S U B	R O U T	I N E =======================================
_repair_cursor_animate:
		LDA		byte_3BF
		BNE		loc_45CD4
		RTS
loc_45CD4:
		INC		byte_3C1
		LDA		byte_3C1
		CMP		#2
		BEQ		loc_45CDF
		RTS
loc_45CDF:
		LDA		#0
		STA		byte_3C1
		LDX		byte_3C2
		LDA		byte_45D7E,X
		CLC
		ADC		_repair_body_index_request
		ASL
		TAX
		LDA		word_45DDF,X
		STA		word_3BA
		LDA		word_45DDF+1,X
		STA		word_3BA+1
		LDA		byte_3B8
		CMP		word_3BA
		BCS		loc_45D1D
		LDA		word_3BA
		SEC
		SBC		byte_3B8
		LSR
		LSR
		STA		word_3BA
		LDA		byte_3B8
		CLC
		ADC		word_3BA
		STA		byte_3B8
		JMP		loc_45D33
loc_45D1D:
		LDA		byte_3B8
		SEC
		SBC		word_3BA
		LSR
		LSR
		STA		word_3BA
		LDA		byte_3B8
		SEC
		SBC		word_3BA
		STA		byte_3B8
loc_45D33:
		LDA		_tmp3C3
		CMP		word_3BA+1
		BCS		loc_45D54
		LDA		word_3BA+1
		SEC
		SBC		_tmp3C3
		LSR
		LSR
		STA		word_3BA+1
		LDA		_tmp3C3
		CLC
		ADC		word_3BA+1
		STA		_tmp3C3
		JMP		loc_45D6A
loc_45D54:
		LDA		_tmp3C3
		SEC
		SBC		word_3BA+1
		LSR
		LSR
		STA		word_3BA+1
		LDA		_tmp3C3
		SEC
		SBC		word_3BA+1
		STA		_tmp3C3
loc_45D6A:
		LDA		word_3BA
		ORA		word_3BA+1
		BNE		locret_45D7D
		LDA		#0
		STA		byte_3BF
		LDA		_repair_body_index_request
		STA		_repair_cur_body_index
locret_45D7D:
		RTS
byte_45D7E:
		.BYTE	0

; =============== S U B	R O U T	I N E =======================================
_hex2str_hiscore:
		STX		_obj_hcoll._pbullet+2
		STY		_obj_hcoll._ebullet
		LDY		#0
loc_45D85:
		LDX		#$FF
		SEC
loc_45D88:
		INX
		LDA		_tmp_score_hex_value
		SBC		_dec_list_lo,Y
		STA		_tmp_score_hex_value
		LDA		_tmp_score_hex_value+1
		SBC		_dec_list_hi,Y
		STA		_tmp_score_hex_value+1
		BCS		loc_45D88
		LDA		_tmp_score_hex_value
		ADC		_dec_list_lo,Y
		STA		_tmp_score_hex_value
		LDA		_tmp_score_hex_value+1
		ADC		_dec_list_hi,Y
		STA		_tmp_score_hex_value+1
		TXA
		CLC
		ADC		_dec_score_chr_ofs_delta
		STA		(_obj_hcoll._pbullet+2),Y
		INY
		CPY		#5
		BNE		loc_45D85
		RTS
_dec_list_lo:
		.BYTE	$10,$E8,$64, $A,  1
_dec_list_hi:
		.BYTE	$27,  3,  0,  0,  0
_repair_update_flag_list:
		.BYTE	  0,  2,  1,  2,  3
_repair_cursor_allow_table:
		.BYTE	$FF,  3,  2,  1
		.BYTE	  0,  2,  4,$FF
		.BYTE	  0,  3,  4,  1
		.BYTE	  0,$FF,  4,  2
		.BYTE	  2,$FF,$FF,  1
word_45DDF:
		.WORD $20A0
		.WORD $5888
		.WORD $40A8
		.WORD $48C0
		.WORD $80CB
_pal3_hiscore:
		.BYTE	 $F,$17,$27,$30, $F,  0,$10,$20, $F,$12,$22,$30, $F, $A,$2A, $F
		.BYTE	 $F,  7,$17,$20, $F,  0,$10,$20, $F,$12,$22,$30, $F,$21,$10,$20
_pal2_title:
		.BYTE	 $F,$27,$26,$30, $F,$11,$21,$30, $F,$27,$38,$30, $F,$27,$36,$30
		.BYTE	 $F,$27,$36,$30, $F,$27,$38,$30, $F,$16,$26, $F, $F,$21,$10,$20
_pal7_ending:
		.BYTE	 $F,$27,$36,$30, $F,$11,$21,$30, $F,$27,$38,$30, $F,$21,$10,$20
		.BYTE	 $F,$27,$36,$30, $F,$11,$21,$30, $F,$27,$38,$30, $F,$21,$10,$20
_pal1_probe:
		.BYTE	 $F,$16,$26, $F, $F, $F, $F, $F, $F, $F, $F, $F, $F,$12,$22,$32
		.BYTE	 $F,$16,$26, $F, $F,$21,$22,$32, $F,$21,$22,$32, $F,$21,$22,$32
_pal0_ocean:
		.BYTE	 $F,  0,$10,$20, $F,  1,$11,$21, $F,  2,$12,$10, $F,  3,$13,$23
		.BYTE	 $F,$16,$26, $F, $F,  5,$15,$25, $F,  6,$16,$26, $F,  7,$17,$27
_pal8_repair:
		.BYTE	 $F,  8,$18,$2B, $F,  0,$10,$20, $F, $B,$1B,$2B, $F, $C,$1C,$2C
		.BYTE	 $F,  0,$10,$20, $F,  6,$16,$26, $F, $B,$1B,$2B, $F, $C,$1C,$2C
_atr0_ocean:
		.BYTE	$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$3F, $F, $F, $F, $F, $F,$FF
		.BYTE	$FF,$88,$AA,$AA,$AA,$AA,$AA,$CC,$FF,$34,  5,  5,  5,  5,  5,$FC
		.BYTE	$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
_atr2_title:
		.BYTE	$55,$55,$55,$55,$55,$55,$55,$55,$76,$59,$DA,$5A,$55,$CE,$77,$59
		.BYTE	$FF,$55,$DD,$55,$55,$8C,$AF,$EC,$FF,$D5,$55,$55,$55,$5D,$53,$DD
		.BYTE	$FF,$55,$55,$55,$55,$55,$55,$55,$FF,$FF,$77,$55,$55,$55,$D5,$FD
		.BYTE	$55,$55,$55,$55,$55,$55,$55,$55
_atr5_lewis:
		.BYTE	$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$AF,$AB,$AF,$AB,$AE,$AF,$AE,$AF
		.BYTE	$EE,$FF,$FF,$55,$55,$FF,$FF,$BB,$EE,$FF,$FF,$11,$44,$DF,$FF,$BB
		.BYTE	$EE,$FF,$F5,$F5,$F5,$F5,$FF,$BB,$AE,$AF,$AF,$AF,$AF,$AF,$AF,$AB
_atr6_robocop:
		.BYTE	$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$AF,$AB,$AF,$AB,$AE,$AF,$AE,$AF
		.BYTE	$EE,$FF,$9B,$A6,$A5,$DD,$FF,$BB,$EE,$FF,$A9,$A8,$AA,$DD,$FF,$BB
		.BYTE	$EE,$FF,$FA,$FA,$FA,$FD,$FF,$BB,$AE,$AF,$AF,$AF,$AF,$AF,$AF,$AB
_atr7_ending:
		.BYTE	$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$BF,$AF,$AF,$AF,$FF,$FF,$DF,$FF
		.BYTE	$AA,$AA,$AA,$AA,$32,$4C,$55,$CD,$AA,$AA,$AA,$AA,$5A,$54,  5,$58
		.BYTE	$AA,$AA,$AA,$AA,$55,$55,$55,$55,$AA,$AA,$AA,$AA,$55,$FF,$FF,$FF
		.BYTE	  0,  0,  0,  0,$FF,$FF,$FF,$FF
_atr3_hiscore:
		.BYTE	$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$AF,$AB,$AF,$AB,$AE,$AF,$AE,$AF
		.BYTE	$EE,$FF,$FF,$FF,$FF,$FF,$FF,$BB,$EE,$FF,$FF,$FF,$FF,$FF,$FF,$BB
		.BYTE	$EE,$FF,$FF,$FF,$FF,$FF,$FF,$BB,$AE,$AF,$AF,$AF,$AF,$AF,$AF,$AB
_atr4_frame:
		.BYTE	$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$AF,$AB,$AF,$AB,$AE,$AF,$AE,$AF
		.BYTE	$EE,$FF,$FF,$FF,$FF,$FF,$FF,$BB,$EE,$FF,$FF,$FF,$FF,$FF,$FF,$BB
		.BYTE	$EE,$FF,$FF,$FF,$FF,$FF,$FF,$BB,$AE,$AF,$AF,$AF,$AF,$AF,$AF,$AB
_atr1_probe:
		.BYTE	$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
		.BYTE	$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
_atr8_repair:
		.BYTE	$FF, $C,  3,  3,$FF,$FF,$C0,$F0,$3F,  0,  0,$C0,$FF,$FF,$BA,$EA
		.BYTE	  0,$5C,$9F,$FA,$FF,$FF,$FF,$AF,  5,  5,  5,$DD,$EF,$FF,$BB,$EA
		.BYTE	  0,  0,  0,$DD,$BA,$EF,$FF,$FF,$FF,$FF,  0,$DD,$FF,$FE,$FE,$FF
		.BYTE	$AF,$2F,$A0, $D,$AF,$8F,$AF,$8F
_tlm0_ocean:
		.BYTE	$81,  1,$7F,  0,$67,  0,$88,  2,  3,  4,  5,  6,  7,  8,  9,  4
		.BYTE	 $A,$88, $B, $C, $D, $E, $F,$10,$11,$12, $A,  0,$98,$13,$14,$15
		.BYTE	$16,$17,$18,$19,$16,$17,$18,$1A,$16,$1B,$18,$19,$16,$17,$18,$1C
		.BYTE	$1D,$1E,$1F,$20,$21,  8,  0,$98,$22,$23,$24,$25,$26,$27,$28,$25
		.BYTE	$26,$29,$2A,$2B,$2C,$2D,$28,$25,$26,$2E,$2F,$30,$31,$32,$33,$34
		.BYTE	  8,  0,$98,$35,$36,$37,$38,$39,$3A,$3B,$38,$39,$3C,$3D,$3E,$3F
		.BYTE	$40,$3B,$38,$39,$41,$42,$43,$44,$45,$46,$47,  8,  0,$98,$48,$49
		.BYTE	$4A,$4B,$4C,$4D,$4E,$4B,$4C,$4F,$50,$51,$52,$53,$54,$4B,$4C,$4D
		.BYTE	$55,$56,$57,$58,$59,$5A,  8,  0,$98,$5B,$5C,$5D,$5E,$5F,$60,$61
		.BYTE	$5E,$5F,$60,$61,$5E,$62,$63,$61,$5E,$5F,$64,$65,$66,$67,$68,$69
		.BYTE	$6A,  8,  0,$98,$6B,$6C,$6D,$6E,$6F,$70,$71,$72,$6F,$70,$71,$72
		.BYTE	$6F,$70,$71,$72,$6F,$73,$74,$75,$76,$77,$78,$79, $A,  0,$88,$7A
		.BYTE	$7B,$7C,$7D,$7E,$7F,$80,$81,  4,$82,$88,$83,$84,$85,$86,$87,$88
		.BYTE	$89,$8A,$7F,  0,$7F,  0,$7F,  0,$FF,$FF
_tlm2_title:
		.BYTE	$F2,  1,  2,  3,  4,  5,  6,  7,  8,  9,  6,  7, $A, $B,  6,  7
		.BYTE	 $C, $D, $E, $F,$10,$11,$12,  7,  8,  9,  6,  7, $A,$13,$14,$15
		.BYTE	$16,$17,$18,$19,$1A,$1B,$1C,$1D,$1E,$1F,$20,$1D,$21,$22,$1C,$1D
		.BYTE	$23,$24,$25,$26,$27,$28,$1C,$1D,$1E,$1F,$20,$1D,$29,$2A,$2B,$2C
		.BYTE	$2D,$2E,$2F,$30,$31,$32,$33,$34,$35,$36,$37,$38,$39,$3A,$33,$34
		.BYTE	$35,$3B,$3C,  0,$3D,$3E,$33,$34,$3F,$40,$41,$42,$43,  0,$44,$45
		.BYTE	$46,$47,$48,$49,$4A,$4B,$4C,$4D,$4E,$4F,$50,$51,$52,$53,$4C,$4D
		.BYTE	$54,$55,$56,  2,  0,$87,$57,$4C,$58,$59,$5A,$5B,$5C,  2,  0,$9D
		.BYTE	$5D,$5E,$5F,$47,$60,$61,$62,$63,$64,$65,$66,$67,$68,$65,$69,$6A
		.BYTE	$6B,$65,$6C,$6D,$6E,$6F,$70,$71,$72,$73,$74,$75,$76,  2,  0,$A4
		.BYTE	$77,$78,$79,$7A,$7B,$7C,$7D,$7E,$7F,$80,$81,$82,$83,$84,$81,$85
		.BYTE	$86,$87,$88,$89,$8A,$8B,$8C,$8D,$8E,$8F,$90,$91,$92,$93,$94,$95
		.BYTE	$96,$97,$98,$99,  2,  0,$85,$9A,$9B,$9C,$9D,$9E,  6,  0,$93,$9F
		.BYTE	$A0,$A1,$A2,$A3,  0,$A4,$A5,$A6,  0,$A7,$A8,$A9,$AA,$AB,$AC,$AD
		.BYTE	$AE,$AF,  2,  0,$87,$B0,$B1,$B2,$B3,$B4,$B5,$B6,  4,  0,$93,$B7
		.BYTE	$B8,$B9,$BA,$BB,$BC,$BD,$BE,$BF,$C0,$C1,$C2,$C3,$C4,$C5,$C6,  0
		.BYTE	$C7,$C8,  4,  0,$85,$C9,$CA,$CB,$CC,$CD,  4,  0,$89,$CE,$CF,$D0
		.BYTE	$D1,$D2,$D3,$D4,$D5,$D6,  6,  0,$81,$D7,  7,  0,$86,$D8,$D9,$DA
		.BYTE	$DB,$DC,$DD,  3,  0,$88,$DE,$DF,$E0,$E1,$E2,$E3,$E4,$E5,  7,  0
		.BYTE	$81,$E6,  7,  0,$82,$E7,$E8,  2,  0,$82,$E9,$EA,  3,  0,$8F,$EB
		.BYTE	$EC,$ED,$EE,  0,$EF,$F0,$F1,$F2,  0,$F3,$F4,$F5,$F6,$F7,  8,  0
		.BYTE	$86,  1,  2,  0,  3,  4,  5,  4,  0,$8F,  6,  7,  8,  9, $A, $B
		.BYTE	  0, $C, $D, $E, $F,$10,$11,$12,$13,  7,  0,$8B,$14,$15,$16,$17
		.BYTE	$18,$19,$1A,$1B,$1C,$1D,$1E,  2,  0,$83,$1F,$20,$21,  2,  0,$87
		.BYTE	$22,$23,$24,$25,$26,$27,$28,  8,  0,$98,$29,  0,$2A,$2B,$2C,$2D
		.BYTE	$2E,$2F,$30,$31,$32,$33,$34,$35,$36,$37,  0,$38,$39,$3A,$3B,  0
		.BYTE	$3C,$3D,  7,  0,$82,$3E,$3F,  2,  0,$8E,$40,$41,$42,$43,$44,$45
		.BYTE	$46,$47,$48,$49,$4A,$4B,$4C,$4D,  3,  0,$84,$4E,$4F,  0,$50,  7
		.BYTE	  0,$82,$51,$52,  2,  0,$96,$53,$54,$55,$56,$57,$58,$59,$5A,$5B
		.BYTE	$49,$5C,$5D,$5E,$5F,$60,$61,  0,$62,$63,$64,$65,$66,  6,  0,$8D
		.BYTE	$67,$68,$69,  0,$6A,$6B,$6C,$6D,$6E,$6F,$70,$71,$72,  2,$49,$8B
		.BYTE	$73,$74,$75,$76,$77,$78,$79,$7A,$7B,$7C,$7D,  6,  0,$81,$7E,  2
		.BYTE	  0,$98,$7F,$80,$81,$82,$83,$84,  0,$85,  0,$86,$87,$88,$89,$8A
		.BYTE	$8B,$8C,$8D,$8E,$8F,$90,  0,$91,$92,$93,  5,  0,$87,$94,$95,$96
		.BYTE	$97,$98,$99,$9A,  2,  0,$92,$9B,$9C,  0,$9D,$9E,$9F,$A0,$A1,$A2
		.BYTE	$A3,$A4,$A5,$A6,$A7,  0,$A8,$A9,$AA,  6,  0,$83,$AB,$AC,$AD,  2
		.BYTE	  0,$95,$AE,$AF,  0,$B0,$B1,  0,$B2,$B3,$B4,$B5,$B6,$B7,$B8,$B9
		.BYTE	$BA,$BB,$BC,$BD,$BE,$BF,$C0, $B,  0,$94,$C1,$C2,  0,$C3,$C4,$C5
		.BYTE	$C6,$C7,$C8,$C9,$CA,$CB,$CC,$CD,$CE,  0,$CF,$D0,$D1,$D2, $C,  0
		.BYTE	$8F,$D3,$D4,$D5,$D6,$D7,$D8,  0,$D9,$DA,$DB,$DC,$DD,$DE,$DF,$E0
		.BYTE	$11,  0,$8F,$E1,$E2,$E3,$E4,$E5,  0,$E6,$E7,$E8,$E9,$EA,$EB,$EC
		.BYTE	$ED,$EE,$11,  0,$88,$EF,$F0,$F1,$F2,$61,$F3,$F4,$F5,$18,  0,$88
		.BYTE	$F6,$F7,$F8,$F9,$FA,$FB,$FC,$FD,$7F,  0,$7F,  0,$FF,$FF
_spr2_title:
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$41,$43,$45,$47,$49,$4B,  0,$4D,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, $F,$11,$13,$15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$17,$19,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$1B,$1D,$1F,$21,$23,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	$FF,  0,  0,  0,$A5,$A7,$81,$A3,$A7,$FF,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$25,$27,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	$FF,  0,  0,  0,$85,$9D,$9B,$A7,$91,$9B,$A9,$89,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,$A5,$9D,$A9,$9B,$87,  0,$9D,$8B,$8B,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,$99,$A9,$A5,$91,$85,  0,$9D,$8B,$8B,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
_tlm5_lewis:
		.BYTE	$7F,  0,$28,  0,$81,$27,  7,  0,$82,$28,$29,  7,  0,$81,$27, $E
		.BYTE	  0,$81,$2A,  2,  0,$81,$2B,  2,$2C,$86,$2D,$2E,$2F,$30,$31,$32
		.BYTE	  2,$2C,$81,$33,  2,  0,$81,$2A,  8,  0,$87,$34,$35,$36,$37,$38
		.BYTE	$39,$3A,  3,$3B,$8A,$3C,$3D,$3E,$3F,$40,$41,$42,$43,$44,$45,  3
		.BYTE	$3B,$87,$3A,$46,$47,$48,$49,$4A,$4B,  2,  0,$81,$4C, $B,  0,$86
		.BYTE	$4D,$4E,$4F,$50,$51,$52, $B,  0,$81,$53,  2,  0,$81,$54, $B,  0
		.BYTE	$86,$55,$56,$57,$58,$59,$5A, $B,  0,$81,$5B,  2,  0,$81,$5C, $A
		.BYTE	  0,$87,$5D,$5E,$5F,$60,$61,$62,$63, $B,  0,$81,$64,  2,  0,$81
		.BYTE	$5C, $A,  0,$88,$65,$66,$67,$68,$69,$6A,$6B,$6C, $A,  0,$81,$64
		.BYTE	  2,  0,$81,$5C, $B,  0,$87,$6D,$6E,$6F,$70,$71,$72,$73, $A,  0
		.BYTE	$81,$64,  2,  0,$81,$74, $B,  0,$86,$75,$76,$77,$78,$79,$7A, $B
		.BYTE	  0,$84,$7B,  0,$7C,$7D, $B,  0,$86,$7E,$7F,$80,$81,$82,$83, $B
		.BYTE	  0,$84,$84,$85,$86,$87, $A,  0,$89,$88,$89,$8A,$8B,$8C,$8D,$8E
		.BYTE	$8F,$90,  9,  0,$84,$91,$92,  0,$93,  8,  0,$8E,$94,$95,$96,$97
		.BYTE	$98,$99,$9A,$9B,$9C,$9D,$9E,$9F,$A0,$A1,  6,  0,$81,$A2,  2,  0
		.BYTE	$81,$5C,  7,  0,$8F,$A3,$A4,$A5,$A6,$A7,$A8,$A9,$AA,$AB,$AC,$AD
		.BYTE	$AE,$AF,$B0,$B1,  6,  0,$81,$64,  2,  0,$81,$5C,$1C,  0,$81,$64
		.BYTE	  2,  0,$81,$5C,$1C,  0,$81,$64,  2,  0,$81,$B2,$1C,  0,$81,$B3
		.BYTE	  2,  0,$81,$B4,$1C,  0,$81,$B5,  2,  0,$84,$B6,$B7,$B8,$B9,  3
		.BYTE	$BA,$90,$BB,$BC,$BD,$BE,$BF,$C0,$C1,$C2,$C3,$C4,$C5,$C6,$C7,$C8
		.BYTE	$C9,$CA,  3,$CB,$84,$CC,$CD,$CE,$CF, $B,  0,$81,$D0,  2,$D1,$86
		.BYTE	$D2,$D3,$D4,$D5,$D6,$D7,  2,$D1,$81,$D8,$7F,  0,$7F,  0,$FF,$FF
_tlm6_robocop:
		.BYTE	$1A,  0,$26,  0,$8D,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,$5A,  0,$81,$27,  7,  0,$82,$28,$29,  7,  0,$81,$27, $E
		.BYTE	  0,$81,$2A,  2,  0,$81,$2B,  2,$2C,$86,$2D,$2E,$2F,$30,$31,$32
		.BYTE	  2,$2C,$81,$33,  2,  0,$81,$2A,  8,  0,$87,$34,$35,$36,$37,$38
		.BYTE	$39,$3A,  3,$3B,$8A,$3C,$3D,$3E,$3F,$40,$41,$42,$43,$44,$45,  3
		.BYTE	$3B,$87,$3A,$46,$47,$48,$49,$4A,$4B,  2,  0,$81,$4C,$1C,  0,$81
		.BYTE	$4D,  2,  0,$81,$4E,  8,  0,$85,$4F,$50,$51,$52,$53,  3,$54,$83
		.BYTE	$55,$56,$57,  9,  0,$81,$58,  2,  0,$81,$59,  8,  0,$87,$5A,$5B
		.BYTE	$5C,$5D,$5E,$5F,$60,  2,  0,$83,$5F,$61,$57,  8,  0,$81,$62,  2
		.BYTE	  0,$81,$59,  6,  0,$87,$63,$64,$65,$66,$67,$68,$69,  2,  0,$82
		.BYTE	$5F,$60,  2,  0,$81,$6A,  8,  0,$81,$62,  2,  0,$81,$59,  6,  0
		.BYTE	$89,$6B,$6C,$6D,$6E,$6F,$70,$71,$72,$73, $D,  0,$81,$62,  2,  0
		.BYTE	$81,$74,  6,  0,$8B,$75,$76,$77,$78,$79,$7A,$7B,$7C,$7D,$7E,$7F
		.BYTE	  2,  0,$81,$80,  8,  0,$84,$81,  0,$82,$83,  7,  0,$8D,$84,$85
		.BYTE	$86,$87,$88,$89,$8A,$8B,$8C,$8D,$8E,$8F,$90,  8,  0,$84,$91,$92
		.BYTE	$93,$94,  6,  0,$8E,$95,$96,$97,$98,$99,$9A,$9B,$9C,$9D,$9E,$9F
		.BYTE	$A0,$A1,$A2,  8,  0,$84,$A3,$A4,  0,$A5,  6,  0,$8E,$A6,$A7,$A8
		.BYTE	$A9,$AA,$AB,$AC,$AD,$AE,$AF,$B0,$B1,$B2,$B3,  8,  0,$81,$B4,  2
		.BYTE	  0,$81,$59,  7,  0,$8D,$B5,$B6,$B7,$B8,$B9,$BA,$BB,$BC,$BD,$BE
		.BYTE	$BF,$C0,$C1,  8,  0,$81,$62,  2,  0,$81,$59,$1C,  0,$81,$62,  2
		.BYTE	  0,$81,$59,$1C,  0,$81,$62,  2,  0,$81,$C2,$1C,  0,$81,$C3,  2
		.BYTE	  0,$81,$C4,$1C,  0,$81,$C5,  2,  0,$84,$C6,$C7,$C8,$C9,  3,$CA
		.BYTE	$90,$CB,$CC,$CD,$CE,$CF,$D0,$D1,$D2,$D3,$D4,$D5,$D6,$D7,$D8,$D9
		.BYTE	$DA,  3,$DB,$84,$DC,$DD,$DE,$DF, $B,  0,$81,$E0,  2,$E1,$86,$E2
		.BYTE	$E3,$E4,$E5,$E6,$E7,  2,$E1,$81,$E8,$7F,  0,$FF,$FF
_tlm7_ending:
		.BYTE	$40,  1,$81,  2,  2,  1,$89,  3,  4,  5,  6,  7,  8,  1,  9, $A
		.BYTE	$14,  1,$81, $B,  2,  1,$81, $C,  3, $D,$88, $E, $F,$10,$11,$12
		.BYTE	$13,$14,$15,  8,  1,$82,$16,$17,  7,  1,$84,$18,$19,  1,$1A,  3
		.BYTE	 $D,$88,$1B,$1C,  0,$1D,$1E,$1F,$20,$21,  7,  1,$83,$22,$23,$24
		.BYTE	  7,  1,$90,$25,$26,$27,$28,$29,$2A,$2B,$2C,$2D,  0,$2E,$2F,$30
		.BYTE	$31,$32,$33,  5,  1,$84,$34,$35,$36,$24,  7,  1,$85,$37,$38,$39
		.BYTE	  0,$3A,  2,  0,$85,$3B,  0,$3A,  0,$3B,  2,  0,$85,$3C,  0,$3D
		.BYTE	$3E,$3F,  2,  1,$85,$40,$41,$36,$42,$43,  6,  1,$90,$44,  0,$45
		.BYTE	$46,  0,$3A,$47,$48,$49,$4A,$47,$48,  0,$3A,$4B,$4C,  3,  0,$87
		.BYTE	$4D,$4E,$4F,$41,$36,$50,$51,  4,  1,$96,$52,$53,$54,$55,$56,  0
		.BYTE	$57,$58,$59,$5A,$5B,$5C,$5D,$5E,$5F,$60,$61,$62,$49,$4A,  0,$63
		.BYTE	  2,  0,$84,$41,$64,$50,$51,  3,$43,$B3,$65,$66,$67,  0,$68,  0
		.BYTE	$69,$6A,  0,$6B,$6C,$6D,$6E,$6F,$70,$71,$72,$73,$74,  0,$3A,  0
		.BYTE	$75,$76,$77,$78,$79,$50,$51,$7A,$7B,$7C,$7D,$7E,$7F,$80,$81,$82
		.BYTE	$83,$84,$85,$86,$87,$88,$89,$8A,$8B,$8C,$8D,$8E,$8F,  2,  0,$84
		.BYTE	$90,$91,$92,$93,  2,  0,$98,$94,$95,$96,  0,$97,$98,$99,$9A,  0
		.BYTE	$9B,$9C,$9D,$9E,$9F,$A0,$A1,$A2,$A3,$A4,$A5,$A6,$A7,$A8,$A9,  2
		.BYTE	  0,$DF,$AA,$AB,$AC,$AD,$AE,$AF,$B0,$B1,$B2,$B3,$B4,$B5,$B6,$B7
		.BYTE	$B8,$B9,$BA,$BB,$BC,$BD,$BE,$BF,$C0,$C1,$C2,$C3,$C4,$C5,$C6,$C7
		.BYTE	$C8,$C9,$CA,$CB,$CC,$CD,$CE,$CF,$D0,$D1,$D2,$D3,$D4,$D5,$D6,$D7
		.BYTE	$D8,$D9,$DA,$DB,$DC,$DD,$DE,$DF,$E0,$E1,$E2,$E3,$E4,$E5,$E6,$E7
		.BYTE	$E8,$E9,$EA,$EB,$EC,  0,$ED,$EE,$EF,$F0,$F1,$F2,$F3,$F4,$F5,$F6
		.BYTE	  1,  2,  3,  4,  5,  6,  7,  8,  9,  6, $A, $B, $C, $D,  6, $E
		.BYTE	 $F,  3,  0,$8F,$10,$11,$12,$13,$14,$15,$16,$17,$18,$19,$1A,$1B
		.BYTE	$1C,$1D,$1E,  4,  6,$81,$1F,  6,  6,$95,$20,$21,$22,$23,$24,$25
		.BYTE	$26,$27,$28,$29,$2A,$2B,$2C,  0,$2D,$2E,$2F,$30,$31,$32,$33, $C
		.BYTE	  6,$8A,$34,$35,$36,$37,$38,$39,$3A,$3B,$3C,$3D,  5,  0,$86,$3E
		.BYTE	$3F,$40,$41,$42,$43,  5,  6,$82,$44,$45,  3,  6,$87,$46,$47,$48
		.BYTE	$49,$4A,$4B,$4C,  6,  0,$8D,$4D,$4E,$4F,$50,$51,$52,$53,$54,$55
		.BYTE	  6,$56,$57,$58,  2,  6,$AA,$59,$5A,  6,$5B,$5C,$5D,$5E,$5F,$60
		.BYTE	$61,$62,$63,$64,$65,$66,$67,$68,$69,$6A,$6B,$6C,$6D,$6E,$6F,$70
		.BYTE	$71,$72,$73,$74,$75,$76,$77,$78,$79,$7A,$7B,$7C,$7D,$7E,$7F,  0
		.BYTE	$80,  5,  0,$82,$81,$82,  3,  0,$96,$83,$84,$85,$86,$87,$88,$89
		.BYTE	$8A,$8B,$8C,$8D,$8E,$8F,$90,$91,$92,$93,  0,$94,$95,$96,$97,  9
		.BYTE	  0,$97,$98,$99,$9A,$9B,$9C,$9D,$9E,$9F,$A0,$A1,$A2,$A3,$A4,$A5
		.BYTE	$A6,$A7,$A8,  0,$A9,$AA,$AB,$AC,$AD,  6,  0,$A5,$AE,$AF,$B0,$B1
		.BYTE	$B2,$B3,$B4,  0,$B5,$B6,$B7,$B8,$B9,$BA,$BB,$BC,$BD,$BE,$BF,$C0
		.BYTE	$C1,  0,$C2,$C3,$C4,$C5,$C6,  0,$C7,$C8,$C9,$CA,$CB,$CC,$CD,$CE
		.BYTE	$CF,  3,  0,$9E,$9B,$D0,$D1,$D2,$D3,$D4,$D5,$D6,$D7,$D8,$D9,$DA
		.BYTE	$DB,  0,$DC,$DD,$DE,$DF,$E0,$E1,$E2,$E3,$E4,$E5,$E6,$E7,$E8,$E9
		.BYTE	$EA,$EB,  2,  0,$90,$B4,$EC,$ED,$EE,$ED,$EE,  0,$EF,  0,$F0,$F1
		.BYTE	$F2,$F3,  0,$F4,$F5,  2,  0,$8C,$F6,$F7,$F8,$F9,$FA,$FB,$FC,$FD
		.BYTE	$FE,$FE,$FE,$FE,$7F,  0,$7F,  0,$FF,$FF
_tlm3_hiscore:
		.BYTE	$7F,  0,$28,  0,$81,$27,  7,  0,$82,$28,$29,  7,  0,$81,$27, $E
		.BYTE	  0,$81,$2A,  2,  0,$81,$2B,  2,$2C,$86,$2D,$2E,$2F,$30,$31,$32
		.BYTE	  2,$2C,$81,$33,  2,  0,$81,$2A,  8,  0,$87,$34,$35,$36,$37,$38
		.BYTE	$39,$3A,  3,$3B,$8A,$3C,$3D,$3E,$3F,$40,$41,$42,$43,$44,$45,  3
		.BYTE	$3B,$87,$3A,$46,$47,$48,$49,$4A,$4B,  2,  0,$81,$4C,  9,  0,$8A
		.BYTE	  8,  9,  7,  8,$13,  3, $F,$12,  5,$13,  9,  0,$81,$53,  2,  0
		.BYTE	$81,$54,$1C,  0,$81,$5B,  2,  0,$81,$5C,  2,  0,$83,$1B,$24,  0
		.BYTE	  7, $F,  3,  0,$84,  7,$24,  8,  1,  2,$12,$84,  9,$13, $F, $E
		.BYTE	  3,  0,$81,$64,  2,  0,$81,$5C,$1C,  0,$81,$64,  2,  0,$81,$5C
		.BYTE	  2,  0,$83,$1C,$24,  0,  7, $F,  3,  0,$87,$13,$24, $E,  9,  3
		.BYTE	 $F, $C,  6,  0,$81,$64,  2,  0,$81,$74,$1C,  0,$84,$7B,  0,$7C
		.BYTE	$7D,  2,  0,$83,$1D,$24,  0,  7, $F,  3,  0,$85, $A,$24,  2, $C
		.BYTE	 $F,  2,  7,$81,$13,  5,  0,$84,$84,$85,$86,$87,$1C,  0,$84,$91
		.BYTE	$92,  0,$93,  2,  0,$83,$1E,$24,  0,  7, $F,  3,  0,$85, $A,$24
		.BYTE	  2, $C, $F,  2,  7,$81,$13,  5,  0,$81,$A2,  2,  0,$81,$5C,$1C
		.BYTE	  0,$81,$64,  2,  0,$81,$5C,  2,  0,$83,$1F,$24,  0,  7, $F,  3
		.BYTE	  0,$85, $A,$24,  2, $C, $F,  2,  7,$81,$13,  5,  0,$81,$64,  2
		.BYTE	  0,$81,$5C,$1C,  0,$81,$64,  2,  0,$81,$B2,  2,  0,$83,$20,$24
		.BYTE	  0,  7, $F,  3,  0,$85, $A,$24,  2, $C, $F,  2,  7,$81,$13,  5
		.BYTE	  0,$81,$B3,  2,  0,$81,$B4,$1C,  0,$81,$B5,  2,  0,$84,$B6,$B7
		.BYTE	$B8,$B9,  3,$BA,$90,$BB,$BC,$BD,$BE,$BF,$C0,$C1,$C2,$C3,$C4,$C5
		.BYTE	$C6,$C7,$C8,$C9,$CA,  3,$CB,$84,$CC,$CD,$CE,$CF, $B,  0,$81,$D0
		.BYTE	  2,$D1,$86,$D2,$D3,$D4,$D5,$D6,$D7,  2,$D1,$81,$D8,$7F,  0,$7F
		.BYTE	  0,$FF,$FF
_tlm4_frame:
		.BYTE	$7F,  0,$28,  0,$81,$27,  7,  0,$82,$28,$29,  7,  0,$81,$27, $E
		.BYTE	  0,$81,$2A,  2,  0,$81,$2B,  2,$2C,$86,$2D,$2E,$2F,$30,$31,$32
		.BYTE	  2,$2C,$81,$33,  2,  0,$81,$2A,  8,  0,$87,$34,$35,$36,$37,$38
		.BYTE	$39,$3A,  3,$3B,$8A,$3C,$3D,$3E,$3F,$40,$41,$42,$43,$44,$45,  3
		.BYTE	$3B,$87,$3A,$46,$47,$48,$49,$4A,$4B,  2,  0,$81,$4C,$1C,  0,$81
		.BYTE	$53,  2,  0,$81,$54,$1C,  0,$81,$5B,  2,  0,$81,$5C,$1C,  0,$81
		.BYTE	$64,  2,  0,$81,$5C,$1C,  0,$81,$64,  2,  0,$81,$5C,$1C,  0,$81
		.BYTE	$64,  2,  0,$81,$74,$1C,  0,$84,$7B,  0,$7C,$7D,$1C,  0,$84,$84
		.BYTE	$85,$86,$87,$1C,  0,$84,$91,$92,  0,$93,$1C,  0,$81,$A2,  2,  0
		.BYTE	$81,$5C,$1C,  0,$81,$64,  2,  0,$81,$5C,$1C,  0,$81,$64,  2,  0
		.BYTE	$81,$5C,$1C,  0,$81,$64,  2,  0,$81,$B2,$1C,  0,$81,$B3,  2,  0
		.BYTE	$81,$B4,$1C,  0,$81,$B5,  2,  0,$84,$B6,$B7,$B8,$B9,  3,$BA,$90
		.BYTE	$BB,$BC,$BD,$BE,$BF,$C0,$C1,$C2,$C3,$C4,$C5,$C6,$C7,$C8,$C9,$CA
		.BYTE	  3,$CB,$84,$CC,$CD,$CE,$CF, $B,  0,$81,$D0,  2,$D1,$86,$D2,$D3
		.BYTE	$D4,$D5,$D6,$D7,  2,$D1,$81,$D8,$7F,  0,$7F,  0,$FF,$FF
_tlm1_probe:
		.BYTE	$7F,  0,$7F,  0,$2B,  0,$8F,$9C,$9D,$9E,$9F,$A0,$A1,$A2,$A3,$A4
		.BYTE	$A5,$A6,$A7,$A8,$A9,$AA,$11,  0,$8F,$AB,$AC,$AD,$AE,$AF,$B0,$B1
		.BYTE	$B2,$B3,$B4,$B5,$B6,$B7,$B8,$B9,$11,  0,$8F,$BA,$BB,$BC,$BD,$BE
		.BYTE	$BF,$C0,$C1,$C2,$C3,$C4,$C5,$C6,$C7,$C8,$11,  0,$8F,$AB,$C9,$CA
		.BYTE	$CB,$CC,$CD,$CE,$CF,$D0,$D1,$D2,$D3,$D4,$D5,$D6,$11,  0,$8F,$D7
		.BYTE	$D8,$D9,$DA,$DB,$DC,$DD,$DE,$DF,$E0,$E1,$E2,$E3,$E4,$E5,$11,  0
		.BYTE	$8F,$E6,$E7,$E8,$E9,$EA,$EB,$EC,$ED,$EE,$EF,$F0,$F1,$F2,$F3,$F4
		.BYTE	$7F,  0,$7F,  0,$7F,  0,$7F,  0,$FF,$FF
_tlm8_repair:
		.BYTE	 $A,  0,$81,  1,  3,  0,$81,  2,  3,  0,$83,  3,  4,  5,  5,  0
		.BYTE	$86,  6,  7,  0,  8,  9, $A,  5,  0,$81,  2,  4,  0,$81, $B,  3
		.BYTE	  0,$81, $C,  3,  0,$84, $D,  0, $E, $F,  2,  0,$81,$10,  3,  0
		.BYTE	$84,$11,$12,$13,$14,  5,  0,$81, $C,  2,  0,$87,$15,$16,$17,$18
		.BYTE	$19,  0, $C,  2,  0,$85,$1A,$1B,$1C,$1D,$1E,  2,  0,$82,$1F,$20
		.BYTE	 $B,  0,$8A, $C,  0,$21,$22,  0,  7,$23,$24,$25, $C,  2,  0,$87
		.BYTE	$26,$27,$28,$29,$2A,$2B,$2C, $D,  0,$8A, $C,  0,  6,  0,  7,  0
		.BYTE	  8,  9, $A, $C,  2,  0,$8D,$2D,$2E,$2F,$30,$31,$32,$33,$34,  0
		.BYTE	$35,$1F,$20,$36,  6,  0,$83,$10, $C,$10,  3,  0,$92,$11,$12,$13
		.BYTE	$14,$37,  0,$38,$39,$3A,$3B,$3C,$3D,$3E,$3F,$40,$41,$42,$36,  2
		.BYTE	  0,$82,$43,$36,  4,  0,$82,$10, $C,  3,$10,  7,  0,$8E,$44,$45
		.BYTE	$46,$47,$48,$49,$4A,$4B,$4C,  0,$4D,  0,$43,$36,  5,  0,  2,$10
		.BYTE	$81, $C,  2,$10,  2,  0,$83,$35,$1F,$20,  3,  0,$8B,$4E,$4F,$50
		.BYTE	$51,$52,$53,$54,$55,$56,  0,$57,  7,  0,  2,$10,$82,  0,$37, $A
		.BYTE	  0,$8A,$58,$59,$5A,$5B,$5C,$5D,$5E,$5F,$60,$61,  7,  0,  2,$10
		.BYTE	  9,  0,$90,$35,$1F,$20,  0,$62,$63,$64,$65,$66,$67,$68,$69,  0
		.BYTE	$6A,$6B,$6C,  5,  0,$8B,$35,$1F,$20,  0,$6D,$6E,$6D,$6E,  0,$1F
		.BYTE	$20,  4,  0,$8D,$6F,$70,$71,$72,$73,$74,$75,$76,$77,$78,$79,$7A
		.BYTE	$7B,  8,  0,$84,  1,  2,  1,  2,  7,  0,$8A,  3,  4,  5,  6,  7
		.BYTE	  8,  9, $A, $B, $C,  2,  0,$84, $D, $E, $F,$10, $D,$11,$81,$12
		.BYTE	  3,  0,$8F,$13,$14,$15,$16,$17,$18,$19,$1A,$1B,$1C,$1D,$1E,$1F
		.BYTE	$20,$21, $C,$22,$82,$23,$24,  4,  0,$8C,$25,$26,$27,$28,$29,  0
		.BYTE	$2A,$2B,$2C,$2D,$2E,$2F,  2,  0, $A,$30,$84,$31,$32,$33,$24,  2
		.BYTE	  0,$81,$34,  2,  0,$8A,$35,$36,$37,$38,  0,$39,$3A,$3B,$3C,$3D
		.BYTE	  3,  0,$87,$3E,$3F,$40,$41,$42,$43,$44,  4,$45,$83,$46,$33,$24
		.BYTE	  2,  0,$82,$47,$48,  2,  0,$86,$49,$4A,$4B,  0,$4C,$4D,  6,  0
		.BYTE	$85,$45,$4E,$4F,$50,$51,  6,$45,$83,$52,$33,$24,  2,  0,$8B,$53
		.BYTE	$54,  0,$55,$56,$57,$58,  0,$59,$5A,$5B,  5,  0,  4,$45,$81,$51
		.BYTE	  6,$45,$83,$52,$33,$24,  5,  0,$84,$55,  0,$5C,$5D,  2,  0,$83
		.BYTE	$5E,$5F,$60,  4,  0, $B,$45,$83,$52,$33,$24,  5,  0,$81,$55,  2
		.BYTE	  0,$82,$63,$64,  2,  0,$82,$65,$3D,  4,  0,  8,$61,$81,$62,  2
		.BYTE	$45,$83,$52,$33,$24,  7,  0,$84,$67,$68,$69,$6A, $F,  0,$81,$66
		.BYTE	  2,$45,$83,$52,$33,$24,  7,  0,$84,$6C,  0,$6D,$6E, $F,  0,$81
		.BYTE	$6B,  2,$45,$83,$52,$33,$24, $A,  0,$81,$6F, $F,  0,$81,$6B,  2
		.BYTE	$45,$83,$52,$33,$24,$1A,  0,$81,$6B,  2,$45,$83,$52,$33,$24,$1A
		.BYTE	  0,$81,$6B,  2,$45,$83,$52,$33,$24,$1A,  0,$81,$66,  2,$45,$83
		.BYTE	$52,$33,$24,$7F,  0,$7F,  0,$FF,$FF
_spr8_repair:
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,$81,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	$82,$83,  0,$44,$45,  0,  0,$86,$87,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	$88,$89,  0,  0,  0,  0,  0, $A,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	$8B,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,$8C,$8D,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,$8E,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,$8F,$90,$91,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$92,$93,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$94,$95,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$96,$97,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	$98,  0,  0,  0,  0,  0,$99,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	$9A,  0,  0,  0,  0,  0,$9B,$9C,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,$9D,$9E,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,$9F,$A0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,$A1,$A2,$A3,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,$A4,$A5,$A6,$A7,$A8,$A9,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,$AA,$AB,$AC,$AD,$AE,$AF,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,$B0,  0,$B1,$B2,$B3,$B4,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,$B5,$B6,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,$B7,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
_tlm_repair_head_lib:
		.BYTE	  3,  4,  5,  0
		.BYTE	 $D,  0, $E, $F
		.BYTE	$1B,$1C,$1D,$1E
		.BYTE	$27,$28,$29,$2A
		.BYTE	  3,$7C,$7D,  0
		.BYTE	 $D,  0,$7E,$7F
		.BYTE	$1B,$1C,$80,$1E
		.BYTE	$27,$28,$29,$2A
		.BYTE	  3,$81,$82,$83
		.BYTE	$84,$85,$86,$7F
		.BYTE	$1B,$1C,$87,$1E
		.BYTE	$27,$28,$29,$2A
_tlm_repair_larm_lib0:
		.BYTE	$38,$39,$3A
		.BYTE	$44,$45,$46
		.BYTE	$4E,$4F,$50
		.BYTE	$58,$59,$5A
		.BYTE	$62,$63,$64
		.BYTE	$6F,$70,$71
		.BYTE	$38,$39,$3A
		.BYTE	$88,$89,$46
		.BYTE	$8A,$8B,$8C
		.BYTE	$8D,$59,$5A
		.BYTE	$62,$8E,$8F
		.BYTE	$6F,$70,$71
		.BYTE	$38,$39,$3A
		.BYTE	$88,$90,$91
		.BYTE	$92,$93,$94
		.BYTE	$95,$96,$5A
		.BYTE	$97,$98,$8F
		.BYTE	$6F,$99,$71
_tlm_repair_larm_lib1:
		.BYTE	$2C
		.BYTE	$33
		.BYTE	$3F
		.BYTE	$4B
		.BYTE	$55
		.BYTE	$5F
		.BYTE	$2C
		.BYTE	$9A
		.BYTE	$3F
		.BYTE	$9B
		.BYTE	$9C
		.BYTE	$9D
		.BYTE	$2C
		.BYTE	$9E
		.BYTE	$9F
		.BYTE	$A0
		.BYTE	$A1
		.BYTE	$9D
_tlm_repair_body_lib:
		.BYTE	$3B,$3C,$3D,$3E
		.BYTE	$47,$48,$49,$4A
		.BYTE	$51,$52,$53,$54
		.BYTE	$5B,$5C,$5D,$5E
		.BYTE	$3B,$3C,$A2,$A3
		.BYTE	$A4,$A5,$A6,$A7
		.BYTE	$A8,$A9,$AA,$54
		.BYTE	$AB,$AC,$AD,$5E
		.BYTE	$3B,$3C,$A2,$AE
		.BYTE	$AF,$B0,$B1,$B2
		.BYTE	$B3,$B4,$B5,$B6
		.BYTE	$AB,$AC,$B7,$5E
_tlm_repair_legs_lib:
		.BYTE	  5,  6,  7,  8,  9,  $A, $B,  $C
		.BYTE	$14,$15,$16,$17,$18,$19,$1A,$1B
		.BYTE	$25,$26,$27,$28,$29,  0,$2A,$2B
		.BYTE	  0,$35,$36,$37,$38,  0,$39,$3A
		.BYTE	  0,  0,$49,$4A,$4B,  0,$4C,$4D
		.BYTE	  0,  0,$56,$57,$58,  0,$59,$5A
		.BYTE	  5,$70,$71,  8,  9,$72,$73,$74
		.BYTE	$14,$75,$76,$77,$18,$19,$1A,$1B
		.BYTE	$25,$26,$78,$79,$29,  0,$2A,$7A
		.BYTE	  0,$35,$36,$37,$38,  0,$39,$7B
		.BYTE	  0,  0,$49,$7C,$7D,  0,$4C,$7E
		.BYTE	  0,  0,$56,$57,$58,  0,$59,$5A
		.BYTE	  5,$70,$7F,  8,$80,$72,$81,$74
		.BYTE	$14,$75,$82,$83,$84,$85,$1A,$86
		.BYTE	$25,$26,$87,$88,$29,  0,$2A,$89
		.BYTE	  0,$35,$8A,$8B,$8C,  0,$8D,$8E
		.BYTE	  0,  0,$49,$8F,$90,  0,$4C,$91
		.BYTE	  0,  0,$56,$92,$93,  0,$59,$5A
_tlm_repair_rarm_lib_unused:
		.BYTE	$60,$61,  0,  0
		.BYTE	  0,$6A,$6B,$6C
		.BYTE	$77,$78,$79,$7A
		.BYTE	  0,  0,  0,  0
		.BYTE	$B8,$B9,  0,  0
		.BYTE	  0,$BA,$6B,$6C
		.BYTE	$77,$78,$79,$7A
		.BYTE	  0,  0,  0,  0
		.BYTE	$B8,$B9,  0,  0
		.BYTE	  0,$BB,$BC,$6C
		.BYTE	$77,$78,$BD,$7A
		.BYTE	  0,  0,  0,  0
byte_476DC:
		.BYTE	$30,$90,$60,$90,  0
_repair_tlm0:
		.BYTE	  5,  6,  7,  8,  9, $A, $B, $C
		.BYTE	$14,$15,$16,$17,$18,$19,$1A,$1B
		.BYTE	$25,$26,$27,$28,$29,  0,$2A,$2B
		.BYTE	  0,$35,$36,$37,$38,  0,$39,$3A
		.BYTE	  0,  0,$49,$4A,$4B,  0,$4C,$4D
		.BYTE	  0,  0,$56,$57,$58,  0,$59,$5A
_repair_tlm1:
		.BYTE	  0,  0,$94,$95,$96,  0,  0,  0
		.BYTE	  0,  0,$97,  0,$98,$99,  0,  0
		.BYTE	  0,$9A,$9B,$9C,$9D,$9E,  0,  0
		.BYTE	  0,$9F,$A0,$A1,$A2,$A3,$A4,$A5
		.BYTE	  0,$A6,$A7,$A8,$A9,$AA,$AB,$AC
		.BYTE	$AD,$AE,$AF,$B0,$B1,$B2,$B3,$B4
_repair_tlm2:
		.BYTE	$A6,$A7,$A8,$A9,$AA,$AB,$AC,  0
		.BYTE	$AE,$AF,$B0,$B1,$B2,$B3,$B4,  0
		.BYTE	$B5,$B6,$B7,$B8,$B9,$BA,$BB,$BC
		.BYTE	$BD,$BE,$BF,$C0,$C1,$C2,$C3,$C4
		.BYTE	$C5,$C6,$C7,$C8,$C9,$CA,$CB,$CC
		.BYTE	$CD,$CE,$CF,$D0,$D1,$D2,$D3,  0
_repair_tlm3:
		.BYTE	$D4,$B5,$B6,  0,$AC,  0,  0,  0
		.BYTE	$D5,$BD,$BE,  0,$BB,  0,$D6,  0
		.BYTE	$D7,$C5,$C6,  0,$C3,$D8,  0,  0
		.BYTE	$D9,$CD,$CE,  0,$CB,$CC,$DA,  0
		.BYTE	$DB,$DC,$DD,$DE,  0,  0,$DF,$E0
		.BYTE	  3,  4,  5,  0,$E1,  0,$E2,$E3
byte_477A1:
		.BYTE	$45,$F1,$F2,$F3
byte_477A5:
		.BYTE	$F4,$F5,$F6,$F7
byte_477A9:
		.BYTE	$45,$F8,$F9,$FA
byte_477AD:
		.BYTE	$FB,$FC,$FD,$FE

; =============== S U B	R O U T	I N E =======================================
_call_j_far_apu_play:
		JSR		_j_far_apu_play
		RTS

; =============== S U B	R O U T	I N E =======================================
_asci_bios_msg_display:
		STA		byte_3B4
		ASL
		TAX
		LDA		_asci_msg_lib,X
		STA		_text_data_tmp_ptr
		LDA		_asci_msg_lib+1,X
		STA		_text_data_tmp_ptr+1
		LDA		#0
		STA		_event_idx
		STA		byte_3B5
		STA		byte_3D7
		STA		_PPU_CTRL
		STA		_PPU_MASK
		STA		_delay_counter
		LDA		#8
		STA		byte_3B2
		LDA		#1
		STA		byte_3D3
		LDA		#7
		STA		_event_reload_idx
		LDA		#$FF
		STA		byte_3B1
		JSR		_spr_buf_clear
		JSR		_log_msg_buf_clear
		LDA		#4
		STA		_screen_idx
		JSR		_screen_display
		LDX		#0
		STX		_PPU_CTRL
		STX		_PPU_MASK
		BIT		_PPU_STATUS
		LDA		#$21
		STA		_PPU_ADDR
		LDA		#$23
		STA		_PPU_ADDR
		JSR		_asci_rom_bios_hdr_display
		BIT		_PPU_STATUS
		LDA		#$21
		STA		_PPU_ADDR
		LDA		#$43
		STA		_PPU_ADDR
		JSR		_asci_rom_bios_hdr_display
		BIT		_PPU_STATUS
		LDA		#$21
		STA		_PPU_ADDR
		LDA		#$63
		STA		_PPU_ADDR
		JSR		_asci_rom_bios_hdr_display
		LDA		#$80
		STA		_PPU_CTRL
		JMP		_event_exec

; =============== S U B	R O U T	I N E =======================================
_asci_rom_bios_hdr_display:
		LDA		_asci_rom_bios_hdr,X
		CMP		#$FE
		BEQ		loc_4784A
		SEC
		SBC		#$40
		STA		_PPU_DATA
		INX
		BNE		_asci_rom_bios_hdr_display
loc_4784A:
		INX
		RTS
_asci_msg_lib:
		.WORD _asci_msg0_lvl0_briefing
		.WORD _asci_msg1_lvl1_briefing
		.WORD _asci_msg2_lvl2_briefing
		.WORD _asci_msg3_lvl3_briefing
		.WORD _asci_msg4_lvl4_briefing
_asci_copyrights_ofs:
		.WORD _asci_copyrights
		.WORD _asci_prepare_to_repair
		.WORD _asci_confirm_repair
		.WORD _asci_get_out_of_there
		.WORD _asci_dummy
_asci_rom_bios_hdr:
		.BYTE	$52,$4F,$4D,$40,$42,$49,$4F,$53,$FE
		.BYTE	$44,$41,$54,$41,$40,$52,$45,$54,$52,$49,$45,$56,$41,$4C,$40,$53
		.BYTE	$59,$53,$54,$45,$4D,$FE
		.BYTE	$56,$45,$52,$53,$49,$4F,$4E,$40,$5D,$64,$4F,$FE
_asci_msg0_lvl0_briefing:
		.BYTE	$43,$41,$44,$49,$4C,$4C,$41,$43,$40,$48,$45,$49,$47,$48,$54,$53
		.BYTE	$66,$40,$44,$55,$53,$4B,$64,$FE,$FC,$FE,$52,$45,$53,$43,$55,$45
		.BYTE	$40,$59,$4F,$55,$52,$40,$54,$52,$41,$50,$50,$45,$44,$FE,$43,$4F
		.BYTE	$4C,$4C,$45,$41,$47,$55,$45,$53,$40,$46,$52,$4F,$4D,$40,$54,$48
		.BYTE	$45,$FE,$43,$4C,$55,$54,$43,$48,$45,$53,$40,$4F,$46,$40,$54,$48
		.BYTE	$45,$FE,$45,$56,$49,$4C,$40,$53,$50,$4C,$41,$54,$54,$45,$52,$50
		.BYTE	$55,$4E,$4B,$53,$64,$64,$FC,$FE,$FE,$54,$48,$49,$4E,$47,$53,$40
		.BYTE	$4D,$41,$59,$40,$47,$4F,$40,$4F,$4E,$FE,$42,$45,$48,$49,$4E,$44
		.BYTE	$40,$43,$4C,$4F,$53,$45,$44,$40,$44,$4F,$4F,$52,$53,$64,$64,$64
		.BYTE	$FE,$FC,$FA
_asci_msg1_lvl1_briefing:
		.BYTE	$52,$4F,$43,$4B,$45,$54,$40,$4D,$4F,$54,$4F,$52,$53,$40,$46,$41
		.BYTE	$43,$54,$4F,$52,$59,$64,$FE,$FC,$FE,$4E,$45,$47,$4F,$54,$49,$41
		.BYTE	$54,$45,$40,$54,$48,$45,$FE,$50,$45,$52,$49,$4C,$4F,$55,$53,$40
		.BYTE	$4F,$42,$53,$54,$41,$43,$4C,$45,$53,$FE,$49,$4E,$40,$54,$48,$45
		.BYTE	$40,$4F,$4C,$44,$40,$41,$42,$41,$4E,$44,$4F,$4E,$45,$44,$FE,$46
		.BYTE	$41,$43,$54,$4F,$52,$59,$64,$FC,$FE,$FE,$41,$4E,$44,$40,$4D,$41
		.BYTE	$4B,$45,$40,$59,$4F,$55,$52,$40,$57,$41,$59,$40,$54,$4F,$40,$54
		.BYTE	$48,$45,$FE,$55,$4C,$54,$49,$4D,$41,$54,$45,$40,$43,$4F,$4E,$46
		.BYTE	$4C,$49,$43,$54,$64,$64,$64,$FE,$FC,$FA
_asci_msg2_lvl2_briefing:
		.BYTE	$43,$41,$44,$49,$4C,$4C,$41,$43,$40,$48,$45,$49,$47,$48,$54,$53
		.BYTE	$66,$40,$44,$41,$59,$64,$FE,$FC,$FE,$48,$41,$56,$49,$4E,$47,$40
		.BYTE	$44,$4F,$4E,$4E,$45,$44,$40,$59,$4F,$55,$52,$FE,$50,$4F,$57,$45
		.BYTE	$52,$46,$55,$4C,$40,$4A,$45,$54,$40,$50,$41,$43,$4B,$66,$FE,$59
		.BYTE	$4F,$55,$40,$53,$54,$4F,$52,$4D,$40,$54,$48,$52,$4F,$55,$47,$48
		.BYTE	$40,$54,$48,$45,$FE,$57,$52,$45,$43,$4B,$45,$44,$40,$53,$54,$52
		.BYTE	$45,$45,$54,$53,$64,$64,$64,$FC,$FE,$FE,$54,$4F,$57,$41,$52,$44
		.BYTE	$40,$54,$48,$45,$40,$52,$45,$48,$41,$42,$53,$FE,$53,$55,$50,$45
		.BYTE	$52,$40,$54,$41,$4E,$4B,$64,$64,$64,$FE,$FC,$FE,$54,$48,$45,$40
		.BYTE	$4D,$41,$49,$4E,$40,$47,$55,$4E,$40,$49,$53,$FE,$41,$49,$4D,$45
		.BYTE	$44,$40,$41,$54,$40,$49,$4E,$4E,$4F,$43,$45,$4E,$54,$FE,$43,$49
		.BYTE	$54,$49,$5A,$45,$4E,$53,$64,$64,$64,$40,$41,$4E,$44,$40,$59,$4F
		.BYTE	$55,$64,$FE,$FC,$FA
_asci_msg3_lvl3_briefing:
		.BYTE	$57,$41,$4C,$4B,$40,$42,$41,$43,$4B,$FE,$54,$4F,$40,$54,$48,$45
		.BYTE	$40,$4F,$43,$50,$40,$54,$4F,$57,$45,$52,$64,$FE,$FC,$FE,$FE,$59
		.BYTE	$4F,$55,$52,$40,$4A,$45,$54,$40,$50,$41,$43,$4B,$40,$49,$53,$40
		.BYTE	$45,$4D,$50,$54,$59,$64,$64,$64,$FE,$FC,$FE,$4D,$41,$4B,$45,$40
		.BYTE	$59,$4F,$55,$52,$40,$57,$41,$59,$40,$46,$52,$4F,$4D,$FE,$4F,$4E
		.BYTE	$45,$40,$44,$49,$53,$54,$41,$4E,$54,$40,$45,$4E,$44,$40,$4F,$46
		.BYTE	$40,$54,$4F,$57,$4E,$FE,$42,$41,$43,$4B,$40,$54,$4F,$40,$54,$41
		.BYTE	$4B,$45,$40,$4F,$4E,$40,$4D,$43,$64,$44,$41,$47,$47,$45,$54,$54
		.BYTE	$FE,$FC,$FE,$FE,$52,$45,$53,$43,$55,$45,$40,$4D,$41,$52,$49,$45
		.BYTE	$40,$41,$4E,$44,$40,$4B,$45,$49,$4B,$4F,$FE,$FC,$FE,$49,$54,$40
		.BYTE	$49,$53,$40,$4E,$4F,$54,$40,$47,$4F,$49,$4E,$47,$40,$54,$4F,$40
		.BYTE	$42,$45,$FE,$45,$41,$53,$59,$64,$FE,$FC,$FA
_asci_msg4_lvl4_briefing:
		.BYTE	$54,$48,$45,$40,$54,$4F,$50,$40,$4F,$46,$40,$54,$48,$45,$40,$4F
		.BYTE	$43,$50,$40,$54,$4F,$57,$45,$52,$64,$FE,$FC,$FE,$55,$53,$45,$40
		.BYTE	$59,$4F,$55,$52,$40,$41,$43,$43,$45,$53,$53,$40,$53,$50,$49,$4B
		.BYTE	$45,$FE,$54,$4F,$40,$54,$41,$50,$40,$49,$4E,$54,$4F,$40,$54,$48
		.BYTE	$45,$40,$4B,$41,$4E,$45,$4D,$49,$54,$53,$55,$FE,$43,$4F,$4D,$50
		.BYTE	$55,$54,$45,$52,$64,$FE,$FC,$FE,$FE,$49,$54,$40,$50,$52,$4F,$56
		.BYTE	$49,$44,$45,$53,$FE,$4B,$45,$49,$4B,$4F,$40,$57,$49,$54,$48,$40
		.BYTE	$49,$4E,$46,$4F,$52,$4D,$41,$54,$49,$4F,$4E,$FE,$56,$49,$54,$41
		.BYTE	$4C,$40,$54,$4F,$40,$59,$4F,$55,$52,$40,$53,$41,$46,$45,$54,$59
		.BYTE	$64,$64,$64,$FE,$FC,$FE,$FE,$4C,$4F,$4F,$4B,$40,$42,$4F,$54,$48
		.BYTE	$40,$57,$41,$59,$53,$64,$64,$64,$FE,$FC,$FA
_asci_dummy:
		.BYTE	$FA
_asci_copyrights:
		.BYTE	$40,$40,$40,$40,$40,$40,$40,$40,$40,$52,$4F,$42,$4F,$43,$4F,$50
		.BYTE	$65,$40,$5D,$40,$FE,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40
		.BYTE	$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40
		.BYTE	$40,$FE,$40,$40,$40,$40,$54,$4D,$40,$41,$4E,$44,$40,$43,$4F,$50
		.BYTE	$59,$52,$49,$47,$48,$54,$40,$5B,$63,$63,$5C,$40,$FE,$40,$4F,$52
		.BYTE	$49,$4F,$4E,$40,$50,$49,$43,$54,$55,$52,$45,$53,$40,$43,$4F,$52
		.BYTE	$50,$4F,$52,$41,$54,$49,$4F,$4E,$FE,$40,$40,$40,$40,$40,$41,$4C
		.BYTE	$4C,$40,$52,$49,$47,$48,$54,$53,$40,$52,$45,$53,$45,$52,$56,$45
		.BYTE	$44,$40,$40,$40,$FE,$54,$4D,$40,$44,$45,$53,$49,$47,$4E,$41,$54
		.BYTE	$45,$53,$40,$41,$40,$54,$52,$41,$44,$45,$4D,$41,$52,$4B,$40,$4F
		.BYTE	$46,$FE,$40,$4F,$52,$49,$4F,$4E,$40,$50,$49,$43,$54,$55,$52,$45
		.BYTE	$53,$40,$43,$4F,$52,$50,$4F,$52,$41,$54,$49,$4F,$4E,$FE,$FE,$40
		.BYTE	$40,$40,$40,$40,$40,$40,$43,$4F,$50,$59,$52,$49,$47,$48,$54,$40
		.BYTE	$5B,$63,$63,$5C,$FE,$40,$40,$40,$4F,$43,$45,$41,$4E,$40,$4F,$46
		.BYTE	$40,$41,$4D,$45,$52,$49,$43,$41,$66,$40,$49,$4E,$43,$64,$FE,$40
		.BYTE	$40,$40,$40,$40,$40,$40,$40,$4C,$49,$43,$45,$4E,$53,$45,$44,$40
		.BYTE	$42,$59,$FE,$40,$40,$40,$40,$40,$40,$40,$40,$40,$4E,$49,$4E,$54
		.BYTE	$45,$4E,$44,$4F,$64,$40,$40,$40,$40,$40,$40,$40,$40,$40,$FA
_asci_prepare_to_repair:
		.BYTE	$57,$45,$4C,$4C,$40,$44,$4F,$4E,$45,$66,$40,$4F,$46,$46,$49,$43
		.BYTE	$45,$52,$64,$FE,$FC,$FE,$50,$52,$45,$50,$41,$52,$45,$40,$54,$4F
		.BYTE	$40,$45,$4E,$54,$45,$52,$FE,$52,$45,$50,$41,$49,$52,$40,$4C,$41
		.BYTE	$42,$4F,$52,$41,$54,$4F,$52,$59,$64,$64,$64,$FE,$FC,$FA
_asci_confirm_repair:
		.BYTE	$FD,$50,$4C,$45,$41,$53,$45,$40,$43,$4F,$4E,$46,$49,$52,$4D,$40
		.BYTE	$52,$45,$50,$41,$49,$52,$53,$64,$64,$64,$FE,$FD,$FE,$50,$52,$45
		.BYTE	$53,$53,$40,$41,$40,$54,$4F,$40,$41,$43,$43,$45,$50,$54,$FE,$50
		.BYTE	$52,$45,$53,$53,$40,$42,$40,$54,$4F,$40,$52,$45,$41,$4C,$4C,$4F
		.BYTE	$43,$41,$54,$45,$FE,$F9,$FB,$43,$4F,$4E,$46,$49,$52,$4D,$45,$44
		.BYTE	$64,$FE,$FD,$FA
_asci_get_out_of_there:
		.BYTE	$53,$55,$43,$43,$45,$53,$53,$64,$64,$64,$FE,$4E,$4F,$57,$40,$47
		.BYTE	$45,$54,$40,$4F,$55,$54,$40,$4F,$46,$40,$54,$48,$45,$52,$45,$64
		.BYTE	$FE,$FE,$FC,$FA

		; banks 6 and 7 has the RESET vector set to STUB address instead of
		; system RESET address, as against first 4 banks. this doesn't matter,
		; eighter wectorw will work here, but we have two byte difference from
		; the original if using this common stub, which is not important at all.

		SAFE_STUB
