	.arch armv7-a
	.eabi_attribute 28, 1
	.fpu vfpv3-d16
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 2
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"Lab4.c"
	.section	.text.startup,"ax",%progbits
	.align	2
	.global	main
	.syntax unified
	.thumb
	.thumb_func
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 480008
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	movw	r6, #:lower16:__stack_chk_guard
	sub	sp, sp, #479232
	movt	r6, #:upper16:__stack_chk_guard
	sub	sp, sp, #780
	movs	r0, #1		@для srand(1)
	ldr	r3, [r6]
	add	r2, sp, #479232
	add	r2, r2, #772
	mov	r4, sp	@r4=sp
	str	r3, [r2]
	bl	srand
	add	r3, sp, #4		@r3=sp+4
	add	r5, r3, #479232		
	add	r5, r5, #764	@r5=sp+4+479996 (sp+119999+1)
.L2:
	bl	rand
	str	r0, [r4, #4]!	@неявная адресная инкрементация записываем r0 по адресу r4+4
	cmp	r5, r4
	bne	.L2		@пока не равны
	bl	clock
	add	r5, sp, #479232
	add	r4, sp, #4	@r4=sp+4
	add	r5, r5, #772	@r5=sp+480004
	mov	r7, r0	@r7=start
.L3:
	mov	r3, r4	@r3=r4=sp+4 (j=i)
.L5:
	ldr	r0, [r3]	@r0=massive[j]
	adds	r3, r3, #4	@j++
	ldr	r1, [r4]	@r1=massive[i]
	cmp	r1, r0	@сравниваем massive[i] massive[j]
	itt	lt	@сравнение < (signed) (применяется на следующие 2 инструкции)
	strlt	r0, [r4]
	strlt	r1, [r3, #-4]
	cmp	r3, r5 @условия внутреннего
	bne	.L5
	adds	r4, r4, #4 @i++
	cmp	r4, r3	@условия внешнего цикла
	bne	.L3
	bl	clock
	movw	r1, #:lower16:.LC0
	movt	r1, #:upper16:.LC0
	subs	r2, r0, r7	@finish - start
	movs	r0, #1
	bl	__printf_chk
	ldr	r2, [sp, #4]
	movw	r1, #:lower16:.LC1
	movs	r0, #1
	movt	r1, #:upper16:.LC1
	bl	__printf_chk
	add	r3, sp, #479232
	add	r3, r3, #772
	movs	r0, #0
	ldr	r2, [r3]
	ldr	r3, [r6]
	cmp	r2, r3
	bne	.L12
	add	sp, sp, #479232
	add	sp, sp, #780
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L12:
	bl	__stack_chk_fail
	.size	main, .-main
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"Time = %d\012\000"
	.space	1
.LC1:
	.ascii	"Frist element in massive: %d\000"
	.ident	"GCC: (Ubuntu/Linaro 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609"
	.section	.note.GNU-stack,"",%progbits
