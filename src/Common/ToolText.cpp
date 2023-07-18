#include "AsciiFlowCommon/ToolText.h"
#include "AsciiFlowCommon/IsPrint.h"

namespace Common
{
// https://en.cppreference.com/w/cpp/utility/variant/visit
// helper constant for the visitor #3
template <class>
inline constexpr bool always_false_v = false;

ToolText::ToolText(AsciiArtData &data) : m_data(data), m_cursor{0, 0} {}

void ToolText::OnMousePressed(Point point) { m_start = m_cursor = point; }
void ToolText::OnMouseMove(Rect /* area */, Point /*point*/) {}
void ToolText::OnMouseReleased(Point /* point */) {}

void ToolText::OnKeyPressed(KeyEvent event)
{
	if (event.state != KeyState::Down)
	{
		return;
	}

	std::visit(
		[&](auto &&arg)
		{
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, char>)
			{
				if (IsPrint(arg))
				{
					m_data.Set(m_cursor.x, m_cursor.y, arg);
					m_cursor.x += 1;
				}
			}
			else if constexpr (std::is_same_v<T, SpecialKey>)
			{
				// special key
				switch (arg)
				{
					case SpecialKey::Undefined:
						break;
					case SpecialKey::BackSpace:
					case SpecialKey::Del:
						m_cursor.x -= 1;
						m_data.Set(m_cursor.x, m_cursor.y, ' ');
						break;
					case SpecialKey::CursorLeft:
						m_cursor.x -= 1;
						break;
					case SpecialKey::CursorRight:
						m_cursor.x += 1;
						break;
					case SpecialKey::CursorUp:
						m_cursor.y -= 1;
						break;
					case SpecialKey::CursorDown:
						m_cursor.y += 1;
						break;
					case SpecialKey::Return:
						m_cursor.x = m_start.x;
						m_cursor.y += 1;
						break;
				}
			}
			else
				static_assert(always_false_v<T>, "non-exhaustive visitor!");
		},
		event.key);

	// sanity check
	if (m_cursor.x < 0)
	{
		m_cursor.x = 0;
	}
	if (m_cursor.y < 0)
	{
		m_cursor.y = 0;
	}

	if (m_cursor.x >= m_data.Width())
	{
		m_data.Set(m_cursor.x, m_cursor.y, ' ');
	}
	if (m_cursor.y > m_data.Height())
	{
		m_data.Set(m_cursor.x, m_cursor.y, ' ');
	}
}

std::vector<Rect> ToolText::MarkedAreas()
{
	std::vector<Rect> ret;
	ret.push_back(Rect{m_cursor.x, m_cursor.y, 1, 1});
	return ret;
}
} // namespace Common