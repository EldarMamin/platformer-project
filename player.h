#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

class Player {
public:
    Player(const Vector2& pos)
        : pos(pos), y_velocity(0.0f), looking_forward(true), moving(false), on_ground(false) {}

    [[nodiscard]] Vector2 get_pos() const {
        return pos;
    }

    void set_pos(const Vector2& new_pos) {
        pos = new_pos;
    }

    [[nodiscard]] float get_y_velocity() const {
        return y_velocity;
    }

    void set_y_velocity(float velocity) {
        y_velocity = velocity;
    }

    [[nodiscard]] bool is_looking_forward() const {
        return looking_forward;
    }

    void set_looking_forward(bool looking) {
        looking_forward = looking;
    }

    [[nodiscard]] bool is_moving() const {
        return moving;
    }

    void set_moving(bool move) {
        moving = move;
    }

    [[nodiscard]] bool is_on_ground() const {
        return on_ground;
    }

    void set_on_ground(bool on_ground_state) {
        on_ground = on_ground_state;
    }

    void reset(const Vector2& start_pos) {
        pos = start_pos;
        y_velocity = 0.0f;
        looking_forward = true;
        moving = false;
        on_ground = false;
    }

private:
    Vector2 pos;
    float y_velocity;
    bool looking_forward;
    bool moving;
    bool on_ground;
};

#endif // PLAYER_H