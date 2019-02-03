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
			if (minimum_size() != point{ 0, 0 })
			{
				info->ptMinTrackSize.x = static_cast<LONG>(minimum_size().x);
				info->ptMinTrackSize.y = static_cast<LONG>(minimum_size().y);
			}
			if (maximum_size() != point{ 0, 0 })
			{
				info->ptMaxTrackSize.x = static_cast<LONG>(maximum_size().x);
				info->ptMaxTrackSize.y = static_cast<LONG>(maximum_size().y);
			}
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

	point form::maximum_size() const noexcept
	{
		return maximum_size_;
	}
	void form::maximum_size(const point& new_maximum_size) noexcept
	{
		maximum_size_ = new_maximum_size;
	}
	void form::maximum_size(std::int32_t w, std::int32_t h) noexcept
	{
		maximum_size_ = { w, h };
	}
	point form::minimum_size() const noexcept
	{
		return minimum_size_;
	}
	void form::minimum_size(const point& new_minimum_size) noexcept
	{
		minimum_size_ = new_minimum_size;
	}
	void form::minimum_size(std::int32_t w, std::int32_t h) noexcept
	{
		minimum_size_ = { w, h };
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