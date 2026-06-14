#include <gtest/gtest.h>

#include "Entity.hpp"
#include "Components.hpp"

TEST(Scene, EntityIDsAreUnique)
{
    Scene scene{};

    auto e1 = scene.CreateEntity();
    auto e2 = scene.CreateEntity();

    EXPECT_NE(e1, e2);
}

TEST(Scene, EntitySlotsAreReused)
{
    Scene scene{};

    auto e1 = scene.CreateEntity();
    auto e2 = scene.CreateEntity();
    auto e3 = scene.CreateEntity();
    auto e4 = scene.CreateEntity();

    scene.DestroyEntity(e3);

    auto e5 = scene.CreateEntity();

    EXPECT_EQ(GetEntityIndex(e3), GetEntityIndex(e5));
}

TEST(Scene, ComponentTypesAreUnique)
{
    auto c1 = GetComponentTypeID<TransformComponent>();
    auto c2 = GetComponentTypeID<SpriteComponent>();

    EXPECT_NE(c1, c2);
}

TEST(Scene, SameComponentTypesHaveSameIDs)
{
    auto c1 = GetComponentTypeID<WanderComponent>();
    auto c2 = GetComponentTypeID<WanderComponent>();

    EXPECT_EQ(c1, c2);
}

TEST(Scene, GenerationIncrementsOnDestruction)
{
    Scene scene{};

    auto e1 = scene.CreateEntity();
    EntityGeneration e1Generation = GetEntityGeneration(e1);
    scene.DestroyEntity(e1);

    auto e2 = scene.CreateEntity();
    EntityGeneration e2Generation = GetEntityGeneration(e2);

    EXPECT_NE(e1Generation, e2Generation);
}

TEST(Scene, CanAddComponent)
{
    Scene scene{};

    auto e1 = scene.CreateEntity();
    auto c1 = scene.AddComponent<TransformComponent>(e1);

    auto c1ID = GetComponentTypeID<TransformComponent>();

    auto entityDesc = scene.entities[GetEntityIndex(e1)];

    bool hasComponent = entityDesc.mask.test(c1ID);

    EXPECT_EQ(hasComponent, true);
}

TEST(Scene, CanRemoveComponent)
{

}

TEST(Scene, CanDestroyEntity)
{

}

TEST(Scene, CanGetComponent)
{

}

TEST(View, MakeView)
{

}