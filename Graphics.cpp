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

int Graphics::GetInput() {
    const int key = cv::waitKey(0); // waits indefinitely for a key press
    return key;
}

void Graphics::ClearFrame() {
    _canvas.Clear(_init_color);
}

void Graphics::PutPixel(int x, int y, const open_3d::Color color) {
    _canvas.PutPixel(x, y, color);
}

open_3d::Color Graphics::GetPixel(int x, int y) const {
    return _canvas.GetPixel(x, y);
}

void Graphics::PutPixelAlpha(int x, int y, open_3d::Color color) {
    _canvas.PutPixelAlpha(x, y, color);
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

void Graphics::DrawFlatTriangleTex(const open_3d::TexVertex& v0, const open_3d::TexVertex& v1, const open_3d::TexVertex& v2, const Surface& tex, const open_3d::TexVertex& dv0, const open_3d::TexVertex& dv1, open_3d::TexVertex& itEdge1) {

    // create edge interpolant for left edge (always v0)
    open_3d::TexVertex itEdge0 = v0;

    // calculate start and end scanlines
    const int yStart = (int)ceil(v0.pos.y - 0.5f);
    const int yEnd = (int)ceil(v2.pos.y - 0.5f); // the scanline AFTER the last line drawn

    // do interpolant prestep
    itEdge0 += dv0 * (float(yStart) + 0.5f - v0.pos.y);
    itEdge1 += dv1 * (float(yStart) + 0.5f - v0.pos.y);

    // init tex width/height and clamp values
    const float tex_width = float(tex.GetWidth());
    const float tex_height = float(tex.GetHeight());
    const float tex_clamp_x = tex_width - 1.0f;
    const float tex_clamp_y = tex_height - 1.0f;

    for (int y = yStart; y < yEnd; y++, itEdge0 += dv0, itEdge1 += dv1)
    {
        // calculate start and end pixels
        const int xStart = (int)ceil(itEdge0.pos.x - 0.5f);
        const int xEnd = (int)ceil(itEdge1.pos.x - 0.5f); // the pixel AFTER the last pixel drawn

        // calculate scanline dTexCoord / dx
        const open_3d::Vec2 dtcLine = (itEdge1.tc - itEdge0.tc) / (itEdge1.pos.x - itEdge0.pos.x);

        // create scanline tex coord interpolant and prestep
        open_3d::Vec2 itcLine = itEdge0.tc + dtcLine * (float(xStart) + 0.5f - itEdge0.pos.x);

        for (int x = xStart; x < xEnd; x++, itcLine += dtcLine)
        {
            PutPixel(x, y, tex.GetPixel(
                int(std::min(itcLine.x * tex_width, tex_clamp_x)),
                int(std::min(itcLine.y * tex_height, tex_clamp_y))));
            // need std::min b/c tc.x/y == 1.0, we'll read off edge of tex
            // and with fp err, tc.x/y can be > 1.0 (by a tiny amount)
        }
    }
}

void Graphics::DrawFlatTopTriangleTex(const open_3d::TexVertex& v0, const open_3d::TexVertex& v1, const open_3d::TexVertex& v2, const Surface& tex) {
    // calulcate dVertex / dy
    const float delta_y = v2.pos.y - v0.pos.y;
    const open_3d::TexVertex dv0 = (v2 - v0) / delta_y;
    const open_3d::TexVertex dv1 = (v2 - v1) / delta_y;

    // create right edge interpolant
    open_3d::TexVertex itEdge1 = v1;

    // call the flat triangle render routine
    DrawFlatTriangleTex(v0, v1, v2, tex, dv0, dv1, itEdge1);
}

void Graphics::DrawFlatBottomTriangleTex(const open_3d::TexVertex& v0, const open_3d::TexVertex& v1, const open_3d::TexVertex& v2, const Surface& tex) {
    // calulcate dVertex / dy
    const float delta_y = v2.pos.y - v0.pos.y;
    const open_3d::TexVertex dv0 = (v1 - v0) / delta_y;
    const open_3d::TexVertex dv1 = (v2 - v0) / delta_y;

    // create right edge interpolant
    open_3d::TexVertex itEdge1 = v0;

    // call the flat triangle render routine
    DrawFlatTriangleTex(v0, v1, v2, tex, dv0, dv1, itEdge1);
}

void Graphics::DrawTriangleTex(const open_3d::TexVertex& v0, const open_3d::TexVertex& v1, const open_3d::TexVertex& v2, const Surface& tex) {
    // using pointers so we can swap (for sorting purposes)
    const open_3d::TexVertex* pv0 = &v0;
    const open_3d::TexVertex* pv1 = &v1;
    const open_3d::TexVertex* pv2 = &v2;

    // sorting vertices by y
    if (pv1->pos.y < pv0->pos.y) std::swap(pv0, pv1);
    if (pv2->pos.y < pv1->pos.y) std::swap(pv1, pv2);
    if (pv1->pos.y < pv0->pos.y) std::swap(pv0, pv1);

    if (pv0->pos.y == pv1->pos.y) // natural flat top
    {
        // sorting top vertices by x
        if (pv1->pos.x < pv0->pos.x) std::swap(pv0, pv1);
        DrawFlatTopTriangleTex(*pv0, *pv1, *pv2, tex);
    }
    else if (pv1->pos.y == pv2->pos.y) // natural flat bottom
    {
        // sorting bottom vertices by x
        if (pv2->pos.x < pv1->pos.x) std::swap(pv1, pv2);
        DrawFlatBottomTriangleTex(*pv0, *pv1, *pv2, tex);
    }
    else // general triangle
    {
        // find splitting vertex
        const float alphaSplit =
            (pv1->pos.y - pv0->pos.y) /
            (pv2->pos.y - pv0->pos.y);
        const open_3d::TexVertex vi = pv0->InterpolateTo(*pv2, alphaSplit);

        if (pv1->pos.x < vi.pos.x) // major right
        {
            DrawFlatBottomTriangleTex(*pv0, *pv1, vi, tex);
            DrawFlatTopTriangleTex(*pv1, vi, *pv2, tex);
        }
        else // major left
        {
            DrawFlatBottomTriangleTex(*pv0, vi, *pv1, tex);
            DrawFlatTopTriangleTex(vi, *pv1, *pv2, tex);
        }
    }
}

void Graphics::DrawFlatTriangleTexWrap(const open_3d::TexVertex& v0, const open_3d::TexVertex& v1, const open_3d::TexVertex& v2, const Surface& tex, const open_3d::TexVertex& dv0, const open_3d::TexVertex& dv1, open_3d::TexVertex& itEdge1) {
    // create edge interpolant for left edge (always v0)
    open_3d::TexVertex itEdge0 = v0;

    // calculate start and end scanlines
    const int yStart = (int)ceil(v0.pos.y - 0.5f);
    const int yEnd = (int)ceil(v2.pos.y - 0.5f); // the scanline AFTER the last line drawn

    // do interpolant prestep
    itEdge0 += dv0 * (float(yStart) + 0.5f - v0.pos.y);
    itEdge1 += dv1 * (float(yStart) + 0.5f - v0.pos.y);

    // init tex width/height and clamp values
    const float tex_width = float(tex.GetWidth());
    const float tex_height = float(tex.GetHeight());
    const float tex_clamp_x = tex_width - 1.0f;
    const float tex_clamp_y = tex_height - 1.0f;

    for (int y = yStart; y < yEnd; y++, itEdge0 += dv0, itEdge1 += dv1)
    {
        // calculate start and end pixels
        const int xStart = (int)ceil(itEdge0.pos.x - 0.5f);
        const int xEnd = (int)ceil(itEdge1.pos.x - 0.5f); // the pixel AFTER the last pixel drawn

        // calculate scanline dTexCoord / dx
        const open_3d::Vec2 dtcLine = (itEdge1.tc - itEdge0.tc) / (itEdge1.pos.x - itEdge0.pos.x);

        // create scanline tex coord interpolant and prestep
        open_3d::Vec2 itcLine = itEdge0.tc + dtcLine * (float(xStart) + 0.5f - itEdge0.pos.x);

        for (int x = xStart; x < xEnd; x++, itcLine += dtcLine)
        {
            PutPixel(x, y, tex.GetPixel(
                int(std::fmod(itcLine.x * tex_width, tex_clamp_x)),
                int(std::fmod(itcLine.y * tex_height, tex_clamp_y))));
            // need std::min b/c tc.x/y == 1.0, we'll read off edge of tex
            // and with fp err, tc.x/y can be > 1.0 (by a tiny amount)
        }
    }
}

void Graphics::DrawFlatTopTriangleTexWrap(const open_3d::TexVertex& v0, const open_3d::TexVertex& v1, const open_3d::TexVertex& v2, const Surface& tex) {
    // calulcate dVertex / dy
    const float delta_y = v2.pos.y - v0.pos.y;
    const open_3d::TexVertex dv0 = (v2 - v0) / delta_y;
    const open_3d::TexVertex dv1 = (v2 - v1) / delta_y;

    // create right edge interpolant
    open_3d::TexVertex itEdge1 = v1;

    // call the flat triangle render routine
    DrawFlatTriangleTexWrap(v0, v1, v2, tex, dv0, dv1, itEdge1);
}

void Graphics::DrawFlatBottomTriangleTexWrap(const open_3d::TexVertex& v0, const open_3d::TexVertex& v1, const open_3d::TexVertex& v2, const Surface& tex) {
    // calulcate dVertex / dy
    const float delta_y = v2.pos.y - v0.pos.y;
    const open_3d::TexVertex dv0 = (v1 - v0) / delta_y;
    const open_3d::TexVertex dv1 = (v2 - v0) / delta_y;

    // create right edge interpolant
    open_3d::TexVertex itEdge1 = v0;

    // call the flat triangle render routine
    DrawFlatTriangleTexWrap(v0, v1, v2, tex, dv0, dv1, itEdge1);
}

void Graphics::DrawTriangleTexWrap(const open_3d::TexVertex& v0, const open_3d::TexVertex& v1, const open_3d::TexVertex& v2, const Surface& tex) {
    // using pointers so we can swap (for sorting purposes)
    const open_3d::TexVertex* pv0 = &v0;
    const open_3d::TexVertex* pv1 = &v1;
    const open_3d::TexVertex* pv2 = &v2;

    // sorting vertices by y
    if (pv1->pos.y < pv0->pos.y) std::swap(pv0, pv1);
    if (pv2->pos.y < pv1->pos.y) std::swap(pv1, pv2);
    if (pv1->pos.y < pv0->pos.y) std::swap(pv0, pv1);

    if (pv0->pos.y == pv1->pos.y) // natural flat top
    {
        // sorting top vertices by x
        if (pv1->pos.x < pv0->pos.x) std::swap(pv0, pv1);
        DrawFlatTopTriangleTexWrap(*pv0, *pv1, *pv2, tex);
    }
    else if (pv1->pos.y == pv2->pos.y) // natural flat bottom
    {
        // sorting bottom vertices by x
        if (pv2->pos.x < pv1->pos.x) std::swap(pv1, pv2);
        DrawFlatBottomTriangleTexWrap(*pv0, *pv1, *pv2, tex);
    }
    else // general triangle
    {
        // find splitting vertex
        const float alphaSplit =
            (pv1->pos.y - pv0->pos.y) /
            (pv2->pos.y - pv0->pos.y);
        const open_3d::TexVertex vi = pv0->InterpolateTo(*pv2, alphaSplit);

        if (pv1->pos.x < vi.pos.x) // major right
        {
            DrawFlatBottomTriangleTexWrap(*pv0, *pv1, vi, tex);
            DrawFlatTopTriangleTexWrap(*pv1, vi, *pv2, tex);
        }
        else // major left
        {
            DrawFlatBottomTriangleTexWrap(*pv0, vi, *pv1, tex);
            DrawFlatTopTriangleTexWrap(vi, *pv1, *pv2, tex);
        }
    }
}
