#pragma once

#include "AsciiFlowCommon/AsciiFlowCommonAPI.h"

#include "AsciiFlowCommon/Point.h"

namespace Common
{
struct AsciiFlowCommon_API Rect
{
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;

	bool Contains(Point c) const;
};
} // namespace Common