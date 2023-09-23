#include "../Others.h"

#include <FxxkGML.h>
#include <sstream> // For std::ostringstream::precision()

fgm::instance me;
fgm::asset obj_FgmObj;
fgm::asset spr_Player, spr_PlayerMask;

fgm::vec2 currentSpeed(0, 0);
const double maxSpeed = 7;

int shotCdMax = 10, shotCd = 0;

int score = 0;

void fgm_init() {
	obj_FgmObj = fgm::asset_get_index("obj_FgmObj");

	spr_Player = fgm::sprite_add("Player/Player.png", 1, false, false, 32, 32);
	spr_PlayerMask = fgm::sprite_add("Player/PlayerMask.png", 1, false, false, 32, 32);
	fgm::sprite_collision_mask(spr_PlayerMask, false, fgm::bboxmode_automatic, 0, 0, 0, 0, fgm::bboxkind_precise, 0);

	me = { fgm::self };
	me.set_sprite(spr_Player);
	me.set_mask(spr_PlayerMask);
	me.set_image_scale(0.6, 0.6);
	me.set_image_angle(90);
}

void Shot();

void fgm_main() {
	score = static_cast<int>(fgm::variable_global_get_real("gScore"));

	int rotDir = fgm::keyboard_check('A') - fgm::keyboard_check('D');
	me.set_image_angle(me.get_image_angle() + rotDir * 5);

	fgm::vec2 moveDis = fgm::vec2(fgm::keyboard_check('W') - fgm::keyboard_check('S'), 0).rotate(-me.get_image_angle());
	if(moveDis.m_x != 0 || moveDis.m_y != 0) {

		currentSpeed += moveDis.normalize() * 0.05;

		if(currentSpeed.length() > maxSpeed) {
			currentSpeed = currentSpeed.normalize() * maxSpeed;
		}
	}
	me.move(currentSpeed);

	const fgm::vec2 & pos = me.get_pos();
	if(pos.m_x < 0) {
		me.set_x(pos.m_x + gRoomWidth);
	}
	if(pos.m_x > gRoomWidth - 1) {
		me.set_x(pos.m_x - gRoomWidth);
	}
	if(pos.m_y < 0) {
		me.set_y(pos.m_y + gRoomHeight);
	}
	if(pos.m_y > gRoomHeight - 1) {
		me.set_y(pos.m_y - gRoomHeight);
	}

	if(shotCd > 0) {
		shotCd--;
	} else
	if(fgm::keyboard_check(fgm::vk_space)) {
		shotCd = shotCdMax;
		Shot();
	}

	fgm::ins_id collInsId = fgm::instance_place(me.get_x(), me.get_y(), obj_FgmObj);
	if(collInsId != fgm::noone && fgm::variable_instance_get_real(collInsId, "isEnemy")) {
		fgm::show_message("YOU DIED!\nYour Score: " + std::to_string(score));
		fgm::game_restart();

		currentSpeed = fgm::vec2(0, 0);
		shotCd = 0;

		fgm::variable_global_set_real("gScore", 0);
		score = 0;

		me.destroy();
	}
}

void fgm_assistant(int assistant_index) {
	if(assistant_index == EAssistantIndex::DrawEvent) {
		fgm::draw_self();

		const fgm::vec2 & pos = me.get_pos();
		fgm::vec2 posAdd(0, 0);
		if(pos.m_x < me.get_spr_width()) {
			posAdd.m_x += gRoomWidth;
		}
		if(pos.m_x > gRoomWidth - me.get_spr_width()) {
			posAdd.m_x -= gRoomWidth;
		}
		if(pos.m_y < me.get_spr_height()) {
			posAdd.m_y += gRoomHeight;
		}
		if(pos.m_y > gRoomHeight - me.get_spr_height()) {
			posAdd.m_y -= gRoomHeight;
		}
		if(posAdd.m_x != 0 || posAdd.m_y != 0) {
			fgm::draw_sprite_ext(me.get_sprite(), me.get_image_index(), pos + posAdd, me.get_image_scale(), me.get_image_angle(), me.get_image_blend(), me.get_image_alpha());
		}

		std::ostringstream strGui;
		strGui.precision(2);
		strGui << "Current Speed: " << std::fixed << currentSpeed.length() << std::endl;
		strGui << "Score: " + std::to_string(score);
		fgm::draw_text(0, 0, strGui.str());
	}
}

void Shot() {
	fgm::instance ins_Bullet(me.get_pos(), me.get_depth() + 1, obj_FgmObj);
	fgm::FxxkGML_Init(ins_Bullet.get_id(), "Player/Bullet.dll");
	ins_Bullet.set_image_angle(me.get_image_angle());
}
