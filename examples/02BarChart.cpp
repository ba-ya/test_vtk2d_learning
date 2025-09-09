#include "00headers.h"

#include <QString>
#include <QStringList>
#include <vtkAxis.h>
#include <vtkChartXY.h>
#include <vtkColorSeries.h>
#include <vtkIntArray.h>
#include <vtkNamedColors.h>
#include <vtkPlot.h>
#include <vtkTable.h>
#include <vtkTextProperty.h>

static int data_2008[] = {10822, 10941, 9979,  10370, 9460, 11228,
                          15093, 12231, 10160, 9816,  9384, 7892};
static int data_2009[] = {9058,  9474,  9979,  9408, 8900, 11569,
                          14688, 12231, 10294, 9585, 8957, 8590};
static int data_2010[] = {9058,  10941, 9979,  10270, 8900, 11228,
                          14688, 12231, 10160, 9585,  9384, 8590};

void BarChart::Draw(std::vector<vtkSmartPointer<vtkContextView>> views, vtkSmartPointer<vtkChartXY> chart)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    auto view = views[0];
    view->GetRenderer()->SetBackground(colors->GetColor3d("Seashell").GetData());
    view->GetScene()->AddItem(chart);

    auto color_series = vtkSmartPointer<vtkColorSeries>::New();
    color_series->SetColorScheme(vtkColorSeries::BREWER_DIVERGING_SPECTRAL_3);

    vtkColor3d axis_color = colors->GetColor3d("Black");
    vtkColor3d title_color = colors->GetColor3d("MidnightBlue");

    auto set_axis_properties = [colors, axis_color](vtkAxis *axis, QString title) {
        axis->SetTitle(title.toUtf8().constData());
        axis->GetTitleProperties()->SetColor(axis_color.GetData());;
        axis->GetTitleProperties()->SetFontSize(16);
        axis->GetTitleProperties()->ItalicOn();
        axis->GetLabelProperties()->SetColor(axis_color.GetData());
        axis->GetGridPen()->SetColor(colors->GetColor4ub("Black"));
    };
    auto xaxis = chart->GetAxis(vtkAxis::BOTTOM);
    set_axis_properties(xaxis, "Monthly");
    auto yaxis = chart->GetAxis(vtkAxis::LEFT);
    set_axis_properties(yaxis, "Circulation");

    chart->SetTitle("Circulation 2008, 2009, 2010");
    chart->GetTitleProperties()->SetFontSize(24);
    chart->GetTitleProperties()->SetColor(title_color.GetData());
    chart->GetTitleProperties()->BoldOn();

    auto table = vtkSmartPointer<vtkTable>::New();
    QStringList names = {
        "Month",
        "2008",
        "2009",
        "2010"
    };
    for (auto &name : names) {
        auto arr = vtkSmartPointer<vtkIntArray>::New();
        arr->SetName(name.toUtf8().constData());
        table->AddColumn(arr);
    }

    auto cnt_row = 12;
    table->SetNumberOfRows(cnt_row);
    for (int i = 0; i < cnt_row; ++i) {
        table->SetValue(i, 0, i + 1);
        table->SetValue(i, 1, data_2008[i]);
        table->SetValue(i, 2, data_2009[i]);
        table->SetValue(i, 3, data_2010[i]);
    }

    vtkPlot* line = 0;
    for (int i = 0; i < 3; ++i) {
        line = chart->AddPlot(vtkChart::BAR);
        line->SetColorF(color_series->GetColor(i).GetRed() / 255.0,
                        color_series->GetColor(i).GetGreen() / 255.0,
                        color_series->GetColor(i).GetBlue() / 255.0);
        line->SetInputData(table, 0, i + 1);// 1, 2, 3
    }

}
