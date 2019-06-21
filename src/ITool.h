#pragma once

#include <QPoint>
#include <QRect>
#include <QString>
#include <QVector>

#include "AsciiArtData.h"

enum class Tool
{
	Move,
	Rectangle,
	Class,
	Arrow,
	Text,
	Freehand,
	Erase
};

// Strategy
class ITool
{
public:
	virtual ~ITool();

	virtual void OnMousePressed(QPoint point) = 0;
	virtual void OnMouseMove(QRect area, QPoint point) = 0;
	virtual void OnMouseReleased(QPoint point) = 0;

	virtual void OnKeyPressed(QString c) = 0;

	virtual QVector<QRect> MarkedAreas() = 0;
};
