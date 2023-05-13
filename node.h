#ifndef NODE_H
#define NODE_H

#include "imgui.h"
#include "node_editor.h"
#include "node_pin.h"
#include <cstdint>
#include <format>
#include <iostream>
#include <vector>
#include <any>
#include "node_link.h"
namespace app {
 
class node {
protected:
  node * flow;
  std::vector<node_pin *> in_pins;
  std::vector<node_pin *> out_pins;
  imnode::NodeId id;
  ImU32 header_color = ImColor(59, 59, 209);
  node_editor *parent;
  std::string name;
   bool logs_auto_scroll;
  enum LogLevel { ERROR, WARNING, INFO };
  std::string logs;

public:
  node(node_editor *parent);
  ~node(){};
  void add_pin(){};
  void delete_pin(){};
  void virtual draw();
  void virtual draw_header();
  void virtual draw_pins();
  void draw_details();
  void draw_logs();
   void log(std::string &str, LogLevel level);
   void log_info(std::string s);
   void log_warning(std::string s);
   void log_error(std::string s);

   void virtual draw_properites();
   virtual void process(){};
   void add_pin(std::string name, node_editor::pin_type type);
   virtual void evaluate_pin(node_pin *pin){};
   imnode::PinId get_pin_id();

   void err_pin_not_found(std::string &name) {
     logger::log_error(
         std::format("trying to access pin {} which does not exits", name));
  }
  virtual std::any x(std::string & name){};
  template<typename T>
  T v(std::string  name){
    T ret;
    node_pin * ppin = nullptr;
    for(auto p : in_pins){
      if (p->name == name) {
        ppin = p;
        logger::log_info(
                         std::format("pin searching for {} == {}", name, p->name));
        break;
}
      logger::log_info(std::format("pin searching for {} != {}",name,p->name));
    }
    logger::log_info(std::format("ppin == {:#010x}",reinterpret_cast<intptr_t>(ppin)));
    if(!ppin)
      logger::log_error(std::format("trying to access pin {} which does not exits, {}",name,(void *)ppin));

    logger::log_info(
        std::format("ppin == {:#010x}", reinterpret_cast<intptr_t>(ppin)));
    node_pin *that_pin = ppin->link->from;
    node *nn =that_pin->parent;
    if(!nn)
      logger::log_error(
                        std::format("can not reach remote node,local pin {}, remote pin {}, link {}",name,ppin->name,ppin->link->name));

    auto val = nn->x(that_pin->name);

    if(!val.has_value())
      logger::log_error(std::format("trying to use no any value, local pin {}, remote pin {}",name,nn->name));
        try {
          ret = std::any_cast<T>(val);
        }
        catch(const std::bad_any_cast & e){
          logger::log_error(std::format("casting error {}",e.what()));
        }
        return ret;
  }
};
class start_node : public node {
public:
  start_node(node_editor *parent) : node(parent) {
    add_pin("in1", node_editor::pin_type::INPUT);
    add_pin("in1", node_editor::pin_type::INPUT);
    add_pin("out1", node_editor::pin_type::OUTPUT);
  }
  // void draw() override {
  //  logger::log_info("drawing the start node");
  //  std::cout << "n_id: " << n_id.Get() << " pi_id:" << pi_id.Get()
  //            << " po_id:" << po_id.Get() << std::endl;
  //  imnode::BeginNode(n_id);
  //  ImGui::Text("Node A");
  //  imnode::BeginPin(pi_id, imnode::PinKind::Input);
  //  ImGui::Text("-> In");
  //  imnode::EndPin();
  //  ImGui::SameLine();
  //  imnode::BeginPin(po_id, imnode::PinKind::Output);
  //  ImGui::Text("Out ->");
  //  imnode::EndPin();
  //  imnode::EndNode();
  //}

private:
  imnode::NodeId n_id;
  imnode::PinId pi_id;
  imnode::PinId po_id;
};

} // namespace app

#endif // DEBUG
