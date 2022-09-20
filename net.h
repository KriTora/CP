#include "parameters.h"

struct Node
{
	Node* next;

	int x, y;
	char model;
	bool powered;
	bool cable;
	int id; // 1 generator, 2 cable, 3 lamp
	bool ctg; // connected to generator
	bool ctc;

	Node(int ix, int iy, char m, bool p, bool c, int i)
	{
		// b = false so only have to include this parameter when adding a generator
		// c = false so only have to include this parameter when adding a cable
		x = ix, y = iy;
		model = m;
		powered = p;
		cable = c;
		id = i;
		next = nullptr;
		ctg = false;
		ctc = false;
	};
};

class Net
{
	Node* Top;

public:
	Net() { Top = nullptr; };
	void place_object(int*, int*, char, bool, bool, int);
	void draw_objects(char*);
	void destroy_object(int*, int*);
	void connect_cables();
	void power_up();
	void cables_power_up();
};