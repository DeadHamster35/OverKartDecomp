#include "MainInclude.h"
#include "OKInclude.h"

/*
 * Do not include menu_items.h from MainInclude TUs: ObjBlock, gMenuItems,
 * TBSeq_psel_*, and several menu funcs are already typed in Struct.h /
 * GameOffsets.h. seq_data_block layout matches struct_8018DEE0_entry.
 */
typedef struct {
	void *textureSequence;
	s32 sequenceIndex;
	s32 frameCountDown;
	u32 visible;
	s32 menuTextureIndex;
	s32 unk14;
} SeqDataBlock;

extern SeqDataBlock seq_data_block[];

int loopCount = 1;

void loadArrows(void)
{
	*sourceAddress = (int)(&ArrowsSpriteROM);
	*targetAddress = (int)(&ok_FreeSpace);
	dataLength = 0x200;
	runDMA();
	*sourceAddress = (int)(&ok_FreeSpace);
	*targetAddress = (int)(&lit_arrowsSprite);
	runMIO();
}

void loadCoinSprite(void)
{
	*sourceAddress = (int)(&RCSpriteROM);
	*targetAddress = (int)(&ok_FreeSpace);
	dataLength = 0x100;
	runDMA();
	*sourceAddress = (int)(&ok_FreeSpace);
	*targetAddress = (int)(&ok_menucoinsprite);
	runMIO();
}

void loadNumberSprites(void)
{
	*sourceAddress = (int)(&NumbersSpriteROM);
	*targetAddress = (int)(&ok_FreeSpace);
	dataLength = 0x470;
	runDMA();
	*sourceAddress = (int)(&ok_FreeSpace);
	*targetAddress = (int)(&lit_numberSprite);
	runMIO();
}

void setAlwaysAdvance(void)
{
	/* Qualify lives in check_kart_rap: currentPosition < 4 || GPMode == 1. */
}

void ShowMusicNoteAnim(int playerIndex)
{
	SetAnimMusicNote((char)playerIndex);
}

void AddTopSpeed(int SpeedGain)
{
	ChangeMaxSpeed(0, (float)SpeedGain);
}

void resetMenuUpdateTimers(void)
{
	seq_data_block[0].frameCountDown = 0;
	seq_data_block[1].frameCountDown = 0;
	seq_data_block[2].frameCountDown = 0;
	seq_data_block[3].frameCountDown = 0;
}

void setZoomLevel(int Zoom)
{
	g_zoomLevelPlayer1 = Zoom;
	g_zoomLevelPlayer2 = Zoom;
	g_zoomLevelPlayer3 = Zoom;
	g_zoomLevelPlayer4 = Zoom;
}
