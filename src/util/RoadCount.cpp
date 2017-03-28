#include "util\RoadCount.h"

int RoadCount::s_roadCount = 0;

void RoadCount::reset()
{
	s_roadCount = 0;
}

void RoadCount::inc()
{
	s_roadCount++;
}

int RoadCount::getLast()
{
	return s_roadCount - 1;
}
