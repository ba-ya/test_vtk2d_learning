#include "00headers.h"
#include <qDebug>
#include <vtkChartXY.h>
#include <vtkNamedColors.h>
#include <vtkTextProperty.h>
#include <vtkAxis.h>
#include <vtkTable.h>
#include <vtkFloatArray.h>
#include <vtkCharArray.h>
#include <vtkPlotArea.h>
#include <vtkBrush.h>

void AreaPlot::Draw(std::vector<vtkSmartPointer<vtkContextView> > views, vtkSmartPointer<vtkChartXY> chart)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    auto view = views[0];
    view->GetRenderer()->SetBackground(colors->GetColor3d("SlateGray").GetData());
    view->GetScene()->AddItem(chart);

    chart->SetTitle("Area Plot");
    chart->GetTitleProperties()->SetFontSize(36);
    chart->GetTitleProperties()->SetColor(colors->GetColor3d("Banana").GetData());
    for (int i = 0; i < 2; ++i) {
        chart->GetAxis(i)->GetTitleProperties()->SetFontSize(24);
        chart->GetAxis(i)->GetTitleProperties()->SetColor(colors->GetColor3d("Orange").GetData());
        chart->GetAxis(i)->GetLabelProperties()->SetColor(colors->GetColor3d("Beige").GetData());
        chart->GetAxis(i)->GetLabelProperties()->SetFontSize(18);
    }

    QStringList col_names = {
        "X Axis",
        "Cosine",
        "Sine",
        "Sine2",
        "Sine3",
        "One",
    };
    auto table = vtkSmartPointer<vtkTable>::New();
    for (auto &name : col_names) {
        auto col = vtkSmartPointer<vtkFloatArray>::New();
        col->SetName(name.toUtf8().constData());
        table->AddColumn(col);
    }

    auto valid_mask = vtkSmartPointer<vtkCharArray>::New();
    valid_mask->SetName("ValidMask");
    table->AddColumn(valid_mask);

    int cnt_points = 69;
    float inc = 8.0 / (cnt_points - 1);
    table->SetNumberOfRows(cnt_points);
    for (int i = 0; i < cnt_points; ++i) {
        table->SetValue(i, 0, i * inc);
        table->SetValue(i, 1, cos(i * inc) + .01);
        table->SetValue(i, 2, sin(i * inc) + .01);
        table->SetValue(i, 3, sin(i * inc) + .5);
        table->SetValue(i, 4, sin(i * inc) + sin(i * inc) + .01);
        table->SetValue(i, 5, 1.0);

        // 69个点, 中间31-39,9个点没有数据
        valid_mask->SetValue(i, (30 < i && i < 40) ? 0 : 1);
    }

    auto tomato = colors->GetColor3d("Tomato");
    vtkPlotArea *area = dynamic_cast<vtkPlotArea *>(chart->AddPlot(vtkChart::AREA));
    area->SetInputData(table);
    area->SetInputArray(0, "X Axis");
    area->SetInputArray(2, "Sine");
    area->SetInputArray(1, "Sine2");
    area->SetValidPointMaskName("ValidMask");
    area->GetBrush()->SetColorF(tomato.GetRed(), tomato.GetGreen(), tomato.GetBlue(), .6);

}
