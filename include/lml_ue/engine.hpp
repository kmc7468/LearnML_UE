#pragma once

#include <cstdint>
#include <Windows.h>

namespace lml_ue
{
	extern HINSTANCE instance;

	std::uint32_t initialize(HINSTANCE instance) noexcept;
}