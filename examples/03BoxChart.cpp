#include "00headers.h"
#include <QString>
#include <vtkAxis.h>
#include <vtkChartBox.h>
#include <vtkComputeQuartiles.h>
#include <vtkContextScene.h>
#include <vtkIntArray.h>
#include <vtkLookupTable.h>
#include <vtkNamedColors.h>
#include <vtkPlot.h>
#include <vtkStatisticsAlgorithm.h>
#include <vtkTable.h>
#include <vtkTextProperty.h>

void BoxChart::Draw(std::vector<vtkSmartPointer<vtkContextView>> views, vtkSmartPointer<vtkChartBox> chart)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    auto view = views[0];
    view->GetRenderer()->SetBackground(colors->GetColor3d("Seashell").GetData());
    chart->ClearPlots();
    view->GetScene()->AddItem(chart);

    int cnt_param = 5;
    auto table = vtkSmartPointer<vtkTable>::New();
    for (int i = 0; i < cnt_param; ++i) {
        auto arr_index = vtkSmartPointer<vtkIntArray>::New();
        arr_index->SetName(QString("Run %1").arg(i + 1).toUtf8().constData());
        table->AddColumn(arr_index);
    }

    auto cnt_row = 20;
    table->SetNumberOfRows(cnt_row);
    double values[20][5] = {
                            {850, 960, 880, 890, 890},
                            {740, 940, 880, 810, 840},
                            {900, 960, 880, 810, 780},
                            {1070, 940, 860, 820, 810},
                            {930, 880, 720, 800, 760},
                            {850, 800, 720, 770, 810},
                            {950, 850, 620, 760, 790},
                            {980, 880, 860, 740, 810},
                            {980, 900, 970, 750, 820},
                            {880, 840, 950, 760, 850},
                            {1000, 830, 880, 910, 870},
                            {980, 790, 910, 920, 870},
                            {930, 810, 850, 890, 810},
                            {650, 880, 870, 860, 740},
                            {760, 880, 840, 880, 810},
                            {810, 830, 840, 720, 940},
                            {1000, 800, 850, 840, 950},
                            {1000, 790, 840, 850, 800},
                            {960, 760, 840, 850, 810},
                            {960, 800, 840, 780, 870}
    };
    for (int j = 0; j < cnt_row; ++j) {
        for (int i = 0; i < cnt_param; ++i) {
            table->SetValue(j, i, values[j][i]);
        }
    }

    // 统计计算
    // vtkComputeQuartiles 会基于原始 table 计算每列的分位数（最小值、25%、50%、75%、最大值）。
    // 输出还是一个 vtkTable（就是 out_table），但内容不再是原始数值，而是统计后的结果。
    // 根据 列col 进行统计, 5列,每列20个数据
    auto quartiles = vtkSmartPointer<vtkComputeQuartiles>::New();
    quartiles->SetInputData(vtkStatisticsAlgorithm::INPUT_DATA, table);
    quartiles->Update();

    vtkTable *out_table = quartiles->GetOutput();
    auto lookup = vtkSmartPointer<vtkLookupTable>::New();
    lookup->SetNumberOfColors(cnt_row);
    lookup->SetRange(0, 4);
    lookup->Build();

    chart->GetPlot(0)->SetInputData(out_table);
    chart->SetShowLegend(true);
    chart->SetColumnVisibilityAll(true);
    chart->SetTitle("Michelson-Morley experiment");
    chart->GetTitleProperties()->SetFontSize(16);
    chart->GetYAxis()->SetTitle("Speed of Light(km/s - 299000)");

    auto labels = vtkSmartPointer<vtkStringArray>::New();
    labels->SetNumberOfValues(cnt_param);
    for (int i = 0; i < cnt_param; ++i) {
        labels->SetValue(i, QString("Run %1").arg(i + 1).toUtf8().constData());
    }
    chart->GetPlot(0)->SetLabels(labels);

}
