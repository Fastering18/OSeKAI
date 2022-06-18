#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define GDT_NULL = 0x00
#define GDT_CODE_16 = 0x08
#define GDT_DATA_16 = 0x10
#define GDT_CODE_32 = 0x18
#define GDT_DATA_32 = 0x20
#define GDT_CODE_64 = 0x28
#define GDT_DATA_64 = 0x30
#define GDT_USER_CODE_64 = 0x38
#define GDT_USER_DATA_64 = 0x40
#define GDT_TSS = 0x48

struct GDTDescriptor
{
    uint16_t Size;
    uint64_t Offset;
} __attribute__((packed));

struct GDTEntry
{
    uint16_t Limit0;
    uint16_t Base0;
    uint8_t Base1;
    uint8_t Access;
    uint8_t Granularity;
    uint8_t Base2;
} __attribute__((packed));

struct TSSEntry
{
    uint16_t Length;
    uint16_t Base0;
    uint8_t  Base1;
    uint8_t  Flags1;
    uint8_t  Flags2;
    uint8_t  Base2;
    uint32_t Base3;
    uint32_t Reserved;
} __attribute__((packed));

typedef struct
{
    struct GDTEntry Null;
    struct GDTEntry _16BitCode;
    struct GDTEntry _16BitData;
    struct GDTEntry _32BitCode;
    struct GDTEntry _32BitData;
    struct GDTEntry _64BitCode;
    struct GDTEntry _64BitData;
    struct GDTEntry UserCode;
    struct GDTEntry UserData;
    struct TSSEntry Tss;
} __attribute__((packed, aligned(0x1000))) GDT;

typedef struct
{
    uint32_t Reserved0;
    uint64_t RSP[3];
    uint64_t Reserved1;
    uint64_t IST[7];
    uint64_t Reserved2;
    uint16_t Reserved3;
    uint16_t IOPBOffset;
} __attribute__((packed)) TSS;


extern GDT DefaultGDT;
extern TSS *tss;
extern bool initialised;

void LoadGDT(struct GDTDescriptor *gdtDescriptor);
void LoadTSS();

void reloadall(size_t cpu);
void reloadgdt();
void reloadtss();
void init();