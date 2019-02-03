#include <lml_ue/control.hpp>

#include <lml_ue/engine.hpp>
#include <lml_ue/errorcode.hpp>
#include <lml_ue/form.hpp>

#include <algorithm>
#include <cassert>

namespace lml_ue
{
	control::~control()
	{
		if (created())
		{
			DestroyWindow(handle_);
			handle_ = nullptr;
		}
	}

	control::control(HWND handle) noexcept
		: handle_(handle)
	{}

	bool control::created() const noexcept
	{
		return handle_ != nullptr;
	}

	void control::show()
	{
		show(SW_SHOW);
	}
	void control::show(int show)
	{
		assert(created());

		ShowWindow(handle_, show);
	}
	void control::hide()
	{
		show(SW_HIDE);
	}

	std::uint64_t control::add_control(const std::shared_ptr<control>& control)
	{
		assert(this != control.get());
		assert(std::find_if(controls_.begin(), controls_.end(), [&control](const std::pair<std::uint64_t, control_ptr>& element)
		{
			return element.second == control;
		}) == controls_.end());
		assert(dynamic_cast<form*>(control.get()) == nullptr);

		const std::uint64_t index = controls_ig_();
		controls_[index] = control;

		control->set_parent_(handle_);
		control->set_id_(reinterpret_cast<HMENU>(index));

		if (created() && !control->created())
		{
			control->handle_ = control->initialize_control_();
		}

		return index;
	}
	void control::remove_control(const std::shared_ptr<control>& control)
	{
		const auto iter = std::find_if(controls_.begin(), controls_.end(), [&control](const std::pair<std::uint64_t, control_ptr>& element)
		{
			return element.second == control;
		});
		assert(iter != controls_.end());

		SetParent(iter->second->handle_, nullptr);
		SetMenu(control->handle_, nullptr);

		controls_.erase(iter);
	}
	std::shared_ptr<control> control::get_control(std::uint64_t index)
	{
		assert(controls_.find(index) != controls_.end());
		return controls_[index];
	}

	LRESULT control::wnd_proc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
	{
		switch (message)
		{
		case WM_LBUTTONDOWN:
			mouse_downed_ = true; break;

		case WM_LBUTTONUP:
			if (mouse_downed_)
			{
				mouse_downed_ = false;
				clicked();
			}
			break;

		case WM_MOUSEHOVER:
			mouse_enter();

		case WM_MOUSELEAVE:
			mouse_downed_ = false;
			mouse_leave();
			break;

		default:
			return DefWindowProc(handle, message, wparam, lparam);
		}

		return 0;
	}

	HWND control::initialize_control_()
	{
		HWND handle;
		if ((handle = CreateWindow(TEXT("control"), TEXT(""), WS_CHILD | WS_VISIBLE, 0, 0, 100, 50,
			parent_, id_, instance, nullptr)) == nullptr) throw LML_UE_ERRORCODE_FAILED_TO_CREATE_CONTROL;
		
		SetLastError(0);
		if (SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this)) == 0 && GetLastError() != 0) throw LML_UE_ERRORCODE_FAILED_TO_CREATE_CONTROL;

		for (const auto& [id, child] : controls_)
		{
			child->parent_ = handle;
			child->handle_ = child->initialize_control_();
		}

		return handle;
	}

	void control::set_parent_(HWND new_parent) noexcept
	{
		parent_ = new_parent;

		if (created())
		{
			SetParent(handle_, parent_);
		}
	}
	void control::set_id_(HMENU new_id) noexcept
	{
		id_ = new_id;

		if (created())
		{
			SetMenu(handle_, id_);
		}
	}

	LRESULT control::wnd_proc_(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
	{
		control* address = reinterpret_cast<control*>(GetWindowLongPtr(handle, GWLP_USERDATA));

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