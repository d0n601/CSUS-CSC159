	.file	"sample.c"
	.version	"01.01"
.stabs "/gaia/class/student/kozakr/csc159/prep2/",100,0,0,.Ltext0
.stabs "sample.c",100,0,0,.Ltext0
.text
.Ltext0:
	.stabs	"gcc2_compiled.", 0x3c, 0, 0, 0
.stabs "int:t1=r1;-2147483648;2147483647;",128,0,0,0
.stabs "char:t2=r2;0;127;",128,0,0,0
.stabs "long int:t3=r1;-2147483648;2147483647;",128,0,0,0
.stabs "unsigned int:t4=r1;0;-1;",128,0,0,0
.stabs "long unsigned int:t5=r1;0;-1;",128,0,0,0
.stabs "long long int:t6=r1;01000000000000000000000;0777777777777777777777;",128,0,0,0
.stabs "long long unsigned int:t7=r1;0000000000000;01777777777777777777777;",128,0,0,0
.stabs "short int:t8=r1;-32768;32767;",128,0,0,0
.stabs "short unsigned int:t9=r1;0;65535;",128,0,0,0
.stabs "signed char:t10=r1;-128;127;",128,0,0,0
.stabs "unsigned char:t11=r1;0;255;",128,0,0,0
.stabs "float:t12=r1;4;0;",128,0,0,0
.stabs "double:t13=r1;8;0;",128,0,0,0
.stabs "long double:t14=r1;12;0;",128,0,0,0
.stabs "complex int:t15=s8real:1,0,32;imag:1,32,32;;",128,0,0,0
.stabs "complex float:t16=r16;4;0;",128,0,0,0
.stabs "complex double:t17=r17;8;0;",128,0,0,0
.stabs "complex long double:t18=r18;12;0;",128,0,0,0
.stabs "void:t19=19",128,0,0,0
.stabs "__gnuc_va_list:t20=*19",128,0,23,0
.stabs "va_list:t20",128,0,112,0
.stabs "int8:t2",128,0,26,0
.stabs "int16:t8",128,0,27,0
.stabs "int32:t1",128,0,28,0
.stabs "int64:t6",128,0,29,0
.stabs "uint8:t11",128,0,31,0
.stabs "uint16:t9",128,0,32,0
.stabs "uint32:t4",128,0,33,0
.stabs "uint64:t7",128,0,34,0
.stabs "pos_t:t5",128,0,37,0
.stabs "REGISTER:t4",128,0,38,0
.stabs "ptrdiff_t:t3",128,0,43,0
.stabs "BOOLEAN:t11",128,0,46,0
.stabs "BOOL:t4",128,0,47,0
.stabs "BYTE:t11",128,0,50,0
.stabs "WORD:t9",128,0,51,0
.stabs "DWORD:t4",128,0,52,0
.stabs "ADDRESS:t4",128,0,54,0
.stabs "caddr_t:t21=*2",128,0,56,0
.stabs "off_t:t3",128,0,57,0
.stabs "TEXT:t11",128,0,59,0
.stabs "STRPTR:t22=*11",128,0,60,0
.stabs "uchar:t11",128,0,62,0
.stabs "ushort:t9",128,0,63,0
.stabs "uint:t4",128,0,64,0
.stabs "ulong:t5",128,0,65,0
.stabs "u_char:t11",128,0,75,0
.stabs "u_short:t9",128,0,76,0
.stabs "u_int:t4",128,0,77,0
.stabs "u_int32_t:t4",128,0,78,0
.stabs "u_int16_t:t4",128,0,79,0
.stabs "int32_t:t1",128,0,81,0
.stabs "uint8_t:t11",128,0,83,0
.stabs "uint16_t:t9",128,0,84,0
.stabs "uint32_t:t4",128,0,85,0
.stabs "n_short:t9",128,0,87,0
.stabs "n_time:t4",128,0,88,0
.stabs "n_long:t4",128,0,89,0
.stabs "splx_t:t23=ar1;0;0;4",128,0,36,0
.stabs "LONGWORD:t4",128,0,40,0
.stabs "DebugSupport:T24=e_DEBUG_BEFORE:-1,_DEBUG_NOPE:0,\\",128,0,0,0
.stabs "_DEBUG_USING_GDB:1,;",128,0,0,0
.section	.rodata
.LC0:
	.string	"%d Hello world %d \nECS"
.LC1:
	.string	"--> Hello world <--\nCPE/CSC"
.text
	.align 4
.stabs "main:F1",36,0,6,main
.globl main
	.type	 main,@function
main:
.stabn 68,0,6,.LM1-main
.LM1:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
.stabn 68,0,7,.LM2-main
.LM2:
.LBB2:
.stabn 68,0,8,.LM3-main
.LM3:
	movl $128,-4(%ebp)
.stabn 68,0,9,.LM4-main
.LM4:
	movl -4(%ebp),%edx
	movl %edx,%eax
	addl %eax,%edx
	pushl %edx
	movl -4(%ebp),%eax
	pushl %eax
	pushl $.LC0
	call printf
	addl $12,%esp
.stabn 68,0,10,.LM5-main
.LM5:
	pushl $.LC1
	call cons_printf
	addl $4,%esp
.stabn 68,0,11,.LM6-main
.LM6:
	xorl %eax,%eax
	jmp .L1
	.align 4
.stabn 68,0,12,.LM7-main
.LM7:
.LBE2:
.stabn 68,0,12,.LM8-main
.LM8:
.L1:
	leave
	ret
.Lfe1:
	.size	 main,.Lfe1-main
.stabs "i:3",128,0,7,-4
.stabn 192,0,0,.LBB2-main
.stabn 224,0,0,.LBE2-main
	.text
	.stabs "",100,0,0,.Letext
.Letext:
	.ident	"GCC: (GNU) 2.7.2.3"
