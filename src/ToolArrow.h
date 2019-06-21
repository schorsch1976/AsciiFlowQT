#pragma once

#include "AsciiArtData.h"
#include "ITool.h"

///////////////////////////////////////////////////////////////////////////////
// Tool Arrow
///////////////////////////////////////////////////////////////////////////////
class ToolArrow : public ITool
{
public:
	explicit ToolArrow(AsciiArtData &data);
	virtual void OnMousePressed(QPoint point) override;
	virtual void OnMouseMove(QRect area, QPoint point) override;
	virtual void OnMouseReleased(QPoint point) override;

	virtual void OnKeyPressed(QString c) override;

	virtual QVector<QRect> MarkedAreas() override;

private:
	AsciiArtData &m_data;
	QPoint m_start;
};
