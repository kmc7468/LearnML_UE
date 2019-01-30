#include <lml_ue/engine.hpp>

#include <lml_ue/control.hpp>
#include <lml_ue/errorcode.hpp>
#include <lml_ue/form.hpp>

namespace lml_ue
{
	HINSTANCE instance = nullptr;

	std::uint32_t initialize(HINSTANCE instance) noexcept
	{
		lml_ue::instance = instance;

		static WNDCLASS form;

		form.cbClsExtra = 0;
		form.cbWndExtra = 0;
		form.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
		form.hCursor = LoadCursor(nullptr, IDC_ARROW);
		form.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		form.hInstance = instance;
		form.lpfnWndProc = form::wnd_proc_;
		form.lpszClassName = TEXT("form");
		form.lpszMenuName = nullptr;
		form.style = CS_HREDRAW | CS_VREDRAW;

		if (RegisterClass(&form) == 0) return LML_UE_ERRORCODE_FAILED_TO_REGISTER_FORM;

		static WNDCLASS control;

		control.cbClsExtra = 0;
		control.cbWndExtra = 0;
		control.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
		control.hCursor = LoadCursor(nullptr, IDC_ARROW);
		control.hIcon = nullptr;
		control.hInstance = nullptr;
		control.lpfnWndProc = control::wnd_proc_;
		control.lpszClassName = TEXT("control");
		control.lpszMenuName = nullptr;
		control.style = CS_HREDRAW | CS_VREDRAW;

		if (RegisterClass(&control) == 0) return LML_UE_ERRORCODE_FAILED_TO_REGISTER_CONTROL;

		return 0;
	}
}