#pragma once

#include <memory>
#include <string>
#include "Vec2.hpp"
#include "Colors.hpp"
#include "Surface.hpp"

class Graphics {
public:
    Graphics(const std::string window_name, const int frame_width, const int frame_height);
    ~Graphics();

    int GetFrameWidth() const noexcept;
    int GetFrameHeight() const noexcept;

    void BeginFrame();
    void EndFrame();

    int GetInput();

    void PutPixel(int x, int y, const Color color);
    Color GetPixel(int x, int y) const;
    void PutPixelAlpha(int x, int y, Color color);

    void DrawLine(float x1, float y1, float x2, float y2, Color color);
    void DrawLine(const Vec2& p1, const Vec2& p2, Color c);

private:
    void ClearFrame();
private:
    const std::string _window_name{};
    const Color _init_color{ Colors::Black };

    Surface _canvas;
};
