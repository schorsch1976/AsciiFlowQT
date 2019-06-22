#include "ToolResize.h"

ToolResize::ToolResize(AsciiArtData &data) : m_data(data) {}

void ToolResize::OnMousePressed(QPoint point)
{
	m_start = point;

	if (m_data.At(m_start) == '+')
	{
		// S-------------1
		// |             |
		// 3-------------2
		QPoint p1_0;
		if (Find(1, 0, m_start, p1_0))
		{
			QPoint p1_1;
			if (Find(0, 1, p1_0, p1_1))
			{
				QPoint p1_2;
				if (Find(-1, 0, p1_1, p1_2))
				{
					QPoint p1_3;
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
		QPoint p2_0;
		if (Find(-1, 0, m_start, p2_0))
		{
			QPoint p2_1;
			if (Find(0, 1, p2_0, p2_1))
			{
				QPoint p2_2;
				if (Find(1, 0, p2_1, p2_2))
				{
					QPoint p2_3;
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
		QPoint p3_0;
		if (Find(1, 0, m_start, p3_0))
		{
			QPoint p3_1;
			if (Find(0, -1, p3_0, p3_1))
			{
				QPoint p3_2;
				if (Find(-1, 0, p3_1, p3_2))
				{
					QPoint p3_3;
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
		QPoint p4_0;
		if (Find(-1, 0, m_start, p4_0))
		{
			QPoint p4_1;
			if (Find(0, -1, p4_0, p4_1))
			{
				QPoint p4_2;
				if (Find(1, 0, p4_1, p4_2))
				{
					QPoint p4_3;
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
void ToolResize::OnMouseMove(QRect /* area */, QPoint point)
{
	for (auto &c : m_move_1)
	{
		m_data.Set(c, QChar::Space);
	}
	for (auto &c : m_move_2)
	{
		m_data.Set(c, QChar::Space);
	}
	for (auto &c : m_move_3)
	{
		m_data.Set(c, QChar::Space);
	}
	for (auto &c : m_move_4)
	{
		m_data.Set(c, QChar::Space);
	}

	QVector<QPoint> edge_points;
	if (m_move_1.size())
	{
		QPoint fix_point = m_move_1[0];
		for (auto &c : m_move_1)
		{
			if (c.x() > fix_point.x() || c.y() > fix_point.y())
			{
				fix_point = c;
			}
		}

		DrawRectangle(edge_points, fix_point, point);
	}

	if (m_move_2.size())
	{
		QPoint fix_point = m_move_2[0];
		for (auto &c : m_move_2)
		{
			if (c.x() < fix_point.x() || c.y() > fix_point.y())
			{
				fix_point = c;
			}
		}
		DrawRectangle(edge_points, fix_point, point);
	}

	if (m_move_3.size())
	{
		QPoint fix_point = m_move_3[0];
		for (auto &c : m_move_3)
		{
			if (c.x() > fix_point.x() || c.y() < fix_point.y())
			{
				fix_point = c;
			}
		}
		DrawRectangle(edge_points, fix_point, point);
	}

	if (m_move_4.size())
	{
		QPoint fix_point = m_move_4[0];
		for (auto &c : m_move_4)
		{
			if (c.x() < fix_point.x() || c.y() < fix_point.y())
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
void ToolResize::OnMouseReleased(QPoint /* point */)
{
	m_move_1.clear();
	m_move_2.clear();
	m_move_3.clear();
	m_move_4.clear();
}

void ToolResize::OnKeyPressed(QString /* c */) {}

QVector<QRect> ToolResize::MarkedAreas()
{
	QVector<QRect> ret;

	for (auto &c : m_move_1)
	{
		ret.push_back(QRect(c.x(), c.y(), 1, 1));
	}
	for (auto &c : m_move_2)
	{
		ret.push_back(QRect(c.x(), c.y(), 1, 1));
	}
	for (auto &c : m_move_3)
	{
		ret.push_back(QRect(c.x(), c.y(), 1, 1));
	}
	for (auto &c : m_move_4)
	{
		ret.push_back(QRect(c.x(), c.y(), 1, 1));
	}
	return ret;
}

bool ToolResize::Find(int dx, int dy, QPoint start, QPoint &res)
{
	QPoint current = start;

	if (m_data.At(start) != '+')
	{
		return false;
	}

	QChar linec;
	if (dx > 0 || dx < 0)
	{
		linec = '-';
	}
	else if (dy > 0 || dy < 0)
	{
		linec = '|';
	}

	while ((current.x() <= m_data.Width() && dx > 0) ||
		   (current.x() >= 0 && dx < 0) ||

		   (current.y() <= m_data.Height() && dy > 0) ||
		   (current.y() >= 0 && dy < 0))
	{
		current.setX(current.x() + dx);
		current.setY(current.y() + dy);

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

void ToolResize::Selected(QVector<QPoint> &target, QPoint p1, QPoint p2,
						  QPoint p3, QPoint p4)
{
	target.clear();

	QPoint s = p1;

	int dir_x = p1.x() < p2.x() ? 1 : -1;
	int dir_y = 0;
	while (s != p2)
	{
		target.push_back(s);
		s.setX(s.x() + dir_x);
		s.setY(s.y() + dir_y);
	}
	target.push_back(p2);

	dir_x = 0;
	dir_y = p2.y() < p3.y() ? 1 : -1;
	while (s != p3)
	{
		target.push_back(s);
		s.setX(s.x() + dir_x);
		s.setY(s.y() + dir_y);
	}
	target.push_back(p3);

	dir_x = p3.x() < p4.x() ? 1 : -1;
	dir_y = 0;
	while (s != p4)
	{
		target.push_back(s);
		s.setX(s.x() + dir_x);
		s.setY(s.y() + dir_y);
	}
	target.push_back(p4);

	dir_x = 0;
	dir_y = p4.y() < p1.y() ? 1 : -1;
	while (s != p1)
	{
		target.push_back(s);
		s.setX(s.x() + dir_x);
		s.setY(s.y() + dir_y);
	}
	target.push_back(p4);
}

void ToolResize::DrawRectangle(QVector<QPoint> &edge_points, QPoint p1,
							   QPoint p2)
{
	int dx = p1.x() < p2.x() ? 1 : -1;
	int dy = p1.y() < p2.y() ? 1 : -1;

	for (int y = p1.y(); y != p2.y(); y += dy)
	{
		if (y == p1.y() || y == p2.y())
		{
			for (int x = p1.x(); x != p2.x(); x += dx)
			{
				m_data.Set(x, y, '-');
			}
			m_data.Set(p1.x(), y, '+');
			m_data.Set(p2.x(), y, '+');
		}
		else
		{
			m_data.Set(p1.x(), y, '|');
			m_data.Set(p2.x(), y, '|');
		}
	}
	for (int x = p1.x(); x != p2.x(); x += dx)
	{
		m_data.Set(x, p2.y(), '-');
		m_data.Set(p1.x(), p2.y(), '+');
		m_data.Set(p2.x(), p2.y(), '+');
	}

	edge_points.push_back(QPoint(p1.x(), p1.y()));
	edge_points.push_back(QPoint(p1.x(), p2.y()));

	edge_points.push_back(QPoint(p2.x(), p1.y()));
	edge_points.push_back(QPoint(p2.x(), p2.y()));
}
