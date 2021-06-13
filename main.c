#include <gb/gb.h>

#include "maps/bigmap_map.h"
#include "maps/bigmap_tiles.h"

#define camera_max_y ((bigmap_mapHeight - 18) * 8)
#define camera_max_x ((bigmap_mapWidth - 20) * 8)

#define MIN(A, B) ((A) < (B) ? (A) : (B))

UBYTE joy;

// current and old positions of the camera in pixels
UWORD camera_x, camera_y, old_camera_x, old_camera_y;
// current and old position of the map in tiles
UBYTE map_pos_x, map_pos_y, old_map_pos_x, old_map_pos_y;
// redraw flag, indicates that camera position was changed
UBYTE redraw;

UBYTE moving_camera = TRUE;

void set_camera()
{
    // update hardware scroll position
    SCY_REG = camera_y;
    SCX_REG = camera_x;
    // up or down
    map_pos_y = (UINT8)(camera_y / 8);
    if (map_pos_y != old_map_pos_y)
    {
        if (camera_y < old_camera_y)
        {
            // Moved up
            set_bkg_submap(
                map_pos_x,                             // x
                map_pos_y,                             // y
                MIN(21u, bigmap_mapWidth - map_pos_x), // w
                1,                                     // h
                bigmap_map,                            // map data
                bigmap_mapWidth);                      // map width (number of tiles)
        }
        else
        {
            // Moved down
            if ((bigmap_mapHeight - 18u) > map_pos_y)
                set_bkg_submap(
                    map_pos_x,                             // x
                    map_pos_y + 18u,                       // y
                    MIN(21u, bigmap_mapWidth - map_pos_x), // w
                    1,                                     // h
                    bigmap_map,                            // map data
                    bigmap_mapWidth);                      // map width (number of tiles)
        }
        old_map_pos_y = map_pos_y;
    }
    // left or right
    map_pos_x = (UINT8)(camera_x / 8);
    if (map_pos_x != old_map_pos_x)
    {
        if (camera_x < old_camera_x)
        {
            // Moved left
            set_bkg_submap(
                map_pos_x,                              // x
                map_pos_y,                              // y
                1,                                      // w
                MIN(19u, bigmap_mapHeight - map_pos_y), // h
                bigmap_map,                             // map data
                bigmap_mapWidth);                       // map width (number of tiles)
        }
        else
        {
            // Moved right
            if ((bigmap_mapWidth - 20u) > map_pos_x)
                set_bkg_submap(
                    map_pos_x + 20u,                        // x
                    map_pos_y,                              // y
                    1,                                      // w
                    MIN(19u, bigmap_mapHeight - map_pos_y), // h
                    bigmap_map,                             // map data
                    bigmap_mapWidth);                       // map width (number of tiles)
        }
        old_map_pos_x = map_pos_x;
    }
    // set old camera position to current camera position
    old_camera_x = camera_x, old_camera_y = camera_y;
}

void main()
{
    DISPLAY_OFF;
    SHOW_BKG;
    set_bkg_data(0, 241u, bigmap_tiles);

    map_pos_x = map_pos_y = 0;
    old_map_pos_x = old_map_pos_y = 255;
    set_bkg_submap(map_pos_x, map_pos_y, 20, 18, bigmap_map, bigmap_mapWidth);
    DISPLAY_ON;

    camera_x = camera_y = 0;
    old_camera_x = camera_x;
    old_camera_y = camera_y;

    redraw = FALSE;

    SCX_REG = camera_x;
    SCY_REG = camera_y;
    while (TRUE)
    {
        joy = joypad();

        if (moving_camera)
        {
            moving_camera = FALSE;
            for (UINT8 i = 0; i < 8 * 20; i++)
            {
                camera_x += 1;
                wait_vbl_done();
                set_camera();
            }
        }

        // up or down
        if (joy & J_UP)
        {
            if (camera_y)
            {
                camera_y--;
                redraw = TRUE;
            }
        }
        else if (joy & J_DOWN)
        {
            if (camera_y < camera_max_y)
            {
                camera_y++;
                redraw = TRUE;
            }
        }
        // left or right
        if (joy & J_LEFT)
        {
            if (camera_x)
            {
                camera_x--;
                redraw = TRUE;
            }
        }
        else if (joy & J_RIGHT)
        {
            if (camera_x < camera_max_x)
            {
                camera_x++;
                redraw = TRUE;
            }
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