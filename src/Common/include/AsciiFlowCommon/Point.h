#pragma once

#include "AsciiFlowCommon/AsciiFlowCommonAPI.h"

#include <compare>
#include <cstdint>

namespace Common
{
struct AsciiFlowCommon_API Point
{
	int32_t x = 0;
	int32_t y = 0;

	Point operator+(const Point &rhs) const;
	Point operator-(const Point &rhs) const;

	auto operator<=>(const Point &) const = default;
};

} // namespace Common
