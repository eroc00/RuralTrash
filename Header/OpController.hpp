#pragma once

enum Mode{standby, manual, test_auto, automatic, transition_state};

class OpController {

public:
	OpController();
	~OpController() = default;

	Mode currMode() const; // can use switch case when calling this

	Mode setMode(int mode);

	void up();

	void down();

	void disable();

private:
	Mode m_mode;

};
