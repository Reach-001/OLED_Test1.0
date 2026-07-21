/**
 * @file    font_cn_16.h
 * @brief   Minimal 16x16 CJK font table for UI bring-up.
 */

#ifndef FONT_CN_16_H
#define FONT_CN_16_H

#include <stdint.h>
#include "st7789_font.h"

static const st7789_cn_glyph_t cn_glyph_16[] = {
};

const st7789_cn_font_t cn_font_16 = {
    .count  = 0,
    .glyphs = cn_glyph_16,
};

const uint8_t *cn_font_lookup(uint16_t unicode)
{
    (void)unicode;
    return NULL;
}

#endif /* FONT_CN_16_H */
