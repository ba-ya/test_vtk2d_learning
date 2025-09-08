在 VTK 的 **2D context 系统**（`vtkChartXY`, `vtkPlot`, …）里，**Chart 必须有持久的生命周期**，不能只在函数里临时 new 出来然后交给 scene。

------

### **为什么？**

- `view->GetScene()->AddItem(chart)` 只是把 `chart` 注册到 `vtkContextScene`。
- 但是 VTK 并不会“接管”它的引用计数，渲染时还需要访问 chart。
- 如果 `chart` 是函数里的临时局部变量，函数退出后它就被销毁，导致 `view->Render()` 时访问野指针 → 崩溃。

这跟 3D 世界的 `vtkActor`/`vtkRenderer` 有点不一样：3D 管线里通常都是基于 `vtkSmartPointer` + 渲染器强引用，chart 这类 2D 对象则比较“轻”，需要你自己保持。

------

### **解决方法**

你有两种方式：

#### ✅ 方法 1：类里存一份（推荐）

在 `AreaPlot` 类里定义成员变量：

```cpp
class AreaPlot {
public:
    void Draw(std::vector<vtkSmartPointer<vtkContextView>> views);

private:
    vtkSmartPointer<vtkChartXY> chart;
};
```

实现：

```cpp
void AreaPlot::Draw(std::vector<vtkSmartPointer<vtkContextView>> views)
{
    auto view = views[0];
    chart = vtkSmartPointer<vtkChartXY>::New(); // 存成成员变量
    chart->SetTitle("Area Plot");

    view->GetScene()->AddItem(chart);
}
```

这样 `chart` 的生命周期就和 `AreaPlot` 实例一样长，不会挂掉。

------

#### ✅ 方法 2：容器里存一份

如果你没有类成员，可以用一个全局或外部容器保存：

```cpp
std::vector<vtkSmartPointer<vtkChartXY>> charts;

void AreaPlot::Draw(std::vector<vtkSmartPointer<vtkContextView>> views)
{
    auto view = views[0];
    auto chart = vtkSmartPointer<vtkChartXY>::New();
    chart->SetTitle("Area Plot");

    view->GetScene()->AddItem(chart);
    charts.push_back(chart); // 存起来
}
```

------

### **总结**

- **2D 的 chart 必须有持久引用**，不能只临时创建。
- 推荐在 `AreaPlot` 类里存为成员变量，或者维护一个全局容器。
- 3D actor 和 2D chart 在生命周期管理上的差异，是因为 `vtkContextScene` 并不会强引用 chart。

