#include "SelectionMenu.h"

void SelectionMenu::Entry::Draw(Graphics & gfx) const {
	if (highlighted) {
		gfx.DrawRect(rect.GetExpanded(highlightThickness), highlightColor);
	}
	gfx.DrawRect(rect, Colors::Black);
	DrawSizeText(s, rect.GetCenter(), gfx);
}

bool SelectionMenu::Entry::IsHit(const Vei2 & pt) const {
	return rect.Contains(pt);
}

void SelectionMenu::Entry::ResetHighlight() {
	highlighted = false;
}

void SelectionMenu::Entry::SetHighlight() {
	highlighted = true;
}

bool SelectionMenu::Entry::IsHighlighted() const {
	return highlighted;
}

SelectionMenu::Size SelectionMenu::Entry::GetSize() const {
	return s;
}

void SelectionMenu::Entry::DrawSizeText(Size s, const Vei2 & pos, Graphics & gfx) {
	switch (s) {
	case Size::Small:
		SpriteCodex::DrawSmall(pos, gfx);
		break;
	case Size::Medium:
		SpriteCodex::DrawMedium(pos, gfx);
		break;
	case Size::Large:
		SpriteCodex::DrawLarge(pos, gfx);
		break;
	}
}

SelectionMenu::Size SelectionMenu::ProcessMouse(const Mouse::Event & e) {
		switch (e.GetType()) {
		case Mouse::Event::Type::Move:
			for (auto& n : entries) {
				if (n.IsHit(e.GetPos())) {
					// need to test here to prevent sfx from firing
					// on every mouse move event
					if (!n.IsHighlighted()) {
						ResetHighlights();
						n.SetHighlight();
						hover.Play();
					}
					// immediately exit if found a hit
					// (if we don't, highlight will be reset below)
					return Size::Invalid;
				}
			}
			// if no entry was hit, reset all highlights
			ResetHighlights();
			break;
		case Mouse::Event::Type::LPress:
			for (auto& n : entries) {
				if (n.IsHit(e.GetPos())) {
					return n.GetSize();
				}
			}
			break;
		}
		return Size::Invalid;
}

void SelectionMenu::Draw(Graphics & gfx) const {
	for (const auto& n : entries) {
		n.Draw(gfx);
	}
}

void SelectionMenu::ResetHighlights() {
	for (auto& n : entries) {
		n.ResetHighlight();
	}
}
