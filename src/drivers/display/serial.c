

// serial print
void serial_print(char *str)
{
    while (*str)
    {
        while (!(inb(0x3F8 + 5) & 0x20));
        outb(0x3F8, *str);
        str++;
    }
}