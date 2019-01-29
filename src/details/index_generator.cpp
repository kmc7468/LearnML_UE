#include <lml_ue/details/index_generator.hpp>

namespace lml_ue::details
{
	index_generator::index_generator(const index_generator& generator)
		: max_(generator.max_), unallocated_(generator.unallocated_)
	{}

	index_generator& index_generator::operator=(const index_generator& generator)
	{
		max_ = generator.max_;
		unallocated_ = generator.unallocated_;

		return *this;
	}
	std::uint64_t index_generator::operator()()
	{
		return generate();
	}

	std::uint64_t index_generator::generate()
	{
		if (unallocated_.size() != 0)
		{
			const std::uint64_t result = unallocated_.front();
			return unallocated_.pop(), result;
		}
		else
		{
			return max_++;
		}
	}
	void index_generator::unallocate(std::uint64_t index)
	{
		unallocated_.push(index);
	}
}