void set_level(UINT8 map_width, UINT8 map_height, const UINT8 *map_tiles, const UINT8 *map, UINT16 start_camera_x, UINT16 start_camera_y);

void disable_camera_x();
void disable_camera_y();

UBYTE move_camera_up();
UBYTE move_camera_down();
UBYTE move_camera_left();
UBYTE move_camera_right();

void set_camera();
void show_position();
