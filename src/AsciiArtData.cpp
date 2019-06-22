#include "AsciiArtData.h"

#include <QRegularExpression>

static const int MIN_WIDTH = 80;
static const int MIN_HEIGHT = 50;

///////////////////////////////////////////////////////////////////////////////
// The AsciiArtData
///////////////////////////////////////////////////////////////////////////////
AsciiArtData::AsciiArtData(QWidget *parent) : QObject(parent)
{
	m_width = m_height = 0;
	ResizeData(QPoint(-1, -1));
}
AsciiArtData::~AsciiArtData() {}

AsciiArtData::AsciiArtData(const AsciiArtData &rhs) : QObject(rhs.parent())
{
	*this = rhs;
}
AsciiArtData &AsciiArtData::operator=(const AsciiArtData &rhs)
{
	m_width = rhs.m_width;
	m_height = rhs.m_height;
	m_data = rhs.m_data;

	ResizeData(QPoint(m_width - 1, m_height - 1));
	return *this;
}

AsciiArtData::AsciiArtData(AsciiArtData &&rhs) { *this = rhs; }
AsciiArtData &AsciiArtData::operator=(AsciiArtData &&rhs)
{
	m_width = rhs.m_width;
	m_height = rhs.m_height;
	m_data = rhs.m_data;

	ResizeData(QPoint(m_width - 1, m_height - 1));
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

	int last_used_line = -1;
	for (int y = 0; y < cleaned.size(); ++y)
	{
		if (cleaned[y].size())
		{
			last_used_line = y;
		}
	}
	QString ret;
	for (int y = 0; y <= last_used_line; ++y)
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

	m_width = m_height = -1;

	ResizeData(QPoint(-1, -1));
	CheckData();
}

void AsciiArtData::Clear()
{
	m_data.clear();
	m_width = 0;
	m_height = 0;
	ResizeData(QPoint(10 - 1, 10 - 1));
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
void AsciiArtData::Set(int x, int y, QChar c) { Set(QPoint(x, y), c); }

QChar AsciiArtData::At(QPoint p)
{
	ResizeData(p);
	if (p.x() < 0 || p.y() < 0)
	{
		return QChar(0);
	}
	return m_data[p.y()][p.x()];
}
QChar AsciiArtData::At(int x, int y) { return At(QPoint(x, y)); }

int AsciiArtData::Width() { return m_width; }
int AsciiArtData::Height() { return m_height; }

// private
void AsciiArtData::ResizeData(QPoint point)
{
	if (m_width > 0 && m_height > 0 && point.x() >= 0 && point.y() > 0 &&
		point.x() < m_width && point.y() < m_height)
	{
		return;
	}

	// determine minimum needed size
	int old_width = m_width;
	int old_height = m_height;

	int min_needed_width = std::max(MIN_WIDTH, point.x() + 1);
	min_needed_width = std::max(min_needed_width, m_width);

	int min_needed_height = std::max(MIN_HEIGHT, point.y() + 1);
	min_needed_height = std::max(min_needed_height, old_height);
	min_needed_height = std::max(min_needed_height, m_data.size());

	int current_width = 0;
	for (int y = 0; y < m_data.size(); ++y)
	{
		current_width = std::max(current_width, m_data[y].size());
	}
	min_needed_width = std::max(min_needed_width, current_width);

	// now resize everything to the needed size
	m_data.resize(min_needed_height);
	for (int y = 0; y < m_data.size(); ++y)
	{
		QString &line = m_data[y];
		line.resize(min_needed_width, QChar::Space);
	}

	m_width = min_needed_width;
	m_height = min_needed_height;

	CheckData();

	if (m_width != old_width || m_height != old_height)
	{
		emit DataAreaChanged(QPoint(m_width, m_height));
	}
}
void AsciiArtData::CheckData()
{
	for (int y = 0; y < m_data.size(); ++y)
	{
		QString &line = m_data[y];
		for (int x = 0; x < line.size(); ++x)
		{
			if (line[x].toLatin1() == char(0) || !line[x].isPrint())
			{
				line[x] = QChar::Space;
			}
		}
	}
}
