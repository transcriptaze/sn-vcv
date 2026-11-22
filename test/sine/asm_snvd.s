	.build_version macos, 11, 0	sdk_version 27, 2
	.section	__TEXT,__text,regular,pure_instructions
	.globl	"__ZN19asm_lookup_sincos2d2υERKNS_2SNEj" ; -- Begin function _ZN19asm_lookup_sincos2d2υERKNS_2SNEj
	.p2align	2
"__ZN19asm_lookup_sincos2d2υERKNS_2SNEj": ; @"_ZN19asm_lookup_sincos2d2\CF\85ERKNS_2SNEj"
Lfunc_begin0:
	.file	1 "sn-vcv/test/sine" "benchmarks/asm_snvd.cpp"
	.loc	1 7 0                           ; benchmarks/asm_snvd.cpp:7:0
	.cfi_startproc
; %bb.0:
	;DEBUG_VALUE: υ:sn <- $x0
	;DEBUG_VALUE: υ:φ <- $w1
	sub	sp, sp, #48
	stp	x20, x19, [sp, #16]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #32]             ; 16-byte Folded Spill
	add	x29, sp, #32
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
Ltmp0:
	;DEBUG_VALUE: υ:sn <- $x19
	.loc	1 8 34 prologue_end             ; benchmarks/asm_snvd.cpp:8:34
	ldr	w8, [x0, #48]
	.loc	1 8 36 is_stmt 0                ; benchmarks/asm_snvd.cpp:8:36
	mul	w8, w8, w1
	.loc	1 8 31                          ; benchmarks/asm_snvd.cpp:8:31
	ucvtf	d0, w8
	.loc	1 8 49                          ; benchmarks/asm_snvd.cpp:8:49
	ldr	d1, [x0, #112]
	.loc	1 8 41                          ; benchmarks/asm_snvd.cpp:8:41
	fsub	d0, d0, d1
	.loc	1 8 31                          ; benchmarks/asm_snvd.cpp:8:31
	fcvtzu	w0, d0
Ltmp1:
	;DEBUG_VALUE: υ:αʼ <- $w0
	;DEBUG_VALUE: υ:x <- [DW_OP_plus_uconst 8, DW_OP_deref] $sp
	;DEBUG_VALUE: υ:y <- [DW_OP_deref] $sp
	.loc	1 11 9 is_stmt 1                ; benchmarks/asm_snvd.cpp:11:9
	mov	x1, sp
Ltmp2:
	;DEBUG_VALUE: υ:φ <- [DW_OP_LLVM_entry_value 1] $w1
	add	x2, sp, #8
	bl	__Z12sincos_lut_djRdS_
Ltmp3:
	.loc	1 13 40                         ; benchmarks/asm_snvd.cpp:13:40
	ldp	d3, d0, [sp]
Ltmp4:
	;DEBUG_VALUE: υ:x <- $d0
	.loc	1 13 34 is_stmt 0               ; benchmarks/asm_snvd.cpp:13:34
	ldp	d1, d2, [x19, #56]
Ltmp5:
	;DEBUG_VALUE: υ:y <- $d3
	.loc	1 13 54                         ; benchmarks/asm_snvd.cpp:13:54
	fmul	d2, d3, d2
	.loc	1 13 42                         ; benchmarks/asm_snvd.cpp:13:42
	fnmsub	d1, d0, d1, d2
	.loc	1 13 66                         ; benchmarks/asm_snvd.cpp:13:66
	ldr	d2, [x19, #72]
	.loc	1 13 58                         ; benchmarks/asm_snvd.cpp:13:58
	fadd	d1, d1, d2
Ltmp6:
	;DEBUG_VALUE: υ:xʼ <- $d1
	.loc	1 14 54 is_stmt 1               ; benchmarks/asm_snvd.cpp:14:54
	mov.d	v0[1], v3[0]
Ltmp7:
	.loc	1 14 34 is_stmt 0               ; benchmarks/asm_snvd.cpp:14:34
	ldr	q2, [x19, #80]
	.loc	1 14 54                         ; benchmarks/asm_snvd.cpp:14:54
	fmul.2d	v0, v2, v0
	.loc	1 14 66                         ; benchmarks/asm_snvd.cpp:14:66
	ldr	d2, [x19, #96]
	.loc	1 14 58                         ; benchmarks/asm_snvd.cpp:14:58
	faddp.2d	d0, v0
	fadd	d0, d2, d0
Ltmp8:
	;DEBUG_VALUE: υ:yʼ <- $d0
	.loc	1 0 58                          ; benchmarks/asm_snvd.cpp:0:58
	mov	x8, #79491254714368             ; =0x484c00000000
	movk	x8, #14772, lsl #48
	fmov	d2, x8
	.loc	1 15 36 is_stmt 1               ; benchmarks/asm_snvd.cpp:15:36
	fmadd	d1, d1, d1, d2
Ltmp9:
	.loc	1 15 48 is_stmt 0               ; benchmarks/asm_snvd.cpp:15:48
	fmadd	d1, d0, d0, d1
Ltmp10:
	;DEBUG_VALUE: υ:r <- $d1
	.loc	1 17 22 is_stmt 1               ; benchmarks/asm_snvd.cpp:17:22
	fsqrt	d1, d1
Ltmp11:
	.loc	1 17 20 is_stmt 0               ; benchmarks/asm_snvd.cpp:17:20
	fdiv	d0, d0, d1
Ltmp12:
	.loc	1 17 9 epilogue_begin           ; benchmarks/asm_snvd.cpp:17:9
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #16]             ; 16-byte Folded Reload
Ltmp13:
	;DEBUG_VALUE: υ:sn <- [DW_OP_LLVM_entry_value 1] $x0
	add	sp, sp, #48
	ret
Ltmp14:
Lfunc_end0:
	.cfi_endproc
