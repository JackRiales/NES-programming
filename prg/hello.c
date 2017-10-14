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
#include "nesdev.h"

/* Data inclusions */
#include "gamedata.c"
#include "test.h"

/* Set the background palette to some 'flash palette' for one frame at a time */
void flash (void)
{
	static u8 flashing = 0;
	if (flashing)
	{
		flashing = 0;
		pal_bg(BG_PALETTES);
	}
	else
	{
		flashing = 1;
		pal_bg(BG_PALETTES_FLASH);
	}
}

/* Randomly scroll around the background for a shake effect */
void shake (void)
{
	scroll_x = rand8() % shake_force;
	scroll_y = rand8() % shake_force;
	if (rand8() > 128)
		scroll_x *= -1;
	if (rand8() > 128)
		scroll_y *= -1;
}

/* Initialize nametables here (pre-ppu state) */
void nt_init (void)
{
	// Unroll level data to vram
	vram_adr(NAMETABLE_A);
	vram_unrle(test);

	// Create the points digit array and set it as updatable
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

	world_bounds.w = MAX_X - 24;
	world_bounds.h = MAX_Y - 48;
	move_rect(&world_bounds, 8, 32);

	player_col.w = 8;
	player_col.h = 16;
	move_rect(&player_col, player.x, player.y);

	stick_col.w = 8;
	stick_col.h = 8;
	move_rect(&stick_col, stick.x, stick.y);

	// Set our speed
	player_speed = INITIAL_SPEED;

	// We're gonna fade in the screen
	bg_bright = 0;
	spr_bright = 0;

	// Set initial brightnesses
	pal_bg_bright(bg_bright);
	pal_bg_bright(spr_bright);

	// Start music
	music_play(0);
}

/* Update game state */
void update (void)
{
	/* Fading in the screen */
	if (bg_bright < 4)
	{
		music_pause(1);
		++bg_bright; ++spr_bright;
		sfx_play(0, 1);
		delay(15);
		return; // Skip input and stuff
	}
	else
		music_pause(0);

	/* Get input and move player */
	pad1 = pad_poll(0); // Get gamepad state
	if (pad1 & PAD_LEFT && player.x > world_bounds.x)          
		player.x -= player_speed;
	if (pad1 & PAD_RIGHT && player.x < world_bounds.max_x)  
		player.x += player_speed;
	if (pad1 & PAD_UP && player.y > world_bounds.y)          
		player.y -= player_speed;
	if (pad1 & PAD_DOWN && player.y < world_bounds.max_y) 
		player.y += player_speed;
	
	// Update collision box
	move_rect(&player_col, player.x, player.y);
	
	// Touch event
	touching = rect_collides(&player_col, &stick_col);
	if (touching             || 
		stick.x < 8          ||
		stick.y < 8          ||
		stick.x > MAX_X - 16 || 
		stick.y > MAX_Y - 16)
	{
		// Move stick somewhere else
		stick.x = rand8();
		stick.y = rand8();

		// Update collision box
		move_rect(&stick_col, stick.x, stick.y);
	}

	// Increment points and update the NT. Also juice!
	if (touching)
	{
		digit_increment(&points, 1);
		TEXTBUF_Points[TEXTBUF_Index(1)] = points.segments[2]+ASCII_NUMBERSPOSITION;
		TEXTBUF_Points[TEXTBUF_Index(2)] = points.segments[1]+ASCII_NUMBERSPOSITION;
		TEXTBUF_Points[TEXTBUF_Index(3)] = points.segments[0]+ASCII_NUMBERSPOSITION;

		// Juice!
		flash_time = 5;
		shake_time = 10;
		sfx_play(1, 0);
	}

	// Flash the screen 
	if (flash_time > 0)
	{
		flash();
		-- flash_time;
	}
	else
		pal_bg(BG_PALETTES);

	// Shake the screen
	if (shake_time > 0)
	{
		shake();
		-- shake_time;
	}
	else
	{
		scroll_x = 0;
		scroll_y = 0;
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
	
	init();
	
	while(1)
	{
		ppu_wait_nmi();
		
		// Main update
		update();

		// OAM draw
		oam_ptr = 0;
		draw();

		// Standard update stuff
		pal_bg_bright(bg_bright);
		pal_bg_bright(spr_bright);
		scroll(scroll_x, scroll_y);
		
		// Count frames
		++frame;
	}
}