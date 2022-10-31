#include <vtkChartXY.h>
#include <vtkContextScene.h>
#include <vtkContextView.h>
#include <vtkFloatArray.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPen.h>
#include <vtkPlot.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkTable.h>
#include <vtkPlotPoints.h>
#include <vtkTextProperty.h>
#include <vtkChartLegend.h>
#include <vtkDelimitedTextWriter.h>
#include<vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingContextOpenGL2)
//#include <locale>
//#include <string.h>
//#include <atlconv.h>



int main(int, char*[])
{
	vtkNew<vtkNamedColors> colors;

	// Create a table with some points in it.
	vtkNew<vtkTable> table;

	vtkNew<vtkFloatArray> arrX;
	arrX->SetName("X Axis");
	table->AddColumn(arrX);

	vtkNew<vtkFloatArray> arrC;
	arrC->SetName("pattren1");
	table->AddColumn(arrC);

	vtkNew<vtkFloatArray> arrS;
	arrS->SetName("pattren2");
	table->AddColumn(arrS);


	// Fill in the table with some example values.
	int numPoints = 69;
	float inc = 7.5 / (numPoints - 1);
	table->SetNumberOfRows(numPoints);
	for (int i = 0; i < numPoints; ++i)
	{
		table->SetValue(i, 0, i * inc);
		//table->SetValue(i, 1, cos(i * inc));
		table->SetValue(i, 1, (2 * (i * inc) + 5));//2x+0.5
		table->SetValue(i, 2, sin(i * inc));
	}

	std::string fileOut =  "C:\\Users\\Upeka\\source\\repos\\Project5\\my.txt";

	vtkSmartPointer<vtkDelimitedTextWriter> writer =vtkSmartPointer<vtkDelimitedTextWriter>::New();
	writer->SetFileName(fileOut.c_str());
	writer->SetInputData(table);
	writer->Write();


	// Set up the view
	vtkNew<vtkContextView> view;
	
	view->GetRenderWindow()->SetWindowName("LinePlot");
	view->GetRenderWindow()->SetSize( 900, 400);
	view->GetRenderer()->SetBackground(colors->GetColor3d("SlateGray").GetData());

   // Add multiple line plots, setting the colors etc.
	vtkNew<vtkChartXY> chart;
	view->GetScene()->AddItem(chart);
	vtkPlot* line = chart->AddPlot(vtkChart::LINE);
	line->SetInputData(table, 0, 1);
	line->SetColor(0, 255, 0, 255);
	line->SetWidth(1.0);

	line = chart->AddPlot(vtkChart::POINTS);
	line->SetInputData(table, 0, 2);
	line->SetColor(0, 0, 255, 255);
	line->SetWidth(1.0);
	dynamic_cast<vtkPlotPoints*>(line)->SetMarkerStyle(vtkPlotPoints::CROSS);

	chart->SetTitle("xy chart");
	chart->GetTitleProperties()->SetFontSize(27);
	chart->GetTitleProperties()->SetColor(colors->GetColor3d("banana").GetData());



	chart->SetShowLegend(true);
	chart->GetLegend()->SetInline(false);
	chart->GetLegend()->SetHorizontalAlignment(vtkChartLegend::RIGHT);
	chart->GetLegend()->SetVerticalAlignment(vtkChartLegend::TOP);
	// For dotted line, the line type can be from 2 to 5 for different dash/dot
	// patterns (see enum in vtkPen containing DASH_LINE, value 2):
	// #ifndef WIN32
	//   line->GetPen()->SetLineType(vtkPen::DASH_LINE);
	// #endif
	// (ifdef-ed out on Windows because DASH_LINE does not work on Windows
	//  machines with built-in Intel HD graphics card...)

	// view->GetRenderWindow()->SetMultiSamples(0);

	// Start interactor
    view->GetRenderWindow();
	view->GetInteractor()->Initialize();
	view->GetInteractor()->Start();

    return EXIT_SUCCESS;
}