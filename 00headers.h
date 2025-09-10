#include "common.h"

class vtkContextView;
class vtkChartXY;
class vtkChartBox;
class vtkChartMatrix;
namespace AreaPlot {
void Draw(std::vector<vtkSmartPointer<vtkContextView>> views, vtkSmartPointer<vtkChartXY> chart);
}

namespace BarChart {
void Draw(std::vector<vtkSmartPointer<vtkContextView>> views, vtkSmartPointer<vtkChartXY> chart);
}

namespace BoxChart {
void Draw(std::vector<vtkSmartPointer<vtkContextView>> views, vtkSmartPointer<vtkChartBox> chart);
}

namespace ChartMatrix {
void Draw(std::vector<vtkSmartPointer<vtkContextView>> views, vtkSmartPointer<vtkChartMatrix> chart);
}

