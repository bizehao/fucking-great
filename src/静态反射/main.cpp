/*************************************************************************
	> File Name: StaticRefl.cpp
	> Author: Netcan
	> Descripton: StaticRefl usecase
	> Blog: https://netcan.github.io/
	> Mail: 1469709759@qq.com
	> Created Time: 2020-07-31 20:17
************************************************************************/
#include <cstdio>
#include <iostream>
#include <sstream>
#include "reflection.hpp"

DEFINE_STRUCT(Point,
	(double)x,
	(double)y
)

struct AABB {
	int a;
	double b;
	char c;
};
REFLECTION(AABB, a, b, c)

int main(int argc, char** argv) {

	AABB aabb{ 10,3.1415926,'M' };

	Point pp{ 88,66 };

	iguana::for_each(pp, [&pp](auto vv, auto bb) {
		std::cout << pp.*vv << std::endl;
		});
	return 0;
}

