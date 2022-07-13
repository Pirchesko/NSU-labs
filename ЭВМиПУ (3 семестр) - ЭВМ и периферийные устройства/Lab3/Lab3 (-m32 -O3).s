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
	leal	4(%esp), %ecx
	.cfi_def_cfa 1, 0
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	.cfi_escape 0x10,0x5,0x2,0x75,0
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	pushl	%ecx
	.cfi_escape 0xf,0x3,0x75,0x70,0x6
	.cfi_escape 0x10,0x7,0x2,0x75,0x7c
	.cfi_escape 0x10,0x6,0x2,0x75,0x78
	.cfi_escape 0x10,0x3,0x2,0x75,0x74
	leal	-480028(%ebp), %esi	#сохраняем адреса границ массива
	leal	-28(%ebp), %edi #от edi до esi вверх по стеку
	subl	$480036, %esp
	movl	%esi, %ebx
	pushl	$1
	movl	%gs:20, %eax
	movl	%eax, -28(%ebp)
	xorl	%eax, %eax
	call	srand #начинается программа
	addl	$16, %esp
	.p2align 4,,10 #https://sourceware.org/binutils/docs/as/P2align.html#P2align
	.p2align 3
.L2:
	call	rand
	addl	$4, %ebx #i+1 (по адресу)
	movl	%eax, -4(%ebx) #зваписываем massive[i-1]
	cmpl	%ebx, %edi #сравниваем пока не не дойдём доконца
	jne	.L2
	subl	$8, %esp
	xorl	%ebx, %ebx
	pushl	$.LC0
	pushl	$1
	call	__printf_chk
	addl	$16, %esp
.L3:
	subl	$4, %esp #будем выводить первые  5 элементов
	pushl	-480028(%ebp,%ebx,4) #massive[i]
	addl	$1, %ebx #i++
	pushl	$.LC1
	pushl	$1
	call	__printf_chk
	addl	$16, %esp
	cmpl	$5, %ebx #i<5
	jne	.L3	#i!=5
	subl	$12, %esp 
	pushl	$.LC2 #"..."
	call	puts
	movl	$.LC3, (%esp) # "Start sort"
	call	puts
	call	clock
	addl	$16, %esp
	movl	%eax, %ebx #фиксируем время страрта
	.p2align 4,,10
	.p2align 3
.L4:
	movl	%esi, %eax # берём адресс начала массива
	.p2align 4,,10
	.p2align 3
.L6:
	movl	(%esi), %edx #записываем в память massive[i] и massive[j]
	movl	(%eax), %ecx
	cmpl	%ecx, %edx 
	jge	.L5	# и сравниваем их
	movl	%ecx, (%esi) # и если их надо поменять местами то меняем
	movl	%edx, (%eax)
.L5:
	addl	$4, %eax #j++
	cmpl	%eax, %edi #пока j!=N
	jne	.L6
	addl	$4, %esi #i++
	cmpl	%edi, %esi #пока i !=N
	jne	.L4
	call	clock #фиксируем финишное время
	subl	$12, %esp
	movl	%eax, %esi #finish
	pushl	$.LC4
	subl	%ebx, %esi #finish - start
	xorl	%ebx, %ebx
	call	puts
	addl	$12, %esp
	pushl	%esi
	pushl	$.LC5
	pushl	$1
	call	__printf_chk #выводим время
	popl	%eax
	popl	%edx
	pushl	$.LC6 #"After"
	pushl	$1
	call	__printf_chk
	addl	$16, %esp
.L8:
	subl	$4, %esp #начинаем выводить 5 элементов 
	pushl	-480028(%ebp,%ebx,4) #massive[i]
	addl	$1, %ebx #i++
	pushl	$.LC1
	pushl	$1
	call	__printf_chk
	addl	$16, %esp
	addl	$16, %esp
	cmpl	$5, %ebx #i!=5
	jne	.L8
	subl	$12, %esp #просто выводим первод новой строки
	pushl	$.LC7
	call	puts
	addl	$16, %esp
	xorl	%eax, %eax #тут идёт массовая зачитска перед выходом программы
	movl	-28(%ebp), %ebx
	xorl	%gs:20, %ebx
	jne	.L16
	leal	-16(%ebp), %esp
	popl	%ecx
	.cfi_remember_state
	.cfi_restore 1
	.cfi_def_cfa 1, 0
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%edi
	.cfi_restore 7
	popl	%ebp
	.cfi_restore 5
	leal	-4(%ecx), %esp
	.cfi_def_cfa 4, 4
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
