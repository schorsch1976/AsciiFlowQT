#include "AsciiFlowCommon/ToolClass.h"
namespace Common
{
ToolClass::ToolClass(AsciiArtData &data) : m_data(data) {}

void ToolClass::OnMousePressed(Point /* point */) {}
void ToolClass::OnMouseMove(Rect area, Point /* point */)
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

	if (area.height > 2)
	{
		for (int x = area.x; x < area.x + area.width - 1; ++x)
		{
			m_data.Set(x, area.y + 2, '-');
		}
		m_data.Set(area.x, area.y + 2, '+');
		m_data.Set(area.x + area.width - 1, area.y + 2, '+');
	}
}
void ToolClass::OnMouseReleased(Point /* point */) {}

void ToolClass::OnKeyPressed(KeyEvent /* c */) {}

std::vector<Rect> ToolClass::MarkedAreas() { return std::vector<Rect>{}; }
} // namespace Common