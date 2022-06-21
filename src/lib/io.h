#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H 

#include <stdbool.h>

/** farpeek1:
 * Reads a byte from a far pointer.
 * @return: The byte read.
*/
extern inline uint32_t farpeekl(uint16_t sel, void* off);


extern inline void farpokeb(uint16_t sel, void* off, uint8_t v);

extern inline bool are_interrupts_enabled();

extern inline unsigned long save_irqdisable(void);

extern inline void irqrestore(unsigned long flags);

extern void intended_usage(void);

/** outb:
 *  Sends the give data to the given I/O port. Defined in io.s
 *
 *  @param port The I/O port to send the data to
 *  @param data The data to send to the I/O port
 */
extern inline void outb(uint16_t port, uint8_t val);

/** inb:
 *  Read a byte from an I/O port.
 *
 *  @param  port The address of the I/O port
 *  @return      The read byte
 */
extern inline uint8_t inb(uint16_t port);

/** io_wait:
 *  Wait for the I/O port to be ready.
 *  Defined in io.c
*/
extern inline void io_wait(void);

#endif /* INCLUDE_IO_H */