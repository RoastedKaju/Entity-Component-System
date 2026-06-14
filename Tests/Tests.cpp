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
    Scene scene{};

    auto e1 = scene.CreateEntity();
    scene.AddComponent<TransformComponent>(e1);

    scene.RemoveComponent<TransformComponent>(e1);

    auto entityDesc = scene.entities[GetEntityIndex(e1)];

    const bool hasComponent = entityDesc.mask.test(GetComponentTypeID<TransformComponent>());

    EXPECT_NE(hasComponent, true);
}

TEST(Scene, CanDestroyEntity)
{
    Scene scene{};

    auto e1 = scene.CreateEntity();
    scene.DestroyEntity(e1);

    const bool isEmpty = scene.freeIndices.size() == 1;

    EXPECT_EQ(isEmpty, true);
}

TEST(Scene, CanGetComponent)
{
    Scene scene{};

    auto e1 = scene.CreateEntity();
    scene.AddComponent<TransformComponent>(e1);

    auto c1 = scene.GetComponent<TransformComponent>(e1);

    EXPECT_NE(c1, nullptr);
}

TEST(View, MakeView)
{
    Scene scene{};

    // Create 10 entities
    for (size_t i = 0; i < 10; ++i)
    {
        scene.CreateEntity();
    }

    // Add only transform to first 5
    for (size_t i = 0; i < 5; ++i)
    {
        auto entityDesc = scene.entities[i];
        auto eID = CreateEntityID(i, entityDesc.generation);

        scene.AddComponent<TransformComponent>(eID);
    }

    // Add both transform and sprite to last 5
    for (size_t i = 5; i < 10; ++i)
    {
        auto entityDesc = scene.entities[i];
        auto eID = CreateEntityID(i, entityDesc.generation);

        scene.AddComponent<TransformComponent>(eID);
        scene.AddComponent<SpriteComponent>(eID);
    }

    View<TransformComponent, SpriteComponent> view{scene};
    uint8_t viewCount = 0;
    view.Each([&viewCount](EntityID id, TransformComponent &transform, SpriteComponent &sprite)
              { ++viewCount; });

    EXPECT_EQ(viewCount, 5);
}