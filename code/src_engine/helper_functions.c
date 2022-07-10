/**
 * ================================================================================
 * @file helper_functions.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of the different helper functions.
 * @version 0.2
 * @date 2022-01-10
 * @copyright JuvuSoft (c) 2022
 * ================================================================================
 */

#define _USE_MATH_DEFINES

/* Standard includes: */
#include <stdlib.h>
#include <math.h>
#include <assert.h>

/* Program includes: */
#include <utils.h>

f32
trim_f32(f32 value, f32 low_bnd, f32 high_bnd)
{
    if (value < low_bnd) 
    {
        return low_bnd;
    }
    if (value > high_bnd) 
    {
        return high_bnd;
    }
    return value;
}

b32
check_collision(V2_f32_t *BL_mv, V2_f32_t *UR_mv, V2_f32_t *BL_st, V2_f32_t *UR_st, 
    char *collision_side)
{
    /* Calculation the objects widths and heights. */
    f32 width_mv = UR_mv->x - BL_mv->x;
    f32 height_mv = UR_mv->y - BL_mv->y;
    f32 width_st = UR_st->x - BL_st->x;
    f32 height_st = UR_st->y - BL_st->y;

    /* Additional parameters. */
    b32 is_collision = false;
    f32 xc_mv, yc_mv;  /* Coordinates of the center of the moving object. */
    f32 xc_st, yc_st;  /* Coordinates of the center of the static object. */
    f32 alpha;  /* Angle between block center and ball center. */
    f32 UR_angle;  /* Critical up-right angle (with respect to static object). */
    f32 DR_angle;  /* Critical down-right angle (with respect to static object). */
    f32 UL_angle;  /* Critical up-left angle (with respect to static object). */
    f32 DL_angle;  /* Critical down-left angle (with respect to static object). */

    /* Check the fact of the collision. */
    if ((BL_mv->x > (BL_st->x - width_mv)) &&
        (UR_mv->x < (UR_st->x + width_mv)) &&
        (BL_mv->y > (BL_st->y - height_mv)) &&
        (UR_mv->y < (UR_st->y + height_mv))) 
    {
        is_collision = true;
    }

    if (is_collision) 
    {
        /* Calculate the centers coordinates. */
        xc_st = BL_st->x + (width_st / 2.0f);
        yc_st = BL_st->y + (height_st / 2.0f);

        xc_mv = BL_mv->x + (width_mv / 2.0f);
        yc_mv = BL_mv->y + (height_mv / 2.0f);

        /* Check the angle and side of the collision. */
        alpha = atanf((yc_mv - yc_st) / (xc_mv - xc_st));

        if (alpha > 0) 
            if (xc_mv < xc_st) 
                alpha = alpha + M_PI;
        else 
            if (xc_mv > xc_st) 
                alpha = alpha + 2.0f * M_PI;
            else 
                alpha = alpha + M_PI;

        /* Calculate the critical values of angles. */
        UR_angle = atanf((height_st + height_mv) / (width_st + width_mv));
        DR_angle = 2 * M_PI - UR_angle;
        UL_angle = M_PI - UR_angle;
        DL_angle = UR_angle + M_PI;

        /* alpha >= DR_angle and alpha < UR_angle. */
        if (alpha >= DR_angle) 
            *collision_side = 'r';
        
        if (alpha < UR_angle) 
            *collision_side = 'r';

        /* alpha >= UR_angle and alpha < UL_angle. */
        if ((alpha >= UR_angle) && (alpha < UL_angle)) 
            *collision_side = 't';

        /* alpha >= UL_angle and alpha < DL_angle. */
        if ((alpha >= UL_angle) && (alpha < DL_angle)) 
            *collision_side = 'l';

        /* alpha >= DL_angle and alpha < DR_angle. */
        if ((alpha >= DL_angle) && (alpha < DR_angle)) 
            *collision_side = 'b';
    }
    return is_collision;
}

void
swap_V2_u32(V2_u32_t *vertix_1, V2_u32_t *vertix_2)
{
    V2_u32_t v_temp;

    v_temp.x = vertix_1->x; 
    v_temp.y = vertix_1->y;
    vertix_1->x = vertix_2->x; 
    vertix_1->y = vertix_2->y;      
    vertix_2->x = v_temp.x;
    vertix_2->y = v_temp.y;
}