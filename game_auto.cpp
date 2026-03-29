#include <SDL2/SDL.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <sys/stat.h>

// ── Tiny 5x7 bitmap font (same as game.cpp) ─────────────────────────────────
static const unsigned char FONT_DATA[][7] = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00}, // ' '
    {0x04,0x04,0x04,0x04,0x04,0x00,0x04}, // '!'
    {0x0A,0x0A,0x00,0x00,0x00,0x00,0x00}, // '"'
    {0x0A,0x1F,0x0A,0x0A,0x1F,0x0A,0x00}, // '#'
    {0x04,0x0F,0x14,0x0E,0x05,0x1E,0x04}, // '$'
    {0x19,0x1A,0x02,0x04,0x0B,0x13,0x00}, // '%'
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00}, // '&'
    {0x04,0x04,0x00,0x00,0x00,0x00,0x00}, // '\''
    {0x02,0x04,0x08,0x08,0x08,0x04,0x02}, // '('
    {0x08,0x04,0x02,0x02,0x02,0x04,0x08}, // ')'
    {0x00,0x04,0x15,0x0E,0x15,0x04,0x00}, // '*'
    {0x00,0x04,0x04,0x1F,0x04,0x04,0x00}, // '+'
    {0x00,0x00,0x00,0x00,0x00,0x04,0x08}, // ','
    {0x00,0x00,0x00,0x1F,0x00,0x00,0x00}, // '-'
    {0x00,0x00,0x00,0x00,0x00,0x00,0x04}, // '.'
    {0x01,0x02,0x02,0x04,0x08,0x08,0x10}, // '/'
    {0x0E,0x11,0x13,0x15,0x19,0x11,0x0E}, // '0'
    {0x04,0x0C,0x04,0x04,0x04,0x04,0x0E}, // '1'
    {0x0E,0x11,0x01,0x06,0x08,0x10,0x1F}, // '2'
    {0x0E,0x11,0x01,0x06,0x01,0x11,0x0E}, // '3'
    {0x02,0x06,0x0A,0x12,0x1F,0x02,0x02}, // '4'
    {0x1F,0x10,0x1E,0x01,0x01,0x11,0x0E}, // '5'
    {0x06,0x08,0x10,0x1E,0x11,0x11,0x0E}, // '6'
    {0x1F,0x01,0x02,0x04,0x08,0x08,0x08}, // '7'
    {0x0E,0x11,0x11,0x0E,0x11,0x11,0x0E}, // '8'
    {0x0E,0x11,0x11,0x0F,0x01,0x02,0x0C}, // '9'
    {0x00,0x00,0x04,0x00,0x04,0x00,0x00}, // ':'
    {0x00,0x00,0x04,0x00,0x04,0x04,0x08}, // ';'
    {0x02,0x04,0x08,0x10,0x08,0x04,0x02}, // '<'
    {0x00,0x00,0x1F,0x00,0x1F,0x00,0x00}, // '='
    {0x08,0x04,0x02,0x01,0x02,0x04,0x08}, // '>'
    {0x0E,0x11,0x01,0x02,0x04,0x00,0x04}, // '?'
    {0x0E,0x11,0x17,0x15,0x17,0x10,0x0E}, // '@'
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

const double SAMPLE_STEP = 0.5;
const int BALL_R = 4;
const int FPS = 30;

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

// ── JSON parsing ─────────────────────────────────────────────────────────────
struct Waypoint { double x, y; };

std::vector<Waypoint> parse_json(const std::string& filename) {
    std::ifstream f(filename);
    if (!f.is_open()) { fprintf(stderr, "Error: cannot open %s\n", filename.c_str()); exit(1); }
    std::stringstream ss;
    ss << f.rdbuf();
    std::string json = ss.str();

    std::vector<Waypoint> pts;
    size_t pos = 0;
    while (pos < json.size()) {
        size_t brace = json.find('{', pos);
        if (brace == std::string::npos) break;
        size_t end_brace = json.find('}', brace);
        if (end_brace == std::string::npos) break;
        std::string obj = json.substr(brace, end_brace - brace + 1);
        double x = 0, y = 0;
        bool got_x = false, got_y = false;
        size_t xpos = obj.find("\"x\"");
        if (xpos != std::string::npos) { size_t c = obj.find(':', xpos+3); if (c!=std::string::npos) { x = strtod(obj.c_str()+c+1, nullptr); got_x = true; } }
        size_t ypos = obj.find("\"y\"");
        if (ypos != std::string::npos) { size_t c = obj.find(':', ypos+3); if (c!=std::string::npos) { y = strtod(obj.c_str()+c+1, nullptr); got_y = true; } }
        if (got_x && got_y) pts.push_back({x, y});
        pos = end_brace + 1;
    }
    return pts;
}

double interpolate_y(const std::vector<Waypoint>& pts, double x) {
    if (pts.empty()) return 0.0;
    if (x <= pts.front().x) return pts.front().y;
    if (x >= pts.back().x) return pts.back().y;
    for (size_t i = 0; i + 1 < pts.size(); i++) {
        if (x >= pts[i].x && x <= pts[i + 1].x) {
            double dx = pts[i + 1].x - pts[i].x;
            if (dx < 1e-9) return pts[i].y;
            double t = (x - pts[i].x) / dx;
            return pts[i].y + t * (pts[i + 1].y - pts[i].y);
        }
    }
    return pts.back().y;
}

// ── Render one frame ─────────────────────────────────────────────────────────
void render_frame(SDL_Renderer* ren, double time_x, double ball_y,
                  int samples_on_line, int samples_total, bool is_gameover) {
    SDL_SetRenderDrawColor(ren, 20, 20, 30, 255);
    SDL_RenderClear(ren);

    // Canvas background
    SDL_SetRenderDrawColor(ren, 30, 30, 45, 255);
    SDL_Rect canvas = {MARGIN_LEFT, MARGIN_TOP, CANVAS_W, CANVAS_H};
    SDL_RenderFillRect(ren, &canvas);

    // Grid
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

    // Curve
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
    int ball_r = BALL_R;
    bool on_line = circle_hits_curve(bx, by, ball_r);

    // Glow
    if (on_line) SDL_SetRenderDrawColor(ren, 255, 255, 100, 40);
    else SDL_SetRenderDrawColor(ren, 255, 80, 80, 40);
    for (int dy = -ball_r-4; dy <= ball_r+4; dy++)
        for (int dx = -ball_r-4; dx <= ball_r+4; dx++)
            if (dx*dx + dy*dy <= (ball_r+4)*(ball_r+4))
                SDL_RenderDrawPoint(ren, bx+dx, by+dy);

    // Solid ball
    if (on_line) SDL_SetRenderDrawColor(ren, 255, 255, 50, 255);
    else SDL_SetRenderDrawColor(ren, 255, 60, 60, 255);
    for (int dy = -ball_r; dy <= ball_r; dy++)
        for (int dx = -ball_r; dx <= ball_r; dx++)
            if (dx*dx + dy*dy <= ball_r*ball_r)
                SDL_RenderDrawPoint(ren, bx+dx, by+dy);

    // HUD
    double accuracy = samples_total > 0 ? 100.0 * samples_on_line / samples_total : 100.0;
    char hud[128];

    SDL_SetRenderDrawColor(ren, 100, 200, 255, 255);
    snprintf(hud, sizeof(hud), "TIME: %.0f/100", time_x);
    draw_text(ren, 10, 10, hud, 2);

    snprintf(hud, sizeof(hud), "ACCURACY: %.1f%%", accuracy);
    draw_text(ren, 250, 10, hud, 2);

    if (on_line) { SDL_SetRenderDrawColor(ren, 100, 255, 100, 255); draw_text(ren, 550, 10, "ON LINE", 2); }
    else { SDL_SetRenderDrawColor(ren, 255, 80, 80, 255); draw_text(ren, 550, 10, "OFF LINE", 2); }

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

    // Auto mode label
    SDL_SetRenderDrawColor(ren, 80, 80, 100, 255);
    draw_text(ren, MARGIN_LEFT, WIN_H - 12, "AUTO MODE", 1);

    // Game over overlay
    if (is_gameover) {
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
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s <waypoints.json>\n", argv[0]);
        printf("Renders an automated run to /runs/<accuracy>_<timestamp>.mp4\n");
        return 1;
    }

    auto waypoints = parse_json(argv[1]);
    if (waypoints.empty()) { fprintf(stderr, "Error: no waypoints in %s\n", argv[1]); return 1; }

    // Sort by x
    for (size_t i = 0; i + 1 < waypoints.size(); i++)
        for (size_t j = i + 1; j < waypoints.size(); j++)
            if (waypoints[j].x < waypoints[i].x)
                std::swap(waypoints[i], waypoints[j]);

    printf("Loaded %zu waypoints from %s\n", waypoints.size(), argv[1]);

    // ── First pass: compute accuracy + per-sample data ─────────────────────
    struct SampleData { double x, ball_y; bool on_line; };
    std::vector<SampleData> samples;
    int samples_total = 0, samples_on_line = 0;
    {
        double last_sample_x = -1.0;
        for (double x = 0.0; x <= GAME_X_MAX; x += 0.1) {
            if (x - last_sample_x >= SAMPLE_STEP) {
                last_sample_x += SAMPLE_STEP;
                double by = interpolate_y(waypoints, last_sample_x);
                int sbx = to_px_x(last_sample_x);
                int sby = to_px_y(by);
                bool hit = circle_hits_curve(sbx, sby, BALL_R);
                samples_total++;
                if (hit) samples_on_line++;
                samples.push_back({last_sample_x, by, hit});
            }
        }
    }
    double accuracy = samples_total > 0 ? 100.0 * samples_on_line / samples_total : 0.0;
    printf("Accuracy: %.1f%%\n", accuracy);

    // Print per-sample results
    printf("\n%-8s %-10s %-8s\n", "X", "BallY", "OnLine");
    for (const auto& s : samples) {
        printf("%-8.1f %-10.2f %s\n", s.x, s.ball_y, s.on_line ? "YES" : "NO");
    }
    printf("\n");

    // ── Build output filename ────────────────────────────────────────────────
    mkdir("runs", 0755);
    time_t now = time(nullptr);
    struct tm* t = localtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d_%H-%M-%S", t);
    char outpath[256];
    snprintf(outpath, sizeof(outpath), "runs/%.1f%%_%s.mp4", accuracy, timestamp);

    // Append best run to best_runs.csv (overwrite with latest best)
    {
        // Check if current accuracy beats what's in best_runs.csv
        bool is_new_best = true;
        FILE* check = fopen("best_runs.csv", "r");
        if (check) {
            char line[256];
            if (fgets(line, sizeof(line), check)) { // skip header
                if (fgets(line, sizeof(line), check)) { // read first data line
                    double old_acc = 0;
                    sscanf(line, "%*f,%*f,%*[^,],%lf", &old_acc);
                    // We'll always overwrite — the agent decides whether to keep
                }
            }
            fclose(check);
        }
        FILE* csv = fopen("best_runs.csv", "w");
        if (csv) {
            fprintf(csv, "x,ball_y,on_line\n");
            for (const auto& s : samples) {
                fprintf(csv, "%.1f,%.2f,%s\n", s.x, s.ball_y, s.on_line ? "YES" : "NO");
            }
            fclose(csv);
        }
    }

    printf("Rendering to: %s\n", outpath);

    // ── Init SDL2 offscreen ──────────────────────────────────────────────────
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("Render", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                       WIN_W, WIN_H, 0);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if (!ren) ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_TARGETTEXTURE);
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

    // Create offscreen render target
    SDL_Texture* target = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888,
                                            SDL_TEXTUREACCESS_TARGET, WIN_W, WIN_H);
    SDL_SetRenderTarget(ren, target);

    // ── Open ffmpeg pipe ─────────────────────────────────────────────────────
    char cmd[512];
    snprintf(cmd, sizeof(cmd),
        "ffmpeg -y -f rawvideo -pixel_format rgb24 -video_size %dx%d -framerate %d "
        "-i pipe:0 -c:v libx264 -pix_fmt yuv420p -preset fast -crf 18 \"%s\" 2>/dev/null",
        WIN_W, WIN_H, FPS, outpath);
    FILE* ffmpeg = popen(cmd, "w");
    if (!ffmpeg) { fprintf(stderr, "Error: cannot start ffmpeg\n"); return 1; }

    // Pixel buffer for reading frames
    std::vector<unsigned char> pixels(WIN_W * WIN_H * 3);

    // ── Simulation parameters ────────────────────────────────────────────────
    double speed_per_frame = GAME_X_MAX / (10.0 * FPS); // 10 second video
    double time_x = 0.0;
    int running_samples = 0, running_on_line = 0;
    double last_sample_x = -1.0;
    int total_frames = (int)(10.0 * FPS);

    printf("Rendering %d frames...\n", total_frames + FPS * 2); // +2s for game over

    for (int frame = 0; frame < total_frames; frame++) {
        time_x = (double)frame / total_frames * GAME_X_MAX;
        if (time_x > GAME_X_MAX) time_x = GAME_X_MAX;

        double ball_y = interpolate_y(waypoints, time_x);

        // Update running accuracy
        while (last_sample_x + SAMPLE_STEP <= time_x) {
            last_sample_x += SAMPLE_STEP;
            double by = interpolate_y(waypoints, last_sample_x);
            int sbx = to_px_x(last_sample_x);
            int sby = to_px_y(by);
            running_samples++;
            if (circle_hits_curve(sbx, sby, BALL_R)) running_on_line++;
        }

        SDL_SetRenderTarget(ren, target);
        render_frame(ren, time_x, ball_y, running_on_line, running_samples, false);

        // Read pixels from render target
        SDL_Rect rect = {0, 0, WIN_W, WIN_H};
        std::vector<unsigned char> argb(WIN_W * WIN_H * 4);
        SDL_RenderReadPixels(ren, &rect, SDL_PIXELFORMAT_ARGB8888, argb.data(), WIN_W * 4);

        // Convert ARGB to RGB24
        for (int i = 0; i < WIN_W * WIN_H; i++) {
            pixels[i * 3 + 0] = argb[i * 4 + 2]; // R
            pixels[i * 3 + 1] = argb[i * 4 + 1]; // G
            pixels[i * 3 + 2] = argb[i * 4 + 0]; // B
        }

        fwrite(pixels.data(), 1, pixels.size(), ffmpeg);

        if (frame % 30 == 0) {
            printf("\r  Frame %d/%d (%.0f%%)", frame, total_frames, 100.0 * frame / total_frames);
            fflush(stdout);
        }
    }

    // ── Game over frames (hold for 2 seconds) ───────────────────────────────
    double final_ball_y = interpolate_y(waypoints, GAME_X_MAX);
    for (int f = 0; f < FPS * 2; f++) {
        SDL_SetRenderTarget(ren, target);
        render_frame(ren, GAME_X_MAX, final_ball_y, running_on_line, running_samples, true);

        SDL_Rect rect = {0, 0, WIN_W, WIN_H};
        std::vector<unsigned char> argb(WIN_W * WIN_H * 4);
        SDL_RenderReadPixels(ren, &rect, SDL_PIXELFORMAT_ARGB8888, argb.data(), WIN_W * 4);
        for (int i = 0; i < WIN_W * WIN_H; i++) {
            pixels[i * 3 + 0] = argb[i * 4 + 2];
            pixels[i * 3 + 1] = argb[i * 4 + 1];
            pixels[i * 3 + 2] = argb[i * 4 + 0];
        }
        fwrite(pixels.data(), 1, pixels.size(), ffmpeg);
    }

    printf("\r  Done! %d frames rendered.          \n", total_frames + FPS * 2);

    pclose(ffmpeg);
    SDL_DestroyTexture(target);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    printf("\nSaved: %s\n", outpath);
    printf("Accuracy: %.1f%% (%d/%d on line)\n", accuracy, samples_on_line, samples_total);

    return 0;
}
