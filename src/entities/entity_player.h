#pragma once

#include "simulating_entity_ifc.h"
#include "world_entity.h"


class Entity_player : public Simulating_entity_ifc
{
public:
    Entity_player();

    void on_sim_entity_start(pool_elem_key_t key) override;
    void tick_sim_entity() override;
    void on_sim_entity_destroy() override;

    World_entity m_world_entity;

private:
    pool_elem_key_t m_sim_entity_key;
};
