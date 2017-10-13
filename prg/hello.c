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
#include <stdlib.h>
#include "neslib.h"
#include "nesdev.c"

/* Data inclusions */
#include "gamedata.c"
#include "level.h"

/* Initialize nametables here (pre-ppu state) */
void nt_init (void)
{
	// Unroll level data to vram
	vram_adr(NAMETABLE_A);
	vram_unrle(level);

	digit_init(&points, 3);
	memcpy(TEXTBUF_Points, TEXTBUFINIT_Points, sizeof(TEXTBUFINIT_Points));
	set_vram_update(TEXTBUF_Points);
}

/* Initialize game state here (post-ppu state) */
void init (void)
{
	player.x = MAX_X / 2;
	player.y = MAX_Y / 2;
	player.index_buffer = (u8*)META_PersonSprite;

	stick.x = rand8();
	stick.y = rand8();
	stick.tile_index = SPRITE_STICK;

	player_col.x = player.x;
	player_col.y = player.y;
	player_col.w = 8;
	player_col.h = 8;
	player_col.max_x = player_col.x + player_col.w;
	player_col.max_y = player_col.y + player_col.h;

	stick_col.x = stick.x;
	stick_col.y = stick.y;
	stick_col.w = 8;
	stick_col.h = 8;
	stick_col.max_x = stick_col.x + stick_col.w;
	stick_col.max_y = stick_col.y + stick_col.h;

	touching = 0;
	player_speed = INITIAL_SPEED;
}

/* Update game state */
void update (void)
{
	/* Get input and move player */
	pad1 = pad_poll(0); // Get gamepad state
	if (pad1 & PAD_LEFT && player.x > 0)          
		player.x -= player_speed;
	if (pad1 & PAD_RIGHT && player.x < MAX_Y - 8)  
		player.x += player_speed;
	if (pad1 & PAD_UP && player.y > 0)          
		player.y -= player_speed;
	if (pad1 & PAD_DOWN && player.y < MAX_Y - 16) 
		player.y += player_speed;
	
	player_col.x = player.x;
	player_col.y = player.y;
	player_col.max_x = player.x + player_col.w;
	player_col.max_y = player.y + player_col.h;
	
	touching = rect_collides(&player_col, &stick_col);
	if (touching || stick.x > MAX_X - 8 || stick.y > MAX_Y - 8)
	{
		// Move stick somewhere else
		stick.x = rand8();
		stick.y = rand8();
		stick_col.x = stick.x;
		stick_col.y = stick.y;
		stick_col.max_x = stick.x + stick_col.w;
		stick_col.max_y = stick.y + stick_col.h;
		
		// Increment points and update the NT
		digit_increment(&points, 1);
		TEXTBUF_Points[1 * TEXTBUF_VariableIndex - 1] = points.segments[0]+0x10;
		TEXTBUF_Points[2 * TEXTBUF_VariableIndex - 1] = points.segments[1]+0x10;
		TEXTBUF_Points[3 * TEXTBUF_VariableIndex - 1] = points.segments[2]+0x10;
	}
}

/* Uses the OAM to draw our sprites */
void draw (void)
{
	metaspr(&player, &oam_ptr);
	spr(&stick, &oam_ptr);
}

void main (void)
{
	pal_bg(BG_PALETTES);
	pal_spr(SPRITE_PALETTES);

	nt_init();
	ppu_on_all();
	
	i       = 0;
	j       = 0;
	frame   = 0;
	pad1    = 0;
	pad2    = 0;
	oam_ptr = 0;
	init();
	
	while(1)
	{
		ppu_wait_frame();
		update();

		oam_ptr = 0;
		draw();
		
		++frame;
	}
}