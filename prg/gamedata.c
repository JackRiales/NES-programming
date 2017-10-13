
/* Zero-page Globals (commonly used vars) */
#pragma bss-name(push, "ZEROPAGE")
/* Often used vars */
u8 i;               /* Iterator */
u8 j;               /* Iterator */
u8 frame;           /* Frame counter */

/* Input state */
u8 pad1;
u8 pad2;

/* Current sprite oam offset */
u8 oam_ptr;

/* Our sprite data */
metasprite_t player;
sprite_t stick;

/* Colliders */
rect_t player_col;
rect_t stick_col;
u8 touching;

/* Some fun gameplay stuff */
u8 player_speed;
digits_t points;
#pragma bss-name(pop)

/* Palettes */
const u8 BG_PALETTES[16] =
{
	0x0f,0x17,0x27,0x37,
	0x0f,0x11,0x21,0x31,
	0x0f,0x15,0x25,0x35,
	0x0f,0x19,0x29,0x39
};

const u8 SPRITE_PALETTES[16] =
{
	0x0f,0x17,0x27,0x37,
	0x0f,0x11,0x21,0x31,
	0x0f,0x15,0x25,0x35,
	0x0f,0x19,0x29,0x39
};

/* Meta sprite buffer format */
/* X-offset | Y-offset | TILE_INDEX | Attribute */
const u8 META_PersonSprite[] =
{
	0, 0, 0x60, 0,
	0, 8, 0x70, 0,
	METASPR_EOB /* End of buffer */
};

/* Points text buffer */
#define TEXTBUFSIZE_Points (3*3+1)
#define TEXTBUF_VariableIndex 3
u8 TEXTBUF_Points[TEXTBUFSIZE_Points];
const u8 TEXTBUFINIT_Points[TEXTBUFSIZE_Points] =
{
	MSB(NTADR_A(4, 2)), LSB(NTADR_A(4, 2)), ASCII_NUMBERSPOSITION,
	MSB(NTADR_A(5, 2)), LSB(NTADR_A(5, 2)), ASCII_NUMBERSPOSITION,
	MSB(NTADR_A(6, 2)), LSB(NTADR_A(6, 2)), ASCII_NUMBERSPOSITION,
	NT_UPD_EOF
};

#define INITIAL_SPEED 2

#define SPRITE_STICK 0x61