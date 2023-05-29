#include "error_message_reset.h"
#include "GX_layers.h"
#include "gx.h"
#include "unk_02031734.h"
#include "unk_0202F150.h"
#include "bg_window.h"
#include "PAD_pad.h"
#include "font.h"
#include "brightness.h"
#include "render_window.h"
#include "msgdata/msg.naix"


static const struct WindowTemplate sErrorMessageWindowTemplate = {
    .bgId = GF_BG_LYR_MAIN_0,
    .tilemapLeft = 3,
    .tilemapTop = 3,
    .width = 26,
    .height = 18,
    .paletteNum = 1,
    .baseTile = 0x23
};

static const struct HeapParam sErrorMessageHeapParams = {
    .size = 0x00020000,
    .arena = OS_ARENA_MAIN
};

static const struct GraphicsModes sErrorMessageBgModeSet = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_2D
};

static const struct BgTemplate sErrorMessageBgTemplate = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0x0000,
    .charBase = GX_BG_CHARBASE_0x18000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 1,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE
};

static const struct GraphicsBanks sErrorMessageBanksConfig = {
    .bg = GX_VRAM_BG_256_AB,
    .bgextpltt = GX_VRAM_BGEXTPLTT_NONE,
    .subbg = GX_VRAM_SUB_BG_NONE,
    .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_NONE,
    .obj = GX_VRAM_OBJ_NONE,
    .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
    .subobj = GX_VRAM_SUB_OBJ_NONE,
    .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
    .tex = GX_VRAM_TEX_NONE,
    .texpltt = GX_VRAM_TEXPLTT_NONE,
};

static u32 sErrorMessagePrinterLock;

extern void FUN_0200E3A0(PMLCDTarget, int);
extern void SetMasterBrightnessNeutral(u32 screen);

THUMB_FUNC void VBlankHandler()
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
    MI_WaitDma(GX_DEFAULT_DMAID);
}

THUMB_FUNC void PrintErrorMessageAndReset()
{

    struct BgConfig *ptr;
    struct Window buf;

    if (sErrorMessagePrinterLock != TRUE)
    {
        sErrorMessagePrinterLock = TRUE;
        OS_SetArenaHi(OS_ARENA_MAIN, OS_GetInitArenaHi(OS_ARENA_MAIN));
        OS_SetArenaLo(OS_ARENA_MAIN, OS_GetInitArenaLo(OS_ARENA_MAIN));

        InitHeapSystem(&sErrorMessageHeapParams, 1, 1, 0);
        FUN_0200E3A0(PM_LCD_TOP, 0);
        FUN_0200E3A0(PM_LCD_BOTTOM, 0);

        OS_DisableIrqMask(OS_IE_V_BLANK);
        OS_SetIrqFunction(OS_IE_V_BLANK, &VBlankHandler);
        OS_EnableIrqMask(OS_IE_V_BLANK);

        Main_SetVBlankIntrCB(NULL, NULL);
        Main_SetHBlankIntrCB(NULL, NULL);

        GX_DisableEngineALayers();
        GX_DisableEngineBLayers();

        GX_SetVisiblePlane(0);
        GXS_SetVisiblePlane(0);

        SetKeyRepeatTimers(4, 8);

        gMain.screensFlipped = FALSE;
        GX_SwapDisplay();

        G2_BlendNone();
        G2S_BlendNone();

        GX_SetVisibleWnd(0);
        GXS_SetVisibleWnd(0);

        GX_SetBanks(&sErrorMessageBanksConfig);
        ptr = BgConfig_Alloc(0);
        SetBothScreensModesAndDisable(&sErrorMessageBgModeSet);

        InitBgFromTemplate(ptr, 0, &sErrorMessageBgTemplate, GX_BGMODE_0);
        BgClearTilemapBufferAndCommit(ptr, GF_BG_LYR_MAIN_0);

        LoadUserFrameGfx1(ptr, GF_BG_LYR_MAIN_0, 503, 2, 0, 0);

        LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_OFFSET_1, 0);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, 0);
        BG_SetMaskColor(GF_BG_LYR_MAIN_0, GX_RGB(1, 1, 27));
        BG_SetMaskColor(GF_BG_LYR_SUB_0, GX_RGB(1, 1, 27));

        struct MsgData *msg_data = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_MSGDATA_MSG, NARC_msg_narc_0200_bin, 0);
        struct String *str = String_ctor(6 << 6, 0);

        ResetAllTextPrinters();
        AddWindow(ptr, &buf, &sErrorMessageWindowTemplate);
        FillWindowPixelRect(&buf, 15, 0, 0, 0xd0, 0x90);
        DrawFrameAndWindow1(&buf, FALSE, 0x1f7, 2);

        ReadMsgDataIntoString(msg_data, 3, str); //todo: msgenc needs to be updated to use the constant

        AddTextPrinterParameterized(&buf, 0, str, 0, 0, 0, NULL);

        String_dtor(str);
        GX_BothDispOn();
        SetMasterBrightnessNeutral(PM_LCD_TOP);
        SetMasterBrightnessNeutral(PM_LCD_BOTTOM);
        SetBlendBrightness(0, 0x3f, 3);
        FUN_02032DAC();

        while (TRUE)
        {
            HandleDSLidAction();
            FUN_0202FB80();
            if (FUN_02033678())
                break;
            OS_WaitIrq(TRUE, OS_IE_V_BLANK);
        }

        while (TRUE)
        {
            HandleDSLidAction();
            if ((PAD_Read() & PAD_BUTTON_A))
                break;
            OS_WaitIrq(TRUE, OS_IE_V_BLANK);
        }
        FUN_0200E3A0(PM_LCD_TOP, 0x7FFF);
        FUN_0200E3A0(PM_LCD_BOTTOM, 0x7FFF);

        RemoveWindow(&buf);

        DestroyMsgData(msg_data);
        FreeToHeap(ptr);

        OS_ResetSystem(0);
    }
}
