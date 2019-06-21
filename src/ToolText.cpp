#include "ToolText.h"

ToolText::ToolText(AsciiArtData &data) : m_data(data) {}

void ToolText::OnMousePressed(QPoint point) { m_cursor = point; }
void ToolText::OnMouseMove(QRect /* area */, QPoint point) { m_cursor = point; }
void ToolText::OnMouseReleased(QPoint /* point */) {}

void ToolText::OnKeyPressed(QString c)
{
	int delta_x = 0;
	for (int i = 0; i < c.size(); ++i)
	{
		if (c[i].isPrint() && c[i].toLatin1())
		{
			m_data.Set(m_cursor.x() + i, m_cursor.y(), c[i]);
			delta_x++;
		}
	}
	m_cursor.setX(m_cursor.x() + delta_x);
}

QVector<QRect> ToolText::MarkedAreas()
{
	QVector<QRect> ret;
	ret.push_back(QRect(m_cursor.x(), m_cursor.y(), 1, 1));
	return ret;
}
