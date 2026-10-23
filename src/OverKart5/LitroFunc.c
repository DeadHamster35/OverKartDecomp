#include "MainInclude.h"
#include "OKInclude.h"

/*
 * Do not include menu_items.h from MainInclude TUs: ObjBlock, MenuItems,
 * TBSeq_psel_*, and several menu funcs are already typed in Struct.h /
 * GameOffsets.h. SEQDataBlock layout matches struct_8018DEE0_entry.
 */
typedef struct {
	void *TextureSequence;
	s32 SequenceIndex;
	s32 FrameCountDown;
	u32 Visible;
	s32 MenuTextureIndex;
	s32 unk14;
} SeqDataBlock;

extern SeqDataBlock SEQDataBlock[];

int LoopCount = 1;

void LoadArrows(void)
{
	*SourceAddress = (int)(&ArrowsSpriteROM);
	*TargetAddress = (int)(&ok_FreeSpace);
	DataLength = 0x200;
	RunDMA();
	*SourceAddress = (int)(&ok_FreeSpace);
	*TargetAddress = (int)(&lit_arrowsSprite);
	RunMIO();
}

void LoadCoinSprite(void)
{
	*SourceAddress = (int)(&RCSpriteROM);
	*TargetAddress = (int)(&ok_FreeSpace);
	DataLength = 0x100;
	RunDMA();
	*SourceAddress = (int)(&ok_FreeSpace);
	*TargetAddress = (int)(&ok_menucoinsprite);
	RunMIO();
}

void LoadNumberSprites(void)
{
	*SourceAddress = (int)(&NumbersSpriteROM);
	*TargetAddress = (int)(&ok_FreeSpace);
	DataLength = 0x470;
	RunDMA();
	*SourceAddress = (int)(&ok_FreeSpace);
	*TargetAddress = (int)(&lit_numberSprite);
	RunMIO();
}

void SetAlwaysAdvance(void)
{
	/* Qualify lives in CheckKartRap: currentPosition < 4 || GPMode == 1. */
}

void ShowMusicNoteAnim(int playerIndex)
{
	SetAnimMusicNote((char)playerIndex);
}

void AddTopSpeed(int SpeedGain)
{
	ChangeMaxSpeed(0, (float)SpeedGain);
}

void ResetMenuUpdateTimers(void)
{
	SEQDataBlock[0].FrameCountDown = 0;
	SEQDataBlock[1].FrameCountDown = 0;
	SEQDataBlock[2].FrameCountDown = 0;
	SEQDataBlock[3].FrameCountDown = 0;
}

void SetZoomLevel(int Zoom)
{
	KoteiGakakuMode[0] = Zoom;
	KoteiGakakuMode[1] = Zoom;
	KoteiGakakuMode[2] = Zoom;
	KoteiGakakuMode[3] = Zoom;
}
