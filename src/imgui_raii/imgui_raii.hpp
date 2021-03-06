//          Copyright Dominic Koepke 2021 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef IMGUI_RAII_IMGUI_RAII_HPP
#define IMGUI_RAII_IMGUI_RAII_HPP

#pragma once

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
			requires requires
			{
				{ std::invoke(VBeginFunc, std::declval<TArgs>()...) };
			}
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
			std::invoke(func);
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
			requires requires
			{
				{ std::invoke(VBeginFunc, std::declval<TArgs>()...) } -> std::convertible_to<bool>;
			}
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

		ConditionalRAIIWrapper& operator >(std::invocable<> auto func)
		{
			if (m_Result)
				std::invoke(func);
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

	constexpr auto beginTabBar = []<class... TArgs>(TArgs&&... args)
	{
		return ImGui::BeginTabBar(std::forward<TArgs>(args)...);
	};

	constexpr auto beginTabItem = []<class... TArgs>(TArgs&&... args)
	{
		return ImGui::BeginTabItem(std::forward<TArgs>(args)...);
	};

	constexpr auto beginTable = []<class... TArgs>(TArgs&&... args)
	{
		return ImGui::BeginTable(std::forward<TArgs>(args)...);
	};

	constexpr auto pushId = []<class... TArgs>(TArgs&&... args)
	{
		return ImGui::PushID(std::forward<TArgs>(args)...);
	};

	constexpr auto pushStyleColor = []<class... TArgs>(TArgs&&... args)
	{
		return ImGui::PushStyleColor(std::forward<TArgs>(args)...);
	};

	constexpr auto popStyleColor = [] { return ImGui::PopStyleColor(1); };

	constexpr auto pushStyleVar = []<class... TArgs>(TArgs&&... args)
	{
		return ImGui::PushStyleVar(std::forward<TArgs>(args)...);
	};

	constexpr auto popStyleVar = [] { return ImGui::PopStyleVar(1); };

	constexpr auto pushTextWrapPos = []<class... TArgs>(TArgs&&... args)
	{
		return ImGui::PushTextWrapPos(std::forward<TArgs>(args)...);
	};
}

namespace imgui_raii
{
	class BeginContext
	{
	public:
		template <class... TArgs>
			requires requires
			{
				{ ImGui::CreateContext(std::declval<TArgs>()...) } -> std::convertible_to<ImGuiContext*>;
			}
		[[nodiscard]]
		explicit BeginContext(TArgs&&... args) :
			m_Context{ ImGui::CreateContext(std::forward<TArgs>(args)...) }
		{
		}

		[[nodiscard]]
		BeginContext(BeginContext&& other) noexcept :
			m_Context{ std::exchange(other.m_Context, nullptr) }
		{
		}

		BeginContext& operator =(BeginContext&& other) noexcept
		{
			std::swap(m_Context, other.m_Context);
			return *this;
		}

		~BeginContext() noexcept
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

		BeginContext(const BeginContext&) = delete;
		BeginContext& operator =(const BeginContext&) = delete;

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
	using BeginMenu = detail::ConditionalRAIIWrapper<detail::beginMenu, &ImGui::EndMenu, true>;
	using BeginTabBar = detail::ConditionalRAIIWrapper<detail::beginTabBar, &ImGui::EndTabBar, true>;
	using BeginTabItem = detail::ConditionalRAIIWrapper<detail::beginTabItem, &ImGui::EndTabItem, true>;
	using BeginTable = detail::ConditionalRAIIWrapper<detail::beginTable, &ImGui::EndTable, true>;

	using BeginDisabled = detail::RAIIWrapper<detail::beginDisabled, &ImGui::EndDisabled>;
	using BeginGroup = detail::RAIIWrapper<&ImGui::BeginGroup, &ImGui::EndGroup>;
	using BeginTooltip = detail::RAIIWrapper<&ImGui::BeginTooltip, &ImGui::EndTooltip>;
	using NewFrame = detail::RAIIWrapper<&ImGui::NewFrame, &ImGui::Render>;

	using PushAllowKeyboardFocus = detail::RAIIWrapper<&ImGui::PushAllowKeyboardFocus, &ImGui::PopAllowKeyboardFocus>;
	using PushButtonRepeat = detail::RAIIWrapper<&ImGui::PushButtonRepeat, &ImGui::PopButtonRepeat>;
	using PushClipRect = detail::RAIIWrapper<&ImGui::PushClipRect, &ImGui::PopClipRect>;
	using PushFont = detail::RAIIWrapper<&ImGui::PushFont, &ImGui::PopFont>;
	using PushID = detail::RAIIWrapper<detail::pushId, &ImGui::PopID>;
	using PushItemWidth = detail::RAIIWrapper<&ImGui::PushItemWidth, &ImGui::PopItemWidth>;
	using PushStyleColor = detail::RAIIWrapper<detail::pushStyleColor, detail::popStyleColor>;
	using PushStyleVar= detail::RAIIWrapper<detail::pushStyleVar, detail::popStyleVar>;
	using PushTextWrapPos= detail::RAIIWrapper<detail::pushTextWrapPos, &ImGui::PopTextWrapPos>;
}

#endif
