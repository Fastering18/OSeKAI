#ifndef H_PIT
#define H_PIT 1

void pit_handler(struct registers_t *regs);
void pit_init();

void pit_wait(uint16_t s);

#endif