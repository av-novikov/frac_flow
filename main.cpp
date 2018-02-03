#include <iostream>
#include <memory>

#include "src/Scene.hpp"

#include "src/fmodel/Fmodel.hpp"
#include "src/fmodel/FmodelSolver.hpp"

using namespace std;

fmodel::Properties* getProps()
{
	fmodel::Properties* props = new fmodel::Properties();

	props->cellsNum_x = 10;
	props->cellsNum_y = 10;
	props->w2 = 1.0;
	props->r_w = props->w2 / 10.0;
	props->l2 = 10.0;

	props->p_init = 100.0 * 1.E+5;

	return props;
}

int main()
{
	Scene<fmodel::Fmodel, fmodel::FmodelSolver, fmodel::Properties> scene;
	const auto props = getProps();
	scene.load(*props);

	return 0;
}