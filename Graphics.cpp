#include <cassert>
#include <functional>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utils/logger.hpp>
#include "Graphics.hpp"

Graphics::Graphics(const std::string window_name, const int frame_width, const int frame_height)
    : _window_name(window_name), _canvas(frame_width, frame_height) {

    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);
    cv::namedWindow(_window_name, cv::WINDOW_AUTOSIZE);
}

Graphics::~Graphics() {
    cv::destroyAllWindows();
}

int Graphics::GetFrameWidth() const noexcept { return _canvas.GetWidth(); }
int Graphics::GetFrameHeight() const noexcept { return _canvas.GetHeight(); }

void Graphics::BeginFrame() {
    ClearFrame();
}

void Graphics::EndFrame() {
    cv::imshow(_window_name, *_canvas.GetBufferPtr());
}

int Graphics::GetInput(const int delay) {
    return cv::waitKey(delay);
}

void Graphics::ClearFrame() {
    _canvas.Clear(_init_color);
}

void Graphics::PutPixel(int x, int y, const Color color) {
    _canvas.PutPixel(x, y, color);
}

Color Graphics::GetPixel(int x, int y) const {
    return _canvas.GetPixel(x, y);
}

void Graphics::PutPixelAlpha(int x, int y, Color color) {
    _canvas.PutPixelAlpha(x, y, color);
}

void Graphics::DrawLine(float x1, float y1, float x2, float y2, Color color)
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

void Graphics::DrawLine(const Vec2& p1, const Vec2& p2, Color c)
{
    DrawLine(p1.x, p1.y, p2.x, p2.y, c);
}
