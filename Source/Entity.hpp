#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Common.hpp"

/// @brief ID Class template for type-safe IDs
/// @tparam Tag
template <typename Tag>
class ID
{
public:
    constexpr ID(size_t id) : m_id(id) {}
    constexpr bool operator==(const ID &other) const { return m_id == other.m_id; }

    const size_t &Get() const { return m_id; }

private:
    size_t m_id;
};

struct EntityTag
{
};

#endif // ENTITY_HPP