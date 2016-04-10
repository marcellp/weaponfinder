#pragma once

typedef struct weapon_node weapon_node;
struct weapon_node
{
	int modelid;
	const char *weapon;
	CVector pos;
	unsigned char rgb[3];
	weapon_node *next;
};

void change_font_size(int val);
bool CALLBACK draw_text(CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride, CONST RGNDATA *pDirtyRegion);