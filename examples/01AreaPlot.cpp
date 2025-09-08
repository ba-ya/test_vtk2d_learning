#include "00headers.h"

#include <vtkChartXY.h>

void AreaPlot::Draw(std::vector<vtkSmartPointer<vtkContextView>> views)
{
    auto view = views[0];
    auto chart = vtkSmartPointer<vtkChartXY>::New();
    chart->SetTitle("Area Plot");

    view->GetScene()->AddItem(chart);
}
