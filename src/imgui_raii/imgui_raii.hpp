#ifndef IMGUI_RAII_IMGUI_RAII_HPP
#define IMGUI_RAII_IMGUI_RAII_HPP

#include <concepts>
#include <functional>

#include <imgui.h>

namespace imgui_raii::detail
{
	using EndFunc = void(*)();

	template <auto VBeginFunc, EndFunc VEnd>
	class RAIIWrapper
	{
	public:
		template <class... TArgs>
		explicit RAIIWrapper(TArgs&&... args)
		{
			std::invoke(VBeginFunc, std::forward<TArgs>(args)...);
		}

		~RAIIWrapper() noexcept
		{
			std::invoke(VEnd);
		}

		RAIIWrapper& operator /(std::invocable<> auto func)
		{
			std::invoke(std::ref(func));
			return *this;
		}

		RAIIWrapper(const RAIIWrapper&) = delete;
		RAIIWrapper& operator =(const RAIIWrapper&) = delete;
		RAIIWrapper(RAIIWrapper&&) = delete;
		RAIIWrapper& operator =(RAIIWrapper&&) = delete;
	};

	template <auto VBeginFunc, EndFunc VEnd, bool VConditionalEnd>
	class ConditionalRAIIWrapper
	{
	public:
		template <class... TArgs>
		explicit ConditionalRAIIWrapper(TArgs&&... args) :
			m_Result{ std::invoke(VBeginFunc, std::forward<TArgs>(args)...) }
		{
		}

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

		ConditionalRAIIWrapper& operator /(std::invocable<> auto func)
		{
			if (m_Result)
				std::invoke(std::ref(func));
			return *this;
		}

		ConditionalRAIIWrapper(const ConditionalRAIIWrapper&) = delete;
		ConditionalRAIIWrapper& operator =(const ConditionalRAIIWrapper&) = delete;
		ConditionalRAIIWrapper(ConditionalRAIIWrapper&&) = delete;
		ConditionalRAIIWrapper& operator =(ConditionalRAIIWrapper&&) = delete;

	private:
		const bool m_Result;
	};

	constexpr auto begin = []<class... TArgs>(TArgs&&... args)
	{
		return ImGui::Begin(std::forward<TArgs>(args)...);
	};

	constexpr auto beginChild = []<class... TArgs>(TArgs&&... args)
	{
		return ImGui::BeginChild(std::forward<TArgs>(args)...);
	};

	constexpr auto beginChildFrame = []<class... TArgs>(TArgs&&... args)
	{
		return ImGui::BeginChildFrame(std::forward<TArgs>(args)...);
	};

	constexpr auto beginCombo = []<class... TArgs>(TArgs&&... args)
	{
		return ImGui::BeginCombo(std::forward<TArgs>(args)...);
	};

	constexpr auto beginDisabled = []<class... TArgs>(TArgs&&... args)
	{
		ImGui::BeginDisabled(std::forward<TArgs>(args)...);
	};

	constexpr auto beginGroup = []<class... TArgs>(TArgs&&... args)
	{
		ImGui::BeginGroup(std::forward<TArgs>(args)...);
	};

	constexpr auto beginListBox = []<class... TArgs>(TArgs&&... args)
	{
		return ImGui::BeginListBox(std::forward<TArgs>(args)...);
	};

	constexpr auto beginMenuBar = []<class... TArgs>(TArgs&&... args)
	{
		return ImGui::BeginMenuBar(std::forward<TArgs>(args)...);
	};

	constexpr auto beginMainMenuBar = []<class... TArgs>(TArgs&&... args)
	{
		return ImGui::BeginMainMenuBar(std::forward<TArgs>(args)...);
	};

	constexpr auto beginMenu = []<class... TArgs>(TArgs&&... args)
	{
		return ImGui::BeginMenu(std::forward<TArgs>(args)...);
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

	using Begin = detail::ConditionalRAIIWrapper<detail::begin, &ImGui::End, false>;
	using BeginChild = detail::ConditionalRAIIWrapper<detail::beginChild, &ImGui::EndChild, false>;
	using BeginChildFrame = detail::ConditionalRAIIWrapper<detail::beginChildFrame, &ImGui::EndChildFrame, false>;
	using BeginCombo = detail::ConditionalRAIIWrapper<detail::beginCombo, &ImGui::EndCombo, true>;
	using BeginListBox = detail::ConditionalRAIIWrapper<detail::beginListBox, &ImGui::EndListBox, true>;
	using BeginMenuBar = detail::ConditionalRAIIWrapper<detail::beginMenuBar, &ImGui::EndMenuBar, true>;
	using BeginMainMenuBar = detail::ConditionalRAIIWrapper<detail::beginMainMenuBar, &ImGui::EndMainMenuBar, true>;

	using BeginDisabled = detail::RAIIWrapper<detail::beginDisabled, &ImGui::EndDisabled>;
	using BeginGroup = detail::RAIIWrapper<&ImGui::BeginGroup, &ImGui::EndDisabled>;
}

#endif
