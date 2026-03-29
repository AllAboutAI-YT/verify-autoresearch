#include <SDL2/SDL.h>
#include <cmath>
#include <vector>
#include <cstdio>
#include <cstring>

// ── Tiny 5x7 bitmap font ────────────────────────────────────────────────────
// Each glyph is 5 columns x 7 rows, stored as 7 bytes (LSB = left col)
static const unsigned char FONT_DATA[][7] = {
    // ' ' (32)
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    // '!' (33)
    {0x04,0x04,0x04,0x04,0x04,0x00,0x04},
    // '"' (34)
    {0x0A,0x0A,0x00,0x00,0x00,0x00,0x00},
    // '#' (35)
    {0x0A,0x1F,0x0A,0x0A,0x1F,0x0A,0x00},
    // '$' (36)
    {0x04,0x0F,0x14,0x0E,0x05,0x1E,0x04},
    // '%' (37)
    {0x19,0x1A,0x02,0x04,0x0B,0x13,0x00},
    // '&' (38)
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    // '\'' (39)
    {0x04,0x04,0x00,0x00,0x00,0x00,0x00},
    // '(' (40)
    {0x02,0x04,0x08,0x08,0x08,0x04,0x02},
    // ')' (41)
    {0x08,0x04,0x02,0x02,0x02,0x04,0x08},
    // '*' (42)
    {0x00,0x04,0x15,0x0E,0x15,0x04,0x00},
    // '+' (43)
    {0x00,0x04,0x04,0x1F,0x04,0x04,0x00},
    // ',' (44)
    {0x00,0x00,0x00,0x00,0x00,0x04,0x08},
    // '-' (45)
    {0x00,0x00,0x00,0x1F,0x00,0x00,0x00},
    // '.' (46)
    {0x00,0x00,0x00,0x00,0x00,0x00,0x04},
    // '/' (47)
    {0x01,0x02,0x02,0x04,0x08,0x08,0x10},
    // '0' (48)
    {0x0E,0x11,0x13,0x15,0x19,0x11,0x0E},
    // '1' (49)
    {0x04,0x0C,0x04,0x04,0x04,0x04,0x0E},
    // '2' (50)
    {0x0E,0x11,0x01,0x06,0x08,0x10,0x1F},
    // '3' (51)
    {0x0E,0x11,0x01,0x06,0x01,0x11,0x0E},
    // '4' (52)
    {0x02,0x06,0x0A,0x12,0x1F,0x02,0x02},
    // '5' (53)
    {0x1F,0x10,0x1E,0x01,0x01,0x11,0x0E},
    // '6' (54)
    {0x06,0x08,0x10,0x1E,0x11,0x11,0x0E},
    // '7' (55)
    {0x1F,0x01,0x02,0x04,0x08,0x08,0x08},
    // '8' (56)
    {0x0E,0x11,0x11,0x0E,0x11,0x11,0x0E},
    // '9' (57)
    {0x0E,0x11,0x11,0x0F,0x01,0x02,0x0C},
    // ':' (58)
    {0x00,0x00,0x04,0x00,0x04,0x00,0x00},
    // ';' (59)
    {0x00,0x00,0x04,0x00,0x04,0x04,0x08},
    // '<' (60)
    {0x02,0x04,0x08,0x10,0x08,0x04,0x02},
    // '=' (61)
    {0x00,0x00,0x1F,0x00,0x1F,0x00,0x00},
    // '>' (62)
    {0x08,0x04,0x02,0x01,0x02,0x04,0x08},
    // '?' (63)
    {0x0E,0x11,0x01,0x02,0x04,0x00,0x04},
    // '@' (64)
    {0x0E,0x11,0x17,0x15,0x17,0x10,0x0E},
    // A-Z (65-90)
    {0x0E,0x11,0x11,0x1F,0x11,0x11,0x11}, // A
    {0x1E,0x11,0x11,0x1E,0x11,0x11,0x1E}, // B
    {0x0E,0x11,0x10,0x10,0x10,0x11,0x0E}, // C
    {0x1E,0x11,0x11,0x11,0x11,0x11,0x1E}, // D
    {0x1F,0x10,0x10,0x1E,0x10,0x10,0x1F}, // E
    {0x1F,0x10,0x10,0x1E,0x10,0x10,0x10}, // F
    {0x0E,0x11,0x10,0x17,0x11,0x11,0x0E}, // G
    {0x11,0x11,0x11,0x1F,0x11,0x11,0x11}, // H
    {0x0E,0x04,0x04,0x04,0x04,0x04,0x0E}, // I
    {0x07,0x02,0x02,0x02,0x02,0x12,0x0C}, // J
    {0x11,0x12,0x14,0x18,0x14,0x12,0x11}, // K
    {0x10,0x10,0x10,0x10,0x10,0x10,0x1F}, // L
    {0x11,0x1B,0x15,0x11,0x11,0x11,0x11}, // M
    {0x11,0x19,0x15,0x13,0x11,0x11,0x11}, // N
    {0x0E,0x11,0x11,0x11,0x11,0x11,0x0E}, // O
    {0x1E,0x11,0x11,0x1E,0x10,0x10,0x10}, // P
    {0x0E,0x11,0x11,0x11,0x15,0x12,0x0D}, // Q
    {0x1E,0x11,0x11,0x1E,0x14,0x12,0x11}, // R
    {0x0E,0x11,0x10,0x0E,0x01,0x11,0x0E}, // S
    {0x1F,0x04,0x04,0x04,0x04,0x04,0x04}, // T
    {0x11,0x11,0x11,0x11,0x11,0x11,0x0E}, // U
    {0x11,0x11,0x11,0x11,0x0A,0x0A,0x04}, // V
    {0x11,0x11,0x11,0x15,0x15,0x1B,0x11}, // W
    {0x11,0x11,0x0A,0x04,0x0A,0x11,0x11}, // X
    {0x11,0x11,0x0A,0x04,0x04,0x04,0x04}, // Y
    {0x1F,0x01,0x02,0x04,0x08,0x10,0x1F}, // Z
};

void draw_char(SDL_Renderer* r, int x, int y, char c, int scale) {
    int idx = -1;
    if (c >= 32 && c <= 90) idx = c - 32;
    else if (c >= 'a' && c <= 'z') idx = c - 'a' + ('A' - 32);
    else return;
    if (idx < 0 || idx >= (int)(sizeof(FONT_DATA)/sizeof(FONT_DATA[0]))) return;
    for (int row = 0; row < 7; row++) {
        unsigned char bits = FONT_DATA[idx][row];
        for (int col = 0; col < 5; col++) {
            if (bits & (0x10 >> col)) {
                SDL_Rect px = {x + col * scale, y + row * scale, scale, scale};
                SDL_RenderFillRect(r, &px);
            }
        }
    }
}

void draw_text(SDL_Renderer* r, int x, int y, const char* text, int scale = 2) {
    for (int i = 0; text[i]; i++) {
        draw_char(r, x + i * 6 * scale, y, text[i], scale);
    }
}

// ── Game constants ───────────────────────────────────────────────────────────
const int WIN_W = 900;
const int WIN_H = 600;
const int MARGIN_LEFT = 60;
const int MARGIN_RIGHT = 30;
const int MARGIN_TOP = 60;
const int MARGIN_BOTTOM = 50;
const int CANVAS_W = WIN_W - MARGIN_LEFT - MARGIN_RIGHT;
const int CANVAS_H = WIN_H - MARGIN_TOP - MARGIN_BOTTOM;

const double GAME_X_MIN = 0.0, GAME_X_MAX = 100.0;
const double GAME_Y_MIN = -10.0, GAME_Y_MAX = 10.0;

struct InputRecord {
    double time_x;
    const char* key_name;
    double ball_y;
    double target_y;
};

double curve(double x) {
    return 3.0 * sin(x * M_PI / 18.0)
         + 2.0 * sin(x * M_PI / 7.0 + 0.8)
         + 1.5 * cos(x * M_PI / 11.0 - 0.3)
         + 1.2 * sin(x * M_PI / 4.5 + 1.2)
         + 0.8 * cos(x * M_PI / 3.0 - 0.7)
         + 0.6 * sin(x * M_PI / 2.2 + 2.0)
         + 0.4 * cos(x * M_PI / 1.5 + 0.5);
}

int to_px_x(double gx) {
    return MARGIN_LEFT + (int)((gx - GAME_X_MIN) / (GAME_X_MAX - GAME_X_MIN) * CANVAS_W);
}

int to_px_y(double gy) {
    return MARGIN_TOP + CANVAS_H - (int)((gy - GAME_Y_MIN) / (GAME_Y_MAX - GAME_Y_MIN) * CANVAS_H);
}

double dist_sq_point_to_segment(double px, double py,
                                double ax, double ay,
                                double bx, double by) {
    double abx = bx - ax;
    double aby = by - ay;
    double apx = px - ax;
    double apy = py - ay;
    double ab_len_sq = abx * abx + aby * aby;
    double t = 0.0;
    if (ab_len_sq > 0.0) {
        t = (apx * abx + apy * aby) / ab_len_sq;
        if (t < 0.0) t = 0.0;
        if (t > 1.0) t = 1.0;
    }

    double closest_x = ax + t * abx;
    double closest_y = ay + t * aby;
    double dx = px - closest_x;
    double dy = py - closest_y;
    return dx * dx + dy * dy;
}

bool circle_hits_curve(int center_x, int center_y, int radius, int extra_tolerance = 1) {
    int min_col = center_x - MARGIN_LEFT - radius - 2;
    int max_col = center_x - MARGIN_LEFT + radius + 2;
    if (max_col < 0 || min_col >= CANVAS_W - 1) return false;
    if (min_col < 0) min_col = 0;
    if (max_col > CANVAS_W - 2) max_col = CANVAS_W - 2;

    int hit_radius = radius + extra_tolerance;
    double hit_radius_sq = (double)hit_radius * hit_radius;

    for (int col = min_col; col <= max_col; col++) {
        double gx0 = GAME_X_MIN + (double)col / CANVAS_W * (GAME_X_MAX - GAME_X_MIN);
        double gx1 = GAME_X_MIN + (double)(col + 1) / CANVAS_W * (GAME_X_MAX - GAME_X_MIN);
        double x0 = MARGIN_LEFT + col;
        double y0 = to_px_y(curve(gx0));
        double x1 = MARGIN_LEFT + col + 1;
        double y1 = to_px_y(curve(gx1));

        if (dist_sq_point_to_segment(center_x, center_y, x0, y0, x1, y1) <= hit_radius_sq) {
            return true;
        }
    }

    return false;
}

int main(int, char**) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("Ball On Line",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H, 0);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    double ball_y = curve(0.0);
    double time_x = 0.0;
    double speed = 0.4;
    double move_step = 0.3;
    // No tolerance - ball pixel must match curve pixel

    int samples_total = 0, samples_on_line = 0;
    double last_sample_x = -1.0;
    const double SAMPLE_STEP = 0.5; // sample every 0.5 units of X movement
    std::vector<InputRecord> inputs;

    enum State { TITLE, PLAYING, GAMEOVER } state = TITLE;
    bool quit = false;

    Uint32 last_tick = SDL_GetTicks();
    const int TICK_MS = 33; // ~30 fps

    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) { quit = true; }
            if (e.type == SDL_KEYDOWN) {
                SDL_Keycode k = e.key.keysym.sym;
                if (state == TITLE) {
                    state = PLAYING;
                    last_tick = SDL_GetTicks();
                } else if (state == PLAYING) {
                    double target = curve(time_x);
                    if (k == SDLK_UP) {
                        ball_y += move_step;
                        if (ball_y > GAME_Y_MAX) ball_y = GAME_Y_MAX;
                        inputs.push_back({time_x, "UP", ball_y, target});
                    } else if (k == SDLK_DOWN) {
                        ball_y -= move_step;
                        if (ball_y < GAME_Y_MIN) ball_y = GAME_Y_MIN;
                        inputs.push_back({time_x, "DOWN", ball_y, target});
                    } else if (k == SDLK_RIGHT) {
                        time_x += move_step;
                        if (time_x >= GAME_X_MAX) {
                            time_x = GAME_X_MAX;
                            state = GAMEOVER;
                        }
                        inputs.push_back({time_x, "RIGHT", ball_y, curve(time_x)});
                    } else if (k == SDLK_ESCAPE || k == SDLK_q) {
                        state = GAMEOVER;
                    }
                } else if (state == GAMEOVER) {
                    if (k == SDLK_r) {
                        // Restart
                        ball_y = curve(0.0);
                        time_x = 0.0;
                        samples_total = samples_on_line = 0;
                        last_sample_x = -1.0;
                        inputs.clear();
                        state = PLAYING;
                        last_tick = SDL_GetTicks();
                    } else if (k == SDLK_ESCAPE || k == SDLK_q) {
                        quit = true;
                    }
                }
            }
        }

        // Handle held keys for smooth movement
        if (state == PLAYING) {
            const Uint8* keys = SDL_GetKeyboardState(NULL);
            if (keys[SDL_SCANCODE_UP]) {
                ball_y += move_step * 0.3;
                if (ball_y > GAME_Y_MAX) ball_y = GAME_Y_MAX;
            }
            if (keys[SDL_SCANCODE_DOWN]) {
                ball_y -= move_step * 0.3;
                if (ball_y < GAME_Y_MIN) ball_y = GAME_Y_MIN;
            }
            if (keys[SDL_SCANCODE_RIGHT]) {
                time_x += speed * 0.3;
                if (time_x >= GAME_X_MAX) {
                    time_x = GAME_X_MAX;
                    state = GAMEOVER;
                }
            }
        }

        // Sample accuracy based on X position advancement
        if (state == PLAYING && time_x - last_sample_x >= SAMPLE_STEP) {
            while (last_sample_x + SAMPLE_STEP <= time_x) {
                last_sample_x += SAMPLE_STEP;
                samples_total++;
                int sbx = to_px_x(last_sample_x);
                int sby = to_px_y(ball_y);
                if (circle_hits_curve(sbx, sby, 4)) samples_on_line++;
            }
        }

        // ── RENDER ────────────────────────────────────────────────────
        SDL_SetRenderDrawColor(ren, 20, 20, 30, 255);
        SDL_RenderClear(ren);

        if (state == TITLE) {
            SDL_SetRenderDrawColor(ren, 100, 200, 255, 255);
            draw_text(ren, WIN_W/2 - 150, WIN_H/2 - 80, "BALL ON LINE", 3);
            SDL_SetRenderDrawColor(ren, 180, 180, 180, 255);
            draw_text(ren, WIN_W/2 - 190, WIN_H/2 - 20, "KEEP THE BALL ON THE CURVE", 2);
            draw_text(ren, WIN_W/2 - 170, WIN_H/2 + 20, "USE UP/DOWN ARROWS TO MOVE", 2);
            SDL_SetRenderDrawColor(ren, 255, 255, 100, 255);
            draw_text(ren, WIN_W/2 - 130, WIN_H/2 + 80, "PRESS ANY KEY TO START", 2);
            SDL_RenderPresent(ren);
            continue;
        }

        // Draw canvas background
        SDL_SetRenderDrawColor(ren, 30, 30, 45, 255);
        SDL_Rect canvas = {MARGIN_LEFT, MARGIN_TOP, CANVAS_W, CANVAS_H};
        SDL_RenderFillRect(ren, &canvas);

        // Grid lines
        SDL_SetRenderDrawColor(ren, 50, 50, 65, 255);
        for (double gx = 0; gx <= 100; gx += 10) {
            int px = to_px_x(gx);
            SDL_RenderDrawLine(ren, px, MARGIN_TOP, px, MARGIN_TOP + CANVAS_H);
        }
        for (double gy = GAME_Y_MIN; gy <= GAME_Y_MAX; gy += 2.0) {
            int py = to_px_y(gy);
            SDL_RenderDrawLine(ren, MARGIN_LEFT, py, MARGIN_LEFT + CANVAS_W, py);
        }

        // Axes
        SDL_SetRenderDrawColor(ren, 100, 100, 120, 255);
        int zero_y = to_px_y(0.0);
        SDL_RenderDrawLine(ren, MARGIN_LEFT, zero_y, MARGIN_LEFT + CANVAS_W, zero_y);
        SDL_RenderDrawLine(ren, MARGIN_LEFT, MARGIN_TOP, MARGIN_LEFT, MARGIN_TOP + CANVAS_H);

        // Axis labels
        SDL_SetRenderDrawColor(ren, 150, 150, 170, 255);
        for (double gx = 0; gx <= 100; gx += 20) {
            char buf[8]; snprintf(buf, sizeof(buf), "%.0f", gx);
            draw_text(ren, to_px_x(gx) - 4, MARGIN_TOP + CANVAS_H + 8, buf, 1);
        }
        for (double gy = GAME_Y_MIN; gy <= GAME_Y_MAX; gy += 5.0) {
            char buf[8]; snprintf(buf, sizeof(buf), "%+.0f", gy);
            draw_text(ren, 5, to_px_y(gy) - 4, buf, 1);
        }

        // Draw the curve as connected segments so the visuals match hit detection.
        SDL_SetRenderDrawColor(ren, 255, 220, 50, 255);
        for (int px = 0; px < CANVAS_W - 1; px++) {
            double gx0 = GAME_X_MIN + (double)px / CANVAS_W * (GAME_X_MAX - GAME_X_MIN);
            double gx1 = GAME_X_MIN + (double)(px + 1) / CANVAS_W * (GAME_X_MAX - GAME_X_MIN);
            int py0 = to_px_y(curve(gx0));
            int py1 = to_px_y(curve(gx1));
            SDL_RenderDrawLine(ren, MARGIN_LEFT + px, py0, MARGIN_LEFT + px + 1, py1);
        }

        // Ball
        int bx = to_px_x(time_x);
        int by = to_px_y(ball_y);
        int ball_r = 4;

        bool on_line = circle_hits_curve(bx, by, ball_r);

        // Glow
        if (on_line) {
            SDL_SetRenderDrawColor(ren, 255, 255, 100, 40);
        } else {
            SDL_SetRenderDrawColor(ren, 255, 80, 80, 40);
        }
        for (int dy = -ball_r-4; dy <= ball_r+4; dy++) {
            for (int dx = -ball_r-4; dx <= ball_r+4; dx++) {
                if (dx*dx + dy*dy <= (ball_r+4)*(ball_r+4)) {
                    SDL_RenderDrawPoint(ren, bx+dx, by+dy);
                }
            }
        }

        // Solid ball
        if (on_line) {
            SDL_SetRenderDrawColor(ren, 255, 255, 50, 255);
        } else {
            SDL_SetRenderDrawColor(ren, 255, 60, 60, 255);
        }
        for (int dy = -ball_r; dy <= ball_r; dy++) {
            for (int dx = -ball_r; dx <= ball_r; dx++) {
                if (dx*dx + dy*dy <= ball_r*ball_r) {
                    SDL_RenderDrawPoint(ren, bx+dx, by+dy);
                }
            }
        }

        // ── HUD ──
        double accuracy = samples_total > 0 ? 100.0 * samples_on_line / samples_total : 100.0;
        char hud[128];

        SDL_SetRenderDrawColor(ren, 100, 200, 255, 255);
        snprintf(hud, sizeof(hud), "TIME: %.0f/100", time_x);
        draw_text(ren, 10, 10, hud, 2);

        snprintf(hud, sizeof(hud), "ACCURACY: %.1f%%", accuracy);
        draw_text(ren, 250, 10, hud, 2);

        if (on_line) {
            SDL_SetRenderDrawColor(ren, 100, 255, 100, 255);
            draw_text(ren, 550, 10, "ON LINE", 2);
        } else {
            SDL_SetRenderDrawColor(ren, 255, 80, 80, 255);
            draw_text(ren, 550, 10, "OFF LINE", 2);
        }

        snprintf(hud, sizeof(hud), "BALL Y: %.1f  TARGET: %.1f", ball_y, curve(time_x));
        SDL_SetRenderDrawColor(ren, 150, 150, 170, 255);
        draw_text(ren, 10, 35, hud, 1);

        // Progress bar
        SDL_SetRenderDrawColor(ren, 60, 60, 80, 255);
        SDL_Rect progBg = {MARGIN_LEFT, WIN_H - 25, CANVAS_W, 10};
        SDL_RenderFillRect(ren, &progBg);
        SDL_SetRenderDrawColor(ren, 100, 200, 255, 255);
        SDL_Rect progFill = {MARGIN_LEFT, WIN_H - 25, (int)(CANVAS_W * time_x / 100.0), 10};
        SDL_RenderFillRect(ren, &progFill);

        // Controls hint
        SDL_SetRenderDrawColor(ren, 80, 80, 100, 255);
        draw_text(ren, MARGIN_LEFT, WIN_H - 12, "UP/DOWN: MOVE   Q/ESC: QUIT", 1);

        // ── GAME OVER OVERLAY ──
        if (state == GAMEOVER) {
            // Dim overlay
            SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(ren, 0, 0, 0, 180);
            SDL_Rect overlay = {0, 0, WIN_W, WIN_H};
            SDL_RenderFillRect(ren, &overlay);

            double final_acc = samples_total > 0 ? 100.0 * samples_on_line / samples_total : 0.0;

            SDL_SetRenderDrawColor(ren, 100, 200, 255, 255);
            draw_text(ren, WIN_W/2 - 100, WIN_H/2 - 80, "GAME OVER", 3);

            SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
            snprintf(hud, sizeof(hud), "ACCURACY: %.1f%%", final_acc);
            draw_text(ren, WIN_W/2 - 90, WIN_H/2 - 20, hud, 2);

            snprintf(hud, sizeof(hud), "ON LINE: %d / %d SAMPLES", samples_on_line, samples_total);
            draw_text(ren, WIN_W/2 - 150, WIN_H/2 + 10, hud, 2);

            snprintf(hud, sizeof(hud), "INPUTS: %zu KEY PRESSES", inputs.size());
            draw_text(ren, WIN_W/2 - 120, WIN_H/2 + 40, hud, 2);

            SDL_SetRenderDrawColor(ren, 255, 255, 100, 255);
            draw_text(ren, WIN_W/2 - 120, WIN_H/2 + 80, "R: RESTART  Q: QUIT", 2);
        }

        SDL_RenderPresent(ren);
        SDL_Delay(8);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    // Print input log to terminal
    printf("\n=== BALL ON LINE - RESULTS ===\n");
    double final_acc = samples_total > 0 ? 100.0 * samples_on_line / samples_total : 0.0;
    printf("Accuracy: %.1f%%\n", final_acc);
    printf("On line: %d / %d samples\n", samples_on_line, samples_total);
    printf("Total inputs: %zu\n\n", inputs.size());
    printf("%-8s %-6s %-10s %-10s %-8s\n", "Time", "Key", "BallY", "TargetY", "Diff");
    for (const auto& inp : inputs) {
        printf("%-8.1f %-6s %-10.1f %-10.1f %-8.1f\n",
               inp.time_x, inp.key_name, inp.ball_y, inp.target_y,
               std::abs(inp.ball_y - inp.target_y));
    }

    return 0;
}
