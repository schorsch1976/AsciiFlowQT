#pragma once

#include "AsciiArtData.h"
#include "ITool.h"

///////////////////////////////////////////////////////////////////////////////
// Tool Resize
///////////////////////////////////////////////////////////////////////////////
class ToolResize : public ITool
{
public:
	explicit ToolResize(AsciiArtData &data);
	virtual void OnMousePressed(QPoint point) override;
	virtual void OnMouseMove(QRect area, QPoint point) override;
	virtual void OnMouseReleased(QPoint point) override;

	virtual void OnKeyPressed(QString c) override;

	virtual QVector<QRect> MarkedAreas() override;

private:
	bool Find(int dx, int dy, QPoint start, QPoint &res);
	void Selected(QVector<QPoint> &target, QPoint p1, QPoint p2, QPoint p3,
				  QPoint p4);

	void DrawRectangle(QVector<QPoint> &edge_points, QPoint p1, QPoint p2);

	AsciiArtData &m_data;
	QPoint m_start;

	QVector<QPoint> m_move_1;
	QVector<QPoint> m_move_2;
	QVector<QPoint> m_move_3;
	QVector<QPoint> m_move_4;
};
