#pragma once

#include "AsciiFlowCommon/AsciiFlowCommonAPI.h"

#include "AsciiFlowCommon/AsciiArtData.h"
#include "ITool.h"

///////////////////////////////////////////////////////////////////////////////
// Tool Erase
///////////////////////////////////////////////////////////////////////////////
namespace Common
{
class AsciiFlowCommon_API ToolErase : public ITool
{
public:
	explicit ToolErase(AsciiArtData &data);
	virtual void OnMousePressed(Point point) override;
	virtual void OnMouseMove(Rect area, Point point) override;
	virtual void OnMouseReleased(Point point) override;

	virtual void OnKeyPressed(KeyEvent c) override;

	virtual std::vector<Rect> MarkedAreas() override;

private:
	AsciiArtData &m_data;
	Rect m_marked;
};
} // namespace Common