#include "src/fmodel/Fmodel.hpp"

using namespace fmodel;

Fmodel::Fmodel() {};
Fmodel::~Fmodel() {};
void Fmodel::setProps(const Properties& props)
{
	cellsNum_x = props.cellsNum_x;
	cellsNum_y = props.cellsNum_y;
	cellsNum = (cellsNum_x + 2) * (cellsNum_y + 2);

	r_w = props.r_w;
	height = props.height;
	l2 = props.l2;	w2 = props.w2;

	p_init = props.p_init;

	make_dimless();
};
void Fmodel::make_dimless()
{
	R_dim = 10.0 * r_w;
	P_dim = p_init;

	r_w /= R_dim;
	height /= R_dim;
	l2 /= R_dim;
	w2 /= R_dim;

	p_init /= P_dim;
};
void Fmodel::setInitialState()
{
	for (int i = 0; i < cellsNum; i++)
	{
		auto& var = (*this)[i];
		var.u_prev.p = var.u_iter.p = var.u_next.p = p_init;
	}
};