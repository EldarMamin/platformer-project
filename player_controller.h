#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "player.h"

class PlayerController {
public:
    static PlayerController& get_instance() {
        static PlayerController instance;
        return instance;
    }

    PlayerController(const PlayerController&) = delete;
    PlayerController& operator=(const PlayerController&) = delete;
    PlayerController(PlayerController&&) = delete;
    PlayerController& operator=(PlayerController&&) = delete;

    Vector2 get_position() const {
        return player.get_pos();
    }

    void set_position(const Vector2& pos) {
        player.set_pos(pos);
    }

    float get_y_velocity() const {
        return player.get_y_velocity();
    }

    void set_y_velocity(float velocity) {
        player.set_y_velocity(velocity);
    }

    bool is_on_ground() const {
        return player.is_on_ground();
    }

    void set_on_ground(bool on_ground) {
        player.set_on_ground(on_ground);
    }

    bool is_moving_now() const {
        return player.is_moving();
    }

    void set_moving_now(bool moving) {
        player.set_moving(moving);
    }

    bool is_moving_forward() const {
        return player.is_looking_forward();
    }

    void set_moving_forward(bool looking) {
        player.set_looking_forward(looking);
    }

    int get_lives() const {
        return lives;
    }

    void set_lives(int new_lives) {
        lives = new_lives;
    }

    int get_total_player_score() const {
        return total_score;
    }

    void add_score(int score) {
        total_score += score;
    }

    void update_player_gravity() {
        update_player();
    }

    void reset(const Vector2& start_pos) {
        lives = 3;
        total_score = 0;
        player.reset(start_pos);
    }

    void move_player_horizontally(float delta);
    void update_player();
    void jump();
    void kill_player();

private:
    PlayerController() : player({0.0f, 0.0f}), lives(3), total_score(0) {}

    ~PlayerController() = default;

    Player player;
    int lives;
    int total_score;
};

#endif // PLAYER_CONTROLLER_H