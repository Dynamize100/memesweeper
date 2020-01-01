#pragma once
#include "Graphics.h"

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
		void Draw(const Vei2& screenPos, Graphics& gfx) const;
		void Reveal();
		bool IsRevealed() const;
		void ToggleFlag();
		bool IsFlagged() const;
	private:
		State state = State::HIDDEN;
		bool hasMeme = false;
	};
public:
	MemeField(int nMemes);
	void Draw(Graphics& gfx) const;
	RectI MemeField::GetRect() const;
	void OnRevealClick(const Vei2& screePos);
	void OnFlagClick(const Vei2& screePos);
private:
	Tile& MemeField::TileAt(const Vei2& gridPos);
	const Tile& MemeField::TileAt(const Vei2& gridPos) const;
	Vei2 ScreenToGrid(const Vei2& screenPos);
private:
	static constexpr int width = 20;
	static constexpr int heigth = 20;
	Tile field[width*heigth];
};
