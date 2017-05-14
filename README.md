# Example stand-alone MTX operating system development
## Under the guidance of Dr. KC Wang @ Washington State University
## Project is written and managed primarily in C and ASM in junction with QEMU
### This operating system project highlights:
* Fundamental operating system concepts
 * Mutex control
 * Semaphore control
* Disk booting
* BIOS development
 * Building our own I/O operations from BIOS putchar
* Kernel development
  * Process control
  * Kernel/User space register control
  * CPU tick-rate emulation for process control
  * User syscalls
* Basic operating system environment
  * File I/O
  * USB I/O
  * Keyboard I/O
  * Video I/O
  * Serial port I/O
    * Stream management
  * Thread control
* User management
  * User login
  * User shell
    * Grep
    * Cat
    * Login
    * More
    * Pipe
    * Link / Symlink
