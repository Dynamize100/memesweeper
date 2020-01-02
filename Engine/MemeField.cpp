#include "MemeField.h"
#include <assert.h>
#include <random>
#include "Vei2.h"
#include "SpriteCodex.h"
#include <algorithm>

void MemeField::Tile::SpawnMeme() {
	assert(!hasMeme);
	hasMeme = true;
}

bool MemeField::Tile::HasMeme() const {
	return hasMeme;
}

void MemeField::Tile::Draw(const Vei2 & screenPos, bool exploded, Graphics & gfx) const {
	if (!exploded) {
		switch (state) {
		case State::HIDDEN:
			SpriteCodex::DrawTileButton(screenPos, gfx);
			break;
		case State::FLAGGED:
			SpriteCodex::DrawTileButton(screenPos, gfx);
			SpriteCodex::DrawTileFlag(screenPos, gfx);
			break;
		case State::REVEALED:
			if (HasMeme()) SpriteCodex::DrawTileBomb(screenPos, gfx);
			else SpriteCodex::DrawTileNumber(screenPos, nNeighbourMemes, gfx); 
			break;
		}
	} else {
		switch (state) {
		case State::HIDDEN:
			if (HasMeme()) {
				SpriteCodex::DrawTileBomb(screenPos, gfx);
			} else {
				SpriteCodex::DrawTileButton(screenPos, gfx);
			}
			break;
		case State::FLAGGED:
			if (HasMeme()) {
				SpriteCodex::DrawTileBomb(screenPos, gfx);
				SpriteCodex::DrawTileFlag(screenPos, gfx);
			} else {
				SpriteCodex::DrawTileBomb(screenPos, gfx);
				SpriteCodex::DrawTileCross(screenPos, gfx);
			}
			break;
		case State::REVEALED:
			if (HasMeme()) {
				SpriteCodex::DrawTileBombRed(screenPos, gfx);
			} else {
				SpriteCodex::DrawTileNumber(screenPos, nNeighbourMemes, gfx);
			}
			break;
		}
	}

}

void MemeField::Tile::Reveal() {
	assert(state == State::HIDDEN);
	state = State::REVEALED;
}

bool MemeField::Tile::IsRevealed() const {
	return state == State::REVEALED;
}

void MemeField::Tile::ToggleFlag() {
	assert(!IsRevealed());
	if (state == State::HIDDEN) {
		state = State::FLAGGED;
	} else {
		state = State::HIDDEN;
	}
}

bool MemeField::Tile::IsFlagged() const {
	return state == State::FLAGGED;
}

void MemeField::Tile::SetNeighbourMemeCount(int memeCount) {
	assert(nNeighbourMemes == -1);
	nNeighbourMemes = memeCount;
}

MemeField::MemeField(const Vei2& center, int nMemes) 
	:
	topLeft(center - Vei2 (width * SpriteCodex::tileSize, heigth * SpriteCodex::tileSize) / 2) {
	assert(nMemes > 0 && nMemes < width * heigth);
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> xDist(0, width - 1);
	std::uniform_int_distribution<int> yDist(0, heigth - 1);

	for (int nSpawned = 0; nSpawned < nMemes; ++nSpawned) {
		Vei2 spawnPos;
		do {
			spawnPos = { xDist(rng), yDist(rng) };
		} while (TileAt(spawnPos).HasMeme());
		TileAt(spawnPos).SpawnMeme();
	}
	for (Vei2 gridPos = { 0,0 }; gridPos.y < heigth; gridPos.y++) {
		for (gridPos.x = 0; gridPos.x < width; gridPos.x++) {
			TileAt(gridPos).SetNeighbourMemeCount(CountNeighbourMemes(gridPos));
		}
	}
}

void MemeField::Draw(Graphics & gfx) const {
	gfx.DrawRect(GetRect(), SpriteCodex::baseColor);
	for (Vei2 gridPos = { 0,0 }; gridPos.y < heigth; gridPos.y++) {
		for (gridPos.x = 0; gridPos.x < width; gridPos.x++) {
			TileAt(gridPos).Draw( topLeft + gridPos * SpriteCodex::tileSize, isExploded, gfx);
		}
	}
}

RectI MemeField::GetRect() const {
	return RectI(topLeft, width * SpriteCodex::tileSize, heigth * SpriteCodex::tileSize);
}

void MemeField::OnRevealClick(const Vei2 & screePos) {
	if (isExploded) return;
	const Vei2 gridPos = ScreenToGrid(screePos);
	assert(gridPos.x >= 0 && gridPos.x < width && gridPos.y >= 0 && gridPos.y < heigth);
	Tile& tile = TileAt(gridPos);
	if (!tile.IsRevealed() && !tile.IsFlagged()) {
		tile.Reveal();
		if (tile.HasMeme()) {
			isExploded = true;
		}
	}
}

void MemeField::OnFlagClick(const Vei2 & screePos) {
	if (isExploded) return;
	const Vei2 gridPos = ScreenToGrid(screePos);
	assert(gridPos.x >= 0 && gridPos.x < width && gridPos.y >= 0 && gridPos.y < heigth);
	Tile& tile = TileAt(gridPos);
	if (!tile.IsRevealed()) {
		tile.ToggleFlag();
	}
}

MemeField::Tile & MemeField::TileAt(const Vei2 & gridPos) {
	return field[gridPos.y * width + gridPos.x];
}

const MemeField::Tile & MemeField::TileAt(const Vei2 & gridPos) const {
	return field[gridPos.y * width + gridPos.x];
}

Vei2 MemeField::ScreenToGrid(const Vei2 & screenPos) {
	return (screenPos - topLeft) / SpriteCodex::tileSize;
}

int MemeField::CountNeighbourMemes(const Vei2 & gridPos) {
	const int xStart = std::max(0, gridPos.x - 1);
	const int yStart = std::max(0, gridPos.y - 1);
	const int xEnd = std::min(width - 1, gridPos.x + 1);
	const int yEnd = std::min(heigth - 1, gridPos.y + 1);

	int count = 0;
	for (Vei2 gridPos = { xStart, yStart }; gridPos.y <= yEnd; gridPos.y++) {
		for (gridPos.x = xStart; gridPos.x <= xEnd; gridPos.x++) {
			if (TileAt(gridPos).HasMeme()) {
				count++;
			}
		}
	}
	return count;
}
