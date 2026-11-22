	.build_version macos, 11, 0	sdk_version 27, 2
	.section	__TEXT,__text,regular,pure_instructions
	.globl	"__ZN18asm_lookup_sincos22υERKNS_2SNEj" ; -- Begin function _ZN18asm_lookup_sincos22υERKNS_2SNEj
	.p2align	2
"__ZN18asm_lookup_sincos22υERKNS_2SNEj": ; @"_ZN18asm_lookup_sincos22\CF\85ERKNS_2SNEj"
Lfunc_begin0:
	.file	1 "sn-vcv/test/sine" "benchmarks/asm_snv.cpp"
	.loc	1 7 0                           ; benchmarks/asm_snv.cpp:7:0
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
	.loc	1 8 34 prologue_end             ; benchmarks/asm_snv.cpp:8:34
	ldr	w8, [x0, #48]
	.loc	1 8 36 is_stmt 0                ; benchmarks/asm_snv.cpp:8:36
	mul	w8, w8, w1
	.loc	1 8 31                          ; benchmarks/asm_snv.cpp:8:31
	ucvtf	d0, w8
	.loc	1 8 49                          ; benchmarks/asm_snv.cpp:8:49
	ldr	d1, [x0, #112]
	.loc	1 8 41                          ; benchmarks/asm_snv.cpp:8:41
	fsub	d0, d0, d1
	.loc	1 8 31                          ; benchmarks/asm_snv.cpp:8:31
	fcvtzu	w0, d0
Ltmp1:
	;DEBUG_VALUE: υ:αʼ <- $w0
	;DEBUG_VALUE: υ:x <- [DW_OP_plus_uconst 12, DW_OP_deref] $sp
	;DEBUG_VALUE: υ:y <- [DW_OP_plus_uconst 8, DW_OP_deref] $sp
	.loc	1 11 9 is_stmt 1                ; benchmarks/asm_snv.cpp:11:9
	add	x1, sp, #8
Ltmp2:
	;DEBUG_VALUE: υ:φ <- [DW_OP_LLVM_entry_value 1] $w1
	add	x2, sp, #12
	bl	__Z10sincos_lutjRfS_
Ltmp3:
	.loc	1 13 39                         ; benchmarks/asm_snv.cpp:13:39
	ldr	s0, [sp, #12]
Ltmp4:
	;DEBUG_VALUE: υ:x <- $s0
	fcvt	d0, s0
Ltmp5:
	.loc	1 13 33 is_stmt 0               ; benchmarks/asm_snv.cpp:13:33
	ldp	d1, d2, [x19, #56]
	.loc	1 13 65                         ; benchmarks/asm_snv.cpp:13:65
	ldp	d3, d4, [x19, #72]
	.loc	1 13 41                         ; benchmarks/asm_snv.cpp:13:41
	fmadd	d1, d1, d0, d3
	.loc	1 13 55                         ; benchmarks/asm_snv.cpp:13:55
	ldr	s3, [sp, #8]
Ltmp6:
	;DEBUG_VALUE: υ:y <- $s3
	fcvt	d3, s3
Ltmp7:
	.loc	1 13 57                         ; benchmarks/asm_snv.cpp:13:57
	fmsub	d1, d2, d3, d1
	.loc	1 13 27                         ; benchmarks/asm_snv.cpp:13:27
	fcvt	s1, d1
Ltmp8:
	;DEBUG_VALUE: υ:xʼ <- $s1
	.loc	1 14 37 is_stmt 1               ; benchmarks/asm_snv.cpp:14:37
	fmul	d0, d4, d0
	.loc	1 14 49 is_stmt 0               ; benchmarks/asm_snv.cpp:14:49
	ldp	d2, d4, [x19, #88]
	.loc	1 14 41                         ; benchmarks/asm_snv.cpp:14:41
	fmadd	d0, d2, d3, d0
	.loc	1 14 57                         ; benchmarks/asm_snv.cpp:14:57
	fadd	d0, d0, d4
	.loc	1 14 27                         ; benchmarks/asm_snv.cpp:14:27
	fcvt	s0, d0
Ltmp9:
	;DEBUG_VALUE: υ:yʼ <- $s0
	.loc	1 0 27                          ; benchmarks/asm_snv.cpp:0:27
	mov	w8, #16992                      ; =0x4260
	movk	w8, #3490, lsl #16
	fmov	s2, w8
	.loc	1 15 35 is_stmt 1               ; benchmarks/asm_snv.cpp:15:35
	fmadd	s1, s1, s1, s2
Ltmp10:
	.loc	1 15 47 is_stmt 0               ; benchmarks/asm_snv.cpp:15:47
	fmadd	s1, s0, s0, s1
Ltmp11:
	;DEBUG_VALUE: υ:r <- $s1
	;DEBUG_VALUE: sqrt:__x <- $s1
	.file	2 "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__math" "roots.h"
	.loc	2 26 71 is_stmt 1               ; /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__math/roots.h:26:71 @[ benchmarks/asm_snv.cpp:17:22 ]
	fsqrt	s1, s1
Ltmp12:
	.loc	1 17 20                         ; benchmarks/asm_snv.cpp:17:20
	fdiv	s0, s0, s1
Ltmp13:
	.loc	1 17 9 epilogue_begin is_stmt 0 ; benchmarks/asm_snv.cpp:17:9
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #16]             ; 16-byte Folded Reload
Ltmp14:
	;DEBUG_VALUE: υ:sn <- [DW_OP_LLVM_entry_value 1] $x0
	add	sp, sp, #48
	ret
Ltmp15:
Lfunc_end0:
	.cfi_endproc
