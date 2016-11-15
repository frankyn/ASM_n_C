#include <stdio.h>
#include <assert.h>

int test_func(int num) {
  for (int i = 0; i < 10; ++i) {
    num += 2;
  }

  return num;
}

int test_asm_func(int num) {
  int ret = 0;
  
  asm ("pushq %%rbp\n"
       "movq  %%rsp, %%rbp\n"
       "movl  %1, -4(%%rbp)\n"
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
       "mov   %%eax, %0\n"
       "popq  %%rbp\n"
       : "=r" (ret)
       : "r"  (num)
       : "eax"
      );
  return ret;
}

int main(int argc, char** argv) {
  puts("hello world");
  
  int ten = test_asm_func(10);
  printf("test_asm_func printed: %d\n", ten);  
    
  assert(test_asm_func(10) == test_func(10));
  return 0;
}

