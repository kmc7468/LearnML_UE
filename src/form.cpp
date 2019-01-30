#include <lml_ue/form.hpp>

#include <lml_ue/engine.hpp>
#include <lml_ue/errorcode.hpp>

namespace lml_ue
{
	form::form()
		: control(initialize_form_())
	{}

	LRESULT form::wnd_proc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
	{
		switch (message)
		{
		case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO info = reinterpret_cast<LPMINMAXINFO>(lparam);
			if (minimum_size != point{ 0, 0 })
			{
				info->ptMinTrackSize.x = static_cast<LONG>(minimum_size.x);
				info->ptMinTrackSize.y = static_cast<LONG>(minimum_size.y);
			}
			if (maximum_size != point{ 0, 0 })
			{
				info->ptMaxTrackSize.x = static_cast<LONG>(maximum_size.x);
				info->ptMaxTrackSize.y = static_cast<LONG>(maximum_size.y);
			}
			return 0;
		}

		case WM_COMMAND:
		{
			const std::uint64_t index = static_cast<std::uint64_t>(LOWORD(wparam));
			get_control(index)->clicked();
			return 0;
		}

		default:
			return control::wnd_proc(handle, message, wparam, lparam);
		}
	}

	HWND form::initialize_form_()
	{
		HWND handle;
		if ((handle = CreateWindow(TEXT("form"), TEXT(""), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, 980, 680,
			nullptr, nullptr, instance, nullptr)) == nullptr) throw LML_UE_ERRORCODE_FAILED_TO_CREATE_FORM;

		SetLastError(0);
		if (SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this)) == 0 && GetLastError() != 0) throw LML_UE_ERRORCODE_FAILED_TO_CREATE_FORM;

		return handle;
	}

	LRESULT form::wnd_proc_(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
	{
		form* address = reinterpret_cast<form*>(GetWindowLongPtr(handle, GWLP_USERDATA));

		if (address)
		{
			return address->wnd_proc(handle, message, wparam, lparam);
		}
		else
		{
			return DefWindowProc(handle, message, wparam, lparam);
		}
	}
}

namespace lml_ue
{
	LRESULT main_form::wnd_proc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
	{
		switch (message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		default:
			return form::wnd_proc(handle, message, wparam, lparam);
		}
	}
}