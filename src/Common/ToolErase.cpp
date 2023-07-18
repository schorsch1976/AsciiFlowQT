#include "AsciiFlowCommon/ToolErase.h"

namespace Common
{
ToolErase::ToolErase(AsciiArtData &data) : m_data(data) {}

void ToolErase::OnMousePressed(Point /* point */) {}
void ToolErase::OnMouseMove(Rect area, Point /* point */) { m_marked = area; }
void ToolErase::OnMouseReleased(Point /* point */)
{
	for (int y = m_marked.y; y < m_marked.y + m_marked.height; ++y)
	{
		for (int x = m_marked.x; x < m_marked.x + m_marked.width; ++x)
		{
			m_data.Set(x, y, ' ');
		}
	}
	m_marked = Rect();
}

void ToolErase::OnKeyPressed(KeyEvent /* c */) {}

std::vector<Rect> ToolErase::MarkedAreas()
{
	std::vector<Rect> ret;
	if (m_marked.width > 0 && m_marked.height > 0)
	{
		ret.push_back(m_marked);
	}
	return ret;
}
} // namespace Common