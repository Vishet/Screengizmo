#include "Keyboard.h"

Keyboard::Event::Event(unsigned char code, bool pressed) noexcept :
	code{ code },
	state{ pressed }
{
}

unsigned char Keyboard::Event::GetCode() const noexcept
{
	return code;
}

bool Keyboard::Event::IsPressing() const noexcept
{
	return state;
}

void Keyboard::Clear() noexcept
{
	keyboardState.reset();
}

std::optional <Keyboard::Event> Keyboard::ReadKey() noexcept
{
	if (!eventQueue.empty())
	{
		Keyboard::Event e{ eventQueue.front() };
		eventQueue.pop();
		return e;
	}
	else
	{
		return {};
	}
}

char Keyboard::ReadChar() noexcept
{
	if (!charQueue.empty())
	{
		char character{ charQueue.front() };
		charQueue.pop();
		return character;
	}
	else
	{
		return 0;
	}
}

bool Keyboard::IsKeyDown(unsigned char code) const noexcept
{
	return keyboardState.test(code);
}

bool Keyboard::IsCharEmpty() const noexcept
{
	return charQueue.empty();
}

void Keyboard::OnKeyPressed(unsigned char code) noexcept
{
	keyboardState[code] = true;
	eventQueue.emplace(Event(code, true));
	TrimQueue(eventQueue);
}

void Keyboard::OnKeyReleased(unsigned char code) noexcept
{
	keyboardState[code] = false;
	eventQueue.emplace(Event(code, false));
	TrimQueue(eventQueue);
}

void Keyboard::OnChar(char character) noexcept
{
	charQueue.push(character);
	TrimQueue(charQueue);
}

template<typename T>
void Keyboard::TrimQueue(std::queue<T>& queue) noexcept
{
	while (queue.size() > maxEvents)
	{
		queue.pop();
	}
}