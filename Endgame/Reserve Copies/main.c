#include "../inc/header.h"

int main(void) {
	if (!initializeSDL(void)) {
		return 1;
	}

	switch (showmenu(void)) {
		case 0:
			loc_room(void);
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
	}

	closeSDL(void);
	return 0;
}

