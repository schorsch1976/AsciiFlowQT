#include "AsciiFlowCommon/ToolRectangle.h"
namespace Common
{
ToolRectangle::ToolRectangle(AsciiArtData &data) : m_data(data) {}

void ToolRectangle::OnMousePressed(Point /* point */) {}
void ToolRectangle::OnMouseMove(Rect area, Point /* point */)
{
	for (int y = area.y; y < area.y + area.height - 1; ++y)
	{
		if (y == area.y || y == area.y + area.height - 2)
		{
			for (int x = area.x; x < area.x + area.width - 1; ++x)
			{
				m_data.Set(x, y, '-');
			}
			m_data.Set(area.x, y, '+');
			m_data.Set(area.x + area.width - 1, y, '+');
		}
		else
		{
			m_data.Set(area.x, y, '|');
			m_data.Set(area.x + area.width - 1, y, '|');
		}
	}
}
void ToolRectangle::OnMouseReleased(Point /* point */) {}

void ToolRectangle::OnKeyPressed(KeyEvent /* c */) {}

std::vector<Rect> ToolRectangle::MarkedAreas() { return std::vector<Rect>{}; }
} // namespace Common