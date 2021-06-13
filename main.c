#include <gb/gb.h>
#include <stdint.h>

#include "submap.h"

#include "maps/bkg_overworld_map.h"
#include "maps/bkg_overworld_tiles.h"

#include "maps/bkg_apartment_scroll_map.h"
#include "maps/bkg_apartment_scroll_tiles.h"

UINT8 joy;
UINT8 redraw;

void main()
{
    DISPLAY_OFF;

    set_level(BKG_OVERWORLD_MAP_WIDTH, BKG_OVERWORLD_MAP_HEIGHT, bkg_overworld_tiles, bkg_overworld_map);

    DISPLAY_ON;

    redraw = FALSE;

    while (TRUE)
    {
        joy = joypad();
        // up or down
        if (joy & J_UP)
        {
            if (move_camera_up())
            {
                redraw = TRUE;
            }
        }
        else if (joy & J_DOWN)
        {
            if (move_camera_down())
            {
                redraw = TRUE;
            }
        }
        // left or right
        if (joy & J_LEFT)
        {
            if (move_camera_left())
            {
                redraw = TRUE;
            }
        }
        else if (joy & J_RIGHT)
        {
            if (move_camera_right())
            {
                redraw = TRUE;
            }
        }
        else if (joy & J_SELECT)
        {
            set_level(BKG_OVERWORLD_MAP_WIDTH, BKG_OVERWORLD_MAP_HEIGHT, bkg_overworld_tiles, bkg_overworld_map);
        }
        else if (joy & J_START)
        {
            set_level(BKG_APARTMENT_SCROLL_MAP_WIDTH, BKG_APARTMENT_SCROLL_MAP_HEIGHT, bkg_apartment_scroll_tiles, bkg_apartment_scroll_map);
        }
        if (redraw)
        {
            wait_vbl_done();
            set_camera();
            redraw = FALSE;
        }
        else
            wait_vbl_done();
    }
}