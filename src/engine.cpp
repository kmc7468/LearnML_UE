#include <lml_ue/engine.hpp>

namespace lml_ue
{
	std::uint32_t initialize(HINSTANCE instance) noexcept
	{
		lml_ue::instance = instance;

		return 0;
	}
}