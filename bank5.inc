
		ORG $8000, $00

; =============== S U B	R O U T	I N E =======================================
; dumb delay, input arg X
;
_delay:
		LDY		#0
loc_440C1:
		DEY
		BNE		loc_440C1
		DEX
		BNE		loc_440C1
		RTS
		
; =============== S U B	R O U T	I N E =======================================
; start to display the copyright message, then start the main title attract
; event with logos, title and hi-score table
;
_attract_display:
		LDA		#4
		STA		_screen_idx
		JSR		_screen_display
; --
; text display routine, print out a text resource at once on the screen
; they used the same routine in many other places, but with typing method
;
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
		LDA		#<[_msg5_copyrights]					; here X were used before, it is 0 at this point
		STA		_txt_ptr
		LDA		#>[_msg5_copyrights]
		STA		_txt_ptr+1								; display copyrights
loc_44059:
		LDA		(_txt_ptr),Y
		CMP		#$FE
		BEQ		.do_next_line							; $FE - return symbol
		CMP		#$FA
		BEQ		.do_music_and_delay						; $FA - stop symbol
;		CMP		#$C0
;		BCS		loc_4406A								; tiles above $C0 goes unchanged
;		SEC
;		SBC		#$40									; fonts are at 00, so they used delta $40 for regular ascii symbols
;loc_4406A:
		STA		_PPU_DATA
loc_4406A0:
		INC		_txt_ptr
		BNE		loc_44059
		INC		_txt_ptr+1
		JMP		loc_44059
.do_next_line:
		LDA		_intro_txt_ppu_ofs_tmp					; set ppu offset to the next line
		CLC
		ADC		#$20
		STA		_intro_txt_ppu_ofs_tmp
		LDA		_intro_txt_ppu_ofs_tmp+1
		ADC		#0
		STA		_intro_txt_ppu_ofs_tmp+1
		BIT		_PPU_STATUS								; reset ppu offset state just in case
		LDA		_intro_txt_ppu_ofs_tmp+1				; reload new offset
		STA		_PPU_ADDR
		LDA		_intro_txt_ppu_ofs_tmp
		STA		_PPU_ADDR
		JMP		loc_4406A0
; --
.do_music_and_delay:
		JSR		_star_field_init
		LDA		#$81
		STA		_PPU_CTRL								; enable NMI back, select scroll high nt bit
		LDA		#$1D
		JSR		_far_apu_mus_load_prg5_safe				; init title sound
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
		LDX		#3
		JSR		_delay
;		JSR		_far_apu_play_prg5_safe					; do play sync 
		JSR		_star_field_animate
		PLA
		TAX
		DEX
		BNE		.copyright_screen_dealy
		LDA		#0
		STA		_event_idx
		STA		_delay_counter
		STA		_delay_counter+1
		STA		_screen_idx
		LDA		#2
		STA		_event_reload_idx						; set actual title event session index
		LDA		#$FF
		STA		byte_3B1
;		LDA		#8										; not actually used
;		STA		_ppu_scroll_x_override_redundant
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
		STA		byte_3B4
		STA		byte_3B5
		STA		byte_3D7
		STA		_PPU_CTRL
		STA		_PPU_MASK
		STA		_delay_counter
		LDA		#8										; init pad prev value with START button to make sure it won't
		STA		_pad_prev								; skip intro immediately after you press start at the title screen
		LDA		#1
		STA		byte_3D3								
		LDA		#3
		STA		_event_reload_idx
		LDA		#$FF
		STA		byte_3B1
		JSR		_spr_buf_clear
		JSR		_log_clear_buf
		LDA		#4
		STA		_screen_idx
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
		STA		_PPU_CTRL
		STA		_PPU_MASK
		STA		byte_3D4
		STA		_delay_counter
		STA		_delay_counter+1
		STA		_pl_bkcp_continues
		STA		_pad_prev
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
		JSR		_far_apu_mus_load_prg5_safe
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
		JSR		_far_apu_mus_load_prg5_safe
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
; go to the Game Overs screen, test for continues, etc..
;
_game_over_display:
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
		STA		_pad_prev
		LDA		#1
		STA		byte_3D3
		LDA		#6
		STA		_event_reload_idx
		LDA		#$FF
		STA		byte_3B1
		LDA		_pl_bkcp_continues						; test for player 1 continues, as against other
		BEQ		loc_4429E								; 2-player code, this assuming there is only 1 player
		LDA		#1
		STA		_title_menu_idx							; continue are left, set the menu cursor to CONTINUE
		BNE		loc_442AB
loc_4429E:
		LDA		#0
		STA		_title_menu_idx							; or to START and disable continues
		STA		_new_game_start_flag
		LDA		_cur_lvl_idx
		STA		_pl_bckp_lvl_idx
loc_442AB:
		JSR		_spr_buf_clear
		JSR		_log_clear_buf
		LDA		#4
		STA		_screen_idx
		JSR		_screen_display							; draw regular frame tilemap
		JSR		_star_field_init
		BIT		_PPU_STATUS
		LDA		#$21
		STA		_PPU_ADDR
		LDA		#$CB
		STA		_PPU_ADDR
		LDX		#0
loc_442E2:
		LDA		_txt_game_over,X
		STA		_PPU_DATA
		INX
		CPX		#$A
		BCC		loc_442E2
loc_442ED:
		LDA		#$20
		JSR		_far_apu_mus_load_prg5_safe
		LDA		#$81
		STA		_PPU_CTRL
		JMP		_event_exec
_txt_game_over:
		.BYTE	_G,_A,_M,_E,_,_,_O,_V,_E,_R

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
_event_exec:
		LDA		_event_idx
		BEQ		_event_exec
		ASL
		TAX
		LDA		_events_lib,X
		STA		_tptr0
		INX
		LDA		_events_lib,X
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

_spr_presents:
		.BYTE	_P,_R,_E,_S,_E,_N,_T,_S
_spr_oduction:
		.BYTE	_O,_D,_U,_C,_T,_I,_O,_N

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
		JMP		.do_title_generic_handlers
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
		BNE		.probe_logo_overlay_display
		LDA		#$68
		PHA
		LDX		#0										; "PRESENTS" sprite overlay for "OCEAN" logo
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
.probe_logo_overlay_display:
		LDA		_screen_idx
		CMP		#1
		BNE		loc_44445
		BIT		_PPU_STATUS
		LDA		#$22									; write tile patch "PR" for "PRODUCTION"
		STA		_PPU_ADDR
		LDA		#$C
		STA		_PPU_ADDR
		LDA		#$92
		STA		_PPU_DATA
		LDA		#$93
		STA		_PPU_DATA
		LDA		#$84									; sprite overlay for PROBE logo, "A" letter
		STA		_spr_buf+$C3
		LDA		#$34
		STA		_spr_buf+$C0
		LDA		#1
		STA		_spr_buf+$C1
		LDA		#0
		STA		_spr_buf+$C2
		LDA		#$70
		PHA
		LDX		#0										; write "ODUCTION" sprites overlay
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
		JSR		_draw_hiscore_table
		JSR		_star_field_init
loc_4444F:
		LDA		#$81
		STA		_PPU_CTRL
		JSR		_read_pad
		INC		_screen_idx
.do_title_generic_handlers:
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
;		JSR		_far_apu_play_prg5_safe
		JMP		_event_exec
loc_44491:
		LDA		_disp_spr0hit_use
		BNE		loc_444AD
		LDX		#3
		JSR		_delay
		JSR		_read_pad
;		JSR		_far_apu_play_prg5_safe
		LDA		#1
		STA		_logo_skip_forbid_flag
		JMP		_event_exec
loc_444AD:
		LDX		#1
		JSR		_delay
;		JSR		_far_apu_play_prg5_safe
		JSR		_read_pad
		LDY		_title_menu_idx							
;		LDX		byte_44651,Y							; here we table calc the sprite offset pos in buffer
;		LDA		_A_mul_4_table,X					; selected menu item to make it blink idx * 4 previously
;		TAX
		LDX		byte_44651,Y							; but why we just can't have already precalc table for it? lol
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
		JMP		.wait_for_midscreen_change
loc_444E6:
		LDA		#0
		STA		_logo_skip_forbid_flag
loc_444EA:
		LDA		_pad
		CMP		_pad_prev
		BNE		loc_444FC
		STA		_pad_prev
		LDA		_generic_nmi_over_counter
		AND		#$1F
		BNE		loc_44540
loc_444FC:
		LDA		_pad
		AND		#4
		STA		_pad_prev
		BNE		loc_44510
		LDA		_pad
		STA		_pad_prev
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
		LDA		_apu_sound_enable
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
		LDA		_apu_music_enable
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
		STA		_pad_prev
		LDA		#0
		STA		_new_game_start_flag
		STA		_cur_lvl_idx
		STA		_pl_bckp_lvl_idx
		JMP		_intro_display
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
		JMP		.wait_for_midscreen_change
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
		LDA		_apu_sound_enable
		CLC
		ADC		#1
		AND		#1
		STA		_apu_sound_enable
loc_445E7:
		LDA		_title_menu_idx
		CMP		#3
		BNE		.wait_for_midscreen_change
		LDA		_pad
		CMP		byte_3D3
		BEQ		.wait_for_midscreen_change
		STA		byte_3D3
		AND		#9
		BEQ		.wait_for_midscreen_change
		JSR		_reload_delay_counter_2
		LDA		_apu_music_enable
		CLC
		ADC		#1
		AND		#1
		STA		_apu_music_enable
		BEQ		loc_44615
		LDA		#$1D
		JSR		_far_apu_mus_load_prg5_safe
		JMP		.wait_for_midscreen_change
loc_44615:
		LDA		#0
		JSR		_far_apu_mus_load_prg5_safe
.wait_for_midscreen_change:
		BIT		_PPU_STATUS
		BMI		loc_44623
		BVC		.wait_for_midscreen_change
		LDX		#$13									; delay 96 clocks
loc_44623:
		DEX
		BNE		loc_44623
		LDA		#$1A									; mm1_chr0 call - 48 clocks
;		STA		$BFFF									; direct set - 30 clocks to change bank
;		LSR
;		STA		$BFFF
;		LSR
;		STA		$BFFF
;		LSR
;		STA		$BFFF
;		LSR
;		STA		$BFFF
		JSR		_mmc1_chr0
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
;byte_44651:
;		.BYTE	$15,$1C,$24,$2C
byte_44651:
		.BYTE	$54,$70,$90,$B0
byte_44655:
		.BYTE	  5,  8,  8,  8
byte_44659:
		.BYTE	$8F,$9F,$AF,$BF

; =============== S U B	R O U T	I N E =======================================
_event3_intro:
		LDX		#4
		JSR		_delay
;		JSR		_far_apu_play_prg5_safe
		JSR		_read_pad
		LDA		_pad
		CMP		_pad_prev
		BEQ		loc_44684
		STA		_pad_prev
		AND		#9
		BEQ		loc_44684								; test if button is pressed to skip
;		LDA		byte_3B4								; one more leftover, the idx here can be only up to 2
;		CMP		#7
;		BEQ		loc_44684
.i_cmdFA_break:
		RTS
loc_44684:
		LDA		byte_3B4								; 3B4 screen index temp value
		ASL
		TAX
		LDA		_lvl0_intro_log_lib,X
		STA		_txt_ptr
		INX
		LDA		_lvl0_intro_log_lib,X
		STA		_txt_ptr+1
		LDY		byte_3B5								; cur message pos temp value
		LDA		(_txt_ptr),Y							; check for special symbols
		CMP		#$FF
		BEQ		.i_cmdFF_next_screen					; $FF - next screen code
		CMP		#$FE
		BEQ		.i_cmdFE_scroll_up						; $FE - scroll buffer up
;		CMP		#$FD
;		BNE		loc_446A8
;		JMP		.i_cmdFD_short_delay					; $FD - unused short delay
;loc_446A8:
		CMP		#$FC
		BEQ		.i_cmdFC_delay							; $FC - long delay
		CMP		#$FB
		BEQ		.i_cmdFB_clear_buf						; $FB - clear current buf
		CMP		#$FA
		BEQ		.i_cmdFA_break							; $FA code - end of messages
		LDX		byte_3D7								; output buf pos temp value
;		LDA		(_txt_ptr),Y							; why read it one more time?
		STA		_log_msg_buf+$54,X						; start to display at the bottom line of the buf
		INC		byte_3B5
		INC		byte_3D7
		LDA		#0
		STA		_generic_nmi_over_counter
.j_event_exec0:
		JMP		_event_exec
; --------------------------------
.i_cmdFF_next_screen:
		INC		byte_3B4
		LDA		#0
		STA		byte_3D7
		STA		byte_3B5
		INC		_screen_idx
		LDA		_screen_idx
		JSR		_screen_display
		LDA		#$81
		STA		_PPU_CTRL
		JMP		_event_exec
; --------------------------------
.i_cmdFE_scroll_up:
		LDA		_generic_nmi_over_counter
		AND		#$F
		BNE		.j_event_exec0
		JSR		_log_msg_buf_scroll_up
		INC		byte_3B5
		LDA		#0
		STA		byte_3D7
		JMP		_event_exec
; --------------------------------
.i_cmdFB_clear_buf:
		LDX		#$6F
		LDA		#0
loc_446FF:
		STA		_log_msg_buf,X
		DEX
		BPL		loc_446FF
		INC		byte_3B5
		LDA		#0
		STA		byte_3D7
		JMP		_event_exec
; --------------------------------
.i_cmdFC_delay:
		LDA		_delay_counter
		CLC
		ADC		#1
		STA		_delay_counter
		CMP		#$3F
		BCC		.j_event_exec0
		INC		byte_3B5
		LDA		#0
		STA		byte_3D7
		STA		_delay_counter
		JMP		_event_exec
; --------------------------------
;.i_cmdFD_short_delay:
;		LDA		_delay_counter
;		CLC
;		ADC		#1
;		STA		_delay_counter
;		CMP		#7
;		BCC		.j_event_exec0
;		INC		byte_3B5
;		LDA		#0
;		STA		byte_3D7
;		STA		_delay_counter
;		JMP		_event_exec
		
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
		BCS		_title_screen_display
		JMP		_hi_score_entry_event
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
		JSR		_delay
loc_4487B:
		BIT		_PPU_STATUS
		BMI		loc_448B8
		BVC		loc_4487B
		NOP												; delay 66 clocks
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
;		NOP
;		NOP
;		NOP
;		NOP
;		NOP
;		NOP
;		NOP
;		NOP
;		NOP
		LDA		#$1F									; direct delay 30 clocks
;		STA		$BFFF									; with call - 48 clocks
;		LSR
;		STA		$BFFF
;		LSR
;		STA		$BFFF
;		LSR
;		STA		$BFFF
;		LSR
;		STA		$BFFF
		JSR		_mmc1_chr0
loc_448B8:
;		JSR		_far_apu_play_prg5_safe
		JMP		_event_exec

; =============== S U B	R O U T	I N E =======================================
; used to redisplay the title screen after the game over or ending sequences
; as well after the hi-score entry screen
;
_title_screen_display:
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
;		LDA		#8										; not actually used
;		STA		_ppu_scroll_x_override_redundant
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
		JSR		_far_apu_mus_load_prg5_safe
		LDA		#1
		STA		_pad_prev
		STA		byte_3D3
		JMP		_event_exec

; =============== S U B	R O U T	I N E =======================================
_hi_score_entry_event:
		LDA		byte_3D4
		BNE		loc_44965
		JSR		_sort_hiscore_test
		LDA		_hi_score_line_idx
		CMP		#6
		BCS		_title_screen_display
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
		JSR		_draw_hiscore_table
		JSR		_star_field_init
		LDA		#$FF
		STA		byte_3D4
		LDA		#$81
		STA		_PPU_CTRL
loc_44965:
		JSR		_star_field_animate
;		JSR		_far_apu_play_prg5_safe
		JSR		_read_pad
		LDX		_hi_score_line_idx
		LDA		_hi_score_ram_ofs_list,X
		CLC
		ADC		byte_3D7
		TAX
		LDA		_pad
		CMP		_pad_prev
		BEQ		loc_44987
		STA		_pad_prev
		JMP		loc_44992
loc_44987:
		LDA		_generic_nmi_over_counter
		AND		#$1F
		BEQ		loc_44992
		CMP		#$14
		BNE		.j_event_exec3
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
		BEQ		.j_event_exec3
		JMP		_title_screen_display
.j_event_exec3:
		JMP		_event_exec
		
_hi_score_ram_ofs_list:
		.BYTE	$75,$84,$93,$A2,$B1,$C0

; =============== S U B	R O U T	I N E =======================================
_event5_repair:
		LDX		#1
		JSR		_delay
;		LDA		#$66									; draw sprite0 for screen split effect
;		STA		_spr_buf								; for unknown reason, it is done in two
;		LDA		#$30									; different places, one in the display
;		STA		_spr_buf+1								; drawing routine, one is here hardcoded
;		LDA		#$FF
;		STA		_spr_buf+2
;		LDA		#$30
;		STA		_spr_buf+3
		LDA		_tmp3C3									; draw cursor
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
		STA		_str_ptr
		STA		_str_ptr+1
		LDX		_repair_body_index_request
		LDY		_repair_update_flag_list,X
		LDA		_cur_head_percent,Y
		STA		_str_ptr
		LDA		#$E6
		STA		_hex2dec_tile_delta
		LDX		#<[_repair_cur_percent_disp_buf_tmp]
		LDY		#>[_repair_cur_percent_disp_buf_tmp]
		JSR		_hex2str
		LDA		#$9A									; draw repair tank
		STA		_spr_buf+$F0
		LDA		#$F8
		STA		_spr_buf+$F1
		LDA		#0
		STA		_spr_buf+$F2
		LDA		#$17
		STA		_spr_buf+$F3
		STA		_spr_buf+$F7
		LDA		#$A2
		STA		_spr_buf+$F4
		LDA		#$F9
		STA		_spr_buf+$F5
		LDA		#0
		STA		_spr_buf+$F6
		JSR		_read_pad
		LDA		_pad
		AND		#8
		BEQ		loc_44AB8
		LDA		#0
		STA		_event_reload_idx
		LDA		#7
		JMP		_bios_msg_display_ex
		RTS
loc_44AB8:
		LDA		_cur_repair_tanks						; draw only single digit of tanks if more than 9, draw always 9
		CMP		#$A										; there were original bug, drawing % instead, since you doubtly
		BCC		loc_44AC1								; obtain more than 10 at once and this case never tested
		LDA		#9
loc_44AC1:
		CLC
		ADC		#$E6
		STA		_cur_repair_tanks_number_tile_tmp
		LDA		_pad
		AND		#1
		CMP		_pad_prev
		BEQ		loc_44AF1
		STA		_pad_prev
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
		NOP												; 60 clocks delay
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
;		NOP
;		NOP
;		NOP
;		NOP
;		NOP
;		NOP
;		NOP
;		NOP
;		NOP
		LDA		#$17									; 30 clocks direct set to change bank
;		STA		$BFFF
;		LSR
;		STA		$BFFF
;		LSR
;		STA		$BFFF
;		LSR
;		STA		$BFFF
;		LSR
;		STA		$BFFF
		JSR		_mmc1_chr0								; 48 clocks call
loc_44B2B:
		JSR		_repair_cursor_input
		JSR		_repair_cursor_animate
		JSR		_dpad_convert
;		JSR		_far_apu_play_prg5_safe
		JMP		_event_exec

; =============== S U B	R O U T	I N E =======================================
_event6_game_over:
		LDX		#3
		JSR		_delay
;		JSR		_far_apu_play_prg5_safe
		JSR		_star_field_animate
		JSR		_read_pad								; read pad
		LDA		_pad
		BEQ		loc_44B5A								; if nothing is pressed, do times
		LDA		_pad_prev								; check if _pad_prev is zero, so we has released any buttons before	
		BEQ		.game_over_set_max_timers				; then jump to max timers set!
		JMP		.game_over_timers_increase
loc_44B5A:
		LDA		#0
		STA		_pad_prev
		BEQ		.game_over_timers_increase
.game_over_set_max_timers:
		LDA		#2										; here we set timers to max value and jump to test it and so skip!
		STA		_delay_counter+1						; why don't you just jupm to skip here?! maybe because it's mean some
		LDA		#$30									; kind of reinitialization of the same timers used by the title screen
		STA		_delay_counter							
		BNE		.game_over_timers_test
.game_over_timers_increase:
		LDA		_delay_counter
		CLC
		ADC		#1
		STA		_delay_counter
		LDA		_delay_counter+1
		ADC		#0
		STA		_delay_counter+1
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
		STA		byte_3D4
		LDA		#3
		STA		_screen_idx
		JMP		_hi_score_entry_event
.j_event_exec2:
		JMP		_event_exec

; =============== S U B	R O U T	I N E =======================================
_event7_log_msg:
		LDX		#3
		JSR		_delay
;		JSR		_far_apu_play_prg5_safe
		LDA		_log_msg_skip_forbid
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
		LDY		byte_3B5								; msg pos temp value
		LDA		(_txt_ptr),Y
		CMP		#$FD
		BNE		loc_44BE1								; special $FD cmd, force forbid input skip
		LDA		#$FF									; used only in repair lab confirm message
		STA		_log_msg_skip_forbid
		INC		byte_3B5
		JMP		_event_exec
loc_44BE1:
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
		LDX		byte_3D7
;		LDA		(_txt_ptr),Y							; no need to read it one more time
;		SEC
;		SBC		#$40
		STA		_log_msg_buf+$54,X
		INC		byte_3B5
		INC		byte_3D7
		LDA		#0
		STA		_generic_nmi_over_counter
.j_event_exec1:
		JMP		_event_exec
; --------------------------------
; unused here, but used in intro copy of this code, no debug though
;.l_cmdFF_next_screen:
;		INC		byte_3B4
;		LDA		#0
;		STA		byte_3D7
;		STA		byte_3B5
;		INC		_screen_idx
;		LDA		_screen_idx
;		JSR		_screen_display
;		LDA		#$81
;		STA		_PPU_CTRL
;		JMP		_event_exec
; --------------------------------
.l_cmdFE_scroll_up:
		LDA		_generic_nmi_over_counter
		AND		#$F
		BNE		.j_event_exec1
		JSR		_log_msg_buf_scroll_up
		INC		byte_3B5
		LDA		#0
		STA		byte_3D7
		JMP		_event_exec
; --------------------------------
.l_cmdFC_delay:
		LDA		_delay_counter
		CLC
		ADC		#1
		STA		_delay_counter
		CMP		#$3F
		BCC		.j_event_exec1
		INC		byte_3B5
		LDA		#0
		STA		byte_3D7
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
		INC		byte_3B5
		LDA		#0
		STA		byte_3D7
		JMP		_event_exec
; --------------------------------
.l_cmdF9_repair_confirmation:
		JSR		_read_pad
		LDA		_pad
		AND		#1
		BEQ		loc_44CA9
		LDX		#0										; instead of _player_number they use this zero initialized X
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
		JMP		_event_exec
loc_44CA9:
		LDA		_pad
		AND		#2
		BNE		loc_44CB3
		JMP		_event_exec
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
;		STA		$BFFF
;		LSR
;		STA		$BFFF
;		LSR
;		STA		$BFFF
;		LSR
;		STA		$BFFF
;		LSR
;		STA		$BFFF
		JSR		_mmc1_chr0
		LDA		_chr1_bank_shadow
;		STA		$DFFF
;		LSR
;		STA		$DFFF
;		LSR
;		STA		$DFFF
;		LSR
;		STA		$DFFF
;		LSR
;		STA		$DFFF
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
		LDA		_repair_cur_percent_disp_buf_tmp+2
		STA		_PPU_DATA
		LDA		_repair_cur_percent_disp_buf_tmp+3
		STA		_PPU_DATA
		LDA		_repair_cur_percent_disp_buf_tmp+4
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
		LDA		_cur_repair_tanks_number_tile_tmp
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
;		LDA		_cur_arms_percent
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
;
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
_nmi0_common:
		JSR		_nmi0_scroll_reload
		LDA		_event_reload_idx
		STA		_event_idx
		LDA		#$1E
		STA		_PPU_MASK
		LDA		_special_controls_flag					; used only for title screen, overrides generic
		BEQ		.nmi0_generic_input						; input controls for the rest of the screens displayed
		LDA		#$A8
;		ORA		_nmi_ppu_ctrl_override_redundant
		STA		_PPU_CTRL
		INC		_generic_nmi_over_counter
		JSR		_far_apu_play_prg5_safe					; put it here, because we always run in prg5 during
		LDA		_irq_regs_backup						; intro sequences, removed from event loops
		LDX		_irq_regs_backup+1
		LDY		_irq_regs_backup+2
		RTI
.nmi0_generic_input:
		LDA		_event_reload_idx
		CMP		#4
		BNE		.j_nmi0_finish
		LDA		_screen_idx
		CMP		#3
		BEQ		.nmi0_hiscore_entry_ppu_update
.j_nmi0_finish:
		JMP		.nmi0_finish
; --------------------------------
.nmi0_hiscore_entry_ppu_update:
		LDA		#0										; calc the hiscore table data offset
		CLC												; ofs = hi_score_line_idx * 15
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
; --------------------------------
.nmi0_finish:
		LDA		#$88
;		ORA		_nmi_ppu_ctrl_override_redundant
		STA		_PPU_CTRL
		INC		_generic_nmi_over_counter
		JSR		_far_apu_play_prg5_safe					; put it here, because we always run in prg5 during
		LDA		_irq_regs_backup						; intro sequences, removed from event loops
		LDX		_irq_regs_backup+1
		LDY		_irq_regs_backup+2
		RTI
		
_hi_score_ppu_ofs_list:
		.WORD $2151
		.WORD $2191
		.WORD $21D1
		.WORD $2211
		.WORD $2251
		.WORD $2291
		
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
;		LDA		_ppu_scroll_x_override_redundant		; see, overrided
		LDA		#0
		STA		_PPU_SCROLL
		LDA		_disp_extra_shift
		BEQ		loc_4544C
		LDA		#$E0
loc_4544C:
		STA		_PPU_SCROLL
;		INC		_nmi0_scroll_reload_counter_redundant
		RTS

; =============== S U B	R O U T	I N E =======================================
_dpad_convert:
		LDA		_pad
		LSR
		LSR
		LSR
		LSR
		TAX
		LDA		_dpad_conv_tbl,X
		STA		_dpad
		RTS
		
_dpad_conv_tbl:
		.BYTE	0,1,3,0,4,0,0,0,2,0,0,0,0,0,0,0
		
;		LDA		_pad
;		AND		#$F0
;		LSR
;		LSR
;		LSR
;		TAX
;		LDA		off_4546B,X
;		STA		_tmp_buf0
;		LDA		off_4546B+1,X
;		STA		_tmp_buf0+1
;		JMP		(_tmp_buf0)
;off_4546B:
;		.WORD _dpad_0
;		.WORD _dpad_1
;		.WORD _dpad_3
;		.WORD _dpad_0
;		.WORD _dpad_4
;		.WORD _dpad_0_0
;		.WORD _dpad_0_1
;		.WORD _dpad_0
;		.WORD _dpad_2
;		.WORD _dpad_0_2
;		.WORD _dpad_0_3
;		.WORD _dpad_0
;		.WORD _dpad_0
;		.WORD _dpad_0
;		.WORD _dpad_0
;		.WORD _dpad_0

; =============== S U B	R O U T	I N E =======================================
;_dpad_0:
;		LDA		#0
;		STA		_dpad
;		RTS

; =============== S U B	R O U T	I N E =======================================
;_dpad_1:
;		LDA		#1
;		STA		_dpad
;		RTS

; =============== S U B	R O U T	I N E =======================================
;_dpad_3:
;		LDA		#3
;		STA		_dpad
;		RTS

; =============== S U B	R O U T	I N E =======================================
;_dpad_4:
;		LDA		#4
;		STA		_dpad
;		RTS

; =============== S U B	R O U T	I N E =======================================
;_dpad_2:
;		LDA		#2
;		STA		_dpad
;		RTS

; =============== S U B	R O U T	I N E =======================================
;_dpad_0_0:
;		LDA		#0
;		STA		_dpad
;		RTS

; =============== S U B	R O U T	I N E =======================================
;_dpad_0_1:
;		LDA		#0
;		STA		_dpad
;		RTS

; =============== S U B	R O U T	I N E =======================================
;_dpad_0_2:
;		LDA		#0
;		STA		_dpad
;		RTS

; =============== S U B	R O U T	I N E =======================================
;_dpad_0_3:
;		LDA		#0
;		STA		_dpad
;		RTS

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
		.WORD _pal3456_frame
		.WORD _pal3456_frame
		.WORD _pal3456_frame
		.WORD _pal3456_frame
		.WORD _pal7_ending
		.WORD _pal8_repair
_disp_spr_tile_delta:
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,$BE
_disp_chr1_list:
		.BYTE	$18,$18,$19,$1C,$1C,$1C,$1D,$1E,$16
_disp_chr0_list:
		.BYTE	$1C,$1C,$1B,$18,$18,$18,$18,$1E,$16
_disp_spr0hit_use_flags:
		.BYTE	  0,  0,$FF,  0,  0,  0,  0,$FF,$FF
_disp_spr0hit_x:
		.BYTE	  0,  0,$BD,  0,  0,  0,  0,$F8,$30
_disp_spr0hit_y:
		.BYTE	  0,  0,$65,  0,  0,  0,  0,$7C,$66
_disp_spr0hit_tile:
		.BYTE	  0,  0,$FF,  0,  0,  0,  0,$30,$30
_special_controls_flag_flags_list:
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
		LDA		_special_controls_flag_flags_list,X
		STA		_special_controls_flag
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
		LDA		(_tptr0),Y
		BPL		loc_4592B
		CMP		#$FF
		BEQ		_disp_atr_send
		AND		#$7F
		TAX
		JSR		_ptr5_inc
loc_4591E:
		LDA		(_tptr0),Y
		STA		_PPU_DATA
		JSR		_ptr5_inc
		DEX
		BNE		loc_4591E
		BEQ		_disp_tlm_send
loc_4592B:
		TAX
		JSR		_ptr5_inc
		LDA		(_tptr0),Y
loc_45931:
		STA		_PPU_DATA
		DEX
		BNE		loc_45931
		JSR		_ptr5_inc
		JMP		_disp_tlm_send
_ptr5_inc:
		INC		_tptr0
		BNE		locret_45943
		INC		_tptr0+1
locret_45943:
		RTS
_disp_atr_send:
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
		LDA		_str_ptr
		BNE		_disp_spr_send
		LDA		_str_ptr+1
		BNE		_disp_spr_send
		JMP		locret_459F1
_disp_spr_send:
		LDA		#$C
		STA		_tptr0
		LDA		#$FF
		STA		_tptr0+1
		LDA		#0
		STA		_disp_spr_send_tmp
loc_45979:
		LDX		#0
loc_4597B:
		LDY		#0
		LDA		(_str_ptr),Y
		BEQ		loc_459D4
		STA		_tmpE4
		CMP		#$FF
		BNE		loc_45992
		LDA		_disp_spr_send_tmp
		EOR		#$80
		STA		_disp_spr_send_tmp
		JMP		loc_459D4
loc_45992:
		LDA		_tmpE4
		AND		#$3F
		LDY		_tptr0
		TXA
		PHA
		LDX		_screen_idx
		LDA		_tmpE4
		AND		#$3F
		CLC
		ADC		_disp_spr_tile_delta,X
		BIT		_disp_spr_send_tmp
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
		LDA		_tptr0+1
		CLC
		ADC		_disp_extra_shift
		STA		_spr_buf,Y
		LDA		_tmpE4
		LSR
		LSR
		LSR
		LSR
		LSR
		LSR
		STA		_spr_buf+2,Y
		LDA		_tptr0
		CLC
		ADC		#4
		STA		_tptr0
		BCS		locret_459F1
loc_459D4:
		LDA		_str_ptr
		CLC
		ADC		#1
		STA		_str_ptr
		LDA		_str_ptr+1
		ADC		#0
		STA		_str_ptr+1
		INX
		CPX		#$20
		BCC		loc_4597B
		LDA		_tptr0+1
		CLC
		ADC		#8
		STA		_tptr0+1
		CMP		#$C8
		BCC		loc_45979
locret_459F1:
		RTS

; =============== S U B	R O U T	I N E =======================================
_sort_hiscore_test:
		LDX		#0
		LDA		_cur_score_hex
		STA		_pl_bckp_score_hex_lo,X
		STA		_str_ptr
		LDA		_cur_score_hex+1
		STA		_pl_bckp_score_hex_hi,X
		STA		_str_ptr+1
		LDA		#0
		STA		_hex2dec_tile_delta
		LDX		#<[_hi_dec_tmp_buf]
		LDY		#>[_hi_dec_tmp_buf]
		JSR		_hex2str
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
		STA		_tptr1
		LDA		#>[_hi0_score]
		STA		_tptr1+1
loc_45A3B:
		LDY		#0
		LDA		_hi_score_pos_count
		CMP		#6
		BCS		locret_45A7A
loc_45A44:
		LDA		_score_cmp_buf,Y
		CMP		(_tptr1),Y
		BCC		loc_45A52
		BNE		_hiscore_swap
		INY
		CPY		#5
		BNE		loc_45A44
loc_45A52:
		INC		_hi_score_line_idx
loc_45A55:
		INC		_hi_score_pos_count
		LDA		_tptr1
		CLC
		ADC		#$F
		STA		_tptr1
		LDA		_tptr1+1
		ADC		#0
		STA		_tptr1+1
		BNE		loc_45A3B
_hiscore_swap:
		LDY		#$E
loc_45A69:
		LDA		(_tptr1),Y
		TAX
		LDA		_score_cmp_buf,Y
		STA		(_tptr1),Y
		TXA
		STA		_score_cmp_buf,Y
		DEY
		BPL		loc_45A69
		BMI		loc_45A55
locret_45A7A:
		RTS
_nums_table:
		.BYTE	_O,_1,_2,_3,_4,_5,_6,_7,_8,_9

; =============== S U B	R O U T	I N E =======================================
; draws an actual hi-score table data from RAM at the fixed position
; on the screen
;
_draw_hiscore_table:
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
		LDA		_nums_table,Y
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
		LDA		_nums_table,Y
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
		LDA		_nums_table,Y
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
		LDA		_nums_table,Y
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
		LDA		_nums_table,Y
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
		BEQ		loc_45C54
		CMP		#2
		BEQ		loc_45C6B
		CMP		#3
		BEQ		loc_45C82
		JSR		_get_cursor_allow_list_ptr
		LDY		#3
		LDA		(_tptr0),Y
		BMI		locret_45CAF
		CMP		_repair_body_index_request
		BEQ		locret_45CAF
		STA		_repair_body_index_request
		LDA		#1
		STA		byte_3BF
locret_45CAF:
		RTS
loc_45C54:
		JSR		_get_cursor_allow_list_ptr
		LDY		#0
		LDA		(_tptr0),Y
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
		LDA		(_tptr0),Y
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
		LDA		(_tptr0),Y
		BMI		locret_45C98
		CMP		_repair_body_index_request
		BEQ		locret_45C98
		STA		_repair_body_index_request
		LDA		#1
		STA		byte_3BF
locret_45C98:
		RTS

; =============== S U B	R O U T	I N E =======================================
_get_cursor_allow_list_ptr:
		LDA		#0
		STA		_tptr0+1
;		LDX		_repair_redundant_idx					; sone unknown redundant variable, probably
;		LDA		byte_45D7E,X							; meant to be used when 2-player mode is used
;		CLC
		LDA		_repair_cur_body_index
		ASL
		ASL
		ROL		_tptr0+1
		CLC
		ADC		#<[_repair_cursor_allow_table]
		STA		_tptr0
		LDA		_tptr0+1
		ADC		#>[_repair_cursor_allow_table]
		STA		_tptr0+1
		RTS

; =============== S U B	R O U T	I N E =======================================
_repair_cursor_animate:
		LDA		byte_3BF
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
;		LDX		_repair_redundant_idx					; second part of dummy redundant variable
;		LDA		byte_45D7E,X							; presumably for 2-players repair mode screen
;		CLC
;		ADC		_repair_body_index_request
		LDA		_repair_body_index_request
		ASL
		TAX
		LDA		_repair_cursor_anim_data,X
		STA		word_3BA
		LDA		_repair_cursor_anim_data+1,X
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

;byte_45D7E:
;		.BYTE	0

; =============== S U B	R O U T	I N E =======================================
;_hex2str_hiscore:
;		STX		_ptr1
;		STY		_ptr1+1
;		LDY		#0
;loc_45D85:
;		LDX		#$FF
;		SEC
;loc_45D88:
;		INX
;		LDA		_str_ptr
;		SBC		_dec_list_lo,Y
;		STA		_str_ptr
;		LDA		_str_ptr+1
;		SBC		_dec_list_hi,Y
;		STA		_str_ptr+1
;		BCS		loc_45D88
;		LDA		_str_ptr
;		ADC		_dec_list_lo,Y
;		STA		_str_ptr
;		LDA		_str_ptr+1
;		ADC		_dec_list_hi,Y
;		STA		_str_ptr+1
;		TXA
;		CLC
;		ADC		_hex2dec_tile_delta
;		STA		(_ptr1),Y
;		INY
;		CPY		#5
;		BNE		loc_45D85
;		RTS
;_dec_list_lo:
;		.BYTE	$10,$E8,$64, $A,  1
;_dec_list_hi:
;		.BYTE	$27,  3,  0,  0,  0
		
_repair_update_flag_list:
		.BYTE	  0,  2,  1,  2,  3
_repair_cursor_allow_table:
		.BYTE	$FF,  3,  2,  1
		.BYTE	  0,  2,  4,$FF
		.BYTE	  0,  3,  4,  1
		.BYTE	  0,$FF,  4,  2
		.BYTE	  2,$FF,$FF,  1
_repair_cursor_anim_data:
		.WORD $20A0
		.WORD $5888
		.WORD $40A8
		.WORD $48C0
		.WORD $80CB

; displays tilemaps
;
_pal0_ocean:
		.BYTE	 $F,  0,$10,$20, $F,  1,$11,$21, $F,  2,$12,$10, $F,  3,$13,$23
		.BYTE	 $F,$16,$26, $F, $F,  5,$15,$25, $F,  6,$16,$26, $F,  7,$17,$27
_pal1_probe:
		.BYTE	 $F,$16,$26, $F, $F, $F, $F, $F, $F, $F, $F, $F, $F,$12,$22,$32
		.BYTE	 $F,$16,$26, $F, $F,$21,$22,$32, $F,$21,$22,$32, $F,$21,$22,$32
_pal2_title:
		.BYTE	 $F,$27,$26,$30, $F,$11,$21,$30, $F,$27,$38,$30, $F,$27,$36,$30
		.BYTE	 $F,$27,$36,$30, $F,$27,$38,$30, $F,$16,$26, $F, $F,$21,$10,$20
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
_atr3_hiscore:
		.BYTE	$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
		.BYTE	$AF,$AB,$AF,$AB,$AE,$AF,$AE,$AF
		.BYTE	$EE,$FF,$FF,$FF,$FF,$FF,$FF,$BB
		.BYTE	$EE,$FF,$FF,$FF,$FF,$FF,$FF,$BB
		.BYTE	$EE,$FF,$FF,$FF,$FF,$FF,$FF,$BB
		.BYTE	$AE,$AF,$AF,$AF,$AF,$AF,$AF,$AB
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
		.BYTE	$FF,$FF,$FF,$FF,$FF,$FF,$FF,  0
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
		INCBIN	"tlm\tlm0.pak"		
_tlm1_probe:
		INCBIN	"tlm\tlm1.pak"		
_tlm2_title:
		INCBIN	"tlm\tlm2.pak"		
_tlm3_hiscore:
		INCBIN	"tlm\tlm3.pak"		
_tlm4_frame:
		INCBIN	"tlm\tlm4.pak"		
_tlm5_lewis:
		INCBIN	"tlm\tlm5.pak"		
_tlm6_robocop:
		INCBIN	"tlm\tlm6.pak"		
_tlm7_ending:
		INCBIN	"tlm\tlm7.pak"		
_tlm8_repair:
		INCBIN	"tlm\tlm8.pak"		

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
_spr8_repair:
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$81,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$82,$83,  0,$44,$45,  0,  0,$86,$87,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$88,$89,  0,  0,  0,  0,  0, $A,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$8B,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$8C,$8D,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$8E,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$8F,$90,$91,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$92,$93,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$94,$95,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$96,$97,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$98,  0,  0,  0,  0,  0,$99,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$9A,  0,  0,  0,  0,  0,$9B,$9C,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$9D,$9E,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$9F,$A0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$A1,$A2,$A3,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$A4,$A5,$A6,$A7,$A8,$A9,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$AA,$AB,$AC,$AD,$AE,$AF,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$B0,  0,$B1,$B2,$B3,$B4,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$B5,$B6,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$B7,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0

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
		STY		_log_msg_skip_forbid

; =============== S U B	R O U T	I N E =======================================
; may be non skippable
;
_bios_msg_display_ex:
		STA		byte_3B4
		ASL
		TAX
		LDA		_msg_lib,X
		STA		_txt_ptr
		LDA		_msg_lib+1,X
		STA		_txt_ptr+1
		LDA		#0
		STA		_event_idx
		STA		byte_3B5
		STA		byte_3D7
		STA		_PPU_CTRL
		STA		_PPU_MASK
		STA		_delay_counter
		LDA		#8
		STA		_pad_prev
		LDA		#1
		STA		byte_3D3
		LDA		#7
		STA		_event_reload_idx
		LDA		#$FF
		STA		byte_3B1
		JSR		_spr_buf_clear
		JSR		_log_clear_buf
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
		JSR		_rom_bios_hdr_display
		BIT		_PPU_STATUS
		LDA		#$21
		STA		_PPU_ADDR
		LDA		#$43
		STA		_PPU_ADDR
		JSR		_rom_bios_hdr_display
		BIT		_PPU_STATUS
		LDA		#$21
		STA		_PPU_ADDR
		LDA		#$63
		STA		_PPU_ADDR
		JSR		_rom_bios_hdr_display
		LDA		#$80
		STA		_PPU_CTRL
		JMP		_event_exec

; =============== S U B	R O U T	I N E =======================================
_rom_bios_hdr_display:
		LDA		_rom_bios_hdr,X
		CMP		#_SCL
		BEQ		loc_4784A
;		SEC
;		SBC		#$40
		STA		_PPU_DATA
		INX
		BNE		_rom_bios_hdr_display
loc_4784A:
		INX
		RTS
		
_rom_bios_hdr:
		.BYTE	_R,_O,_M,_,_B,_I,_O,_S,_SCL
		.BYTE	_D,_A,_T,_A,_,_R,_E,_T,_R,_I,_E,_V,_A,_L,_,_S,_Y,_S,_T,_E,_M,_SCL
		.BYTE	_V,_E,_R,_S,_I,_O,_N,_,_3,_DT,_O,_SCL

_msg_lib:
		.WORD _msg0_lvl0_briefing
		.WORD _msg1_lvl1_briefing
		.WORD _msg2_lvl2_briefing
		.WORD _msg3_lvl3_briefing
		.WORD _msg4_lvl4_briefing
		.WORD _msg5_copyrights
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

_msg5_copyrights:
		.BYTE	 _, _, _, _, _, _, _, _, _,_R,_O,_B,_O,_C,_O,_P,_TM, _,_3,_SCL
		.BYTE	_SCL
		.BYTE	 _, _, _, _,_T,_M, _,_A,_N,_D, _,_C,_O,_P,_Y,_R,_I,_G,_H,_T, _,_1,_9,_9,_2,_SCL
		.BYTE	 _,_O,_R,_I,_O,_N, _,_P,_I,_C,_T,_U,_R,_E,_S, _,_C,_O,_R,_P,_O,_R,_A,_T,_I,_O,_N,_SCL
		.BYTE	 _, _, _, _, _,_A,_L,_L, _,_R,_I,_G,_H,_T,_S, _,_R,_E,_S,_E,_R,_V,_E,_D,_SCL
		.BYTE	_T,_M, _,_D,_E,_S,_I,_G,_N,_A,_T,_E,_S, _,_A, _,_T,_R,_A,_D,_E,_M,_A,_R,_K, _,_O,_F,_SCL
		.BYTE	 _,_O,_R,_I,_O,_N, _,_P,_I,_C,_T,_U,_R,_E,_S, _,_C,_O,_R,_P,_O,_R,_A,_T,_I,_O,_N,_SCL
		.BYTE	_SCL
		.BYTE	 _, _, _, _, _, _, _,_C,_O,_P,_Y,_R,_I,_G,_H,_T, _,_1,_9, _9,_2,_SCL
		.BYTE	 _, _, _, _,_O,_C,_E,_A,_N, _,_O,_F, _,_A,_M,_E,_R,_I,_C,_A,_CO,_I,_N,_C,_DT,_SCL
		.BYTE	 _, _, _, _,_L,_I,_C,_E,_N,_S,_E,_D, _,_B,_Y, _,_N,_I,_N, _T,_E,_N,_D,_O,_DT,_SCL
		.BYTE	 _, _, _, _,_C,_A,_H,_4,_E,_3, _,_B,_F,_S, _,_G,_M,_B,_H, _,_2,_O,_1,_6,_END

_msg6_prepare_to_repair:
		.BYTE	_W,_E,_L,_L,_,_D,_O,_N,_E,_CO,_,_O,_F,_F,_I,_C,_E,_R,_DT,_SCL
		.BYTE	_DLY,_SCL
		.BYTE	_P,_R,_E,_P,_A,_R,_E,_,_T,_O,_,_E,_N,_T,_E,_R,_SCL
		.BYTE	_R,_E,_P,_A,_I,_R,_,_L,_A,_B,_O,_R,_A,_T,_O,_R,_Y,_DT,_DT,_DT,_SCL
		.BYTE	_DLY,_END
_msg7_confirm_repair:
		.BYTE	_MUT
		.BYTE	_P,_L,_E,_A,_S,_E,_,_C,_O,_N,_F,_I,_R,_M,_,_R,_E,_P,_A,_I,_R,_S,_DT,_DT,_DT,_SCL
		.BYTE	_DLY,_SCL
		.BYTE	_P,_R,_E,_S,_S,_,_A,_,_T,_O,_,_A,_C,_C,_E,_P,_T,_SCL
		.BYTE	_P,_R,_E,_S,_S,_,_B,_,_T,_O,_,_R,_E,_A,_L,_L,_O,_C,_A,_T,_E,_SCL
		.BYTE	_ASK,_CLR
		.BYTE	_C,_O,_N,_F,_I,_R,_M,_E,_D,_DT,_SCL
		.BYTE	_DLY,_END

		ECHO "bank 5 free space", [SAFE_STUB_ORG-.]

		SAFE_STUB
