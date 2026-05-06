////////////////////////////////////////////////////////////////////////
/* @file KSCdraw.c
 * @brief 基本绘图函数
 * @author OneSleepWorm(一只瞌睡虫)
 * @date 2026/1/31
 * @version 1.1.0
 */
/////////////////////////////////////////////////////////////////

#include "../inc/KSCdraw.h"
#include <string.h>

#if __USE_LCD__ ==1

static uint8_t draw_buf[_STATICBUF_SIZE];

static inline intxy _abs(intxy x) {
    return x < 0 ? -x : x;
}

void memset_16(void* buf, size_t size_bytes, uint16_t pattern) {
    if (size_bytes < sizeof(uint16_t)) return;
    uint16_t* p = (uint16_t*)buf;
    size_t len = size_bytes / sizeof(uint16_t);
    p[0] = pattern;
    size_t i;
    for (i = 1; i < len; i <<= 1) {
        size_t copy_size = (i < len - i) ? i : len - i;
        memcpy(p + i, p, copy_size * sizeof(uint16_t));
    }
}

/* 公共函数：保留原接口，内部进行指针和位宽优化 */
void imgchange(const uint8_t* imgbuf,
               uint16_t* imgbuf2,
               const uint16_t length,//bit位长度
               uint16_t bit,
               const uint16_t* colortable)
{
    switch(bit) {
    case 1: {
        const uint8_t *pSrc = imgbuf;
        uint16_t *pDst = imgbuf2;
        const uint8_t *pSrcEnd = imgbuf + (length >> 3);
        uint32_t *pDst32 = (uint32_t*)pDst;

        while (pSrc < pSrcEnd) {
            uint8_t byte = *pSrc++;
            uint16_t c0 = colortable[(byte >> 7) & 0x01];
            uint16_t c1 = colortable[(byte >> 6) & 0x01];
            uint16_t c2 = colortable[(byte >> 5) & 0x01];
            uint16_t c3 = colortable[(byte >> 4) & 0x01];
            uint16_t c4 = colortable[(byte >> 3) & 0x01];
            uint16_t c5 = colortable[(byte >> 2) & 0x01];
            uint16_t c6 = colortable[(byte >> 1) & 0x01];
            uint16_t c7 = colortable[(byte >> 0) & 0x01];

            *pDst32++ = ((uint32_t)c1 << 16) | c0;
            *pDst32++ = ((uint32_t)c3 << 16) | c2;
            *pDst32++ = ((uint32_t)c5 << 16) | c4;
            *pDst32++ = ((uint32_t)c7 << 16) | c6;
        }
        pDst = (uint16_t*)pDst32;

        uint8_t remain = length & 0x07;
        if (remain) {
            uint8_t byte = *pSrc;
            for (uint8_t j = 0; j < remain; j++) {
                *pDst++ = colortable[(byte >> (7 - j)) & 0x01];
            }
        }
        break;
    }

    case 2: {
        const uint8_t *pSrc = imgbuf;
        uint16_t *pDst = imgbuf2;
        const uint8_t *pSrcEnd = imgbuf + (length >> 2);
        uint32_t *pDst32 = (uint32_t*)pDst;

        while (pSrc < pSrcEnd) {
            uint8_t byte = *pSrc++;
            uint16_t c0 = colortable[(byte >> 6) & 0x03];
            uint16_t c1 = colortable[(byte >> 4) & 0x03];
            uint16_t c2 = colortable[(byte >> 2) & 0x03];
            uint16_t c3 = colortable[(byte >> 0) & 0x03];

            *pDst32++ = ((uint32_t)c1 << 16) | c0;
            *pDst32++ = ((uint32_t)c3 << 16) | c2;
        }
        pDst = (uint16_t*)pDst32;

        uint8_t remain = length & 0x03;
        if (remain) {
            uint8_t byte = *pSrc;
            for (uint8_t j = 0; j < remain; j++) {
                *pDst++ = colortable[(byte >> (6 - j*2)) & 0x03];
            }
        }
        break;
    }

    case 4: {
        const uint8_t *pSrc = imgbuf;
        uint16_t *pDst = imgbuf2;
        const uint8_t *pSrcEnd = imgbuf + (length >> 1);
        uint32_t *pDst32 = (uint32_t*)pDst;

        while (pSrc < pSrcEnd) {
            uint8_t byte = *pSrc++;
            uint16_t c0 = colortable[(byte >> 4) & 0x0F];
            uint16_t c1 = colortable[(byte >> 0) & 0x0F];
            *pDst32++ = ((uint32_t)c1 << 16) | c0;
        }
        pDst = (uint16_t*)pDst32;

        if (length & 0x01) {
            *pDst++ = colortable[(*pSrc >> 4) & 0x0F];
        }
        break;
    }

    default:
        return;
    }
}

#define RightUpper 0x01
#define LeftUpper 0x02
#define RightLower 0x04
#define LeftLower 0x08

KSC_buf os_screen = {0};

KSC_mes ksetpixel(KSC_buf* screen, KSCCOLOR color, uintxy x, uintxy y){
    if (!screen) return KSC_ERR;
    screen_setcanvas(x, y, 1, 1);
    screen_setcolorpixels(&color, 1);
    return KSC_OK;
}

KSC_mes ksetscreen(KSC_buf* screen){
    if (!screen) return KSC_ERR;
    os_screen = *screen;
    return KSC_OK;
}

KSC_buf* kgetscreen(){
    return &os_screen;
}

KSC_buf* kinitscreen(uintxy ssx, uintxy ssy, uintxy width, uintxy height, KSCCOLOR bk){
    KSC_buf* screen = (KSC_buf*)malloc(sizeof(KSC_buf));
    if (screen == NULL){
        return NULL;
    }

    screen->Mode = 1<<7; //标记结构体为malloc分配

    screen->ssx = ssx;
    screen->ssy = ssy;
    screen->width = width;
    screen->height = height;
    screen->bk = bk;
    ksetscreen(screen);
    screen_init();
    kfull(screen, bk, 0, 0, width, height);
    return screen;
}

void kfreescreen(KSC_buf* screen){
    if (!screen) return;
    if (screen->Mode & (1<<5)){
        free(screen->fbuf);
    }

    if (screen->Mode & (1<<7)){
        free(screen);
    }
}

KSC_mes kfull(KSC_buf* screen, KSCCOLOR color, uintxy x1, uintxy y1, uintxy w, uintxy h){
    if (!screen) return KSC_ERR;
    if (w == 0 || h == 0) return KSC_OK;
    screen_setcanvas(x1, y1, w, h);
    uint32_t pixelnum = w * h;
    uint16_t* buf = (uint16_t*)draw_buf;
    uint16_t staticbuf_pixel = (_STATICBUF_SIZE >> 1);

    memset_16(buf, _STATICBUF_SIZE, color);
    while (pixelnum > staticbuf_pixel){
        screen_setcolorpixels(buf, staticbuf_pixel);
        pixelnum -= staticbuf_pixel;
    }
    screen_setcolorpixels(buf, (uint16_t)pixelnum);

    return KSC_OK;
}

KSC_mes kline(KSC_buf* screen, KSCCOLOR color, uintxy x1, uintxy y1, uintxy x2, uintxy y2){
    if (!screen) return KSC_ERR;

    if (x1 == x2) {
        uintxy ymin = y1 < y2 ? y1 : y2;
        uintxy ymax = y1 < y2 ? y2 : y1;
        kfull(screen, color, x1, ymin, 1, ymax - ymin + 1);
        return KSC_OK;
    }
    if (y1 == y2) {
        uintxy xmin = x1 < x2 ? x1 : x2;
        uintxy xmax = x1 < x2 ? x2 : x1;
        kfull(screen, color, xmin, y1, xmax - xmin + 1, 1);
        return KSC_OK;
    }

    int dx = _abs((intxy)x2 - (intxy)x1);
    int dy = _abs((intxy)y2 - (intxy)y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        ksetpixel(screen, color, x1, y1);

        if (x1 == x2 && y1 == y2) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
    return KSC_OK;
}

KSC_mes kbox(KSC_buf* screen, KSCCOLOR color, uintxy x1, uintxy y1, uintxy w, uintxy h){
    if (!screen) return KSC_ERR;
    if (w == 0 || h == 0) return KSC_OK;

    kfull(screen, color, x1, y1, w, 1);
    kfull(screen, color, x1, y1, 1, h);
    kfull(screen, color, x1 + w - 1, y1, 1, h);
    kfull(screen, color, x1, y1 + h - 1, w, 1);
    return KSC_OK;
}

KSC_mes kfillbox(KSC_buf* screen, KSCCOLOR color, uintxy x1, uintxy y1, uintxy w, uintxy height){
    return kfull(screen, color, x1, y1, w, height);
}

KSC_mes karc(KSC_buf* screen, KSCCOLOR color, uintxy x0, uintxy y0, uint8_t r, uint8_t Anglediraction){
    if (!screen) return KSC_ERR;
    if ((Anglediraction & 0xF0) != 0) return KSC_ERR;

    intxy x = r;
    intxy y = 0;
    int err = 0;

    while (x >= y) {
        if (Anglediraction & RightLower){
            ksetpixel(screen, color, (uintxy)(x0 + x), (uintxy)(y0 + y));
            ksetpixel(screen, color, (uintxy)(x0 + y), (uintxy)(y0 + x));
        }
        if (Anglediraction & LeftLower){
            ksetpixel(screen, color, (uintxy)(x0 - y), (uintxy)(y0 + x));
            ksetpixel(screen, color, (uintxy)(x0 - x), (uintxy)(y0 + y));
        }
        if (Anglediraction & LeftUpper){
            ksetpixel(screen, color, (uintxy)(x0 - x), (uintxy)(y0 - y));
            ksetpixel(screen, color, (uintxy)(x0 - y), (uintxy)(y0 - x));
        }
        if (Anglediraction & RightUpper){
            ksetpixel(screen, color, (uintxy)(x0 + y), (uintxy)(y0 - x));
            ksetpixel(screen, color, (uintxy)(x0 + x), (uintxy)(y0 - y));
        }

        y += 1;
        if (err <= 0) {
            err += 2*y + 1;
        }
        if (err > 0) {
            x -= 1;
            err -= 2*x + 1;
        }
    }
    return KSC_OK;
}

KSC_mes kcircle(KSC_buf* screen, KSCCOLOR color, uintxy x0, uintxy y0, uint8_t r){
    return karc(screen, color, x0, y0, r, 0x0F);
}

KSC_mes kfillcircle(KSC_buf* screen, KSCCOLOR color, uintxy x0, uintxy y0, uint8_t r){
    if (!screen) return KSC_ERR;
    if (r == 0) {
        return ksetpixel(screen, color, x0, y0);
    }

    int left[256];
    int i;
    for (i = 0; i <= r; i++) left[i] = -1;

    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while (x <= y) {
        if (x > left[y]) left[y] = x;
        if (y > left[x]) left[x] = y;

        if (d < 0) {
            d += 4 * x + 6;
        } else {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }

    for (int dy = 0; dy <= r; dy++) {
        int dx = left[dy];
        if (dx >= 0) {
            int w = 2 * dx + 1;
            kfull(screen, color, x0 - dx, y0 - dy, w, 1);
            if (dy > 0) {
                kfull(screen, color, x0 - dx, y0 + dy, w, 1);
            }
        }
    }

    return KSC_OK;
}

KSC_mes kdrawimage(KSC_buf* screen, const uint16_t* img, uintxy x, uintxy y,
    uint8_t width, uint8_t height){
    if (!screen || !img) return KSC_ERR;
    screen_setcanvas(x, y, width, height);
    uint16_t imgsize = width * height;
    screen_setcolorpixels(img, imgsize);
    return KSC_OK;
}

KSC_mes kdrawimagebig(KSC_buf* screen, const uint16_t* img, uintxy x, uintxy y,
    uint8_t width, uint8_t height, uint8_t scale){
    if (!screen || !img) return KSC_ERR;
    if (scale == 0) return KSC_ERR;

    for (uint8_t h = 0; h < height; h++){
        for (uint8_t w = 0; w < width; w++){
            KSCCOLOR ncolor = *img++;
            kfull(screen, ncolor, x + w * scale, y + h * scale, scale, scale);
        }
    }
    return KSC_OK;
}

KSC_mes kroundrect(KSC_buf* screen, KSCCOLOR color, uintxy x1, uintxy y1, uintxy width, uintxy height, uint8_t r){
    if (!screen) return KSC_ERR;
    if (r == 0){
        return kbox(screen, color, x1, y1, width, height);
    }
    if (2 * r > width){
        r = width / 2;
    }
    if (2 * r > height){
        r = height / 2;
    }
    uintxy x2 = x1 + width;
    uintxy y2 = y1 + height;

    karc(screen, color, x1 + r, y1 + r, r, LeftUpper);
    karc(screen, color, x2 - r, y1 + r, r, RightUpper);
    karc(screen, color, x1 + r, y2 - r, r, LeftLower);
    karc(screen, color, x2 - r, y2 - r, r, RightLower);

    kline(screen, color, x1 + r, y1, x2 - r, y1);
    kline(screen, color, x1 + r, y2, x2 - r, y2);
    kline(screen, color, x1, y1 + r, x1, y2 - r);
    kline(screen, color, x2, y1 + r, x2, y2 - r);
    return KSC_OK;
}

KSC_mes kfillroundrect(KSC_buf* screen, KSCCOLOR color, uintxy x1, uintxy y1, uintxy width, uintxy height, uint8_t r){
    if (!screen) return KSC_ERR;
    if (r == 0){
        return kfillbox(screen, color, x1, y1, width, height);
    }
    if (2 * r > width){
        r = width / 2;
    }
    if (2 * r > height){
        r = height / 2;
    }
    uintxy x2 = x1 + width;
    uintxy y2 = y1 + height;

    kfillbox(screen, color, x1 + r, y1, width - 2 * r, height + 1);
    kfillbox(screen, color, x1, y1 + r, width + 1, height - 2 * r);

    kfillcircle(screen, color, x1 + r, y1 + r, r);
    kfillcircle(screen, color, x2 - r, y1 + r, r);
    kfillcircle(screen, color, x1 + r, y2 - r, r);
    kfillcircle(screen, color, x2 - r, y2 - r, r);
    return KSC_OK;
}

KSC_mes kimagebin(KSC_buf* screen, const uint8_t* img, uintxy x, uintxy y,
    uint8_t width, uint8_t height, KSCCOLOR colorck, KSCCOLOR colorbk){
    if (!screen || !img) return KSC_ERR;

    uint16_t pixelcount = width * height;
    uint16_t needed = pixelcount * COLORBYTE;
    if (needed > _STATICBUF_SIZE) return KSC_ERR;

    uint16_t* imgbuf = (uint16_t*)draw_buf;
    uint16_t colortable[2] = {colorbk, colorck};
    imgchange(img, imgbuf, pixelcount, 1, colortable);
    kdrawimage(screen, imgbuf, x, y, width, height);
    return KSC_OK;
}

KSC_mes kchar(KSC_buf* screen, char ch, uintxy x, uintxy y, KSCCOLOR colorck, KSCCOLOR colorbk){
    if (!screen) return KSC_ERR;
    KSC_Font1* font = &Systemfont0;
    const uint8_t* char_bitmap = font->Getfontfunc(ch);
    kimagebin(screen, char_bitmap, x, y, font->width, font->height, colorck, colorbk);
    return KSC_OK;
}

KSC_mes kstring(KSC_buf* screen, const char* str, uintxy x, uintxy y, KSCCOLOR colorck, KSCCOLOR colorbk){
    if (!screen || !str) return KSC_ERR;
    KSC_Font1* font = &Systemfont0;
    while (*str){
        kchar(screen, *str, x, y, colorck, colorbk);
#if SYSTEMFONT == 7
        x += font->width - 1;
#else
        x += font->width;
#endif
        str++;
    }
    return KSC_OK;
}

#if __USE_CHINESE__ >0
KSC_mes kstringchinese(KSC_buf* screen, const char* str, uintxy x, uintxy y, KSCCOLOR color1, KSCCOLOR color2){
    if (!screen || !str) return KSC_ERR;
    KSC_FontChinese* font = &SystemfontChinese;
    uint8_t* char_bitmap = NULL;
    uint32_t count = utf8get(str, 0, 0);
    for (uint8_t j = 0; j < count; j++){
        char_bitmap = font->Getfontfunc(str, j);
        kimagebin(screen, char_bitmap, x + j * font->width, y,
            font->width, font->height, color1, color2);
    }
    return KSC_OK;
}
#endif

#endif
