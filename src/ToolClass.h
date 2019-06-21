#pragma once

#include "AsciiArtData.h"
#include "ITool.h"

///////////////////////////////////////////////////////////////////////////////
// Tool Class
///////////////////////////////////////////////////////////////////////////////
class ToolClass : public ITool
{
public:
	explicit ToolClass(AsciiArtData &data);
	virtual void OnMousePressed(QPoint point) override;
	virtual void OnMouseMove(QRect area, QPoint point) override;
	virtual void OnMouseReleased(QPoint point) override;

	virtual void OnKeyPressed(QString c) override;

	virtual QVector<QRect> MarkedAreas() override;

private:
	AsciiArtData &m_data;
};
