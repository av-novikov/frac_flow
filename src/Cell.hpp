#ifndef CELL_HPP_
#define CELL_HPP_

namespace grid
{
	enum IdStatus {INIT = -1, TMP = -2};
	enum Type { INNER, LEFT, FRAC, RIGHT, MEDIAN, NO_TYPE };
	class Cell
	{
	public:
		const int id;
		const int row;
		const Type type;

		double x, y;
		double hx, hy, hz;
		double V;

		Cell() : id(INIT), type(NO_TYPE), row(INIT) {};
		Cell(const int _id, const Type _type, double _x, double _y, double _hx, double _hy, double _hz) : 
			id(INIT), type(_type), row(INIT), x(_x), y(_y), hx(_hx), hy(_hy), hz(_hz) {};
		~Cell() {};
	};
};

#endif /* CELL_HPP_ */