#include "GenDsp.h"
#include "oledfont.h"
#include <math.h>

#define DegToRad(x) x / (180 / M_PI)

/* internal variable */
GenDsp_DspArea_Limit_Range DspRange = {
    .width = 0,
    .heigh = 0,
    .x = 0,
    .y = 0,
};

/* internal function definition */

/* external draw funtion definition */
static uint8_t GenDsp_GetStrLen(GenFont_List font, char *str);
static void GenDsp_DrawPoint(uint8_t **map, int16_t x, int16_t y, bool set);
static void GenDsp_SetRange(int16_t x, int16_t y, uint8_t width, uint8_t height);
static bool GenDsp_DrawChar(GenFont_List font, uint8_t **map, char c, int16_t x, int16_t y, bool col_inv);
static void GenDsp_DrawStr(GenFont_List font, uint8_t **map, char *str, int16_t x, int16_t y, bool col_inv);
static void GenDsp_DrawNum(GenFont_List font, uint8_t **map, uint32_t num, int16_t x, int16_t y, bool col_inv);
static void GenDsp_DrawCircle(uint8_t **map, int16_t center_x, int16_t center_y, uint8_t radius, uint8_t option, bool col_inv);
static void GenDsp_DrawRectangle(uint8_t **map, int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t line_size, bool col_inv);
static void GenDsp_DrawLen(uint8_t **map, int8_t start_x, int8_t start_y, int8_t end_x, int8_t end_y, uint8_t line_sidth, bool col_inv);
static void GenDsp_DrawRad(uint8_t **map, int8_t center_x, int8_t center_y, uint8_t radius, uint8_t line_size, int8_t angle, bool col_inv);
static void GenDsp_Draw_RadiusRectangle(uint8_t **map, int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t radius, uint8_t line_width, bool col_inv);
static void GenDsp_FillCircle(uint8_t **map, int16_t x, int16_t y, uint8_t radius, uint8_t option, bool col_inv);
static void GenDsp_FillRectangle(uint8_t **map, int16_t x, int16_t y, uint8_t width, uint8_t height, bool col_inv);
static void GenDsp_Fill_RadiusRectangle(uint8_t **map, int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t radius, bool col_inv);
static void GenDsp_Draw_HLine(uint8_t **map, int16_t x, int16_t y, uint8_t len, uint8_t line_width, bool col_inv);
static void GenDsp_Draw_VLine(uint8_t **map, int16_t x, int16_t y, uint8_t len, uint8_t line_width, bool col_inv);

GeneralDispalyProc_TypeDef GenDsp_Interface = {
    .draw_img = NULL,
    .draw_str = GenDsp_DrawStr,
    .draw_num = GenDsp_DrawNum,
    .draw_line = GenDsp_DrawLen,
    .draw_vertical_line = GenDsp_Draw_VLine,
    .draw_horizon_line = GenDsp_Draw_HLine,
    .str_len = GenDsp_GetStrLen,
    .draw_char = GenDsp_DrawChar,
    .set_range = GenDsp_SetRange,
    .draw_point = GenDsp_DrawPoint,
    .draw_circle = GenDsp_DrawCircle,
    .draw_rectangle = GenDsp_DrawRectangle,
    .draw_radius_rectangle = GenDsp_Draw_RadiusRectangle,
    .fill_circle = GenDsp_FillCircle,
    .fill_rectangle = GenDsp_FillRectangle,
    .fill_radius_rectangle = GenDsp_Fill_RadiusRectangle,
};

static void GenDsp_SetRange(int16_t x, int16_t y, uint8_t width, uint8_t height)
{
    DspRange.heigh = height;
    DspRange.width = width;
    DspRange.x = x;
    DspRange.y = y;
}

static void GenDsp_DrawPoint(uint8_t **map, int16_t x, int16_t y, bool set)
{
    if ((x >= DspRange.width) || (y >= DspRange.heigh) || (x < 0) || (y < 0))
        return;

    if (set)
    {
        map[y][x] = 1;
    }
    else
        map[y][x] = 0;
}

static bool GenDsp_DrawChar(GenFont_List font, uint8_t **map, char c, int16_t x, int16_t y, bool col_inv)
{
    uint8_t temp;
    int16_t y0 = y;
    c -= ' ';

    if (DspRange.width <= x)
        return false;

    for (uint8_t t = 0; t < font; t++)
    {
        if (font == Font_12)
            temp = oled_asc2_1206[c][t];
        else if (font == Font_16)
            temp = oled_asc2_1608[c][t];
        else if (font == Font_8)
            temp = oled_asc2_0808[c][t];

        for (uint8_t bit_index = 0; bit_index < FONT_WIDTH; bit_index++)
        {
            if ((DspRange.x + DspRange.width) <= x)
                return false;

            if (temp & 0x80)
                GenDsp_DrawPoint(map, x, y, col_inv ^ map[y][x]);
            else
                GenDsp_DrawPoint(map, x, y, !col_inv ^ map[y][x]);

            temp <<= 1;

            y++;
            if ((y - y0) >= font)
            {
                y = y0;
                x++;

                break;
            }
        }
    }

    return true;
}

static void GenDsp_DrawStr(GenFont_List font, uint8_t **map, char *str, int16_t x, int16_t y, bool col_inv)
{
    while (*str != '\0')
    {
        if (GenDsp_DrawChar(font, map, *str, x, y, col_inv))
        {
            x += FONT_WIDTH;
            str++;
        }
        else
            break;
    }
}

static void GenDsp_DrawNum(GenFont_List font, uint8_t **map, uint32_t num, int16_t x, int16_t y, bool col_inv)
{
    char num_buff[MAX_DSP_NUM_LEN];

    sprintf(num_buff, "%d", num);
    GenDsp_DrawStr(font, map, num_buff, x, y, col_inv);
}

static uint8_t GenDsp_GetStrLen(GenFont_List font, char *str)
{
    uint8_t len = 0;

    while (*str != '\0')
    {
        len += FONT_WIDTH;
        str++;
    }

    return len;
}

static void GenDsp_DrawLen(uint8_t **map, int8_t start_x, int8_t start_y, int8_t end_x, int8_t end_y, uint8_t line_sidth, bool col_inv)
{
    int16_t xerr = 0;
    int16_t yerr = 0;
    int8_t delta_x = end_x - start_x;
    int8_t delta_y = end_y - start_y;
    int16_t distance;
    int8_t uRow = start_x;
    int8_t uCol = start_y;

    int8_t incx, incy;

    if (delta_x > 0)
    {
        incx = 1;
    }
    else if (delta_x == 0)
    {
        incx = 0;
    }
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)
    {
        incy = 1;
    }
    else if (delta_y == 0)
    {
        incy = 0;
    }
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if (delta_x > delta_y)
    {
        distance = delta_x;
    }
    else
    {
        distance = delta_y;
    }

    for (uint8_t t = 0; t < distance + 1; t++)
    {
        GenDsp_DrawPoint(map, uRow, uCol, col_inv);

        xerr += delta_x;
        yerr += delta_y;

        if (xerr >= distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr >= distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}

static void GenDsp_Draw_VLine(uint8_t **map, int16_t x, int16_t y, uint8_t len, uint8_t line_width, bool col_inv)
{
    GenDsp_DrawLen(map, x, y, x, y + len, line_width, col_inv);
}

static void GenDsp_Draw_HLine(uint8_t **map, int16_t x, int16_t y, uint8_t len, uint8_t line_width, bool col_inv)
{
    GenDsp_DrawLen(map, x, y, x + len, y, line_width, col_inv);
}

static void GenDsp_Draw_Circle_Section(uint8_t **map, int16_t x, int16_t y, int16_t x0, int16_t y0, uint8_t option, bool col_inv)
{
    /* upper right */
    if (option & DRAW_UPPER_RIGHT)
    {
        GenDsp_DrawPoint(map, x0 + x, y0 - y, col_inv);
        GenDsp_DrawPoint(map, x0 + y, y0 - x, col_inv);
    }

    /* upper left */
    if (option & DRAW_UPPER_LEFT)
    {
        GenDsp_DrawPoint(map, x0 - x, y0 - y, col_inv);
        GenDsp_DrawPoint(map, x0 - y, y0 - x, col_inv);
    }

    /* lower right */
    if (option & DRAW_LOWER_RIGHT)
    {
        GenDsp_DrawPoint(map, x0 + x, y0 + y, col_inv);
        GenDsp_DrawPoint(map, x0 + y, y0 + x, col_inv);
    }

    /* lower left */
    if (option & DRAW_LOWER_LEFT)
    {
        GenDsp_DrawPoint(map, x0 - x, y0 + y, col_inv);
        GenDsp_DrawPoint(map, x0 - y, y0 + x, col_inv);
    }
}

static void GenDsp_Draw_Circle(uint8_t **map, int16_t x0, int16_t y0, uint8_t rad, uint8_t option, bool col_inv)
{
    int8_t f;
    int8_t ddF_x;
    int8_t ddF_y;
    int16_t x;
    int16_t y;

    f = 1;
    f -= rad;
    ddF_x = 1;
    ddF_y = 0;
    ddF_y -= rad;
    ddF_y *= 2;
    x = 0;
    y = rad;

    GenDsp_Draw_Circle_Section(map, x, y, x0, y0, option, col_inv);

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        GenDsp_Draw_Circle_Section(map, x, y, x0, y0, option, col_inv);
    }
}

static void GenDsp_DrawCircle(uint8_t **map, int16_t x0, int16_t y0, uint8_t rad, uint8_t option, bool col_inv)
{
    /* draw circle */
    GenDsp_Draw_Circle(map, x0, y0, rad, option, col_inv);
}

static void GenDsp_DrawRectangle(uint8_t **map, int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t line_size, bool col_inv)
{
    int16_t x_tmp = x + width - 1;
    int16_t y_tmp = y + height - 1;

    for (uint8_t width_tmp = x; width_tmp < x_tmp + 1; width_tmp++)
    {
        if (col_inv)
        {
            GenDsp_DrawPoint(map, width_tmp, y, col_inv ^ map[y][width_tmp]);
            GenDsp_DrawPoint(map, width_tmp, y_tmp, col_inv ^ map[y_tmp][width_tmp]);
        }
        else
        {
            GenDsp_DrawPoint(map, width_tmp, y, !col_inv ^ map[y][width_tmp]);
            GenDsp_DrawPoint(map, width_tmp, y_tmp, !col_inv ^ map[y_tmp][width_tmp]);
        }
    }

    for (uint8_t height_tmp = y + 1; height_tmp < y_tmp; height_tmp++)
    {
        if (col_inv)
        {

            GenDsp_DrawPoint(map, x, height_tmp, col_inv ^ map[height_tmp][x]);
            GenDsp_DrawPoint(map, x_tmp, height_tmp, col_inv ^ map[height_tmp][x_tmp]);
        }
        else
        {
            GenDsp_DrawPoint(map, x + 1, height_tmp, !col_inv ^ map[height_tmp][x]);
            GenDsp_DrawPoint(map, x_tmp, height_tmp, !col_inv ^ map[height_tmp][x_tmp]);
        }
    }
}

/* bug in this function */
static void GenDsp_Draw_RadiusRectangle(uint8_t **map, int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t radius, uint8_t line_size, bool col_inv)
{
    int16_t x_start = x + radius;
    int16_t y_start = y + radius;

    int16_t x_tmp = x + width - 1 - radius;
    int16_t y_tmp = y + height - 1 - radius;

    int16_t x_tmp_1 = x + width - 1;
    int16_t y_tmp_1 = y + height - 1;

    /* fixed */
    GenDsp_Draw_Circle(map, x_start, y_tmp, radius, DRAW_LOWER_LEFT, col_inv);
    GenDsp_DrawLen(map, x, y_start, x, y_tmp, line_size, col_inv);

    GenDsp_Draw_Circle(map, x_tmp, y_tmp, radius, DRAW_LOWER_RIGHT, col_inv);
    GenDsp_DrawLen(map, x_start, y_tmp_1, x_tmp, y_tmp_1, line_size, col_inv);

    GenDsp_Draw_Circle(map, x_start, y_start, radius, DRAW_UPPER_LEFT, col_inv);
    GenDsp_DrawLen(map, x_start, y, x_tmp, y, line_size, col_inv);

    GenDsp_Draw_Circle(map, x_tmp, y_start, radius, DRAW_UPPER_RIGHT, col_inv);
    GenDsp_DrawLen(map, x_tmp_1, y_start, x_tmp_1, y_tmp, line_size, col_inv);
}

static void GenDsp_Fill_Circle_Section(uint8_t **map, int16_t x, int16_t y, int16_t x0, int16_t y0, uint8_t option, bool col_inv)
{
    /* upper right */
    if (option & DRAW_UPPER_RIGHT)
    {
        GenDsp_Draw_VLine(map, x0 + x, y0 - y, y + 1, 1, col_inv);
        GenDsp_Draw_VLine(map, x0 + y, y0 - x, x + 1, 1, col_inv);
    }

    /* upper left */
    if (option & DRAW_UPPER_LEFT)
    {
        GenDsp_Draw_VLine(map, x0 - x, y0 - y, y + 1, 1, col_inv);
        GenDsp_Draw_VLine(map, x0 - y, y0 - x, x + 1, 1, col_inv);
    }

    /* lower right */
    if (option & DRAW_LOWER_RIGHT)
    {
        GenDsp_Draw_VLine(map, x0 + x, y0, y + 1, 1, col_inv);
        GenDsp_Draw_VLine(map, x0 + y, y0, x + 1, 1, col_inv);
    }

    /* lower left */
    if (option & DRAW_LOWER_LEFT)
    {
        GenDsp_Draw_VLine(map, x0 - x, y0, y + 1, 1, col_inv);
        GenDsp_Draw_VLine(map, x0 - y, y0, x + 1, 1, col_inv);
    }
}

static void GenDsp_FillCircle(uint8_t **map, int16_t x0, int16_t y0, uint8_t radius, uint8_t option, bool col_inv)
{
    int8_t f;
    int8_t ddF_x;
    int8_t ddF_y;
    int16_t x;
    int16_t y;

    f = 1;
    f -= radius;
    ddF_x = 1;
    ddF_y = 0;
    ddF_y -= radius;
    ddF_y *= 2;
    x = 0;
    y = radius;

    GenDsp_Fill_Circle_Section(map, x, y, x0, y0, option, col_inv);

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        GenDsp_Fill_Circle_Section(map, x, y, x0, y0, option, col_inv);
    }
}

static void GenDsp_FillRectangle(uint8_t **map, int16_t x, int16_t y, uint8_t width, uint8_t height, bool col_inv)
{
    for (int16_t x_tmp = x; x_tmp < x + width; x_tmp++)
    {
        for (int16_t y_tmp = y; y_tmp < y + height; y_tmp++)
        {
            GenDsp_DrawPoint(map, x_tmp, y_tmp, col_inv ^ map[y_tmp][x_tmp]);
        }
    }
}

static void GenDsp_Fill_RadiusRectangle(uint8_t **map, int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t radius, bool col_inv)
{
    GenDsp_FillRectangle(map, x + radius, y, width - 2 * radius, height, col_inv);
    GenDsp_FillRectangle(map, x, y + radius, radius, height - 2 * radius - 1, col_inv);
    GenDsp_FillRectangle(map, x + width - radius, y + radius, radius + 1, height - 2 * radius, col_inv);

    GenDsp_FillCircle(map, x + radius, y + radius, radius, DRAW_UPPER_LEFT, col_inv);
    GenDsp_FillCircle(map, x + width - radius, y + radius, radius, DRAW_UPPER_RIGHT, col_inv);
    GenDsp_FillCircle(map, x + radius, y + height - radius - 2, radius, DRAW_LOWER_LEFT, col_inv);
    GenDsp_FillCircle(map, x + width - radius, y + height - radius - 2, radius, DRAW_LOWER_RIGHT, col_inv);
}
