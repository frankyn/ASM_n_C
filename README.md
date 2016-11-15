# ASM and C
How to reverse-engineer using `objdump`.

## Example 1
In this example, a disassembled function is reused using inline assembly in C. The function is test_func and it has a for loop which adds `20` to an input value `num`.

1. Compile and disassemble

```
gcc -o example_1 example_1.c
objdump -D example_1 > example_1.dump
```

2. In `example_1.dump` you will find something similar to the following snippet

```
_test_func:
100000e90:	55 	pushq	%rbp
100000e91:	48 89 e5 	movq	%rsp, %rbp
100000e94:	89 7d fc 	movl	%edi, -4(%rbp)
100000e97:	c7 45 f8 00 00 00 00 	movl	$0, -8(%rbp)
100000e9e:	83 7d f8 0a 	cmpl	$10, -8(%rbp)
100000ea2:	0f 8d 17 00 00 00 	jge	23 <_test_func+2F>
100000ea8:	8b 45 fc 	movl	-4(%rbp), %eax
100000eab:	83 c0 02 	addl	$2, %eax
100000eae:	89 45 fc 	movl	%eax, -4(%rbp)
100000eb1:	8b 45 f8 	movl	-8(%rbp), %eax
100000eb4:	83 c0 01 	addl	$1, %eax
100000eb7:	89 45 f8 	movl	%eax, -8(%rbp)
100000eba:	e9 df ff ff ff 	jmp	-33 <_test_func+E>
100000ebf:	8b 45 fc 	movl	-4(%rbp), %eax
100000ec2:	5d 	popq	%rbp
100000ec3:	c3 	retq
100000ec4:	66 66 66 2e 0f 1f 84 00 00 00 00 00 	nopw	%cs:(%rax,%rax)
``` 

This snippet is assembly in [AT&T](https://en.wikibooks.org/wiki/X86_Assembly/GAS_Syntax) syntax.

3. Extract the assembly syntax only for this function  

```
pushq %rbp
movq  %rsp, %rbp
movl  %edi, -4(%rbp)
movl  $0, -8(%rbp)
cmpl  $10, -8(%%rbp)
jge   23 
movl  -4(%rbp), %eax
addl  $2, %eax
movl  %eax, -4(%rbp)
movl  -8(%rbp), %eax
addl  $1, %eax
movl  %eax, -8(%rbp)
jmp   -33
movl  -4(%%rbp), %%eax
popq  %rbp
```

4. Wrap asm for use in C and use Labels instead of magic numbers for jump statements
```
int test_asm_func(int num) {
  int ret = 0;

  asm ("pushq %%rbp\n"
       "movq  %%rsp, %%rbp\n"
       "movl  %1, -4(%%rbp)\n" // move num to -4(%%rbp)
       "movl  $0, -8(%%rbp)\n"
       "label2:\n"
       "cmpl  $10, -8(%%rbp)\n"
       "jge   label1\n"
       "movl  -4(%%rbp), %%eax\n"
       "addl  $2, %%eax\n"
       "movl  %%eax, -4(%%rbp)\n"
       "movl  -8(%%rbp), %%eax\n"
       "addl  $1, %%eax\n"
       "movl  %%eax, -8(%%rbp)\n"
       "jmp   label2\n"
       "label1:\n" 
       "movl  -4(%%rbp), %%eax\n"
       "mov   %%eax, %0\n" // move result in %%eax to ret
       "popq  %%rbp\n"
       : "=r" (ret) // output variable
       : "r"  (num) // input  variable
       : "eax"
      );

  return ret;
}
```







