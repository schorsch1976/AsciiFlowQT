#pragma once

#include "AsciiFlowCommon/AsciiFlowCommonAPI.h"

#include "AsciiFlowCommon/Point.h"
#include "ITool.h"

#include <functional>
#include <string>
#include <string_view>
#include <vector>

namespace Common
{
using DataAreaListenerCB = std::function<void(Point)>;

class AsciiFlowCommon_API AsciiArtData
{
public:
	// rule of 5
	AsciiArtData();
	virtual ~AsciiArtData(); // must be not default because DLL/SO boundary

	AsciiArtData(const AsciiArtData &rhs);
	AsciiArtData &operator=(const AsciiArtData &rhs);

	AsciiArtData(AsciiArtData &&rhs) noexcept;
	AsciiArtData &operator=(AsciiArtData &&rhs) noexcept;

	// register the listener
	void Register(const DataAreaListenerCB &listener);

	// access the data
	[[nodiscard]] std::string ExportData() const;
	void ImportData(std::string_view data);

	void Set(Point p, char c);
	void Set(int x, int y, char c);
	char At(Point p);

	void Clear();

	[[nodiscard]] int Width() const;
	[[nodiscard]] int Height() const;

private:
	void ResizeData(Point point);
	void CheckData();

	void SendDataAreaChanged(Point area);

	std::vector<DataAreaListenerCB> m_listeners;

	int m_width;
	int m_height;
	std::vector<std::string> m_data;
};
} // namespace Common
