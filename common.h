#ifndef COMMON_H
#define COMMON_H

#include <vtkContextView.h>
#include <vtkContextScene.h>
#include <vtkRenderer.h>

namespace Helper{
static std::array<double, 4> get_viewport(int row, int col, int rows, int cols) {
    std::array<double, 4> viewport;
    // xmin, ymin, xmax, ymax
    viewport[0] = static_cast<double>(col) / cols;
    viewport[1] = static_cast<double>(rows - row - 1) / rows;
    viewport[2] = static_cast<double>(col + 1) / cols;
    viewport[3] = static_cast<double>(rows - row) / rows;
    return viewport;
}
static void layout_renders_in_grid(std::vector<vtkSmartPointer<vtkContextView>> views,
                                   int grid_rows, int grid_cols) {
    for (int row = 0; row < grid_rows; ++row) {
        for (int col = 0; col < grid_cols; ++col) {
            auto index = row * grid_cols + col;
            auto viewport = get_viewport(row, col, grid_rows, grid_cols);
            if (index >= views.size()) {
                break;
            }
            views[index]->GetRenderer()->SetViewport(viewport.data());
        }
    }
}
}

#endif // COMMON_H
