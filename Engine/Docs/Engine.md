## Intro
This class is the heart of the engine. It contains instances of all the subsystems, allows to create entities and updates and renders the game.
## General usage
You can't instantiate the class "Engine" directly, you have to inherit from it and instantiate your own class overriding the method "bool OnUpdate(const float& dt)".

When instantiated, you can run the game with this simple snippet:

~~~cpp
    while (engine.Update())
    {
    }
~~~

"Update()" returns _false_  when the game receives the closing signal.
# Contents
## Constructor
## Variables
## Methods