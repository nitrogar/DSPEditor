#include "node.h"
#include "ImGuiShapes.h"
#include "UIFontManager.h"
#include "imgui.h"
#include "logger.h"
#include "node_editor.h"
#include "node_pin.h"
#include <format>
#include <iterator>
#include <utility>
namespace app {

node::node(node_editor *parent) {

  this->parent = parent;
  this->id = parent->get_node_id();
  this->name = std::format("NODE{}", id.Get());
  logs_auto_scroll = true;
  logs = "testing logs";
  logger::log_info(
      std::format("node {} has id {}", this->name, this->id.Get()));
}
void node::draw() {
  logger::log_info(std::format("drawing node {}_{}", name, id.Get()));
  ImGui::PushID(this);
  imnode::BeginNode(this->id);
  ImGui::BeginVertical("node");
  ImGui::BeginHorizontal("header");
  ImGui::Spring(0);
  ImGui::TextUnformatted(name.c_str());
  ImGui::Spring(1);
  ImGui::Dummy(ImVec2(0, 28));
  ImGui::Spring(0);
  ImGui::EndHorizontal();
  ImGui::Spring(0, ImGui::GetStyle().ItemSpacing.y * 2.0f);

  ImGui::BeginHorizontal("content");
  ImGui::Spring(0, 4);
  ImGui::PushStyleVar(ImGuiStyleVar_LayoutAlign, 0.0f);
  ImGui::BeginVertical("inputs");
  auto alpha = ImGui::GetStyle().Alpha;
  for (auto &out : this->in_pins) {
    logger::log_info(std::format("drawing out pin {}", out->id.Get()));
    imnode::BeginPin(out->id, imnode::PinKind::Input);
    ImGui::BeginHorizontal(out);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
    ImGui::Spring(0);
    if (!out->name.empty()) {
      ImGui::TextUnformatted(out->name.c_str());
      ImGui::Spring(0);
    }
    ImGui::PopStyleVar();
    ImGui::EndHorizontal();
    imnode::EndPin();
  }
  ImGui::EndVertical();
  // fillItemRect(ImColor(0, 255, 0, 128));
  ImGui::PopStyleVar();
  ImGui::Spring();
  ImGui::PushStyleVar(ImGuiStyleVar_LayoutAlign, 1.0f);
  ImGui::BeginVertical("outputs");
  for (auto &out : this->out_pins) {
    imnode::BeginPin(out->id, imnode::PinKind::Output);
    ImGui::BeginHorizontal(out);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
    ImGui::Spring(0);
    if (!out->name.empty()) {
      ImGui::TextUnformatted(out->name.c_str());
      ImGui::Spring(0);
    }
    ImGui::PopStyleVar();
    ImGui::EndHorizontal();
    imnode::EndPin();
  }
  ImGui::EndVertical();

  ImGui::PopStyleVar();
  ImGui::Spring(0, 4);
  ImGui::EndHorizontal();
  ImGui::EndVertical();
  if (ImGui::IsItemClicked()) {
    logger::log_info(std::format("mouse is hovering now {}", name));
    parent->set_current_node(this);
  }
  imnode::EndNode();
  ImGui::PopID();

  // drawItemRect(ImColor(0, 0, 0), 2);
  // draw_header();
  // draw_pins();
  //  imnode::PushStyleVar(imnode::StyleVar_NodePadding, ImVec4(8, 4, 8, 8));
  //                imnode::BeginNode(id);
  //                ImGui::PushID(id.AsPointer());
  //                ImGui::BeginVertical("node");
  //                ImGui::BeginHorizontal("header");
  //                ImGui::Spring(0);
  //                ImGui::TextUnformatted(name.c_str());
  //                ImGui::Spring(1);
  //                ImGui::Dummy(ImVec2(0, 28));
  //                ImGui::Spring(0);
  //                ImGui::EndHorizontal();
  //                ImGui::Spring(0, ImGui::GetStyle().ItemSpacing.y * 2.0f);

  //               for (auto& out : this->in_pins)
  //                   {

  //                       ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
  //                       DrawPinIcon(out, IsPinLinked(out.ID), (int)(alpha
  //                       * 255)); ImGui::Spring(0); if (!out.Name.empty())
  //                       {
  //                           ImGui::TextUnformatted(out.Name.c_str());
  //                           ImGui::Spring(0);
  //                       }
  //                             ImGui::PopStyleVar();
  //                       builder.EndInput();
  //                   }

  //                   // if (isSimple)
  //                   // {
  //                       builder.Middle();

  //                       ImGui::Spring(1, 0);
  //                       ImGui::TextUnformatted(node.Name.c_str());
  //                       ImGui::Spring(1, 0);
  //                   // }

  //                   for (auto& output : node.Outputs)
  //                   {
  //                              auto alpha = ImGui::GetStyle().Alpha;
  //                       ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
  //                       builder.Output(output.ID);
  //                      if (!output.Name.empty())
  //                       {
  //                           ImGui::Spring(0);
  //                           ImGui::TextUnformatted(output.Name.c_str());
  //                       }
  //                       ImGui::Spring(0);
  //                       DrawPinIcon(output, IsPinLinked(output.ID),
  //                       (int)(alpha * 255)); ImGui::PopStyleVar();
  //                       builder.EndOutput();
  //                   }

  //               builder.End();
  //           }
}

void node::draw_properites() {
  if (ImGui::CollapsingHeader("Node Properities", ImGuiTreeNodeFlags_None)) {
    ImGui::Text("Name: ");
    ImGui::SameLine();
    ImGui::InputText("node name",name.data(),name.size());


  }
}
void node::draw_header() {

  ImVec2 pos = ImGui::GetCursorPos();
  ImGui::SetCursorPos(ImVec2(pos.x - imnode::GetStyle().NodePadding.x,
                             pos.y - imnode::GetStyle().NodePadding.y));
  ImVec2 start = imnode::GetNodeSize(id);
  ImGui::DrawFilledRect(ImVec2(start.x, 60), header_color, 13);
  ImGui::SetCursorPos(ImVec2(pos.x + imnode::GetStyle().NodePadding.x,
                             pos.y + imnode::GetStyle().NodePadding.x));
  ImGui::PushFont(GetUIFont("OpenSans-Bold"));
  ImGui::Text(this->name.c_str());
  ImGui::PopFont();
  ImGui::NewLine();
}

void node::draw_pins() {

  // for (auto p : pins) {
  //   p->draw();
  // }

  // ImGui::SameLine();
}

void node::add_pin(std::string name, node_editor::pin_type t) {
  auto p = new node_pin(this, name, t);
  if (t == node_editor::pin_type::INPUT)
    in_pins.push_back(p);
  if (t == node_editor::pin_type::OUTPUT)
    out_pins.push_back(p);

  parent->add_pin(p);
}

imnode::PinId node::get_pin_id() { return parent->get_pin_id(); }

void node::draw_details() {
  draw_properites();
  draw_logs();
}
 
void node::draw_logs() {

  if (ImGui::CollapsingHeader("Node Logs", ImGuiTreeNodeFlags_None)) {
    if (ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("Auto-scroll", &logs_auto_scroll);
            ImGui::EndPopup();
        }

        // Main window
        if (ImGui::Button("Options"))
            ImGui::OpenPopup("Options");
        ImGui::SameLine();
        bool clear = ImGui::Button("Clear");
        ImGui::SameLine();
        bool copy = ImGui::Button("Copy");
        ImGui::SameLine();
        //Filter.Draw("Filter", -100.0f);

        ImGui::Separator();
        ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        if (clear)
          logs.clear();
        if (copy)
            ImGui::LogToClipboard();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        const char* buf = logs.data();
        const char* buf_end = buf + logs.size();
        ImGui::TextUnformatted(buf, buf_end);
        ImGui::PopStyleVar();

        if (logs_auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();
  }
  }
void node::log(std::string &str, LogLevel level) {}
void node::log_info(std::string s) {
  std::string x = "[INFO] " + s + "\n";
  logs += x;
  //logger::log_info(std::format("adding {} to logs",s));
}
void node::log_warning(std::string s) {
  std::string x = "[WARNING] " + s + "\n";
  logs += x;
}
void node::log_error(std::string s) {
  std::string x = "[ERROR] " + s + "\n";
  logs += x;
}

} // namespace app
