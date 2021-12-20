#ifndef IMGUI_RAII_IMGUI_RAII_HPP
#define IMGUI_RAII_IMGUI_RAII_HPP

#include <concepts>
#include <functional>

#include <imgui.h>

namespace imgui_raii::detail
{
	using EndFunc = void(*)();

	template <class TDerived, EndFunc VEnd>
	class RAIIWrapper
	{
	public:
		RAIIWrapper() noexcept = default;
		~RAIIWrapper() noexcept
		{
			std::invoke(VEnd);
		}

		TDerived& operator /(std::invocable<> auto func)
		{
			std::invoke(std::ref(func));
			return cast();
		}

		RAIIWrapper(const RAIIWrapper&) = delete;
		RAIIWrapper& operator =(const RAIIWrapper&) = delete;
		RAIIWrapper(RAIIWrapper&&) = delete;
		RAIIWrapper& operator =(RAIIWrapper&&) = delete;

	protected:
		[[nodiscard]]
		TDerived& cast() noexcept
		{
			static_assert(std::derived_from<TDerived, RAIIWrapper>, "TDerived must be a subclass if RAIIWrapper");
			return static_cast<TDerived&>(*this);
		}
	};

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
			static_assert(
				std::derived_from<TDerived, ConditionalRAIIWrapper>,
				"TDerived must be a subclass if ConditionalRAIIWrapper"
			);
			return static_cast<TDerived&>(*this);
		}
	};
}

namespace imgui_raii
{
	class Context
	{
	public:
		template <class... TArgs>
			requires requires
			{
				{ ImGui::CreateContext(std::declval<TArgs>()...) } -> std::convertible_to<ImGuiContext*>;
			}
		[[nodiscard]]
		explicit Context(TArgs&&... args) :
			m_Context{ ImGui::CreateContext(std::forward<TArgs>(args)...) }
		{
		}

		[[nodiscard]]
		Context(Context&& other) noexcept :
			m_Context{ std::exchange(other.m_Context, nullptr) }
		{
		}

		Context& operator =(Context&& other) noexcept
		{
			std::swap(m_Context, other.m_Context);
			return *this;
		}

		~Context() noexcept
		{
			if (m_Context)
				ImGui::DestroyContext(m_Context);
		}

		[[nodiscard]]
		explicit operator ImGuiContext*() const noexcept
		{
			return m_Context;
		}

		[[nodiscard]]
		ImGuiContext* context() const noexcept
		{
			return m_Context;
		}

		Context(const Context&) = delete;
		Context& operator =(const Context&) = delete;

	private:
		ImGuiContext* m_Context{};
	};

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

	class BeginChild :
		public detail::ConditionalRAIIWrapper<BeginChild, &ImGui::EndChild, false>
	{
		using super = ConditionalRAIIWrapper<BeginChild, &ImGui::EndChild, false>;

	public:
		template <class... TArgs>
			requires requires
			{
				{ ImGui::BeginChild(std::declval<TArgs>()...) } -> std::convertible_to<bool>;
			}
		explicit BeginChild(TArgs&&... args) :
			super{ ImGui::BeginChild(std::forward<TArgs>(args)...) }
		{
		}
	};

	class BeginChildFrame :
		public detail::ConditionalRAIIWrapper<BeginChildFrame, &ImGui::EndChildFrame, false>
	{
		using super = ConditionalRAIIWrapper<BeginChildFrame, &ImGui::EndChildFrame, false>;

	public:
		template <class... TArgs>
			requires requires
			{
				{ ImGui::BeginChildFrame(std::declval<TArgs>()...) } -> std::convertible_to<bool>;
			}
		explicit BeginChildFrame(TArgs&&... args) :
			super{ ImGui::BeginChildFrame(std::forward<TArgs>(args)...) }
		{
		}
	};

	class BeginCombo :
		public detail::ConditionalRAIIWrapper<BeginChildFrame, &ImGui::EndCombo, true>
	{
		using super = ConditionalRAIIWrapper<BeginChildFrame, &ImGui::EndCombo, true>;

	public:
		template <class... TArgs>
			requires requires
			{
				{ ImGui::BeginCombo(std::declval<TArgs>()...) } -> std::convertible_to<bool>;
			}
		explicit BeginCombo(TArgs&&... args) :
			super{ ImGui::BeginCombo(std::forward<TArgs>(args)...) }
		{
		}
	};

	class BeginDisabled :
		public detail::RAIIWrapper<BeginDisabled, &ImGui::EndDisabled>
	{
	public:
		template <class... TArgs>
			requires requires
			{
				{ ImGui::BeginDisabled(std::declval<TArgs>()...) };
			}
		explicit BeginDisabled(TArgs&&... args)
		{
			ImGui::BeginDisabled(std::forward<TArgs>(args)...);
		}
	};

	class BeginGroup :
		public detail::RAIIWrapper<BeginGroup, &ImGui::EndGroup>
	{
	public:
		template <class... TArgs>
			requires requires
			{
				{ ImGui::BeginGroup(std::declval<TArgs>()...) };
			}
		explicit BeginGroup(TArgs&&... args)
		{
			ImGui::BeginGroup(std::forward<TArgs>(args)...);
		}
	};

	class BeginListBox :
		public detail::ConditionalRAIIWrapper<BeginListBox, &ImGui::EndListBox, true>
	{
		using super = ConditionalRAIIWrapper<BeginListBox, &ImGui::EndListBox, true>;

	public:
		template <class... TArgs>
			requires requires
			{
				{ ImGui::BeginListBox(std::declval<TArgs>()...) } -> std::convertible_to<bool>;
			}
		explicit BeginListBox(TArgs&&... args) :
			super{ ImGui::BeginListBox(std::forward<TArgs>(args)...) }
		{
		}
	};
}

#endif
