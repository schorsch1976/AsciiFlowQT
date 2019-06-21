#pragma once

#include "AsciiArtData.h"
#include "ITool.h"

///////////////////////////////////////////////////////////////////////////////
// Tool Freehand
///////////////////////////////////////////////////////////////////////////////
class ToolFreehand : public ITool
{
public:
	explicit ToolFreehand(AsciiArtData &data);
	virtual void OnMousePressed(QPoint point) override;
	virtual void OnMouseMove(QRect area, QPoint point) override;
	virtual void OnMouseReleased(QPoint point) override;

	virtual void OnKeyPressed(QString c) override;

	virtual QVector<QRect> MarkedAreas() override;

private:
	void Draw();
	AsciiArtData &m_data;
	QVector<QPoint> m_spots;
};
