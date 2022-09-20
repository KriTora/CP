// display area
#define screen_width 120
#define screen_height 30

#define playfield_width 32
#define playfield_height 16

// the graphics
#define pixel_void ' '
#define pixel_wall 219

#define pixel_player 235

#define pixel_player_pointer_horizontal 196
#define pixel_player_pointer_vertical 179

#define pixel_generator 206

#define pixel_cable_disconnected 240
#define pixel_cable_horizontal 196
#define pixel_cable_vertical 179
#define pixel_cable_intersection_urdl 197 // up-right-down-left
#define pixel_cable_intersection_ur 192
#define pixel_cable_intersection_rd 218
#define pixel_cable_intersection_dl 191
#define pixel_cable_intersection_lu 217
#define pixel_cable_intersection_lur 193
#define pixel_cable_intersection_urd 195
#define pixel_cable_intersection_rdl 194
#define pixel_cable_intersection_dlu 180

#define pixel_lamp_unpowered 176
#define pixel_lamp_powered 178

// the position of info labels
#define player_position_info_y 2
#define fps_info_y 1

// the text of info labels
#define label_player_position "Player's coordinates: \0";
#define label_fps "FPS: \0"