
#ifndef DATATYPE_HEADER
#define DATATYPE_HEADER
#include "abstruct_nodes.h"
#include "implot.h"
#include "node.h"
namespace app {
class const_node : public node {
private:
  std::any value;
  int data;

public:
  const_node(node_editor *parent) : node(parent) {
    data = 10;
    add_pin("c", node_editor::pin_type::OUTPUT);
  }
  std::any x(std::string &name) override {
    if (name == "c")
      return data;

    err_pin_not_found(name);
  }

  void draw_properites() override {
    node::draw_properites();
    ImGui::InputInt("Constant", &data);
  }
};

template <class T> class matrix_node : public runable_node {
private:
  std::vector<std::vector<T>> m;
  std::vector<T> wd;
  int stream_idx;
  int w0;
  int w1;
  float TEXT_BASE_WIDTH;
  float TEXT_BASE_HEIGHT;
  std::vector<int> n_plot;
  int c_plot;
  int c_r;
  int c_c;
  int ref_r;
  int ref_c;
  int table_idx;
  bool ref_row_loop;
  bool ref_col_loop;
  bool matrix_loop;
  struct matrix_shape {
    int r;
    int c;
  } shape;

public:
  matrix_node(node_editor *parent) : runable_node(parent) {
    stream_idx = 0;
    c_c = 0;
    c_r = 0;
    ref_c = 0;
    ref_r = 0;
    n_plot.push_back(0);
    c_plot = 0;
    shape.r = 0;
    shape.c = 0;
    table_idx = 0;

    add_pin("src", node_editor::pin_type::INPUT);
    add_pin("sc", node_editor::pin_type::INPUT);
    add_pin("sr", node_editor::pin_type::INPUT);
    add_pin("idx", node_editor::pin_type::INPUT);

    add_pin("d", node_editor::pin_type::OUTPUT);
    add_pin("dx", node_editor::pin_type::OUTPUT);
    add_pin("ptr", node_editor::pin_type::OUTPUT);
    add_pin("sr", node_editor::pin_type::OUTPUT);
    add_pin("cr", node_editor::pin_type::OUTPUT);
  }
  void run() override {
    m.clear();
    shape.r = v<int>("sr");
    shape.c = v<int>("sc");

    logger::log_info(
        std::format("inside matrix constructor, {}", sizeof(std::any)));

    std::vector<T> vv;
    vv.reserve(shape.c);
    m.reserve(shape.r);
    /// fill the matrix
    for (int x = 0; x < shape.r; x++) {
      for (int y = 0; y < shape.c; y++) {
        T x0 = v<T>("src");
        log_info(std::format("adding {}", x0));
        vv.push_back(x0);
      }
      m.push_back(vv);
    }
  }
  std::any x(std::string &name) override {
    if (name == "d") {
      if (c_r >= shape.r && c_c >= shape.c) {
        if (matrix_loop) {
          c_r = 0;
          c_c = 0;
        } else
          return static_cast<T>(0);
      }
      if (c_c >= shape.c) {
        c_c = 0;
        c_r++;
      }
      T x = m[c_r][c_c];
      c_c++;
      return x;
    }
    if (name == "dx") {
      if (ref_c >= shape.c) {
        if (!ref_row_loop)
          return static_cast<T>(0);
        else
          ref_c = 0;
      }
      T x0 = m[ref_r][ref_c];
      if (!ref_col_loop)
        ref_c++;
      return x0;
    }
    if (name == "ptr")
      return m[ref_r].data();

    if (name == "sr")
      return shape.r;

    if (name == "cr")
      return shape.c;

    err_pin_not_found(name);
  }
  void draw_properites() override {

    runable_node::draw_properites();
    if (ImGui::CollapsingHeader("Matrix Properities",
                                ImGuiTreeNodeFlags_None)) {
      ImGui::InputInt("rows", &shape.r);
      // ImGui::SameLine();
      ImGui::InputInt("col", &shape.c);
      ImGui::InputInt("reference row", &ref_r);
      // ImGui::SameLine();
      ImGui::InputInt("reference col", &ref_c);
      ImGui::Checkbox("repeat row", &ref_row_loop);
      // ImGui::SameLine();
      ImGui::Checkbox("repeat col", &ref_col_loop);
      ImGui::InputInt("current row", &c_r);
      // ImGui::SameLine();
      ImGui::InputInt("current col", &c_c);
      ImGui::Checkbox("repeat data", &matrix_loop);
    }
    if (ImGui::CollapsingHeader("Data Visualizer", ImGuiTreeNodeFlags_None)) {

      draw_plots();
    }
    if (ImGui::CollapsingHeader("Data Table", ImGuiTreeNodeFlags_None)) {

      draw_table();
    }
  }
  void draw_plots() {
    if (m.empty())
      return;
    ImPlot::BeginPlot("data plot");
    for (auto i : n_plot) {
      ImPlot::PlotLine(std::format("y = {}", i).c_str(), m[i].data(), shape.c);
    }

    ImPlot::EndPlot();
  }
  void draw_table() {
    if (m.empty())
      return;
    static ImGuiTableFlags flags =
        ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter |
        ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable |
        ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
    static int freeze_cols = 1;
    static int freeze_rows = 1;
    const int table_size = shape.c > 24 ? 24 : shape.c;
    TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
    TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

    // When using ScrollX or ScrollY we need to specify a size for our table
    // container! Otherwise by default the table will fit all available space,
    // like a BeginChild() call.
    ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);
    if (ImGui::BeginTable("table_scrollx", table_size, flags, outer_size)) {
      ImGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);
      ImGui::TableSetupColumn(
          "0",
          ImGuiTableColumnFlags_NoHide); // Make the first column not hideable
                                         // to match our use of
                                         // TableSetupScrollFreeze()
      for (int column = 1; column < table_size; column++)
        ImGui::TableSetupColumn(std::format("{}", table_idx + column).c_str());

      ImGui::TableHeadersRow();
      for (int row = 0; row < shape.r; row++) {
        ImGui::TableNextRow();
        for (int column = 0; column < table_size; column++) {
          // Both TableNextColumn() and TableSetColumnIndex() return true when
          // a column is visible or performing width measurement. Because here
          // we know that:
          // - A) all our columns are contributing the same to row height
          // - B) column 0 is always visible,
          // We only always submit this one column and can skip others.
          // More advanced per-column clipping behaviors may benefit from
          // polling the status flags via TableGetColumnFlags().
          if (!ImGui::TableSetColumnIndex(column) && column > 0)
            continue;
          if (column == 0)
            ImGui::Text("%d", row + 1);
          else
            ImGui::Text(std::format("{}", m[row][table_idx + column]).c_str());
        }
      }
      ImGui::EndTable();
      int floor = m[0].size() - table_size > 0 ? m[0].size() - table_size : 0;
      ImGui::SliderInt("slider int", &table_idx, 0, floor);
    }
  }
};
template <class T> class vmatrix_node : public runable_node {
private:
  T *m;
  std::vector<T> wd;
  int stream_idx;
  int w0;
  int w1;
  float TEXT_BASE_WIDTH;
  float TEXT_BASE_HEIGHT;
  std::vector<int> n_plot;
  int c_plot;
  int c_r;
  int c_c;
  int ref_r;
  int ref_c;
  int table_idx;
  bool ref_row_loop;
  bool ref_col_loop;
  bool matrix_loop;
  bool empty;
  struct matrix_shape {
    int r;
    int c;
  } shape;

public:
  vmatrix_node(node_editor *parent) : runable_node(parent) {
    stream_idx = 0;
    c_c = 0;
    c_r = 0;
    ref_c = 0;
    ref_r = 0;
    n_plot.push_back(0);
    c_plot = 0;
    shape.r = 0;
    shape.c = 0;
    table_idx = 0;
    empty = true;

    add_pin("src", node_editor::pin_type::INPUT);
    add_pin("sc", node_editor::pin_type::INPUT);
    add_pin("sr", node_editor::pin_type::INPUT);
    add_pin("idx", node_editor::pin_type::INPUT);

    add_pin("d", node_editor::pin_type::OUTPUT);
    add_pin("dx", node_editor::pin_type::OUTPUT);
    add_pin("ptr", node_editor::pin_type::OUTPUT);
    add_pin("sr", node_editor::pin_type::OUTPUT);
    add_pin("cr", node_editor::pin_type::OUTPUT);
  }
  void run() override {
    shape.r = v<int>("sr");
    shape.c = v<int>("sc");
    m = v<T*>("src");
    logger::log_info(
        std::format("inside matrix constructor, {}", sizeof(std::any)));
  }
  std::any x(std::string &name) override {
    if (name == "d") {
      if (c_r >= shape.r && c_c >= shape.c) {
        if (matrix_loop) {
          c_r = 0;
          c_c = 0;
        } else
          return static_cast<T>(0);
      }
      if (c_c >= shape.c) {
        c_c = 0;
        c_r++;
      }
      T x = m[c_r * shape.r + c_c];
      c_c++;
      return x;
    }
    if (name == "dx") {
      if (ref_c >= shape.c) {
        if (!ref_row_loop)
          return static_cast<T>(0);
        else
          ref_c = 0;
      }
      T x0 = m[ref_r * shape.r + ref_c];
      if (!ref_col_loop)
        ref_c++;
      return x0;
    }
    if (name == "ptr")
      return m;

    if (name == "sr")
      return shape.r;

    if (name == "cr")
      return shape.c;

    err_pin_not_found(name);
  }
  void draw_properites() override {

    runable_node::draw_properites();
    if (ImGui::CollapsingHeader("Matrix Properities",
                                ImGuiTreeNodeFlags_None)) {
      ImGui::InputInt("rows", &shape.r);
      // ImGui::SameLine();
      ImGui::InputInt("col", &shape.c);
      ImGui::InputInt("reference row", &ref_r);
      // ImGui::SameLine();
      ImGui::InputInt("reference col", &ref_c);
      ImGui::Checkbox("repeat row", &ref_row_loop);
      // ImGui::SameLine();
      ImGui::Checkbox("repeat col", &ref_col_loop);
      ImGui::InputInt("current row", &c_r);
      // ImGui::SameLine();
      ImGui::InputInt("current col", &c_c);
      ImGui::Checkbox("repeat data", &matrix_loop);
    }
    if (ImGui::CollapsingHeader("Data Visualizer", ImGuiTreeNodeFlags_None)) {

      draw_plots();
    }
    if (ImGui::CollapsingHeader("Data Table", ImGuiTreeNodeFlags_None)) {

      draw_table();
    }
  }
  void draw_plots() {
    if (empty)
      return;
    ImPlot::BeginPlot("data plot");
    for (auto i : n_plot) {
      ImPlot::PlotLine(std::format("y = {}", i).c_str(), m[i * shape.r],
                       shape.c);
    }

    ImPlot::EndPlot();
  }
  void draw_table() {
    if (empty)
      return;
    static ImGuiTableFlags flags =
        ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter |
        ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable |
        ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
    static int freeze_cols = 1;
    static int freeze_rows = 1;
    const int table_size = shape.c > 24 ? 24 : shape.c;
    TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
    TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

    // When using ScrollX or ScrollY we need to specify a size for our table
    // container! Otherwise by default the table will fit all available space,
    // like a BeginChild() call.
    ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);
    if (ImGui::BeginTable("table_scrollx", table_size, flags, outer_size)) {
      ImGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);
      ImGui::TableSetupColumn(
          "0",
          ImGuiTableColumnFlags_NoHide); // Make the first column not hideable
                                         // to match our use of
                                         // TableSetupScrollFreeze()
      for (int column = 1; column < table_size; column++)
        ImGui::TableSetupColumn(std::format("{}", table_idx + column).c_str());

      ImGui::TableHeadersRow();
      for (int row = 0; row < shape.r; row++) {
        ImGui::TableNextRow();
        for (int column = 0; column < table_size; column++) {
          // Both TableNextColumn() and TableSetColumnIndex() return true when
          // a column is visible or performing width measurement. Because here
          // we know that:
          // - A) all our columns are contributing the same to row height
          // - B) column 0 is always visible,
          // We only always submit this one column and can skip others.
          // More advanced per-column clipping behaviors may benefit from
          // polling the status flags via TableGetColumnFlags().
          if (!ImGui::TableSetColumnIndex(column) && column > 0)
            continue;
          if (column == 0)
            ImGui::Text("%d", row + 1);
          else
            ImGui::Text(std::format("{}", m[row * shape.r + table_idx + column])
                            .c_str());
        }
      }
      ImGui::EndTable();
      int floor = shape.c - table_size > 0 ? shape.c - table_size : 0;
      ImGui::SliderInt("slider int", &table_idx, 0, floor);
    }
  }
};

} // namespace app
#endif
