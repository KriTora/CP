#include "functions.h"

void get_playfield_limits(int* xl, int* yl)
{
    // calculates where the playfield can be drawn

    *xl = screen_width / 2 - playfield_width / 2,
        * yl = screen_height / 2 - playfield_height / 2;
}

void screen_clean(char* s)
{
    // cleans the screen so that no random symbols would show up

    for (int i = 0; i < screen_width; i++)
        for (int j = 0; j < screen_height; j++)
            s[i + j * screen_width] = pixel_void;
}

void playfield_clean(char* s, int* xl, int* yl)
{
    // cleans only playfield as a preparation for drawing a new frame

    for (int i = *xl; i < *xl + playfield_width; i++)
        for (int j = *yl; j < *yl + playfield_height; j++)
            s[i + j * screen_width] = pixel_void;
}

void playfield_draw(char* s, int* xl, int* yl)
{
    // draws the playfield in the middle of the screen

    // x/yl - 1 because x/yl represent where the playfield should be, not the wall
    for (int i = *xl - 1; i <= *xl + playfield_width; i++)
        for (int j = *yl - 1; j <= *yl + playfield_height; j++)
        {
            // right/left walls
            if ((i == *xl - 1 || i == *xl + playfield_width) && (j >= *yl && j <= *yl + playfield_height))
                s[i + j * screen_width] = pixel_wall;

            // top/bottom walls
            if ((j == *yl - 1 || j == *yl + playfield_height) && (i >= *xl && i <= *xl + playfield_width))
                s[i + j * screen_width] = pixel_wall;
        }
}

void player_draw(char* s, const int* px, const int* py)
{
    // draws player in provided position
    s[*px + *py * screen_width] = pixel_player;
}

bool process_input(int* xl, int* yl, int* px, int* py, bool* shouldQuit, bool* shouldPointOutPlayer, Net* net)
{
    // processes anything and everything related to in-game controls
    // if returns true then new frame will be made

    // service buttons
    if (GetAsyncKeyState(VK_ESCAPE) & 0X8000) quit(shouldQuit, true); // quit the game
    if (GetAsyncKeyState(0x31) & 0X8000)
    {
        // toggle player pointer
        if (!*shouldPointOutPlayer) *shouldPointOutPlayer = true;
        else *shouldPointOutPlayer = false;
        return true;
    }

    // move in 4 directions
    if (GetAsyncKeyState(VK_UP) & 0X8000)
    {
        if (*py - 1 >= *yl) *py -= 1;
        return true;
    }
    if (GetAsyncKeyState(VK_DOWN) & 0X8000)
    {
        if (*py + 1 <= *yl + playfield_height - 1) *py += 1;
        return true;
    }
    if (GetAsyncKeyState(VK_LEFT) & 0X8000)
    {
        if (*px - 1 >= *xl) *px -= 1;
        return true;
    }
    if (GetAsyncKeyState(VK_RIGHT) & 0X8000)
    {
        if (*px <= *xl + playfield_width - 2) *px += 1;
        return true;
    }

    // place and remove objects
    if (GetAsyncKeyState(0x47) & 0X8000)
    {
        // place a generator
        net->place_object(px, py, pixel_generator, true, false, 1);
        return true;
    }
    if (GetAsyncKeyState(0x43) & 0X8000)
    {
        // place a cable
        net->place_object(px, py, pixel_cable_disconnected, false, true, 2); // 4th argument must be false because new cable state is unpowered; 5th argument is true so the game knows this object is a cable
        return true;
    }
    if (GetAsyncKeyState(0x4C) & 0X8000)
    {
        // place a lamp
        net->place_object(px, py, pixel_lamp_unpowered, false, false, 3);
        return true;
    }
    if (GetAsyncKeyState(0x44) & 0X8000)
    {
        net->destroy_object(px, py);
        return true;
    }

    return false;
}

void quit(bool* q, bool flag)
{
    // sets the quit variable to true if the flag is true
    if (!flag) *q = false;
    else *q = true;
}

void player_pointer_draw(char* s, int* px, int* py, int* xl, int* yl)
{
    for (int i = *xl; i < *xl + playfield_width; i++)
        for (int j = *yl; j < *yl + playfield_height; j++)
        {
            // horizontal pointer
            if (j == *py && i != *px) s[i + j * screen_width] = pixel_player_pointer_horizontal;

            // vertical pointer
            if (i == *px && j != *py) s[i + j * screen_width] = pixel_player_pointer_vertical;
        }
}

void player_position_label_draw(char* s, int* px, int* py, int* xoffset, int* yoffset)
{
    // draws the player position label; displays coordinates with the playfield's offset so top left corner is (0; 0), not (n; n)
    std::string l = label_player_position;
    std::string x = "[" + std::to_string(*px - *xoffset) + "; ",
                y = std::to_string(*py - *yoffset) + "]";

    // cleaning label field
    for (int i = 0; i < l.length() * 1.5; i++) s[i + player_position_info_y * screen_width] = ' ';

    // filling it with actual info
    // the label itself
    for (int i = 0; i < l.length(); i++) s[i + player_position_info_y * screen_width] = l[i];

    // the x coordinate
    for (int i = l.length(), j = 0; i < l.length() + x.length() - 1; i++, j++) s[i + player_position_info_y * screen_width] = x[j];

    // the y coordinate
    for (int i = l.length() + x.length(), j = 0; i < l.length() + x.length() + y.length(); i++, j++) s[i + player_position_info_y * screen_width] = y[j];
}

bool time_check(int* time_prev, int* time_now)
{
    // if returns true, then 1 second has passed since last time change and framerate must be updated
    *time_now = time(NULL);
    if (*time_now - *time_prev == 1)
    {
        *time_prev = *time_now;
        return true;
    }
        return false;
}

void fps_label_draw(char* s, int* fps)
{
    // draws the fps in top left corner
    std::string l = label_fps + std::to_string(*fps);

    // cleaning the field
    for (int i = 0; i < l.length() * 2; i++) s[i + fps_info_y * screen_width] = ' ';

    for (int i = 0; i < l.length(); i++) s[i + fps_info_y * screen_width] = l[i];
}