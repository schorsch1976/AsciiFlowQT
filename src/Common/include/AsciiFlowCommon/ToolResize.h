#pragma once

#include "AsciiFlowCommon/AsciiFlowCommonAPI.h"

#include "AsciiArtData.h"
#include "ITool.h"

///////////////////////////////////////////////////////////////////////////////
// Tool Resize
///////////////////////////////////////////////////////////////////////////////
namespace Common
{
class AsciiFlowCommon_API ToolResize : public ITool
{
public:
	explicit ToolResize(AsciiArtData &data);
	virtual void OnMousePressed(Point point) override;
	virtual void OnMouseMove(Rect area, Point point) override;
	virtual void OnMouseReleased(Point point) override;

	virtual void OnKeyPressed(KeyEvent c) override;

	virtual std::vector<Rect> MarkedAreas() override;

private:
	bool Find(int dx, int dy, Point start, Point &res);
	void Selected(std::vector<Point> &target, Point p1, Point p2, Point p3,
				  Point p4);

	void DrawRectangle(std::vector<Point> &edge_points, Point p1, Point p2);

	AsciiArtData &m_data;
	Point m_start;

	std::vector<Point> m_move_1;
	std::vector<Point> m_move_2;
	std::vector<Point> m_move_3;
	std::vector<Point> m_move_4;
};
} // namespace Common