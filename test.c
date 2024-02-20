#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  while (true) {
    printf("Hello world\n");
    sleep(1);
  }
  return 0;
}
