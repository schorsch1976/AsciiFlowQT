#include "ToolRectangle.h"

ToolRectangle::ToolRectangle(AsciiArtData &data) : m_data(data) {}

void ToolRectangle::OnMousePressed(QPoint /* point */) {}
void ToolRectangle::OnMouseMove(QRect area, QPoint /* point */)
{
	for (int y = area.y(); y < area.y() + area.height() - 1; ++y)
	{
		if (y == area.y() || y == area.y() + area.height() - 2)
		{
			for (int x = area.x(); x < area.x() + area.width() - 1; ++x)
			{
				m_data.Set(x, y, '-');
			}
			m_data.Set(area.x(), y, '+');
			m_data.Set(area.x() + area.width() - 1, y, '+');
		}
		else
		{
			m_data.Set(area.x(), y, '|');
			m_data.Set(area.x() + area.width() - 1, y, '|');
		}
	}
}
void ToolRectangle::OnMouseReleased(QPoint /* point */) {}

void ToolRectangle::OnKeyPressed(QString /* c */) {}

QVector<QRect> ToolRectangle::MarkedAreas() { return QVector<QRect>(); }
