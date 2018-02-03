#ifndef FMODELSOLVER_HPP_
#define FMODELSOLVER_HPP_

#include "src/fmodel/Fmodel.hpp"

namespace fmodel
{
	class FmodelSolver
	{
	protected:
		Fmodel* model;
	public:

		FmodelSolver(Fmodel* _model) : model(_model) {};
		~FmodelSolver() {};
	};
}

#endif FMODELSOLVER_HPP_