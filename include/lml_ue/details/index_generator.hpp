#pragma once

#include <cstdint>
#include <queue>
#include <type_traits>

namespace lml_ue::details
{
	class index_generator final
	{
	public:
		index_generator() noexcept = default;
		index_generator(const index_generator& generator);
		~index_generator() = default;

	public:
		index_generator& operator=(const index_generator& generator);
		std::uint64_t operator()();

	public:
		std::uint64_t generate();
		void unallocate(std::uint64_t index);

	private:
		std::uint64_t max_ = 0;

		std::queue<std::uint64_t> unallocated_;
	};
}