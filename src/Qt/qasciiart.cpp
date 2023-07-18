#include "qasciiart.h"

#include <QChar>
#include <QDebug>
#include <QFontDatabase>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>

#include "AsciiFlowCommon/ToolArrow.h"
#include "AsciiFlowCommon/ToolClass.h"
#include "AsciiFlowCommon/ToolErase.h"
#include "AsciiFlowCommon/ToolFreehand.h"
#include "AsciiFlowCommon/ToolLine.h"
#include "AsciiFlowCommon/ToolMove.h"
#include "AsciiFlowCommon/ToolRectangle.h"
#include "AsciiFlowCommon/ToolResize.h"
#include "AsciiFlowCommon/ToolText.h"

namespace
{
// convert a QKeyEvent to a Common::KeyEvent
Common::KeyEvent Convert(const QKeyEvent &evt)
{
	using namespace Common;
	KeyEvent ret;
	ret.state = KeyState::Down;

	auto uc = evt.key();
	auto us = evt.text().toStdString();

	switch (uc)
	{
		case Qt::Key::Key_Left:
			ret.key = SpecialKey::CursorLeft;
			break;
		case Qt::Key::Key_Right:
			ret.key = SpecialKey::CursorRight;
			break;
		case Qt::Key::Key_Up:
			ret.key = SpecialKey::CursorUp;
			break;
		case Qt::Key::Key_Down:
			ret.key = SpecialKey::CursorDown;
			break;
		case Qt::Key::Key_Backspace:
			ret.key = SpecialKey::BackSpace;
			break;
		case Qt::Key::Key_Delete:
			ret.key = SpecialKey::Del;
			break;
		case Qt::Key::Key_Return:
			ret.key = SpecialKey::Return;
			break;
		default:
		{
			if (not us.empty())
			{
				ret.key = us[0];
			}
			else
			{
				ret.key = SpecialKey::Undefined;
			}
			break;
		}
	}

	return ret;
}

// Convert a QPoint to a Common::Point
Common::Point Convert(const QPoint &evt)
{
	return Common::Point{evt.x(), evt.y()};
}

// Convert a QRect to a Common::Rect
Common::Rect Convert(const QRect &rect)
{
	return Common::Rect{rect.x(), rect.y(), rect.width(), rect.height()};
}
} // namespace

///////////////////////////////////////////////////////////////////////////////
// The Widget
///////////////////////////////////////////////////////////////////////////////
QAsciiArt::QAsciiArt(QWidget *parent) : QWidget(parent)
{
	// initialize stuff for drawing
	m_font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
	m_font.setPointSize(FONTSIZE);

	m_solid = QBrush(Qt::SolidLine);

	m_slide_start = QPoint(0, 0);

	m_data.Register([&](Common::Point point)
					{ OnDataAreaChanged(QPoint(point.x, point.y)); });

	ActivateToolHelper(Common::Tool::Line);

	setFocusPolicy(Qt::FocusPolicy::StrongFocus);
}

// Actions from toolbar
QString QAsciiArt::Export()
{
	// issue repaint
	update();
	return {m_data.ExportData().c_str()};
}
void QAsciiArt::Import(const QString &data)
{
	m_data.ImportData(data.toStdString());

	// issue repaint
	update();
}
void QAsciiArt::Clear()
{
	m_data.Clear();
	m_data_backup = m_data;
	m_undo.clear();
	m_redo.clear();

	// Tell others, no redo undo more
	emit RedoAvail(not m_redo.empty());
	emit UndoAvail(not m_undo.empty());

	// issue repaint
	update();
}

QPoint QAsciiArt::CurrentSlideStart() { return m_slide_start; }
void QAsciiArt::SetSlideStart(QPoint point) { m_slide_start = point; }

// Override from QWidget

void QAsciiArt::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QRect area = event->rect();
	qDebug() << "Repainting: " << area;

	painter.eraseRect(area);
	painter.setRenderHint(QPainter::Antialiasing, true);

	QPoint TopLeft = ScreenToText(QPoint(area.x(), area.y())) - QPoint(1, 1);
	QPoint DownRight = ScreenToText(QPoint(area.x() + area.width(),
										   area.y() + area.height())) +
					   QPoint(1, 1);
	if (DownRight.x() > m_data.Width())
	{
		DownRight.setX(m_data.Width());
	}
	if (DownRight.y() > m_data.Height())
	{
		DownRight.setY(m_data.Height());
	}
	qDebug() << "TopLeft: " << TopLeft;
	qDebug() << "DownRight: " << DownRight;

	// background
	{
		QBrush brush;
		brush.setColor(Qt::white);
		brush.setStyle(Qt::BrushStyle::SolidPattern);
		painter.setBrush(brush);

		QPoint screenpos1 = TextToScreen(TopLeft);
		QPoint screenpos2 = TextToScreen(DownRight);

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

		for (int x = TopLeft.x(); x < DownRight.x(); ++x)
		{
			QPoint screenpos1 = TextToScreen(QPoint(x, TopLeft.y()));
			QPoint screenpos2 = TextToScreen(QPoint(x, DownRight.y()));
			painter.drawLine(screenpos1, screenpos2);
		}

		for (int y = TopLeft.y(); y < DownRight.y(); ++y)
		{
			QPoint screenpos1 = TextToScreen(QPoint(TopLeft.x(), y));
			QPoint screenpos2 = TextToScreen(QPoint(DownRight.x(), y));
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

		std::vector<Common::Rect> selected = mp_current_tool->MarkedAreas();
		for (Common::Rect &rect : selected)
		{
			QPoint screenpos1 = TextToScreen(QPoint(rect.x, rect.y));
			QPoint screenpos2 =
				TextToScreen(QPoint(rect.x + rect.width, rect.y + rect.height));

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

		for (int y = TopLeft.y(); y < DownRight.y(); ++y)
		{
			for (int x = TopLeft.x(); x < DownRight.x(); ++x)
			{
				QPoint screenpos = TextToScreen(QPoint(x, y));

				// text point is at the lower line
				painter.drawText(screenpos.x() + 1,
								 screenpos.y() + CELLSIZE - 1,
								 QChar(m_data.At(Common::Point{x, y})));
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
	m_undo.push_back(QString(m_data_backup.ExportData().c_str()));

	mp_current_tool->OnMousePressed(Convert(HitText));
	update();

	emit RedoAvail(not m_redo.empty());
	emit UndoAvail(not m_undo.empty());
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
	mp_current_tool->OnMouseMove(Convert(area), Convert(HitText));

	update();
}
void QAsciiArt::mouseReleaseEvent(QMouseEvent *event)
{
	QPoint HitText = ScreenToText(QPoint(event->x(), event->y()));
	mp_current_tool->OnMouseReleased(Convert(HitText));
	update();
}

void QAsciiArt::keyPressEvent(QKeyEvent *event)
{
	mp_current_tool->OnKeyPressed(Convert(*event));
	update();
}

void QAsciiArt::resizeEvent(QResizeEvent *event)
{
	QPoint on_screen_size =
		TextToScreen(QPoint(m_data.Width(), m_data.Height()));
	this->setMinimumSize(on_screen_size.x(), on_screen_size.y());
	this->setMaximumSize(on_screen_size.x(), on_screen_size.y());
}

void QAsciiArt::ActivateToolMove() { ActivateToolHelper(Common::Tool::Move); }
void QAsciiArt::ActivateToolRectangle()
{
	ActivateToolHelper(Common::Tool::Rectangle);
}
void QAsciiArt::ActivateToolResize()
{
	ActivateToolHelper(Common::Tool::Resize);
}
void QAsciiArt::ActivateToolClass() { ActivateToolHelper(Common::Tool::Class); }
void QAsciiArt::ActivateToolArrow() { ActivateToolHelper(Common::Tool::Arrow); }
void QAsciiArt::ActivateToolLine() { ActivateToolHelper(Common::Tool::Line); }
void QAsciiArt::ActivateToolText() { ActivateToolHelper(Common::Tool::Text); }

void QAsciiArt::ActivateToolFreehand()
{
	ActivateToolHelper(Common::Tool::Freehand);
}
void QAsciiArt::ActivateToolErase() { ActivateToolHelper(Common::Tool::Erase); }

void QAsciiArt::Undo()
{
	if (not m_undo.empty())
	{
		QString tmp = *m_undo.rbegin();
		m_undo.pop_back();

		m_redo.push_back(QString(m_data.ExportData().c_str()));
		m_data.ImportData(tmp.toStdString());
		update();
	}

	emit RedoAvail(not m_redo.empty());
	emit UndoAvail(not m_undo.empty());
}
void QAsciiArt::Redo()
{
	if (m_redo.size() > 0)
	{
		QString tmp = *m_redo.rbegin();
		m_redo.pop_back();

		m_undo.push_back(QString(m_data.ExportData().c_str()));
		m_data.ImportData(tmp.toStdString());
		update();
	}

	emit RedoAvail(not m_redo.empty());
	emit UndoAvail(not m_undo.empty());
}

void QAsciiArt::OnDataAreaChanged(QPoint size)
{
	QPoint on_screen_size = TextToScreen(size);
	this->setMinimumSize(on_screen_size.x(), on_screen_size.y());
	this->setMaximumSize(on_screen_size.x(), on_screen_size.y());
	emit DataAreaChanged(on_screen_size);
}

void QAsciiArt::ActivateToolHelper(Common::Tool tool)
{
	using namespace Common;

	m_current_tool = tool;
	switch (m_current_tool)
	{
		case Tool::Move:
			mp_current_tool.reset(new ToolMove(m_data));
			break;
		case Tool::Rectangle:
			mp_current_tool.reset(new ToolRectangle(m_data));
			break;
		case Tool::Resize:
			mp_current_tool.reset(new ToolResize(m_data));
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
	return {x, y};
}
QPoint QAsciiArt::TextToScreen(QPoint point)
{
	return {point.x() * CELLSIZE, point.y() * CELLSIZE};
}
