	.file	"Lab3.c"
	.section	.rodata
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
	.text
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$480048, %rsp #резервируем пам€ть дл€ массива и прочих элемментов
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$1, %edi #srand(1)
	call	srand
	movl	$0, -480048(%rbp)
	jmp	.L2
.L3:	#заполн€ем цикл рандомами
	call	rand
	movl	%eax, %edx
	movl	-480048(%rbp), %eax
	cltq	#https://sourceware.org/binutils/docs/as/i386_002dMnemonics.html (sign-extend dword in С%eaxТ to quad in С%raxТ)
	movl	%edx, -480016(%rbp,%rax,4)
	addl	$1, -480048(%rbp)
.L2:
	cmpl	$119999, -480048(%rbp)
	jle	.L3
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movl	$0, -480044(%rbp)	#вывод первых 5-и элементов
	jmp	.L4
.L5:
	movl	-480044(%rbp), %eax
	cltq
	movl	-480016(%rbp,%rax,4), %eax
	movl	%eax, %esi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	addl	$1, -480044(%rbp)
.L4:
	cmpl	$4, -480044(%rbp)
	jle	.L5
	movl	$.LC2, %edi
	call	puts
	movl	$.LC3, %edi
	call	puts
	call	clock
	movl	%eax, -480028(%rbp)	#фиксируем врем€ start
	movl	$0, -480040(%rbp)	#начало сортировки
	jmp	.L6
.L10:
	movl	-480040(%rbp), %eax
	movl	%eax, -480036(%rbp) # j := i
	jmp	.L7
.L9:
	movl	-480040(%rbp), %eax #i
	cltq
	movl	-480016(%rbp,%rax,4), %edx #massive[i]
	movl	-480036(%rbp), %eax #j
	cltq
	movl	-480016(%rbp,%rax,4), %eax #massive[j]
	cmpl	%eax, %edx
	jge	.L8 # >= то мен€ем
	movl	-480040(%rbp), %eax
	cltq
	movl	-480016(%rbp,%rax,4), %eax
	movl	%eax, -480024(%rbp)
	movl	-480036(%rbp), %eax
	cltq
	movl	-480016(%rbp,%rax,4), %edx
	movl	-480040(%rbp), %eax
	cltq
	movl	%edx, -480016(%rbp,%rax,4)
	movl	-480036(%rbp), %eax
	cltq
	movl	-480024(%rbp), %edx
	movl	%edx, -480016(%rbp,%rax,4)
.L8:
	addl	$1, -480036(%rbp) # j++ 
.L7:
	cmpl	$119999, -480036(%rbp) # j<N
	jle	.L9
	addl	$1, -480040(%rbp) #i++
.L6:
	cmpl	$119999, -480040(%rbp) # i<N
	jle	.L10
	call	clock
	movl	%eax, -480020(%rbp) # фиксируем finish
	movl	$.LC4, %edi
	call	puts
	movl	-480020(%rbp), %eax
	subl	-480028(%rbp), %eax # finish - start 
	movl	%eax, %esi
	movl	$.LC5, %edi
	movl	$0, %eax
	call	printf
	movl	$.LC6, %edi
	movl	$0, %eax
	call	printf
	movl	$0, -480032(%rbp) #выводим первые 5 элементов
	jmp	.L11
.L12:
	movl	-480032(%rbp), %eax
	cltq
	movl	-480016(%rbp,%rax,4), %eax
	movl	%eax, %esi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	addl	$1, -480032(%rbp)
.L11:
	cmpl	$4, -480032(%rbp)
	jle	.L12
	movl	$.LC7, %edi
	call	puts
	movl	$0, %eax
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L14
	call	__stack_chk_fail
.L14:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.5.0-12ubuntu1~16.04) 5.5.0 20171010"
	.section	.note.GNU-stack,"",@progbits
