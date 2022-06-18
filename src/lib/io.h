#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H 1

/** outb:
 *  Sends the give data to the given I/O port. Defined in io.s
 *
 *  @param port The I/O port to send the data to
 *  @param data The data to send to the I/O port
 */
static inline void outb(uint16_t port, uint8_t val);

/** inb:
 *  Read a byte from an I/O port.
 *
 *  @param  port The address of the I/O port
 *  @return      The read byte
 */
static inline uint8_t inb(uint16_t port);

static inline void io_wait(void);

#endif /* INCLUDE_IO_H */