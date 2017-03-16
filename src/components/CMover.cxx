#include "CMover.h"

NotifyCategoryDef(CMover, "");

TypeHandle CMover::_type_handle;

CMover::CMover(NodePath m_nodepath) {
    m_fwdSpeed = 1.0;
    m_rotSpeed = 1.0;
    m_vec_type = Vec3(0, 0, 0);
}

CMover::CMover(NodePath m_nodepath, float m_fwdSpeed) {
    m_rotSpeed = 1.0;
    m_vec_type = Vec3(0, 0, 0);
}

CMover::CMover(NodePath m_nodepath, float m_fwdSpeed, float m_rotSpeed) {
    m_vec_type = Vec3(0, 0, 0);
}


CMover::~CMover() {
    
}

void CMover::set_fwd_speed(float speed) {
    m_fwdSpeed = speed;
}

void CMover::set_rot_speed(float speed) {
    m_rotSpeed = speed;
}

void CMover::set_dt(double dt) {
    ClockObject *co = ClockObject::get_global_clock();
    co->set_dt(dt);
}

void CMover::reset_dt() {
    ClockObject *co = ClockObject::get_global_clock();
    co->set_dt(0);
}

void CMover::add_c_impulse(string name, CImpulse impulse) {
    CMover_cat.debug() << "Adding CImpulse '" << name << "' to impulse map and setting it's mover to our current mover!" << std::endl;
    m_c_impulses[name] = impulse;
    m_c_impulses[name].set_mover(this);
}

void CMover::remove_c_impulse(string name) {
    if (m_c_impulses.find(name) == m_c_impulses.end()) {
        CMover_cat.debug() << "CImpulse '" << name << "' was not found in impulse map! Returning..." << std::endl;
        return;
    } else {
        pmap<std::string, CImpulse>::iterator it;
        it = m_c_impulses.find(name);
        if (it != m_c_impulses.end()) {
            // Found it? - Delete it!
            CMover_cat.debug() << "Removing CImpulse '" << name << "' from map and destorying CImpulse!" << std::endl;
            m_c_impulses[name].clear_mover(this);
            m_c_impulses.erase(it);
        } else {
            CMover_cat.debug() << "CImpulse '" << name << "' was not found in impulse map! Returning..." << std::endl;
            return;
        }
    }
}

void CMover::process_c_impulses(float dt) {
    CMover_cat.debug() << "Processing CImpulses!" << std::endl;
    for each(pair<string, CImpulse> x in m_c_impulses) {
        x.second.process(dt);
    }
}

void CMover::process_c_impulses() {
    CMover_cat.debug() << "Processing CImpulses!" << std::endl;
    for each(pair<string, CImpulse> x in m_c_impulses) {
        x.second.process(get_dt());
    }
}

void CMover::add_force(Vec3 force) {
    CMover_cat.warning() << "add_force() -- Not Implemented!" << std::endl;
}

void CMover::add_rot_force(Vec3 force) {
    CMover_cat.warning() << "add_rot_force() -- Not Implemented!" << std::endl;
}

void CMover::add_shove(Vec3 shove) {
    if (!m_nodepath.is_empty()) {
        CMover_cat.debug() << "add_shove(Vec3 shove)" << std::endl;
        vel = shove;
        rot_mat = Mat3::rotate_mat_normaxis(m_nodepath.get_h(), Vec3::up());
        step = (Vec3::zero() * get_dt()) + rot_mat.xform(vel);
        m_nodepath.set_fluid_pos(Point3(m_nodepath.get_pos() + step));
    } else {
        CMover_cat.warning() << "add_shove(Vec3 shove) -- Can't push a empty nodepath!" << std::endl;
    }
}

void CMover::add_rot_shove(Vec3 shove) {
    if (!m_nodepath.is_empty()) {
        CMover_cat.debug() << "add_rot_shove(Vec3 shove)" << std::endl;
        vel = shove;
        rotation = vel + (Vec3::zero() * get_dt());
        m_nodepath.set_hpr(Point3(m_nodepath.get_hpr() + rotation));
    } else {
        CMover_cat.warning() << "add_rot_shove(Vec3 shove) -- Can't rotate a empty nodepath!" << std::endl;
    }
}

void CMover::set_node_path(NodePath np) {
    m_nodepath = np;
}

void CMover::integrate() {
    CMover_cat.warning() << "CMover.integrate() is deperacted! Inherit the class in Python or C++ instead." << std::endl;
}

float CMover::get_fwd_speed() {
    return m_fwdSpeed;
}

float CMover::get_rot_speed() {
    return m_rotSpeed;
}

double CMover::get_dt() {
    ClockObject *co = ClockObject::get_global_clock();
    return co->get_dt();
}

CImpulse CMover::get_c_impulse(string name) {
    if (m_c_impulses.find(name) == m_c_impulses.end()) {
        CMover_cat.debug() << "CImpulse '" << name << "' was not found in impulse map! Returning a fake CImpulse..." << std::endl;
        return CImpulse();
    } else {
        return m_c_impulses[name];
    }
}

NodePath CMover::get_node_path() {
    return m_nodepath;
}
