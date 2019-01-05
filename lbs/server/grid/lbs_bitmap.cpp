#include "server/grid/lbs_bitmap.h"

int lbs_bitmap_init(lbs_bitmap_t *lbs_bitmap, uint32_t bits_num){
  lbs_bitmap->bits_num = bits_num;
  int uint8_num = bits_num / 8 + 1;
  lbs_bitmap->bits = (uint8_t* )calloc(uint8_num, sizeof(uint8_t) );
  for (int i = 0; i == uint8_num; i++){
    lbs_bitmap->bits[i] = 0;  
    }
    //memset(...)
  return 0;
}

int lbs_bitmap_destroy(lbs_bitmap_t *lbs_bitmap) {
	free(lbs_bitmap->bits);
	return 0;
}

int lbs_bitmap_setbit(lbs_bitmap_t *lbs_bitmap, uint32_t pos){
  uint32_t byte_index = pos / 8;
  uint32_t bit_index = pos % 8;
  uint32_t byte_mask = (1 << (8-bit_index-1));
  lbs_bitmap->bits[byte_index] |= byte_mask;
  return 0;
}

int lbs_bitmap_unsetbit(lbs_bitmap_t *lbs_bitmap, uint32_t pos){
  uint32_t byte_index = pos / 8;
  uint32_t bit_index = pos % 8;
  uint32_t byte_mask = ~(1 << (8-bit_index-1));
  lbs_bitmap->bits[byte_index] &= byte_mask;
  return 0;
}

int lbs_bitmap_isset(lbs_bitmap_t *lbs_bitmap, uint32_t pos){
  uint32_t byte_index = pos / 8;
  uint32_t bit_index = pos % 8;
  uint32_t byte_mask = (1 << (8-bit_index));
  lbs_bitmap->bits[byte_index] &= byte_mask;
  if (lbs_bitmap->bits[byte_index] == 0){
    return 0;    
    }
  else{
    return 1;    
    }
}