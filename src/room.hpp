#ifndef _ROOM_HPP
#define _ROOM_HPP

enum Room_type { level_entrance, level_exit, wall /* TODO: add more room types? */  };

typedef struct {
	Room_type type;
} room_t;

#endif