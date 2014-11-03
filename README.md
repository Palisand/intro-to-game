intro-to-game
=============

CS3113 Introduction to Game Programming Repository

Assignment 1
------------

* Create a simple 2D scene using textured and untextured polygons
* You can use any images you want, but feel free to use the assets in the class github repo
* At least one element must be animated
  * 3 elements animated: flower, bud, and wave
* You must use at least 3 different textures
  * 4 different hand textures used
* At least one element of the scene must use vertex colors
  * 4 elements using vertex colors: flower, wave, and ground
* Notes:
  * Place the 'zombie_hands' folder in the 'Supporting Files' group

Assignment 2
------------

* Make PONG!
* You do not need to keep score (we have not learned text output yet)
* You must detect player wins
* Use images or basic shapes
  * Basic shapes, just like original
* Can use keyboard, mouse, or joystick
  * Keyboard functionality implemented
* Notes:
  * Score not implemented
  * Top/bottom paddle-ball collision to be changed if necessary

Assignment 3
------------

* Make Space Invaders
* Must have 3 States
  * TITLE SCREEN
  * GAME (called GameLevel in code)
  * GAME OVER
* Must keep and display score
* Must use sprite sheets
* Organized code (well, somewhat organized)
* Notes:
  * DO NOT PLAY if you are prone to epileptic attacks

Assignment 4
------------
* Make a single screen platformer
* Use velocity, acceleration, friction, and gravity
* Use fixed time step
* Player must be able to jump only when standing on ground [UP_ARROW or W]
  * SPACEBAR will, however, allow you to break this rule and jump in midair
* Player collects items
* Uses unanimated graphics

Assignment 5
------------
* Make a simple scrolling sidescroller ~~or topdown~~ game demo
* Use a tilemap
* Implement scrolling
* Load level from a file

Assignment 6 
------------
(To Use: Copy contents into assigment 5 directory)
* Use SDL_mixer to implement sound on to Assignment 5
* One music track
  * Diogenes in Hell 04 - Potential Space (mp3)
* Two sound effects
  * step.wav
  * jump.wav
* Known Glitch:
  * Emits an initial short burst of static when executed

Assignment 7
------------
* Make simple asteroids
* Use transformation matrices
* Use non-axis aligned collisions
* Notes:
  * Expect to see a bunch of white rectangles, with a small one in the middle