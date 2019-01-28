#pragma once

#include <memory>
#include <vector>
#include <Windows.h>

namespace lml_ue
{
	class control
	{
	public:
		control(const control&) = delete;
		virtual ~control();

	protected:
		control();
		control(HWND handle) noexcept;

	public:
		control& operator=(const control&) = delete;

	public:
		void show();
		void show(int show);
		void hide();

		void add_control(const std::shared_ptr<control>& control);
		void remove_control(const std::shared_ptr<control>& control);

	private:
		HWND handle_;

		std::vector<std::shared_ptr<control>> controls_;
	};

	using control_ptr = std::shared_ptr<control>;
}