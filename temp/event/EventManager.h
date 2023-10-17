#pragma once

#include "ll/api/event/Listener.h"

namespace ll::event {

template <typename EventType>
struct PriorityComparator {
    using ListenerType = Listener<EventType>;
    bool operator()(const std::shared_ptr<ListenerType>& lhs, const std::shared_ptr<ListenerType>& rhs) const;
};

/**
 * @brief Event manager class.
 * @tparam EventType Event type.
 * @details This class is used to manage event listeners.
 */
template <typename EventType>
class EventManager {
private:
    using ListenerType = Listener<EventType>;
    static std::set<std::shared_ptr<ListenerType>, PriorityComparator<EventType>> listenerList;

public:
    /**
     * @brief Add an event listener.
     * @param listener The listener instance to add.
     */
    LLAPI static void addListener(std::shared_ptr<ListenerType> listener);

    /**
     * @brief Remove an event listener.
     * @param listener The listener to remove.
     */
    LLAPI static void removeListener(std::weak_ptr<ListenerType> listener);

    /**
     * @brief Get the listener by id.
     * @param id The listener id.
     */
    LLAPI static std::optional<std::weak_ptr<ListenerType>> getListener(int id) noexcept;

    /**
     * @brief Get if the event has listeners.
     * @return True if the event has listeners.
     */
    [[nodiscard]] LLAPI static bool hasListener() noexcept;

    /**
     * @brief Fire the event.
     * @param event Event instance.
     */
    LLAPI static void fireEvent(EventType& event);

    /**
     * @brief Send event to specific plugin.
     * @param event Event instance.
     * @param plugin Plugin handle.
     */
    LLAPI static void fireEvent(EventType& event, HMODULE plugin);
};

} // namespace ll::event
