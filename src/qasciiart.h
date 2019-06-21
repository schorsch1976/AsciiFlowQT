#ifndef QASCIIART_H
#define QASCIIART_H

#include "ITool.h"

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QPoint>
#include <QSharedPointer>
#include <QString>
#include <QWidget>
#include <tuple>

class QAsciiArt : public QWidget
{
	Q_OBJECT
public:
	explicit QAsciiArt(QWidget *parent = nullptr);

	// Actions from toolbar
	QString Export();
	void Import(QString data);
	void Clear();

	// sliding
	QPoint CurrentSlideStart();
	void SetSlideStart(QPoint point);

protected:
	// Override from QWidget
	virtual void paintEvent(QPaintEvent *event) override;

	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent *event) override;
	virtual void mouseReleaseEvent(QMouseEvent *event) override;

	virtual void keyPressEvent(QKeyEvent *event) override;

	virtual void resizeEvent(QResizeEvent *event) override;

public:
signals:
	void ActiveTool(Tool tool);
	void UndoAvail(bool status);
	void RedoAvail(bool status);

	void DataAreaChanged(QPoint size);

public slots:
	void ActivateToolMove();
	void ActivateToolRectangle();
	void ActivateToolClass();
	void ActivateToolArrow();
	void ActivateToolLine();
	void ActivateToolText();
	void ActivateToolFreehand();
	void ActivateToolErase();

	void Undo();
	void Redo();

	void OnDataAreaChanged(QPoint size);

private:
	void ActivateToolHelper(Tool tool);

	QPoint ScreenToText(QPoint screen);
	QPoint TextToScreen(QPoint txt);

private:
	QSharedPointer<ITool> mp_current_tool;
	Tool m_current_tool;

	static const int CELLSIZE = 12;
	static const int FONTSIZE = CELLSIZE - 2;

	// sliding
	QPoint m_slide_start;

	// The data area
	QPoint m_start;
	AsciiArtData m_data;
	AsciiArtData m_data_backup;

	QVector<QString> m_undo;
	QVector<QString> m_redo;

	// for drawing
	QFont m_font;
	QBrush m_solid;
};

#endif // QASCIIART_H
