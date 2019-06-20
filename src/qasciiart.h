#ifndef QASCIIART_H
#define QASCIIART_H

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QPoint>
#include <QSharedPointer>
#include <QString>
#include <QWidget>
#include <tuple>

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

public:
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
