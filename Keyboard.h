#pragma once

#include <bitset>
#include <queue>
#include <optional>

#define VK_S 0x53

class Keyboard
{
public:
	class Event
	{
	private:
		unsigned char code;
		bool state;

	public:
		Event(unsigned char code, bool state) noexcept;
		Event(const Event&) = default;
		Event operator=(const Event&) = delete;

		unsigned char GetCode() const noexcept;
		bool IsPressing() const noexcept;
	};

private:
	static constexpr size_t maxKeys{ 256u };
	static constexpr size_t maxEvents{ 16u };
	std::bitset<maxKeys> keyboardState{};
	std::queue<Event> eventQueue{};
	std::queue<char> charQueue{};

public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard operator=(const Keyboard&) = delete;
	Keyboard(const Keyboard&&) = delete;
	Keyboard operator=(const Keyboard&&) = delete;

	void Clear() noexcept;

	std::optional<Event> ReadKey() noexcept;
	char ReadChar() noexcept;

	bool IsKeyDown(unsigned char code) const noexcept;
	bool IsCharEmpty() const noexcept;

	void OnKeyPressed(unsigned char code) noexcept;
	void OnKeyReleased(unsigned char code) noexcept;
	void OnChar(char character) noexcept;

private:
	template<typename T>
	static void TrimQueue(std::queue<T>& queue) noexcept;
};