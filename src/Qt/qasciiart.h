#pragma once

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QPoint>
#include <QSharedPointer>
#include <QString>
#include <QWidget>
#include <tuple>

#include "AsciiFlowCommon/AsciiArtData.h"
#include "AsciiFlowCommon/ITool.h"

class QAsciiArt : public QWidget
{
	Q_OBJECT
public:
	explicit QAsciiArt(QWidget *parent = nullptr);

	// Actions from toolbar
	QString Export();
	void Import(const QString &data);
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
	void ActiveTool(Common::Tool tool);
	void UndoAvail(bool status);
	void RedoAvail(bool status);

	void DataAreaChanged(QPoint size);

public slots:
	void ActivateToolMove();
	void ActivateToolRectangle();
	void ActivateToolResize();
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
	void ActivateToolHelper(Common::Tool tool);

	QPoint ScreenToText(QPoint point);
	QPoint TextToScreen(QPoint point);

private:
	QSharedPointer<Common::ITool> mp_current_tool;
	Common::Tool m_current_tool;

	static constexpr int CELLSIZE = 12;
	static constexpr int FONTSIZE = CELLSIZE - 2;

	// sliding
	QPoint m_slide_start;

	// The data area
	QPoint m_start;
	Common::AsciiArtData m_data;
	Common::AsciiArtData m_data_backup;

	QVector<QString> m_undo;
	QVector<QString> m_redo;

	// for drawing
	QFont m_font;
	QBrush m_solid;
};
