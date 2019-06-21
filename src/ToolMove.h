#pragma once

#include "AsciiArtData.h"
#include "ITool.h"

///////////////////////////////////////////////////////////////////////////////
// Tool Move
///////////////////////////////////////////////////////////////////////////////
class ToolMove : public ITool
{
public:
	explicit ToolMove(AsciiArtData &data);
	virtual void OnMousePressed(QPoint point) override;
	virtual void OnMouseMove(QRect area, QPoint point) override;
	virtual void OnMouseReleased(QPoint point) override;

	virtual void OnKeyPressed(QString c) override;

	virtual QVector<QRect> MarkedAreas() override;

private:
	AsciiArtData &m_data;

	QPoint m_start;
	QRect m_area;
	QRect m_current_area;

	struct Info
	{
		QPoint p;
		QChar c;
	};
	QVector<Info> m_moved_data;

	enum class State
	{
		NotMarked,
		Marked,
		Moving
	};
	State m_state;
};
