#pragma once

#include "RectI.h"
#include "SpriteCodex.h"
#include "Mouse.h"
#include "Sound.h"

class SelectionMenu {
public:
	enum class Size {
		Small,
		Medium,
		Large,
		Count,
		Invalid
	};
private:
	class Entry {
	public:
		Entry() = default;
		Entry(Size s, const Vei2& pos)
			:
			s(s),
			rect(RectI::FromCenter(pos, SpriteCodex::sizeselWidth / 2, SpriteCodex::sizeselHeight / 2)) {
		}
		void Draw(Graphics& gfx) const;
		bool IsHit(const Vei2& pt) const;
		void ResetHighlight();
		void SetHighlight();
		bool IsHighlighted() const;
		Size GetSize() const;
	private:
		static void DrawSizeText(Size s, const Vei2& pos, Graphics& gfx);
	private:
		static constexpr int highlightThickness = 6;
		static constexpr Color highlightColor = Colors::Yellow;
		bool highlighted = false;
		Size s;
		RectI rect;
	};
public:
	SelectionMenu(const Vei2& pos) {
		auto center = pos;
		for (int i = 0; i < int(Size::Count); i++) {
			entries[i] = { Size(i),center };
			center.y += verticalSpacing;
		}
	}
	// returns Size::Something when an entry is clicked, otherwise returns Size::Invalid
	Size ProcessMouse(const Mouse::Event& e);
	void Draw(Graphics& gfx) const;
private:
	void ResetHighlights();
private:
	static constexpr int verticalSpacing = SpriteCodex::sizeselHeight * 2;
	Sound hover = { L"menu_boop.wav" };
	Entry entries[int(Size::Count)];
};