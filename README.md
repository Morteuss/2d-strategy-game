# 2d-strategy-game

This is a 2d strategy game. Created with C++ std11 and SFML.

Assumptions:
* [x] when creating a new game, the map is randomly generated,
* [x] you cannot directly control a single character. You can only give global orders, e.g. build a wall, cut down a given tree, etc. then the nearest free (not doing anything at the moment) character should take care of it,
* [x] the main goal of the game is to keep the population alive,
* [ ] sometimes random events happen, e.g. attack of evil units, bad weather, etc.,
* [ ] climatic conditions may change,
* [x] trees from time to time grow back in any place and grow with time.

Project implemented with: https://github.com/hakisu in 2017 for classes at the University.
I am currently restoring the files, then I will make corrections (strong improvement of the quality of the code, optimization of the algorithms used) and will develop the game.
Code is written according to the rules (or at least that's what we tried to do at the time) described in the following Google guide: https://google.github.io/styleguide/cppguide.html

# Useful informations:
* Make sure you set the working directory to 2d_strategy_game. Otherwise the assets will not load correctly.


# Gameplay:
* Currently, every new game, causes a new map to be generated anew. When it starts up, a simple loader is displayed asynchronously. When the generation is successful and the new game window is displayed (in addition, a console with logs is displayed), click Space to start the game.

