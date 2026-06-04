#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "Common.hpp"

class ComponentTypeID
{
public:
    template <typename Type>
    static uint64_t Get()
    {
        static const uint64_t id = m_nextId++;
        return id;
    }

private:
    static inline uint64_t m_nextId{0};
};

#endif // COMPONENT_HPP