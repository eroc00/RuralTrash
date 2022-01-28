#pragma once

enum Mode{standby, manual, test_auto, automatic};

class OpController {

public:
	OpController();
	~OpController() = default;

	Mode currMode() const; // can use switch case when calling this

	void up();

	void down();

	void disable();

private:
	Mode m_mode;

};