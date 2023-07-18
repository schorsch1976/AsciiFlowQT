#pragma once

#include "AsciiFlowCommon/AsciiFlowCommonAPI.h"

#include <variant>

namespace Common
{

enum class KeyState
{
	Down,
	Up
};

enum class SpecialKey
{
	Undefined,
	BackSpace,
	Del,
	CursorLeft,
	CursorRight,
	CursorUp,
	CursorDown,
	Return
};

struct AsciiFlowCommon_API KeyEvent
{
	KeyState state;
	std::variant<char, SpecialKey> key;
};

} // namespace Common
