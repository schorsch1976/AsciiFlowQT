#include "AsciiFlowCommon/ToolResize.h"
namespace Common
{
ToolResize::ToolResize(AsciiArtData &data) : m_data(data) {}

void ToolResize::OnMousePressed(Point point)
{
	m_start = point;

	if (m_data.At(m_start) == '+')
	{
		// S-------------1
		// |             |
		// 3-------------2
		Point p1_0;
		if (Find(1, 0, m_start, p1_0))
		{
			Point p1_1;
			if (Find(0, 1, p1_0, p1_1))
			{
				Point p1_2;
				if (Find(-1, 0, p1_1, p1_2))
				{
					Point p1_3;
					if (Find(0, -1, p1_2, p1_3))
					{
						if (p1_3 == point)
						{
							Selected(m_move_1, m_start, p1_0, p1_1, p1_2);
						}
					}
				}
			}
		}

		// 1-------------S
		// |             |
		// 2-------------3
		Point p2_0;
		if (Find(-1, 0, m_start, p2_0))
		{
			Point p2_1;
			if (Find(0, 1, p2_0, p2_1))
			{
				Point p2_2;
				if (Find(1, 0, p2_1, p2_2))
				{
					Point p2_3;
					if (Find(0, -1, p2_2, p2_3))
					{
						if (p2_3 == point)
						{
							Selected(m_move_2, m_start, p2_0, p2_1, p2_2);
						}
					}
				}
			}
		}

		// 3-------------2
		// |             |
		// S-------------1
		Point p3_0;
		if (Find(1, 0, m_start, p3_0))
		{
			Point p3_1;
			if (Find(0, -1, p3_0, p3_1))
			{
				Point p3_2;
				if (Find(-1, 0, p3_1, p3_2))
				{
					Point p3_3;
					if (Find(0, 1, p3_2, p3_3))
					{
						if (p3_3 == point)
						{
							Selected(m_move_3, m_start, p3_0, p3_1, p3_2);
						}
					}
				}
			}
		}

		// 2-------------3
		// |             |
		// 1-------------S
		Point p4_0;
		if (Find(-1, 0, m_start, p4_0))
		{
			Point p4_1;
			if (Find(0, -1, p4_0, p4_1))
			{
				Point p4_2;
				if (Find(1, 0, p4_1, p4_2))
				{
					Point p4_3;
					if (Find(0, 1, p4_2, p4_3))
					{
						if (p4_3 == point)
						{
							Selected(m_move_4, m_start, p4_0, p4_1, p4_2);
						}
					}
				}
			}
		}
	}
}
void ToolResize::OnMouseMove(Rect /* area */, Point point)
{
	for (auto &c : m_move_1)
	{
		m_data.Set(c, ' ');
	}
	for (auto &c : m_move_2)
	{
		m_data.Set(c, ' ');
	}
	for (auto &c : m_move_3)
	{
		m_data.Set(c, ' ');
	}
	for (auto &c : m_move_4)
	{
		m_data.Set(c, ' ');
	}

	std::vector<Point> edge_points;
	if (m_move_1.size())
	{
		Point fix_point = m_move_1[0];
		for (auto &c : m_move_1)
		{
			if (c.x > fix_point.x || c.y > fix_point.y)
			{
				fix_point = c;
			}
		}

		DrawRectangle(edge_points, fix_point, point);
	}

	if (m_move_2.size())
	{
		Point fix_point = m_move_2[0];
		for (auto &c : m_move_2)
		{
			if (c.x < fix_point.x || c.y > fix_point.y)
			{
				fix_point = c;
			}
		}
		DrawRectangle(edge_points, fix_point, point);
	}

	if (m_move_3.size())
	{
		Point fix_point = m_move_3[0];
		for (auto &c : m_move_3)
		{
			if (c.x > fix_point.x || c.y < fix_point.y)
			{
				fix_point = c;
			}
		}
		DrawRectangle(edge_points, fix_point, point);
	}

	if (m_move_4.size())
	{
		Point fix_point = m_move_4[0];
		for (auto &c : m_move_4)
		{
			if (c.x < fix_point.x || c.y < fix_point.y)
			{
				fix_point = c;
			}
		}
		DrawRectangle(edge_points, fix_point, point);
	}

	for (auto &c : edge_points)
	{
		m_data.Set(c, '+');
	}
}
void ToolResize::OnMouseReleased(Point /* point */)
{
	m_move_1.clear();
	m_move_2.clear();
	m_move_3.clear();
	m_move_4.clear();
}

void ToolResize::OnKeyPressed(KeyEvent /* c */) {}

std::vector<Rect> ToolResize::MarkedAreas()
{
	std::vector<Rect> ret;

	for (auto &c : m_move_1)
	{
		ret.push_back(Rect{c.x, c.y, 1, 1});
	}
	for (auto &c : m_move_2)
	{
		ret.push_back(Rect{c.x, c.y, 1, 1});
	}
	for (auto &c : m_move_3)
	{
		ret.push_back(Rect{c.x, c.y, 1, 1});
	}
	for (auto &c : m_move_4)
	{
		ret.push_back(Rect{c.x, c.y, 1, 1});
	}
	return ret;
}

bool ToolResize::Find(int dx, int dy, Point start, Point &res)
{
	Point current = start;

	if (m_data.At(start) != '+')
	{
		return false;
	}

	char linec;
	if (dx > 0 || dx < 0)
	{
		linec = '-';
	}
	else if (dy > 0 || dy < 0)
	{
		linec = '|';
	}

	while (
		(current.x <= m_data.Width() && dx > 0) || (current.x >= 0 && dx < 0) ||

		(current.y <= m_data.Height() && dy > 0) || (current.y >= 0 && dy < 0))
	{
		current.x = current.x + dx;
		current.y = current.y + dy;

		if (m_data.At(current) == '+')
		{
			// found
			res = current;
			return true;
		}
		else if (m_data.At(current) == linec)
		{
			// still on line
			continue;
		}
		else
		{
			// nothing here
			return false;
		}
	}

	return false;
}

void ToolResize::Selected(std::vector<Point> &target, Point p1, Point p2,
						  Point p3, Point p4)
{
	target.clear();

	Point s = p1;

	int dir_x = p1.x < p2.x ? 1 : -1;
	int dir_y = 0;
	while (s != p2)
	{
		target.push_back(s);
		s.x = (s.x + dir_x);
		s.y = (s.y + dir_y);
	}
	target.push_back(p2);

	dir_x = 0;
	dir_y = p2.y < p3.y ? 1 : -1;
	while (s != p3)
	{
		target.push_back(s);
		s.x = (s.x + dir_x);
		s.y = (s.y + dir_y);
	}
	target.push_back(p3);

	dir_x = p3.x < p4.x ? 1 : -1;
	dir_y = 0;
	while (s != p4)
	{
		target.push_back(s);
		s.x = (s.x + dir_x);
		s.y = (s.y + dir_y);
	}
	target.push_back(p4);

	dir_x = 0;
	dir_y = p4.y < p1.y ? 1 : -1;
	while (s != p1)
	{
		target.push_back(s);
		s.x = (s.x + dir_x);
		s.y = (s.y + dir_y);
	}
	target.push_back(p4);
}

void ToolResize::DrawRectangle(std::vector<Point> &edge_points, Point p1,
							   Point p2)
{
	int dx = p1.x < p2.x ? 1 : -1;
	int dy = p1.y < p2.y ? 1 : -1;

	for (int y = p1.y; y != p2.y; y += dy)
	{
		if (y == p1.y || y == p2.y)
		{
			for (int x = p1.x; x != p2.x; x += dx)
			{
				m_data.Set(x, y, '-');
			}
			m_data.Set(p1.x, y, '+');
			m_data.Set(p2.x, y, '+');
		}
		else
		{
			m_data.Set(p1.x, y, '|');
			m_data.Set(p2.x, y, '|');
		}
	}
	for (int x = p1.x; x != p2.x; x += dx)
	{
		m_data.Set(x, p2.y, '-');
		m_data.Set(p1.x, p2.y, '+');
		m_data.Set(p2.x, p2.y, '+');
	}

	edge_points.push_back(Point{p1.x, p1.y});
	edge_points.push_back(Point{p1.x, p2.y});

	edge_points.push_back(Point{p2.x, p1.y});
	edge_points.push_back(Point{p2.x, p2.y});
}
} // namespace Common