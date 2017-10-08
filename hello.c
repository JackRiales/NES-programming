/*
	Basic NES Programming in CC65
	Jack Riales

	I hope to some day compile and run this
	on actual hardware. That would be 
	P. cool.
*/

/* Headers */
#define TV_NTSC 1
#include <stddef.h>
#include "nesdev.h"
#include "reset.h"

/* Zero-page Globals (commonly used vars) */
#pragma bss-name(push, "ZEROPAGE")
u8 i;           /* Iterator */
u8 j;           /* Iterator */
u8 attr_offset; /* Offset into our attributes */

/* PPU write data */
uptr ppu_addr;      /* PPU destination addr */
const u8* ppu_data; /* Pointer to data to copy over */
u8 ppu_data_size;   /* Size in bytes of data */
#pragma bss-name(pop)

#pragma bss-name(push, "OAM")
sprite_t player;
#pragma bss-name(pop)

/* Data inclusions */
#include "gamedata.c"

void ppu_scroll_reset (void)
{
	PPU_SCROLL = 0x00;
	PPU_SCROLL = 0x00;
}

void ppu_write()
{
	PPU_ADDRESS = (u8) (ppu_addr >> 8);
	PPU_ADDRESS = (u8) (ppu_addr);
	for (i = 0; i < ppu_data_size; ++i)
		PPU_DATA = ppu_data[i];
}

void ppu_enable()
{
	PPU_CTRL = PPUCTRL_NAMETABLE_0 | /* Use Nametable 0 */
	           PPUCTRL_BPATTERN_0  | /* Let the bg use pattern table 0 */
	           PPUCTRL_NMI_ON      ; /* Enable NMIs */

	PPU_MASK = PPUMASK_COLOR    | /* Show colors */
	           PPUMASK_BSHOW    | /* Show background as well */
			   PPUMASK_L8_BSHOW | /* Show bg tiles in leftmost 8px */
			   PPUMASK_SSHOW    | /* Show sprites */
			   PPUMASK_L8_SSHOW ; /* Show sprites in leftmost 8px */
}

void draw_background (void)
{
	PPU_ADDRESS = (u8) ((PPU_NAMETABLE_0 + PPU_NAMETABLE_OFFSET) >> 8);
	PPU_ADDRESS = (u8) (PPU_NAMETABLE_0 + PPU_NAMETABLE_OFFSET);

	/* Draw top border */
	PPU_DATA = BORDER_TL;
	for (i = 0; i < (NUM_COLS - 2); ++i)
	{
		PPU_DATA = BORDER_T;
	}
	PPU_DATA = BORDER_TR;

	/* Draw sides */
	for (i = 0; i < (NUM_ROWS - 2); ++i)
	{
		PPU_DATA = BORDER_L;
		for (j = 0; j < (NUM_COLS - 2); ++j)
			PPU_DATA = BLANK_TILE;
		PPU_DATA = BORDER_R;
	}

	/* Draw bottom */
	PPU_DATA = BORDER_BL;
	for (i = 0; i < (NUM_COLS - 2); ++i)
	{
		PPU_DATA = BORDER_B;
	}
	PPU_DATA = BORDER_BR;
}

void main (void)
{
	/* Shoot the PPU our palettes */
	ppu_addr = PPU_PALETTE;
	ppu_data = PALETTE;
	ppu_data_size = sizeof(PALETTE);
	ppu_write();

	/* Pre-draw our background */
	draw_background();

	/* Enable our player sprite */
	player.x = (MAX_X / 2) - (SPRITE_WIDTH / 2);
	player.y = (MAX_Y / 2) - (SPRITE_WIDTH / 2);
	player.tile_index = SPRITE_PLAYER;

	/* Enable rendering */
	ppu_scroll_reset();
	ppu_enable();

	/* Startup main loop */
	while (1)
	{
		WaitFrame();
		ppu_scroll_reset();

		if ((InputPort1 & BUTTON_UP) &&
			player.y > MIN_Y + SPRITE_HEIGHT)
		{
			--player.y;
		}

		if ((InputPort1 & BUTTON_DOWN) &&
			player.y < MAX_Y + (SPRITE_HEIGHT * 2))
		{
			++player.y;
		}

		if ((InputPort1 & BUTTON_LEFT) &&
			player.x > MIN_X + SPRITE_WIDTH)
		{
			--player.x;
		}

		if ((InputPort1 & BUTTON_RIGHT) &&
			player.x < MAX_X + (SPRITE_WIDTH * 2))
		{
			++player.x;
		}
	}
}