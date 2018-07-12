
		ORG $8000, $00

; =============== S U B	R O U T	I N E =======================================
; start to display the copyright message, then start the main title attract
; event with logos, title and hi-score table
;
_attract_display:
		LDA		#$FF
		STA		_pad_block_mask
		LDA		#SND_MTITLE_
		JSR		_far_apu_mus_load_prg5_safe				; init title mus
		JSR		_star_field_init
		LDA		#4
		JSR		_screen_display							; disables ppu as well
		PRINTF	_res_copyrights							; display copyrights
.do_music_and_delay:
		LDA		#$81
		STA		_PPU_CTRL								; enable NMI back, select scroll high nt bit
		LDA		#1
		STA		_event_reload_idx						; set dummy reload nmi
		LDX		#$FF
.copyright_screen_dealy:
		LDA		_event_idx								; wait for nmi, don't call events, just sync
		BEQ		.copyright_screen_dealy
		LDA		#0
		STA		_event_idx								; clear back
		TXA
		PHA
		JSR		_star_field_animate
		PLA
		TAX
		DEX
		BNE		.copyright_screen_dealy
		LDA		#0
		STA		_event_idx
		STA		_delay_counter							; init delay counters
		STA		_delay_counter+1
		STA		_screen_idx
		LDA		#2
		STA		_event_reload_idx						; set actual title event session index
		LDA		#$80
		STA		_PPU_CTRL								; do actual event, not it's NMI-based
		JMP		_event_exec

; =============== S U B	R O U T	I N E =======================================
; start a new game, display the initial intro sequence before the common
; level briefing messages
;
_intro_display:
		LDA		#0
		STA		_event_idx
		STA		_tmp_idx0
		STA		_tmp_idx1
		STA		_tmp_idx2
		STA		_delay_counter
		LDA		#$FF
		STA		_pad_block_mask
		LDA		#3
		STA		_event_reload_idx
		JSR		_log_clear_buf
		LDA		#4
		JSR		_screen_display
		LDA		#$81
		STA		_PPU_CTRL
		JMP		_event_exec

; =============== S U B	R O U T	I N E =======================================
; draw an ending screen, set manual sprite overlays and wait
;
_ending_display:
		LDA		#0
		STA		_event_idx
		STA		_hi_score_entry_display_lock
		STA		_delay_counter
		STA		_delay_counter+1
		STA		_continues								; to prevent game continue after the completion
		STA		_title_menu_idx							; to disable the title menu change
		LDA		#$FF
		STA		_pad_block_mask
		LDA		#4
		STA		_event_reload_idx
		LDA		#SND_MENDING
		JSR		_far_apu_mus_load_prg5_safe
		LDA		#7
		JSR		_screen_display
		LDA		#$81
		STA		_PPU_CTRL
		JMP		_event_exec

; =============== S U B	R O U T	I N E =======================================
; draw the repair lab, fix the tilemaps according to current damage
; run an input event
;
_repair_lab_display:
		LDA		#0
		STA		_event_idx
		LDA		#5
		STA		_event_reload_idx
		LDA		#$A8
		STA		_repair_cursor_spr_y_pos
		LDA		#$40
		STA		_repair_cursor_spr_x_pos
		LDA		#$FF
		STA		_pad_block_mask
		LDA		#2
		STA		_repair_body_index
		STA		_repair_body_index_request
		JSR		_player_percents_backup					; save current to be able to repeat repair
		LDA		_repair_tanks
		STA		_repair_tanks_backup
		LDA		#SND_MREPAIR
		JSR		_far_apu_mus_load_prg5_safe
		LDA		#8
		JSR		_screen_display
		JSR		_repair_ppu_head_upgrade_display
		JSR		_repair_ppu_body_upgrade_display
		JSR		_repair_ppu_arms_upgrade_display
		JSR		_repair_ppu_legs_upgrade_display
		LDA		#6
		STA		_PPU_MASK
		LDA		#$80
		STA		_PPU_CTRL
		JMP		_event_exec

; =============== S U B	R O U T	I N E =======================================
; go to the Game Overs screen, test for continues, etc..
;
_game_over_display:
		LDA		#0
		STA		_event_idx
		STA		_tmp_idx0
		STA		_tmp_idx1
		STA		_tmp_idx2
		STA		_delay_counter
		STA		_delay_counter+1
		LDA		#$FF
		STA		_pad_block_mask
		LDA		#6
		STA		_event_reload_idx
		LDA		_lvl_idx								; test is level > 1
		BNE		loc_442AB0
		STA		_continues								; if = 0, disable continues
		BEQ		loc_442AB
loc_442AB0:
		LDA		_continues								; test for player continues
		BEQ		loc_442AB
		LDA		#1
loc_442AB:
		STA		_title_menu_idx							; or to START and disable continues
		JSR		_log_clear_buf
		JSR		_star_field_init
		LDA		#SND_MGMOVER
		JSR		_far_apu_mus_load_prg5_safe
		LDA		#4
		JSR		_screen_display							; draw regular frame tilemap
		PRINTF	_res_game_over							; display game over
		LDA		#$81
		STA		_PPU_CTRL
		JMP		_event_exec

; =============== S U B	R O U T	I N E =======================================
_event_exec:
		LDA		_event_idx
		BEQ		_event_exec
		ASL
		TAX
		LDA		_events_lib,X
		STA		_tptr0
		LDA		_events_lib+1,X
		STA		_tptr0+1
		LDA		#0
		STA		_event_idx
		JMP		(_tptr0)

_events_lib:
		.WORD _event_exec
		.WORD _event_exec
		.WORD _event2_title
		.WORD _event3_intro
		.WORD _event4_ending
		.WORD _event5_repair
		.WORD _event6_game_over
		.WORD _event7_log_msg

; =============== S U B	R O U T	I N E =======================================
_event2_title:
		LDX		_screen_idx								; this idx is always +1 of the current screen here,
		LDA		_delays_list_hi,X						; so we can ckek next timer value
		CMP		_delay_counter+1
		BNE		.do_title_generic_handlers				; and then switch to next screen
		LDA		#0
		CMP		_delay_counter
		BNE		.do_title_generic_handlers
		LDX		_screen_idx								; yep, reached. check for title loop or skip
		CPX		#4
		BCC		loc_443B5
		DEX												; if jumped over hi-score (3 + 1), jump back to title (2)
		DEX
		STX		_screen_idx								; reload timers
		JSR		_reload_counters
loc_443B5:
		LDA		_screen_idx								; redisplay a new screen by index
		JSR		_screen_display_ex
		LDA		_screen_idx
		CMP		#3
		BNE		loc_4444F
		JSR		_star_field_init						; special init case for hi-score screen
		JSR		_draw_hiscore_table
loc_4444F:
		LDA		#$81									; re-enable ppu again
		STA		_PPU_CTRL
		INC		_screen_idx								; set ptr to next value in delays list to wait for
.do_title_generic_handlers:
		ADD16	_delay_counter, 1						; do idle timers roll on every screen
		LDA		_screen_idx
		CMP		#4
		BNE		loc_44491								; check for special case, a hi-score screen
		JSR		_star_field_animate
		LDA		_pad_edge								; at hi-score screen do check for input lol
		BEQ		loc_4448B								; while no one ever read the pads here, this only
		LDX		#4										; works when any button is held at the title
		STX		_screen_idx								; while it switching to the next screen
		JSR		_reload_counters						; do skip to the title screen
loc_4448B:
		JMP		_event_exec
loc_44491:
		LDA		_disp_spr0hit_use
		BNE		loc_444AD
		JMP		_event_exec
loc_444AD:
		LDY		_title_menu_idx							; select menu item for blink
		LDX		_title_menu_spr_ofs_list,Y
		LDA		_title_menu_spr_num_list,Y
		STA		_title_menu_spr_num_tmp
		LDA		_pad_edge
		AND		#$24
		BEQ		loc_44526
loc_44510:
		LDA		_title_menu_idx							; select or down pressed here
		CLC
		ADC		#1
		JMP		loc_445260
loc_44526:
		LDA		_pad_edge
		AND		#8
		BEQ		.title_menu_update
		LDA		_title_menu_idx
		SEC
		SBC		#1
loc_445260:
		AND		#3
		STA		_title_menu_idx
		JSR		_reload_title_counters
		LDA		#0
		STA		_generic_nmi_over_counter
.title_menu_update:
		LDA		_generic_nmi_over_counter				; check nmi counter, first 20 frames do show all menu items
		AND		#$1F
		CMP		#$15
		BCC		loc_444DA								; from 21th to 31 frames do menu hide. this code is called
		LDA		#$F0									; continuosly on every frame after 20th
		BNE		.title_menu_update_ex					; item visible time is longer than not visible
loc_444DA:
		LDA		_title_menu_pos_list,Y					; get the menu Y value to restore
.title_menu_update_ex:
		STA		_spr_buf,X								; force it to the screen
		INX
		INX
		INX
		INX
		DEC		_title_menu_spr_num_tmp					; previously saved size of menu item in sprites
		BNE		.title_menu_update_ex
		LDA		_apu_enable
		AND		#APU_SND_ENABLE
		BEQ		.menu_sound_off							; if SOUND is on
		LDA		#$4F									; draw "N " sprite
		STA		[_spr_buf+_m2onoff]
		LDA		#1
		STA		[_spr_buf+_m2onoff+4]
		BNE		loc_44569
.menu_sound_off:
		LDA		#$3F									; else, draw "FF"
		STA		[_spr_buf+_m2onoff]
		STA		[_spr_buf+_m2onoff+4]
loc_44569:
		LDA		_apu_enable
		AND		#APU_MUS_ENABLE
		BEQ		.menu_music_off
		LDA		#$4F
		STA		[_spr_buf+_m3onoff]
		LDA		#1
		STA		[_spr_buf+_m3onoff+4]
		BNE		loc_44583
.menu_music_off:
		LDA		#$3F
		STA		[_spr_buf+_m3onoff]
		STA		[_spr_buf+_m3onoff+4]
loc_44583:
		LDA		_title_menu_idx							; test for title menu idx
		BNE		loc_4459F
		LDA		_pad_edge								; start menu idx, test A or Start
		AND		#$90
		BEQ		loc_4459F
		JSR		_player_vars_init						; start new game, reset all players params
		JMP		_intro_display							; begin intro always, one-player code too
loc_4459F:
		LDA		_title_menu_idx
		CMP		#1
		BNE		loc_445C3								; continue option
		LDA		_pad_edge
		AND		#$90
		BEQ		loc_445C3
		LDA		_continues								; recheck the continues for first player
		BNE		loc_445BD
		LDA		#0
		STA		_tmp_idx0
		JMP		.wait_for_midscreen_change				; do nothing, reset counters
loc_445BD:
		LDA		#0										; clear the hi-scores before continue
		STA		_score_hex
		STA		_score_hex+1							; just exit and begin a new game without intro
		STA		_weapon0_idx							; clear weapon if continue too
		STA		_weapon1_idx
		STA		_weapon_type
		RTS
loc_445C3:
		LDA		_title_menu_idx
		CMP		#2
		BNE		loc_445E7
		LDA		_pad_edge
		AND		#$90
		BEQ		loc_445E7
		JSR		_reload_title_counters
		LDA		_apu_enable
		EOR		#APU_SND_ENABLE
		STA		_apu_enable
loc_445E7:
		LDA		_title_menu_idx
		CMP		#3
		BNE		.wait_for_midscreen_change
		LDA		_pad_edge
		AND		#$90
		BEQ		.wait_for_midscreen_change
		JSR		_reload_title_counters
		LDA		_apu_enable
		EOR		#APU_MUS_ENABLE
		STA		_apu_enable
		AND		#APU_MUS_ENABLE
		BEQ		loc_44615
		LDA		#SND_MTITLE_
loc_44615:
		JSR		_far_apu_mus_load_prg5_safe
.wait_for_midscreen_change:
		BIT		_PPU_STATUS
		BMI		loc_44620
		BVC		.wait_for_midscreen_change
		LDX		#$11									
		JSR		_delayx									; delay 97 clocks
		LDA		#$1A									; call 48 clocks
		JSR		_mmc1_chr0
loc_44620:
		JMP		_event_exec

; offset of the menu item sprites from the beginning of the sprite buffer
;
_title_menu_spr_ofs_list:
		.BYTE	[_spr2_title_menu0 - _spr2_title]
		.BYTE	[_spr2_title_menu1 - _spr2_title]
		.BYTE	[_spr2_title_menu2 - _spr2_title]
		.BYTE	[_spr2_title_menu3 - _spr2_title]

; a number of sprites in given menu item string
;
_title_menu_spr_num_list:
		.BYTE	[[_spr2_title_menu1 - _spr2_title_menu0] >> 2]
		.BYTE	[[_spr2_title_menu2 - _spr2_title_menu1] >> 2]
		.BYTE	[[_spr2_title_menu3 - _spr2_title_menu2] >> 2]
		.BYTE	[[_spr2_title_end - _spr2_title_menu3] >> 2]

; fixed constants of the Y position of the menu items on the screen
;
_m0p	EQU	$87
_m1p	EQU	$97
_m2p	EQU	$A7
_m3p	EQU	$B7

_title_menu_pos_list:
		.BYTE	_m0p,_m1p,_m2p,_m3p

; sprite ram offset of the first modified on/off tile of the menu items 2 and 3
; 6 sprites * 4 + 1 (SOUND_O/MUSIC_O)
;
_m2onoff	EQU	[[_spr2_title_menu2 - _spr2_title] + 25]
_m3onoff	EQU	[[_spr2_title_menu3 - _spr2_title] + 25]
		
; =============== S U B	R O U T	I N E =======================================
_reload_counters:
		LDA		_delays_list_hi,X
		STA		_delay_counter+1
		LDA		#0
		STA		_delay_counter
		RTS

; =============== S U B	R O U T	I N E =======================================
_reload_title_counters:
		LDA		_delays_list_hi+2
		STA		_delay_counter+1
		LDA		#0
		STA		_delay_counter
		RTS

_delays_list_hi:
		.BYTE	  0,  1,  2,  6, $A

; =============== S U B	R O U T	I N E =======================================
_event3_intro:
		LDA		_pad_edge
		AND		#$90
		BEQ		loc_44684								; test if button is pressed to skip
.i_cmdFA_break:
		RTS
loc_44684:
		LDA		_tmp_idx0								; 3B4 screen index temp value
		ASL
		TAX
		LDA		_lvl0_intro_log_lib,X
		STA		_txt_ptr
		INX
		LDA		_lvl0_intro_log_lib,X
		STA		_txt_ptr+1
		LDY		_tmp_idx1								; cur message pos temp value
		LDA		(_txt_ptr),Y							; check for special symbols
		CMP		#$FF
		BEQ		.i_cmdFF_next_screen					; $FF - next screen code
		CMP		#$FE
		BEQ		.i_cmdFE_scroll_up						; $FE - scroll buffer up
;loc_446A8:
		CMP		#$FC
		BEQ		.i_cmdFC_delay							; $FC - long delay
		CMP		#$FB
		BEQ		.i_cmdFB_clear_buf						; $FB - clear current buf
		CMP		#$FA
		BEQ		.i_cmdFA_break							; $FA code - end of messages
		LDX		_tmp_idx2								; output buf pos temp value
		STA		_log_msg_buf+$54,X						; start to display at the bottom line of the buf
		INC		_tmp_idx1
		INC		_tmp_idx2
		LDA		#0
		STA		_generic_nmi_over_counter
.j_event_exec0:
		JMP		_event_exec
; --------------------------------
.i_cmdFF_next_screen:
		INC		_tmp_idx0
		LDA		#0
		STA		_tmp_idx2
		STA		_tmp_idx1
		INC		_screen_idx
		LDA		_screen_idx
		JSR		_screen_display_ex
		LDA		#$81
		STA		_PPU_CTRL
		JMP		_event_exec
; --------------------------------
.i_cmdFE_scroll_up:
		LDA		_generic_nmi_over_counter
		AND		#$F
		BNE		.j_event_exec0
		JSR		_log_msg_buf_scroll_up
		INC		_tmp_idx1
		LDA		#0
		STA		_tmp_idx2
		JMP		_event_exec
; --------------------------------
.i_cmdFB_clear_buf:
		LDX		#$6F
		LDA		#0
loc_446FF:
		STA		_log_msg_buf,X
		DEX
		BPL		loc_446FF
		INC		_tmp_idx1
		LDA		#0
		STA		_tmp_idx2
		JMP		_event_exec
; --------------------------------
.i_cmdFC_delay:
		LDA		_delay_counter
		CLC
		ADC		#1
		STA		_delay_counter
		CMP		#$3F
		BCC		.j_event_exec0
		INC		_tmp_idx1
		LDA		#0
		STA		_tmp_idx2
		STA		_delay_counter
		JMP		_event_exec

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
		ADD16	_delay_counter, 1
		LDA		_delay_counter+1
		CMP		#3
		BCC		loc_44858
		CMP		#$D
		BCS		_title_screen_display
		JMP		_hi_score_entry_event
loc_44858:
		LDA		_pad_edge
		AND		#$80
		BEQ		loc_44871
		LDA		#3
		STA		_delay_counter+1
loc_44871:
		BIT		_PPU_STATUS
		BMI		loc_448B8
		BVC		loc_44871
		LDX		#7
		JSR		_delayx									; delay 48 clocks
		LDA		#$1F									; call 48 clocks
		JSR		_mmc1_chr0
loc_448B8:
		JMP		_event_exec

; =============== S U B	R O U T	I N E =======================================
; used to redisplay the title screen after the game over or ending sequences
; as well after the hi-score entry screen
;
_title_screen_display:
		LDA		#0
		STA		_event_idx
		LDA		#2
		STA		_event_reload_idx
		LDA		#$FF
		STA		_pad_block_mask
		JSR		_reload_title_counters
		LDA		#SND_MTITLE_
		JSR		_far_apu_mus_load_prg5_safe
		LDA		#2
		JSR		_screen_display
		LDA		#$80
		STA		_PPU_CTRL
		JMP		_event_exec

; =============== S U B	R O U T	I N E =======================================
_hi_score_entry_event:
		LDA		_hi_score_entry_display_lock
		BNE		loc_44965
		LDA		#$FF
		STA		_hi_score_entry_display_lock
		JSR		_sort_hiscore_test
		LDA		_hi_score_line_idx
		CMP		#6
		BCS		_title_screen_display
		LDA		#0
		STA		_tmp_idx2
		JSR		_star_field_init
		LDA		#3
		JSR		_screen_display
		JSR		_draw_hiscore_table
		LDA		#$81
		STA		_PPU_CTRL
loc_44965:
		JSR		_star_field_animate
		LDX		_hi_score_line_idx
		LDA		_hi_score_ram_name_ofs_list,X
		CLC
		ADC		_tmp_idx2
		TAX
		LDA		_pad_edge
		BNE		loc_44992
		LDA		_generic_nmi_over_counter				; if button is held nmi counter is reset to 3 here
		AND		#$1F									; so we auto-repear back after 17 frames every time
		BEQ		loc_44992
		BNE		.j_event_exec3
loc_44992:
		LDA		_pad_last
		AND		#4
		BEQ		loc_449B3
		LDA		_ram_score_tbl,X
		CLC
		ADC		#1
		CMP		#$25
		BCC		.update_char
		LDA		#0
		BEQ		.update_char
loc_449B3:
		LDA		_pad_last
		AND		#8
		BEQ		loc_449D2
		LDA		_ram_score_tbl,X
		SEC
		SBC		#1
		BCS		.update_char
		LDA		#$24
.update_char:
		STA		_ram_score_tbl,X
		JMP		.reset_counters
loc_449D2:
		LDA		_pad_last
		AND		#1
		BEQ		loc_449EE
		LDA		_tmp_idx2
		CMP		#9
		BCS		.wait_for_start
		INC		_tmp_idx2
		BCC		.reset_counters
loc_449EE:
		LDA		_pad_last
		AND		#2
		BEQ		.wait_for_start
		LDA		_tmp_idx2
		BEQ		.wait_for_start
		DEC		_tmp_idx2
.reset_counters:
		LDA		#3
		STA		_delay_counter+1
		STA		_generic_nmi_over_counter
.wait_for_start:
		LDA		_pad_edge
		AND		#$90
		BEQ		.j_event_exec3
		JMP		_title_screen_display
.j_event_exec3:
		JMP		_event_exec

; =============== S U B	R O U T	I N E =======================================
_sort_hiscore_test:
		LDA		#0
		STA		_hex2str_tile_delta
		STA		_hi_score_pos_count
		STA		_hi_score_line_idx
		LDX		#$E
loc_45A21:
		STA		_score_cmp_buf,X
		DEX
		BPL		loc_45A21
		U16TOS	_score_hex, _score_cmp_buf
loc_45A3B:
		LDY		_hi_score_pos_count					; read cur score line idx to test
		CPY		#6
		BCS		locret_45A7A						; check if no more lines
		LDX		_hi_score_ram_score_ofs_list,Y		; table calc of score data offset
		LDY		#0
.test_loop:
		LDA		_score_cmp_buf,Y					; compare
		CMP		_ram_score_tbl,X
		BCC		.hiscore_no_swap
		BNE		.hiscore_swap
		INX
		INY
		CPY		#5
		BNE		.test_loop
.hiscore_no_swap:
		INC		_hi_score_line_idx					; player position in the list
loc_45A55:
		INC		_hi_score_pos_count					; next line
		JMP		loc_45A3B
.hiscore_swap:
		LDY		_hi_score_pos_count					; return score data offset to the begining
		LDX		_hi_score_ram_score_ofs_list,Y
		LDY		#0
loc_45A69:
		JSR		_do_swap
		CPY		#5
		BNE		loc_45A69
		LDY		_hi_score_pos_count					; get the name offset
		LDX		_hi_score_ram_name_ofs_list,Y
		LDY		#5
loc_45A690:
		JSR		_do_swap
		CPY		#$F
		BNE		loc_45A690
		BEQ		loc_45A55
locret_45A7A:
		RTS
		
; =============== S U B	R O U T	I N E =======================================
_do_swap:
		LDA		_ram_score_tbl,X
		PHA
		LDA		_score_cmp_buf,Y
		STA		_ram_score_tbl,X
		PLA
		STA		_score_cmp_buf,Y
		INX
		INY
		RTS

_hi_score_ram_score_ofs_list:
		.BYTE	0,5,10,15,20,25
_hi_score_ram_name_ofs_list:
		.BYTE	_hi0_name-_ram_score_tbl
		.BYTE	_hi1_name-_ram_score_tbl
		.BYTE	_hi2_name-_ram_score_tbl
		.BYTE	_hi3_name-_ram_score_tbl
		.BYTE	_hi4_name-_ram_score_tbl
		.BYTE	_hi5_name-_ram_score_tbl

; =============== S U B	R O U T	I N E =======================================
; draws an actual hi-score table data from RAM at the fixed position
; on the screen
;
_draw_hiscore_table:
		PRINTF	_res_ram_scores	;	_res_hi_score_header
		BIT		_PPU_STATUS
		LDA		#$21
		STA		_PPU_ADDR
		LDA		#$47
		STA		_PPU_ADDR
		LDX		#0
loc_45AEE:
		LDY		_hi0_score,X
		LDA		_nums_table,Y
		STA		_PPU_DATA
		INX
		CPX		#5
		BCC		loc_45AEE
		LDA		#$F
		STA		_PPU_DATA
		STA		_PPU_DATA
		LDA		#$21
		STA		_PPU_ADDR
		LDA		#$87
		STA		_PPU_ADDR
		LDX		#0
loc_45B26:
		LDY		_hi1_score,X
		LDA		_nums_table,Y
		STA		_PPU_DATA
		INX
		CPX		#5
		BCC		loc_45B26
		LDA		#$F
		STA		_PPU_DATA
		STA		_PPU_DATA
		LDA		#$21
		STA		_PPU_ADDR
		LDA		#$C7
		STA		_PPU_ADDR
		LDX		#0
loc_45B5E:
		LDY		_hi2_score,X
		LDA		_nums_table,Y
		STA		_PPU_DATA
		INX
		CPX		#5
		BCC		loc_45B5E
		LDA		#$F
		STA		_PPU_DATA
		STA		_PPU_DATA
		LDA		#$22
		STA		_PPU_ADDR
		LDA		#7
		STA		_PPU_ADDR
		LDX		#0
loc_45B96:
		LDY		_hi3_score,X
		LDA		_nums_table,Y
		STA		_PPU_DATA
		INX
		CPX		#5
		BCC		loc_45B96
		LDA		#$F
		STA		_PPU_DATA
		STA		_PPU_DATA
		LDA		#$22
		STA		_PPU_ADDR
		LDA		#$47
		STA		_PPU_ADDR
		LDX		#0
loc_45BCE:
		LDY		_hi4_score,X
		LDA		_nums_table,Y
		STA		_PPU_DATA
		INX
		CPX		#5
		BCC		loc_45BCE
		LDA		#$F
		STA		_PPU_DATA
		STA		_PPU_DATA
		LDA		#$22
		STA		_PPU_ADDR
		LDA		#$87
		STA		_PPU_ADDR
		LDX		#0
loc_45C06:
		LDY		_hi5_score,X
		LDA		_nums_table,Y
		STA		_PPU_DATA
		INX
		CPX		#5
		BCC		loc_45C06
		LDA		#$F
		STA		_PPU_DATA
		STA		_PPU_DATA
		RTS

_nums_table:
		.BYTE	_O,_1,_2,_3,_4,_5,_6,_7,_8,_9

; =============== S U B	R O U T	I N E =======================================
_event5_repair:
		LDA		_repair_cursor_spr_x_pos				; update cursor position
		STA		_spr_buf+4
		CLC
		ADC		#8
		STA		_spr_buf+8
		LDA		_repair_cursor_spr_y_pos
		STA		_spr_buf+7
		STA		_spr_buf+$B
		LDA		#$E6
		STA		_hex2str_tile_delta
		LDX		_repair_body_index_request
		LDY		_repair_update_flag_list,X
		LDA		_percents,Y
		U8TOS	_repair_percent_disp_buf_tmp
		LDA		_pad_edge
		AND		#$10
		BEQ		loc_44AB8
		LDA		#0
		STA		_event_reload_idx
		LDA		#7
		JMP		_bios_msg_display_no_skip
loc_44AB8:
		LDA		_repair_tanks							; draw only single digit of tanks if more than 9, draw always 9
		CMP		#$A										; there were original bug, drawing % instead, since you doubtly
		BCC		loc_44AC1								; obtain more than 10 at once and this case never tested
		LDA		#9
loc_44AC1:
		CLC
		ADC		#$E6
		STA		_repair_tanks_number_tile_tmp
		LDA		_pad_edge
		AND		#$80
		BEQ		loc_44AF1
		LDA		_repair_tanks
		BEQ		loc_44AF1
		LDX		_repair_body_index_request
		LDY		_repair_update_flag_list,X
		LDA		_percents,Y
		CMP		#MAX_PERCENT_VALUE
		BCS		loc_44AF1
		CLC
		ADC		#DEF_PERCENT_REPAIR
		CMP		#MAX_PERCENT_VALUE
		BCC		loc_44AF10
		LDA		#MAX_PERCENT_VALUE						; clamp to max
loc_44AF10:
		STA		_percents,Y
		DEC		_repair_tanks
loc_44AF1:
		BIT		_PPU_STATUS
		BMI		loc_44B2B
		BVC		loc_44AF1
		LDX		#6										
		JSR		_delayx									; 43 clocks delay
		LDA		#$17									; 48 clocks call
		JSR		_mmc1_chr0								
loc_44B2B:
		JSR		_repair_cursor_input
		JSR		_repair_cursor_animate
		JMP		_event_exec

; =============== S U B	R O U T	I N E =======================================
_repair_cursor_input:
		LDA		_repair_cursor_animate_in_progress
		BNE		locret_45C98
		LDA		_pad_edge
		AND		#$F
		TAX
		LDY		_dpad_conv_tbl,X
		BNE		loc_45C39
		RTS
loc_45C39:
		DEY
		STY		_tptr0
		LDA		_repair_body_index
		ASL
		ASL
		CLC
		ADC		_tptr0
		TAY
		LDA		_repair_cursor_allow_table,Y
		BMI		locret_45C98
		CMP		_repair_body_index_request
		BEQ		locret_45C98
		STA		_repair_body_index_request
		LDA		#1
		STA		_repair_cursor_animate_in_progress
locret_45C98:
		RTS

UP	EQU	1
RT	EQU 2
DN	EQU	3
LT	EQU	4

_dpad_conv_tbl:
		.BYTE	0,RT,LT,0,DN,0,0,0,UP,0,0,0,0,0,0,0

HEAD	EQU	0
RHAND	EQU 1
BODY	EQU	2
LHAND	EQU	3
LEGS	EQU	4
STOP	EQU $FF

_repair_cursor_allow_table:
;				UP    RT    DN    LT
		.BYTE	STOP ,LHAND,BODY ,RHAND ; HEAD
		.BYTE	HEAD ,BODY ,LEGS ,STOP  ; RHAND
		.BYTE	HEAD ,LHAND,LEGS ,RHAND	; BODY
		.BYTE	HEAD ,STOP ,LEGS ,BODY	; LHAND
		.BYTE	BODY ,LHAND,STOP ,RHAND ; LEGS

; =============== S U B	R O U T	I N E =======================================
_repair_cursor_animate:
		LDA		_repair_cursor_animate_in_progress
		BNE		loc_45CD4
		RTS
loc_45CD4:
		INC		_repair_cursor_animate_tmp
		LDA		_repair_cursor_animate_tmp
		CMP		#2
		BEQ		loc_45CDF
		RTS
loc_45CDF:
		LDA		#0
		STA		_repair_cursor_animate_tmp
		LDA		_repair_body_index_request
		ASL
		TAX
		LDA		_repair_cursor_anim_data,X
		STA		_repair_cursor_pos_dest
		LDA		_repair_cursor_anim_data+1,X
		STA		_repair_cursor_pos_dest+1
		LDA		_repair_cursor_spr_y_pos
		CMP		_repair_cursor_pos_dest
		BCS		loc_45D1D
		LDA		_repair_cursor_pos_dest
		SEC
		SBC		_repair_cursor_spr_y_pos
		LSR
		LSR
		STA		_repair_cursor_pos_dest
		LDA		_repair_cursor_spr_y_pos
		CLC
		ADC		_repair_cursor_pos_dest
		STA		_repair_cursor_spr_y_pos
		JMP		loc_45D33
loc_45D1D:
		LDA		_repair_cursor_spr_y_pos
		SEC
		SBC		_repair_cursor_pos_dest
		LSR
		LSR
		STA		_repair_cursor_pos_dest
		LDA		_repair_cursor_spr_y_pos
		SEC
		SBC		_repair_cursor_pos_dest
		STA		_repair_cursor_spr_y_pos
loc_45D33:
		LDA		_repair_cursor_spr_x_pos
		CMP		_repair_cursor_pos_dest+1
		BCS		loc_45D54
		LDA		_repair_cursor_pos_dest+1
		SEC
		SBC		_repair_cursor_spr_x_pos
		LSR
		LSR
		STA		_repair_cursor_pos_dest+1
		LDA		_repair_cursor_spr_x_pos
		CLC
		ADC		_repair_cursor_pos_dest+1
		STA		_repair_cursor_spr_x_pos
		JMP		loc_45D6A
loc_45D54:
		LDA		_repair_cursor_spr_x_pos
		SEC
		SBC		_repair_cursor_pos_dest+1
		LSR
		LSR
		STA		_repair_cursor_pos_dest+1
		LDA		_repair_cursor_spr_x_pos
		SEC
		SBC		_repair_cursor_pos_dest+1
		STA		_repair_cursor_spr_x_pos
loc_45D6A:
		LDA		_repair_cursor_pos_dest
		ORA		_repair_cursor_pos_dest+1
		BNE		locret_45D7D
		LDA		#0
		STA		_repair_cursor_animate_in_progress
		LDA		_repair_body_index_request
		STA		_repair_body_index
locret_45D7D:
		RTS

_repair_update_flag_list:
		.BYTE	  0,  2,  1,  2,  3
_repair_cursor_anim_data:
		.WORD $20A0
		.WORD $5888
		.WORD $40A8
		.WORD $48C0
		.WORD $80CB

; =============== S U B	R O U T	I N E =======================================
_event6_game_over:
		JSR		_star_field_animate
		LDA		_pad_edge
		BEQ		.game_over_timers_increase
		LDA		#2										; here we set timers to max value and jump to test it and so skip!
		STA		_delay_counter+1						; why don't you just jupm to skip here?! maybe because it's mean some
		LDA		#$30									; kind of reinitialization of the same timers used by the title screen
		STA		_delay_counter
		BNE		.game_over_timers_test
.game_over_timers_increase:
		ADD16	_delay_counter, 1
.game_over_timers_test:
		LDA		_delay_counter+1
		CMP		#2
		BNE		.j_event_exec2
		LDA		_delay_counter
		CMP		#$30
		BCC		.j_event_exec2
		LDA		#4
		STA		_delay_counter+1
		STA		_event_reload_idx
		LDA		#0
		STA		_hi_score_entry_display_lock
		LDA		#3
		STA		_screen_idx
		JMP		_hi_score_entry_event
.j_event_exec2:
		JMP		_event_exec

; =============== S U B	R O U T	I N E =======================================
_event7_log_msg:
		LDA		_log_msg_skip_forbid
		BNE		loc_44BCA
		LDA		_pad_edge
		AND		#$80
		BEQ		loc_44BCA
		RTS
loc_44BCA:
		LDY		_tmp_idx1								; msg pos temp value
		LDA		(_txt_ptr),Y
		CMP		#$F9
		BEQ		.l_cmdF9_repair_confirmation			; special $F9 cmd to ask a question and wait for input
		CMP		#$FE
		BEQ		.l_cmdFE_scroll_up						; cmd $FE, scroll window up
		CMP		#$FC
		BEQ		.l_cmdFC_delay							; cmd $FC, delay
		CMP		#$FB
		BEQ		.l_cmdFB_clear_buf						; cmd $FB, clear buf
		CMP		#$FA
		BNE		loc_44BFC								; cmd $FA, break
		RTS
loc_44BFC:
		LDX		_tmp_idx2
		STA		_log_msg_buf+$54,X
		INC		_tmp_idx1
		INC		_tmp_idx2
		LDA		#0
		STA		_generic_nmi_over_counter
.j_event_exec1:
		JMP		_event_exec
; --------------------------------
.l_cmdFE_scroll_up:
		LDA		_generic_nmi_over_counter
		AND		#$F
		BNE		.j_event_exec1
		JSR		_log_msg_buf_scroll_up
		INC		_tmp_idx1
		LDA		#0
		STA		_tmp_idx2
		JMP		_event_exec
; --------------------------------
.l_cmdFC_delay:
		LDA		_delay_counter
		CLC
		ADC		#1
		STA		_delay_counter
		CMP		#$3F
		BCC		.j_event_exec1
		INC		_tmp_idx1
		LDA		#0
		STA		_tmp_idx2
		STA		_delay_counter
		JMP		_event_exec
; --------------------------------
.l_cmdFB_clear_buf:
		LDX		#$6F
		LDA		#0
loc_44C65:
		STA		_log_msg_buf,X
		DEX
		BPL		loc_44C65
		INC		_tmp_idx1
		LDA		#0
		STA		_tmp_idx2
		JMP		_event_exec
; --------------------------------
.l_cmdF9_repair_confirmation:
		LDA		_pad_edge
		AND		#$80
		BEQ		loc_44CA9
		INC		_tmp_idx1
		JSR		_player_percents_backup					; save new parameters
		JSR		_player_overall_armor_calc				; recalc overall value as well
		JMP		_event_exec
loc_44CA9:
		LDA		_pad_edge
		AND		#$40
		BNE		loc_44CB3
		JMP		_event_exec
loc_44CB3:
		JSR		_player_percents_restore
		LDA		_repair_tanks_backup
		STA		_repair_tanks
		JMP		_repair_lab_display

; =============== S U B	R O U T	I N E =======================================
_log_clear_buf:
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
		JSR		_mmc1_chr0
		LDA		_chr1_bank_shadow
		JSR		_mmc1_chr1
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
		JMP		_nmi0_extra
_repair_mode_dequeue:
		BIT		_PPU_STATUS
		LDA		#$22
		STA		_PPU_ADDR
		LDA		#5
		STA		_PPU_ADDR
		LDA		_repair_body_index_request
		ASL
		TAX
		LDA		_repair_small_frame_txt_lib,X
		STA		_tptr0
		INX
		LDA		_repair_small_frame_txt_lib,X
		STA		_tptr0+1
		LDY		#0
loc_44F6E:
		LDA		(_tptr0),Y
		STA		_PPU_DATA
		INY
		CPY		#4
		BCC		loc_44F6E
		LDA		#$22
		STA		_PPU_ADDR
		LDA		#$25
		STA		_PPU_ADDR
		LDA		_repair_percent_disp_buf_tmp+2
		STA		_PPU_DATA
		LDA		_repair_percent_disp_buf_tmp+3
		STA		_PPU_DATA
		LDA		_repair_percent_disp_buf_tmp+4
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
		LDA		_repair_tanks_number_tile_tmp
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
		LDA		_repair_small_frames_lib,Y
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
		LDA		_repair_small_frames_lib,Y
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
		LDA		_repair_small_frames_lib,Y
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
		LDA		_repair_small_frames_lib,Y
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
		LDA		_repair_small_frames_lib,Y
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
		LDA		_repair_small_frames_lib,Y
		STA		_PPU_DATA
		INY
		INX
		CPX		#8
		BCC		loc_45053
loc_4505F:
		LDA		_generic_nmi_over_counter
		AND		#1
		BEQ		loc_45069
		JMP		_nmi0_extra
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
		JMP		_nmi0_extra
loc_45081:
		JSR		_repair_ppu_head_upgrade_display
		JMP		_nmi0_extra
loc_45087:
		JSR		_repair_ppu_body_upgrade_display
		JMP		_nmi0_extra
loc_4508D:
		JSR		_repair_ppu_arms_upgrade_display
		JMP		_nmi0_extra

; =============== S U B	R O U T	I N E =======================================
_repair_ppu_head_upgrade_display:
		LDA		_percents.head
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
		LDA		_ovl_repair_head,Y
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
		LDA		_ovl_repair_head,Y
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
		LDA		_ovl_repair_head,Y
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
		LDA		_ovl_repair_head,Y
		STA		_PPU_DATA
		INY
		DEX
		BNE		loc_450FC
		RTS

; =============== S U B	R O U T	I N E =======================================
_repair_ppu_arms_upgrade_display:
		LDA		_percents.arms
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
		LDA		_ovl_repair_rarm0,Y
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
		LDA		_ovl_repair_rarm0,Y
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
		LDA		_ovl_repair_rarm0,Y
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
		LDA		_ovl_repair_rarm0,Y
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
		LDA		_ovl_repair_rarm0,Y
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
		LDA		_ovl_repair_rarm0,Y
		STA		_PPU_DATA
		INY
		DEX
		BNE		loc_45199
		LDA		_percents.arms
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
		LDA		_ovl_repair_rarm1,Y
		STA		_PPU_DATA
		INY
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$97
		STA		_PPU_ADDR
		LDA		_ovl_repair_rarm1,Y
		STA		_PPU_DATA
		INY
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$B7
		STA		_PPU_ADDR
		LDA		_ovl_repair_rarm1,Y
		STA		_PPU_DATA
		INY
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$D7
		STA		_PPU_ADDR
		LDA		_ovl_repair_rarm1,Y
		STA		_PPU_DATA
		INY
		LDA		#$20
		STA		_PPU_ADDR
		LDA		#$F7
		STA		_PPU_ADDR
		LDA		_ovl_repair_rarm1,Y
		STA		_PPU_DATA
		INY
		LDA		#$21
		STA		_PPU_ADDR
		LDA		#$17
		STA		_PPU_ADDR
		LDA		_ovl_repair_rarm1,Y
		STA		_PPU_DATA
;
; experimental re-enable the left palm hand damage display for repair lab
; no doubt it looks creepy and weid, so they decided to remove it at all
;
;		LDA		_percents.arms
;		CMP		#$B
;		BCC		loc_451160
;		CMP		#$15
;		BCC		loc_4511A0
;		LDY		#0
;		BEQ		loc_4511C0
;loc_451160:
;		LDY		#$20
;		BNE		loc_4511C0
;loc_4511A0:
;		LDY		#$10
;loc_4511C0:
;		BIT		_PPU_STATUS
;		LDA		#$21
;		STA		_PPU_ADDR
;		LDA		#$18
;		STA		_PPU_ADDR
;		LDX		#4
;loc_4512B0:
;		LDA		_ovl_repair_larm,Y
;		STA		_PPU_DATA
;		INY
;		DEX
;		BNE		loc_4512B0
;		LDA		#$21
;		STA		_PPU_ADDR
;		LDA		#$38
;		STA		_PPU_ADDR
;		LDX		#4
;loc_451410:
;		LDA		_ovl_repair_larm,Y
;		STA		_PPU_DATA
;		INY
;		DEX
;		BNE		loc_451410
;		LDA		#$21
;		STA		_PPU_ADDR
;		LDA		#$58
;		STA		_PPU_ADDR
;		LDX		#4
;loc_451570:
;		LDA		_ovl_repair_larm,Y
;		STA		_PPU_DATA
;		INY
;		DEX
;		BNE		loc_451570
;		RTS

; =============== S U B	R O U T	I N E =======================================
_repair_ppu_body_upgrade_display:
		LDA		_percents.body
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
		LDA		_ovl_repair_body,Y
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
		LDA		_ovl_repair_body,Y
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
		LDA		_ovl_repair_body,Y
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
		LDA		_ovl_repair_body,Y
		STA		_PPU_DATA
		INY
		DEX
		BNE		loc_4528D
		RTS

; =============== S U B	R O U T	I N E =======================================
_repair_ppu_legs_upgrade_display:
		LDA		_percents.legs
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
		LDA		_ovl_repair_legs,Y
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
		LDA		_ovl_repair_legs,Y
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
		LDA		_ovl_repair_legs,Y
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
		LDA		_ovl_repair_legs,Y
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
		LDA		_ovl_repair_legs,Y
		STA		_PPU_DATA
		INY
		DEX
		BNE		loc_45314
; --
; portion of code to display extra missing legs tilemap in repair lab
; original resource data is 8x6 tiles, but only 8x5 is used, restored
;
;		LDA		#$22
;		STA		_PPU_ADDR
;		LDA		#$12
;		STA		_PPU_ADDR
;		LDX		#8
;loc_4532B:
;		LDA		_ovl_repair_legs,Y
;		STA		_PPU_DATA
;		INY
;		DEX
;		BNE		loc_4532B
		RTS

; =============== S U B	R O U T	I N E =======================================
_nmi0_extra:
		JSR		_nmi0_scroll_reload
		LDA		_event_reload_idx
		STA		_event_idx
		LDA		#$1E
		STA		_PPU_MASK
		LDA		_special_controls_flag					; set only for title screen to prevent accidentally
		BEQ		.nmi0_hiscore_handler					; hi-score input handler selection
		LDA		#$A8									; the title screen ends its nmi here
		BNE		.nmi0_finish1
.nmi0_hiscore_handler:
		LDA		_event_reload_idx
		CMP		#4
		BNE		.nmi0_finish0
		LDA		_screen_idx
		CMP		#3
		BNE		.nmi0_finish0
; --------------------------------
		BIT		_PPU_STATUS
		LDX		_hi_score_line_idx						; set the ppu offset to the current name entry line
		LDA		_hi_score_ppu_ofs_list_hi,X
		STA		_PPU_ADDR
		STA		_tmpE6
		LDA		_hi_score_ppu_ofs_list_lo,X
		STA		_PPU_ADDR
		STA		_tmpE7
		LDY		_hi_score_ram_name_ofs_list,X
		LDX		#$A										; update the whole string entered until now
loc_4539C:
		LDA		_ram_score_tbl,Y
		STA		_PPU_DATA
		INY
		DEX
		BNE		loc_4539C								; from here X is zero!
		LDA		_tmpE7									; calc position of the cursor tile (+32-1+cur)
		CLC
		ADC		#$1F
		STA		_tmpE7
		BCC		loc_45390
		INC		_tmpE6
loc_45390:
		BIT		_PPU_STATUS
		LDA		_tmpE6
		STA		_PPU_ADDR
		LDA		_tmpE7
		CLC
		ADC		_tmp_idx2								; here is tmp edit cursor position
		STA		_PPU_ADDR								; set ppu
		LDA		_generic_nmi_over_counter
		AND		#$1F
		CMP		#$13
		BCS		loc_453E0								; draw " _ " or "  " to blink
		STX		_PPU_DATA								; spaces before and after are for clean previous cursor
		LDA		#$DE									; if it's position is changed
		STA		_PPU_DATA
		STX		_PPU_DATA
		BNE		loc_453E8
loc_453E0:
		STX		_PPU_DATA
		STX		_PPU_DATA
loc_453E8:
		LDA		#$20
		STA		_PPU_ADDR
		STX		_PPU_ADDR
; --------------------------------
.nmi0_finish0:
		LDA		#$88
.nmi0_finish1:
		STA		_PPU_CTRL
		INC		_generic_nmi_over_counter
		JSR		_far_apu_play_prg5_safe					; put it here, because we always run in prg5 during
		JSR		_read_pads
		LDA		_irq_regs_backup						; intro sequences, removed from event loops
		LDX		_irq_regs_backup+1
		LDY		_irq_regs_backup+2
		RTI

_hi_score_ppu_ofs_list_lo:
		.BYTE	$51,$91,$D1,$11,$51,$91
_hi_score_ppu_ofs_list_hi:
		.BYTE	$21,$21,$21,$22,$22,$22

_repair_small_frame_txt_lib:
		.WORD _repair_small_frame_txt0
		.WORD _repair_small_frame_txt1
		.WORD _repair_small_frame_txt3
		.WORD _repair_small_frame_txt1
		.WORD _repair_small_frame_txt2

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
		LDA		#0
		STA		_PPU_SCROLL
		LDA		_disp_extra_shift
		BEQ		loc_4544C
		LDA		#$E0
loc_4544C:
		STA		_PPU_SCROLL
		RTS

; =============== S U B	R O U T	I N E =======================================
_star_field_init:
		LDX		#$27
		LDA		#$78
		STA		_star_field_center_pos_x
		LDA		#$70
		STA		_star_field_center_pos_y
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
		RTS

; =============== S U B	R O U T	I N E =======================================
_star_field_animate:
		LDX		#$14
.star_field_loop:
		LDY		_star_field_data0,X
		LDA		_star_field_data1,X
		STA		_mul_argB
		LDA		_star_field_params0,Y
		STA		_star_filed_sign_tmp
		AND		#$7F
		STA		_mul_argA
		JSR		_A_mul_B_mul_2
		LDY		_A_mul_4_table,X
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
		LDY		_A_mul_4_table,X
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
		LDA		_stars_tiles_idx_list,Y
		LDY		_A_mul_4_table,X
		CLC
		ADC		#$8B
		STA		_spr_buf+1,Y
		TXA
		AND		#3
		ORA		#$20
		STA		_spr_buf+2,Y
		DEX
		BMI		locret_4556A
		JMP		.star_field_loop
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
		JMP		.star_field_loop

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

_A_mul_4_table:
		.BYTE	  0,  4,  8, $C,$10,$14,$18,$1C,$20,$24,$28,$2C,$30,$34,$38,$3C
		.BYTE	$40,$44,$48,$4C,$50,$54,$58,$5C,$60,$64,$68,$6C,$70,$74,$78,$7C
		.BYTE	$80,$84,$88,$8C,$90,$94,$98,$9C,$A0,$A4,$A8,$AC,$B0,$B4,$B8,$BC
		.BYTE	$C0,$C4,$C8,$CC,$D0,$D4,$D8,$DC,$E0,$E4,$E8,$EC,$F0,$F4,$F8,$FC
_stars_tiles_idx_list:
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

; =============== S U B	R O U T	I N E =======================================
_screen_display:
		STA		_screen_idx

; =============== S U B	R O U T	I N E =======================================
; disables the PPU/RENDER/NMI, no need to clear the screen before, all
; tilemaps are redraw it completely here, no restore PPU on exit, so
; the code must do it personally
;
_screen_display_ex:
		TAX
		LDY		#0
		STY		_PPU_CTRL
		STY		_PPU_MASK
;loc_45884:
;		BIT		_PPU_STATUS								; wait for nmi status raised
;		BPL		loc_45884
		BIT		_PPU_STATUS
		LDA		#$F0									; Y already 0
loc_45885:
		STA		_spr_buf,Y
		INY
		BNE		loc_45885
		LDA		_disp_spr_data_size_list,X
		STA		_disp_spr_send_tmp
		LDA		_disp_chr0_list,X
		STA		_chr1_bank_shadow
		LDA		_disp_chr1_list,X
		STA		_chr0_bank_shadow
		LDA		_special_controls_flags_list,X
		STA		_special_controls_flag
		LDA		_disp_extra_shifts_list,X
		STA		_disp_extra_shift
		LDA		_disp_spr0hit_use_flags,X
		STA		_disp_spr0hit_use
		TXA
		ASL
		TAX
		LDA		_disp_tlm_lib,X
		STA		_tptr0
		LDA		_disp_spr_lib,X
		STA		_str_ptr
		LDA		_disp_pal_lib,X
		STA		_tptr1
		LDA		_disp_atr_lib,X
		STA		_tptr2
		INX
		LDA		_disp_tlm_lib,X
		STA		_tptr0+1
		LDA		_disp_spr_lib,X
		STA		_str_ptr+1
		LDA		_disp_pal_lib,X
		STA		_tptr1+1
		LDA		_disp_atr_lib,X
		STA		_tptr2+1
		LDA		#$20
		STA		_PPU_ADDR
		STY		_PPU_ADDR								; Y still 0!
; --------------------------------
.disp_tlm_send:
		LDA		(_tptr0),Y
		BPL		loc_4592B
		CMP		#$FF
		BEQ		.disp_atr_send
		AND		#$7F
		TAX
		INCY	_tptr0
loc_4591E:
		LDA		(_tptr0),Y
		STA		_PPU_DATA
		INCY	_tptr0
		DEX
		BNE		loc_4591E
		BEQ		.disp_tlm_send
loc_4592B:
		TAX
		INCY	_tptr0
		LDA		(_tptr0),Y
loc_45931:
		STA		_PPU_DATA
		DEX
		BNE		loc_45931
		INCY	_tptr0
		JMP		.disp_tlm_send
; --------------------------------
.disp_atr_send:
		LDY		#0										; Y not 0 anymore!
		LDA		#$23
		STA		_PPU_ADDR
		LDX		#$C0
		STX		_PPU_ADDR
loc_4594E:
		LDA		(_tptr2),Y
		STA		_PPU_DATA
		INY
		INX
		BNE		loc_4594E
		LDY		#$1F
loc_45959:
		LDA		(_tptr1),Y
		STA		_pal_buf,Y
		DEY
		BPL		loc_45959
		LDY		_disp_spr_send_tmp
		BNE		.disp_spr_send
		RTS
; --------------------------------
.disp_spr_send:
		LDA		(_str_ptr),Y
		STA		_spr_buf,Y
		DEY
		BNE		.disp_spr_send
		LDA		(_str_ptr),Y
		STA		_spr_buf,Y
		RTS
; --
; old autogenerating sprite function, maybe will need later
;		LDA		#$C
;		STA		_tptr0
;		LDA		#$FF
;		STA		_tptr0+1
;		LDA		#0
;		STA		_disp_spr_send_tmp
;loc_45979:
;		LDX		#0
;loc_4597B:
;		LDY		#0
;		LDA		(_str_ptr),Y
;		BEQ		loc_459D4
;		STA		_tmpE4
;		CMP		#$FF
;		BNE		loc_45992
;		LDA		_disp_spr_send_tmp
;		EOR		#$80
;		STA		_disp_spr_send_tmp
;		JMP		loc_459D4
;loc_45992:
;		LDA		_tmpE4
;		AND		#$3F
;		LDY		_tptr0
;		TXA
;		PHA
;		LDX		_screen_idx
;		LDA		_tmpE4
;		AND		#$3F
;		CLC
;		ADC		_disp_spr_tile_delta,X
;		BIT		_disp_spr_send_tmp
;		BPL		loc_459AC
;		ADC		#$34
;loc_459AC:
;		STA		_spr_buf+1,Y
;		PLA
;		TAX
;		ASL
;		ASL
;		ASL
;		STA		_spr_buf+3,Y
;		LDA		_tptr0+1
;		CLC
;		ADC		_disp_extra_shift
;		STA		_spr_buf,Y
;		LDA		_tmpE4
;		LSR
;		LSR
;		LSR
;		LSR
;		LSR
;		LSR
;		STA		_spr_buf+2,Y
;		LDA		_tptr0
;		CLC
;		ADC		#4
;		STA		_tptr0
;		BCS		locret_459F1
;loc_459D4:
;		ADD16	_str_ptr, 1
;		INX
;		CPX		#$20
;		BCC		loc_4597B
;		LDA		_tptr0+1
;		CLC
;		ADC		#8
;		STA		_tptr0+1
;		CMP		#$C8
;		BCC		loc_45979
;locret_459F1:
;		RTS
; --


_disp_tlm_lib:
		.WORD _tlm0_ocean
		.WORD _tlm1_probe
		.WORD _tlm2_title
		.WORD _tlm4_frame	; _tlm3_hiscore
		.WORD _tlm4_frame
		.WORD _tlm5_lewis
		.WORD _tlm6_robocop
		.WORD _tlm7_ending
		.WORD _tlm8_repair
_disp_atr_lib:
		.WORD _atr0_ocean
		.WORD _atr1_probe
		.WORD _atr2_title
		.WORD _atr4_frame
		.WORD _atr4_frame
		.WORD _atr5_lewis
		.WORD _atr6_robocop
		.WORD _atr7_ending
		.WORD _atr8_repair
_disp_spr_lib:
		.WORD 0 ; _spr0_ocean
		.WORD 0 ; _spr1_probe
		.WORD _spr2_title
		.WORD 0
		.WORD 0
		.WORD 0
		.WORD 0
		.WORD _spr7_ending
		.WORD _spr8_repair
_disp_pal_lib:
		.WORD _pal0_ocean
		.WORD _pal1_probe
		.WORD _pal2_title
		.WORD _pal3456_frame
		.WORD _pal3456_frame
		.WORD _pal3456_frame
		.WORD _pal3456_frame
		.WORD _pal7_ending
		.WORD _pal8_repair

_disp_chr1_list:
		.BYTE	$18,$18,$19,$1C,$1C,$1C,$1D,$1E,$16
_disp_chr0_list:
		.BYTE	$1C,$1C,$1B,$18,$18,$18,$18,$1E,$16
_special_controls_flags_list:
		.BYTE	  0,  0,$FF,  0,  0,  0,  0,  0,  0
_disp_spr0hit_use_flags:
		.BYTE	  0,  0,$FF,  0,  0,  0,  0,$FF,$FF
_disp_extra_shifts_list:
		.BYTE	  0,  0,$10,  0,  0,  0,  0,$10,$10
_disp_spr_data_size_list:
		.BYTE	  0
		.BYTE	  0
		.BYTE	[_spr2_title_end - _spr2_title - 1]
		.BYTE	  0
		.BYTE	  0
		.BYTE	  0
		.BYTE	  0
		.BYTE	[_spr7_ending_end - _spr7_ending - 1]
		.BYTE	[_spr8_repair_end - _spr8_repair - 1]

; displays tilemaps
;
_pal0_ocean:
		.BYTE	 $F,  0,$10,$20, $F,  1,$11,$21, $F,  2,$12,$10, $F,$16,$26, $F
		.BYTE	 $F,$16,$26, $F, $F,  5,$15,$25, $F,  6,$16,$26, $F,  7,$17,$27
_pal1_probe:
		.BYTE	 $F,$16,$26, $F, $F, $F, $F, $F, $F, $F, $F, $F, $F,$12,$22,$32
		.BYTE	 $F,$16,$26, $F, $F,$21,$22,$32, $F,$21,$22,$32, $F,$21,$22,$32
_pal2_title:
		.BYTE	 $F,$27,$26,$30, $F,$11,$21,$30, $F,$27,$38,$30, $F,$27,$36,$30
		.BYTE	 $F,$27,$36,$30, $F,$27,$38,$30, $F,$16,$26, $F, $F,$11,$21,$30
_pal3456_frame:
		.BYTE	 $F,$17,$27,$30, $F,  0,$10,$20, $F,$12,$22,$30, $F, $A,$2A, $F
		.BYTE	 $F,  7,$17,$20, $F,  0,$10,$20, $F,$12,$22,$30, $F,$21,$10,$20
_pal7_ending:
		.BYTE	 $F,$27,$36,$30, $F,$11,$21,$30, $F,$27,$38,$30, $F,$21,$10,$20
		.BYTE	 $F,$27,$36,$30, $F,$11,$21,$30, $F,$27,$38,$30, $F,$21,$10,$20
_pal8_repair:
		.BYTE	 $F,  8,$18,$2B, $F,  0,$10,$20, $F, $B,$1B,$2B, $F, $C,$1C,$2C
		.BYTE	 $F,  0,$10,$20, $F,  6,$16,$26, $F, $B,$1B,$2B, $F, $C,$1C,$2C

; displays attributes, even the size of uploaded data is always $40 bytes, the
; attributes size here is only $38 maximum for each display, some of them are
; arranged to have common data for optimal stogare, the last 8 bytes are sending
; garbage, including the first 8 bytes of the first tilemap
;
_atr0_ocean:
		.BYTE	$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
		.BYTE	$FF,$3F, $F, $F, $F, $F, $F,$FF
		.BYTE	$FF,$88,$AA,$AA,$AA,$AA,$AA,$CC
		.BYTE	$FF,$34,  5,  5,  5,  5,  5,$FC
		.BYTE	$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
		.BYTE	$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
_atr2_title:
		.BYTE	$55,$55,$55,$55,$55,$55,$55,$55
		.BYTE	$76,$59,$DA,$5A,$55,$CE,$77,$59
		.BYTE	$FF,$55,$DD,$55,$55,$8C,$AF,$EC
		.BYTE	$FF,$D5,$55,$55,$55,$5D,$53,$DD
		.BYTE	$FF,$55,$55,$55,$55,$55,$55,$55
		.BYTE	$FF,$FF,$77,$55,$55,$55,$D5,$FD
		.BYTE	$55,$55,$55,$55,$55,$55,$55,$55
_atr5_lewis:
		.BYTE	$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
		.BYTE	$AF,$AB,$AF,$AB,$AE,$AF,$AE,$AF
		.BYTE	$EE,$FF,$FF,$55,$55,$FF,$FF,$BB
		.BYTE	$EE,$FF,$FF,$11,$44,$DF,$FF,$BB
		.BYTE	$EE,$FF,$F5,$F5,$F5,$F5,$FF,$BB
		.BYTE	$AE,$AF,$AF,$AF,$AF,$AF,$AF,$AB
_atr6_robocop:
		.BYTE	$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
		.BYTE	$AF,$AB,$AF,$AB,$AE,$AF,$AE,$AF
		.BYTE	$EE,$FF,$9B,$A6,$A5,$DD,$FF,$BB
		.BYTE	$EE,$FF,$A9,$A8,$AA,$DD,$FF,$BB
		.BYTE	$EE,$FF,$FA,$FA,$FA,$FD,$FF,$BB
		.BYTE	$AE,$AF,$AF,$AF,$AF,$AF,$AF,$AB
_atr7_ending:
		.BYTE	$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
		.BYTE	$BF,$AF,$AF,$AF,$FF,$FF,$DF,$FF
		.BYTE	$AA,$AA,$AA,$AA,$32,$4C,$55,$CD
		.BYTE	$AA,$AA,$AA,$AA,$5A,$54,  5,$58
		.BYTE	$AA,$AA,$AA,$AA,$55,$55,$55,$55
		.BYTE	$AA,$AA,$AA,$AA,$55,$FF,$FF,$FF
		.BYTE	  0,  0,  0,  0,$FF,$FF,$FF,$FF
_atr4_frame:
		.BYTE	$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
		.BYTE	$AF,$AB,$AF,$AB,$AE,$AF,$AE,$AF
		.BYTE	$EE,$FF,$FF,$FF,$FF,$FF,$FF,$BB
		.BYTE	$EE,$FF,$FF,$FF,$FF,$FF,$FF,$BB
		.BYTE	$EE,$FF,$FF,$FF,$FF,$FF,$FF,$BB
		.BYTE	$AE,$AF,$AF,$AF,$AF,$AF,$AF,$AB
_atr1_probe:
		.BYTE	$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
		.BYTE	$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
		.BYTE	$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
		.BYTE	$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0
_atr8_repair:
		.BYTE	$FF, $C,  3,  3,$FF,$FF,$C0,$F0
		.BYTE	$3F,  0,  0,$C0,$FF,$FF,$BA,$EA
		.BYTE	  0,$5C,$9F,$FA,$FF,$FF,$FF,$AF
		.BYTE	  5,  5,  5,$DD,$EF,$FF,$BB,$EA
		.BYTE	  0,  0,  0,$DD,$BA,$EF,$FF,$FF
		.BYTE	$FF,$FF,  0,$DD,$FF,$FE,$FE,$FF
		.BYTE	$AF,$2F,$A0, $D,$AF,$8F,$AF,$8F

_tlm0_ocean:
		INCBIN	"../tlm/tlm0.pak"
_tlm1_probe:
		INCBIN	"../tlm/tlm1_a.pak"
_tlm2_title:
		INCBIN	"../tlm/tlm2.pak"
;_tlm3_hiscore:
;		INCBIN	"../tlm/tlm3.pak"							; actual tilemap unused, used regular frame instead
_tlm4_frame:
		INCBIN	"../tlm/tlm4.pak"
_tlm5_lewis:
		INCBIN	"../tlm/tlm5.pak"
_tlm6_robocop:
		INCBIN	"../tlm/tlm6.pak"
_tlm7_ending:
		INCBIN	"../tlm/tlm7.pak"
_tlm8_repair:
		INCBIN	"../tlm/tlm8.pak"

_spr2_title:
		.BYTE	$65,$FF,$FF,$BD,$4F,$01,$01,$A8,$4F,$03,$01,$B0,$4F,$05,$01,$B8
		.BYTE	$4F,$07,$01,$C0,$4F,$09,$01,$C8,$4F,$0B,$01,$D0,$4F,$0D,$01,$E0
		.BYTE	$5F,$0F,$00,$70,$5F,$11,$00,$78,$5F,$13,$00,$80,$5F,$15,$00,$88
		.BYTE	$6F,$17,$00,$A0,$6F,$19,$00,$A8,$7F,$1B,$00,$A8,$7F,$1D,$00,$B0
		.BYTE	$7F,$1F,$00,$B8,$7F,$21,$00,$C0,$7F,$23,$00,$C8,$8F,$25,$00,$C0
		.BYTE	$8F,$27,$00,$C8,$C7,$29,$03,$90,$C7,$2B,$03,$98,$C7,$2D,$03,$A0
		.BYTE	$C7,$2F,$03,$A8,$C7,$31,$03,$B0,$C7,$33,$03,$B8
_spr2_title_menu0:
		.BYTE	_m0p,$59,$02,$20
		.BYTE	_m0p,$5B,$02,$28
		.BYTE	_m0p,$35,$02,$30
		.BYTE	_m0p,$57,$02,$38
		.BYTE	_m0p,$5B,$02,$40
_spr2_title_menu1:
		.BYTE	_m1p,$39,$02,$20
		.BYTE	_m1p,$51,$02,$28
		.BYTE	_m1p,$4F,$02,$30
		.BYTE	_m1p,$5B,$02,$38
		.BYTE	_m1p,$45,$02,$40
		.BYTE	_m1p,$4F,$02,$48
		.BYTE	_m1p,$5D,$02,$50
		.BYTE	_m1p,$3D,$02,$58
_spr2_title_menu2:
		.BYTE	_m2p,$59,$02,$20
		.BYTE	_m2p,$51,$02,$28
		.BYTE	_m2p,$5D,$02,$30
		.BYTE	_m2p,$4F,$02,$38
		.BYTE	_m2p,$3B,$02,$40
		.BYTE	_m2p,$51,$02,$50 ; on/off
		.BYTE	_m2p,$4F,$02,$58
		.BYTE	_m2p,$01,$02,$60
_spr2_title_menu3:
		.BYTE	_m3p,$4D,$02,$20
		.BYTE	_m3p,$5D,$02,$28
		.BYTE	_m3p,$59,$02,$30
		.BYTE	_m3p,$45,$02,$38
		.BYTE	_m3p,$39,$02,$40
		.BYTE	_m3p,$51,$02,$50 ; on/off
		.BYTE	_m3p,$4F,$02,$58
		.BYTE	_m3p,$01,$02,$60
_spr2_title_end:

_spr7_ending:
		.BYTE	$7D,$30,$FF,$00,$7F,$F7,$02,$80,$7F,$43,$02,$88,$7F,$F8,$02,$90
		.BYTE	$7F,$F9,$02,$98,$7F,$FA,$02,$A0,$87,$FB,$02,$80,$87,$FC,$02,$88
		.BYTE	$87,$FD,$02,$90,$87,$FE,$02,$98
_spr7_ending_end:

_spr8_repair:
		.BYTE	$66,$30,$FF,$30,$40,$F6,$00,$A8,$48,$F7,$00,$A8,$1F,$BF,$02,$A8
		.BYTE	$27,$C0,$02,$80,$27,$C1,$02,$88,$27,$C2,$01,$98,$27,$C3,$01,$A0
		.BYTE	$27,$C4,$02,$B8,$27,$C5,$02,$C0,$2F,$C6,$02,$80,$2F,$C7,$02,$88
		.BYTE	$2F,$C8,$00,$B8,$37,$C9,$02,$80,$3F,$CA,$02,$C0,$3F,$CB,$02,$C8
		.BYTE	$47,$CC,$02,$C8,$4F,$CD,$02,$C8,$4F,$CE,$02,$D0,$4F,$CF,$02,$D8
		.BYTE	$57,$D0,$02,$D0,$57,$D1,$02,$D8,$5F,$D2,$02,$D0,$5F,$D3,$02,$D8
		.BYTE	$67,$D4,$02,$D0,$67,$D5,$02,$D8,$6F,$D6,$02,$80,$6F,$D7,$02,$B0
		.BYTE	$77,$D8,$02,$80,$77,$D9,$02,$B0,$77,$DA,$02,$B8,$7F,$DB,$02,$B0
		.BYTE	$7F,$DC,$02,$B8,$87,$DD,$02,$90,$87,$DE,$02,$98,$8F,$DF,$02,$C8
		.BYTE	$8F,$E0,$02,$D0,$8F,$E1,$02,$D8,$97,$E2,$02,$B0,$97,$E3,$02,$B8
		.BYTE	$97,$E4,$02,$C0,$97,$E5,$02,$C8,$97,$E6,$02,$D0,$97,$E7,$02,$D8
		.BYTE	$9F,$E8,$02,$B0,$9F,$E9,$02,$B8,$9F,$EA,$02,$C0,$9F,$EB,$02,$C8
		.BYTE	$9F,$EC,$02,$D0,$9F,$ED,$02,$D8,$A7,$EE,$02,$B0,$A7,$EF,$02,$C0
		.BYTE	$A7,$F0,$02,$C8,$A7,$F1,$02,$D0,$A7,$F2,$02,$D8,$AF,$F3,$02,$B0
		.BYTE	$AF,$F4,$02,$B8,$B7,$F5,$02,$B8,$9A,$F8,$00,$17,$A2,$F9,$00,$17
_spr8_repair_end:

_ovl_repair_head:
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
		
_ovl_repair_rarm0:
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
_ovl_repair_rarm1:
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

; --------------------------------
; unused palm hand repair graphics tilemaps 4x4
;_ovl_repair_larm:
;		.BYTE	$60,$61,  0,  0
;		.BYTE	  0,$6A,$6B,$6C
;		.BYTE	$77,$78,$79,$7A
;		.BYTE	  0,  0,  0,  0

;		.BYTE	$B8,$B9,  0,  0
;		.BYTE	  0,$BA,$6B,$6C
;		.BYTE	$77,$78,$79,$7A
;		.BYTE	  0,  0,  0,  0

;		.BYTE	$B8,$B9,  0,  0
;		.BYTE	  0,$BB,$BC,$6C
;		.BYTE	$77,$78,$BD,$7A
;		.BYTE	  0,  0,  0,  0

_ovl_repair_body:
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

; 6th rows are unused
;
_ovl_repair_legs:
		.BYTE	  5,  6,  7,  8,  9, $A, $B, $C
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

byte_476DC:
		.BYTE	$30,$90,$60,$90,  0

_repair_small_frames_lib:
		.BYTE	  5,  6,  7,  8,  9, $A, $B, $C
		.BYTE	$14,$15,$16,$17,$18,$19,$1A,$1B
		.BYTE	$25,$26,$27,$28,$29,  0,$2A,$2B
		.BYTE	  0,$35,$36,$37,$38,  0,$39,$3A
		.BYTE	  0,  0,$49,$4A,$4B,  0,$4C,$4D
		.BYTE	  0,  0,$56,$57,$58,  0,$59,$5A

		.BYTE	  0,  0,$94,$95,$96,  0,  0,  0
		.BYTE	  0,  0,$97,  0,$98,$99,  0,  0
		.BYTE	  0,$9A,$9B,$9C,$9D,$9E,  0,  0
		.BYTE	  0,$9F,$A0,$A1,$A2,$A3,$A4,$A5
		.BYTE	  0,$A6,$A7,$A8,$A9,$AA,$AB,$AC
		.BYTE	$AD,$AE,$AF,$B0,$B1,$B2,$B3,$B4

		.BYTE	$A6,$A7,$A8,$A9,$AA,$AB,$AC,  0
		.BYTE	$AE,$AF,$B0,$B1,$B2,$B3,$B4,  0
		.BYTE	$B5,$B6,$B7,$B8,$B9,$BA,$BB,$BC
		.BYTE	$BD,$BE,$BF,$C0,$C1,$C2,$C3,$C4
		.BYTE	$C5,$C6,$C7,$C8,$C9,$CA,$CB,$CC
		.BYTE	$CD,$CE,$CF,$D0,$D1,$D2,$D3,  0

		.BYTE	$D4,$B5,$B6,  0,$AC,  0,  0,  0
		.BYTE	$D5,$BD,$BE,  0,$BB,  0,$D6,  0
		.BYTE	$D7,$C5,$C6,  0,$C3,$D8,  0,  0
		.BYTE	$D9,$CD,$CE,  0,$CB,$CC,$DA,  0
		.BYTE	$DB,$DC,$DD,$DE,  0,  0,$DF,$E0
		.BYTE	  3,  4,  5,  0,$E1,  0,$E2,$E3

_repair_small_frame_txt0:
		.BYTE	$45,$F1,$F2,$F3
_repair_small_frame_txt1:
		.BYTE	$F4,$F5,$F6,$F7
_repair_small_frame_txt2:
		.BYTE	$45,$F8,$F9,$FA
_repair_small_frame_txt3:
		.BYTE	$FB,$FC,$FD,$FE

; =============== S U B	R O U T	I N E =======================================
; regular bios msg typing with header text and skippable
;
_bios_msg_display:
		LDY		#0
		.BYTE	$2C
		
; =============== S U B	R O U T	I N E =======================================
_bios_msg_display_no_skip:
		LDY		#$FF
		STY		_log_msg_skip_forbid
		STA		_tmp_idx0
		ASL
		TAX
		LDA		_msg_lib,X
		STA		_txt_ptr
		LDA		_msg_lib+1,X
		STA		_txt_ptr+1
		LDA		#0
		STA		_event_idx
		STA		_tmp_idx1
		STA		_tmp_idx2
		STA		_delay_counter
		LDA		#$FF
		STA		_pad_block_mask
		LDA		#7
		STA		_event_reload_idx
		JSR		_log_clear_buf
		LDA		#4
		JSR		_screen_display
		PRINTF	_res_bios_hdr
		LDA		#$80
		STA		_PPU_CTRL
		JMP		_event_exec

_res_bios_hdr:
		.BYTE	$21,$23
		STRING	_R,_O,_M,_,_B,_I,_O,_S
		.BYTE	$21,$43
		STRING	_D,_A,_T,_A,_,_R,_E,_T,_R,_I,_E,_V,_A,_L,_,_S,_Y,_S,_T,_E,_M
		.BYTE	$21,$63
		STRING	_V,_E,_R,_S,_I,_O,_N,_,_3,_DT,_O
		.BYTE	$FF

_res_game_over:
		.BYTE	$21,$CB
		STRING	_G,_A,_M,_E,_,_,_O,_V,_E,_R
		.BYTE	$FF

_res_copyrights:
		.BYTE	$21,$2B
		STRING	_R,_O,_B,_O,_C,_O,_P,_TM,_,_3
		.BYTE	$21,$66
		STRING	_T,_M,_,_A,_N,_D,_,_C,_O,_P,_Y,_R,_I,_G,_H,_T,_,_1,_9,_9,_2
		.BYTE	$21,$83
		STRING	_O,_R,_I,_O,_N,_,_P,_I,_C,_T,_U,_R,_E,_S,_,_C,_O,_R,_P,_O,_R,_A,_T,_I,_O,_N
		.BYTE	$21,$A7
		STRING	_A,_L,_L,_,_R,_I,_G,_H,_T,_S,_,_R,_E,_S,_E,_R,_V,_E,_D
		.BYTE	$21,$C2
		STRING	_T,_M,_,_D,_E,_S,_I,_G,_N,_A,_T,_E,_S,_,_A,_,_T,_R,_A,_D,_E,_M,_A,_R,_K,_,_O,_F
		.BYTE	$21,$E3
		STRING	_O,_R,_I,_O,_N,_,_P,_I,_C,_T,_U,_R,_E,_S,_,_C,_O,_R,_P,_O,_R,_A,_T,_I,_O,_N
		.BYTE	$22,$29
		STRING	_C,_O,_P,_Y,_R,_I,_G,_H,_T,_,_1,_9,_9,_2
		.BYTE	$22,$46
		STRING	_O,_C,_E,_A,_N,_,_O,_F,_,_A,_M,_E,_R,_I,_C,_A,_CO,_I,_N,_C,_DT
		.BYTE	$22,$66
		STRING	_L,_I,_C,_E,_N,_S,_E,_D,_,_B,_Y,_,_N,_I,_N,_T,_E,_N,_D,_O,_DT
		.BYTE	$22,$86
		STRING	_C,_A,_H,_4,_E,_3,_,_B,_F,_S,_,_G,_M,_B,_H,_,_2,_O,_1,_6
		.BYTE	$FF

_lvl0_intro_log_lib:
		.WORD _lvl0_intro_log0
		.WORD _lvl0_intro_log1
		.WORD _lvl0_intro_log2

_lvl0_intro_log0:
		.BYTE	_C,_O,_M,_M,_A,_N,_D,_DT,_C,_O,_M,_SCL
		.BYTE	_L,_O,_A,_D,_,_B,_I,_O,_S,_SCL
		.BYTE	_M,_E,_M,_O,_R,_Y,_,_S,_E,_T,_SCL
		.BYTE	_S,_Y,_S,_T,_E,_M,_,_S,_T,_A,_T,_U,_S,_SCL
		.BYTE	_O,_K,_DT,_SCL,_DLY,_CLR
		.BYTE	_C,_R,_I,_M,_E,_,_I,_N,_,_P,_R,_O,_G,_R,_E,_S,_S,_DT,_SCL
		.BYTE	_P,_E,_N,_A,_L,_,_C,_O,_D,_E,_,_1,_3,_9,_DT,_1,_SCL
		.BYTE	_I,_N,_,_P,_U,_R,_S,_U,_I,_T,_SCL,_DLY,_CLR,_NXT
_lvl0_intro_log1:
		.BYTE	_V,_O,_I,_C,_E,_,_A,_N,_A,_L,_Y,_S,_I,_S,_SCL
		.BYTE	_O,_F,_F,_I,_C,_E,_R,_,_L,_E,_W,_I,_S,_CO,_A,_N,_N,_E,_SCL,_DLY,_SCL
		.BYTE	_C,_U,_R,_R,_E,_N,_T,_,_P,_O,_S,_I,_T,_I,_O,_N,_DT,_SCL
		.BYTE	_E,_S,_P,_O,_S,_I,_T,_O,_,_A,_N,_D,_,_C,_H,_A,_L,_M,_E,_R,_S,_,_B,_L,_V,_D,_DT,_SCL,_DLY,_CLR,_NXT
_lvl0_intro_log2:
		.BYTE	_I,_,_A,_M,_,_O,_N,_,_M,_Y,_,_W,_A,_Y,_,_L,_E,_W,_I,_S,_DT,_DT,_DT,_SCL,_DLY,_END

_msg_lib:
		.WORD _msg0_lvl0_briefing
		.WORD _msg1_lvl1_briefing
		.WORD _msg2_lvl2_briefing
		.WORD _msg3_lvl3_briefing
		.WORD _msg4_lvl4_briefing
		.WORD 0 ; _res_copyrights
		.WORD _msg6_prepare_to_repair
		.WORD _msg7_confirm_repair
		.WORD _msg8_get_out_of_there

_msg0_lvl0_briefing:
		.BYTE	_C,_A,_D,_I,_L,_L,_A,_C,_,_H,_E,_I,_G,_H,_T,_S,_CO,_,_D,_U,_S,_K,_DT,_SCL
		.BYTE	_DLY,_SCL
		.BYTE	_R,_E,_S,_C,_U,_E,_,_Y,_O,_U,_R,_,_T,_R,_A,_P,_P,_E,_D,_SCL
		.BYTE	_C,_O,_L,_L,_E,_A,_G,_U,_E,_S,_,_F,_R,_O,_M,_,_T,_H,_E,_SCL
		.BYTE	_C,_L,_U,_T,_C,_H,_E,_S,_,_O,_F,_,_T,_H,_E,_SCL
		.BYTE	_E,_V,_I,_L,_,_S,_P,_L,_A,_T,_T,_E,_R,_P,_U,_N,_K,_S,_DT,_DT,_DLY,_SCL
		.BYTE	_SCL
		.BYTE	_T,_H,_I,_N,_G,_S,_,_M,_A,_Y,_,_G,_O,_,_O,_N,_SCL
		.BYTE	_B,_E,_H,_I,_N,_D,_,_C,_L,_O,_S,_E,_D,_,_D,_O,_O,_R,_S,_DT,_DT,_DT,_SCL
		.BYTE	_DLY,_END
_msg1_lvl1_briefing:
		.BYTE	_R,_O,_C,_K,_E,_T,_,_M,_O,_T,_O,_R,_S,_,_F,_A,_C,_T,_O,_R,_Y,_DT,_SCL
		.BYTE	_DLY,_SCL
		.BYTE	_N,_E,_G,_O,_T,_I,_A,_T,_E,_,_T,_H,_E,_SCL
		.BYTE	_P,_E,_R,_I,_L,_O,_U,_S,_,_O,_B,_S,_T,_A,_C,_L,_E,_S,_SCL
		.BYTE	_I,_N,_,_T,_H,_E,_,_O,_L,_D,_,_A,_B,_A,_N,_D,_O,_N,_E,_D,_SCL
		.BYTE	_F,_A,_C,_T,_O,_R,_Y,_DT,_DLY,_SCL
		.BYTE	_SCL
		.BYTE	_A,_N,_D,_,_M,_A,_K,_E,_,_Y,_O,_U,_R,_,_W,_A,_Y,_,_T,_O,_,_T,_H,_E,_SCL
		.BYTE	_U,_L,_T,_I,_M,_A,_T,_E,_,_C,_O,_N,_F,_L,_I,_C,_T,_DT,_DT,_DT,_SCL
		.BYTE	_DLY,_END
_msg2_lvl2_briefing:
		.BYTE	_C,_A,_D,_I,_L,_L,_A,_C,_,_H,_E,_I,_G,_H,_T,_S,_CO,_,_D,_A,_Y,_DT,_SCL
		.BYTE	_DLY,_SCL
		.BYTE	_H,_A,_V,_I,_N,_G,_,_D,_O,_N,_N,_E,_D,_,_Y,_O,_U,_R,_SCL
		.BYTE	_P,_O,_W,_E,_R,_F,_U,_L,_,_J,_E,_T,_,_P,_A,_C,_K,_CO,_SCL
		.BYTE	_Y,_O,_U,_,_S,_T,_O,_R,_M,_,_T,_H,_R,_O,_U,_G,_H,_,_T,_H,_E,_SCL
		.BYTE	_W,_R,_E,_C,_K,_E,_D,_,_S,_T,_R,_E,_E,_T,_S,_DT,_DT,_DT,_DLY,_SCL
		.BYTE	_SCL
		.BYTE	_T,_O,_W,_A,_R,_D,_,_T,_H,_E,_,_R,_E,_H,_A,_B,_S,_SCL
		.BYTE	_S,_U,_P,_E,_R,_,_T,_A,_N,_K,_DT,_DT,_DT,_SCL
		.BYTE	_DLY,_SCL
		.BYTE	_T,_H,_E,_,_M,_A,_I,_N,_,_G,_U,_N,_,_I,_S,_SCL
		.BYTE	_A,_I,_M,_E,_D,_,_A,_T,_,_I,_N,_N,_O,_C,_E,_N,_T,_SCL
		.BYTE	_C,_I,_T,_I,_Z,_E,_N,_S,_DT,_DT,_DT,_,_A,_N,_D,_,_Y,_O,_U,_DT,_SCL
		.BYTE	_DLY,_END
_msg3_lvl3_briefing:
		.BYTE	_W,_A,_L,_K,_,_B,_A,_C,_K,_SCL
		.BYTE	_T,_O,_,_T,_H,_E,_,_O,_C,_P,_,_T,_O,_W,_E,_R,_DT,_SCL
		.BYTE	_DLY,_SCL
		.BYTE	_SCL
		.BYTE	_Y,_O,_U,_R,_,_J,_E,_T,_,_P,_A,_C,_K,_,_I,_S,_,_E,_M,_P,_T,_Y,_DT,_DT,_DT,_SCL
		.BYTE	_DLY,_SCL
		.BYTE	_M,_A,_K,_E,_,_Y,_O,_U,_R,_,_W,_A,_Y,_,_F,_R,_O,_M,_SCL
		.BYTE	_O,_N,_E,_,_D,_I,_S,_T,_A,_N,_T,_,_E,_N,_D,_,_O,_F,_,_T,_O,_W,_N,_SCL
		.BYTE	_B,_A,_C,_K,_,_T,_O,_,_T,_A,_K,_E,_,_O,_N,_,_M,_C,_DT,_D,_A,_G,_G,_E,_T,_T,_SCL
		.BYTE	_DLY,_SCL
		.BYTE	_SCL
		.BYTE	_R,_E,_S,_C,_U,_E,_,_M,_A,_R,_I,_E,_,_A,_N,_D,_,_K,_E,_I,_K,_O,_SCL
		.BYTE	_DLY,_SCL
		.BYTE	_I,_T,_,_I,_S,_,_N,_O,_T,_,_G,_O,_I,_N,_G,_,_T,_O,_,_B,_E,_SCL
		.BYTE	_E,_A,_S,_Y,_DT,_SCL
		.BYTE	_DLY,_END
_msg4_lvl4_briefing:
		.BYTE	_T,_H,_E,_,_T,_O,_P,_,_O,_F,_,_T,_H,_E,_,_O,_C,_P,_,_T,_O,_W,_E,_R,_DT,_SCL
		.BYTE	_DLY,_SCL
		.BYTE	_U,_S,_E,_,_Y,_O,_U,_R,_,_A,_C,_C,_E,_S,_S,_,_S,_P,_I,_K,_E,_SCL
		.BYTE	_T,_O,_,_T,_A,_P,_,_I,_N,_T,_O,_,_T,_H,_E,_,_K,_A,_N,_E,_M,_I,_T,_S,_U,_SCL
		.BYTE	_C,_O,_M,_P,_U,_T,_E,_R,_DT,_SCL
		.BYTE	_DLY,_SCL
		.BYTE	_SCL
		.BYTE	_I,_T,_,_P,_R,_O,_V,_I,_D,_E,_S,_SCL
		.BYTE	_K,_E,_I,_K,_O,_,_W,_I,_T,_H,_,_I,_N,_F,_O,_R,_M,_A,_T,_I,_O,_N,_SCL
		.BYTE	_V,_I,_T,_A,_L,_,_T,_O,_,_Y,_O,_U,_R,_,_S,_A,_F,_E,_T,_Y,_DT,_DT,_DT,_SCL
		.BYTE	_DLY,_SCL
		.BYTE	_SCL
		.BYTE	_L,_O,_O,_K,_,_B,_O,_T,_H,_,_W,_A,_Y,_S,_DT,_DT,_DT,_SCL
		.BYTE	_DLY,_END
_msg8_get_out_of_there:
		.BYTE	_S,_U,_C,_C,_E,_S,_S,_DT,_DT,_DT,_SCL
		.BYTE	_N,_O,_W,_,_G,_E,_T,_,_O,_U,_T,_,_O,_F,_,_T,_H,_E,_R,_E,_DT,_SCL
		.BYTE	_SCL
		.BYTE	_DLY,_END

_msg6_prepare_to_repair:
		.BYTE	_W,_E,_L,_L,_,_D,_O,_N,_E,_CO,_,_O,_F,_F,_I,_C,_E,_R,_DT,_SCL
		.BYTE	_DLY,_SCL
		.BYTE	_P,_R,_E,_P,_A,_R,_E,_,_T,_O,_,_E,_N,_T,_E,_R,_SCL
		.BYTE	_R,_E,_P,_A,_I,_R,_,_L,_A,_B,_O,_R,_A,_T,_O,_R,_Y,_DT,_DT,_DT,_SCL
		.BYTE	_DLY,_END
_msg7_confirm_repair:
		.BYTE	_P,_L,_E,_A,_S,_E,_,_C,_O,_N,_F,_I,_R,_M,_,_R,_E,_P,_A,_I,_R,_S,_DT,_DT,_DT,_SCL
		.BYTE	_DLY,_SCL
		.BYTE	_P,_R,_E,_S,_S,_,_A,_,_T,_O,_,_A,_C,_C,_E,_P,_T,_SCL
		.BYTE	_P,_R,_E,_S,_S,_,_B,_,_T,_O,_,_R,_E,_A,_L,_L,_O,_C,_A,_T,_E,_SCL
		.BYTE	_ASK,_CLR
		.BYTE	_C,_O,_N,_F,_I,_R,_M,_E,_D,_DT,_SCL
		.BYTE	_DLY,_END

		ECHO "bank 5 free space", [SAFE_STUB_ORG-.]

		SAFE_STUB
