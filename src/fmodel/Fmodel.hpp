#ifndef FMODEL_HPP_
#define FMODEL_HPP_

#include "src/Model.hpp"

namespace fmodel
{
	struct Properties
	{
		int cellsNum_x, cellsNum_y;
		double r_w, w2, l2, height;

		double p_init;
	};

	typedef var::containers::TapeVarPressure TapeVariable;
	typedef var::containers::VarPressure Variable;
	class Fmodel : public Model<var::BasicVariables<Variable>, Properties, Fmodel>
	{
		template<typename> friend class snapshotter::VTKSnapshotter;
	protected:
		double p_init;

		void setProps(const Properties& props);
		void make_dimless();
		void setInitialState();

	public:
		Fmodel();
		~Fmodel();
		static const int var_size = Variable::size;
	};
};

#endif /* FMODEL_HPP_ */