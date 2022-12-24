/**
 * ================================================================================
 * @file src_engine/helper_functions.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of the different helper functions.
 * @version 0.2
 * @date 2022-12-05
 * ================================================================================
 */

#define _USE_MATH_DEFINES

#include "include_engine/helper_functions.h"

#include <math.h>
#include <stdlib.h>

#include "include_engine/vector2.h"
#include "include_engine/utils.h"

b32
HelperFcn_CheckCollision(Vec2 *bl_mv, Vec2 *ur_mv, Vec2 *bl_st, Vec2 *ur_st,
   char *collision_side)
{
    /* Calculation the objects widths and heights. */
    f32 width_mv = ur_mv->x - bl_mv->x;
    f32 height_mv = ur_mv->y - bl_mv->y;
    f32 width_st = ur_st->x - bl_st->x;
    f32 height_st = ur_st->y - bl_st->y;
    f32 M_PI_f32 = (f32)M_PI;

    /* Check the fact of the collision. */
    b32 is_collision = false;
    if ((bl_mv->x > (bl_st->x - width_mv)) &&
        (ur_mv->x < (ur_st->x + width_mv)) &&
        (bl_mv->y > (bl_st->y - height_mv)) &&
        (ur_mv->y < (ur_st->y + height_mv))) 
    {
        is_collision = true;
    }

    if (is_collision) 
    {
        /* Calculate the centers coordinates of static (st) and moving (mv) object. */
        f32 xc_st = bl_st->x + (width_st / 2.0f);
        f32 yc_st = bl_st->y + (height_st / 2.0f);
        f32 xc_mv = bl_mv->x + (width_mv / 2.0f);
        f32 yc_mv = bl_mv->y + (height_mv / 2.0f);

        /* Calculate fngle between static object center and moving object center. */
        f32 alpha = atanf((yc_mv - yc_st) / (xc_mv - xc_st));

        if (alpha > 0)
        {
            if (xc_mv < xc_st)
                alpha = alpha + M_PI_f32;
        }
        else
        {
            if (xc_mv > xc_st) 
                alpha = alpha + 2.0f * M_PI_f32;
            else 
                alpha = alpha + M_PI_f32;
        } 
            
        /* Calculate the critical values of angles (with respect to static object). */
        f32 ur_angle = atanf((height_st + height_mv) / (width_st + width_mv));
        f32 dr_angle = 2 * M_PI_f32 - ur_angle;
        f32 ul_angle = M_PI_f32 - ur_angle;
        f32 dl_angle = ur_angle + M_PI_f32;

        /* alpha >= dr_angle and alpha < ur_angle. */
        if (alpha >= dr_angle) 
            *collision_side = 'r';
        
        if (alpha < ur_angle) 
            *collision_side = 'r';

        /* alpha >= ur_angle and alpha < ul_angle. */
        if ((alpha >= ur_angle) && (alpha < ul_angle)) 
            *collision_side = 't';

        /* alpha >= ul_angle and alpha < dl_angle. */
        if ((alpha >= ul_angle) && (alpha < dl_angle)) 
            *collision_side = 'l';

        /* alpha >= dl_angle and alpha < dr_angle. */
        if ((alpha >= dl_angle) && (alpha < dr_angle)) 
            *collision_side = 'b';
    }
    return is_collision;
}
