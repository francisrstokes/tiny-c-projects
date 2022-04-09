#include <stdio.h>
#include <stdint.h>

void printValue(uint32_t x) {
  printf("The value is: %d\n", x);
}

int main() {
  uint32_t a = 42;

  // Let's say we have a pointer
  uint32_t* pa = &a;

  // We can dereference it the normal way
  printValue(*pa);

  // But we can also reference it as if it were an array
  printValue(pa[0]);

  // In the case of the array method, it's basically syntactic sugar for this:
  printValue(*(pa + 0));

  // What you might not realise is that since this syntax is just an addition,
  // you can actually do this:
  printValue(0[pa]);

  // This leads to some *interesting* opportunities for confusing code
  printValue((~0)[pa-=~0]);

  return 0;
}
