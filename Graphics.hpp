#pragma once

#include <memory>
#include <string>
#include "Vec2.hpp"
#include "Colors.hpp"

namespace cv {
    class Mat;
}

class Graphics {
public:
    Graphics(const std::string window_name, const int frame_width, const int frame_height);
    ~Graphics();

    int GetFrameWidth() const noexcept;
    int GetFrameHeight() const noexcept;

    void BeginFrame();
    void EndFrame();

    int GetInput();

    void PutPixel(int x, int y, const open_3d::Color color);
    open_3d::Color GetPixel(int x, int y) const;
    void PutPixelAlpha(int x, int y, open_3d::Color color);

    void DrawLine(float x1, float y1, float x2, float y2, open_3d::Color color);
    void DrawLine(const open_3d::Vec2& p1, const open_3d::Vec2& p2, open_3d::Color c);

    // Triangle Rasterization
    void DrawTriangle(const open_3d::Vec2& v0, const open_3d::Vec2& v1, const open_3d::Vec2& v2, open_3d::Color c);
private:
    void ClearFrame();

    // Triangle Rasterization
    void DrawFlatTopTriangle(const open_3d::Vec2& v0, const open_3d::Vec2& v1, const open_3d::Vec2& v2, open_3d::Color c);
    void DrawFlatBottomTriangle(const open_3d::Vec2& v0, const open_3d::Vec2& v1, const open_3d::Vec2& v2, open_3d::Color c);

private:
    const std::string _window_name{};
    const int _frame_width{};
    const int _frame_height{};
    const open_3d::Color _init_color{ open_3d::Colors::Black };

    std::unique_ptr<cv::Mat> _canvas;
};
