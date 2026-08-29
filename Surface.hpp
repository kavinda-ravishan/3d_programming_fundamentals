#pragma once

#include <memory>
#include <string>
#include "Colors.hpp"

namespace cv {
	class Mat;
}

class Surface {
public:
	Surface(const std::string image_path);
	Surface(unsigned int width, unsigned int height);
	~Surface();

	Surface(Surface&& source) noexcept;
	Surface(Surface&) = delete;
	Surface& operator=(Surface&& donor) noexcept;
	Surface& operator=(const Surface&) = delete;

	unsigned int GetWidth() const;
	unsigned int GetHeight() const;

	cv::Mat* GetBufferPtr() noexcept;

	void Clear(const open_3d::Color color);

	void PutPixel(int x, int y, const open_3d::Color color);
	open_3d::Color GetPixel(int x, int y) const;
	void PutPixelAlpha(int x, int y, open_3d::Color color);

	static Surface FromFile(const std::string& name);
private:
	unsigned int _width{};
	unsigned int _height{};
	std::unique_ptr<cv::Mat> _buffer_ptr{};
};
