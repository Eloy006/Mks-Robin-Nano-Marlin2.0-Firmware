/*
*---------------------------------------------------------------
*                        Lvgl Font Tool                         
*                                                               
* 注:使用unicode编码                                              
* 注:本字体文件由Lvgl Font Tool V0.2 生成                          
* 作者:阿里(qq:617622104)                                         
*---------------------------------------------------------------
*/

#include "../../../inc/MarlinConfigPre.h"

#if HAS_TFT_LVGL_UI

#include "pic_manager.h"
#include <lvgl.h>

#include "../../../inc/MarlinConfig.h"

#if HAS_SPI_FLASH_FONT


typedef struct{
    uint16_t min;
    uint16_t max;
    uint8_t  bpp;
    uint8_t  reserved[3];
}x_header_t;
typedef struct{
    uint32_t pos;
}x_table_t;
typedef struct{
    uint8_t adv_w;
    uint8_t box_w;
}glyph_dsc_t;


static x_header_t __g_xbf_hd = {
    .min = 0,
    .max = 0,
    .bpp = 0,
};
static uint8_t __g_font_buf[630];//如bin文件存在SPI FLASH可使用此buff


static uint8_t *__user_font_getdata(int offset, int size){
    get_spi_flash_data_2((char *)__g_font_buf, offset, size);
    return __g_font_buf;
}


static const uint8_t * __user_font_get_bitmap(const lv_font_t * font, uint32_t unicode_letter) {
    if( __g_xbf_hd.max==0 ) {
        uint8_t *p = __user_font_getdata(0, sizeof(x_header_t));
        memcpy(&__g_xbf_hd, p, sizeof(x_header_t));
    }
    if( unicode_letter>__g_xbf_hd.max || unicode_letter<__g_xbf_hd.min ) {
        return NULL;
    }
    uint32_t unicode_offset = sizeof(x_header_t)+(unicode_letter-__g_xbf_hd.min)*4;
    uint32_t *p_pos = (uint32_t *)__user_font_getdata(unicode_offset, 4);
    if( p_pos[0] != 0 ) {
        uint32_t pos = p_pos[0];
        glyph_dsc_t * gdsc = (glyph_dsc_t*)__user_font_getdata(pos, 2);
        return __user_font_getdata(pos+2, gdsc->box_w*__g_xbf_hd.bpp/8);
    }
    return NULL;
}


static bool __user_font_get_glyph_dsc(const lv_font_t * font, lv_font_glyph_dsc_t * dsc_out, uint32_t unicode_letter, uint32_t unicode_letter_next) {
    if( __g_xbf_hd.max==0 ) {
        uint8_t *p = __user_font_getdata(0, sizeof(x_header_t));
        memcpy(&__g_xbf_hd, p, sizeof(x_header_t));
    }
    if( unicode_letter>__g_xbf_hd.max || unicode_letter<__g_xbf_hd.min ) {
        return NULL;
    }
    uint32_t unicode_offset = sizeof(x_header_t)+(unicode_letter-__g_xbf_hd.min)*4;
    uint32_t *p_pos = (uint32_t *)__user_font_getdata(unicode_offset, 4);
    if( p_pos[0] != 0 ) {
        glyph_dsc_t * gdsc = (glyph_dsc_t*)__user_font_getdata(p_pos[0], 2);
        dsc_out->adv_w = gdsc->adv_w;
        dsc_out->box_h = font->line_height;
        dsc_out->box_w = gdsc->box_w;
        dsc_out->ofs_x = 0;
        dsc_out->ofs_y = 0;
        dsc_out->bpp   = __g_xbf_hd.bpp;
        return true;
    }
    return false;
}


//宋体,常规,26
//字模高度：35
//XBF字体,外部bin文件
lv_font_t gb2312_songti26;
void init_gb2312_songti26(){
    gb2312_songti26.get_glyph_bitmap = __user_font_get_bitmap;
    gb2312_songti26.get_glyph_dsc = __user_font_get_glyph_dsc;
    gb2312_songti26.line_height = 35;
    gb2312_songti26.base_line = 0;
}

#endif // HAS_SPI_FLASH_FONT
#endif // HAS_TFT_LVGL_UI

//end of file