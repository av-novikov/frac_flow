#ifndef VARIABLES_HPP_
#define VARIABLES_HPP_

#include <valarray>
#include "adolc/adouble.h"

namespace var
{
	namespace containers
	{
		struct VarPressure
		{
			static const int size = 1;
			double& p;

			VarPressure(double* data) : p(data[0]) {};
			VarPressure(const double* data) : p(const_cast<double&>(data[0])) {};
		};
		struct TapeVarPressure
		{
			static const int size = 1;
			adouble p;
		};
	};

	template <typename TVariable>
	struct BaseVarWrapper
	{
		TVariable u_prev, u_iter, u_next;
	};
	template <typename TVariable>
	struct BasicVariables
	{
		static const int size = TVariable::size;
		typedef TVariable Container;
		typedef BaseVarWrapper<TVariable> Wrap;
		std::valarray<double> u_prev, u_iter, u_next;

		Wrap operator[](const size_t idx)
		{
			return{ TVariable(&u_prev[idx * size]), TVariable(&u_iter[idx * size]), TVariable(&u_next[idx * size]) };
		};
		Wrap operator[](const size_t idx) const
		{
			return{ TVariable(&u_prev[idx * size]), TVariable(&u_iter[idx * size]), TVariable(&u_next[idx * size]) };
		};
	};
};

#endif /* VARIABLES_HPP_ */
