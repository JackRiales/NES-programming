
#include "nesdev.h"
#include <stdlib.h>

/*
    Draw functions
*/
void __fastcall__ spr (const sprite_t *sprite, u8 *oam_ptr)
{
    *oam_ptr = oam_spr(sprite->x, 
                       sprite->y, 
                       sprite->tile_index, 
                       0, 
                       *oam_ptr);
}

void __fastcall__ metaspr (const metasprite_t *metasprite, u8 *oam_ptr)
{
    *oam_ptr = oam_meta_spr(metasprite->x, 
                            metasprite->y, 
                            *oam_ptr, 
                            metasprite->index_buffer);
}

/*
    Collision system
*/
u8 __fastcall__ rect_collides(const rect_t *first, const rect_t *second)
{
    return (first->max_x > second->x && 
            second->max_x > first->x &&
            first->max_y > second->y && 
            second->max_y > first->y);
}

/*
    Digit system
*/
void __fastcall__ digit_init (digits_t *digits, u8 num)
{
  digits->num_segments = num;
  digits->segments = (u8*) malloc (num * sizeof(u8));
}

void __fastcall__ digit_update(digits_t *digits)
{
  static u8 digit_itr = 0;
  for (digit_itr = 0; digit_itr < digits->num_segments - 2; ++digit_itr)
  {
    if (digits->segments[digit_itr] > 9)
    {
      digits->segments[digit_itr]    = 0;
      digits->segments[digit_itr+1] += 1;
    }
  }
}

void __fastcall__ digit_increment(digits_t *digits, s8 amt)
{
  digits->segments[0] += amt;
  digit_update(digits);
}

void __fastcall__ nt_print(u16 adr, const u8* str)
{
	vram_adr(adr);
	while(1)
	{
		if (!(*str)) break;
		vram_put((*str++)+ASCII_OFFSET);
	}
}