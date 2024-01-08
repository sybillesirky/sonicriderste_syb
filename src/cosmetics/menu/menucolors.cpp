#include "menucolors.hpp"
#include "lib/stdlib.hpp"

constexpr std::array<RGBA, 7> CustomMenuColors = {{
    {0x3a, 0x00, 0x63},	// Deep violet
    {0x00, 0x3f, 0x63},	// Dark Teal
    {0x4a, 0x4a, 0x4a},	// Grey
    {0x84, 0xff, 0x00},	// Neon Green
    {0x7a, 0x5e, 0x00},	// Dark Gold
    {0xf7, 0xe7, 0xd1},	// Pumpkin Spice
    {0xFF, 0xFF, 0xFF},	// Pure White
}};

/**
 * Handles setting a custom main menu color.
 *
 * @param graphicalObject The main menu's graphical object.
 * @param object The main menu task's object data.
 */
ASMUsed void RandomizeCustomMenuColors(GraphicalObject *graphicalObject, MainMenuObject1 *object) {
    // originally coded by joe a tron as gecko code, ported to C++
	constexpr auto vanillaColorCount = 6;
	constexpr auto customColorCount = CustomMenuColors.size();
    //const u32 random = lbl_RNG_Number(customColorCount + vanillaColorCount); // +6 to account for vanilla colors
    const u32 random = lbl_RNG_Number(customColorCount);
    // if (random >= vanillaColorCount) {
    //     object->colorPreset = 0x3; // set it to gray to tint it better
    //     graphicalObject->tint = CustomMenuColors[random - vanillaColorCount];
    // }
    object->colorPreset = 0x3;
    graphicalObject->tint = CustomMenuColors[random];
}