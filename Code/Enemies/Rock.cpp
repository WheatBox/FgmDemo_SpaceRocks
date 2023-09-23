#include "../Others.h"

#include <FxxkGML.h>

std::unordered_map<fgm::ins_id, fgm::instance> rocks;

fgm::asset obj_FgmObj = fgm::noone;
fgm::asset spr_Rock = -1;

const double defaultSpeed = 1.2;

void SetAppeared(fgm::ins_id id, bool appeared) {
	fgm::variable_instance_set_real(id, "appeared", appeared);
}

bool GetAppeared(fgm::ins_id id) {
	return fgm::variable_instance_get_real(id, "appeared");
}

int GetDir(fgm::ins_id id) {
	return fgm::variable_instance_get_real(id, "dir");
}

void fgm_init() {

	// 初次创建岩石时调用，后来创建的岩石不需要调用
	// Called when creating rock for the first time, but later rocks do not need to be called
	if(obj_FgmObj == fgm::noone) {
		obj_FgmObj = fgm::asset_get_index("obj_FgmObj");

		spr_Rock = fgm::sprite_add("Enemies/Rock.png", 1, false, false, 32, 32);
		fgm::sprite_collision_mask(spr_Rock, false, fgm::bboxmode_automatic, 0, 0, 0, 0, fgm::bboxkind_precise, 127);
	}

	fgm::instance me = { fgm::self };
	me.set_sprite(spr_Rock);

	double scale = fgm::random_range(0.6, 1.4);
	me.set_image_scale(scale, scale);

	rocks[me.get_id()] = me;

	fgm::variable_instance_set_real(me.get_id(), "isEnemy", true);
}

void fgm_main() {
	fgm::ins_id currentId = fgm::instance_id_get_self();
	fgm::instance & currentRock = rocks[currentId];

	if(!fgm::rect(0, 0, gRoomWidth, gRoomHeight).collision(currentRock.get_bbox())) {
		if(GetAppeared(currentId)) {
			currentRock.destroy();
			return;
		}
	} else {
		SetAppeared(currentId, true);
	}

	int dir = GetDir(currentId);

	fgm::vec2 vMove = fgm::vec2(1, 0).rotate(dir);
	currentRock.move(vMove * (defaultSpeed / currentRock.get_image_xscale()));

	currentRock.set_image_angle(currentRock.get_image_angle() + ((dir % 2) * 2 - 1) * 2 / currentRock.get_image_xscale());
}

void fgm_assistant(int assistant_index) {
	switch(assistant_index) {
		case EAssistantIndex::DrawEvent:
			fgm::draw_self();
			break;

		case EAssistantIndex::DestroyEvent:
		case EAssistantIndex::CleanUpEvent:
			rocks.erase(fgm::instance_id_get_self());
			break;
	}
}