#include "AsciiFlowCommon/Rect.h"

namespace Common
{

bool Rect::Contains(Point c) const
{
	return c.x >= x && c.x < x + width && c.y >= y && c.y < y + height;
}

} // namespace Common