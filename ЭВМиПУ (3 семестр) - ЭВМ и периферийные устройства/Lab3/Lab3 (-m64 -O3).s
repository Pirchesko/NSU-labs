	.file	"Lab3.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Before: "
.LC1:
	.string	"%d; "
.LC2:
	.string	"..."
.LC3:
	.string	"Start sort..."
.LC4:
	.string	"Finish!"
.LC5:
	.string	"Time = %d\n"
.LC6:
	.string	"After: "
.LC7:
	.string	"...\n"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB8:
	.section	.text.startup,"ax",@progbits
.LHOTB8:
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB38:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	movl	$1, %edi
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	subq	$480024, %rsp
	.cfi_def_cfa_offset 480064
	leaq	480000(%rsp), %r12 #запомнием адресок конца массива
	movq	%rsp, %rbx #начала масстива
	movq	%rsp, %rbp
	movq	%fs:40, %rax
	movq	%rax, 480008(%rsp)
	xorl	%eax, %eax
	call	srand #начинается программа
	.p2align 4,,10
	.p2align 3
.L2:
	call	rand
	addq	$4, %rbp
	movl	%eax, -4(%rbp)
	cmpq	%rbp, %r12
	jne	.L2
	leaq	20(%rsp), %rbp
	movl	$.LC0, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	movq	%rsp, %r12
	call	__printf_chk
.L3:
	movl	(%r12), %edx #будем выводить первые  5 элементов
	xorl	%eax, %eax
	movl	$.LC1, %esi
	movl	$1, %edi
	addq	$4, %r12
	call	__printf_chk
	cmpq	%rbp, %r12
	jne	.L3
	movl	$.LC2, %edi
	call	puts
	movl	$.LC3, %edi
	call	puts
	call	clock
	xorl	%edi, %edi
	movq	%rax, %r12 #фиксируем время страрта
	.p2align 4,,10
	.p2align 3
.L4: #начинаем сортировать
	movl	%edi, %eax 
	.p2align 4,,10
	.p2align 3
.L6:
	movslq	%eax, %rdx
	movl	(%rsp,%rdi,4), %ecx #записываем в память massive[i] и massive[j]
	movl	(%rsp,%rdx,4), %esi
	cmpl	%esi, %ecx
	jge	.L5 # и сравниваем их
	movl	%esi, (%rsp,%rdi,4)
	movl	%ecx, (%rsp,%rdx,4)
.L5:
	addl	$1, %eax #j++
	cmpl	$120000, %eax
	jne	.L6
	addq	$1, %rdi #i++
	cmpq	$120000, %rdi
	jne	.L4
	call	clock #фиксируем финишное время
	movl	$.LC4, %edi
	movq	%rax, %r13 #finish
	call	puts
	movl	%r13d, %edx #finish - start
	movl	$.LC5, %esi
	movl	$1, %edi
	subl	%r12d, %edx
	xorl	%eax, %eax
	call	__printf_chk #выводим время
	movl	$.LC6, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk
.L8:
	movl	(%rbx), %edx #начинаем выводить 5 элементов
	xorl	%eax, %eax
	movl	$.LC1, %esi
	movl	$1, %edi
	addq	$4, %rbx
	call	__printf_chk
	cmpq	%rbx, %rbp
	jne	.L8
	movl	$.LC7, %edi
	call	puts
	xorl	%eax, %eax
	movq	480008(%rsp), %rbx
	xorq	%fs:40, %rbx
	jne	.L16
	addq	$480024, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
.L16:
	.cfi_restore_state
	call	__stack_chk_fail
	.cfi_endproc
.LFE38:
	.size	main, .-main
	.section	.text.unlikely
.LCOLDE8:
	.section	.text.startup
.LHOTE8:
	.ident	"GCC: (Ubuntu 5.5.0-12ubuntu1~16.04) 5.5.0 20171010"
	.section	.note.GNU-stack,"",@progbits
