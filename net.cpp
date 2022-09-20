#include "net.h"

void Net::place_object(int* x, int* y, char m, bool p, bool c, int id)
{
	// when commanded, creates an object with given model where a player is

	// look for objects at given location
	bool match = false;

	Node* temp = Top;
	while (temp != nullptr)
	{
		if (temp->x == *x && temp->y == *y) 
		{
			match = true;
			break;
		}
		temp = temp->next;
	}

	// create an object if the location is free
	if (!match)
	{
		Node* obj = nullptr;

		// a generator
		if (p && !c) obj = new Node(*x, *y, m, true, false, id);

		// a cable
		else if (!p && c) obj = new Node(*x, *y, m, false, true, id);

		// anything else
		else  obj = new Node(*x, *y, m, false, false, id);
		
		obj->next = Top;
		Top = obj;
	}
}

void Net::draw_objects(char* s)
{
	// draws all existing objects
	Node* temp = Top;
	while (temp != nullptr)
	{
		s[temp->x + temp->y * screen_width] = temp->model;
		temp = temp->next;
	}
}

void Net::destroy_object(int* x, int* y)
{
	// destroys an object at player's location
	Node* temp = Top;
	Node* prev = nullptr;

	while (temp != nullptr)
	{
		if (temp->x == *x && temp->y == *y)
		{
			if (prev == nullptr)
			{
				Top = temp->next;
				delete temp;
			}
			else
			{
				prev->next = temp->next;
				delete temp;
			}

			break;
		}

		prev = temp;
		temp = temp->next;
	}
}

void Net::connect_cables()
{
	// connects all cables to something nearby
	bool u = false,
		r = false,
		d = false,
		l = false;

	Node* temp = Top;
	Node* near = nullptr;

	while (temp != nullptr)
	{
		// look for all the cables
		if (temp->cable)
		{
			// when a cable is found look for anything close to this cable
			near = Top;
			while (near != nullptr)
			{
				if (near->y == temp->y - 1 && near->x == temp->x)
				{
					// up
					u = true;
				}
				if (near->x == temp->x + 1 && near->y == temp->y)
				{
					// right
					r = true;
				}
				if (near->y == temp->y + 1 && near->x == temp->x)
				{
					// down
					d = true;
				}
				if (near->x == temp->x - 1 && near->y == temp->y)
				{
					// left
					l = true;
				}

				near = near->next;
			}

			// choose a model for a cable based on whether there are nearby objects or not

			if (!u && !r && !d && !l) temp->model = pixel_cable_disconnected; // no object nearby

			else if (!u && r && !d && l) temp->model = pixel_cable_horizontal;

			else if ((!u && r && !d && !l) || (!u && !r && !d && l)) temp->model = pixel_cable_horizontal;

			else if (u && !r && d && !l) temp->model = pixel_cable_vertical;

			else if ((u && !r && !d && !l) || (!u && !r && d && !l)) temp->model = pixel_cable_vertical;

			else if (u && r && d && l) temp->model = pixel_cable_intersection_urdl;

			else if (u && r && !d && !l) temp->model = pixel_cable_intersection_ur;

			else if (!u && r && d && !l) temp->model = pixel_cable_intersection_rd;

			else if (!u && !r && d && l) temp->model = pixel_cable_intersection_dl;

			else if (u && !r && !d && l) temp->model = pixel_cable_intersection_lu;

			else if (u && r && !d && l) temp->model = pixel_cable_intersection_lur;

			else if (u && r && d && !l) temp->model = pixel_cable_intersection_urd;

			else if (!u && r && d && l) temp->model = pixel_cable_intersection_rdl;

			else if (u && !r && d && l) temp->model = pixel_cable_intersection_dlu;
		}

		u = false, r = false, d = false, l = false;
		temp = temp->next;
	}
}

void Net::power_up()
{
	// makes machines connected to the net work

	Node* temp = Top;
	Node* near = nullptr;

	while (temp != nullptr)
	{
		near = Top;
		// look for anything to be powered up (not cables and generators)
		if (temp->id == 3)
		{
			// power on & off
			while (near != nullptr)
			{
				// if cable is near in any direction and powered then power on
				if ((near->y == temp->y - 1 && near->x == temp->x) ||
					(near->x == temp->x + 1 && near->y == temp->y) ||
					(near->y == temp->y + 1 && near->x == temp->x) ||
					(near->x == temp->x - 1 && near->y == temp->y))
				{
					if (near->powered && (near->id == 1 || near->id == 2))
					{
						temp->powered = true;
						temp->model = pixel_lamp_powered;
						break;
					}
				}
				temp->powered = false;
				temp->model = pixel_lamp_unpowered;
				near = near->next;
			}
		}

		temp = temp->next;
	}
}

void Net::cables_power_up()
{
	// makes the cables carry the power
	
	// look for a cable that's connected to a generator directly
	Node* cab = Top;
	Node* near = nullptr;
	bool ctgFound = false;

	while (cab != nullptr)
	{
		near = Top;

		// once a cable is found,
		if (cab->cable)
			while (near != nullptr)
			{
				cab->ctg = false;
				cab->powered = false; // by default
				// if it happens to be...
				if ((near->y == cab->y - 1 && near->x == cab->x) ||
					(near->x == cab->x + 1 && near->y == cab->y) ||
					(near->y == cab->y + 1 && near->x == cab->x) ||
					(near->x == cab->x - 1 && near->y == cab->y))
				{
					// ...near a generator,
					if (near->id == 1)
					{
						// then it is powered
						cab->ctg = true;
						cab->powered = true;
						ctgFound = true;
						cab->model = 'g';
						break;
					}
				}
				near = near->next;
			}
		cab = cab->next;
	}

	bool ctcFound = false;
	if (ctgFound)
	{
		// there are ctg's; find a cable that's connected to one
		cab = Top;
		while (cab != nullptr)
		{
			if (cab->cable && !cab->ctg) 
			{
				cab->ctc = false;
				near = Top;
				while (near != nullptr)
				{
					if ((near->y == cab->y - 1 && near->x == cab->x) ||
						(near->x == cab->x + 1 && near->y == cab->y) ||
						(near->y == cab->y + 1 && near->x == cab->x) ||
						(near->x == cab->x - 1 && near->y == cab->y))
					{
						if (!ctcFound && (near->cable && near->ctg))
						{
							cab->ctc = true;
							cab->powered = true;
							ctcFound = true;
							cab = Top;
							cab->model = 'p';
						}
						else if (!cab->ctc && near->cable && near->powered)
						{
							cab->powered = true;
							cab->model = 'h';
						}
					}
					near = near->next;
				}
			}
			cab = cab->next;
		}
	}
	else
	{
		// no cable is connected to a generator; unpower all existing cables
		cab = Top;
		while (cab != nullptr)
		{
			if (cab->cable) cab->powered = false;
			cab = cab->next;
		}
	}
}