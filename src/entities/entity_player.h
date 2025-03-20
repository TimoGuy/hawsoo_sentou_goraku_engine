#pragma once

#include "simulating_entity_ifc.h"
#include "ticking_world_simulation_public.h"
#include "world_entity.h"


/*
 @NOCHECKIN: For prototyping.

    YKNOW, IT MAY BE FEASIBLE TO HAVE EVERYTHING DATA BASED.
        NOTE: Have each component belong to some kind of pool thing.

    - Input step
        - None.
        - Player movement input.
        - Bug AI input.
        - Moving platform path input.
        - etc.

    -  Logic step
        - None.
        - Humanoid movement.
        - Move along path.

    - Physics step(s)
        - None.
        - Kinematic physics thing
        - Character controller.
    
    - Model anim step
        - Model animation player. (@NOTE: This is simulation job source for inputs, render job source for pulling data for skinning shader)
        - Skinned model submission to skinning shader. (@NOTE: This is render job source)
    OR
    - Static model step (do nothing).

    - Render object step (@NOTE: Render job source)
        - Pull interpolated physics transform at a rate that is appropriate for the render thread.




    Simulation pipeline:
    - Group 1 (1 job per input-logic-animator set (input and animator optional)):
      - Input step (write to logic step)
            ↓
      - Logic step (set requested animation to play (if have animator))
            ↓
      - Calculate skeletal animation bone matrices.

    - Group 2 (1 job per physics objects)
      - Write velocity and transforms to physics objects based off changes.
      - @NOTE: Happens after skeletal animation bone matrices bc of hit boxes and hurt boxes.

    - Group 3 (Only one job... well until it's multithreaded, then it'll be one job for every core (one master, remaining supporters)).
      - Update physics world.
 */



// @NOCHECKIN: For prototyping.
using Ref_t = void*;
struct Entity_player  // What if this is just essentially an object creation/destruction script?
{
    // // @CHECK: Possibly unneeded? Bc there are things inside the renderer that can take care of position and rotation?
    // Ref_t world_entity;




    // Drives `humanoid_movement_statemachine`.
    // Can place something like "Bug AI input" or smth.
    // @NOTE: Movement input is unaware of 
    Ref_t player_movement_input;  // Pre-physics sim tick.

    // Drives `char_ctrller_actor` and `model_animation_player` (optional).
    // Has all movesets of every humanoid thing w/ a bunch of settings that are tweakable.
    Ref_t humanoid_movement_statemachine;  // @NOTE: Moving thru a doorway is simply just teleporting forward and stopping the entity from being able to do any inputs and 

    // Computes transform data and stores result.
    // Allows `render_object` to pull an interpolated data from this.
    // @NOTE: Can do `kinematic_actor` instead.
    Ref_t char_ctrller_actor;

    // Drives `skinned_model`.
    // @NOTE: Cannot be used to drive a `static_model`.
    // @NOTE: Just calculates the skinning data then provides the data to `skinned_model` which then gets picked up by the renderer so that the referenced render object can use it.
    Ref_t model_animation_player;

    // Provides skinning data for GPU skinning compute shader, then gives that skinned mesh to `render_object`.
    // @NOTE: Can do `static_model` instead.
    Ref_t skinned_model;

    // Reference to renderer's render object.
    Ref_t render_object;
};

// @NOCHECKIN: For prototyping.
struct Entity_solid_platform
{

    // Drives transform of `kinematic_actor`.
    Ref_t moving_platform_input_path;

    // @NOTE: Collider is set here.
    Ref_t kinematic_actor;

    // @NOTE: Connects to kinematic actor.
    Ref_t render_object;

};

// @NOCHECKIN: For prototyping.
struct Entity_static_platform
{
    // @NOTE: No input to drive `kinematic_actor`.

    // @NOTE: Collider is set here.
    // @NOTE: Can set this to be static (But setting initial transform is required).
    Ref_t kinematic_actor;

    // @NOTE: Connects to kinematic actor.
    Ref_t render_object;

};

// @NOCHECKIN: For prototyping.
struct Entity_doorway
{

    Ref_t teleporter;
};


///////////////////////////////////////////////////////////////////////////////

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

    phys_obj::Actor_character_controller m_actor;
};
