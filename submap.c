#include <gb/gb.h>
#include <stdio.h>

#define MIN(A, B) ((A) < (B) ? (A) : (B))

const UINT8 *level_map;

UINT8 level_map_width, level_map_height;

UINT16 camera_max_x, camera_max_y;

// current and old positions of the camera in pixels
UINT16 camera_x, camera_y, old_camera_x, old_camera_y;

// current and old position of the map in tiles
UINT8 map_pos_x, map_pos_y, old_map_pos_x, old_map_pos_y;

void show_position()
{
    printf("x: %u\ny: %u\n", camera_x, camera_y);
}

void disable_camera_x()
{
    camera_max_x = 0;
}

void disable_camera_y()
{
    camera_max_y = 0;
}

void set_level(UINT8 map_width, UINT8 map_height, const UINT8 *map_tiles, const UINT8 *map, UINT16 start_camera_x, UINT16 start_camera_y)
{
    HIDE_BKG;

    level_map_width = map_width;
    level_map_height = map_height;
    level_map = map;

    camera_max_x = (level_map_width - 20) * 8;
    camera_max_y = (level_map_height - 18) * 8;

    camera_x = start_camera_x;
    camera_y = start_camera_y;
    old_camera_x = camera_x;
    old_camera_y = camera_y;

    map_pos_x = (UINT8)(camera_x >> 3u);
    map_pos_y = (UINT8)(camera_y >> 3u);
    old_map_pos_x = old_map_pos_y = 255;

    set_bkg_data(0, 255u, map_tiles);
    set_bkg_submap(map_pos_x, map_pos_y, 20, 18, level_map, level_map_width);

    SCX_REG = camera_x;
    SCY_REG = camera_y;

    SHOW_BKG;
}

UBYTE move_camera_up()
{
    if (camera_y)
    {
        camera_y--;
        return TRUE;
    }
    return FALSE;
}

UBYTE move_camera_down()
{
    if (camera_y < camera_max_y)
    {
        camera_y++;
        return TRUE;
    }
    return FALSE;
}

UBYTE move_camera_left()
{
    if (camera_x)
    {
        camera_x--;
        return TRUE;
    }
    return FALSE;
}

UBYTE move_camera_right()
{
    if (camera_x < camera_max_x)
    {
        camera_x++;
        return TRUE;
    }
    return FALSE;
}

void set_camera()
{
    // update hardware scroll position
    SCY_REG = camera_y;
    SCX_REG = camera_x;
    // up or down
    map_pos_y = (UINT8)(camera_y >> 3u);
    if (map_pos_y != old_map_pos_y)
    {
        if (camera_y < old_camera_y)
        {
            set_bkg_submap(
                map_pos_x,
                map_pos_y,
                MIN(21u, level_map_width - map_pos_x),
                1,
                level_map, level_map_width);
        }
        else
        {
            if ((level_map_height - 18u) > map_pos_y)
            {
                set_bkg_submap(
                    map_pos_x,
                    map_pos_y + 18u,
                    MIN(21u, level_map_width - map_pos_x),
                    1,
                    level_map,
                    level_map_width);
            }
        }
        old_map_pos_y = map_pos_y;
    }
    // left or right
    map_pos_x = (UINT8)(camera_x >> 3u);
    if (map_pos_x != old_map_pos_x)
    {
        if (camera_x < old_camera_x)
        {
            set_bkg_submap(
                map_pos_x,
                map_pos_y,
                1,
                MIN(19u, level_map_height - map_pos_y),
                level_map,
                level_map_width);
        }
        else
        {
            if ((level_map_width - 20u) > map_pos_x)
            {
                set_bkg_submap(
                    map_pos_x + 20u,
                    map_pos_y,
                    1,
                    MIN(19u, level_map_height - map_pos_y),
                    level_map,
                    level_map_width);
            }
        }
        old_map_pos_x = map_pos_x;
    }
    // set old camera position to current camera position
    old_camera_x = camera_x, old_camera_y = camera_y;
}