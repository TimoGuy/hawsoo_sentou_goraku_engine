# Plan for Game Engine.

## Technical requirements for Soranin game engine.

- Nice renderer.
    - 3D and 2D (ui, etc) capabilities.
    - Variety of environments
        - Cloud world (outdoors)
        - Deep, dark caverns (indoors)
        - Lush plains (performant)
        - Candle-lit world (performant)
        - Quick travel thru public transportation (performant)
        - Day-night cycle (dynamic)
    - Extendible
        - Can load new shaders and materials for geometry.

- 3D character controller(s) for player and npcs.
    - Expandability for different movement traversal types.
    - Very very good moving platform compatibility.

- Sword fighting sekiro-like

- Enemy AI engine.

- Physics Maybe? (Coin planets?)

- Large open world rendering and physics.
    - World streaming system.

- Save system.

- Nice audio system.

- Singleplayer.
    - Perhaps, sending photos and stuff to others would be fun! (Well, that can be accomplished with a photo mode and opening the gallery directory)


## Order of Epics

- [x] Basic 3D renderer.

- [ ] 3D character controller.

- [ ] Skeletal animations in renderer.

- [ ] Combat animation and hitbox interaction.

- [ ] Humanoid Enemy AI.
    - ~~[ ] PVP~~ No probably not due to scope and adding more to the renderer or networking.

- [ ] Material collection and weapon materialization/durability/destruction.
    - Not sure if having the ability to merge durability of multiple weapons together would be good.

- [ ] World streaming and level loading.

- [ ] Nice-looking-izing.
