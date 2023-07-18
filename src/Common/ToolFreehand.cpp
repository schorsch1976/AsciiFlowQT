#include "AsciiFlowCommon/ToolFreehand.h"
namespace Common
{
ToolFreehand::ToolFreehand(AsciiArtData &data) : m_data(data) {}

void ToolFreehand::OnMousePressed(Point point)
{
	m_spots.clear();
	m_spots.push_back(point);
	Draw();
}
void ToolFreehand::OnMouseMove(Rect /* area */, Point point)
{
	m_spots.push_back(point);
	Draw();
}
void ToolFreehand::OnMouseReleased(Point point)
{
	m_spots.push_back(point);
	Draw();
	m_spots.clear();
}

void ToolFreehand::OnKeyPressed(KeyEvent /* c */) {}

std::vector<Rect> ToolFreehand::MarkedAreas() { return std::vector<Rect>{}; }

void ToolFreehand::Draw()
{
	for (auto &m_spot : m_spots)
	{
		m_data.Set(m_spot, 'X');
	}
}
} // namespace Common