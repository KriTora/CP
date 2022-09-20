#include <iostream>

#include "parameters.h"
#include "functions.h"

int main()
{
    char screen[screen_width * screen_height + 1];
    screen[screen_width * screen_height] = '\0';

    int playfield_x_left_limit,
        playfield_y_top_limit;

    get_playfield_limits(&playfield_x_left_limit, &playfield_y_top_limit);

    screen_clean(screen); // only call this when starting the game

    // initial player's coordinates
    int px = screen_width / 2,
        py = screen_height / 2;

    // preparing the first frame
    playfield_draw(screen, &playfield_x_left_limit, &playfield_y_top_limit);
    player_draw(screen, &px, &py);
    player_position_label_draw(screen, &px, &py, &playfield_x_left_limit, &playfield_y_top_limit);
    std::cout << screen;

    bool shouldQuit;
    bool shouldPointOutPlayer = false;
    quit(&shouldQuit);

    int fps = 0, frames = 0;
    int t_prev = time(NULL), t_now = time(NULL);

    Net net;

    while (!shouldQuit)
    {
        // fps count
        frames++;
        if (time_check(&t_prev, &t_now))
        {
            fps = frames;
            frames = 0;
            
            fps_label_draw(screen, &fps);
            std::cout << screen; // need to draw new frame so the updated fps is visible
        }

        // controls
        if (process_input(&playfield_x_left_limit, &playfield_y_top_limit, &px, &py, &shouldQuit, &shouldPointOutPlayer, &net))
        {
            // if process_input() returns true then generate new frame
            playfield_clean(screen, &playfield_x_left_limit, &playfield_y_top_limit);
            net.draw_objects(screen);
            net.connect_cables();
            net.cables_power_up();
            net.power_up();

            // draw everything player-related last so no other object covers the player
            player_draw(screen, &px, &py);

            if (shouldPointOutPlayer) player_pointer_draw(screen, &px, &py, &playfield_x_left_limit, &playfield_y_top_limit);
            player_position_label_draw(screen, &px, &py, &playfield_x_left_limit, &playfield_y_top_limit);

            std::cout << screen;

            Sleep(100); // so things don't happen waaaaay too quick
        };
    }

    return 0;
}