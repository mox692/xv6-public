// Intel 8250 serial port (UART).

#include "types.h"
#include "defs.h"
#include "param.h"
#include "traps.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"

// spec: https://www.ti.com/lit/ug/sprugp1/sprugp1.pdf?ts=1637682393247&ref_url=https%253A%252F%252Fwww.google.com%252F#:~:text=The%20UART%20includes%20a%20programmable,internal%20transmitter%20and%20receiver%20logic.
// Ref: https://cstmize.hatenablog.jp/entry/2019/06/15/keyboard%2C_UART%2C_VGA%E3%81%AE%E5%88%9D%E6%9C%9F%E5%8C%96%28xv6%E3%82%92%E4%BE%8B%E3%81%AB%29
//    : https://en.wikipedia.org/wiki/COM_(hardware_interface)#:~:text=I/O-,addresses,-%5Bedit%5D
//    :http://209.68.14.80/ref/mbsys/res/irq/numIRQ4-c.html によると、serial = IRQ4 ?
// MEMO: COM (communication port)[1][2] is the original,
// yet still common, name of the serial port interface on PC-compatible computers.
// MEMO: serial通信はIO port経由で行われてるみたい？(https://wiki.osdev.org/I/O_Ports)
//       なので、この0x3f8はメモリとは全く別の空間のaddrを指す.

#define COM1    0x3f8

static int uart;    // is there a uart?

void
uartinit(void)
{
  char *p;

  // TODO: ここの +xx って値が何を示しているのか.
  // MEMO: intの文脈でのfifo https://www.tij.co.jp/jp/lit/ds/symlink/tl16c750e.pdf?ts=1637683228963&ref_url=https%253A%252F%252Fwww.google.com%252F
  // Turn off the FIFO
  // MEMO: https://wiki.osdev.org/Serial_Ports を読むと、理解できそう
  // MEMO: 模範回答的なのは https://cstmize.hatenablog.jp/entry/2019/06/15/keyboard,_UART,_VGA%E3%81%AE%E5%88%9D%E6%9C%9F%E5%8C%96(xv6%E3%82%92%E4%BE%8B%E3%81%AB)
  // Ref: https://ja.wikipedia.org/wiki/16550_UART

  outb(COM1+2, 0);

  // 9600 baud, 8 data bits, 1 stop bit, parity off.
  outb(COM1+3, 0x80);    // Unlock divisor
  outb(COM1+0, 115200/9600);
  outb(COM1+1, 0);
  outb(COM1+3, 0x03);    // Lock divisor, 8 data bits.
  outb(COM1+4, 0);
  outb(COM1+1, 0x01);    // Enable receive interrupts.

  // If status is 0xFF, no serial port.
  if(inb(COM1+5) == 0xFF)
    return;
  uart = 1;

  // Acknowledge pre-existing interrupt conditions;
  // enable interrupts.
  inb(COM1+2);
  inb(COM1+0);
  ioapicenable(IRQ_COM1, 0);

  // Announce that we're here.
  for(p="xv6...\n"; *p; p++)
    uartputc(*p);
}

void
uartputc(int c)
{
  int i;

  if(!uart)
    return;
  for(i = 0; i < 128 && !(inb(COM1+5) & 0x20); i++)
    microdelay(10);
  outb(COM1+0, c);
}

static int
uartgetc(void)
{
  if(!uart)
    return -1;
  if(!(inb(COM1+5) & 0x01))
    return -1;
  return inb(COM1+0);
}

void
uartintr(void)
{
  consoleintr(uartgetc);
}
