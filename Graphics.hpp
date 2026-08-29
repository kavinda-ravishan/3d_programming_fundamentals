#pragma once

#include <memory>
#include <string>
#include "Vec2.hpp"
#include "Colors.hpp"
#include "Surface.hpp"
#include "TexVertex.hpp"

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
    void DrawTriangleTex(const open_3d::TexVertex& v0, const open_3d::TexVertex& v1, const open_3d::TexVertex& v2, const Surface& tex);
    void DrawTriangleTexWrap(const open_3d::TexVertex& v0, const open_3d::TexVertex& v1, const open_3d::TexVertex& v2, const Surface& tex);

private:
    void ClearFrame();

    // Triangle Rasterization
    void DrawFlatTopTriangle(const open_3d::Vec2& v0, const open_3d::Vec2& v1, const open_3d::Vec2& v2, open_3d::Color c);
    void DrawFlatBottomTriangle(const open_3d::Vec2& v0, const open_3d::Vec2& v1, const open_3d::Vec2& v2, open_3d::Color c);

    void DrawFlatTriangleTex(const open_3d::TexVertex& v0, const open_3d::TexVertex& v1, const open_3d::TexVertex& v2, const Surface& tex,
        const open_3d::TexVertex& dv0, const open_3d::TexVertex& dv1, open_3d::TexVertex& itEdge1);
    void DrawFlatTopTriangleTex(const open_3d::TexVertex& v0, const open_3d::TexVertex& v1, const open_3d::TexVertex& v2, const Surface& tex);
    void DrawFlatBottomTriangleTex(const open_3d::TexVertex& v0, const open_3d::TexVertex& v1, const open_3d::TexVertex& v2, const Surface& tex);

    void DrawFlatTopTriangleTexWrap(const open_3d::TexVertex& v0, const open_3d::TexVertex& v1, const open_3d::TexVertex& v2, const Surface& tex);
    void DrawFlatBottomTriangleTexWrap(const open_3d::TexVertex& v0, const open_3d::TexVertex& v1, const open_3d::TexVertex& v2, const Surface& tex);
    void DrawFlatTriangleTexWrap(const open_3d::TexVertex& v0, const open_3d::TexVertex& v1, const open_3d::TexVertex& v2, const Surface& tex,
        const open_3d::TexVertex& dv0, const open_3d::TexVertex& dv1, open_3d::TexVertex& itEdge1);

private:
    const std::string _window_name{};
    const open_3d::Color _init_color{ open_3d::Colors::Black };

    Surface _canvas;
};
