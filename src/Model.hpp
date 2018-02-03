#ifndef MODEL_HPP_
#define MODEL_HPP_

#include <memory>
#include <string>
#include "src/Variables.hpp"
#include "src/Cell.hpp"
#include "src/VTKSnapshotter.hpp"

template<typename TVariables, typename TProps, typename TModel>
class Model : public TVariables
{
	template<typename> friend class snapshotter::VTKSnapshotter;
public:
	typedef typename snapshotter::VTKSnapshotter<TModel> SnapShotter;
	typedef TProps Properties;
	typedef grid::Cell Cell;
	typedef typename TVariables::Container VarContainer;

protected:
	double R_dim, P_dim;
	double Volume;
	double r_w, l2, w2, height;
	size_t cellsNum, cellsNum_x, cellsNum_y;
	size_t varNum;

	std::vector<Cell> cells;
	std::shared_ptr<SnapShotter> snapshotter;

	virtual void loadMesh()
	{
		buildGrid();
		varNum = VarContainer::size * cellsNum;
		u_prev.resize(varNum);
		u_iter.resize(varNum);
		u_next.resize(varNum);
	}
	void buildGrid() 
	{
		cells.reserve(cellsNum);

		Volume = 0.0;
		int counter = 0;

		double x_prev = r_w;
		double logMax_x = log(l2 / r_w);
		double logStep_x = logMax_x / (double)cellsNum_x;
		double hx = x_prev * (exp(logStep_x) - 1.0);
		double cm_x = r_w;

		double y_prev = r_w;
		double logMax_y = log(w2 / r_w);
		double logStep_y = logMax_y / (double)cellsNum_y;
		double hy = y_prev * (exp(logStep_y) - 1.0);
		double cm_y = r_w;

		// Left
		cells.push_back(Cell(counter++, grid::LEFT, cm_x - r_w, cm_y - r_w, 0.0, 0.0, height));
		for (int i = 0; i < cellsNum_y; i++)
		{
			cm_y = y_prev * (exp(logStep_y) + 1.0) / 2.0;
			hy = y_prev * (exp(logStep_y) - 1.0);
			cells.push_back(Cell(counter++, grid::LEFT, cm_x - r_w, cm_y - r_w, 0.0, hy, height));
			y_prev = y_prev * exp(logStep_y);
		}
		cells.push_back(Cell(counter++, grid::LEFT, cm_x - r_w, cm_y - r_w + hy / 2.0, 0.0, 0.0, height));
		// Middle
		for (int j = 0; j < cellsNum_x; j++)
		{
			cm_x = x_prev * (exp(logStep_x) + 1.0) / 2.0;
			hx = x_prev * (exp(logStep_x) - 1.0);
			cm_y = y_prev = r_w;
			hy = y_prev * (exp(logStep_y) - 1.0);
			cells.push_back(Cell(counter++, grid::MEDIAN, cm_x - r_w, cm_y - r_w, hx, 0.0, height));
			for (int i = 0; i < cellsNum_y; i++)
			{
				cm_y = y_prev * (exp(logStep_y) + 1.0) / 2.0;
				hy = y_prev * (exp(logStep_y) - 1.0);
				cells.push_back(Cell(counter++, grid::INNER, cm_x - r_w, cm_y - r_w, hx, hy, height));
				Volume += cells[cells.size() - 1].V;
				y_prev = y_prev * exp(logStep_y);
			}
			cells.push_back(Cell(counter++, grid::FRAC, cm_x - r_w, cm_y - r_w + hy / 2.0, hx, 0.0, height));
			x_prev = x_prev * exp(logStep_x);
		}
		// Right
		cm_x = l2;
		cm_y = y_prev = r_w;
		hy = y_prev * (exp(logStep_y) - 1.0);
		cells.push_back(Cell(counter++, grid::RIGHT, cm_x - r_w, cm_y - r_w, 0.0, 0.0, height));
		for (int i = 0; i < cellsNum_y; i++)
		{
			cm_y = y_prev * (exp(logStep_y) + 1.0) / 2.0;
			hy = y_prev * (exp(logStep_y) - 1.0);
			cells.push_back(Cell(counter++, grid::RIGHT, cm_x - r_w, cm_y - r_w, 0.0, hy, height));
			y_prev = y_prev * exp(logStep_y);
		}
		cells.push_back(Cell(counter++, grid::RIGHT, cm_x - r_w, cm_y - r_w + hy / 2.0, 0.0, 0.0, height));
	};
	virtual void setProps(const Properties& props) = 0;
	virtual void setInitialState() = 0;
public:
	void load(const Properties& props)
	{
		setProps(props);
		loadMesh();
		setInitialState();
	};
	void setSnapshotter(const TModel* model) { snapshotter = std::make_shared<SnapShotter>(model); };
	void snapshot(const int i) { snapshotter->dump(i); };

	Model() {};
	virtual ~Model() {};
};

#endif MODEL_HPP_