#pragma once
#include "Graphics.h"
#include "Sound.h"

class MemeField {
private:
	class Tile {
	public:
		enum class State {
			HIDDEN,
			FLAGGED,
			REVEALED
		};
	public:
		void SpawnMeme();
		bool HasMeme() const;
		void Draw(const Vei2& screenPos, bool exploded, Graphics& gfx) const;
		void Reveal();
		bool IsRevealed() const;
		void ToggleFlag();
		bool IsFlagged() const;
		void SetNeighbourMemeCount(int memeCount);
	private:
		State state = State::HIDDEN;
		bool hasMeme = false;
		int nNeighbourMemes = -1;
	};
public:
	MemeField(const Vei2& center, int nMemes);
	void Draw(Graphics& gfx) const;
	RectI MemeField::GetRect() const;
	void OnRevealClick(const Vei2& screePos);
	void OnFlagClick(const Vei2& screePos);
	bool GameIsWon() const;
	bool GameIsLost() const;
private:
	Tile& MemeField::TileAt(const Vei2& gridPos);
	const Tile& MemeField::TileAt(const Vei2& gridPos) const;
	Vei2 ScreenToGrid(const Vei2& screenPos);
	int CountNeighbourMemes(const Vei2& gridPos);
private:
	static constexpr int width = 20;
	static constexpr int heigth = 16;
	static constexpr int borderThickness = 10;
	static constexpr Color borderColor = Colors::Blue;
	Sound sndLose = Sound(L"spayed.wav");
	Vei2 topLeft;
	bool isExploded = false;
	Tile field[width*heigth];
};
