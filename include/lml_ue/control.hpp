#pragma once

#include <lml_ue/event.hpp>
#include <lml_ue/details/index_generator.hpp>

#include <cstdint>
#include <map>
#include <memory>
#include <Windows.h>

namespace lml_ue
{
	class control
	{
		friend std::uint32_t initialize(HINSTANCE instance) noexcept;

	public:
		control() = default;
		control(const control&) = delete;
		virtual ~control();

	protected:
		control(HWND handle) noexcept;

	public:
		control& operator=(const control&) = delete;

	public:
		bool created() const noexcept;

		void show();
		void show(int show);
		void hide();

		std::uint64_t add_control(const std::shared_ptr<control>& control);
		void remove_control(const std::shared_ptr<control>& control);
		std::shared_ptr<control> get_control(std::uint64_t index);

	protected:
		virtual LRESULT CALLBACK wnd_proc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam);

	private:
		HWND initialize_control_();

	private:
		void set_parent_(HWND new_parent) noexcept;
		void set_id_(HMENU new_id) noexcept;

	public:
		event<void()> clicked;
	
	private:
		HWND handle_ = nullptr;
		HWND parent_ = nullptr;
		HMENU id_ = nullptr;

		bool mouse_downed_ = false;

		std::map<std::uint64_t, std::shared_ptr<control>> controls_;
		details::index_generator controls_ig_;

	private:
		static LRESULT CALLBACK wnd_proc_(HWND handle, UINT message, WPARAM wparam, LPARAM lparam);
	};

	using control_ptr = std::shared_ptr<control>;
}