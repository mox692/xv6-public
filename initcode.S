# Initial process execs /init.
# This code runs in user space.

#include "syscall.h"
#include "traps.h"


# exec(init, argv)
.globl start
start:
  pushl $argv
  pushl $init
  pushl $0  // where caller pc would be
  # ここは、引数を渡している？
  movl $SYS_exec, %eax
  # trap.h, trap.c(idtのsyscallのエントリをセットしてる.) を参照.
  # ここでソフトウェア割り込みを発生させ、T_SYSCALL(64)で定義されている割り込みを発生させる
  int $T_SYSCALL

# for(;;) exit();
exit:
  movl $SYS_exit, %eax
  int $T_SYSCALL
  jmp exit

# char init[] = "/init\0";
init:
  .string "/init\0"

# char *argv[] = { init, 0 };
.p2align 2
argv:
  .long init
  .long 0

