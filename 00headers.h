#include "common.h"

class vtkContextView;
class vtkChartXY;
namespace AreaPlot {
void Draw(std::vector<vtkSmartPointer<vtkContextView>> views, vtkSmartPointer<vtkChartXY> chart);
}

namespace BarChart {
void Draw(std::vector<vtkSmartPointer<vtkContextView>> views, vtkSmartPointer<vtkChartXY> chart);
}
