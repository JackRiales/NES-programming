
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

	COLOR_DGRAY, COLOR_WHITE, COLOR_LGRAY, /* Sprite palette 0 */
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