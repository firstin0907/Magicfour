# Magicfour
Magic Action Game made with SDL2(Simple Directmedia Layer) library.

# How To Start Game?
Just open .exe file and focus on created window. Then, press 'Z' key and play it!

# How To Play Game?
Press 'Z' key, then skill will be used.

Press 'X' key, then skill you get will be throwed.

Press Left or Right Key, then character will move.

When you are damaged by monster, you lose your skill.

If there are no skills in your hand, and you get touched by monster, you will die, and the game will end.
To kill monsters as many as possible is the subject of this game.

Enjoy it!

# Code Structure
FieldObject class is core class in this project. This represent a object shown throught window.

Update() method is called per every frame, and in this method, state_ variable may be changed by set_state() class.

set_state() method set state and modified time with executing another operation that should do when changing state.

Lastly, in draw() method, that will be called in every frame, will draw that object by using renderer class.
