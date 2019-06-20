#include "qasciiart.h"

#include <QPainter>
#include <QPen>
#include <QFontDatabase>
#include <QMouseEvent>
#include <QChar>
#include <QRegularExpression>

///////////////////////////////////////////////////////////////////////////////
// The AsciiArtData
///////////////////////////////////////////////////////////////////////////////
AsciiArtData::AsciiArtData()
{
    m_width = m_height = 0;
    ResizeData(QPoint(80,80));
}
AsciiArtData::~AsciiArtData()
{
}

AsciiArtData::AsciiArtData(const AsciiArtData& rhs)
{
    *this = rhs;
}
AsciiArtData& AsciiArtData::operator=(const AsciiArtData& rhs)
{
    m_width = rhs.m_width;
    m_height = rhs.m_height;
    m_data = rhs.m_data;

    ResizeData(QPoint(m_width,m_height));
    return *this;
}

AsciiArtData::AsciiArtData(AsciiArtData&& rhs)
{
    *this = rhs;
}
AsciiArtData& AsciiArtData::operator=(AsciiArtData&& rhs)
{
    m_width = rhs.m_width;
    m_height = rhs.m_height;
    m_data = rhs.m_data;

    ResizeData(QPoint(m_width,m_height));
    return *this;
}

// access the data
QString AsciiArtData::ExportData()
{

    QRegularExpression ex(" +$");
    QVector<QString> cleaned;
    for (int y = 0; y < m_data.size(); ++y)
    {
        // remove and space from the end of the line
        QString line = m_data[y];
        line.remove(ex);
        cleaned.push_back(line);
    }

    int last_used_line = 0;
    for (int y =0; y < cleaned.size(); ++y)
    {
        if (cleaned[y].size())
        {
            last_used_line = y;
        }
    }
    QString ret;
    for (int y =0; y <= last_used_line; ++y)
    {
       ret += cleaned[y] + '\n';
    }
    return ret;
}
void AsciiArtData::ImportData(QString data)
{
    m_data.clear();
    m_width = 0;
    m_height = 0;

    // split by \n
    QString line;
    int x = 0;
    for (int i = 0; i < data.size(); ++i)
    {
        QChar c = data[i];
        if (c == '\r')
        {
            continue;
        }
        else if (c == '\n')
        {
            m_data.push_back(line);
            line.clear();
            x = 0;
        }
        else if (c.isPrint() && c.toLatin1())
        {
            line += c;
            ++x;
        }
        else
        {
            // convert to " "
            line += QChar::Space;
            ++x;
        }
        m_width = std::max(x, m_width);

    }
    m_data.push_back(line);
    m_height = m_data.size();

    ResizeData(QPoint(m_width, m_height));
    CheckData();

}

void AsciiArtData::Clear()
{
    m_data.clear();
    m_width= 80;
    m_height = 80;
    ResizeData(QPoint(m_width, m_height));
}
void AsciiArtData::Set(QPoint p, QChar c)
{
    ResizeData(p);
    if (p.x() < 0 || p.y() < 0)
    {
        return;
    }
    m_data[p.y()][p.x()] = c;
}
void AsciiArtData::Set(int x, int y, QChar c)
{
    Set(QPoint(x,y),c);
}

QChar AsciiArtData::At(QPoint p)
{
    ResizeData(p);
    if (p.x() < 0 || p.y() < 0)
    {
        return QChar(0);
    }
    return m_data[p.y()][p.x()];
}
QChar AsciiArtData::At(int x, int y)
{
    return At(QPoint(x,y));
}

int AsciiArtData::Width()
{
    return m_width;
}
int AsciiArtData::Height()
{
    return m_height;
}

// private
void AsciiArtData::ResizeData(QPoint point)
{
    bool resize_needed = false;
    if (point.y() >= m_height)
    {
        m_height = point.y()+1;
        m_data.resize(m_height);
        resize_needed = true;
    }
    if (point.x() >= m_width)
    {
        m_width = point.x() + 1;
        resize_needed = true;
    }

    if (!resize_needed)
    {
        return;
    }

    for (int y = 0; y < m_height; ++y)
    {
        QString& line = m_data[y];
        line.resize(m_width, QChar::Space);
    }

    CheckData();
}
void AsciiArtData::CheckData()
{
    for (int y = 0; y < m_data.size(); ++y)
    {
        QString& line = m_data[y];
        for (int x = 0; x < line.size(); ++x)
        {
            if (line[x].toLatin1() == char(0) ||
                    !line[x].isPrint())
            {
                line[x] = QChar::Space;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// The Strategy
///////////////////////////////////////////////////////////////////////////////
QAsciiArt::ITool::~ITool() {}

namespace
{
///////////////////////////////////////////////////////////////////////////////
// Tool Move
///////////////////////////////////////////////////////////////////////////////
class ToolMove : public QAsciiArt::ITool
{
public:
    explicit ToolMove(AsciiArtData& data);
    virtual void OnMousePressed(QPoint point) override;
    virtual void OnMouseMove(QRect area, QPoint point) override;
    virtual void OnMouseReleased(QPoint point) override;

    virtual void OnKeyPressed(QString c) override;

    virtual QVector<QRect> MarkedAreas() override;
private:
   AsciiArtData& m_data;

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

ToolMove::ToolMove(AsciiArtData& data)
    : m_data(data), m_start(-1,-1), m_state(State::NotMarked)
{}
void ToolMove::OnMousePressed(QPoint point)
{
    switch (m_state)
    {
        case State::NotMarked:
            m_start = point;
            break;
        case State::Marked:
            if (m_area.contains(point))
            {
                m_state = State::Moving;
                m_start = point;
                // copy data
                m_moved_data.clear();
                for (int y = m_area.y(); y < m_area.y()+ m_area.height(); ++y)
                {
                    for (int x = m_area.x(); x < m_area.x() + m_area.width(); ++x)
                    {
                        QPoint p(x,y);
                        m_moved_data.push_back({p, m_data.At(p)});
                    }
                }
            }
            else {
                m_state = State::NotMarked;
            }
            break;
        case State::Moving:
            break;
    }
}
void ToolMove::OnMouseMove(QRect area, QPoint point)
{
    switch (m_state)
    {
        case State::NotMarked:
            m_area = area;
            break;
        case State::Marked:
            break;
        case State::Moving:
            // clear data
            for (Info& c : m_moved_data)
            {
                m_data.Set(c.p, QChar::Space);
            }

            QPoint delta = point - m_start;
            m_current_area = QRect(m_area.x() + delta.x(),
                                   m_area.y() + delta.y(),
                                   m_area.width(),
                                   m_area.height());
            for (Info& c : m_moved_data)
            {
                m_data.Set(c.p + delta, c.c);
            }
            break;
    }
}
void ToolMove::OnMouseReleased(QPoint /*point*/)
{
    switch (m_state)
    {
        case State::NotMarked:
            m_state = State::Marked;
            break;
        case State::Marked:
        case State::Moving:
            m_state = State::NotMarked;
            m_area = m_current_area = QRect();
            break;
    }
}

void ToolMove::OnKeyPressed(QString /* c */)
{

}

QVector<QRect> ToolMove::MarkedAreas()
{
    QVector<QRect> ret;
    switch (m_state)
    {
        case State::NotMarked:
            ret.push_back(m_area);
            break;
        case State::Marked:
            ret.push_back(m_area);
            break;
        case State::Moving:
            ret.push_back(m_area);
            ret.push_back(m_current_area);
            break;
    }

    return ret;
}

///////////////////////////////////////////////////////////////////////////////
// Tool Rectangle
///////////////////////////////////////////////////////////////////////////////
class ToolRectangle : public QAsciiArt::ITool
{
public:
    explicit ToolRectangle(AsciiArtData& data);
    virtual void OnMousePressed(QPoint point) override;
    virtual void OnMouseMove(QRect area, QPoint point) override;
    virtual void OnMouseReleased(QPoint point) override;

    virtual void OnKeyPressed(QString c) override;

    virtual QVector<QRect> MarkedAreas() override;
private:
   AsciiArtData& m_data;
};

ToolRectangle::ToolRectangle(AsciiArtData& data)
    : m_data(data) {}

void ToolRectangle::OnMousePressed(QPoint /* point */)
{
}
void ToolRectangle::OnMouseMove(QRect area, QPoint /* point */)
{
    for (int y = area.y(); y <= area.y() + area.height(); ++y)
    {
        if (y == area.y() || y == area.y() + area.height())
        {
            for (int x = area.x(); x < area.x() + area.width(); ++x)
            {
                m_data.Set(x,y,'-');
            }
            m_data.Set(area.x(),y,'+');
            m_data.Set(area.x() + area.width(),y,'+');
        }
        else
        {
            m_data.Set(area.x(),y,'|');
            m_data.Set(area.x() + area.width(),y,'|');
        }
    }
}
void ToolRectangle::OnMouseReleased(QPoint /* point */)
{

}

void ToolRectangle::OnKeyPressed(QString /* c */)
{

}

QVector<QRect> ToolRectangle::MarkedAreas()
{
    return QVector<QRect>();
}

///////////////////////////////////////////////////////////////////////////////
// Tool Class
///////////////////////////////////////////////////////////////////////////////
class ToolClass : public QAsciiArt::ITool
{
public:
    explicit ToolClass(AsciiArtData& data);
    virtual void OnMousePressed(QPoint point) override;
    virtual void OnMouseMove(QRect area, QPoint point) override;
    virtual void OnMouseReleased(QPoint point) override;

    virtual void OnKeyPressed(QString c) override;

    virtual QVector<QRect> MarkedAreas() override;
private:
   AsciiArtData& m_data;
};

ToolClass::ToolClass(AsciiArtData& data)
    : m_data(data){}

void ToolClass::OnMousePressed(QPoint /* point */)
{

}
void ToolClass::OnMouseMove(QRect area, QPoint /* point */)
{
    for (int y = area.y(); y <= area.y() + area.height(); ++y)
    {
        if (y == area.y() || y == area.y() + area.height())
        {
            for (int x = area.x(); x < area.x() + area.width(); ++x)
            {
                m_data.Set(x,y,'-');
            }
            m_data.Set(area.x(),y,'+');
            m_data.Set(area.x() + area.width(),y,'+');
        }
        else
        {
            m_data.Set(area.x(),y,'|');
            m_data.Set(area.x() + area.width(),y,'|');
        }
    }

    if (area.height() > 2)
    {
        for (int x = area.x(); x < area.x() + area.width(); ++x)
        {
            m_data.Set(x,area.y() + 2,'-');
        }
        m_data.Set(area.x(), area.y() + 2, '+');
        m_data.Set(area.x()+ area.width(), area.y() + 2, '+');
    }
}
void ToolClass::OnMouseReleased(QPoint /* point */)
{

}

void ToolClass::OnKeyPressed(QString /* c */)
{

}

QVector<QRect> ToolClass::MarkedAreas()
{
    return QVector<QRect> ();
}

///////////////////////////////////////////////////////////////////////////////
// Tool Arrow
///////////////////////////////////////////////////////////////////////////////
class ToolArrow : public QAsciiArt::ITool
{
public:
    explicit ToolArrow(AsciiArtData& data);
    virtual void OnMousePressed(QPoint point) override;
    virtual void OnMouseMove(QRect area, QPoint point) override;
    virtual void OnMouseReleased(QPoint point) override;

    virtual void OnKeyPressed(QString c) override;

    virtual QVector<QRect> MarkedAreas() override;
private:
   AsciiArtData& m_data;
   QPoint m_start;
};

ToolArrow::ToolArrow(AsciiArtData& data)
    : m_data(data){}

void ToolArrow::OnMousePressed(QPoint point)
{
    m_start = point;
}
void ToolArrow::OnMouseMove(QRect /* area */, QPoint point)
{
    int start_x = m_start.x();
    int start_y = m_start.y();

    int end_x = point.x();
    int end_y = point.y();

    if (start_x == end_x && start_y == end_y)
    {
        // on start
        m_data.Set(start_x, start_y, '+');
    }
    else if (start_x > end_x && start_y == end_y)
    {
        // left
        for (int x = start_x; x >= end_x; --x)
        {
            m_data.Set(x, start_y, '-');
        }
        m_data.Set(start_x, start_y, '+');
        m_data.Set(end_x, start_y, '<');
    }
    else if (start_x < end_x && start_y == end_y)
    {
        // right
        for (int x = start_x; x <= end_x; ++x)
        {
            m_data.Set(x, start_y, '-');
        }
        m_data.Set(start_x, start_y, '+');
        m_data.Set(end_x, start_y, '>');
    }
    else if (start_x == end_x && start_y < end_y)
    {
        // down
        for (int y = start_y; y <= end_y; ++y)
        {
            m_data.Set(start_x, y, '|');
        }
        m_data.Set(start_x, start_y, '+');
        m_data.Set(start_x, end_y, 'v');
    }
    else if (start_x == end_x && start_y > end_y)
    {
        // up
        for (int y = start_y; y >= end_y; --y)
        {
            m_data.Set(start_x, y, '|');
        }
        m_data.Set(start_x, start_y, '+');
        m_data.Set(start_x, end_y, '^');
    }
    else {
        // general case
        // first left/right, then up/down
        int dir_x = start_x < end_x ? 1 : -1;
        int dir_y = start_y < end_y ? 1 : -1;

        for (int x = start_x; x != end_x; x += dir_x)
        {
            m_data.Set(x,start_y,'-');
        }
        for (int y = start_y; y != end_y; y += dir_y)
        {
            m_data.Set(end_x,y,'|');
        }

        if (dir_y > 0)
        {
            m_data.Set(end_x,end_y,'v');
        }
        else if (dir_y < 0)
        {
            m_data.Set(end_x,end_y,'^');
        }

        m_data.Set(start_x,start_y,'+');
        m_data.Set(end_x,start_y,'+');
    }
}
void ToolArrow::OnMouseReleased(QPoint /* point */)
{

}

void ToolArrow::OnKeyPressed(QString /* c */)
{

}

QVector<QRect> ToolArrow::MarkedAreas()
{
    return QVector<QRect>();
}

///////////////////////////////////////////////////////////////////////////////
// Tool Text
///////////////////////////////////////////////////////////////////////////////
class ToolText : public QAsciiArt::ITool
{
public:
    explicit ToolText(AsciiArtData& data);
    virtual void OnMousePressed(QPoint point) override;
    virtual void OnMouseMove(QRect area, QPoint point) override;
    virtual void OnMouseReleased(QPoint point) override;

    virtual void OnKeyPressed(QString c) override;

    virtual QVector<QRect> MarkedAreas() override;
private:
   AsciiArtData& m_data;
   QPoint m_cursor;
};

ToolText::ToolText(AsciiArtData& data)
    : m_data(data){}

void ToolText::OnMousePressed(QPoint point)
{
    m_cursor = point;
}
void ToolText::OnMouseMove(QRect /* area */, QPoint point)
{
    m_cursor = point;
}
void ToolText::OnMouseReleased(QPoint /* point */)
{

}

void ToolText::OnKeyPressed(QString c)
{
    int delta_x = 0;
    for (int i = 0; i < c.size(); ++i)
    {
        if (c[i].isPrint() && c[i].toLatin1())
        {
            m_data.Set(m_cursor.x() + i, m_cursor.y(), c[i]);
            delta_x++;
        }
    }
    m_cursor.setX(m_cursor.x() + delta_x);
}

QVector<QRect> ToolText::MarkedAreas()
{
    QVector<QRect> ret;
    ret.push_back(QRect(m_cursor.x(), m_cursor.y(), 1, 1));
    return ret;
}

///////////////////////////////////////////////////////////////////////////////
// Tool Freehand
///////////////////////////////////////////////////////////////////////////////
class ToolFreehand : public QAsciiArt::ITool
{
public:
    explicit ToolFreehand(AsciiArtData& data);
    virtual void OnMousePressed(QPoint point) override;
    virtual void OnMouseMove(QRect area, QPoint point) override;
    virtual void OnMouseReleased(QPoint point) override;

    virtual void OnKeyPressed(QString c) override;

    virtual QVector<QRect> MarkedAreas() override;
private:
    void Draw();
   AsciiArtData& m_data;
   QVector<QPoint> m_spots;
};

ToolFreehand::ToolFreehand(AsciiArtData& data)
    : m_data(data){}

void ToolFreehand::OnMousePressed(QPoint point)
{
    m_spots.clear();
    m_spots.push_back(point);
    Draw();
}
void ToolFreehand::OnMouseMove(QRect /* area */, QPoint point)
{
    m_spots.push_back(point);
    Draw();
}
void ToolFreehand::OnMouseReleased(QPoint point)
{
    m_spots.push_back(point);
    Draw();
    m_spots.clear();
}

void ToolFreehand::OnKeyPressed(QString /* c */)
{

}

QVector<QRect> ToolFreehand::MarkedAreas()
{
    return QVector<QRect>();
}

void ToolFreehand::Draw()
{
    for (int i = 0; i < m_spots.size(); ++i)
    {
        m_data.Set(m_spots[i], 'X');
    }
}
///////////////////////////////////////////////////////////////////////////////
// Tool Erase
///////////////////////////////////////////////////////////////////////////////
class ToolErase : public QAsciiArt::ITool
{
public:
    explicit ToolErase(AsciiArtData& data);
    virtual void OnMousePressed(QPoint point) override;
    virtual void OnMouseMove(QRect area, QPoint point) override;
    virtual void OnMouseReleased(QPoint point) override;

    virtual void OnKeyPressed(QString c) override;

    virtual QVector<QRect> MarkedAreas() override;
private:
   AsciiArtData& m_data;
   QRect m_marked;
};

ToolErase::ToolErase(AsciiArtData& data)
    : m_data(data){}

void ToolErase::OnMousePressed(QPoint /* point */)
{

}
void ToolErase::OnMouseMove(QRect area, QPoint /* point */)
{
    m_marked = area;
}
void ToolErase::OnMouseReleased(QPoint /* point */)
{
    for (int y = m_marked.y(); y < m_marked.y() + m_marked.height() ; ++y)
    {
        for (int x = m_marked.x(); x < m_marked.x() + m_marked.width(); ++x)
        {
            m_data.Set(x,y, QChar::Space);
        }
    }
    m_marked = QRect();
}

void ToolErase::OnKeyPressed(QString /* c */)
{

}

QVector<QRect> ToolErase::MarkedAreas()
{
    QVector<QRect> ret;
    if (m_marked.width() > 0 && m_marked.height() > 0)
    {
        ret.push_back(m_marked);
    }
    return ret;
}

} // ns anon
///////////////////////////////////////////////////////////////////////////////
// The Widget
///////////////////////////////////////////////////////////////////////////////
QAsciiArt::QAsciiArt(QWidget *parent) : QWidget(parent)
{
    // initialize stuff for drawing
    m_font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    m_font.setPointSize(FONTSIZE);

    m_solid = QBrush(Qt::SolidLine);

    ActivateToolHelper(Tool::Move);

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

// Override from QWidget

void QAsciiArt::paintEvent(QPaintEvent* /* event */)
{
    QPainter painter(this);
    painter.eraseRect(0,0,width()-1,height()-1);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // background
    {
        QBrush brush;
        brush.setColor(Qt::white);
        brush.setStyle(Qt::BrushStyle::SolidPattern);
        painter.setBrush(brush);

        QPoint screenpos1 = TextToScreen(QPoint(0, 0));
        QPoint screenpos2 = TextToScreen(QPoint(m_data.Width(), m_data.Height()));

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
        for (QRect& rect : selected)
        {
            QPoint screenpos1 = TextToScreen(QPoint(rect.x(), rect.y()));
            QPoint screenpos2 = TextToScreen(QPoint(rect.x()+ rect.width()+1,
                                            rect.y()+ rect.height()+1));

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
                QPoint screenpos = TextToScreen(QPoint(x,y));

                // text point is at the lower line
                painter.drawText(screenpos.x()+1, screenpos.y() + CELLSIZE-1, m_data.At(QPoint(x,y)));
            }
        }
    }
}

void QAsciiArt::mousePressEvent(QMouseEvent *event)
{  
    QPoint HitText = ScreenToText(QPoint(event -> x(), event -> y()));
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
    QPoint HitText = ScreenToText(QPoint(event -> x(), event -> y()));

    int width = std::abs(HitText.x()-m_start.x());
    int height = std::abs(HitText.y()-m_start.y());
    int x = std::min(HitText.x(),m_start.x());
    int y = std::min(HitText.y(),m_start.y());

    QRect area(x, y, width, height);

    m_data = m_data_backup;
    mp_current_tool->OnMouseMove(area, HitText);

    update();
}
void QAsciiArt::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint HitText = ScreenToText(QPoint(event -> x(), event -> y()));
    mp_current_tool->OnMouseReleased(HitText);
    update();
}

void QAsciiArt::keyPressEvent(QKeyEvent *event)
{
    QString txt = event->text();
    mp_current_tool->OnKeyPressed(txt);
    update();
}
void QAsciiArt::ActivateToolMove()
{
    ActivateToolHelper(Tool::Move);
}
void QAsciiArt::ActivateToolRectangle()
{
    ActivateToolHelper(Tool::Rectangle);
}
void QAsciiArt::ActivateToolClass()
{
    ActivateToolHelper(Tool::Class);
}
void QAsciiArt::ActivateToolArrow()
{
    ActivateToolHelper(Tool::Arrow);
}
void QAsciiArt::ActivateToolText()
{
    ActivateToolHelper(Tool::Text);
}

void QAsciiArt::ActivateToolFreehand()
{
    ActivateToolHelper(Tool::Freehand);
}
void QAsciiArt::ActivateToolErase()
{
    ActivateToolHelper(Tool::Erase);
}

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
void QAsciiArt::ActivateToolHelper(Tool tool)
{
    m_current_tool = tool;
    switch(m_current_tool)
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
    return QPoint(x,y);
}
QPoint QAsciiArt::TextToScreen(QPoint point)
{
    return QPoint(point.x() * CELLSIZE,point.y() * CELLSIZE);
}
