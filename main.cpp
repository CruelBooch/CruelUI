#include "CruelUI.h"

//int WinMain(HINSTANCE , HINSTANCE , LPSTR, int)
int main()
{
	Crui::Window window(L"CruelUIWindow", 800, 600, SW_SHOW);
	window.Create();

	Crui::RECTF rect = { 10, 10, 100, 40 };

	Crui::SetColorLight();

	auto btn = window.CreateWidget<Crui::Button>(
		L"BUTTON",
		rect,
		5.0f,
		[]() {
			static int count = 0;
			count++;
			Crui::Log(L"Button Clicked " + std::to_wstring(count) + L" times", LINFO);
		}
	);

	std::cin.get();
	Crui::ReleaseUIResource();
}