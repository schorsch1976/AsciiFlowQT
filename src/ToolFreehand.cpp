#include "ToolFreehand.h"

ToolFreehand::ToolFreehand(AsciiArtData &data) : m_data(data) {}

void ToolFreehand::OnMousePressed(QPoint point)
{
	m_spots.clear();
	m_spots.push_back(point);
	Draw();
}
void ToolFreehand::OnMouseMove(QRect /* area */, QPoint point)
{
	m_spots.push_back(point);
	Draw();
}
void ToolFreehand::OnMouseReleased(QPoint point)
{
	m_spots.push_back(point);
	Draw();
	m_spots.clear();
}

void ToolFreehand::OnKeyPressed(QString /* c */) {}

QVector<QRect> ToolFreehand::MarkedAreas() { return QVector<QRect>(); }

void ToolFreehand::Draw()
{
	for (int i = 0; i < m_spots.size(); ++i)
	{
		m_data.Set(m_spots[i], 'X');
	}
}
