#pragma once

#include "util.h"
#include "MarginManager.h"

#include <pandabase.h>
#include <typedObject.h>

class MarginCell; 

class MarginPopup : public TypedObject {
    PUBLISHED:
        MarginPopup();
        ~MarginPopup();

        void set_visible(bool flag);
        bool get_visible();
        
        void set_priority(int priority);
        int get_priority();
        
        virtual bool is_displayed();
        
        virtual void margin_visibility_changed()=0;
        
        virtual void manage(MarginManager* manager);
        virtual void unmanage(MarginManager* manager);
        
    public:
        void set_last_cell(MarginCell* cell);
        MarginCell* get_last_cell();
        
        void set_assigned_cell(MarginCell* cell);
        MarginCell* get_assigned_cell();

    protected:
        MarginManager* m_manager;
        int m_priority;
        bool m_visible;
        MarginCell* m_last_cell;
        MarginCell* m_assigned_cell;
        
    TYPE_HANDLE(MarginPopup, TypedObject);
};