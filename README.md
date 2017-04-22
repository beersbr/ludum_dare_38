# ludum_dare_38
Ludum Dare 38 A small world


We have a way to draw tiles now.

We should probably create entities for each tile in the level object. The level object then just becomes a data holder. We probably want an entity to have an attachement to a tile. I would probably just keep it simple and add a flag to the entity struct that says "is_tile" and just add all the tile stuff to the entity. Then we can just do simple asks and keep our data SUPER flat.

```
struct entity {
	bool is_tile
	tile *tile_thing; // probably needs its coordinates

	bool is_enemy
	// ... enemy properties ...

	bool is_player;
	// ... player properties .. .

	bool is_item
	// so on and so forth

}
```

that way we dont need any complicated lookups and we can just jump to the things we need to later on when we start handling memory.

We will need a way to capture items and modifiers. I'm not sure how to handle that entirely.

for example say we have items 

A: causes the player to move two spaces in the chosen direction
B: causes player to do damage based on turn (base damage + damage%turn) [ a turn would be the player taking an action ]
C: causes the player to gain 1hp at the end of every turn


to handle all that generically seems like a pain... BUT we can just use function pointers. We could also just use a class with all the methods for it and use polymorphism to handle it (potentially ew :( ) 

So I say maybe we do something like declare all the items as data:

```
#define BEFORE_TURN 0x01
#define AFTER_TURN 0x02
#define BEFORE_MOVE 0x04
#define AFTER_MOVE 0x08
#define BEFORE_ATTACK 0x10
#define AFTER_ATTACK 0x10

typedef void (*item_callback)(entity_t *player_entity)


struct item {
	string description
	string name

	item_callback before_turn;
	item_callbcak after_turn;
	item_callback before_move;
	item_callback after_move;
	item_callback before_attack;
	item_callback after_attack;
	/// etc;
}
```

then we can just define the items as data during runtime via a text file or something using lua or even python as the scripting language for it.

we dont ahve to use a bitfield we could just sort the items by when they happen and then we can just know when to use each item instead of iterating over them. IE a map/dictionary


with items we need someway to hold items. a list of them can probalby just be kept on the entity

```

struct entity {
//...  

	bool is_item_holder;
	std::vector<item_t> item_list;
/...
}
```

We also need a way to show text in the game... I can probably handle that tomorrow 


## Saturday 04/22/17 at 01:07AM - 629 files in 1.02 secs TODO REVIEW
### NOTE (5)
1. src/main.cpp:50    This is the default shader, it doesnt do anything super interesting right now... and its on the stack
2. src/main.cpp:57    Create the scene. The scene handles all the objects and stuff that we need for our game at a certain
3. src/main.cpp:76    This is a mesh. a mesh, right now, is jst the aggregation of a bunch of vertices. we probably dont want it
4. src/main.cpp:134   a model is the opengl manifsstation of a mesh. (it could have more than one mesh) and handles
5. src/main.cpp:140   An entity is where we care abot things. It is the holder for the actual game object.

### TODO (2)
1. src/core.cpp:27    These should be added to any scaling/rtoation/transaltion that is on the model
2. src/main.cpp:51    We may need a shader maanger


## Regular TODO

* Add text/font rendering things and functions to do it.
* add state manager to manage game states (paused, menu)
	* be able to transition between state nicely via animations ( this means that we will need time passed around [SDL_TICKS() should work fine for us ])
* It would be nice to be able to move the camera around via the mouse (perhaps holding the middle mouse button will allow the user to move the camera around the center)

