#pragma once

#include "AsciiFlowCommon/AsciiFlowCommonAPI.h"

#include "AsciiArtData.h"
#include "ITool.h"

///////////////////////////////////////////////////////////////////////////////
// Tool Freehand
///////////////////////////////////////////////////////////////////////////////
namespace Common
{
class AsciiFlowCommon_API ToolFreehand : public ITool
{
public:
	explicit ToolFreehand(AsciiArtData &data);
	virtual void OnMousePressed(Point point) override;
	virtual void OnMouseMove(Rect area, Point point) override;
	virtual void OnMouseReleased(Point point) override;

	virtual void OnKeyPressed(KeyEvent c) override;

	virtual std::vector<Rect> MarkedAreas() override;

private:
	void Draw();
	AsciiArtData &m_data;
	std::vector<Point> m_spots;
};
} // namespace Common