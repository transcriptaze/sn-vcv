	.build_version macos, 11, 0	sdk_version 27, 2
	.section	__TEXT,__text,regular,pure_instructions
	.globl	"__ZN19asm_lookup_sincos2f2υERKNS_2SNEj" ; -- Begin function _ZN19asm_lookup_sincos2f2υERKNS_2SNEj
	.p2align	2
"__ZN19asm_lookup_sincos2f2υERKNS_2SNEj": ; @"_ZN19asm_lookup_sincos2f2\CF\85ERKNS_2SNEj"
Lfunc_begin0:
	.file	1 "test/sine" "benchmarks/asm_snvf.cpp"
	.loc	1 7 0                           ; benchmarks/asm_snvf.cpp:7:0
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
	.loc	1 8 34 prologue_end             ; benchmarks/asm_snvf.cpp:8:34
	ldr	w8, [x0, #48]
	.loc	1 8 36 is_stmt 0                ; benchmarks/asm_snvf.cpp:8:36
	mul	w8, w8, w1
	.loc	1 8 31                          ; benchmarks/asm_snvf.cpp:8:31
	ucvtf	s0, w8
	.loc	1 8 49                          ; benchmarks/asm_snvf.cpp:8:49
	ldr	s1, [x0, #80]
	.loc	1 8 41                          ; benchmarks/asm_snvf.cpp:8:41
	fsub	s0, s0, s1
	.loc	1 8 31                          ; benchmarks/asm_snvf.cpp:8:31
	fcvtzu	w0, s0
Ltmp1:
	;DEBUG_VALUE: υ:αʼ <- $w0
	;DEBUG_VALUE: υ:x <- [DW_OP_plus_uconst 12, DW_OP_deref] $sp
	;DEBUG_VALUE: υ:y <- [DW_OP_plus_uconst 8, DW_OP_deref] $sp
	.loc	1 11 9 is_stmt 1                ; benchmarks/asm_snvf.cpp:11:9
	add	x1, sp, #8
Ltmp2:
	;DEBUG_VALUE: υ:φ <- [DW_OP_LLVM_entry_value 1] $w1
	add	x2, sp, #12
	bl	__Z10sincos_lutjRfS_
Ltmp3:
	.loc	1 13 39                         ; benchmarks/asm_snvf.cpp:13:39
	ldp	s3, s0, [sp, #8]
Ltmp4:
	;DEBUG_VALUE: υ:x <- $s0
	.loc	1 13 33 is_stmt 0               ; benchmarks/asm_snvf.cpp:13:33
	ldp	s1, s2, [x19, #52]
Ltmp5:
	;DEBUG_VALUE: υ:y <- $s3
	.loc	1 13 53                         ; benchmarks/asm_snvf.cpp:13:53
	fmul	s2, s3, s2
	.loc	1 13 41                         ; benchmarks/asm_snvf.cpp:13:41
	fnmsub	s1, s0, s1, s2
	.loc	1 13 65                         ; benchmarks/asm_snvf.cpp:13:65
	ldr	s2, [x19, #60]
	.loc	1 13 57                         ; benchmarks/asm_snvf.cpp:13:57
	fadd	s1, s1, s2
Ltmp6:
	;DEBUG_VALUE: υ:xʼ <- $s1
	.loc	1 14 53 is_stmt 1               ; benchmarks/asm_snvf.cpp:14:53
	mov.s	v0[1], v3[0]
Ltmp7:
	.loc	1 14 33 is_stmt 0               ; benchmarks/asm_snvf.cpp:14:33
	ldr	d2, [x19, #64]
	.loc	1 14 53                         ; benchmarks/asm_snvf.cpp:14:53
	fmul.2s	v0, v2, v0
	.loc	1 14 65                         ; benchmarks/asm_snvf.cpp:14:65
	ldr	s2, [x19, #72]
	.loc	1 14 57                         ; benchmarks/asm_snvf.cpp:14:57
	faddp.2s	s0, v0
	fadd	s0, s2, s0
Ltmp8:
	;DEBUG_VALUE: υ:yʼ <- $s0
	.loc	1 0 57                          ; benchmarks/asm_snvf.cpp:0:57
	mov	w8, #16992                      ; =0x4260
	movk	w8, #3490, lsl #16
	fmov	s2, w8
	.loc	1 15 35 is_stmt 1               ; benchmarks/asm_snvf.cpp:15:35
	fmadd	s1, s1, s1, s2
Ltmp9:
	.loc	1 15 47 is_stmt 0               ; benchmarks/asm_snvf.cpp:15:47
	fmadd	s1, s0, s0, s1
Ltmp10:
	;DEBUG_VALUE: υ:r <- $s1
	;DEBUG_VALUE: sqrt:__x <- $s1
	.file	2 "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__math" "roots.h"
	.loc	2 26 71 is_stmt 1               ; /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__math/roots.h:26:71 @[ benchmarks/asm_snvf.cpp:17:22 ]
	fsqrt	s1, s1
Ltmp11:
	.loc	1 17 20                         ; benchmarks/asm_snvf.cpp:17:20
	fdiv	s0, s0, s1
Ltmp12:
	.loc	1 17 9 epilogue_begin is_stmt 0 ; benchmarks/asm_snvf.cpp:17:9
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #16]             ; 16-byte Folded Reload
Ltmp13:
	;DEBUG_VALUE: υ:sn <- [DW_OP_LLVM_entry_value 1] $x0
	add	sp, sp, #48
	ret
Ltmp14:
Lfunc_end0:
	.cfi_endproc
