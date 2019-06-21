#include "qasciiart.h"

#include <QChar>
#include <QFontDatabase>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>

#include "ToolArrow.h"
#include "ToolClass.h"
#include "ToolErase.h"
#include "ToolFreehand.h"
#include "ToolLine.h"
#include "ToolMove.h"
#include "ToolRectangle.h"
#include "ToolText.h"

///////////////////////////////////////////////////////////////////////////////
// The Widget
///////////////////////////////////////////////////////////////////////////////
QAsciiArt::QAsciiArt(QWidget *parent)
	: QWidget(parent), m_data(this), m_data_backup(this)
{
	// initialize stuff for drawing
	m_font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
	m_font.setPointSize(FONTSIZE);

	m_solid = QBrush(Qt::SolidLine);

	m_slide_start = QPoint(0, 0);

	connect(&m_data, &AsciiArtData::DataAreaChanged, this,
			&QAsciiArt::OnDataAreaChanged);

	ActivateToolHelper(Tool::Line);

	setFocusPolicy(Qt::FocusPolicy::StrongFocus);
}

// Actions from toolbar
QString QAsciiArt::Export()
{
	// issue repaint
	update();
	return m_data.ExportData();
}
void QAsciiArt::Import(QString data)
{
	m_data.ImportData(data);

	// issue repaint
	update();
}
void QAsciiArt::Clear()
{
	m_data.Clear();
	m_data_backup = m_data;
	m_undo.clear();
	m_redo.clear();

	// issue repaint
	update();
}

QPoint QAsciiArt::CurrentSlideStart() { return m_slide_start; }
void QAsciiArt::SetSlideStart(QPoint point) { m_slide_start = point; }

// Override from QWidget

void QAsciiArt::paintEvent(QPaintEvent * /* event */)
{
	QPainter painter(this);
	painter.eraseRect(0, 0, width() - 1, height() - 1);
	painter.setRenderHint(QPainter::Antialiasing, true);

	// background
	{
		QBrush brush;
		brush.setColor(Qt::white);
		brush.setStyle(Qt::BrushStyle::SolidPattern);
		painter.setBrush(brush);

		QPoint screenpos1 = TextToScreen(QPoint(0, 0));
		QPoint screenpos2 =
			TextToScreen(QPoint(m_data.Width(), m_data.Height()));

		QRect screenpos(screenpos1.x(), screenpos1.y(),
						screenpos2.x() - screenpos1.x(),
						screenpos2.y() - screenpos1.y());
		painter.drawRect(screenpos);
	}

	// background grid
	{
		QPen pen;
		pen.setColor(Qt::lightGray);
		pen.setWidthF(0.5);
		painter.setPen(pen);

		for (int x = 0; x < m_data.Width(); ++x)
		{
			QPoint screenpos1 = TextToScreen(QPoint(x, 0));
			QPoint screenpos2 = TextToScreen(QPoint(x, m_data.Height()));
			painter.drawLine(screenpos1, screenpos2);
		}

		for (int y = 0; y < m_data.Height(); ++y)
		{
			QPoint screenpos1 = TextToScreen(QPoint(0, y));
			QPoint screenpos2 = TextToScreen(QPoint(m_data.Width(), y));
			painter.drawLine(screenpos1, screenpos2);
		}
	}

	// selection
	{
		QBrush filled;
		filled.setStyle(Qt::BrushStyle::DiagCrossPattern);
		filled.setColor(Qt::lightGray);
		painter.setBrush(filled);

		QPen pen;
		pen.setColor(Qt::lightGray);
		pen.setWidthF(0.5);
		painter.setPen(pen);

		QVector<QRect> selected = mp_current_tool->MarkedAreas();
		for (QRect &rect : selected)
		{
			QPoint screenpos1 = TextToScreen(QPoint(rect.x(), rect.y()));
			QPoint screenpos2 = TextToScreen(
				QPoint(rect.x() + rect.width(), rect.y() + rect.height()));

			QRect screenpos(screenpos1.x(), screenpos1.y(),
							screenpos2.x() - screenpos1.x(),
							screenpos2.y() - screenpos1.y());
			painter.drawRect(screenpos);
		}
	}

	// Text
	{
		QBrush brush;
		brush.setColor(Qt::black);
		painter.setBrush(brush);

		QPen pen;
		pen.setColor(Qt::black);
		pen.setWidthF(1);
		painter.setPen(pen);

		for (int y = 0; y < m_data.Height(); ++y)
		{
			for (int x = 0; x < m_data.Width(); ++x)
			{
				QPoint screenpos = TextToScreen(QPoint(x, y));

				// text point is at the lower line
				painter.drawText(screenpos.x() + 1,
								 screenpos.y() + CELLSIZE - 1,
								 m_data.At(QPoint(x, y)));
			}
		}
	}
}

void QAsciiArt::mousePressEvent(QMouseEvent *event)
{
	QPoint HitText = ScreenToText(QPoint(event->x(), event->y()));
	m_start = HitText;

	m_data_backup = m_data;

	m_redo.clear();
	m_undo.push_back(m_data_backup.ExportData());

	mp_current_tool->OnMousePressed(HitText);
	update();

	emit RedoAvail(m_redo.size() > 0);
	emit UndoAvail(m_undo.size() > 0);
}
void QAsciiArt::mouseMoveEvent(QMouseEvent *event)
{
	QPoint HitText = ScreenToText(QPoint(event->x(), event->y()));

	int width = std::abs(HitText.x() - m_start.x() + 1);
	int height = std::abs(HitText.y() - m_start.y() + 1);
	int x = std::min(HitText.x(), m_start.x());
	int y = std::min(HitText.y(), m_start.y());

	QRect area(x, y, width, height);

	m_data = m_data_backup;
	mp_current_tool->OnMouseMove(area, HitText);

	update();
}
void QAsciiArt::mouseReleaseEvent(QMouseEvent *event)
{
	QPoint HitText = ScreenToText(QPoint(event->x(), event->y()));
	mp_current_tool->OnMouseReleased(HitText);
	update();
}

void QAsciiArt::keyPressEvent(QKeyEvent *event)
{
	QString txt = event->text();
	mp_current_tool->OnKeyPressed(txt);
	update();
}

void QAsciiArt::resizeEvent(QResizeEvent *event)
{
	QPoint on_screen_size =
		TextToScreen(QPoint(m_data.Width(), m_data.Height()));
	this->setMinimumSize(on_screen_size.x(), on_screen_size.y());
	this->setMaximumSize(on_screen_size.x(), on_screen_size.y());
}

void QAsciiArt::ActivateToolMove() { ActivateToolHelper(Tool::Move); }
void QAsciiArt::ActivateToolRectangle() { ActivateToolHelper(Tool::Rectangle); }
void QAsciiArt::ActivateToolClass() { ActivateToolHelper(Tool::Class); }
void QAsciiArt::ActivateToolArrow() { ActivateToolHelper(Tool::Arrow); }
void QAsciiArt::ActivateToolLine() { ActivateToolHelper(Tool::Line); }
void QAsciiArt::ActivateToolText() { ActivateToolHelper(Tool::Text); }

void QAsciiArt::ActivateToolFreehand() { ActivateToolHelper(Tool::Freehand); }
void QAsciiArt::ActivateToolErase() { ActivateToolHelper(Tool::Erase); }

void QAsciiArt::Undo()
{
	if (m_undo.size() > 0)
	{
		QString tmp = *m_undo.rbegin();
		m_undo.pop_back();

		m_redo.push_back(m_data.ExportData());
		m_data.ImportData(tmp);
		update();
	}

	emit RedoAvail(m_redo.size() > 0);
	emit UndoAvail(m_undo.size() > 0);
}
void QAsciiArt::Redo()
{
	if (m_redo.size() > 0)
	{
		QString tmp = *m_redo.rbegin();
		m_redo.pop_back();

		m_undo.push_back(m_data.ExportData());
		m_data.ImportData(tmp);
		update();
	}

	emit RedoAvail(m_redo.size() > 0);
	emit UndoAvail(m_undo.size() > 0);
}

void QAsciiArt::OnDataAreaChanged(QPoint size)
{
	QPoint on_screen_size = TextToScreen(size);
	this->setMinimumSize(on_screen_size.x(), on_screen_size.y());
	this->setMaximumSize(on_screen_size.x(), on_screen_size.y());
	emit DataAreaChanged(on_screen_size);
}

void QAsciiArt::ActivateToolHelper(Tool tool)
{
	m_current_tool = tool;
	switch (m_current_tool)
	{
		case Tool::Move:
			mp_current_tool.reset(new ToolMove(m_data));
			break;
		case Tool::Rectangle:
			mp_current_tool.reset(new ToolRectangle(m_data));
			break;
		case Tool::Class:
			mp_current_tool.reset(new ToolClass(m_data));
			break;
		case Tool::Arrow:
			mp_current_tool.reset(new ToolArrow(m_data));
			break;
		case Tool::Line:
			mp_current_tool.reset(new ToolLine(m_data));
			break;
		case Tool::Text:
			mp_current_tool.reset(new ToolText(m_data));
			break;
		case Tool::Freehand:
			mp_current_tool.reset(new ToolFreehand(m_data));
			break;
		case Tool::Erase:
			mp_current_tool.reset(new ToolErase(m_data));
			break;
	}

	update();
	emit ActiveTool(m_current_tool);
}

QPoint QAsciiArt::ScreenToText(QPoint point)
{
	int x = point.x() / CELLSIZE;
	int y = point.y() / CELLSIZE;
	if (x < 0)
	{
		x = 0;
	}
	if (y < 0)
	{
		y = 0;
	}
	return QPoint(x, y);
}
QPoint QAsciiArt::TextToScreen(QPoint point)
{
	return QPoint(point.x() * CELLSIZE, point.y() * CELLSIZE);
}
