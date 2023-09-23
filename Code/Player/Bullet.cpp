#include "../Others.h"

#include <FxxkGML.h>

std::unordered_map<fgm::ins_id, fgm::instance> bullets;

fgm::asset obj_FgmObj = fgm::noone;
fgm::asset spr_Bullet = -1;

const double moveSpeed = 8;

void SetLife(fgm::ins_id id, int life) {
	fgm::variable_instance_set_real(id, "life", life);
}

int GetLife(fgm::ins_id id) {
	return fgm::variable_instance_get_real(id, "life");
}

void fgm_init() {

	// 初次创建子弹时调用，后来创建的子弹不需要调用
	// Called when creating bullet for the first time, but later bullets do not need to be called
	if(obj_FgmObj == fgm::noone) {
		obj_FgmObj = fgm::asset_get_index("obj_FgmObj");

		spr_Bullet = fgm::sprite_add("Player/Bullet.png", 1, false, false, 32, 32);
		fgm::sprite_collision_mask(spr_Bullet, false, fgm::bboxmode_manual, 16, 26, 47, 37, fgm::bboxkind_ellipse, 0);
	}

	fgm::instance me = { fgm::self };
	me.set_sprite(spr_Bullet);
	me.set_image_scale(0.6, 0.6);

	SetLife(me.get_id(), 180);

	bullets[me.get_id()] = me;
}

void fgm_main() {
	fgm::ins_id currentId = fgm::instance_id_get_self();
	fgm::instance & currentBullet = bullets[currentId];

	int life = GetLife(currentId);
	if(life <= 0) {
		currentBullet.destroy();
	} else {
		SetLife(currentId, life - 1);
		currentBullet.move(fgm::vec2(moveSpeed, 0).rotate(-currentBullet.get_image_angle(true)));
	}

	fgm::ins_id collInsId = fgm::instance_place(currentBullet.get_x(), currentBullet.get_y(), obj_FgmObj);
	if(collInsId != fgm::noone && fgm::variable_instance_get_real(collInsId, "isEnemy")) {
		fgm::variable_global_set_real("gScore", fgm::variable_global_get_real("gScore") + 1);
		fgm::instance_destroy(collInsId);
		currentBullet.destroy();
	}
}

void fgm_assistant(int assistant_index) {
	switch(assistant_index) {
		case EAssistantIndex::DrawEvent:
			fgm::draw_self();
			break;

		case EAssistantIndex::DestroyEvent:
		case EAssistantIndex::CleanUpEvent:
			bullets.erase(fgm::instance_id_get_self());
			break;
	}
}