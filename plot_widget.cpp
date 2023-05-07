#include "plot_widget.h"
#include "imgui.h"
#include "implot.h"
#include "implot_internal.h"
#include "logger.h"
namespace app {
plot_widget::plot_widget(window *parent) : widget(parent) {
  this->name = "plot widget";
}
plot_widget::~plot_widget(){};
void plot_widget::draw() {

  ImGui::Begin(name.c_str()); // Create a window called "Hello, world!"
  static float xs1[1001], ys1[1001];
  for (int i = 0; i < 1001; ++i) {
    xs1[i] = i * 0.001f;
    ys1[i] = 0.5f + 0.5f * sinf(50 * (xs1[i] + (float)ImGui::GetTime() / 10));
  }
  static double xs2[20], ys2[20];
  for (int i = 0; i < 20; ++i) {
    xs2[i] = i * 1 / 19.0f;
    ys2[i] = xs2[i] * xs2[i];
  }
  if (ImPlot::BeginPlot("Line Plots")) {
    ImPlot::SetupAxes("x", "y");
    ImPlot::PlotLine("f(x)", xs1, ys1, 1001);
    ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
    ImPlot::PlotLine("g(x)", xs2, ys2, 20, ImPlotLineFlags_Segments);
    ImPlot::EndPlot();
  }
  // use a format strings too)
  ImGui::End();
};

} // namespace app
