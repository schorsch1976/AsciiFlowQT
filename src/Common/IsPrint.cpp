#include "AsciiFlowCommon/IsPrint.h"

#include <cctype>

namespace Common
{
///////////////////////////////////////////////////////////////////////////////
// Wrapper for std::isprint
// https://en.cppreference.com/w/cpp/string/byte/isprint
///////////////////////////////////////////////////////////////////////////////
bool IsPrint(char character)
{
	return std::isprint(static_cast<unsigned char>(character));
}
} // namespace Common
