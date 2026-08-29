#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp> 
#include <opencv2/core/utils/logger.hpp>
#include "Surface.hpp"

Surface::Surface(const std::string image_path) {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);

    _buffer_ptr = std::make_unique<cv::Mat>(cv::imread(image_path, cv::IMREAD_UNCHANGED));

    if (nullptr == _buffer_ptr.get() || _buffer_ptr->empty()) {
        throw std::runtime_error("Could not load image : " + image_path);
    }

    _width = _buffer_ptr->cols;
    _height = _buffer_ptr->rows;

    // Check channel count
    if (_buffer_ptr->channels() == 3) {
        // Convert RGB/BGR to BGRA/RGBA if it only has 3 channels
        cv::cvtColor(*_buffer_ptr, *_buffer_ptr, cv::COLOR_BGR2BGRA);
    }
}

Surface::Surface(unsigned int width, unsigned int height)
    :
    _buffer_ptr(std::make_unique<cv::Mat>(height, width, CV_8UC4)),
    _width(width),
    _height(height) {}

Surface::~Surface() {}

Surface::Surface(Surface&& source) noexcept
    :
    _buffer_ptr(std::move(source._buffer_ptr)),
    _width(source._width),
    _height(source._height) {}

Surface& Surface::operator=(Surface&& donor) noexcept {
    _width = donor._width;
    _height = donor._height;

    _buffer_ptr = std::move(donor._buffer_ptr);
    donor._buffer_ptr = nullptr;
    return *this;
}

unsigned int Surface::GetWidth() const {
    return _width;
}

unsigned int Surface::GetHeight() const {
    return _height;
}

cv::Mat* Surface::GetBufferPtr() noexcept {
    return _buffer_ptr.get();
}

void Surface::Clear(const Color color) {
    // Interpret the canvas as 32-bit pixels (CV_8UC4 assumed)
    uint32_t* ptr = _buffer_ptr->ptr<uint32_t>(0);

    // Total number of pixels
    size_t total = _buffer_ptr->rows * _buffer_ptr->cols;

    // Fill with the initial color (packed BGRA)
    std::fill(ptr, ptr + total, color.dword);
}

void Surface::PutPixel(int x, int y, const Color color) {
    assert(x >= 0);
    assert(y >= 0);
    assert(x < _width);
    assert(y < _height);

    // Pointer to the pixel (BGRA order in OpenCV)
    uint32_t* ptr = _buffer_ptr->ptr<uint32_t>(y);
    ptr[x] = color.dword;
}

Color Surface::GetPixel(int x, int y) const {
    assert(x >= 0);
    assert(y >= 0);
    assert(x < _width);
    assert(y < _height);

    const uint32_t* ptr = _buffer_ptr->ptr<uint32_t>(y);
    return ptr[x];
}

void Surface::PutPixelAlpha(int x, int y, Color color) {
    assert(x >= 0);
    assert(y >= 0);
    assert(x < _width);
    assert(y < _height);

    // load source pixel
    const Color background_color = GetPixel(x, y);

    // blend channels
    const unsigned char rsltRed = (color.GetR() * color.GetA() + background_color.GetR() * (255u - color.GetA())) / 256u;
    const unsigned char rsltGreen = (color.GetG() * color.GetA() + background_color.GetG() * (255u - color.GetA())) / 256u;
    const unsigned char rsltBlue = (color.GetB() * color.GetA() + background_color.GetB() * (255u - color.GetA())) / 256u;

    // pack channels back into pixel and fire pixel onto surface
    PutPixel(x, y, { rsltRed,rsltGreen,rsltBlue });
}
