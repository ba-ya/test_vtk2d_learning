#include "common.h"

class vtkContextView;
class vtkChartXY;
class vtkChartBox;
namespace AreaPlot {
void Draw(std::vector<vtkSmartPointer<vtkContextView>> views, vtkSmartPointer<vtkChartXY> chart);
}

namespace BarChart {
void Draw(std::vector<vtkSmartPointer<vtkContextView>> views, vtkSmartPointer<vtkChartXY> chart);
}

namespace BoxChart {
void Draw(std::vector<vtkSmartPointer<vtkContextView>> views, vtkSmartPointer<vtkChartBox> chart);
}

