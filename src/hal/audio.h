#pragma once
#include <cstdint>

class Audio
{
public:
	void begin();
	void update();
	void playTone(uint32_t frequencyHz, uint32_t durationMs);

private:
	bool _toneActive = false;
	uint32_t _toneStartedAt = 0;
	uint32_t _toneDurationsMs = 0;
};
