#pragma once
#include "ll/api/base/Global.h"

namespace ll::perm {


template <typename T>
class PermVector : public std::vector<T> {

    using Base = std::vector<T>;

public:
    PermVector() : Base() {}
    PermVector(Base const& base) : Base(base) {}
    PermVector(Base&& base) : Base(base) {}
    PermVector(PermVector<T> const& other) = default;
    PermVector(PermVector<T>&& other)      = default;

    bool contains(T const& xuid) const {
        for (auto& member : *this) {
            if (member == xuid) { return true; }
        }
        return false;
    }

    size_t count(T const& xuid) const {
        size_t result = 0;
        for (auto& member : *this) {
            if (member == xuid) { result++; }
        }
        return result;
    }

    T& push_back(T const& xuid) {
        if (contains(xuid)) { throw std::out_of_range("Failed to add the element: the element already exists"); }
        Base::push_back(xuid);
        return this->back();
    }

    template <typename... Args>
    T& emplace_back(Args&&... args) {
        return this->push_back(T(std::forward<Args>(args)...));
    }

    PermVector<T>& operator=(Base const& other) { return (PermVector<T>&)(((Base&)*this) = other); }
    PermVector<T>& operator=(PermVector<T> const& other) = default;
    PermVector<T>& operator=(PermVector<T>&& other)      = default;
};


template <typename T>
class PermContainer : public std::vector<T> {

    using Base = std::vector<T>;

public:
    PermContainer() : Base() {}
    PermContainer(Base const& base) : Base(base) {}
    PermContainer(Base&& base) : Base(base) {}
    PermContainer(PermContainer<T> const& other) = default;
    PermContainer(PermContainer<T>&& other)      = default;

    bool contains(std::string const& name) const {
        for (auto& el : *this) {
            if (el.name == name) { return true; }
        }
        return false;
    }

    typename Base::iterator find(std::string const& name) {
        for (auto it = this->begin(); it != this->end(); it++) {
            if (it->name == name) { return it; }
        }
        return this->end();
    }

    typename Base::const_iterator find(std::string const& name) const {
        for (auto it = this->begin(); it != this->end(); it++) {
            if (it->name == name) { return it; }
        }
        return this->end();
    }

    size_t count(std::string const& name) const {
        size_t result = 0;
        for (auto& el : *this) {
            if (el.name == name) { result++; }
        }
        return result;
    }

    template <typename... Args>
    T& getOrCreate(std::string const& name, Args&&... args) {
        for (auto& el : *this) {
            if (el.name == name) { return el; }
        }
        auto& el = this->emplace_back(args...);
        return el;
    }

    T& at(std::string const& name) {
        for (auto& el : *this) {
            if (el.name == name) { return el; }
        }
        throw std::out_of_range("Failed to get the element: the element does not exist");
    }
    T const& at(std::string const& name) const {
        for (auto& el : *this) {
            if (el.name == name) { return el; }
        }
        throw std::out_of_range("Failed to get the element: the element does not exist");
    }

    T& push_back(T const& el) {
        if (contains(el.name)) {
            throw std::out_of_range("Failed to add the element: the element with the same name already exists");
        }
        Base::push_back(el);
        return this->back();
    }

    template <typename... Args>
    T& emplace_back(Args&&... args) {
        Base::emplace_back(std::forward<Args>(args)...);
        if (count(this->back().name) > 1) {
            this->pop_back();
            throw std::out_of_range("Failed to add the element: the element with the same name already exists");
        }
        return this->back();
    }

    void remove(std::string const& name) {
        for (auto it = this->begin(); it != this->end(); it++) {
            if (it->name == name) {
                this->erase(it);
                return;
            }
        }
        throw std::out_of_range("Failed to remove the element: the target element does not exist");
    }

    T& operator[](std::string const& name) {
        T def{};
        def.name = name;
        return this->getOrCreate(name, def);
    }

    PermContainer<T>& operator=(PermContainer<T> const& other) = default;
    PermContainer<T>& operator=(PermContainer<T>&& other)      = default;
    PermContainer<T>& operator=(Base const& other) { return (PermContainer<T>&)(((Base&)*this) = other); }
};


template <typename T>
class PermPtrContainer : public std::vector<std::shared_ptr<T>> {

    using Base = std::vector<std::shared_ptr<T>>;

public:
    PermPtrContainer() : Base() {}
    PermPtrContainer(Base const& base) : Base(base) {}
    PermPtrContainer(Base&& base) : Base(base) {}
    PermPtrContainer(PermPtrContainer<T> const& other) = default;
    PermPtrContainer(PermPtrContainer<T>&& other)      = default;

    bool contains(std::string const& name) const {
        for (auto& el : *this) {
            if (el->name == name) { return true; }
        }
        return false;
    }

    typename Base::iterator find(std::string const& name) {
        for (auto it = this->begin(); it != this->end(); it++) {
            if (it->name == name) { return it; }
        }
        return this->end();
    }

    typename Base::const_iterator find(std::string const& name) const {
        for (auto it = this->begin(); it != this->end(); it++) {
            if (it->name == name) { return it; }
        }
        return this->end();
    }

    size_t count(std::string const& name) const {
        size_t result = 0;
        for (auto& el : *this) {
            if (el->name == name) { result++; }
        }
        return result;
    }

    template <typename... Args>
    std::shared_ptr<T>& getOrCreate(std::string const& name, Args&&... args) {
        for (auto& el : *this) {
            if (el->name == name) { return el; }
        }
        auto& el = this->emplace_back(args...);
        return el;
    }

    std::shared_ptr<T>& at(std::string const& name) {
        for (auto& el : *this) {
            if (el->name == name) { return el; }
        }
        throw std::out_of_range("Failed to get the element: the element does not exist");
    }
    const std::shared_ptr<T>& at(std::string const& name) const {
        for (auto& el : *this) {
            if (el->name == name) { return el; }
        }
        throw std::out_of_range("Failed to get the element: the element does not exist");
    }

    T& push_back(T const& el) {
        if (contains(el->name)) {
            throw std::out_of_range("Failed to add the element: the element with the same name already exists");
        }
        Base::push_back(std::make_shared<T>(el));
        return *this->back();
    }
    std::shared_ptr<T>& push_back(const std::shared_ptr<T>& el) {
        if (contains(el->name)) {
            throw std::out_of_range("Failed to add the element: the element with the same name already exists");
        }
        Base::push_back(el);
        return this->back();
    }

    template <typename... Args>
    std::shared_ptr<T>& emplace_back(Args&&... args) {
        Base::emplace_back(std::forward<Args>(args)...);
        if (count(this->back()->name) > 1) {
            this->pop_back();
            throw std::out_of_range("Failed to add the element: the element with the same name already exists");
        }
        return this->back();
    }

    void remove(std::string const& name) {
        for (auto it = this->begin(); it != this->end(); it++) {
            if ((*it)->name == name) {
                this->erase(it);
                return;
            }
        }
        throw std::out_of_range("Failed to remove the element: the target element does not exist");
    }

    std::shared_ptr<T>& operator[](std::string const& name) {
        auto def  = std::make_shared<T>(new T);
        def->name = name;
        return this->getOrCreate(name, def);
    }

    PermPtrContainer<T>& operator=(PermPtrContainer<T> const& other) = default;
    PermPtrContainer<T>& operator=(PermPtrContainer<T>&& other)      = default;
    PermPtrContainer<T>& operator=(Base const& other) { return (PermPtrContainer<T>&)(((Base&)*this) = other); }
};

} // namespace ll::perm