#include "ClickablePopup.h"

#include <eventHandler.h>
#include <throw_event.h>
#include <luse.h>

static EventHandler* g_event_handler = EventHandler::get_global_event_handler();

NotifyCategoryDef(ClickablePopup, "");

TypeHandle ClickablePopup::_type_handle;
unsigned int ClickablePopup::ClickablePopup_serial = 0;

ClickablePopup::ClickablePopup(NodePath* camera) : PandaNode("popup"), EventReceiver(), m_mouse_watcher(NametagGlobals::m_mouse_watcher), m_click_sound(NametagGlobals::m_click_sound), m_rollover_sound(NametagGlobals::m_rollover_sound), m_cam(camera), m_disabled(false), m_clicked(false), m_hovered(false), m_click_state(CLICKSTATE_NORMAL), m_click_event("") {
    ClickablePopup_cat.debug() << "__init__(NodePath camera)" << std::endl;
    m_name = "ClickablePopup-";
    m_name += ClickablePopup::ClickablePopup_serial++;
    m_from_id = 0;
    m_event_parameter = EventParameter(0);
    
    if (m_mouse_watcher != nullptr && m_mouse_watcher != NULL) {
        m_region_name = m_name;
        m_region_name += "-region";
        m_region = new MouseWatcherRegion(m_region_name, 0, 0, 0, 0);
        
        std::string mouse_enter_name = "mouse-enter-";
        mouse_enter_name += m_name;
        std::string mouse_leave_name = "mouse-leave-";
        mouse_leave_name += m_name;
        std::string button_down_name = "button-down-";
        button_down_name += m_name;
        std::string button_up_name = "button-up-";
        button_up_name += m_name;
        
        m_mouse_watcher->set_enter_pattern(mouse_enter_name);
        m_mouse_watcher->set_leave_pattern(mouse_leave_name);
        m_mouse_watcher->set_button_down_pattern(button_down_name);
        m_mouse_watcher->set_button_up_pattern(button_up_name);
        
        if (m_region != nullptr && m_region != NULL) {
            if (m_mouse_watcher != nullptr && m_mouse_watcher != NULL) {
                if (!m_mouse_watcher->has_region(m_region)) {
                    ClickablePopup_cat.debug() << "Adding Region!" << std::endl;
                    m_mouse_watcher->add_region(m_region);
                    ClickablePopup_cat.debug() << "Sorting Regions!" << std::endl;
                    m_mouse_watcher->sort_regions();
                }
            }
        }
    
        ClickablePopup_cat.debug() << "Accepting Enter Pattern!" << std::endl;
        accept(mouse_enter_name);
        ClickablePopup_cat.debug() << "Accepting Leave Pattern!" << std::endl;
        accept(mouse_leave_name);
        ClickablePopup_cat.debug() << "Accepting Button Down Pattern!" << std::endl;
        accept(button_down_name);
        ClickablePopup_cat.debug() << "Accepting Button Up Pattern!" << std::endl;
        accept(button_up_name);
        ClickablePopup_cat.debug() << "Finished Initializing!" << std::endl;
    }
}

ClickablePopup::~ClickablePopup() {
    ignore_all();
}

void ClickablePopup::destroy() {
    ClickablePopup_cat.debug() << "destory()" << std::endl;
    if (m_mouse_watcher != nullptr && m_mouse_watcher != NULL) {
        m_mouse_watcher->remove_region(m_region);
    }
    ignore_all();
}

void ClickablePopup::set_click_region_event(const std::string& event, int do_id) {
    ClickablePopup_cat.debug() << "set_click_region_event(" << event << ")" << std::endl;
    if (!event.size()) {
        m_disabled = true;
    } else {
        m_click_event = event;
        m_from_id = do_id;
        m_event_parameter = EventParameter(do_id);
        m_disabled = false;
    }
    
    update_click_state();
}

int ClickablePopup::get_click_state() {
    ClickablePopup_cat.debug() << "get_click_state()" << std::endl;
    
    if (!m_click_state) {
        m_click_state = 0;
    }
    
    return m_click_state;
}


const std::string ClickablePopup::get_event(const std::string& pattern) {
    ClickablePopup_cat.debug() << "get_event(string pattern)" << std::endl;
    std::string result = *new std::string(pattern);
    result.replace(result.find("%r"), m_name.size(), m_name);
    return result;
}

void ClickablePopup::update_click_state() {
    ClickablePopup_cat.debug() << "update_click_state()" << std::endl;
    int state, old_state;
    
    if (m_disabled) {
        state = CLICKSTATE_DISABLED;
    } else if (m_clicked) {
        state = CLICKSTATE_CLICK;
    } else if (m_hovered) {
        state = CLICKSTATE_HOVER;
    } else {
        state = CLICKSTATE_NORMAL;
    }
        
    if (m_click_state == state) {
        return;
    }
        
    old_state = m_click_state;
    m_click_state = state;
    
    if (old_state == CLICKSTATE_NORMAL && state == CLICKSTATE_HOVER) {
        if (m_rollover_sound != nullptr && m_rollover_sound != NULL) {
            m_rollover_sound->play();  
        }
    } else if (state == CLICKSTATE_CLICK) {
        if (m_click_sound != nullptr && m_click_sound != NULL) {
            m_click_sound->play();  
        }
    } else if (old_state == CLICKSTATE_CLICK && state == CLICKSTATE_HOVER) {
        if (m_from_id != 0) {
            throw_event(m_click_event, m_event_parameter);
        } else {
            throw_event(m_click_event);
        }
    }
}

void ClickablePopup::accept(const std::string& ev) {
    ClickablePopup_cat.debug() << "accept(" << ev << ")" << std::endl;
    g_event_handler->add_hook(ev, &ClickablePopup::handle_event, (void*)this);
}

void ClickablePopup::ignore_all() {
    ClickablePopup_cat.debug() << "ignore_all()" << std::endl;
    g_event_handler->remove_hooks_with(this);
}

void ClickablePopup::update_click_region(float left, float right, float bottom, float top) {
    ClickablePopup_cat.debug() << "update_click_region(" << left << " " << right << " " << bottom << " " << top << ")" << std::endl;
    CPT(TransformState) transform = NodePath::any_path(this).get_net_transform();
    if (m_cam != nullptr && m_cam != NULL) {
        CPT(TransformState) cam_transform = m_cam->get_net_transform();
        transform = cam_transform->get_inverse()->compose(transform);
    }
    
    transform = transform->set_quat(LQuaternionf());
    LMatrix4f mat = transform->get_mat();
    LVecBase3f c_top_left = mat.xform_point(LPoint3f(left, 0, top));
    LVecBase3f c_bottom_right = mat.xform_point(LPoint3f(right, 0, bottom));
    
    LPoint2f s_top_left, s_bottom_right;
    
    if (m_cam != nullptr && m_cam != NULL) {
        PT(Lens) lens = DCAST(Camera, m_cam->node())->get_lens();
        
        if (!lens->project(LPoint3f(c_top_left), s_top_left) || !lens->project(LPoint3f(c_bottom_right), s_bottom_right)) {
            if (m_region != nullptr && m_region != NULL) {
                m_region->set_active(false);
            }
            return;
        }
    } else {
        s_top_left = LPoint2f(s_top_left.get_x(), s_top_left.get_y());
        s_bottom_right = LPoint2f(s_bottom_right.get_x(), s_bottom_right.get_y());
    }
    
    if (m_region != nullptr && m_region != NULL) {
        m_region->set_frame(s_top_left.get_x(), s_bottom_right.get_x(), s_top_left.get_y(), s_bottom_right.get_y());
        m_region->set_active(true);
    }
}

void ClickablePopup::mouse_enter(const Event* ev) {
    ClickablePopup_cat.debug() << "mouse_enter(const Event ev)" << std::endl;
    m_hovered = true;
    update_click_state();
}

void ClickablePopup::mouse_leave(const Event* ev) {
    ClickablePopup_cat.debug() << "mouse_leave(const Event ev)" << std::endl;
    m_hovered = false;
    update_click_state();
}

void ClickablePopup::button_down(const Event* ev) {
    ClickablePopup_cat.debug() << "button_down(const Event ev)" << std::endl;
    if (ev->get_parameter(1).get_string_value() == "button1") {
        m_clicked = true;
        update_click_state();
    }
}

void ClickablePopup::button_up(const Event* ev) {
    ClickablePopup_cat.debug() << "button_up(const Event ev)" << std::endl;
    if (ev->get_parameter(1).get_string_value() == "button1") {
        m_clicked = false;
        update_click_state();
    }
}


void ClickablePopup::handle_event(const Event* ev, void* data) {
    ClickablePopup_cat.debug() << "handle_event(const Event ev, void* data)" << std::endl;
    ClickablePopup* _this = (ClickablePopup*)data;
    const std::string name = ev->get_name();
    
    if (name == _this->get_event(_this->m_mouse_watcher->get_enter_pattern()))
        _this->mouse_enter(ev);
        
    if (name == _this->get_event(_this->m_mouse_watcher->get_leave_pattern()))
        _this->mouse_leave(ev);
    
    if (name == _this->get_event(_this->m_mouse_watcher->get_button_down_pattern()))
        _this->button_down(ev);
    
    if (name == _this->get_event(_this->m_mouse_watcher->get_button_up_pattern()))
        _this->button_up(ev);
}