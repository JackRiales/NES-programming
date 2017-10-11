
/* Zero-page Globals (commonly used vars) */
#pragma bss-name(push, "ZEROPAGE")
/* Often used vars */
u8 i;               /* Iterator */
u8 j;               /* Iterator */

/* PPU write data */
uptr ppu_addr;      /* PPU destination addr */
const u8* ppu_data; /* Pointer to data to copy over */
u8 ppu_data_size;   /* Size in bytes of data */

/* Input state */
u8 pad;

/* Game data */
rect_t player_rect; /* Player collision box */
rect_t stick_rect;  /* Stick collision box */
u8 colliding;       /* Is the player and stick colliding */
points_t points;
#pragma bss-name(pop)

/* Sprite data */
sprite_t player;
sprite_t stick;

const u8 PALETTE[] = {
	COLOR_BLACK, /* Background color */

	COLOR_BLUE, COLOR_WHITE, COLOR_RED, /* BG Palette 0 */
	0,
	0, 0, 0,
	0,
	0, 0, 0,
	0,
	0, 0, 0,

	COLOR_BLACK, /* Background color (mirror) */

	COLOR_RED, COLOR_YELLOW, COLOR_BLUE, /* Sprite palette 0 */
	0,
	0, 0, 0,
	0,
	0, 0, 0,
	0,
	0, 0, 0,
};

/* Sprite tile indices */
#define BLANK_TILE    0x00
#define BORDER_TL     0x01
#define BORDER_TR     0x02
#define BORDER_BL     0x11
#define BORDER_BR     0x12
#define BORDER_T      0x04
#define BORDER_B      0x14
#define BORDER_L      0x03
#define BORDER_R      0x13
#define SPRITE_PLAYER 0x10
#define SPRITE_STICK  0x05