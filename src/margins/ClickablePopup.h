#pragma once

#include "util.h"
#include "nametagGlobals.h"
#include <audioSound.h>
#include <pandabase.h>
#include <pandaNode.h>
#include <nodePath.h>
#include <mouseWatcher.h>
#include <mouseWatcherRegion.h>
#include <eventReceiver.h>

class ClickablePopup;

typedef void(*callback_t)(const Event* ev);
typedef pmap<std::string, callback_t> event_table_t;

class EXPCL_LIBOTP ClickablePopup : public virtual EventReceiver, public PandaNode {
    
    PUBLISHED:
        ClickablePopup(NodePath* camera=NULL);
        ~ClickablePopup();
        
    public:
        virtual void destroy();
        virtual void set_click_region_event(const std::string& event);
        
        int get_click_state();
        virtual void click_state_changed()=0;

        static void handle_event(const Event* ev, void* data);
        
    protected:
        const std::string get_event(const std::string& pattern);
        void update_click_state();
        void update_click_region(float left, float right, float bottom, float top);

        PT(MouseWatcher) m_mouse_watcher;
        PT(AudioSound) m_click_sound, m_rollover_sound;
        NodePath* m_cam;
        
        bool m_disabled;
        bool m_clicked;
        bool m_hovered;
        
        int m_click_state;
        std::string m_click_event;
        std::string m_name;
        std::string m_region_name;
        PT(MouseWatcherRegion) m_region;
    
    private:
        static unsigned int ClickablePopup_serial;
        
        void mouse_enter(const Event* ev);
        void mouse_leave(const Event* ev);
        void button_down(const Event* ev);
        void button_up(const Event* ev);
        
        void accept(const std::string& ev);
        void ignore_all();
        
    TYPE_HANDLE(ClickablePopup, TypedObject);
};