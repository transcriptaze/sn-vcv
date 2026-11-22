	.build_version macos, 11, 0	sdk_version 27, 2
	.file	1 "sn-vcv/test/sine" "benchmarks/asm_lutd.cpp"
	.file	2 "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/_types" "_uint64_t.h"
	.section	__TEXT,__text,regular,pure_instructions
	.globl	__Z12sincos_lut_djRdS_          ; -- Begin function _Z12sincos_lut_djRdS_
	.p2align	2
__Z12sincos_lut_djRdS_:                 ; @_Z12sincos_lut_djRdS_
Lfunc_begin0:
	.cfi_startproc
; %bb.0:
	;DEBUG_VALUE: sincos_lut_d:phase <- $w0
	;DEBUG_VALUE: sincos_lut_d:s <- $x1
	;DEBUG_VALUE: sincos_lut_d:c <- $x2
	.loc	1 13 33 prologue_end            ; benchmarks/asm_lutd.cpp:13:33
	add	w8, w0, #256, lsl #12           ; =1048576
	.loc	1 13 56 is_stmt 0               ; benchmarks/asm_lutd.cpp:13:56
	lsr	w8, w8, #21
Ltmp0:
	;DEBUG_VALUE: sincos_lut_d:idx <- $w8
	.loc	1 14 41 is_stmt 1               ; benchmarks/asm_lutd.cpp:14:41
	lsl	w9, w0, #11
	.loc	1 14 50 is_stmt 0               ; benchmarks/asm_lutd.cpp:14:50
	eor	w9, w9, #0x80000000
	.loc	1 14 34                         ; benchmarks/asm_lutd.cpp:14:34
	lsr	x9, x9, #11
	mov	x10, #4607182418800017408       ; =0x3ff0000000000000
	.loc	1 14 72                         ; benchmarks/asm_lutd.cpp:14:72
	bfi	x10, x9, #31, #21
Ltmp1:
	;DEBUG_VALUE: sincos_lut_d:m <- $x10
	.loc	1 17 5 is_stmt 1                ; benchmarks/asm_lutd.cpp:17:5
	fmov	d0, x10
Ltmp2:
	;DEBUG_VALUE: sincos_lut_d:m <- $d0
	;DEBUG_VALUE: sincos_lut_d:f <- $d0
	.loc	1 0 5 is_stmt 0                 ; benchmarks/asm_lutd.cpp:0:5
	mov	x9, #1610612736                 ; =0x60000000
	movk	x9, #8699, lsl #32
	movk	x9, #16233, lsl #48
	fmov	d1, x9
	mov	x9, #2281701376                 ; =0x88000000
	movk	x9, #55676, lsl #32
	movk	x9, #49010, lsl #48
	fmov	d2, x9
	.loc	1 18 32 is_stmt 1               ; benchmarks/asm_lutd.cpp:18:32
	fmadd	d0, d0, d1, d2
Ltmp3:
	;DEBUG_VALUE: sincos_lut_d:d <- $d0
	.loc	1 20 19                         ; benchmarks/asm_lutd.cpp:20:19
Lloh0:
	adrp	x9, _tabd@GOTPAGE
Lloh1:
	ldr	x9, [x9, _tabd@GOTPAGEOFF]
	add	x8, x9, w8, uxtw #4
Ltmp4:
	ldp	d1, d2, [x8]
Ltmp5:
	;DEBUG_VALUE: sincos_lut_d:e <- [DW_OP_LLVM_fragment 0 64] $d1
	;DEBUG_VALUE: sincos_lut_d:e <- [DW_OP_LLVM_fragment 64 64] $d2
	.loc	1 21 13                         ; benchmarks/asm_lutd.cpp:21:13
	fmadd	d3, d2, d0, d1
	.loc	1 21 7 is_stmt 0                ; benchmarks/asm_lutd.cpp:21:7
	str	d3, [x1]
	.loc	1 22 13 is_stmt 1               ; benchmarks/asm_lutd.cpp:22:13
	fmsub	d0, d1, d0, d2
Ltmp6:
	.loc	1 22 7 is_stmt 0                ; benchmarks/asm_lutd.cpp:22:7
	str	d0, [x2]
	.loc	1 23 1 is_stmt 1                ; benchmarks/asm_lutd.cpp:23:1
	ret
Ltmp7:
	.loh AdrpLdrGot	Lloh0, Lloh1
Lfunc_end0:
	.cfi_endproc
                                        ; -- End function
