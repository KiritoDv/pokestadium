#include <ultra64.h>
#include "global.h"
#include "rsp.h"

// All from n64 programming manual section 27.2
#define SRAM_START_ADDR 0x08000000
#define SRAM_SIZE 0x8000
#define SRAM_latency 0x5
#define SRAM_pulse 0x0c
#define SRAM_pageSize 0xd
#define SRAM_relDuration 0x2

OSThread gRspThread;
u8 unk_80081AB0[0x2000];
u8 gRspThreadStack[0x9C];
OSMesg D_80083B4C;
u8 unk_80083B50[0x3C];
OSMesg D_80083B8C;
u8 unk_80083B90[0x3C];
OSMesg D_80083BCC;
struct UnkStruct80083BD0 D_80083BD0;
s16 D_80083C1C;

s32 func_800005C0(void) {
    UNUSED s32 unused;
    UNUSED s32 unused2[6];
    s32 result;

    result = LeoCJCreateLeoManager(0x95, 0x96, &D_80083B4C, 0x10);
    if (result == 0) {
        if (osGetMemSize() >= 0x800000U) {
            D_80083C1C = 1;
        }
    }
    D_80083C1C = !result;
    return result;
}

OSPiHandle* func_80000628(void) {
    static OSPiHandle sramHandle;

    void* baseAddr = OS_PHYSICAL_TO_K1(SRAM_START_ADDR);
    if (baseAddr != (void*)(uintptr_t)sramHandle.baseAddress) {
        sramHandle.type = DEVICE_TYPE_SRAM;
        sramHandle.baseAddress = (uintptr_t)baseAddr;
        sramHandle.latency = SRAM_latency;
        sramHandle.pulse = SRAM_pulse;
        sramHandle.pageSize = SRAM_pageSize;
        sramHandle.relDuration = SRAM_relDuration;

        sramHandle.domain = PI_DOMAIN2;
        sramHandle.speed = 0;
        bzero(&sramHandle.transferInfo, sizeof(sramHandle.transferInfo));
        osEPiLinkHandle(&sramHandle);
    }
    return &sramHandle;
}

s32 func_800006C4(struct UnkStruct800006C4_2* arg0) {
    OSIoMesg msg;
    OSPiHandle* handle = func_80000628();

    msg.hdr.pri = 0;
    msg.hdr.retQueue = &D_80083BD0.queue2;
    msg.dramAddr = arg0->vaddr;
    msg.devAddr = arg0->unk1C;
    msg.size = arg0->size;

    osInvalDCache(arg0->vaddr, arg0->size);
    osEPiStartDma(handle, &msg, 0);
    osRecvMesg(&D_80083BD0.queue2, &D_80083BD0.unk0, 1);
    return 0;
}

s32 func_8000074C(struct UnkStruct800006C4_2* arg0) {
    OSIoMesg msg;
    OSPiHandle* handle = func_80000628();

    msg.hdr.pri = 0;
    msg.hdr.retQueue = &D_80083BD0.queue2;
    msg.dramAddr = arg0->vaddr;
    msg.devAddr = arg0->unk1C;
    msg.size = arg0->size;

    osWritebackDCache(arg0->vaddr, arg0->size);
    osEPiStartDma(handle, &msg, 1);
    osRecvMesg(&D_80083BD0.queue2, &D_80083BD0.unk0, OS_MESG_BLOCK);
    return 0;
}

s32 func_800007D4(struct UnkStruct800006C4_2* arg0, s32 arg1) {
    OSIoMesg msg;
    OSPiHandle* handle;

    if (arg1 == 0) {
        handle = osCartRomInit();
    } else {
        handle = osDriveRomInit();
    }

    msg.hdr.pri = 0;
    msg.hdr.retQueue = &D_80083BD0.queue2;
    msg.dramAddr = arg0->vaddr;
    msg.devAddr = arg0->unk1C;
    msg.size = arg0->size;

    osInvalDCache(arg0->vaddr, arg0->size);
    osEPiStartDma(handle, &msg, 0);
    osRecvMesg(&D_80083BD0.queue2, &D_80083BD0.unk0, OS_MESG_BLOCK);
    return 0;
}

s32 func_8000087C(struct UnkStruct800006C4_2* arg0) {
    OSIoMesg msg;
    OSPiHandle* handle;

    handle = osCartRomInit();

    msg.hdr.pri = 0;
    msg.hdr.retQueue = &D_80083BD0.queue2;
    msg.dramAddr = arg0->vaddr;
    msg.devAddr = arg0->unk1C;
    msg.size = arg0->size;

    osWritebackDCache(arg0->vaddr, arg0->size);
    osEPiStartDma(handle, &msg, 1);
    osRecvMesg(&D_80083BD0.queue2, &D_80083BD0.unk0, OS_MESG_BLOCK);
    return 0;
}

s32 func_80000904(struct UnkStruct800006C4_2* arg0) {
    UNUSED s32 arr[5];
    s32 sp20;

    osInvalDCache(arg0->vaddr, 0x80);
    func_8000D0B4((uintptr_t)&sp20, 0, arg0->unk1C, arg0->vaddr, 1, &D_80083BD0.queue2);
    osRecvMesg(&D_80083BD0.queue2, &D_80083BD0.unk0, OS_MESG_BLOCK);
    return 0;
}

s32 func_80000974(struct UnkStruct800006C4_2* arg0) {
    UNUSED s32 arr[5];
    s32 sp20_4;
    UNUSED s32 sp20_5;

    func_8000CEE4((uintptr_t)&sp20_4, 0, arg0->vaddr, &D_80083BD0.queue2);
    osRecvMesg(&D_80083BD0.queue2, &D_80083BD0.unk0, OS_MESG_BLOCK);
    return 0;
}

void* func_800009C8(void) {
    void* msg;

    osRecvMesg(&D_80083BD0.queue2, &msg, OS_MESG_BLOCK);
    return msg;
}

void* func_800009F8(struct UnkStruct800006C4_2* arg0) {
    LeoReadWrite(arg0, 0, arg0->unk1C, arg0->vaddr, arg0->size, &D_80083BD0.queue2);
    return func_800009C8();
}

void* func_80000A3C(struct UnkStruct800006C4_2* arg0) {
    LeoReadWrite(arg0, 1, arg0->unk1C, arg0->vaddr, arg0->size, &D_80083BD0.queue2);
    return func_800009C8();
}

void* func_80000A80(struct UnkStruct80000A80* arg0) {
    LeoReadDiskID(&arg0->cmd, arg0->addr, &D_80083BD0.queue2);
    return func_800009C8();
}

void* func_80000AB0(struct UnkStruct80000A80* arg0) {
    LeoSeek(&arg0->cmd, arg0->lba, &D_80083BD0.queue2);
    return func_800009C8();
}

void* func_80000AE0(struct UnkStruct80000A80* arg0) {
    LeoSpdlMotor(&arg0->cmd, arg0->mode, &D_80083BD0.queue2);
    return func_800009C8();
}

void* func_80000B10(struct UnkStruct80000A80* arg0) {
    void* temp_v0;

    LeoReadRTC(&arg0->cmd, &D_80083BD0.queue2);
    temp_v0 = func_800009C8();
    if (temp_v0 == NULL) {
        *(LEODiskTime*)arg0->addr = arg0->cmd.data.time;
    }
    return temp_v0;
}

void* func_80000B74(struct UnkStruct80000A80* arg0) {
    LeoSetRTC(&arg0->cmd, arg0->addr, &D_80083BD0.queue2);
    return func_800009C8();
}

void thread20_rsp(UNUSED void* arg) {
    struct UnkStruct800006C4_2* sp2C;
    OSMesg var_v0;

    func_800005C0();
    func_8000C8F8();
    while (1) {
        osRecvMesg(&D_80083BD0.queue1, (void*)&sp2C, OS_MESG_BLOCK);
        switch (sp2C->unk0) {
            case 0xF0:
                var_v0 = (OSMesg)INT2VOID(func_800007D4(sp2C, 0));
                break;
            case 0xF1:
                var_v0 = (OSMesg)INT2VOID(func_800007D4(sp2C, 1));
                break;
            case 0xF2:
                var_v0 = (OSMesg)INT2VOID(func_800006C4(sp2C));
                break;
            case 0xF3:
                var_v0 = (OSMesg)INT2VOID(func_8000074C(sp2C));
                break;
            case 0xF4:
                var_v0 = (OSMesg)INT2VOID(func_8000087C(sp2C));
                break;
            case 0xF5:
                var_v0 = (OSMesg)INT2VOID(func_80000904(sp2C));
                break;
            case 0xF6:
                var_v0 = (OSMesg)INT2VOID(func_80000974(sp2C));
                break;
            case 0x5:
                var_v0 = (OSMesg)func_800009F8(sp2C);
                break;
            case 0x6:
                var_v0 = (OSMesg)func_80000A3C(sp2C);
                break;
            case 0xC:
                /*
                 * TODO: These castings imply the 2 struct defs are the same struct, but
                 * there is very tenuous aliasing going on due to s16 and u8 overlap where
                 * there should be word loads. What is going on here?
                 */
                var_v0 = (OSMesg)func_80000A80((struct UnkStruct80000A80*)sp2C);
                break;
            case 0x7:
                var_v0 = (OSMesg)func_80000AB0((struct UnkStruct80000A80*)sp2C);
                break;
            case 0x8:
                var_v0 = (OSMesg)func_80000AE0((struct UnkStruct80000A80*)sp2C);
                break;
            case 0xD:
                var_v0 = (OSMesg)func_80000B10((struct UnkStruct80000A80*)sp2C);
                break;
            case 0xE:
                var_v0 = (OSMesg)func_80000B74((struct UnkStruct80000A80*)sp2C);
                break;
        }
        if ((OSMesgQueue*)INT2VOID(sp2C->unk28) != NULL) {
            osSendMesg(INT2VOID(sp2C->unk28), var_v0, 0);
        }
        Util_Free(sp2C);
    }
}

void rsp_init(void) {
    osCreateMesgQueue(&D_80083BD0.queue2, &D_80083BCC, 1);
    osCreateMesgQueue(&D_80083BD0.queue1, &D_80083B8C, 16);
    osCreatePiManager(0x96, (void*)&D_80083BD0.unk4, (OSMesg)&gRspThreadStack[0x1C], 0x20);
    osCreateThread(&gRspThread, 20, thread20_rsp, NULL, gRspThreadStack, 90);
    osStartThread(&gRspThread);
}

void func_80000DF4(void) {
    if (D_80083C1C != 0) {
        LeoReset();
        D_80083C1C = 0;
    }
}

void func_80000E2C(void* arg0, s32 arg1) {
    if (arg1 == 1) {
        osJamMesg(&D_80083BD0.queue1, arg0, OS_MESG_BLOCK);
        return;
    }
    osSendMesg(&D_80083BD0.queue1, arg0, OS_MESG_BLOCK);
}
