#include "stage_changes.hpp"
#include "mechanics/magneticimpulse.hpp"
#include "riders/gamemode.hpp"
#include "riders/object.hpp"
#include "riders/player.hpp"
#include "riders/stage.hpp"
#include "lib/stdlib.hpp"

// this structure for object1 will be specifically used for this code
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct TornadoObject {
	f32 origin_x;
	f32 origin_y;
	f32 origin_z;
	struct unknown *unknown;
	f32 current_x;
	f32 current_y;
	f32 current_z;
	fillerData<0x20> filler;
	u8 playerIndex;// only for tornadoes, determines which player's tornado it is via index
	fillerData<0x3> filler2;
	u8 tornadoLevel;
	fillerData<0x8> filler3;
	u32 item_icon;
};
#pragma GCC diagnostic pop

inline void lbl_update_item(ObjectNode *object, u32 item) {
	auto *object1 = static_cast<Object1 *>(object->object);
	object->item_id = item;
	object1->item_icon = item;
}

static void func_TornadoSlingshot(ObjectNode *object) {
	auto *object1 = static_cast<TornadoObject *>(object->object);
	static std::array<u8, 4> Timers __attribute((section ("SBSS"))) = {0};

	for(auto &player : getCurrentPlayerList()) {
		if(!player.ignoreTurbulence && !player.playerType) {
			const u8 &index = player.index;
			u8 &timer = Timers[index];
			if(timer != 0) {
				timer--;
				continue;
			}
			if(index == object1->playerIndex) {
				continue;
			}
			const Vector3F delta{
					player.x - object1->current_x,
					player.y - object1->current_y,
					player.z - object1->current_z
			};
			const f32 distance = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
			constexpr std::array<f32, 3> TornadoSlingshotDistances = {
					5.0f,// level 1 tornado
					6.0f,// level 2
					14.0f// level 3
			};
			if(TornadoSlingshotDistances[object1->tornadoLevel] < distance) {
				continue;
			}
			timer = 30;// 30 frames
			const Vector3F rotation{
					player.verticalRotation,
					player.horizontalRotation,
					player.rotationRoll
			};
			const Vector3F forward = rotation.getForwardVectorForRidersRotation();
			const Vector3F directionToTornado = delta.normalized();
			const f32 angle = forward.calculateAngle(directionToTornado);
			// angle at which you're "looking" at the tornado and won't slingshot
			if(angle < 0.7f) {
				continue;
			}
			if(player.state == Stun) {
				continue;
			}
			player.speed += pSpeed(60) + MI::scaleUsingCurrentMI(player, pSpeed(60));

			// set recipro extends, both the vanilla one and the special one
			player.specialReciproExtend = true;
			player.reciproExtendTimer = 0;

			auto& camera = playerCameraStruct[index];
			camera.cameraPreset = 130;// boost state camera preset
			camera.cameraPresetProperty = 0x0;

			player.dreamTrail_timer = 30;
			PlayAudioFromDAT(Sound::SFX::TornadoSlingshot);
		}
	}
}

ASMUsed void func_StageChanges(ObjectNode *currentObject) {
	if(!isInGame()) { return; }
	if(currentObject == nullptr) { return; }
	if(currentObject->object == nullptr) { return; }

	auto *object1 = static_cast<Object1 *>(currentObject->object);

	const u16 &object_type = currentObject->object_type;
	if(object_type == 0x1E || object_type == 0x1F) {
		func_TornadoSlingshot(currentObject);
	}

	const u16 &object_id = currentObject->object_id;
	switch(CurrentStage) {
		case GreenCave: {
			if(object_id == 0xE8 && object_type == FlightRing) {
				object1->rotation.x = static_cast<s32>(0xFFFFF06D);
			} else if(object_type == ItemBox) {
				switch(object_id) {
					case 0xBB:
						lbl_update_item(currentObject, HundredAir);
						break;
					case 0xBC:
					case 0xBD:
						lbl_update_item(currentObject, SpeedShoes);
						break;
					default:
						break;
				}
			}
			break;
		}
		case WhiteCave: {
			if(object_id == 0xC4 && object_type == WhiteCaveWeb) {
				if(currentObject->state != 1) {
					currentObject->state = 1;
				}
			} else if(object_id >= 0x1C && object_id <= 0x1E && object_type == DashPanel) {
				const u32 index = object_id - 0x1C;// generate index
				auto *obj1 = static_cast<Object1 *>(currentObject->object);
				auto& [pos, rot] = data_stageChanges.whiteCaveDashPanels[index]; // https://en.cppreference.com/w/cpp/language/structured_binding
				obj1->pos = pos;
				obj1->rotation = rot;
			}
			break;
		}
		case EggFactory: {
			if(object_id >= 0x104 && object_id <= 0x106 && object_type == DashPanel) {
				const u32 index = object_id - 0x104;// generate index
				auto *obj1 = static_cast<Object1 *>(currentObject->object);
				obj1->pos.z = data_stageChanges.eggFactoryDashPanelZPos[index];
				obj1->pos.x = data_stageChanges.eggFactoryDashPanelXPos;
			} else if(object_id >= 0x125 && object_id <= 0x126 && object_type == FlightRing) {
				const u32 index = object_id - 0x125;
				auto *obj1 = static_cast<Object1 *>(currentObject->object);
				auto& [pos, rot] = data_stageChanges.eggFactoryFlyRings[index];
				obj1->pos = pos;
				obj1->rotation = rot;
			}
			break;
		}

		case RedCanyon: {
			if(object_id >= 0xA4 && object_id <= 0xA7 && object_type == DashPanel) {
				const u32 index = 0xA7 - object_id;
				auto *obj1 = static_cast<Object1 *>(currentObject->object);
				auto& [x, z] = data_stageChanges.redCanyonDashPanelXZPositions[index];
				obj1->pos.x = x;
				obj1->pos.y = data_stageChanges.redCanyonDashPanelYPos;
				obj1->pos.z = z;

				obj1->rotation.y = static_cast<s32>(0xFFFFF5A0);
			}
			break;
		}
		default:
			break;
	}

	if(object_type != ItemBox) { return; }

	switch(CurrentStage) {
		case MetalCity: {
			if(object_id == 0x2A) {
				lbl_update_item(currentObject, TwentyRings);
			}
			break;
		}
		case NightChase: {
			if(object_id >= 0x67 && object_id <= 0x6E) {
				lbl_update_item(currentObject, TenRings);
			} else if(object_id >= 0xFC && object_id <= 0x100) {
				lbl_update_item(currentObject, RNG);
			}
			break;
		}
		case SegaIllusion: {
			switch(object_id) {
				case 0xDD:
					lbl_update_item(currentObject, FiftyAir);
					break;
				case 0xDC:
					lbl_update_item(currentObject, ThirtyAir);
					break;
				default:
					break;
			}
			if(object_id >= 0xB6 && object_id <= 0xB9) {
				lbl_update_item(currentObject, TenRings);
			} else if(object_id >= 0xBA && object_id <= 0xBD) {
				lbl_update_item(currentObject, FiftyAir);
			}
			break;
		}
		case SandRuins: {
			if(object_id >= 0x1A1 && object_id <= 0x1A4) {
				lbl_update_item(currentObject, TenRings);
			}
			switch(object_id) {
				case 0x189:
					lbl_update_item(currentObject, TwentyRings);
					break;
				case 0x18A:
					lbl_update_item(currentObject, TenRings);
					break;
				case 0x18C:
					lbl_update_item(currentObject, FiftyAir);
					break;
				case 0x18D:
					lbl_update_item(currentObject, FiftyAir);
					break;
				case 0x18E:
					lbl_update_item(currentObject, HundredAir);
					break;
				default:
					break;
			}
			break;
		}
		// case SplashCanyon: {
		// 	if(object_id == 0xCC) {
		// 		lbl_update_item(currentObject, ThirtyRings);
		// 	}
		// 	break;
		// }
		case DigitalDimension: {
			if(object_id >= 0xC && object_id <= 0xF) {
				object1->pos.z = data_stageChanges.digitalDimensionFirstRowZPos;
			} else if(object_id >= 0x10 && object_id <= 0x13) {
				lbl_update_item(currentObject, TenRings);
				const u32 index = object_id - 0x10;
				auto *obj1 = static_cast<Object1 *>(currentObject->object);
				auto &itemPos = data_stageChanges.digitalDimensionItemBoxPositions[index];
				obj1->pos = itemPos;
			}
			break;
		}
		case SkyRoad: {
			if(object_id >= 0xD8 && object_id <= 0xDF) {
				lbl_update_item(currentObject, TenRings);
			}
			if (object_id >= 0xE0 && object_id <= 0xE7) {
				lbl_update_item(currentObject, TenRings);
			}
			break;
		}
		default:
			break;
	}

	if (currentObject->item_id == RNG && CurrentGameMode != TimeTrial) {
		if (lbl_RNG_Number(2) == 0) {
			lbl_update_item(currentObject, TenRings);
		}
		else {
			lbl_update_item(currentObject, ThirtyAir);
		}
	}
}