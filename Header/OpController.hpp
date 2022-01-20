#pragma once

enum Mode{standby, manual, test_auto, automatic};

class OpController {

public:
	OpController() : m_mode(off) {}
	~OpController() = default;

	Mode currMode() const { return m_mode; } // can use switch case when calling this

	void up() {
		m_mode = (Mode)(((int)m_mode + 1) % 4);
	}

	void down() {
		m_mode = (Mode)(((int)m_mode + 3) % 4);
	}

	void disable() { m_mode = standby; };

private:
	Mode m_mode;

};