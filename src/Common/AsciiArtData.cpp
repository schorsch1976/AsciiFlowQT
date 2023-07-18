#include "AsciiFlowCommon/AsciiArtData.h"
#include "AsciiFlowCommon/IsPrint.h"

#include <regex>

namespace Common
{
static const int MIN_WIDTH = 80;
static const int MIN_HEIGHT = 50;

///////////////////////////////////////////////////////////////////////////////
// The AsciiArtData
///////////////////////////////////////////////////////////////////////////////
AsciiArtData::AsciiArtData()
{
	m_width = m_height = 0;
	ResizeData(Point{-1, -1});
}
AsciiArtData::~AsciiArtData() = default;

AsciiArtData::AsciiArtData(const AsciiArtData &rhs) { *this = rhs; }
AsciiArtData &AsciiArtData::operator=(const AsciiArtData &rhs)
{
	m_width = rhs.m_width;
	m_height = rhs.m_height;
	m_data = rhs.m_data;

	ResizeData(Point{m_width - 1, m_height - 1});
	return *this;
}

AsciiArtData::AsciiArtData(AsciiArtData &&rhs) noexcept { *this = rhs; }
AsciiArtData &AsciiArtData::operator=(AsciiArtData &&rhs) noexcept
{
	m_width = rhs.m_width;
	m_height = rhs.m_height;
	m_data = rhs.m_data;

	ResizeData(Point{m_width - 1, m_height - 1});
	return *this;
}

// register the listener
void AsciiArtData::Register(const DataAreaListenerCB &listener)
{
	m_listeners.push_back(listener);
}
// access the data
std::string AsciiArtData::ExportData() const
{
	std::regex ex(" +$");
	std::vector<std::string> cleaned;
	for (auto line : m_data)
	{
		// remove and space from the end of the line
		std::string replaced;
		std::regex_replace(std::back_inserter<>(replaced), line.begin(),
						   line.end(), ex, "");

		cleaned.push_back(replaced);
	}

	int last_used_line = -1;
	for (int y = 0; y < static_cast<int>(cleaned.size()); ++y)
	{
		if (cleaned[y].size())
		{
			last_used_line = y + 1;
		}
	}
	std::string ret;
	for (int y = 0; y < last_used_line; ++y)
	{
		ret += cleaned[y] + '\n';
	}
	return ret;
}
void AsciiArtData::ImportData(std::string_view data)
{
	m_data.clear();
	m_width = 0;
	m_height = 0;

	// split by \n
	std::string line;
	int x = 0;
	for (char c : data)
	{
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
		else if (IsPrint(c))
		{
			line += c;
			++x;
		}
		else
		{
			// convert to " "
			line += " ";
			++x;
		}
		m_width = std::max(x, m_width);
	}
	m_data.push_back(line);

	m_width = m_height = -1;

	ResizeData(Point{-1, -1});
	CheckData();
}

void AsciiArtData::Clear()
{
	m_data.clear();
	m_width = 0;
	m_height = 0;
	ResizeData(Point{10 - 1, 10 - 1});
}
void AsciiArtData::Set(Point p, char c)
{
	ResizeData(p);
	if (p.x < 0 || p.y < 0)
	{
		return;
	}
	m_data[p.y][p.x] = c;
}
void AsciiArtData::Set(int x, int y, char c) { Set(Point{x, y}, c); }

char AsciiArtData::At(Point p)
{
	if (p.x < 0 || p.y < 0)
	{
		return char(0);
	}
	if (p.x >= m_width || p.y >= m_height)
	{
		ResizeData(p);
	}
	return m_data[p.y][p.x];
}

int AsciiArtData::Width() const { return m_width; }
int AsciiArtData::Height() const { return m_height; }

// private
void AsciiArtData::ResizeData(Point point)
{
	if (m_width > 0 && m_height > 0 && point.x >= 0 && point.y > 0 &&
		point.x < m_width && point.y < m_height)
	{
		return;
	}

	// determine minimum needed size
	int old_width = m_width;
	int old_height = m_height;

	int min_needed_width = std::max(MIN_WIDTH, point.x + 1);
	min_needed_width = std::max(min_needed_width, static_cast<int>(m_width));

	int min_needed_height = std::max(MIN_HEIGHT, point.y + 1);
	min_needed_height = std::max(min_needed_height, old_height);
	min_needed_height =
		std::max(min_needed_height, static_cast<int>(m_data.size()));

	int current_width = 0;
	for (auto &y : m_data)
	{
		current_width = std::max(current_width, static_cast<int>(y.size()));
	}
	min_needed_width = std::max(min_needed_width, current_width);

	// now resize everything to the needed size
	m_data.resize(min_needed_height);
	for (auto &line : m_data)
	{
		line.resize(min_needed_width, ' ');
	}

	m_width = min_needed_width;
	m_height = min_needed_height;

	CheckData();

	if (m_width != old_width || m_height != old_height)
	{
		SendDataAreaChanged(Point{m_width, m_height});
	}
}
void AsciiArtData::CheckData()
{
	for (auto &line : m_data)
	{
		for (char &x : line)
		{
			if (!IsPrint(x))
			{
				x = ' ';
			}
		}
	}
}

void AsciiArtData::SendDataAreaChanged(Point area)
{
	for (auto &c : m_listeners)
	{
		c(area);
	}
}
} // namespace Common
