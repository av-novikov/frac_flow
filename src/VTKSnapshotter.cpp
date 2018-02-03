#include <string>
#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkDoubleArray.h>
#include <vtkIntArray.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkPolygon.h>

#include "src/VTKSnapshotter.hpp"
#include "src/fmodel/Fmodel.hpp"

#define BAR_TO_PA 1.E+5

using namespace std;
using namespace snapshotter;

template<class modelType>
VTKSnapshotter<modelType>::VTKSnapshotter(const Model* _model) : model(_model)
{
	R_dim = model->R_dim;
	P_dim = model->P_dim;
	pattern = prefix + "Mesh_%{STEP}.vtp";
}
template<class modelType>
VTKSnapshotter<modelType>::~VTKSnapshotter()
{
}
template<class modelType>
string VTKSnapshotter<modelType>::replace(string filename, string from, string to)
{
	size_t start_pos = 0;
	while ((start_pos = filename.find(from, start_pos)) != string::npos)
	{
		filename.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
	return filename;
}
template<class modelType>
std::string VTKSnapshotter<modelType>::getFileName(const int i)
{
	string filename = pattern;
	return replace(filename, "%{STEP}", to_string(i));
}
template<class modelType>
void VTKSnapshotter<modelType>::dump(const int snap_idx)
{}
void VTKSnapshotter<fmodel::Fmodel>::dump(const int snap_idx)
{
	using namespace fmodel;

	const int nx = model->cellsNum_x + 2;
	const int ny = model->cellsNum_y + 2;

	// Grid
	auto grid = vtkSmartPointer<vtkPolyData>::New();

	// Points
	auto points = vtkSmartPointer<vtkPoints>::New();
	for (int k = 1; k < nx; k++)
	{
		for (int j = 1; j < ny; j++)
		{
			const Cell& cell = model->cells[k * ny + j];
			points->InsertNextPoint(R_dim * (cell.x - cell.hx / 2.0), R_dim * (cell.y - cell.hy / 2.0), 0.0);
		}
	}
	grid->SetPoints(points);

	// Data
	auto polygons = vtkSmartPointer<vtkCellArray>::New();
	auto polygon = vtkSmartPointer<vtkPolygon>::New();
	polygon->GetPointIds()->SetNumberOfIds(4);
	auto pres = vtkSmartPointer<vtkDoubleArray>::New();
	pres->SetName("pressure");

	// Middle cells
	int k, j, idx, idx1;
	for (k = 0; k < nx - 2; k++)
	{
		for (j = 0; j < ny - 2; j++)
		{
			idx = k * (ny - 1) + j;
			idx1 = k * ny + j + 1;
			const Cell& cell = model->cells[idx1];
			const auto& var = (*model)[idx1].u_next;

			polygon->GetPointIds()->SetId(0, idx);
			polygon->GetPointIds()->SetId(1, idx + ny - 1);
			polygon->GetPointIds()->SetId(2, idx + ny);
			polygon->GetPointIds()->SetId(3, idx + 1);
			polygons->InsertNextCell(polygon);
			pres->InsertNextValue(var.p * P_dim / BAR_TO_PA);
		}
	}

	grid->SetPolys(polygons);
	vtkCellData* fd = grid->GetCellData();
	fd->AddArray(pres);

	// Writing
	auto writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
	writer->SetFileName(getFileName(snap_idx).c_str());
	writer->SetInputData(grid);
	writer->Write();
}

template class VTKSnapshotter<fmodel::Fmodel>;