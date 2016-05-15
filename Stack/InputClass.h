#pragma once
#include <vector>
class InputClass
{
public:
	void Initialize();

	void Reset();
	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool m_keys[256];
	std::vector<int> m_pressedKeys;
};

