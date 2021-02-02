#pragma once

#include <vector>
#include <functional>

namespace lev
{
	template <typename T, typename ...TArgs>
	auto MakeListener(T& obj, void(T::* function)(TArgs...))
	{
		return [&obj, function](TArgs... args) { (obj.*function)(args...); };
	}

	template <typename ...TArgs>
	class EventHandler
	{
	private:

		std::vector<std::function<void(TArgs...)>> listeners;

	public:

		EventHandler() = default;

		EventHandler& operator+=(std::function<void(TArgs...)> function)
		{
			listeners.push_back(function);
			return *this;
		}

		void operator()(TArgs&&... args) const
		{
			for (auto& function : listeners)
			{
				function(std::forward<TArgs>(args)...);
			}
		}
	};
}