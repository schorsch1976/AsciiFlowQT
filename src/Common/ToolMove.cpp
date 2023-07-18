#include "AsciiFlowCommon/ToolMove.h"
namespace Common
{
ToolMove::ToolMove(AsciiArtData &data)
	: m_data(data), m_start{-1, -1}, m_state(State::NotMarked)
{
}
void ToolMove::OnMousePressed(Point point)
{
	switch (m_state)
	{
		case State::NotMarked:
			m_start = point;
			break;
		case State::Marked:
			if (m_area.Contains(point))
			{
				m_state = State::Moving;
				m_start = point;
				// copy data
				m_moved_data.clear();
				for (int y = m_area.y; y < m_area.y + m_area.height; ++y)
				{
					for (int x = m_area.x; x < m_area.x + m_area.width; ++x)
					{
						Point p{x, y};
						m_moved_data.push_back({p, m_data.At(p)});
					}
				}
			}
			else
			{
				m_state = State::NotMarked;
			}
			break;
		case State::Moving:
			break;
	}
}
void ToolMove::OnMouseMove(Rect area, Point point)
{
	switch (m_state)
	{
		case State::NotMarked:
			m_area = area;
			break;
		case State::Marked:
			break;
		case State::Moving:
			// clear data
			for (Info &c : m_moved_data)
			{
				m_data.Set(c.p, ' ');
			}

			Point delta = point - m_start;
			m_current_area = Rect{m_area.x + delta.x, m_area.y + delta.y,
								  m_area.width, m_area.height};
			for (Info &c : m_moved_data)
			{
				m_data.Set(c.p + delta, c.c);
			}
			break;
	}
}
void ToolMove::OnMouseReleased(Point /*point*/)
{
	switch (m_state)
	{
		case State::NotMarked:
			m_state = State::Marked;
			break;
		case State::Marked:
		case State::Moving:
			m_state = State::NotMarked;
			m_area = m_current_area = Rect();
			break;
	}
}

void ToolMove::OnKeyPressed(KeyEvent /* c */) {}

std::vector<Rect> ToolMove::MarkedAreas()
{
	std::vector<Rect> ret;
	switch (m_state)
	{
		case State::NotMarked:
		case State::Marked:
			ret.push_back(m_area);
			break;
		case State::Moving:
			ret.push_back(m_area);
			ret.push_back(m_current_area);
			break;
	}

	return ret;
}
} // namespace Common