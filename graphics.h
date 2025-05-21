#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "enemies_controller.h"
#include "player_controller.h"
#include "globals.h"

extern Player player;

void draw_text(Text& text) {
    Vector2 dimensions = MeasureTextEx(*text.font, text.str.c_str(), text.size * screen_scale, text.spacing);
    Vector2 pos = {
        (screen_size.x * text.position.x) - (0.5f * dimensions.x),
        (screen_size.y * text.position.y) - (0.5f * dimensions.y)
    };
    DrawTextEx(*text.font, text.str.c_str(), pos, dimensions.y, text.spacing, text.color);
}

void derive_graphics_metrics_from_loaded_level() {
    screen_size.x = static_cast<float>(GetScreenWidth());
    screen_size.y = static_cast<float>(GetScreenHeight());

    cell_size = screen_size.y / static_cast<float>(LEVELS[level_index].rows);
    screen_scale = std::min(screen_size.x, screen_size.y) / SCREEN_SCALE_DIVISOR;

    float larger_screen_side = std::max(screen_size.x, screen_size.y);

    if (screen_size.x > screen_size.y) {
        background_size = {larger_screen_side, larger_screen_side / 16 * 10};
    } else {
        background_size = {larger_screen_side / 10 * 16, larger_screen_side};
    }
    background_y_offset = (screen_size.y - background_size.y) * 0.5f;
}

void draw_game_overlay() {
    const float ICON_SIZE = 48.0f * screen_scale;

    float slight_vertical_offset = 8.0f;
    slight_vertical_offset *= screen_scale;

    // Hearts
    for (int i = 0; i < PlayerController::get_instance().get_lives(); i++) {
        const float SPACE_BETWEEN_HEARTS = 4.0f * screen_scale;
        draw_image(heart_image, {ICON_SIZE * i + SPACE_BETWEEN_HEARTS, slight_vertical_offset}, ICON_SIZE);
    }

    // Timer
    Vector2 timer_dimensions = MeasureTextEx(menu_font, std::to_string(timer / 60).c_str(), ICON_SIZE, 2.0f);
    Vector2 timer_position = {(GetRenderWidth() - timer_dimensions.x) * 0.5f, slight_vertical_offset};
    DrawTextEx(menu_font, std::to_string(timer / 60).c_str(), timer_position, ICON_SIZE, 2.0f, WHITE);

    // Score
    Vector2 score_dimensions = MeasureTextEx(menu_font, std::to_string(get_total_player_score()).c_str(), ICON_SIZE, 2.0f);
    Vector2 score_position = {GetRenderWidth() - score_dimensions.x - ICON_SIZE, slight_vertical_offset};
    DrawTextEx(menu_font, std::to_string(get_total_player_score()).c_str(), score_position, ICON_SIZE, 2.0f, WHITE);
    draw_sprite(coin_sprite, {GetRenderWidth() - ICON_SIZE, slight_vertical_offset}, ICON_SIZE);
}


void draw_parallax_background() {
    // First uses the player's position
    float initial_offset = -(PlayerController::get_instance().get_position().x * PARALLAX_PLAYER_SCROLLING_SPEED + game_frame * PARALLAX_IDLE_SCROLLING_SPEED);

    // Calculate offsets for different layers
    float background_offset   = initial_offset;
    float middleground_offset = background_offset * PARALLAX_LAYERED_SPEED_DIFFERENCE;
    float foreground_offset   = middleground_offset * PARALLAX_LAYERED_SPEED_DIFFERENCE;

    // Wrap offsets to create a loop effect
    background_offset   = fmod(background_offset, 1.0f);
    middleground_offset = fmod(middleground_offset, 1.0f);
    foreground_offset   = fmod(foreground_offset, 1.0f);

    // Scale to background size
    background_offset   *= background_size.x;
    middleground_offset *= background_size.x;
    foreground_offset   *= background_size.x;

    // Each layer is drawn twice, side by side, the first starting from its offset, and the other from its offset + background_size
    // This ensures a seamless scrolling effect, because when one copy moves out of sight, the second jumps into its place.
    draw_image(background,   {background_offset + background_size.x, background_y_offset},   background_size.x, background_size.y);
    draw_image(background,   {background_offset,                     background_y_offset},   background_size.x, background_size.y);

    draw_image(middleground, {middleground_offset + background_size.x, background_y_offset}, background_size.x, background_size.y);
    draw_image(middleground, {middleground_offset,                     background_y_offset}, background_size.x, background_size.y);

    draw_image(foreground,   {foreground_offset + background_size.x, background_y_offset},   background_size.x, background_size.y);
    draw_image(foreground,   {foreground_offset,                     background_y_offset},   background_size.x, background_size.y);
}

void draw_level() {
    horizontal_shift = (screen_size.x - cell_size) / 2;

    for (size_t row = 0; row < current_level.rows; ++row) {
        for (size_t column = 0; column < current_level.columns; ++column) {
            Vector2 pos = {
                (static_cast<float>(column) - PlayerController::get_instance().get_position().x)
                * cell_size + horizontal_shift,
                static_cast<float>(row) * cell_size
            };

            char cell = get_level_cell(row, column);
            switch (cell) {
                case WALL:
                    draw_image(wall_image, pos, cell_size);
                    break;
                case WALL_DARK:
                    draw_image(wall_dark_image, pos, cell_size);
                    break;
                case SPIKE:
                    draw_image(spike_image, pos, cell_size);
                    break;
                case COIN:
                    draw_sprite(coin_sprite, pos, cell_size);
                    break;
                case EXIT:
                    draw_image(exit_image, pos, cell_size);
                    break;
                default:
                    break;
            }
        }
    }

    draw_player();
    draw_enemies();
}

void draw_player() {
    horizontal_shift = (screen_size.x - cell_size) / 2;

    Vector2 pos = {
        horizontal_shift,
        PlayerController::get_instance().get_position().y * cell_size
    };

    if (game_state == GAME_STATE) {
        if (!PlayerController::get_instance().is_on_ground()) {
            draw_image(
                (PlayerController::get_instance().is_moving_forward()
                    ? player_jump_forward_image
                    : player_jump_backwards_image),
                pos, cell_size);
        } else if (PlayerController::get_instance().is_moving_now()) {
            draw_sprite(
                (PlayerController::get_instance().is_moving_forward()
                    ? player_walk_forward_sprite
                    : player_walk_backwards_sprite),
                pos, cell_size);
        } else {
            draw_image(
                (PlayerController::get_instance().is_moving_forward()
                    ? player_stand_forward_image
                    : player_stand_backwards_image),
                pos, cell_size);
        }
    } else {
        draw_image(player_dead_image, pos, cell_size);
    }
}

void draw_enemies() {
    for (auto& enemy : EnemiesController::get_instance().get_enemies()) {
        horizontal_shift = (screen_size.x - cell_size) / 2;

        Vector2 pos = {
            (enemy.get_pos().x - PlayerController::get_instance().get_position().x) * cell_size + horizontal_shift,
            enemy.get_pos().y * cell_size
        };

        draw_sprite(enemy_walk, pos, cell_size);
    }
}

#endif // GRAPHICS_H