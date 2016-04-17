#include "weaponfinder.hpp"

const char *weapon_names[] = { "Nitestick", "Knife", "Bat", "Shovel", "Pool cue", "Katana", "",
								"Chainsaw", "Grenade", "Tear gas", "Molotov", "", "9mm", "Silenced", "Deagle",
								"Shotgun", "Sawnoff", "Spaz12", "Uzi", "MP5", "", "AK47", "M4", "Rifle", "Sniper",
								"RPG", "Heat seeking RPG", "Flame-thrower", "Minigun", "Satchel charges", "Detonator",
								"Spray can", "Fire extinguisher", "Camera", "Night vision", "Thermal goggles", "Parachute",
								"", "TEC9" };

// We need this for the cool rainbow thingy we want to create.
static void hsvtorgb(unsigned char *r, unsigned char *g, unsigned char *b, unsigned char h, unsigned char s, unsigned char v)
{
	unsigned char region, fpart, p, q, t;

	if (s == 0) {
		*r = *g = *b = v;
		return;
	}

	region = h / 43;
	fpart = (h - (region * 43)) * 6;

	p = (v * (255 - s)) >> 8;
	q = (v * (255 - ((s * fpart) >> 8))) >> 8;
	t = (v * (255 - ((s * (255 - fpart)) >> 8))) >> 8;

	switch (region) {
	case 0:
		*r = v; *g = t; *b = p; break;
	case 1:
		*r = q; *g = v; *b = p; break;
	case 2:
		*r = p; *g = v; *b = t; break;
	case 3:
		*r = p; *g = q; *b = v; break;
	case 4:
		*r = t; *g = p; *b = v; break;
	default:
		*r = v; *g = p; *b = q; break;
	}

	return;
}

void change_font_size(int val)
{
	if (val < 1 || val > 30) {
		pprintf("Your font size must be between 1 and 30.");
	}
	else {
		SF->getRender()->ReleaseFont(font);
		font = SF->getRender()->CreateNewFont("Tahoma", val, FCR_BORDER);
		pprintf("Font size changed to %d.", val);
	}
}

static int find_objects(weapon_node *weps_head)
{
	stObjectPool *object_pool = SF->getSAMP()->getInfo()->pPools->pObject;
	stObject *object;
	actor_info *me = SF->getSAMP()->getPlayers()->pLocalPlayer->pSAMP_Actor->pGTA_Ped;
	CVector ppos;
	float distance;
	weapon_node *weps_actual = weps_head;
	int ret = 0;

	for (int i = 0; i < SAMP_MAX_OBJECTS; i++) {
		object = object_pool->object[i];

		if (object_pool->iIsListed[i] != 1)
			continue;
		if (object == NULL)
			continue;
		if (object->pGTAEntity == NULL)
			continue;

		if (object->iModel >= 334 && object->iModel <= 372) {
			// Let's move to the next element of the linked list if this isn't
			// the first element or if the previous iteration failed for some reason
			// (we didn't allocate anything on the heap and thus next is still NULL or
			// malloc failed which also returns NULL).
			if (ret != 0 && weps_actual->next != NULL)
				weps_actual = weps_actual->next;

			weps_actual->next = NULL;
			weps_actual->pos.fX = object->pGTAEntity->base.matrix[(4 * 3) + 0];
			weps_actual->pos.fY = object->pGTAEntity->base.matrix[(4 * 3) + 1];
			weps_actual->pos.fZ = object->pGTAEntity->base.matrix[(4 * 3) + 2];

			// Ensure that the absolute value of the vector is not zero (less than epsilon).
			if ((weps_actual->pos).Length() < 0.0001f)
				continue;

			weps_actual->weapon = weapon_names[(object->iModel) - 334];
			weps_actual->modelid = object->iModel;

			// We're using HSV (H: 0-170, SV: 255) to color the labels depending
			// on how close the objects are. We're supposed to get a neat rainbow pattern.
			// We're using 250 units as our stream bubble, everything farther from that
			// will be colored blue.
			ppos.fX = me->base.matrix[(4 * 3) + 0];
			ppos.fY = me->base.matrix[(4 * 3) + 1];
			ppos.fZ = me->base.matrix[(4 * 3) + 2];

			distance = floorf((weps_actual->pos - ppos).Length());
			distance = (distance / 250) * 170;

			if (distance > 170 || distance < 0) {
				weps_actual->rgb[0] = 0;
				weps_actual->rgb[1] = 0;
				weps_actual->rgb[2] = 255;
			}
			else {
				hsvtorgb(&weps_actual->rgb[0], &weps_actual->rgb[1], &weps_actual->rgb[2],
					(int)floor((distance + 0.5)), 255, 255);
			}

			weps_actual->next = (weapon_node*)malloc(sizeof(weapon_node));
			ret++;
		}
	}

	// If the linked list contains at least one element, there's now an uninitialized
	// chunk of memory on the heap. Free it.
	if (ret != 0)
		free(weps_actual->next);

	weps_actual->next = NULL;

	return ret;
}

bool CALLBACK draw_text(CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride, CONST RGNDATA *pDirtyRegion)
{
	weapon_node *weps_head;
	weapon_node *weps_temp;
	CVector2D pos2d;

	if (!overlay_toggle || SF->getGame()->isGTAMenuActive())
		return true;

	weps_head = (weapon_node*)malloc(sizeof(weapon_node));
	weps_temp = weps_head;

	if (SUCCEEDED(SF->getRender()->BeginRender()) && find_objects(weps_head) != 0) {
		while (weps_temp != NULL) {
			// We have to check if the object is actually on the screen.
			SF->getCLEO()->callOpcode("00C2: %f %f %f %f", weps_temp->pos.fX, weps_temp->pos.fY, weps_temp->pos.fZ, 0.0);
			if (SF->getCLEO()->GetCondResult()) {
				SF->getGame()->convert3DCoordsToScreen(weps_temp->pos.fX, weps_temp->pos.fY, weps_temp->pos.fZ,
					&pos2d.fX, &pos2d.fY);

				if (color_toggle) {
					font->Print(weps_temp->weapon,
						D3DCOLOR_ARGB(255, weps_temp->rgb[0], weps_temp->rgb[1], weps_temp->rgb[2]),
						pos2d.fX, pos2d.fY, false);
				}
				else {
					font->Print(weps_temp->weapon,
						D3DCOLOR_ARGB(255, 255, 255, 0),
						pos2d.fX, pos2d.fY, false);
				}
			}
			weps_temp = weps_temp->next;
		}
		SF->getRender()->EndRender();
	}

	while (weps_head != NULL) {
		weps_temp = weps_head;
		weps_head = weps_temp->next;
		free(weps_temp);
	}

	return true;
}