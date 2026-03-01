# Personal Space Invaders

A reimplementation of the Taito classic as a simple project to learn how games can be made. 
You could say this is my "personal" implementation of _Space Invaders_. 
And hopefully a different enough name to avoid lawsuits...

_Space Invaders_ made sense as a first learning project for a couple reasons.

1. It has a small, well-defined feature set which makes developing a viable product a very achievable target.
2. The art and audio is simple enough that even someone without art skills (such as myself) can create it.

This application was written using the [SDL3](https://wiki.libsdl.org/SDL3/FrontPage) and [SDL3_mixer](https://wiki.libsdl.org/SDL3_mixer/FrontPage) libraries.

Images/graphics/text/etc. were created using the [GNU Image Manipulation Program](https://www.gimp.org/) (GIMP) and SDL3.

Audio was created using [Little Sound DJ](https://www.littlesounddj.com/lsd/index.php), [BGB GameBoy Emulator](https://bgb.bircd.org/), and [Cakewalk SONAR](https://www.cakewalk.com/sonar).

No LLMs were used in the creation of any of the above.

All knowledge of game development related patterns was acquired from skimming [the web version](https://gameprogrammingpatterns.com/) of _Game Programming Patterns_ by Robert Nystrum and watching [this YouTube video](https://www.youtube.com/watch?v=GFO_txvwK_c).

_All intellectual property rights related to the original Space Invaders are owned by Taito Corporation._

## Play the Game!

The game can be played in the browser [on Itch.io here](https://phonaputer.itch.io/personal-space-invaders) (compiled to WASM using [Emscripten](https://emscripten.org/)).

## Thoughts about this project

I'm reasonably satisfied with how this game turned out.
The only thing missing from the actual _Space Invaders_ game is the mystery ship (which I may add later if I feel like it).
Code quality could be better, but for a small-size, one-off project I'm ok with leaving things as-is.
I got the "game dev" learning I wanted out of it.

SDL was quite a handy library.
The most similar thing I've used previously is the HTML5 canvas API, which feels slightly higher level.
Given how fiddly C/C++ can sometimes be, it is great that I can compile a single codebase for both Linux and browsers (as WASM).
And I did not need to write a single OS-specific compiler directive to achieve that.

I suspect this project would have taken a fraction of the time if I used a real game engine, though that would have defeated the purpose since I wrote this for fun.
Maybe I'll try rewriting with a full-fledged engine one day and see how much more productive the engine makes me.

Off the top of my head, stuff that I would improve if I planned to add features to this codebase or maintain it long-term includes...

- Add unit tests.
- Improve usage of polymorphism/indirection (currently a lot of things depend on concrete types).
- Read up on best practices for structuring C++ code. At minimum, it feels like there should be some good practices related to minimizing impacted files when recompiling after tweaking code.
- Read up more on game archiecture and thumb through some example codebases written by people with more experience than me.
