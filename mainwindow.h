#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVTKOpenGLStereoWidget.h>
#include <vtkChartXY.h>
#include <vtkContextView.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void init();
    void init_examples();
    void do_something(QString name_class);

    void clear();
    void resize_render(int count, int grid_rows, int grid_cols);
    void do_render();

private:
    Ui::MainWindow *ui;

    std::shared_ptr<QVTKOpenGLStereoWidget> vtk_widget;
    std::vector<vtkSmartPointer<vtkContextView>> views;
    vtkSmartPointer<vtkChartXY> chartxy;
};
#endif // MAINWINDOW_H
