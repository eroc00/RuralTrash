#include "OpController.hpp"

OpController::OpController() : m_mode(off) {}

Mode OpController::currMode() const { return m_mode; } // can use switch case when calling this

void OpController::up() {
	m_mode = (Mode)(((int)m_mode + 1) % 4);
}

void OpController::down() {
	m_mode = (Mode)(((int)m_mode + 3) % 4);
}

void OpController::disable() { m_mode = standby; };