#pragma once

#include "simulating_entity_ifc.h"
#include "world_entity.h"


class Entity_player
    : public Simulating_entity_ifc
    , public World_entity
{
public:
    Entity_player();

    virtual void on_sim_entity_start(pool_elem_key_t key) override;
    virtual void tick_sim_entity() override;
    virtual void on_sim_entity_destroy() override;

private:
    pool_elem_key_t m_sim_entity_key;

    
};
