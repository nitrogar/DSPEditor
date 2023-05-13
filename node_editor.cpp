#include "node_editor.h"
#include "imgui.h"
#include "imgui_node_editor.h"
#include "logger.h"
#include "node.h"
#include "node_link.h"
#include "node_pin.h"
#include "widget.h"
#include <algorithm>
#include <ostream>
#include <format>
#include "abstruct_nodes.h"
#include "math_nodes.h"
#include "datatype_nodes.h"
namespace app {

node_editor::node_editor(window *parent) : widget(parent) {
  this->start = new start_node(this);
  this->end = new start_node(this);
  this->name = std::string("Node Editor");
  this->ctx = imnode::CreateEditor();
  this->current_node = nullptr;
  nodes.push_back(new const_node(this));
  nodes.push_back(new const_node(this));
  nodes.push_back(new isum_node(this));
  nodes.push_back(new sink_node(this));
  nodes.push_back(new matrix_node<int>(this));

}

imnode::PinId node_editor::get_pin_id() { return ++current_id; }
imnode::LinkId node_editor::get_link_id() { return ++current_id; }
imnode::NodeId node_editor::get_node_id() { return ++current_id; }
void node_editor::add_pin(node_pin *p) { pins[p->id.Get()] = p; }
void node_editor::draw() {
  bool windowFocused = ImGui::IsWindowFocused();
  ImGui::Begin("node editor");
  imnode::SetCurrentEditor(this->ctx);
  imnode::Begin(name.c_str());
  imnode::EnableShortcuts(windowFocused);
   start->draw();
   end->draw();
  for (auto &n : this->nodes) {
    n->draw();
  }

  for (auto &l : links)
    imnode::Link(l->id, l->from->id, l->to->id, l->color, l->thickness);

  // Handle creation action, returns true if editor want to create new object
  // (node or link)

  if (imnode::BeginCreate()) {

    imnode::PinId inputPinId, outputPinId;
    if (imnode::QueryNewLink(&inputPinId, &outputPinId)) {
      // QueryNewLink returns true if editor want to create new link between
      // pins.
      //
      // Link can be created only for two valid pins, it is up to you to
      // validate if connection make sense. Editor is happy to make any.
      //
      // Link always goes from input to output. User may choose to drag
      // link from output pin or input pin. This determine which pin ids
      // are valid and which are not:
      //   * input valid, output invalid - user started to drag new ling from
      //   input pin
      //   * input invalid, output valid - user started to drag new ling from
      //   output pin
      //   * input valid, output valid   - user dragged link over other pin,
      //   can
      //   be validated
      logger::log_info(std::format("Tryig to create a link between {} {}",inputPinId.Get(),outputPinId.Get()));
      if (inputPinId && outputPinId) // both are valid, let's accept link
      {
        // imnode::AcceptNewItem() return true when user release mouse button.
        if (imnode::AcceptNewItem()) {
          // Since we accepted new link, lets add one to our list of links.
           logger::log_info("accepting links");
           auto ll = new node_link(this);
           auto pi = pins[inputPinId.Get()];
           auto po = pins[outputPinId.Get()];
           ll->from = pi;
           ll->to = po;
           links.push_back(ll);
           pi->connect_to(ll);
           po->connect_to(ll);
          //// Draw new link.
           imnode::Link(ll->id, ll->from->id, ll->to->id);
        }

        // You may choose to reject connection between these nodes
        // by calling imnode::RejectNewItem(). This will allow editor to give
        // visual feedback by changing link thickness and color.
      }
    }
  }
  imnode::EndCreate(); // Wraps up object creation action handling.

   logger::log_info("start deleting");
   // Handle deletion action
   if (imnode::BeginDelete()) {
     // There may be many links marked for deletion, let's loop over them.
     imnode::LinkId deletedLinkId;
     while (imnode::QueryDeletedLink(&deletedLinkId)) {
       // If you agree that link can be deleted, accept deletion.
       if (imnode::AcceptDeletedItem()) {
         // Then remove link from your data.
         // logger::log_info("accept for deleting");
         auto x = std::remove_if(
             links.begin(), links.end(),
             [deletedLinkId](node_link *l) { return l->id == deletedLinkId; });
         links.erase(x);
       }

       // You may reject link deletion by calling:
       // imnode::RejectDeletedItem();
     }
   }
   imnode::EndDelete(); // Wrap up deletion action

   // End of interaction with editor.

      imnode::End();
   imnode::SetCurrentEditor(nullptr);
   ImGui::End();
   ShowProperities();
   ShowControlMenu();
}

    void node_editor::set_current_node(node * n){current_node = n;}
  void node_editor::ShowProperities(
      bool *show) {
    if (!ImGui::Begin("Node Properities")) {
      ImGui::End();
      return;
    }
    if(current_node)
      current_node->draw_details();
    ImGui::End();
  }
  void node_editor::ShowControlMenu(bool *show) {
    ImGui::Begin("Node Actions");

    if (ImGui::Button("RUN")) {
      logger::log_info("RUN");
    }
    ImGui::SameLine();
   if (ImGui::Button("STOP")) {
      //... my_code
    }
   ImGui::SameLine();
    if (ImGui::Button("ADD")) {
      //... my_code
    }

    ImGui::End();
  }
    void node_editor::ShowStyleEditor(bool *show) {
      if (!ImGui::Begin("Style", show)) {
       ImGui::End();
       return;
      }

      auto paneWidth = ImGui::GetContentRegionAvail().x;
      auto &editorStyle = imnode::GetStyle();
      ImGui::BeginHorizontal("Style buttons", ImVec2(paneWidth, 0), 1.0f);
      ImGui::TextUnformatted("Values");
      ImGui::Spring();
      if (ImGui::Button("Reset to defaults"))
       editorStyle = imnode::Style();
      ImGui::EndHorizontal();
      ImGui::Spacing();
      ImGui::DragFloat4("Node Padding", &editorStyle.NodePadding.x, 0.1f, 0.0f,
                        40.0f);
      ImGui::DragFloat("Node Rounding", &editorStyle.NodeRounding, 0.1f, 0.0f,
                       40.0f);
      ImGui::DragFloat("Node Border Width", &editorStyle.NodeBorderWidth, 0.1f,
                       0.0f, 15.0f);
      ImGui::DragFloat("Hovered Node Border Width",
                       &editorStyle.HoveredNodeBorderWidth, 0.1f, 0.0f, 15.0f);
      ImGui::DragFloat("Selected Node Border Width",
                       &editorStyle.SelectedNodeBorderWidth, 0.1f, 0.0f, 15.0f);
      ImGui::DragFloat("Pin Rounding", &editorStyle.PinRounding, 0.1f, 0.0f,
                       40.0f);
      ImGui::DragFloat("Pin Border Width", &editorStyle.PinBorderWidth, 0.1f,
                       0.0f, 15.0f);
      ImGui::DragFloat("Link Strength", &editorStyle.LinkStrength, 1.0f, 0.0f,
                       500.0f);
      // ImVec2  SourceDirection;
      // ImVec2  TargetDirection;
      ImGui::DragFloat("Scroll Duration", &editorStyle.ScrollDuration, 0.001f,
                       0.0f, 2.0f);
      ImGui::DragFloat("Flow Marker Distance", &editorStyle.FlowMarkerDistance,
                       1.0f, 1.0f, 200.0f);
      ImGui::DragFloat("Flow Speed", &editorStyle.FlowSpeed, 1.0f, 1.0f,
                       2000.0f);
      ImGui::DragFloat("Flow Duration", &editorStyle.FlowDuration, 0.001f, 0.0f,
                       5.0f);
      // ImVec2  PivotAlignment;
      // ImVec2  PivotSize;
      // ImVec2  PivotScale;
      // float   PinCorners;
      // float   PinRadius;
      // float   PinArrowSize;
      // float   PinArrowWidth;
      ImGui::DragFloat("Group Rounding", &editorStyle.GroupRounding, 0.1f, 0.0f,
                       40.0f);
      ImGui::DragFloat("Group Border Width", &editorStyle.GroupBorderWidth,
                       0.1f, 0.0f, 15.0f);

      ImGui::Separator();

      static ImGuiColorEditFlags edit_mode = ImGuiColorEditFlags_DisplayRGB;
      ImGui::BeginHorizontal("Color Mode", ImVec2(paneWidth, 0), 1.0f);
      ImGui::TextUnformatted("Filter Colors");
      ImGui::Spring();
      ImGui::RadioButton("RGB", &edit_mode, ImGuiColorEditFlags_DisplayRGB);
      ImGui::Spring(0);
      ImGui::RadioButton("HSV", &edit_mode, ImGuiColorEditFlags_DisplayHSV);
      ImGui::Spring(0);
      ImGui::RadioButton("HEX", &edit_mode, ImGuiColorEditFlags_DisplayHex);
      ImGui::EndHorizontal();

      static ImGuiTextFilter filter;
      filter.Draw("##", paneWidth);

      ImGui::Spacing();

      ImGui::PushItemWidth(-160);
      for (int i = 0; i < imnode::StyleColor_Count; ++i) {
       auto name = imnode::GetStyleColorName((imnode::StyleColor)i);
       if (!filter.PassFilter(name))
         continue;

       ImGui::ColorEdit4(name, &editorStyle.Colors[i].x, edit_mode);
      }
      ImGui::PopItemWidth();

      ImGui::End();
    }

  } // namespace app
