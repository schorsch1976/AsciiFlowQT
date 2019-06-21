#include "ToolErase.h"

ToolErase::ToolErase(AsciiArtData &data) : m_data(data) {}

void ToolErase::OnMousePressed(QPoint /* point */) {}
void ToolErase::OnMouseMove(QRect area, QPoint /* point */) { m_marked = area; }
void ToolErase::OnMouseReleased(QPoint /* point */)
{
	for (int y = m_marked.y(); y < m_marked.y() + m_marked.height(); ++y)
	{
		for (int x = m_marked.x(); x < m_marked.x() + m_marked.width(); ++x)
		{
			m_data.Set(x, y, QChar::Space);
		}
	}
	m_marked = QRect();
}

void ToolErase::OnKeyPressed(QString /* c */) {}

QVector<QRect> ToolErase::MarkedAreas()
{
	QVector<QRect> ret;
	if (m_marked.width() > 0 && m_marked.height() > 0)
	{
		ret.push_back(m_marked);
	}
	return ret;
}
