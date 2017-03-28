#ifndef ROAD_COUNT_H
#define ROAD_COUNT_H


class RoadCount
{
public:
	static void reset();
	static void inc();
	static int getLast();
private:
	static int s_roadCount;
};

#endif // !ROAD_COUNT_H