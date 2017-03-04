#pragma once

#include "util.h"

#include "ClickablePopup.h"
#include "NametagGlobals.h"

#include <nodepath.h>
#include <pandanode.h>
#include <textNode.h>
#include <textFont.h>
#include <mouseWatcher.h>
#include <pandabase.h>

class Nametag;
class ChatBalloon;
class NametagGroup; 

class EXPCL_LIBOTP Nametag : public ClickablePopup {
    
    friend class NametagGroup;
    
    PUBLISHED:
        Nametag(bool is_3d=false);
        ~Nametag();

        void set_draw_order(uint8_t draw_order);
        void clear_draw_order();
        void set_contents(int contents);
        void set_active(bool active);
        void set_chat_wordwrap(uint16_t chat_wordwrap);
        void set_avatar(NodePath *avatar);
        void clear_avatar();

        uint8_t get_draw_order();
        unsigned int get_contents();
        bool get_active();
        uint16_t get_chat_wordwrap();
        NodePath *get_avatar();

    public:
        virtual void destroy();
    
        uint8_t CSpeech;
        uint8_t CThought;
        uint8_t CName;
        
        static const float name_padding;
        static const float chat_alpha;
        
    protected:
        virtual void show_balloon(ChatBalloon* balloon, const std::wstring& text);
        virtual void click_state_changed();
        virtual void show_thought();
        virtual void show_speech();
        virtual void show_name();
        virtual void update();
        
        virtual ChatBalloon* get_speech_balloon()=0;
        virtual ChatBalloon* get_thought_balloon()=0;
        
        NodePath get_button();
        
        uint8_t m_draw_order;
        
        int m_contents;
        int m_chat_flags;
        
        float m_wordwrap;
        float m_chat_wordwrap;
        
        bool m_active;
        
        PT(TextFont) m_font;
        
        LVecBase4f m_name_fg;
        LVecBase4f m_name_bg;
        LVecBase4f m_chat_fg;
        LVecBase4f m_chat_bg;
        LVecBase4f m_qt_color;
        
        NametagGlobals::ColorCode m_color_code;
        
        NodePath *m_avatar;
        NodePath m_inner_np;
        NodePath m_icon;
        
        buttons_map_t m_buttons;
        
        std::wstring m_name;
        std::wstring m_display_name;
        std::wstring m_chat_string;

    private:
        //PandaNode m_pandaNode;

    TYPE_HANDLE(Nametag, TypedObject);
};