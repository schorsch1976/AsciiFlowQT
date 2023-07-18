#pragma once

#include "AsciiFlowCommon/AsciiFlowCommonAPI.h"

#include "AsciiArtData.h"
#include "ITool.h"

///////////////////////////////////////////////////////////////////////////////
// Tool Text
///////////////////////////////////////////////////////////////////////////////
namespace Common
{
class AsciiFlowCommon_API ToolText : public ITool
{
public:
	explicit ToolText(AsciiArtData &data);
	virtual void OnMousePressed(Point point) override;
	virtual void OnMouseMove(Rect area, Point point) override;
	virtual void OnMouseReleased(Point point) override;

	virtual void OnKeyPressed(KeyEvent c) override;

	virtual std::vector<Rect> MarkedAreas() override;

private:
	AsciiArtData &m_data;
	Point m_start;
	Point m_cursor;
};
} // namespace Common