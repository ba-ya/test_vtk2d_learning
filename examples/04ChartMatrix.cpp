#include "00headers.h"
#include <QStringList>
#include <qdebug.h>
#include <vtkAxis.h>
#include <vtkChart.h>
#include <vtkChartMatrix.h>
#include <vtkFloatArray.h>
#include <vtkNamedColors.h>
#include <vtkPlot.h>
#include <vtkPlotPoints.h>
#include <vtkTable.h>

void ChartMatrix::Draw(std::vector<vtkSmartPointer<vtkContextView>> views, vtkSmartPointer<vtkChartMatrix> chart)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    auto view = views[0];
    view->GetRenderer()->SetBackground(colors->GetColor3d("navajo_white").GetData());
    // clear old plots
    int pos1[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    for (int i = 0; i < chart->GetNumberOfCharts(); ++i) {
        vtkChart *c = chart->GetChart(vtkVector2i(pos1[i][0], pos1[i][1]));
        c->ClearPlots();
    }
    view->GetScene()->AddItem(chart);

    chart->SetSize(vtkVector2i(2, 2));
    chart->SetGutter(vtkVector2f(30.0, 30.0));

    auto table = vtkSmartPointer<vtkTable>::New();
    QStringList names = {
        "X Axis",
        "Cosine",
        "Sine",
        "Sine2",
        "Tangent",
    };
    for (auto &name : names) {
        auto array = vtkSmartPointer<vtkFloatArray>::New();
        array->SetName(name.toUtf8().constData());
        table->AddColumn(array);
    }

    int cnt_points = 42;
    float inc = 7.5 / (cnt_points - 1);
    table->SetNumberOfRows(cnt_points);
    for (int i = 0; i < cnt_points; ++i) {
        table->SetValue(i, 0, i * inc);
        table->SetValue(i, 1, cos(i * inc));
        table->SetValue(i, 2, sin(i * inc));
        table->SetValue(i, 3, sin(i * inc) + .5);
        table->SetValue(i, 4, tan(i * inc));
    }

    int pos[5][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 0}, {1, 1}};
    int type[5] = {vtkChart::POINTS, vtkChart::LINE, vtkChart::LINE, vtkChart::LINE, vtkChart::BAR};
    int data_id[5] = {1, 2, 1, 2, 4};
    for (int i = 0; i < 5; ++i) {
        vtkChart *c = chart->GetChart(vtkVector2i(pos[i][0], pos[i][1]));
        vtkPlot *plot = c->AddPlot(type[i]);
        if (type[i] == vtkChart::POINTS) {
            // now, only 1 and 3 can used
            dynamic_cast<vtkPlotPoints*>(plot)->SetMarkerStyle(1);
            dynamic_cast<vtkPlotPoints*>(plot)->SetMarkerSize(8);
        }
        plot->SetInputData(table, 0, data_id[i]);
        plot->SetColor(colors->GetColor3ub("rose_madder").GetRed(),
                       colors->GetColor3ub("rose_madder").GetGreen(),
                       colors->GetColor3ub("rose_madder").GetBlue(), 255);
        plot->GetXAxis()->GetGridPen()->SetColorF(colors->GetColor3d("warm_grey").GetData());
        plot->GetYAxis()->GetGridPen()->SetColorF(colors->GetColor3d("warm_grey").GetData());
    }

}
