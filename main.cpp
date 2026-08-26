#include <cassert>
#include <string>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utils/logger.hpp> // Required header

#include "Math.hpp"
#include "Vec2.hpp"
#include "Mat2.hpp"
#include "Colors.hpp"

class Graphics {
public:
    Graphics(const std::string window_name, const int frame_width, const int frame_height)
        : _window_name(window_name), _frame_width(frame_width), _frame_height(frame_height) {

        cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);
        _canvas = cv::Mat(_frame_height, _frame_width, CV_8UC4);
        cv::namedWindow(_window_name, cv::WINDOW_AUTOSIZE);
    }

    ~Graphics() {
        cv::destroyAllWindows();
    }

    int GetFrameWidth() const noexcept { return _frame_width; }
    int GetFrameHeight() const noexcept { return _frame_height; }

    void BeginFrame() {
        ClearFrame();
    }

    void EndFrame() {
        cv::imshow(_window_name, _canvas);
    }

    int GetInput() {
        const int key = cv::waitKey(0); // waits indefinitely for a key press
        //std::cout << "Key code: " << key << ", Char : " << static_cast<char>(key) << "\n";
        return key;
    }

    void ClearFrame() {
        // Interpret the canvas as 32-bit pixels (CV_8UC4 assumed)
        uint32_t* ptr = _canvas.ptr<uint32_t>(0);

        // Total number of pixels
        size_t total = _canvas.rows * _canvas.cols;

        // Fill with the initial color (packed BGRA)
        std::fill(ptr, ptr + total, _init_color.dword);
    }

    void PutPixel(int x, int y, const open_3d::Color color) {
        assert(x >= 0);
        assert(y >= 0);
        assert(x < _frame_width);
        assert(y < _frame_height);

        // Pointer to the pixel (BGRA order in OpenCV)
        uint32_t* ptr = _canvas.ptr<uint32_t>(y);
        ptr[x] = color.dword;
    }

    open_3d::Color GetPixel(int x, int y) const {
        assert(x >= 0);
        assert(y >= 0);
        assert(x < _frame_width);
        assert(y < _frame_height);

        const uint32_t* ptr = _canvas.ptr<uint32_t>(y);
        return ptr[x];
    }

    void PutPixelAlpha(int x, int y, open_3d::Color color)
    {
        assert(x >= 0);
        assert(y >= 0);
        assert(x < _frame_width);
        assert(y < _frame_height);

        // load source pixel
        const open_3d::Color background_color = GetPixel(x, y);

        // blend channels
        const unsigned char rsltRed = (color.GetR() * color.GetA() + background_color.GetR() * (255u - color.GetA())) / 256u;
        const unsigned char rsltGreen = (color.GetG() * color.GetA() + background_color.GetG() * (255u - color.GetA())) / 256u;
        const unsigned char rsltBlue = (color.GetB() * color.GetA() + background_color.GetB() * (255u - color.GetA())) / 256u;

        // pack channels back into pixel and fire pixel onto surface
        PutPixel(x, y, { rsltRed,rsltGreen,rsltBlue });
    }

    void DrawLine(float x1, float y1, float x2, float y2, open_3d::Color color)
    {
        const float dx = x2 - x1;
        const float dy = y2 - y1;

        if (dy == 0.0f && dx == 0.0f)
        {
            PutPixel(int(x1), int(y1), color);
        }
        else if (abs(dy) > abs(dx))
        {
            if (dy < 0.0f)
            {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }

            const float m = dx / dy;
            float y = y1;
            int lastIntY{};
            for (float x = x1; y < y2; y += 1.0f, x += m)
            {
                lastIntY = int(y);
                PutPixel(int(x), lastIntY, color);
            }
            if (int(y2) > lastIntY)
            {
                PutPixel(int(x2), int(y2), color);
            }
        }
        else
        {
            if (dx < 0.0f)
            {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }

            const float m = dy / dx;
            float x = x1;
            int lastIntX{};
            for (float y = y1; x < x2; x += 1.0f, y += m)
            {
                lastIntX = int(x);
                PutPixel(lastIntX, int(y), color);
            }
            if (int(x2) > lastIntX)
            {
                PutPixel(int(x2), int(y2), color);
            }
        }
    }

    void DrawLine(const open_3d::Vec2& p1, const open_3d::Vec2& p2, open_3d::Color c)
    {
        DrawLine(p1.x, p1.y, p2.x, p2.y, c);
    }

private:
    const std::string _window_name{};
    const int _frame_width{};
    const int _frame_height{};
    const open_3d::Color _init_color{ open_3d::Colors::Black };

    cv::Mat _canvas;
};

class Game {
public:
    Game(const std::string window_name, const int frame_width, const int frame_height)
        : _gfx(window_name, frame_width, frame_height) {}

    ~Game() = default;

    void Go() {

        InitModel();

        while (_main_loop_active) {

            _gfx.BeginFrame();

            UpdateModel();
            ComposeFrame();

            _gfx.EndFrame();
            ManageInputs(_gfx.GetInput());
        }

    }

private:
    Graphics _gfx;
    bool _main_loop_active{ true };

    /********************************/
    /*  User Variables              */
    std::vector<open_3d::Vec2> star;
    float theta = 0.0f;
    static constexpr float vRot = open_3d::PI / 60.0f;
    static constexpr float radInner = 1.0f;
    static constexpr float radOuter = 2.0f;
    static constexpr int nflares = 5;
    static constexpr float size = 100.0f;
    /********************************/

private:
    void ManageInputs(const int key) {
        switch (key) {
        case -1:
        case 27:
        {
            // ESC key or when window is closed
            _main_loop_active = false;
            break;
        }
        case 'a':
            break;
        case 'd':
            break;
        case 's':
            break;
        case 'w':
            break;
        case 'q':
            break;
        case 'e':
            break;
        default:
            break;
        }
    }

    void InitModel() {
        const float dTheta = 2.0f * open_3d::PI / float(nflares * 2);
        for (int i = 0; i < nflares * 2; i++)
        {
            const float rad = (i % 2 == 0) ? radOuter : radInner;
            star.emplace_back(
                rad * cos(float(i) * dTheta),
                rad * sin(float(i) * dTheta)
            );
        }
    }

    void UpdateModel() {
        theta += vRot;
    }

    void ComposeFrame() {
        const open_3d::Color model_color = open_3d::Colors::White;

        const open_3d::Vec2 trl = { float(_gfx.GetFrameWidth()) / 2.0f,float(_gfx.GetFrameHeight()) / 2.0f };
        const open_3d::Mat2 trf = open_3d::Mat2::Rotation(theta) * open_3d::Mat2::Scaling(size);
        auto vtx(star);
        for (auto& v : vtx)
        {
            v *= trf;
            v += trl;
        }
        for (auto i = vtx.cbegin(), end = std::prev(vtx.cend()); i != end; i++)
        {
            _gfx.DrawLine(*i, *std::next(i), model_color);
        }
        _gfx.DrawLine(vtx.front(), vtx.back(), model_color);
    }
};


int main() {
    const int frame_width{ 640 };
    const int frame_height{ 480 };
    
    Game game{"3D Programming Fundamentals", frame_width, frame_height};
    game.Go();

    return 0;
}
