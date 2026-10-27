#pragma once
#include <algorithm>
#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

template <typename U>
concept Dereferenceable = requires(const U & u) { *u; };

template <typename T>
class Collection {
private:
    std::vector<T> items;

public:
    Collection() = default;
    Collection(std::initializer_list<T> init) : items(init) {}

    void add(const T& item) { items.push_back(item); }
    void add(T&& item) { items.push_back(std::move(item)); }

    Collection& operator+=(const T& item) { add(item); return *this; }
    Collection& operator+=(T&& item) { add(std::move(item)); return *this; }

    bool remove(std::size_t index) {
        if (index >= items.size()) return false;
        items.erase(items.begin() + static_cast<std::ptrdiff_t>(index));
        return true;
    }

    template <typename Pred>
    std::size_t remove_if(Pred pred) {
        const std::size_t before = items.size();
        auto removed = std::ranges::remove_if(items, pred);
        items.erase(removed.begin(), removed.end());
        return before - items.size();
    }

    T& get(std::size_t index) {
        if (index >= items.size()) throw std::out_of_range("Collection: index out of range");
        return items[index];
    }
    const T& get(std::size_t index) const {
        if (index >= items.size()) throw std::out_of_range("Collection: index out of range");
        return items[index];
    }
    T& operator[](std::size_t index) { return get(index); }
    const T& operator[](std::size_t index) const { return get(index); }

    template <typename Pred>
    std::optional<std::size_t> find_index(Pred pred) const {
        for (std::size_t i = 0; i < items.size(); ++i) {
            if (pred(items[i])) return i;
        }
        return std::nullopt;
    }

    template <typename Pred>
    T* find_if(Pred pred) {
        for (auto& item : items) {
            if (pred(item)) return &item;
        }
        return nullptr;
    }
    template <typename Pred>
    const T* find_if(Pred pred) const {
        for (const auto& item : items) {
            if (pred(item)) return &item;
        }
        return nullptr;
    }

    std::optional<std::size_t> index_of(const T& value) const
        requires std::equality_comparable<T>
    {
        for (std::size_t i = 0; i < items.size(); ++i) {
            if (items[i] == value) return i;
        }
        return std::nullopt;
    }
    bool contains(const T& value) const
        requires std::equality_comparable<T>
    {
        return index_of(value).has_value();
    }

    std::size_t get_size() const { return items.size(); }
    bool empty() const { return items.empty(); }
    void clear() { items.clear(); }

    void print(std::ostream& os) const {
        if (items.empty()) {
            os << "  (collection is empty)\n";
            return;
        }
        for (std::size_t i = 0; i < items.size(); ++i) {
            const auto& item = items[i];
            os << "  " << i + 1 << ". ";
            if constexpr (Dereferenceable<T>) {
                if (item) os << *item;
                else os << "(null)";
            }
            else {
                os << item;
            }
            os << "\n";
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Collection& c) {
        c.print(os);
        return os;
    }

    std::vector<T>::iterator begin() { return items.begin(); }
    std::vector<T>::iterator end() { return items.end(); }
    std::vector<T>::const_iterator begin() const { return items.begin(); }
    std::vector<T>::const_iterator end() const { return items.end(); }
};

template <typename T, typename Pred>
Collection<T> filter_collection(const Collection<T>& source, Pred pred) {
    Collection<T> result;
    for (const auto& item : source) {
        if (pred(item)) result.add(item);
    }
    return result;
}

template <typename T, typename Pred>
Collection<std::size_t> select_indices(const Collection<T>& source, Pred pred) {
    Collection<std::size_t> result;
    for (std::size_t i = 0; i < source.get_size(); ++i) {
        if (pred(source[i])) result.add(i);
    }
    return result;
}

template <typename T, typename Func>
auto transform_collection(const Collection<T>& source, Func func) {
    using R = std::decay_t<decltype(func(std::declval<const T&>()))>;
    Collection<R> result;
    for (const auto& item : source) result.add(func(item));
    return result;
}

template <typename T, typename Pred>
std::size_t count_matching(const Collection<T>& source, Pred pred) {
    std::size_t count = 0;
    for (const auto& item : source) {
        if (pred(item)) ++count;
    }
    return count;
}

template <typename T, typename KeyFunc>
const T* find_max_by(const Collection<T>& source, KeyFunc key) {
    const T* best = nullptr;
    for (const auto& item : source) {
        if (best == nullptr || key(item) > key(*best)) best = &item;
    }
    return best;
}
