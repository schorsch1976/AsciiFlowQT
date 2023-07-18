#pragma once

#include "AsciiFlowCommon/AsciiFlowCommonAPI.h"

#include "AsciiFlowCommon/KeyEvent.h"
#include "AsciiFlowCommon/Point.h"
#include "AsciiFlowCommon/Rect.h"

#include <vector>

namespace Common
{
enum class Tool
{
	Move,
	Rectangle,
	Resize,
	Class,
	Arrow,
	Line,
	Text,
	Freehand,
	Erase
};

// Strategy
class AsciiFlowCommon_API ITool
{
public:
	virtual ~ITool() = default;

	virtual void OnMousePressed(Point point) = 0;
	virtual void OnMouseMove(Rect area, Point point) = 0;
	virtual void OnMouseReleased(Point point) = 0;

	virtual void OnKeyPressed(KeyEvent c) = 0;

	virtual std::vector<Rect> MarkedAreas() = 0;
};

} // namespace Common