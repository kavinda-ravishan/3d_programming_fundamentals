#include <cassert>
#include <functional>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utils/logger.hpp> // Required header
#include "Graphics.hpp"

Graphics::Graphics(const std::string window_name, const int frame_width, const int frame_height)
    : _window_name(window_name), _frame_width(frame_width), _frame_height(frame_height) {

    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);
    _canvas = std::make_unique<cv::Mat>(_frame_height, _frame_width, CV_8UC4);
    cv::namedWindow(_window_name, cv::WINDOW_AUTOSIZE);
}

Graphics::~Graphics() {
    cv::destroyAllWindows();
}

int Graphics::GetFrameWidth() const noexcept { return _frame_width; }
int Graphics::GetFrameHeight() const noexcept { return _frame_height; }

void Graphics::BeginFrame() {
    ClearFrame();
}

void Graphics::EndFrame() {
    cv::imshow(_window_name, *_canvas);
}

int Graphics::GetInput() {
    const int key = cv::waitKey(0); // waits indefinitely for a key press
    return key;
}

void Graphics::ClearFrame() {
    // Interpret the canvas as 32-bit pixels (CV_8UC4 assumed)
    uint32_t* ptr = _canvas->ptr<uint32_t>(0);

    // Total number of pixels
    size_t total = _canvas->rows * _canvas->cols;

    // Fill with the initial color (packed BGRA)
    std::fill(ptr, ptr + total, _init_color.dword);
}

void Graphics::PutPixel(int x, int y, const open_3d::Color color) {
    assert(x >= 0);
    assert(y >= 0);
    assert(x < _frame_width);
    assert(y < _frame_height);

    // Pointer to the pixel (BGRA order in OpenCV)
    uint32_t* ptr = _canvas->ptr<uint32_t>(y);
    ptr[x] = color.dword;
}

open_3d::Color Graphics::GetPixel(int x, int y) const {
    assert(x >= 0);
    assert(y >= 0);
    assert(x < _frame_width);
    assert(y < _frame_height);

    const uint32_t* ptr = _canvas->ptr<uint32_t>(y);
    return ptr[x];
}

void Graphics::PutPixelAlpha(int x, int y, open_3d::Color color)
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

void Graphics::DrawLine(float x1, float y1, float x2, float y2, open_3d::Color color)
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

void Graphics::DrawLine(const open_3d::Vec2& p1, const open_3d::Vec2& p2, open_3d::Color c)
{
    DrawLine(p1.x, p1.y, p2.x, p2.y, c);
}

void Graphics::DrawTriangle(const open_3d::Vec2& v0, const open_3d::Vec2& v1, const open_3d::Vec2& v2, open_3d::Color c)
{
    // using pointers so we can swap (for sorting purposes)
    const open_3d::Vec2* pv0 = &v0;
    const open_3d::Vec2* pv1 = &v1;
    const open_3d::Vec2* pv2 = &v2;

    // sorting vertices by y
    if (pv1->y < pv0->y) std::swap(pv0, pv1);
    if (pv2->y < pv1->y) std::swap(pv1, pv2);
    if (pv1->y < pv0->y) std::swap(pv0, pv1);

    if (pv0->y == pv1->y) // natural flat top
    {
        // sorting top vertices by x
        if (pv1->x < pv0->x) std::swap(pv0, pv1);
        DrawFlatTopTriangle(*pv0, *pv1, *pv2, c);
    }
    else if (pv1->y == pv2->y) // natural flat bottom
    {
        // sorting bottom vertices by x
        if (pv2->x < pv1->x) std::swap(pv1, pv2);
        DrawFlatBottomTriangle(*pv0, *pv1, *pv2, c);
    }
    else // general triangle
    {
        // find splitting vertex
        const float alphaSplit =
            (pv1->y - pv0->y) /
            (pv2->y - pv0->y);
        const open_3d::Vec2 vi = *pv0 + (*pv2 - *pv0) * alphaSplit;

        if (pv1->x < vi.x) // major right
        {
            DrawFlatBottomTriangle(*pv0, *pv1, vi, c);
            DrawFlatTopTriangle(*pv1, vi, *pv2, c);
        }
        else // major left
        {
            DrawFlatBottomTriangle(*pv0, vi, *pv1, c);
            DrawFlatTopTriangle(vi, *pv1, *pv2, c);
        }
    }
}

void Graphics::DrawFlatTopTriangle(const open_3d::Vec2& v0, const open_3d::Vec2& v1, const open_3d::Vec2& v2, open_3d::Color c)
{
    // calulcate slopes in screen space
    float m0 = (v2.x - v0.x) / (v2.y - v0.y);
    float m1 = (v2.x - v1.x) / (v2.y - v1.y);

    // calculate start and end scanlines
    const int yStart = (int)ceil(v0.y - 0.5f);
    const int yEnd = (int)ceil(v2.y - 0.5f); // the scanline AFTER the last line drawn

    for (int y = yStart; y < yEnd; y++)
    {
        // caluclate start and end points (x-coords)
        // add 0.5 to y value because we're calculating based on pixel CENTERS
        const float px0 = m0 * (float(y) + 0.5f - v0.y) + v0.x;
        const float px1 = m1 * (float(y) + 0.5f - v1.y) + v1.x;

        // calculate start and end pixels
        const int xStart = (int)ceil(px0 - 0.5f);
        const int xEnd = (int)ceil(px1 - 0.5f); // the pixel AFTER the last pixel drawn

        for (int x = xStart; x < xEnd; x++)
        {
            PutPixel(x, y, c);
        }
    }
}

void Graphics::DrawFlatBottomTriangle(const open_3d::Vec2& v0, const open_3d::Vec2& v1, const open_3d::Vec2& v2, open_3d::Color c)
{
    // calulcate slopes in screen space
    float m0 = (v1.x - v0.x) / (v1.y - v0.y);
    float m1 = (v2.x - v0.x) / (v2.y - v0.y);

    // calculate start and end scanlines
    const int yStart = (int)ceil(v0.y - 0.5f);
    const int yEnd = (int)ceil(v2.y - 0.5f); // the scanline AFTER the last line drawn

    for (int y = yStart; y < yEnd; y++)
    {
        // caluclate start and end points
        // add 0.5 to y value because we're calculating based on pixel CENTERS
        const float px0 = m0 * (float(y) + 0.5f - v0.y) + v0.x;
        const float px1 = m1 * (float(y) + 0.5f - v0.y) + v0.x;

        // calculate start and end pixels
        const int xStart = (int)ceil(px0 - 0.5f);
        const int xEnd = (int)ceil(px1 - 0.5f); // the pixel AFTER the last pixel drawn

        for (int x = xStart; x < xEnd; x++)
        {
            PutPixel(x, y, c);
        }
    }
}