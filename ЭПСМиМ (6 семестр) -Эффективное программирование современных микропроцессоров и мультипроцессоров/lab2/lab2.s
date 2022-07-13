	.file	"lab2.c"
	.text
	.p2align 4
	.type	_mm_malloc.constprop.0, @function
_mm_malloc.constprop.0:
.LFB5324:
	.cfi_startproc
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movq	%rsp, %rdi
	movl	$512000000, %edx
	movl	$32, %esi
	movq	%fs:40, %rax
	movq	%rax, 8(%rsp)
	xorl	%eax, %eax
	call	posix_memalign@PLT
	testl	%eax, %eax
	movl	$0, %eax
	cmove	(%rsp), %rax
	movq	8(%rsp), %rcx
	xorq	%fs:40, %rcx
	jne	.L8
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L8:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE5324:
	.size	_mm_malloc.constprop.0, .-_mm_malloc.constprop.0
	.p2align 4
	.globl	min
	.type	min, @function
min:
.LFB5318:
	.cfi_startproc
	endbr64
	vminsd	%xmm1, %xmm0, %xmm0
	ret
	.cfi_endproc
.LFE5318:
	.size	min, .-min
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%f\t"
	.text
	.p2align 4
	.globl	print_massive
	.type	print_massive, @function
print_massive:
.LFB5319:
	.cfi_startproc
	endbr64
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$24, %rsp
	.cfi_def_cfa_offset 80
	movq	%rdi, 8(%rsp)
	movl	%edx, 4(%rsp)
	testl	%edx, %edx
	jle	.L11
	movl	%esi, %r12d
	xorl	%r14d, %r14d
	xorl	%r13d, %r13d
	leaq	.LC0(%rip), %rbp
	.p2align 4,,10
	.p2align 3
.L12:
	testl	%r12d, %r12d
	jle	.L15
	movq	8(%rsp), %rdx
	movslq	%r14d, %rcx
	leal	-1(%r12), %eax
	addq	%rcx, %rax
	leaq	(%rdx,%rcx,8), %r15
	leaq	8(%rdx,%rax,8), %rbx
	.p2align 4,,10
	.p2align 3
.L13:
	vmovsd	(%r15), %xmm0
	movq	%rbp, %rsi
	movl	$1, %edi
	movl	$1, %eax
	addq	$8, %r15
	call	__printf_chk@PLT
	cmpq	%rbx, %r15
	jne	.L13
.L15:
	movl	$10, %edi
	incl	%r13d
	call	putchar@PLT
	addl	%r12d, %r14d
	cmpl	%r13d, 4(%rsp)
	jne	.L12
.L11:
	addq	$24, %rsp
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	movl	$10, %edi
	popq	%r15
	.cfi_def_cfa_offset 8
	jmp	putchar@PLT
	.cfi_endproc
.LFE5319:
	.size	print_massive, .-print_massive
	.p2align 4
	.globl	init_ro
	.type	init_ro, @function
init_ro:
.LFB5320:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	andq	$-32, %rsp
	subq	$8, %rsp
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	.cfi_offset 3, -56
	movl	%edx, -52(%rsp)
	vmovsd	%xmm0, (%rsp)
	vmovsd	%xmm1, -24(%rsp)
	vmovsd	%xmm2, -48(%rsp)
	vmovsd	%xmm3, -64(%rsp)
	vmovsd	%xmm4, -8(%rsp)
	vmovsd	%xmm5, -72(%rsp)
	vmovsd	%xmm6, -16(%rsp)
	vmovsd	%xmm7, -80(%rsp)
	testl	%edx, %edx
	jle	.L76
	vmovsd	16(%rbp), %xmm6
	vminsd	24(%rbp), %xmm6, %xmm0
	vmulsd	.LC8(%rip), %xmm0, %xmm0
	vmulsd	%xmm0, %xmm0, %xmm13
	testl	%esi, %esi
	jle	.L76
	leal	-1(%rsi), %ebx
	movl	%esi, %r10d
	movl	%ebx, -56(%rsp)
	shrl	$3, %r10d
	movl	%esi, %r9d
	movq	(%rdi), %rax
	movl	%edx, %r11d
	salq	$6, %r10
	andl	$-8, %r9d
	xorl	%ecx, %ecx
	xorl	%r8d, %r8d
	.p2align 4,,10
	.p2align 3
.L53:
	vxorpd	%xmm7, %xmm7, %xmm7
	vcvtsi2sdl	%r8d, %xmm7, %xmm0
	vmovsd	-80(%rsp), %xmm7
	vfmadd132sd	-72(%rsp), %xmm7, %xmm0
	vsubsd	-48(%rsp), %xmm0, %xmm1
	vsubsd	-64(%rsp), %xmm0, %xmm0
	vmulsd	%xmm1, %xmm1, %xmm12
	vmulsd	%xmm0, %xmm0, %xmm6
	vmovsd	%xmm6, -32(%rsp)
	cmpl	$6, %ebx
	jbe	.L64
	movslq	%ecx, %rdx
	leaq	(%rax,%rdx,8), %rdx
	vbroadcastsd	-8(%rsp), %ymm11
	vbroadcastsd	-16(%rsp), %ymm10
	vbroadcastsd	(%rsp), %ymm9
	vbroadcastsd	-24(%rsp), %ymm7
	vmovdqa	.LC1(%rip), %ymm5
	vbroadcastsd	%xmm12, %ymm8
	vbroadcastsd	%xmm13, %ymm4
	vbroadcastsd	%xmm6, %ymm6
	leaq	(%rdx,%r10), %r12
	vmovsd	%xmm12, -40(%rsp)
	jmp	.L28
	.p2align 4,,10
	.p2align 3
.L22:
	vptest	%ymm0, %ymm0
	jne	.L78
.L23:
	vsubpd	%ymm7, %ymm3, %ymm3
	vsubpd	%ymm7, %ymm1, %ymm1
	vcmplepd	%ymm15, %ymm4, %ymm15
	vfmadd132pd	%ymm3, %ymm6, %ymm3
	vfmadd132pd	%ymm1, %ymm6, %ymm1
	vcmplepd	%ymm14, %ymm4, %ymm14
	vcmpltpd	%ymm4, %ymm3, %ymm2
	vcmpltpd	%ymm4, %ymm1, %ymm0
	vpand	%ymm15, %ymm2, %ymm2
	vptest	%ymm2, %ymm2
	vpand	%ymm14, %ymm0, %ymm0
	jne	.L79
.L24:
	vptest	%ymm0, %ymm0
	jne	.L80
.L25:
	vcmplepd	%ymm3, %ymm4, %ymm2
	vcmplepd	%ymm1, %ymm4, %ymm0
	vpand	%ymm15, %ymm2, %ymm2
	vptest	%ymm2, %ymm2
	vpand	%ymm14, %ymm0, %ymm0
	jne	.L81
.L26:
	vptest	%ymm0, %ymm0
	jne	.L82
	addq	$64, %rdx
	cmpq	%rdx, %r12
	je	.L83
.L28:
	vmovdqa	%ymm5, %ymm0
	vcvtdq2pd	%xmm0, %ymm3
	vfmadd132pd	%ymm11, %ymm10, %ymm3
	vextracti128	$0x1, %ymm0, %xmm0
	vcvtdq2pd	%xmm0, %ymm0
	vmovapd	%ymm0, %ymm1
	vfmadd132pd	%ymm11, %ymm10, %ymm1
	vsubpd	%ymm9, %ymm3, %ymm15
	vpaddd	.LC2(%rip), %ymm5, %ymm5
	vfmadd132pd	%ymm15, %ymm8, %ymm15
	vsubpd	%ymm9, %ymm1, %ymm14
	vfmadd132pd	%ymm14, %ymm8, %ymm14
	vcmpltpd	%ymm4, %ymm15, %ymm2
	vptest	%ymm2, %ymm2
	vcmpltpd	%ymm4, %ymm14, %ymm0
	je	.L22
	vptest	%ymm0, %ymm0
	vmovapd	.LC3(%rip), %ymm12
	vmaskmovpd	%ymm12, %ymm2, (%rdx)
	je	.L23
	.p2align 4,,10
	.p2align 3
.L78:
	vmovapd	.LC3(%rip), %ymm2
	vmaskmovpd	%ymm2, %ymm0, 32(%rdx)
	jmp	.L23
	.p2align 4,,10
	.p2align 3
.L82:
	vxorpd	%xmm3, %xmm3, %xmm3
	vmaskmovpd	%ymm3, %ymm0, 32(%rdx)
	addq	$64, %rdx
	cmpq	%rdx, %r12
	jne	.L28
	.p2align 4,,10
	.p2align 3
.L83:
	vmovsd	-40(%rsp), %xmm12
	cmpl	%esi, %r9d
	je	.L29
	movl	%r9d, %edx
.L21:
	vxorpd	%xmm5, %xmm5, %xmm5
	vcvtsi2sdl	%edx, %xmm5, %xmm0
	vmovsd	-16(%rsp), %xmm5
	leal	(%rcx,%rdx), %r12d
	movslq	%r12d, %r12
	vfmadd132sd	-8(%rsp), %xmm5, %xmm0
	leaq	(%rax,%r12,8), %r12
	vsubsd	(%rsp), %xmm0, %xmm1
	vfmadd132sd	%xmm1, %xmm12, %xmm1
	vcomisd	%xmm1, %xmm13
	ja	.L30
	vsubsd	-24(%rsp), %xmm0, %xmm0
	vmovsd	.LC6(%rip), %xmm2
	vxorpd	%xmm1, %xmm1, %xmm1
	vfmadd213sd	-32(%rsp), %xmm0, %xmm0
	vcmpltsd	%xmm13, %xmm0, %xmm0
	vblendvpd	%xmm0, %xmm2, %xmm1, %xmm0
	vmovsd	%xmm0, (%r12)
.L32:
	leal	1(%rdx), %r12d
	cmpl	%esi, %r12d
	jge	.L29
	vxorpd	%xmm6, %xmm6, %xmm6
	vcvtsi2sdl	%r12d, %xmm6, %xmm0
	vmovsd	-16(%rsp), %xmm7
	addl	%ecx, %r12d
	movslq	%r12d, %r12
	vfmadd132sd	-8(%rsp), %xmm7, %xmm0
	leaq	(%rax,%r12,8), %r12
	vsubsd	(%rsp), %xmm0, %xmm1
	vfmadd132sd	%xmm1, %xmm12, %xmm1
	vcomisd	%xmm1, %xmm13
	ja	.L33
	vsubsd	-24(%rsp), %xmm0, %xmm0
	vmovsd	.LC6(%rip), %xmm2
	vxorpd	%xmm1, %xmm1, %xmm1
	vfmadd213sd	-32(%rsp), %xmm0, %xmm0
	vcmpltsd	%xmm13, %xmm0, %xmm0
	vblendvpd	%xmm0, %xmm2, %xmm1, %xmm0
	vmovsd	%xmm0, (%r12)
.L35:
	leal	2(%rdx), %r12d
	cmpl	%r12d, %esi
	jle	.L29
	vxorpd	%xmm5, %xmm5, %xmm5
	vcvtsi2sdl	%r12d, %xmm5, %xmm0
	vmovsd	-16(%rsp), %xmm6
	addl	%ecx, %r12d
	movslq	%r12d, %r12
	vfmadd132sd	-8(%rsp), %xmm6, %xmm0
	leaq	(%rax,%r12,8), %r12
	vsubsd	(%rsp), %xmm0, %xmm1
	vfmadd132sd	%xmm1, %xmm12, %xmm1
	vcomisd	%xmm1, %xmm13
	ja	.L36
	vsubsd	-24(%rsp), %xmm0, %xmm0
	vmovsd	.LC6(%rip), %xmm2
	vxorpd	%xmm1, %xmm1, %xmm1
	vfmadd213sd	-32(%rsp), %xmm0, %xmm0
	vcmpltsd	%xmm13, %xmm0, %xmm0
	vblendvpd	%xmm0, %xmm2, %xmm1, %xmm0
	vmovsd	%xmm0, (%r12)
.L38:
	leal	3(%rdx), %r12d
	cmpl	%r12d, %esi
	jle	.L29
	vxorpd	%xmm7, %xmm7, %xmm7
	vcvtsi2sdl	%r12d, %xmm7, %xmm0
	vmovsd	-16(%rsp), %xmm7
	addl	%ecx, %r12d
	movslq	%r12d, %r12
	vfmadd132sd	-8(%rsp), %xmm7, %xmm0
	leaq	(%rax,%r12,8), %r12
	vsubsd	(%rsp), %xmm0, %xmm1
	vfmadd132sd	%xmm1, %xmm12, %xmm1
	vcomisd	%xmm1, %xmm13
	ja	.L39
	vsubsd	-24(%rsp), %xmm0, %xmm0
	vmovsd	.LC6(%rip), %xmm2
	vxorpd	%xmm1, %xmm1, %xmm1
	vfmadd213sd	-32(%rsp), %xmm0, %xmm0
	vcmpltsd	%xmm13, %xmm0, %xmm0
	vblendvpd	%xmm0, %xmm2, %xmm1, %xmm0
	vmovsd	%xmm0, (%r12)
.L41:
	leal	4(%rdx), %r12d
	cmpl	%r12d, %esi
	jle	.L29
	vxorpd	%xmm6, %xmm6, %xmm6
	vcvtsi2sdl	%r12d, %xmm6, %xmm0
	vmovsd	-16(%rsp), %xmm5
	addl	%ecx, %r12d
	movslq	%r12d, %r12
	vfmadd132sd	-8(%rsp), %xmm5, %xmm0
	leaq	(%rax,%r12,8), %r12
	vsubsd	(%rsp), %xmm0, %xmm1
	vfmadd132sd	%xmm1, %xmm12, %xmm1
	vcomisd	%xmm1, %xmm13
	ja	.L42
	vsubsd	-24(%rsp), %xmm0, %xmm0
	vmovsd	.LC6(%rip), %xmm2
	vxorpd	%xmm1, %xmm1, %xmm1
	vfmadd213sd	-32(%rsp), %xmm0, %xmm0
	vcmpltsd	%xmm13, %xmm0, %xmm0
	vblendvpd	%xmm0, %xmm2, %xmm1, %xmm0
	vmovsd	%xmm0, (%r12)
.L44:
	leal	5(%rdx), %r12d
	cmpl	%r12d, %esi
	jle	.L29
	vxorpd	%xmm7, %xmm7, %xmm7
	vcvtsi2sdl	%r12d, %xmm7, %xmm0
	vmovsd	-16(%rsp), %xmm6
	addl	%ecx, %r12d
	movslq	%r12d, %r12
	vfmadd132sd	-8(%rsp), %xmm6, %xmm0
	leaq	(%rax,%r12,8), %r12
	vsubsd	(%rsp), %xmm0, %xmm1
	vfmadd132sd	%xmm1, %xmm12, %xmm1
	vcomisd	%xmm1, %xmm13
	ja	.L45
	vsubsd	-24(%rsp), %xmm0, %xmm0
	vmovsd	.LC6(%rip), %xmm2
	vxorpd	%xmm1, %xmm1, %xmm1
	vfmadd213sd	-32(%rsp), %xmm0, %xmm0
	vcmpltsd	%xmm13, %xmm0, %xmm0
	vblendvpd	%xmm0, %xmm2, %xmm1, %xmm0
	vmovsd	%xmm0, (%r12)
.L47:
	addl	$6, %edx
	cmpl	%edx, %esi
	jle	.L29
	vxorpd	%xmm5, %xmm5, %xmm5
	vcvtsi2sdl	%edx, %xmm5, %xmm0
	vmovsd	-16(%rsp), %xmm6
	addl	%ecx, %edx
	movslq	%edx, %rdx
	vfmadd132sd	-8(%rsp), %xmm6, %xmm0
	leaq	(%rax,%rdx,8), %rdx
	vsubsd	(%rsp), %xmm0, %xmm1
	vfmadd132sd	%xmm1, %xmm12, %xmm1
	vcomisd	%xmm1, %xmm13
	jbe	.L73
	movq	.LC7(%rip), %r15
	movq	%r15, (%rdx)
.L29:
	incl	%r8d
	addl	%esi, %ecx
	cmpl	%r8d, %r11d
	jne	.L53
	movl	%esi, %ebx
	negl	%ebx
	movslq	%ebx, %rdx
	movq	%rdx, -16(%rsp)
	movl	%esi, %edx
	movq	8(%rdi), %r13
	shrl	$2, %edx
	movl	%esi, %edi
	andl	$-4, %edi
	salq	$5, %rdx
	movq	%rdx, %r12
	leal	1(%rdi), %edx
	movl	%edx, -80(%rsp)
	movl	-56(%rsp), %edx
	movl	%edi, -72(%rsp)
	addl	$2, %edi
	movslq	%esi, %rcx
	movl	%edi, -84(%rsp)
	leaq	8(%rax,%rdx,8), %rdi
	vmovsd	.LC11(%rip), %xmm7
	vmovsd	.LC12(%rip), %xmm6
	movq	%rcx, -64(%rsp)
	movq	%rcx, (%rsp)
	movq	$0, -8(%rsp)
	movl	$0, -24(%rsp)
	movq	%rdi, -96(%rsp)
	movl	%esi, -40(%rsp)
	vmovapd	.LC9(%rip), %ymm3
	vmovapd	.LC10(%rip), %ymm2
	leal	(%rbx,%rsi), %r15d
	vmovapd	%xmm7, %xmm5
	vmovapd	%xmm6, %xmm4
	movl	%ebx, %r14d
	.p2align 4,,10
	.p2align 3
.L60:
	movq	-8(%rsp), %rdi
	movq	-16(%rsp), %r8
	leaq	0(,%rdi,8), %rcx
	movq	(%rsp), %rbx
	salq	$3, %r8
	leaq	0(%r13,%rcx), %rdx
	leaq	40(%rax,%rcx), %r9
	salq	$3, %rbx
	leaq	32(%r13,%rcx), %r11
	incl	-24(%rsp)
	movl	%r14d, -48(%rsp)
	movq	%r8, -32(%rsp)
	movl	%r15d, %r14d
	leaq	-8(%rax,%rcx), %rdi
	addl	-40(%rsp), %r15d
	addq	%rax, %r8
	cmpq	%r9, %rdx
	setnb	%r10b
	cmpq	%r11, %rdi
	setnb	%r9b
	orl	%r9d, %r10d
	leaq	32(%rax,%rbx), %r9
	leaq	(%rax,%rbx), %rsi
	cmpq	%r9, %rdx
	setnb	%r9b
	cmpq	%r11, %rsi
	setnb	%bl
	orl	%ebx, %r9d
	andl	%r9d, %r10d
	cmpl	$2, -56(%rsp)
	seta	%r9b
	testb	%r9b, %r10b
	je	.L63
	movq	-32(%rsp), %rbx
	leaq	32(%rax,%rbx), %r9
	cmpq	%r9, %rdx
	setnb	%r9b
	cmpq	%r8, %r11
	setbe	%r10b
	orb	%r10b, %r9b
	je	.L63
	leaq	(%rax,%rcx), %r9
	leaq	8(%rax,%rcx), %r10
	xorl	%ecx, %ecx
	.p2align 4,,10
	.p2align 3
.L56:
	vmovupd	(%r10,%rcx), %ymm1
	vaddpd	(%rdi,%rcx), %ymm1, %ymm0
	vmovupd	(%rsi,%rcx), %ymm1
	vaddpd	(%r8,%rcx), %ymm1, %ymm1
	vaddpd	%ymm1, %ymm0, %ymm0
	vmulpd	%ymm3, %ymm0, %ymm0
	vfmadd231pd	(%r9,%rcx), %ymm2, %ymm0
	vmovupd	%ymm0, (%rdx,%rcx)
	addq	$32, %rcx
	cmpq	%rcx, %r12
	jne	.L56
	movl	-72(%rsp), %ebx
	movl	-40(%rsp), %r10d
	cmpl	%r10d, %ebx
	je	.L59
	leal	(%rbx,%r14), %edx
	movl	-48(%rsp), %r11d
	movslq	%edx, %rdx
	leal	(%rbx,%r15), %edi
	leaq	0(,%rdx,8), %rcx
	movslq	%edi, %rdi
	movl	%r11d, %esi
	vmovsd	(%rax,%rdi,8), %xmm0
	vmovsd	-8(%rax,%rcx), %xmm1
	addl	%ebx, %esi
	movslq	%esi, %rsi
	vaddsd	(%rax,%rsi,8), %xmm0, %xmm0
	vaddsd	8(%rax,%rcx), %xmm1, %xmm1
	movl	-80(%rsp), %ebx
	vaddsd	%xmm1, %xmm0, %xmm0
	vmulsd	%xmm7, %xmm0, %xmm0
	vfmadd231sd	(%rax,%rdx,8), %xmm6, %xmm0
	vmovsd	%xmm0, 0(%r13,%rcx)
	cmpl	%r10d, %ebx
	jge	.L59
	leal	(%rbx,%r14), %edx
	movslq	%edx, %rdx
	leal	(%rbx,%r15), %edi
	leaq	0(,%rdx,8), %rcx
	movslq	%edi, %rdi
	vmovsd	(%rax,%rdi,8), %xmm0
	vmovsd	8(%rax,%rcx), %xmm1
	addl	%r11d, %ebx
	movslq	%ebx, %rsi
	vaddsd	(%rax,%rsi,8), %xmm0, %xmm0
	vaddsd	-8(%rax,%rcx), %xmm1, %xmm1
	movl	-84(%rsp), %edi
	vaddsd	%xmm1, %xmm0, %xmm0
	vmulsd	%xmm7, %xmm0, %xmm0
	vfmadd231sd	(%rax,%rdx,8), %xmm6, %xmm0
	vmovsd	%xmm0, 0(%r13,%rcx)
	cmpl	%edi, %r10d
	jle	.L59
	leal	(%rdi,%r14), %edx
	movslq	%edx, %rdx
	leal	(%rdi,%r15), %esi
	leaq	0(,%rdx,8), %rcx
	movslq	%esi, %rsi
	vmovsd	(%rax,%rsi,8), %xmm0
	vmovsd	8(%rax,%rcx), %xmm1
	addl	%edi, %r11d
	movslq	%r11d, %rdi
	vaddsd	(%rax,%rdi,8), %xmm0, %xmm0
	vaddsd	-8(%rax,%rcx), %xmm1, %xmm1
	vaddsd	%xmm1, %xmm0, %xmm0
	vmulsd	%xmm7, %xmm0, %xmm0
	vfmadd231sd	(%rax,%rdx,8), %xmm6, %xmm0
	vmovsd	%xmm0, 0(%r13,%rcx)
.L59:
	movq	-64(%rsp), %rbx
	movl	-24(%rsp), %edi
	addq	%rbx, (%rsp)
	addq	%rbx, -8(%rsp)
	addq	%rbx, -16(%rsp)
	cmpl	%edi, -52(%rsp)
	jne	.L60
	vzeroupper
.L76:
	leaq	-40(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
	.p2align 4,,10
	.p2align 3
.L81:
	.cfi_restore_state
	vxorpd	%xmm3, %xmm3, %xmm3
	vmaskmovpd	%ymm3, %ymm2, (%rdx)
	jmp	.L26
	.p2align 4,,10
	.p2align 3
.L80:
	vmovapd	.LC4(%rip), %ymm2
	vmaskmovpd	%ymm2, %ymm0, 32(%rdx)
	jmp	.L25
	.p2align 4,,10
	.p2align 3
.L79:
	vmovapd	.LC4(%rip), %ymm12
	vmaskmovpd	%ymm12, %ymm2, (%rdx)
	jmp	.L24
	.p2align 4,,10
	.p2align 3
.L30:
	movq	.LC7(%rip), %r14
	movq	%r14, (%r12)
	jmp	.L32
	.p2align 4,,10
	.p2align 3
.L33:
	movq	.LC7(%rip), %r15
	movq	%r15, (%r12)
	jmp	.L35
	.p2align 4,,10
	.p2align 3
.L36:
	movq	.LC7(%rip), %r14
	movq	%r14, (%r12)
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L39:
	movq	.LC7(%rip), %r15
	movq	%r15, (%r12)
	jmp	.L41
	.p2align 4,,10
	.p2align 3
.L42:
	movq	.LC7(%rip), %r14
	movq	%r14, (%r12)
	jmp	.L44
	.p2align 4,,10
	.p2align 3
.L45:
	movq	.LC7(%rip), %r15
	movq	%r15, (%r12)
	jmp	.L47
	.p2align 4,,10
	.p2align 3
.L73:
	vsubsd	-24(%rsp), %xmm0, %xmm0
	vmovsd	.LC6(%rip), %xmm7
	vxorpd	%xmm1, %xmm1, %xmm1
	vfmadd213sd	-32(%rsp), %xmm0, %xmm0
	vcmpltsd	%xmm13, %xmm0, %xmm0
	vblendvpd	%xmm0, %xmm7, %xmm1, %xmm0
	vmovsd	%xmm0, (%rdx)
	jmp	.L29
	.p2align 4,,10
	.p2align 3
.L63:
	movq	-8(%rsp), %rbx
	movq	(%rsp), %r8
	movq	-16(%rsp), %rdi
	leaq	(%rax,%rcx), %rsi
	subq	%rbx, %r8
	addq	-96(%rsp), %rcx
	subq	%rbx, %rdi
	.p2align 4,,10
	.p2align 3
.L58:
	vmovsd	(%rsi,%r8,8), %xmm0
	vmovsd	8(%rsi), %xmm1
	vaddsd	(%rsi,%rdi,8), %xmm0, %xmm0
	vaddsd	-8(%rsi), %xmm1, %xmm1
	addq	$8, %rsi
	addq	$8, %rdx
	vaddsd	%xmm1, %xmm0, %xmm0
	vmulsd	%xmm5, %xmm0, %xmm0
	vfmadd231sd	-8(%rsi), %xmm4, %xmm0
	vmovsd	%xmm0, -8(%rdx)
	cmpq	%rsi, %rcx
	jne	.L58
	jmp	.L59
.L64:
	xorl	%edx, %edx
	jmp	.L21
	.cfi_endproc
.LFE5320:
	.size	init_ro, .-init_ro
	.p2align 4
	.globl	steps
	.type	steps, @function
steps:
.LFB5321:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	andq	$-32, %rsp
	subq	$40, %rsp
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	.cfi_offset 3, -56
	movq	%rdi, -8(%rsp)
	movq	%rdx, -16(%rsp)
	movl	%ecx, (%rsp)
	cmpl	$2, %r8d
	jle	.L94
	leal	-1(%rcx), %eax
	movq	%rsi, %r11
	movl	%eax, -28(%rsp)
	leal	-2(%rcx), %esi
	leal	-1(%r8), %eax
	movl	%eax, -32(%rsp)
	movl	%esi, %eax
	shrl	$2, %eax
	salq	$5, %rax
	movl	%esi, -72(%rsp)
	andl	$-4, %esi
	movq	%rax, -80(%rsp)
	leal	1(%rsi), %eax
	movl	%eax, -92(%rsp)
	leal	2(%rsi), %eax
	movl	%eax, -88(%rsp)
	leal	3(%rsi), %eax
	movl	%eax, -96(%rsp)
	movslq	%ecx, %rax
	leaq	(%r11,%rax,8), %rbx
	movq	%rax, -64(%rsp)
	negq	%rax
	leaq	(%r11,%rax,8), %r13
	leal	-3(%rcx), %eax
	movl	%eax, -68(%rsp)
	addq	$2, %rax
	vmovq	.LC14(%rip), %xmm11
	movl	%esi, -84(%rsp)
	movl	%ecx, 32(%rsp)
	movl	$1, 36(%rsp)
	movq	%rax, -104(%rsp)
	movq	%r13, -112(%rsp)
	movq	%rbx, -120(%rsp)
	vmovapd	%xmm0, %xmm10
	vmovapd	.LC13(%rip), %ymm5
	vxorpd	%xmm0, %xmm0, %xmm0
	vbroadcastsd	%xmm10, %ymm9
	vbroadcastsd	%xmm1, %ymm8
	vbroadcastsd	%xmm3, %ymm7
	vbroadcastsd	%xmm2, %ymm6
	vmovapd	%xmm11, %xmm12
	.p2align 4,,10
	.p2align 3
.L86:
	cmpl	$1, -28(%rsp)
	jle	.L93
	movslq	32(%rsp), %rbx
	movq	-64(%rsp), %rsi
	leaq	8(,%rbx,8), %rax
	movq	-8(%rsp), %rdi
	leaq	5(%rsi,%rbx), %rcx
	movq	%rbx, -56(%rsp)
	subq	%rsi, %rbx
	salq	$3, %rcx
	leaq	40(,%rbx,8), %r9
	leaq	32(%rax), %r13
	leaq	(%rdi,%r13), %r15
	leaq	(%rdi,%rax), %rdx
	leaq	-32(%r9), %rsi
	leaq	-32(%rcx), %rdi
	movq	%rdi, -48(%rsp)
	movq	%rsi, -40(%rsp)
	leaq	-40(%r11,%rcx), %rdi
	leaq	-8(%r11,%rax), %rsi
	movq	-16(%rsp), %r8
	movq	%rdi, 8(%rsp)
	movq	%rsi, 16(%rsp)
	leaq	40(%r11,%rax), %rdi
	leaq	8(%rax), %rsi
	cmpq	%rdi, %rdx
	leaq	-40(%r11,%r9), %r14
	leaq	(%r11,%rsi), %r10
	leaq	(%r8,%rax), %rbx
	movq	%r14, -24(%rsp)
	setnb	%r14b
	cmpq	%r15, %r10
	setnb	%dil
	movq	%rbx, 24(%rsp)
	orl	%edi, %r14d
	leaq	(%r8,%r13), %rdi
	cmpq	%rdi, %rdx
	setnb	%dil
	cmpq	%r15, 24(%rsp)
	setnb	7(%rsp)
	movzbl	7(%rsp), %r8d
	leaq	-24(%r11,%rcx), %r12
	orl	%r8d, %edi
	andl	%edi, %r14d
	cmpl	$2, -68(%rsp)
	seta	%dil
	addq	%r11, %r13
	andl	%edi, %r14d
	cmpq	%r13, %rdx
	setnb	%dil
	cmpq	%r15, 16(%rsp)
	setnb	%r13b
	orl	%r13d, %edi
	andl	%edi, %r14d
	leaq	8(%r11,%rcx), %rdi
	cmpq	%rdi, %rdx
	setnb	%r13b
	cmpq	%r15, %r12
	setnb	%dil
	orl	%edi, %r13d
	leaq	8(%r11,%r9), %rdi
	andl	%r13d, %r14d
	leaq	-24(%r11,%r9), %rbx
	cmpq	%rdi, %rdx
	setnb	%dil
	cmpq	%r15, %rbx
	setnb	%r13b
	orl	%r13d, %edi
	movq	%r15, %r8
	addq	%r11, %rcx
	movq	8(%rsp), %r15
	andl	%edi, %r14d
	cmpq	%rcx, %rdx
	setnb	%cl
	cmpq	%r8, %r15
	setnb	%dil
	orl	%edi, %ecx
	testb	%cl, %r14b
	je	.L87
	movq	-24(%rsp), %r14
	addq	%r11, %r9
	cmpq	%r9, %rdx
	setnb	%cl
	cmpq	%r8, %r14
	setnb	%dil
	orb	%dil, %cl
	je	.L87
	movq	-48(%rsp), %rdi
	movq	-40(%rsp), %rsi
	movq	24(%rsp), %r9
	movq	-80(%rsp), %r8
	movq	16(%rsp), %r13
	addq	%r11, %rdi
	addq	%r11, %rsi
	addq	%r11, %rax
	vbroadcastsd	%xmm0, %ymm0
	xorl	%ecx, %ecx
	.p2align 4,,10
	.p2align 3
.L88:
	vmovupd	0(%r13,%rcx), %ymm4
	vmovupd	(%r14,%rcx), %ymm15
	vaddpd	(%r10,%rcx), %ymm4, %ymm13
	vmovupd	(%rdi,%rcx), %ymm4
	vaddpd	(%rsi,%rcx), %ymm4, %ymm4
	vmulpd	%ymm8, %ymm4, %ymm4
	vfmadd231pd	%ymm9, %ymm13, %ymm4
	vaddpd	(%rbx,%rcx), %ymm15, %ymm13
	vmovupd	(%r15,%rcx), %ymm15
	vaddpd	(%r12,%rcx), %ymm15, %ymm14
	vaddpd	%ymm14, %ymm13, %ymm13
	vfmadd213pd	(%r9,%rcx), %ymm7, %ymm13
	vaddpd	%ymm13, %ymm4, %ymm4
	vmulpd	%ymm6, %ymm4, %ymm4
	vmovupd	%ymm4, (%rdx,%rcx)
	vsubpd	(%rax,%rcx), %ymm4, %ymm4
	addq	$32, %rcx
	vandpd	%ymm5, %ymm4, %ymm4
	vmaxpd	%ymm4, %ymm0, %ymm0
	cmpq	%rcx, %r8
	jne	.L88
	vextractf128	$0x1, %ymm0, %xmm4
	vmaxpd	%xmm0, %xmm4, %xmm0
	movl	-84(%rsp), %edx
	vunpckhpd	%xmm0, %xmm0, %xmm4
	vmaxpd	%xmm0, %xmm4, %xmm0
	cmpl	%edx, -72(%rsp)
	je	.L93
	movl	32(%rsp), %esi
	movl	-92(%rsp), %eax
	movl	(%rsp), %ebx
	addl	%esi, %eax
	movslq	%eax, %rdx
	vmovsd	-8(%r11,%rdx,8), %xmm4
	leal	(%rax,%rbx), %ecx
	movslq	%ecx, %rcx
	vaddsd	8(%r11,%rdx,8), %xmm4, %xmm13
	subl	%ebx, %eax
	vmovsd	(%r11,%rcx,8), %xmm4
	cltq
	vaddsd	(%r11,%rax,8), %xmm4, %xmm4
	vmovsd	-8(%r11,%rcx,8), %xmm14
	movq	-16(%rsp), %rdi
	vmulsd	%xmm1, %xmm4, %xmm4
	vaddsd	8(%r11,%rcx,8), %xmm14, %xmm14
	movq	-8(%rsp), %r10
	movl	-28(%rsp), %r9d
	vfmadd231sd	%xmm10, %xmm13, %xmm4
	vmovsd	-8(%r11,%rax,8), %xmm13
	vaddsd	8(%r11,%rax,8), %xmm13, %xmm13
	vaddsd	%xmm14, %xmm13, %xmm13
	vfmadd213sd	(%rdi,%rdx,8), %xmm3, %xmm13
	vaddsd	%xmm13, %xmm4, %xmm4
	vmulsd	%xmm2, %xmm4, %xmm4
	vmovsd	%xmm4, (%r10,%rdx,8)
	vsubsd	(%r11,%rdx,8), %xmm4, %xmm4
	movl	-88(%rsp), %edx
	vandpd	%xmm12, %xmm4, %xmm4
	vmaxsd	%xmm4, %xmm0, %xmm0
	cmpl	%edx, %r9d
	jle	.L93
	movl	%esi, %eax
	addl	%edx, %eax
	movslq	%eax, %rdx
	vmovsd	-8(%r11,%rdx,8), %xmm4
	leal	(%rbx,%rax), %ecx
	movslq	%ecx, %rcx
	vaddsd	8(%r11,%rdx,8), %xmm4, %xmm13
	subl	%ebx, %eax
	vmovsd	(%r11,%rcx,8), %xmm4
	cltq
	vaddsd	(%r11,%rax,8), %xmm4, %xmm4
	vmovsd	-8(%r11,%rcx,8), %xmm14
	vmulsd	%xmm1, %xmm4, %xmm4
	vaddsd	8(%r11,%rcx,8), %xmm14, %xmm14
	vfmadd231sd	%xmm13, %xmm10, %xmm4
	vmovsd	-8(%r11,%rax,8), %xmm13
	vaddsd	8(%r11,%rax,8), %xmm13, %xmm13
	vaddsd	%xmm14, %xmm13, %xmm13
	vfmadd213sd	(%rdi,%rdx,8), %xmm3, %xmm13
	vaddsd	%xmm13, %xmm4, %xmm4
	vmulsd	%xmm2, %xmm4, %xmm4
	vmovsd	%xmm4, (%r10,%rdx,8)
	vsubsd	(%r11,%rdx,8), %xmm4, %xmm4
	movl	-96(%rsp), %edx
	vandpd	%xmm12, %xmm4, %xmm4
	vmaxsd	%xmm4, %xmm0, %xmm0
	cmpl	%edx, %r9d
	jle	.L93
	movl	%esi, %eax
	addl	%edx, %eax
	movslq	%eax, %rdx
	vmovsd	-8(%r11,%rdx,8), %xmm4
	leal	(%rbx,%rax), %ecx
	movslq	%ecx, %rcx
	vaddsd	8(%r11,%rdx,8), %xmm4, %xmm13
	subl	%ebx, %eax
	vmovsd	(%r11,%rcx,8), %xmm4
	cltq
	vaddsd	(%r11,%rax,8), %xmm4, %xmm4
	vmovsd	-8(%r11,%rcx,8), %xmm14
	vmulsd	%xmm1, %xmm4, %xmm4
	vaddsd	8(%r11,%rcx,8), %xmm14, %xmm14
	vfmadd231sd	%xmm13, %xmm10, %xmm4
	vmovsd	-8(%r11,%rax,8), %xmm13
	vaddsd	8(%r11,%rax,8), %xmm13, %xmm13
	vaddsd	%xmm14, %xmm13, %xmm13
	vfmadd213sd	(%rdi,%rdx,8), %xmm3, %xmm13
	vaddsd	%xmm13, %xmm4, %xmm4
	vmulsd	%xmm2, %xmm4, %xmm4
	vmovsd	%xmm4, (%r10,%rdx,8)
	vsubsd	(%r11,%rdx,8), %xmm4, %xmm4
	vandpd	%xmm12, %xmm4, %xmm4
	vmaxsd	%xmm4, %xmm0, %xmm0
.L93:
	incl	36(%rsp)
	movl	(%rsp), %edx
	addl	%edx, 32(%rsp)
	movl	36(%rsp), %eax
	cmpl	-32(%rsp), %eax
	jne	.L86
	vzeroupper
	leaq	-40(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
	.p2align 4,,10
	.p2align 3
.L87:
	.cfi_restore_state
	movq	-56(%rsp), %rdx
	movq	-112(%rsp), %r15
	addq	-104(%rsp), %rdx
	movq	-120(%rsp), %r8
	movq	-8(%rsp), %rcx
	movq	-16(%rsp), %rdi
	salq	$3, %rdx
	jmp	.L90
	.p2align 4,,10
	.p2align 3
.L107:
	addq	$8, %rsi
.L90:
	vmovsd	-8(%r11,%rax), %xmm4
	vmovsd	-8(%r8,%rax), %xmm14
	vaddsd	8(%r11,%rax), %xmm4, %xmm13
	vmovsd	(%r8,%rax), %xmm4
	vaddsd	8(%r8,%rax), %xmm14, %xmm14
	vaddsd	(%r15,%rax), %xmm4, %xmm4
	vmulsd	%xmm1, %xmm4, %xmm4
	vfmadd231sd	%xmm13, %xmm10, %xmm4
	vmovsd	-8(%r15,%rax), %xmm13
	vaddsd	8(%r15,%rax), %xmm13, %xmm13
	vaddsd	%xmm14, %xmm13, %xmm13
	vfmadd213sd	(%rdi,%rax), %xmm3, %xmm13
	vaddsd	%xmm13, %xmm4, %xmm4
	vmulsd	%xmm2, %xmm4, %xmm4
	vmovsd	%xmm4, (%rcx,%rax)
	vsubsd	(%r11,%rax), %xmm4, %xmm4
	movq	%rsi, %rax
	vandpd	%xmm11, %xmm4, %xmm4
	vmaxsd	%xmm4, %xmm0, %xmm0
	cmpq	%rsi, %rdx
	jne	.L107
	jmp	.L93
.L94:
	leaq	-40(%rbp), %rsp
	vxorpd	%xmm0, %xmm0, %xmm0
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5321:
	.size	steps, .-steps
	.p2align 4
	.globl	v_steps
	.type	v_steps, @function
v_steps:
.LFB5322:
	.cfi_startproc
	endbr64
	cmpl	$2, %r8d
	jle	.L116
	cmpl	$2, %ecx
	jle	.L116
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	leal	-3(%rcx), %eax
	shrl	$2, %eax
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r13
	movl	$1, %r11d
	.cfi_offset 13, -24
	leaq	5(,%rax,4), %r13
	pushq	%r12
	.cfi_offset 12, -32
	movslq	%ecx, %r12
	movq	%r12, %rcx
	pushq	%rbx
	negq	%rcx
	.cfi_offset 3, -40
	leal	-1(%r8), %ebx
	andq	$-32, %rsp
	leaq	(%rsi,%r12,8), %r8
	leaq	(%rsi,%rcx,8), %rcx
	movq	%r12, %r10
	.p2align 4,,10
	.p2align 3
.L111:
	leaq	0(%r13,%r10), %r9
	leaq	8(,%r10,8), %rax
	salq	$3, %r9
	.p2align 4,,10
	.p2align 3
.L112:
	vmovapd	(%r8,%rax), %ymm6
	vmovapd	-8(%rsi,%rax), %ymm7
	vaddpd	(%rcx,%rax), %ymm6, %ymm5
	vaddpd	8(%rsi,%rax), %ymm7, %ymm4
	vmovapd	-8(%rcx,%rax), %ymm6
	vmulpd	%ymm1, %ymm5, %ymm5
	vmovapd	-8(%r8,%rax), %ymm7
	vaddpd	8(%rcx,%rax), %ymm6, %ymm6
	vfmadd132pd	%ymm0, %ymm5, %ymm4
	vaddpd	8(%r8,%rax), %ymm7, %ymm5
	vaddpd	%ymm6, %ymm5, %ymm5
	vfmadd213pd	(%rdx,%rax), %ymm3, %ymm5
	vaddpd	%ymm5, %ymm4, %ymm4
	vmulpd	%ymm2, %ymm4, %ymm4
	vmovupd	%ymm4, (%rdi,%rax)
	addq	$32, %rax
	cmpq	%rax, %r9
	jne	.L112
	incl	%r11d
	addq	%r12, %r10
	cmpl	%ebx, %r11d
	jne	.L111
	leaq	-24(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	vxorpd	%xmm0, %xmm0, %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
.L116:
	.cfi_restore 3
	.cfi_restore 6
	.cfi_restore 12
	.cfi_restore 13
	vxorpd	%xmm0, %xmm0, %xmm0
	ret
	.cfi_endproc
.LFE5322:
	.size	v_steps, .-v_steps
	.section	.rodata.str1.1
.LC22:
	.string	"wb"
.LC23:
	.string	"file"
.LC25:
	.string	"Time: %lf\n"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC26:
	.string	"delta = %e; Nx = %d, Ny = %d, Nt = %d\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB5323:
	.cfi_startproc
	endbr64
	leaq	8(%rsp), %r10
	.cfi_def_cfa 10, 0
	andq	$-32, %rsp
	pushq	-8(%r10)
	movl	$8, %esi
	movl	$2, %edi
	pushq	%rbp
	movq	%rsp, %rbp
	.cfi_escape 0x10,0x6,0x2,0x76,0
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%r10
	.cfi_escape 0xf,0x3,0x76,0x60,0x6
	.cfi_escape 0x10,0xe,0x2,0x76,0x78
	.cfi_escape 0x10,0xd,0x2,0x76,0x70
	.cfi_escape 0x10,0xc,0x2,0x76,0x68
	pushq	%rbx
	subq	$72, %rsp
	.cfi_escape 0x10,0x3,0x2,0x76,0x58
	movq	%fs:40, %rax
	movq	%rax, -56(%rbp)
	xorl	%eax, %eax
	call	calloc@PLT
	movq	%rax, %r13
	call	_mm_malloc.constprop.0
	movq	%rax, %r12
	movq	%rax, 0(%r13)
	call	_mm_malloc.constprop.0
	movq	%rax, %r14
	movl	$512000000, %edx
	xorl	%esi, %esi
	movq	%r12, %rdi
	movq	%rax, 8(%r13)
	call	memset@PLT
	movl	$512000000, %edx
	xorl	%esi, %esi
	movq	%r14, %rdi
	call	memset@PLT
	movl	$8, %esi
	movl	$2, %edi
	call	calloc@PLT
	movq	%rax, %rbx
	call	_mm_malloc.constprop.0
	movq	%rax, (%rbx)
	call	_mm_malloc.constprop.0
	movq	(%rbx), %rdi
	movl	$512000000, %edx
	xorl	%esi, %esi
	movq	%rax, 8(%rbx)
	call	memset@PLT
	movq	8(%rbx), %rdi
	movl	$512000000, %edx
	xorl	%esi, %esi
	call	memset@PLT
	vmovsd	.LC15(%rip), %xmm4
	vmovsd	.LC18(%rip), %xmm2
	vmovsd	.LC16(%rip), %xmm0
	vmovsd	.LC17(%rip), %xmm1
	subq	$16, %rsp
	vxorpd	%xmm7, %xmm7, %xmm7
	vmovsd	%xmm2, 8(%rsp)
	vmovsd	%xmm2, (%rsp)
	vmovapd	%xmm4, %xmm5
	vmovapd	%xmm0, %xmm3
	movl	$8000, %edx
	vmovapd	%xmm7, %xmm6
	vmovapd	%xmm1, %xmm2
	movl	$8000, %esi
	movq	%rbx, %rdi
	call	init_ro
	leaq	-96(%rbp), %rsi
	movl	$4, %edi
	call	clock_gettime@PLT
	popq	%rax
	vmovapd	.LC19(%rip), %ymm5
	vmovapd	.LC20(%rip), %ymm4
	vmovapd	.LC21(%rip), %ymm3
	popq	%rdx
	movq	%r12, %r8
	xorl	%esi, %esi
	movl	$8, %r11d
	.p2align 4,,10
	.p2align 3
.L123:
	incl	%esi
	movl	%esi, %eax
	movq	%r8, %rdx
	andl	$1, %eax
	movq	%r11, %r9
	movq	8(%rbx), %r10
	movq	0(%r13,%rax,8), %r8
	movl	$8000, %ecx
	leaq	64000(%rdx), %rax
	subq	%rdx, %r9
	.p2align 4,,10
	.p2align 3
.L120:
	leaq	(%r9,%rax), %rdx
	leaq	64000(%rax), %rdi
	.p2align 4,,10
	.p2align 3
.L121:
	vmovapd	-64000(%rax), %ymm6
	vmovapd	64000(%rax), %ymm7
	vaddpd	-63984(%rax), %ymm6, %ymm0
	vaddpd	64016(%rax), %ymm7, %ymm1
	vmovapd	(%rax), %ymm6
	vmovapd	64008(%rax), %ymm7
	vaddpd	%ymm1, %ymm0, %ymm0
	vaddpd	-63992(%rax), %ymm7, %ymm2
	vaddpd	16(%rax), %ymm6, %ymm1
	addq	$32, %rax
	vaddpd	%ymm2, %ymm1, %ymm1
	vmulpd	%ymm5, %ymm1, %ymm1
	vfmadd132pd	%ymm4, %ymm1, %ymm0
	vaddpd	(%r10,%rdx), %ymm0, %ymm0
	vmulpd	%ymm3, %ymm0, %ymm0
	vmovupd	%ymm0, (%r8,%rdx)
	addq	$32, %rdx
	cmpq	%rax, %rdi
	jne	.L121
	addq	$8000, %rcx
	cmpq	$63992000, %rcx
	je	.L122
	movq	%rdi, %rax
	jmp	.L120
	.p2align 4,,10
	.p2align 3
.L122:
	cmpl	$100, %esi
	jne	.L123
	leaq	-80(%rbp), %rsi
	movl	$4, %edi
	vzeroupper
	call	clock_gettime@PLT
	leaq	.LC22(%rip), %rsi
	leaq	.LC23(%rip), %rdi
	call	fopen@PLT
	movq	%rax, %rcx
	movl	$64000000, %edx
	movl	$8, %esi
	movq	%r12, %rdi
	call	fwrite@PLT
	movq	-72(%rbp), %rax
	vxorps	%xmm2, %xmm2, %xmm2
	subq	-88(%rbp), %rax
	vcvtsi2sdq	%rax, %xmm2, %xmm0
	movq	-80(%rbp), %rax
	leaq	.LC25(%rip), %rsi
	subq	-96(%rbp), %rax
	vcvtsi2sdq	%rax, %xmm2, %xmm2
	vmovapd	%xmm0, %xmm1
	movl	$1, %edi
	movl	$1, %eax
	vfmadd132sd	.LC24(%rip), %xmm2, %xmm1
	vmovapd	%xmm1, %xmm0
	call	__printf_chk@PLT
	movl	$100, %r8d
	movl	$8000, %ecx
	movl	$8000, %edx
	vxorpd	%xmm0, %xmm0, %xmm0
	leaq	.LC26(%rip), %rsi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk@PLT
	movq	-56(%rbp), %rax
	xorq	%fs:40, %rax
	jne	.L128
	leaq	-40(%rbp), %rsp
	popq	%rbx
	popq	%r10
	.cfi_remember_state
	.cfi_def_cfa 10, 0
	popq	%r12
	popq	%r13
	popq	%r14
	xorl	%eax, %eax
	popq	%rbp
	leaq	-8(%r10), %rsp
	.cfi_def_cfa 7, 8
	ret
.L128:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE5323:
	.size	main, .-main
	.section	.rodata.cst32,"aM",@progbits,32
	.align 32
.LC1:
	.long	0
	.long	1
	.long	2
	.long	3
	.long	4
	.long	5
	.long	6
	.long	7
	.align 32
.LC2:
	.long	8
	.long	8
	.long	8
	.long	8
	.long	8
	.long	8
	.long	8
	.long	8
	.align 32
.LC3:
	.long	0
	.long	1072693248
	.long	0
	.long	1072693248
	.long	0
	.long	1072693248
	.long	0
	.long	1072693248
	.align 32
.LC4:
	.long	0
	.long	-1074790400
	.long	0
	.long	-1074790400
	.long	0
	.long	-1074790400
	.long	0
	.long	-1074790400
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC6:
	.long	0
	.long	-1074790400
	.align 8
.LC7:
	.long	0
	.long	1072693248
	.align 8
.LC8:
	.long	2576980378
	.long	1069128089
	.section	.rodata.cst32
	.align 32
.LC9:
	.long	0
	.long	1070596096
	.long	0
	.long	1070596096
	.long	0
	.long	1070596096
	.long	0
	.long	1070596096
	.align 32
.LC10:
	.long	0
	.long	1073741824
	.long	0
	.long	1073741824
	.long	0
	.long	1073741824
	.long	0
	.long	1073741824
	.section	.rodata.cst8
	.align 8
.LC11:
	.long	0
	.long	1070596096
	.align 8
.LC12:
	.long	0
	.long	1073741824
	.section	.rodata.cst32
	.align 32
.LC13:
	.long	4294967295
	.long	2147483647
	.long	4294967295
	.long	2147483647
	.long	4294967295
	.long	2147483647
	.long	4294967295
	.long	2147483647
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC14:
	.long	4294967295
	.long	2147483647
	.long	0
	.long	0
	.section	.rodata.cst8
	.align 8
.LC15:
	.long	251286998
	.long	1061184212
	.align 8
.LC16:
	.long	1431655765
	.long	1073042773
	.align 8
.LC17:
	.long	1431655765
	.long	1074091349
	.align 8
.LC18:
	.long	0
	.long	1074790400
	.section	.rodata.cst32
	.align 32
.LC19:
	.long	134217728
	.long	1096712844
	.long	134217728
	.long	1096712844
	.long	134217728
	.long	1096712844
	.long	134217728
	.long	1096712844
	.align 32
.LC20:
	.long	134217728
	.long	1094615692
	.long	134217728
	.long	1094615692
	.long	134217728
	.long	1094615692
	.long	134217728
	.long	1094615692
	.align 32
.LC21:
	.long	2111276338
	.long	1046141354
	.long	2111276338
	.long	1046141354
	.long	2111276338
	.long	1046141354
	.long	2111276338
	.long	1046141354
	.section	.rodata.cst8
	.align 8
.LC24:
	.long	3894859413
	.long	1041313291
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
