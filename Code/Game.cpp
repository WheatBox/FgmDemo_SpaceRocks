#include <FxxkGML.h>

#include "Others.h"

fgm::asset obj_FgmObj;

void fgm_init() {
	fgm::randomize();

	obj_FgmObj = fgm::asset_get_index("obj_FgmObj");
	
	fgm::instance ins_Player(gRoomWidth / 2, gRoomHeight / 2, -1000, obj_FgmObj);
	fgm::FxxkGML_Init(ins_Player.get_id(), "Player/Player.dll");
}

int spawnCdMax = 30, spawnCd = 0;
bool spawnFirst = true;

void fgm_main() {
	if(spawnCd > 0) {
		spawnCd--;
	} else {
		spawnCd = spawnCdMax;

		fgm::vec2 spawnPos;
		if(spawnFirst) {
			spawnFirst = false;
			spawnPos = fgm::vec2(gRoomWidth / 2, gRoomHeight / 2);
		} else {
			spawnPos = fgm::vec2(fgm::irandom_range(128, gRoomWidth - 128), fgm::irandom_range(128, gRoomHeight - 128));
		}

		int spawnDir = fgm::irandom(359);

		fgm::rect roomArea(0, 0, gRoomWidth, gRoomHeight);
		fgm::rect spawnHitbox(-48, -48, 48, 48);
		fgm::vec2 spawnCheckAdd = fgm::vec2(-100, 0).rotate(spawnDir);
		while(roomArea.collision(spawnHitbox + spawnPos)) {
			spawnPos += spawnCheckAdd;
		}

		fgm::instance ins_Rock(spawnPos, -1000, obj_FgmObj);
		fgm::FxxkGML_Init(ins_Rock.get_id(), "Enemies/Rock.dll");

		fgm::variable_instance_set_real(ins_Rock.get_id(), "dir", spawnDir);
	}
}

void fgm_assistant(int assistant_index) {
	
}
