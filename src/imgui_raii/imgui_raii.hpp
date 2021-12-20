#ifndef IMGUI_RAII_IMGUI_RAII_HPP
#define IMGUI_RAII_IMGUI_RAII_HPP

#include <concepts>
#include <functional>

#include <imgui.h>

namespace imgui_raii::detail
{
	using EndFunc = void(*)();

	template <class TDerived, EndFunc VEnd, bool VConditionalEnd>
	class ConditionalRAIIWrapper
	{
	public:
		~ConditionalRAIIWrapper() noexcept
		{
			if constexpr (VConditionalEnd)
			{
				if (m_Result)
					std::invoke(VEnd);
			}
			else
			{
				std::invoke(VEnd);
			}
		}

		explicit operator bool() const noexcept
		{
			return m_Result;
		}

		[[nodiscard]]
		bool result() const noexcept
		{
			return m_Result;
		}

		TDerived& operator /(std::invocable<> auto func)
		{
			if (m_Result)
				std::invoke(std::ref(func));
			return cast();
		}

		ConditionalRAIIWrapper(const ConditionalRAIIWrapper&) = delete;
		ConditionalRAIIWrapper& operator =(const ConditionalRAIIWrapper&) = delete;
		ConditionalRAIIWrapper(ConditionalRAIIWrapper&&) = delete;
		ConditionalRAIIWrapper& operator =(ConditionalRAIIWrapper&&) = delete;

	protected:
		explicit ConditionalRAIIWrapper(bool result) noexcept :
			m_Result{ result }
		{
		}

	private:
		const bool m_Result;

		[[nodiscard]]
		TDerived& cast() noexcept
		{
			static_assert(std::derived_from<TDerived, ConditionalRAIIWrapper>, "TDerived must be a subclass if ConditionalRAIIWrapper");
			return static_cast<TDerived&>(*this);
		}
	};
}

namespace imgui_raii
{
	class Begin :
		public detail::ConditionalRAIIWrapper<Begin, &ImGui::End, false>
	{
		using super = ConditionalRAIIWrapper<Begin, &ImGui::End, false>;

	public:
		template <class... TArgs>
			requires requires
			{
				{ ImGui::Begin(std::declval<TArgs>()...) } -> std::convertible_to<bool>;
			}
		explicit Begin(TArgs&&... args) :
			super{ ImGui::Begin(std::forward<TArgs>(args)...) }

		{
		}
	};
}

#endif
