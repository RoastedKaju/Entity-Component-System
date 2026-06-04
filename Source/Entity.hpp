#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Common.hpp"

template <typename Tag = struct DefaultTag>
class Entity
{
public:
    constexpr explicit Entity(uint64_t id) : m_id{id} {}

    constexpr bool operator==(const Entity<Tag> &other) const { return m_id == other.m_id; }
    constexpr bool operator!=(const Entity<Tag> &other) const { return !(*this == other); }

    constexpr uint64_t Get() const { return m_id; }

private:
    uint64_t m_id;
};

struct BaseEntity
{
    BaseEntity() = default;
    ~BaseEntity() = default;
};

#endif // ENTITY_HPP