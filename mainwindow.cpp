#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "00headers.h"
#include <QVTKOpenGLStereoWidget.h>
#include <vtkChartXY.h>
#include <vtkFloatArray.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkTable.h>
#include <vtkUnsignedCharArray.h>
#include <vtkAutoInit.h>
#include <QDir>
#include <vtkNamedColors.h>

VTK_MODULE_INIT(vtkRenderingContextOpenGL2);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::init()
{
    auto vtk_w = std::make_shared<QVTKOpenGLStereoWidget>();
    auto vtk_cv = vtkSmartPointer<vtkContextView>::New();
    vtk_cv->SetRenderWindow(vtk_w->renderWindow());
    vtk_widget = vtk_w;
    ui->layout->addWidget(vtk_widget.get());

    if (views.size() != 1) {
        resize_render(1, 1, 1);
    }

    init_examples();
}

void MainWindow::init_examples()
{
    auto &&get_class_name = [this]() {
        QStringList names;
        auto dir = QDir(QDir::currentPath());
        dir.cdUp(); dir.cdUp();
        auto file_path = QDir(dir).absoluteFilePath("00headers.h");
        QFile file(file_path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << file_path << "open fail";
            return names;
        }
        QTextStream in(&file);
        QString content = in.readAll();
        file.close();

        // 使用正则匹配 namespace 名称
        static QRegularExpression regex(R"(namespace\s*(\w+)\s+)");
        QRegularExpressionMatchIterator i = regex.globalMatch(content);
        while (i.hasNext()) {
            names << i.next().captured(1);
        }
        return names;
    };

    auto &&init_table = [](QTableWidget *table, QStringList files) {
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        auto cnt_row = files.size();
        table->setRowCount(cnt_row);
        table->setColumnCount(1);
        for (int i = 0; i < cnt_row; ++i) {
            table->setItem(i, 0, new QTableWidgetItem(files.at(i)));
        }
    };
    init_table(ui->table, get_class_name());
    connect(ui->table, &QTableWidget::itemDoubleClicked, this, [this](QTableWidgetItem *item) {
        ui->lineEdit_name->setText(QString("%1-%2").arg(item->row()+ 1).arg(item->text()));
        do_something(item->text());
    });

    chartxy = vtkSmartPointer<vtkChartXY>::New();
}

void MainWindow::do_something(QString name_class)
{
    views[0]->GetScene()->RemoveAllItems();
    if (name_class == "AreaPlot") {
        auto id = views[0]->GetScene()->AddItem(chartxy);
        AreaPlot::Draw(chartxy);
    }
    do_render();
}

void MainWindow::resize_render(int count, int grid_rows, int grid_cols)
{
    for (auto &view : views) {
        auto render = view->GetRenderer();
        vtk_widget->renderWindow()->RemoveRenderer(render);
    }
    views.clear();
    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    std::vector<double *> color_list = {
        colors->GetColor3d("SlateGray").GetData(),
        colors->GetColor3d("DarkSlateGray").GetData(),
    };
    for (int i = 0; i < count; ++i) {
        auto view = vtkSmartPointer<vtkContextView>::New();
        auto render = view->GetRenderer();
        render->SetBackground(color_list.at(i % color_list.size()));
        view->SetRenderWindow(vtk_widget->renderWindow());
        views.push_back(view);
    }
    Helper::layout_renders_in_grid(views, grid_rows, grid_cols);
}

void MainWindow::do_render()
{
    vtk_widget->renderWindow()->Render();
}

