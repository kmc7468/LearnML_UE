#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <utility>
#include <vector>

namespace lml_ue
{
	template<typename Result_, typename... Args_>
	class event_handler;

	template<typename Result_, typename... Args_>
	class event_handler<Result_(Args_...)> final
	{
	public:
		template<typename Function_>
		event_handler(Function_&& function)
			: function_(std::forward<Function_>(function))
		{
			static std::uint64_t id = 1;
			id_ = id++;
		}
		event_handler(const event_handler& handler)
			: function_(handler.function_), id_(handler.id_)
		{}
		event_handler(event_handler&& handler) noexcept
			: function_(std::move(handler.function_)), id_(handler.id_)
		{
			handler.id_ = 0;
		}
		~event_handler() = default;

	public:
		event_handler& operator=(const event_handler& handler)
		{
			function_ = handler.function_;
			id_ = handler.id_;

			return *this;
		}
		event_handler& operator=(event_handler&& handler) noexcept
		{
			function_ = std::move(handler.function_);
			id_ = handler.id_;

			handler.id_ = 0;

			return *this;
		}
		friend bool operator==(const event_handler& a, const event_handler& b) noexcept
		{
			return a.id_ == b.id_;
		}
		friend bool operator!=(const event_handler& a, const event_handler& b) noexcept
		{
			return a.id_ != b.id_;
		}
		template<typename... Args2_>
		Result_ operator()(Args2_&&... args) const
		{
			return function_(std::forward<Args2_>(args)...);
		}

	private:
		std::function<Result_(Args_...)> function_;
		std::uint64_t id_;
	};

	template<typename... Args_>
	class event_handler<void(Args_...)> final
	{
	public:
		template<typename Function_>
		event_handler(Function_&& function)
			: function_(std::forward<Function_>(function))
		{
			static std::uint64_t id = 1;
			id_ = id++;
		}
		event_handler(const event_handler& handler)
			: function_(handler.function_), id_(handler.id_)
		{}
		event_handler(event_handler&& handler) noexcept
			: function_(std::move(handler.function_)), id_(handler.id_)
		{
			handler.id_ = 0;
		}
		~event_handler() = default;

	public:
		event_handler& operator=(const event_handler& handler)
		{
			function_ = handler.function_;
			id_ = handler.id_;

			return *this;
		}
		event_handler& operator=(event_handler&& handler) noexcept
		{
			function_ = std::move(handler.function_);
			id_ = handler.id_;

			handler.id_ = 0;

			return *this;
		}
		friend bool operator==(const event_handler& a, const event_handler& b) noexcept
		{
			return a.id_ == b.id_;
		}
		friend bool operator!=(const event_handler& a, const event_handler& b) noexcept
		{
			return a.id_ != b.id_;
		}
		template<typename... Args2_>
		void operator()(Args2_&&... args) const
		{
			function_(std::forward<Args2_>(args)...);
		}

	private:
		std::function<void(Args_...)> function_;
		std::uint64_t id_;
	};

	template<typename Result_, typename... Args_>
	class event;

	template<typename Result_, typename... Args_>
	class event<Result_(Args_...)> final
	{
	public:
		using handler = event_handler<Result_(Args_...)>;
		
	public:
		event() noexcept = default;
		event(const event&) = delete;
		~event() = default;

	public:
		event& operator=(const event&) = delete;
		template<typename... Args2_>
		std::vector<Result_> operator()(Args2_&&... args) const
		{
			std::vector<Result_> result(handlers_.size());

			for (std::size_t i = 0; i < result.size(); ++i)
			{
				result[i] = handlers_[i](std::forward<Args2_>(args)...);
			}
			
			return result;
		}
		event& operator+=(const handler& handler)
		{
			handlers_.push_back(handler);
			return *this;
		}
		event& operator+=(handler&& handler)
		{
			handlers_.push_back(std::move(handler));
			return *this;
		}
		event& operator-=(const handler& handler)
		{
			handlers_.erase(std::find(handlers_.begin(), handlers_.end(), handler));
			return *this;
		}

	private:
		std::vector<handler> handlers_;
	};

	template<typename... Args_>
	class event<void(Args_...)> final
	{
	public:
		using handler = event_handler<void(Args_...)>;

	public:
		event() noexcept = default;
		event(const event&) = delete;
		~event() = default;

	public:
		event& operator=(const event&) = delete;
		template<typename... Args2_>
		void operator()(Args2_&&... args) const
		{
			for (const handler& h : handlers_)
			{
				h(std::forward<Args2_>(args)...);
			}
		}
		event& operator+=(const handler& handler)
		{
			handlers_.push_back(handler);
			return *this;
		}
		event& operator+=(handler&& handler)
		{
			handlers_.push_back(std::move(handler));
			return *this;
		}
		event& operator-=(const handler& handler)
		{
			handlers_.erase(std::find(handlers_.begin(), handlers_.end(), handler));
			return *this;
		}

	private:
		std::vector<handler> handlers_;
	};
}