#include "AsciiFlowCommon/Point.h"

namespace Common
{
Point Point::operator+(const Point &rhs) const
{
	Point ret{*this};

	ret.x += rhs.x;
	ret.y += rhs.y;

	return ret;
}
Point Point::operator-(const Point &rhs) const
{
	Point ret{*this};

	ret.x -= rhs.x;
	ret.y -= rhs.y;

	return ret;
}

} // namespace Common
