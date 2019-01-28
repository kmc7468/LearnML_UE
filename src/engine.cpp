#include <lml_ue/engine.hpp>

#include <lml_ue/errorcode.hpp>
#include <lml_ue/form.hpp>

namespace lml_ue
{
	HINSTANCE instance = nullptr;

	std::uint32_t initialize(HINSTANCE instance) noexcept
	{
		lml_ue::instance = instance;

		static WNDCLASS wndclass;
		ZeroMemory(&wndclass, sizeof(wndclass));

		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
		wndclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		wndclass.hInstance = instance;
		wndclass.lpfnWndProc = form::wnd_proc_;
		wndclass.lpszClassName = TEXT("form");
		wndclass.lpszMenuName = nullptr;
		wndclass.style = CS_HREDRAW | CS_VREDRAW;

		if (RegisterClass(&wndclass) == 0) return LML_UE_ERRORCODE_FAILED_TO_REGISTER_FORM;

		return 0;
	}
}