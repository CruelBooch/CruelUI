#pragma once

#include "CrWindow.h"
#include "CrWidgets.h"
#include "CrGraphics.h"


#include <d2d1.h>


namespace CruelUI {
	enum CrColor_ {
		CrColor_Text,
		CrColor_SecondaryText,
		CrColor_Button,
		CrColor_ButtonHovered,
		CrColor_ButtonActived,
		CrColor_ButtonDisabled,
		CrColor_WndBg
	};

	struct CruelUIStyle
	{
		COLORF Colors[20] = {
			COLORF(0x000000, 1.0f),
			COLORF(0x000000, 1.0f),
			COLORF(0x000000, 1.0f),
			COLORF(0x000000, 1.0f),
			COLORF(0x000000, 1.0f),
			COLORF(0x000000, 1.0f),
			COLORF(0x000000, 1.0f),
			COLORF(0x000000, 1.0f),
			COLORF(0x000000, 1.0f),
			COLORF(0x000000, 1.0f),
			COLORF(0x000000, 1.0f),
			COLORF(0x000000, 1.0f),
			COLORF(0x000000, 1.0f),
			COLORF(0x000000, 1.0f),
			COLORF(0x000000, 1.0f),
			COLORF(0x000000, 1.0f),
			COLORF(0x000000, 1.0f),
			COLORF(0x000000, 1.0f),
			COLORF(0x000000, 1.0f),
			COLORF(0x000000, 1.0f),
		};
	};

	extern CruelUIStyle Dark, Light, Theme;
}

namespace Crui = CruelUI;
