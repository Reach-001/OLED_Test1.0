/**
 * @file    st7789_font.h
 * @brief   ST7789 移植字体系统 — 字体类型定义与 API 声明
 * @note    脱离 U8g2 后自建，支持 ASCII 字库 + 自定义 16×16 中文字库
 */

#ifndef ST7789_FONT_H
#define ST7789_FONT_H

#include <stdint.h>

/*===========================================================================
 * 字体结构体
 *===========================================================================*/

/** @brief 单字节 ASCII 字体描述符（等宽） */
typedef struct {
    const uint8_t  char_w;       /**< 字符宽度（像素）              */
    const uint8_t  char_h;       /**< 字符高度（像素）              */
    const uint8_t  first;        /**< 字库起始字符编码               */
    const uint8_t  last;         /**< 字库结束字符编码               */
    const uint8_t *bitmap;       /**< 字模数据基址 (row-major)      */
} st7789_font_t;

/** @brief 单个中文字符位图（16×16 固定大小，32 字节） */
typedef struct {
    const uint16_t unicode;      /**< Unicode 码点                   */
    const uint8_t  bitmap[32];   /**< 16×16 字模，row-major, 2B/row */
} st7789_cn_glyph_t;

/** @brief 中文字库描述符 */
typedef struct {
    const uint16_t          count;   /**< 收录汉字数                 */
    const st7789_cn_glyph_t *glyphs; /**< 字形数组基址               */
} st7789_cn_font_t;

/*===========================================================================
 * 内置 ASCII 字库声明（数据在 font_8x16.h / draw_driver.c 中定义）
 *===========================================================================*/

extern const st7789_font_t font_8x16;

/*===========================================================================
 * 内置中文字库声明（数据在 font_cn_16.h 中定义）
 *===========================================================================*/

extern const st7789_cn_font_t cn_font_16;

/*===========================================================================
 * 字库查找函数
 *===========================================================================*/

/**
 * @brief 在 cn_font_16 中查找 Unicode 字符的字模
 * @param unicode  Unicode 码点（如 '设' = 0x8BBE）
 * @return 32 字节字模指针，未找到返回 NULL
 */
const uint8_t *cn_font_lookup(uint16_t unicode);

#endif /* ST7789_FONT_H */
