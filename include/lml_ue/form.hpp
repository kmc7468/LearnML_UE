#pragma once

#include <lml_ue/control.hpp>
#include <lml_ue/point.hpp>

#include <cstdint>
#include <memory>
#include <Windows.h>

namespace lml_ue
{
	class form : public control
	{
		friend std::uint32_t initialize(HINSTANCE instance) noexcept;

	public:
		form(const form&) = delete;
		virtual ~form() override = default;

	protected:
		form();

	public:
		form& operator=(const form&) = delete;

	protected:
		virtual LRESULT CALLBACK wnd_proc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam);

	private:
		HWND initialize_form_();

	public:
		point maximum_size;
		point minimum_size;

	private:
		static LRESULT CALLBACK wnd_proc_(HWND handle, UINT message, WPARAM wparam, LPARAM lparam);
	};

	using form_ptr = std::shared_ptr<form>;

	class main_form final : public form
	{
	public:
		main_form() = default;
		main_form(const main_form&) = delete;
		virtual ~main_form() = default;

	public:
		main_form& operator=(const main_form&) = delete;

	protected:
		virtual LRESULT CALLBACK wnd_proc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam) override;
	};
}