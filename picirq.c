#include "types.h"
#include "x86.h"
#include "traps.h"

// I/O Addresses of the two programmable interrupt controllers
#define IO_PIC1         0x20    // Master (IRQs 0-7)
#define IO_PIC2         0xA0    // Slave (IRQs 8-15)

// Don't use the 8259A interrupt controllers.  Xv6 assumes SMP hardware.
// MEMO: 
// 8259A: https://ja.wikipedia.org/wiki/Intel_8259
//  - メジャーなint controller. 各周辺機器から飛んでくるintを捌いて、cpuに通知(マルチプレクサ的な動きをする.)
// SMP: https://ja.wikipedia.org/wiki/%E5%AF%BE%E7%A7%B0%E5%9E%8B%E3%83%9E%E3%83%AB%E3%83%81%E3%83%97%E3%83%AD%E3%82%BB%E3%83%83%E3%82%B7%E3%83%B3%E3%82%B0
//  - 複数のcpuがマスタスレーブの関係にならないようなpc.理想的には、割り込みなどが(特定のcpuに偏ることなく)どのcpuにも満遍なく通知される
void
picinit(void)
{
  // mask all interrupts
  // MEMO: (addr, data)
  // 復習: out instruction https://c9x.me/x86/html/file_module_x86_id_222.html
  outb(IO_PIC1+1, 0xFF);
  outb(IO_PIC2+1, 0xFF);
}

//PAGEBREAK!
// Blank page.
