#pragma once

#include "AsciiFlowCommon/AsciiFlowCommonAPI.h"

#include "AsciiArtData.h"
#include "ITool.h"

///////////////////////////////////////////////////////////////////////////////
// Tool Move
///////////////////////////////////////////////////////////////////////////////
namespace Common
{
class AsciiFlowCommon_API ToolMove : public ITool
{
public:
	explicit ToolMove(AsciiArtData &data);
	virtual void OnMousePressed(Point point) override;
	virtual void OnMouseMove(Rect area, Point point) override;
	virtual void OnMouseReleased(Point point) override;

	virtual void OnKeyPressed(KeyEvent c) override;

	virtual std::vector<Rect> MarkedAreas() override;

private:
	AsciiArtData &m_data;

	Point m_start;
	Rect m_area;
	Rect m_current_area;

	struct Info
	{
		Point p;
		char c;
	};
	std::vector<Info> m_moved_data;

	enum class State
	{
		NotMarked,
		Marked,
		Moving
	};
	State m_state;
};
} // namespace Common