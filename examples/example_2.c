#include <stdio.h>

int one_func() {
  // always returns a one
  return 1;
}

int call_another_func() {
  int one = one_func();
  
  return one;
}

int call_another_asm_func() {
  int one = 0;

  asm ("pushq %%rbp\n"
       "movq  %%rsp, %%rbp\n"
       "subq  $16, %%rsp\n"
       "callq _one_func\n"
       "movl  %%eax,  -4(%%rbp)\n"
       "movl  -4(%%rbp), %%eax\n"
       "addq  $16, %%rsp\n"
       "movl  %%eax, %0\n"
       "popq  %%rbp\n"
       : "=r" (one)
       : 
       : "eax"
      );

  return one;
}

int main(int argc, char** argv) {
  int value = call_another_asm_func();
  printf("Returned: %d\n", value);

  return 0;
}
