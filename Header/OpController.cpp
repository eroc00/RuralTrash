#include "OpController.hpp"

OpController::OpController() : m_mode(standby) {}

Mode OpController::currMode() const { return m_mode; } // can use switch case when calling this

Mode OpController::setMode(int mode) {

	m_mode = (Mode)mode;
	return m_mode;

}

void OpController::up() {
	m_mode = (Mode)(((int)m_mode + 1) % 4);
}

void OpController::down() {
	m_mode = (Mode)(((int)m_mode + 3) % 4);
}

void OpController::disable() { m_mode = standby; };
