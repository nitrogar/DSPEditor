#include "node_pin.h"
#include "ImGuiShapes.h"
#include "logger.h"
#include "node.h"
namespace app {

node_pin::node_pin(node *parent, std::string &name,
                   node_editor::pin_type type) {
  this->parent = parent;
  this->name = name;
  this->id = parent->get_pin_id();
  this->type = type;
  this->state = FLOATING;
}
void node_pin::draw() {
  if (type == node_editor::pin_type::OUTPUT) {
    auto alpha = ImGui::GetStyle().Alpha;
    imnode::BeginPin(this->id, imnode::PinKind::Output);
    imnode::PinPivotAlignment(ImVec2(1.0f, 0.5f));
    imnode::PinPivotSize(ImVec2(0, 0));
    ImGui::BeginHorizontal(this->id.AsPointer());
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);

    if (state == WIRED) {
      ImGui::DrawFilledCircle(10, color);
      logger::log_info("ping status is wired");
    }

    else if (state == FLOATING) {
      ImGui::DrawCircle(10, color);
      logger::log_info("ping status is floating");
    }
    if (!this->name.empty()) {
      ImGui::TextUnformatted(this->name.c_str());
      ImGui::Spring(0);
    }
    ImGui::Spring(0, ImGui::GetStyle().ItemSpacing.x / 2);
    ImGui::EndHorizontal();
    ImGui::PopStyleVar();
    imnode::EndPin();
  } else {
    const float rounding = 5.0f;
    const float padding = 12.0f;
    //ImGui::Dummy(ImVec2(0, padding));
    //ImGui::Spring(1, 0);
    auto inputsRect = ImGui_GetItemRect();

    imnode::PushStyleVar(imnode::StyleVar_PinArrowSize, 10.0f);
    imnode::PushStyleVar(imnode::StyleVar_PinArrowWidth, 10.0f);
#if IMGUI_VERSION_NUM > 18101
    imnode::PushStyleVar(imnode::StyleVar_PinCorners,
                         ImDrawFlags_RoundCornersBottom);
#else
    imnode::PushStyleVar(imnode::StyleVar_PinCorners, 12);
#endif
    imnode::BeginPin(id, imnode::PinKind::Input);
    imnode::PinPivotRect(inputsRect.GetTL(), inputsRect.GetBR());
    imnode::PinRect(inputsRect.GetTL(), inputsRect.GetBR());
    imnode::EndPin();
    imnode::PopStyleVar(3);
  }
}

void node_pin::Begin() {
  imnode::PinKind pinKind = type == node_editor::pin_type::INPUT
                                ? imnode::PinKind::Input
                                : imnode::PinKind::Output;
  imnode::BeginPin(id, pinKind);
}
void node_pin::connect_to(node_link *l) {
  this->link = l;
  this->state = WIRED;
}
void node_pin::End() { imnode::EndPin(); }
} // namespace app
