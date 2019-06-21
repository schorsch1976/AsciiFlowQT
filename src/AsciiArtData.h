#pragma once

#include <QObject>
#include <QPoint>
#include <QString>
#include <QVector>
#include <QWidget>

#include "ITool.h"

class AsciiArtData : public QObject
{
	Q_OBJECT
public:
	// rule of 5
	explicit AsciiArtData(QWidget *parent);
	virtual ~AsciiArtData();

	AsciiArtData(const AsciiArtData &rhs);
	AsciiArtData &operator=(const AsciiArtData &rhs);

	AsciiArtData(AsciiArtData &&rhs);
	AsciiArtData &operator=(AsciiArtData &&rhs);

	// access the data
	QString ExportData();
	void ImportData(QString data);

	void Set(QPoint p, QChar c);
	void Set(int x, int y, QChar c);
	QChar At(QPoint p);
	QChar At(int x, int y);

	void Clear();

	int Width();
	int Height();

signals:
	void DataAreaChanged(QPoint size);

private:
	void ResizeData(QPoint point);
	void CheckData();

	int m_width;
	int m_height;
	QVector<QString> m_data;
};
