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
static void GenDsp_DrawPoint(uint8_t **map, int8_t x, int8_t y, bool set);
static void GenDsp_SetRange(int8_t x, int8_t y, uint8_t width, uint8_t height);
static bool GenDsp_DrawChar(GenFont_List font, uint8_t **map, char c, int8_t x, int8_t y, bool col_inv);
static void GenDsp_DrawStr(GenFont_List font, uint8_t **map, char *str, int8_t x, int8_t y, bool col_inv);
static void GenDsp_DrawNum(GenFont_List font, uint8_t **map, uint32_t num, int8_t x, int8_t y, bool col_inv);
static void GenDsp_DrawCircle(uint8_t **map, int8_t center_x, int8_t center_y, uint8_t radius, uint8_t line_size);
static void GenDsp_DrawRectangle(uint8_t **map, int8_t x, int8_t y, uint8_t width, uint8_t height, uint8_t line_size);
static void GenDsp_DrawLen(uint8_t **map, int8_t start_x, int8_t start_y, int8_t end_x, int8_t end_y, uint8_t line_sidth);
static void GenDsp_DrawRad(uint8_t **map, int8_t center_x, int8_t center_y, uint8_t radius, uint8_t line_size, int8_t angle);
static void GenDsp_Draw_RadiusRectangle(uint8_t **map, int8_t x, int8_t y, uint8_t width, uint8_t height, uint8_t radius, uint8_t line_width);
static void GenDsp_FillCircle(uint8_t **map, int8_t x, int8_t y, uint8_t radius, uint8_t option);
static void GenDsp_FillRectangle(uint8_t **map, int8_t x, int8_t y, uint8_t width, uint8_t height);
static void GenDsp_Fill_RadiusRectangle(uint8_t **map, int8_t x, int8_t y, uint8_t width, uint8_t height, uint8_t radius);
static void GenDsp_Draw_HLine(uint8_t **map, int8_t x, int8_t y, uint8_t len, uint8_t line_width);
static void GenDsp_Draw_VLine(uint8_t **map, int8_t x, int8_t y, uint8_t len, uint8_t line_width);

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

static void GenDsp_SetRange(int8_t x, int8_t y, uint8_t width, uint8_t height)
{
    DspRange.heigh = height;
    DspRange.width = width;
    DspRange.x = x;
    DspRange.y = y;
}

static void GenDsp_DrawPoint(uint8_t **map, int8_t x, int8_t y, bool set)
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

static bool GenDsp_DrawChar(GenFont_List font, uint8_t **map, char c, int8_t x, int8_t y, bool col_inv)
{
    uint8_t temp;
    int8_t y0 = y;
    c -= ' ';

    if (DspRange.width <= x)
        return false;

    for (uint8_t t = 0; t < font; t++)
    {
        if (font == Font_12)
            temp = oled_asc2_1206[c][t];
        else
            temp = oled_asc2_1608[c][t];

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

static void GenDsp_DrawStr(GenFont_List font, uint8_t **map, char *str, int8_t x, int8_t y, bool col_inv)
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

static void GenDsp_DrawNum(GenFont_List font, uint8_t **map, uint32_t num, int8_t x, int8_t y, bool col_inv)
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

static void GenDsp_DrawLen(uint8_t **map, int8_t start_x, int8_t start_y, int8_t end_x, int8_t end_y, uint8_t line_sidth)
{
    int8_t xerr = 0;
    int8_t yerr = 0;
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
        GenDsp_DrawPoint(map, uRow, uCol, true);

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

static void GenDsp_Draw_VLine(uint8_t **map, int8_t x, int8_t y, uint8_t len, uint8_t line_width)
{
    GenDsp_DrawLen(map, x, y, x, y + len, line_width);
}

static void GenDsp_Draw_HLine(uint8_t **map, int8_t x, int8_t y, uint8_t len, uint8_t line_width)
{
    GenDsp_DrawLen(map, x, y, x + len, y, line_width);
}

static void GenDsp_Draw_Circle_Section(uint8_t **map, int8_t x, int8_t y, int8_t x0, int8_t y0, uint8_t option)
{
    /* upper right */
    if (option & DRAW_UPPER_RIGHT)
    {
        GenDsp_DrawPoint(map, x0 + x, y0 - y, 1);
        GenDsp_DrawPoint(map, x0 + y, y0 - x, 1);
    }

    /* upper left */
    if (option & DRAW_UPPER_LEFT)
    {
        GenDsp_DrawPoint(map, x0 - x, y0 - y, 1);
        GenDsp_DrawPoint(map, x0 - y, y0 - x, 1);
    }

    /* lower right */
    if (option & DRAW_LOWER_RIGHT)
    {
        GenDsp_DrawPoint(map, x0 + x, y0 + y, 1);
        GenDsp_DrawPoint(map, x0 + y, y0 + x, 1);
    }

    /* lower left */
    if (option & DRAW_LOWER_LEFT)
    {
        GenDsp_DrawPoint(map, x0 - x, y0 + y, 1);
        GenDsp_DrawPoint(map, x0 - y, y0 + x, 1);
    }
}

static void GenDsp_Draw_Circle(uint8_t **map, int8_t x0, int8_t y0, uint8_t rad, uint8_t option)
{
    int8_t f;
    int8_t ddF_x;
    int8_t ddF_y;
    int8_t x;
    int8_t y;

    f = 1;
    f -= rad;
    ddF_x = 1;
    ddF_y = 0;
    ddF_y -= rad;
    ddF_y *= 2;
    x = 0;
    y = rad;

    GenDsp_Draw_Circle_Section(map, x, y, x0, y0, option);

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

        GenDsp_Draw_Circle_Section(map, x, y, x0, y0, option);
    }
}

static void GenDsp_DrawCircle(uint8_t **map, int8_t x0, int8_t y0, uint8_t rad, uint8_t option)
{
    /* draw circle */
    GenDsp_Draw_Circle(map, x0, y0, rad, DRAW_ALL);
}

static void GenDsp_DrawRectangle(uint8_t **map, int8_t x, int8_t y, uint8_t width, uint8_t height, uint8_t line_size)
{
    int8_t x_tmp = x + width - 1;
    int8_t y_tmp = y + height - 1;

    GenDsp_DrawLen(map, x, y, x_tmp, y, line_size);
    GenDsp_DrawLen(map, x, y, x, y_tmp, line_size);

    GenDsp_DrawLen(map, x_tmp, y, x_tmp, y_tmp, line_size);
    GenDsp_DrawLen(map, x, y_tmp, x_tmp, y_tmp, line_size);
}

/* bug in this function */
static void GenDsp_Draw_RadiusRectangle(uint8_t **map, int8_t x, int8_t y, uint8_t width, uint8_t height, uint8_t radius, uint8_t line_size)
{
    int8_t x_start = x + radius;
    int8_t y_start = y + radius;

    int8_t x_tmp = x + width - 1 - radius;
    int8_t y_tmp = y + height - 1 - radius;

    int8_t x_tmp_1 = x + width - 1;
    int8_t y_tmp_1 = y + height - 1;

    /* fixed */
    GenDsp_Draw_Circle(map, x_start, y_tmp, radius, DRAW_LOWER_LEFT);
    GenDsp_DrawLen(map, x, y_start, x, y_tmp, line_size);

    GenDsp_Draw_Circle(map, x_tmp, y_tmp, radius, DRAW_LOWER_RIGHT);
    GenDsp_DrawLen(map, x_start, y_tmp_1, x_tmp, y_tmp_1, line_size);

    GenDsp_Draw_Circle(map, x_start, y_start, radius, DRAW_UPPER_LEFT);
    GenDsp_DrawLen(map, x_start, y, x_tmp, y, line_size);

    GenDsp_Draw_Circle(map, x_tmp, y_start, radius, DRAW_UPPER_RIGHT);
    GenDsp_DrawLen(map, x_tmp_1, y_start, x_tmp_1, y_tmp, line_size);
}

static void GenDsp_Fill_Circle_Section(uint8_t **map, int8_t x, int8_t y, int8_t x0, int8_t y0, uint8_t option)
{
    /* upper right */
    if (option & DRAW_UPPER_RIGHT)
    {
        GenDsp_Draw_VLine(map, x0 + x, y0 - y, y + 1, 1);
        GenDsp_Draw_VLine(map, x0 + y, y0 - x, x + 1, 1);
    }

    /* upper left */
    if (option & DRAW_UPPER_LEFT)
    {
        GenDsp_Draw_VLine(map, x0 - x, y0 - y, y + 1, 1);
        GenDsp_Draw_VLine(map, x0 - y, y0 - x, x + 1, 1);
    }

    /* lower right */
    if (option & DRAW_LOWER_RIGHT)
    {
        GenDsp_Draw_VLine(map, x0 + x, y0, y + 1, 1);
        GenDsp_Draw_VLine(map, x0 + y, y0, x + 1, 1);
    }

    /* lower left */
    if (option & DRAW_LOWER_LEFT)
    {
        GenDsp_Draw_VLine(map, x0 - x, y0, y + 1, 1);
        GenDsp_Draw_VLine(map, x0 - y, y0, x + 1, 1);
    }
}

static void GenDsp_FillCircle(uint8_t **map, int8_t x0, int8_t y0, uint8_t radius, uint8_t option)
{
    int8_t f;
    int8_t ddF_x;
    int8_t ddF_y;
    int8_t x;
    int8_t y;

    f = 1;
    f -= radius;
    ddF_x = 1;
    ddF_y = 0;
    ddF_y -= radius;
    ddF_y *= 2;
    x = 0;
    y = radius;

    GenDsp_Fill_Circle_Section(map, x, y, x0, y0, option);

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

        GenDsp_Fill_Circle_Section(map, x, y, x0, y0, option);
    }
}

static void GenDsp_FillRectangle(uint8_t **map, int8_t x, int8_t y, uint8_t width, uint8_t height)
{
    for (int8_t x_tmp = x; x_tmp < x + width; x_tmp++)
    {
        for (int8_t y_tmp = y; y_tmp < y + height; y_tmp++)
        {
            GenDsp_DrawPoint(map, x_tmp, y_tmp, true);
        }
    }
}

static void GenDsp_Fill_RadiusRectangle(uint8_t **map, int8_t x, int8_t y, uint8_t width, uint8_t height, uint8_t radius)
{
    GenDsp_FillRectangle(map, x + radius, y, width - 2 * radius, height);
    GenDsp_FillRectangle(map, x, y + radius, radius + 1, height - 2 * radius - 1);
    GenDsp_FillRectangle(map, x + width - radius, y + radius, radius + 1, height - 2 * radius);

    GenDsp_FillCircle(map, x + radius, y + radius, radius, DRAW_UPPER_LEFT);
    GenDsp_FillCircle(map, x + width - radius, y + radius, radius, DRAW_UPPER_RIGHT);
    GenDsp_FillCircle(map, x + radius, y + height - radius - 2, radius, DRAW_LOWER_LEFT);
    GenDsp_FillCircle(map, x + width - radius, y + height - radius - 2, radius, DRAW_LOWER_RIGHT);
}
