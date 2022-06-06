#pragma once
#pragma execution_character_set("utf-8")

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <string>
#include <memory>
#include <algorithm>
#include <functional>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <fstream>

#include "utility.h"


constexpr int		GROUND_Y = 550;
constexpr int		GAME_W = 1'280;
constexpr double	GRAVITY = 3'500;
constexpr Uint32	ETERNAL = (1U << 30) - 1;
constexpr Uint32	SECOND = 1'000;

constexpr SDL_Color	WHITE = { 255, 255, 255, 255 };
constexpr SDL_Color	BLACK = { 0, 0, 0, 255 };
constexpr SDL_Color	RED = { 255, 0, 0, 255 };
constexpr SDL_Color	GREEN = { 0, 255, 0, 255 };
constexpr SDL_Color	BLUE = { 0, 0, 255, 255 };
constexpr SDL_Color	YELLOW = { 255, 255, 0, 255 };

class App;

class Window;
class Renderer;
class Initializer;
class FieldObject;

class ObjectManager;
class SoundManager;
class TimeManager;


class Character;
class CharacterOnSkillBird1;
class CharacterOnSkillBird2;
class CharacterOnSkillDuck1;
class CharacterOnSkillDuck2;
class CharacterOnSkillOctopus1;
class CharacterOnSkillOctopus2;

class Monster;
class MonsterBird;
class MonsterDuck;
class MonsterOctopus;

class DamageBox;
template <int X_SPEED, int Y_SPEED>
class DamageBoxThorn;
class DamageBoxLeg;
class DamageBoxBead;
class DmaageBoxNormalAttack;

class Effect;
class FontEffect;

struct Texture;