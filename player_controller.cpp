#include "player_controller.h"
#include "utilities.h"
#include "globals.h"

void PlayerController::move_player_horizontally(float delta) {
    Vector2 current_pos = player.get_pos();
    current_pos.x += delta;
    player.set_pos(current_pos);
    player.set_moving(true);
    player.set_looking_forward(delta > 0.0f);
}

void PlayerController::update_player() {
    Vector2 pos = player.get_pos();
    pos.y += player.get_y_velocity();

    if (get_level_cell(static_cast<size_t>(pos.y + 1), static_cast<size_t>(pos.x)) == WALL) {
        player.set_on_ground(true);
        player.set_y_velocity(0.0f);
    } else {
        player.set_on_ground(false);
        player.set_y_velocity(player.get_y_velocity() + GRAVITY_FORCE);
    }

    player.set_pos(pos);
}

void PlayerController::jump() {
    if (player.is_on_ground()) {
        player.set_y_velocity(-JUMP_STRENGTH);
        player.set_on_ground(false);
    }
}

void PlayerController::kill_player() {
    lives--;
    if (lives > 0) {
        reset({1.0f, static_cast<float>(LEVELS[level_index].rows - 2)});
    }
}