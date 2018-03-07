#ifndef Box_H
#define Box_H
#include <vector>
class Box
{
public:
	int direction;//0:y 1:x
	int type;//1:cube 2:cylinder 3:cone
	double x;
	double y;
	double length;
	int texture;
	int objNumber;
	double addition_x;
	double addition_y;
	int addition_texture;
	int list;
	Box()
	{
		direction = 0;
		type = 0;
		x = 0;
		y = 0;
		length = 0;
		texture = 0;
		/*objNumber = 0;
		addition_x = 0;
		addition_y = 0;
		addition_texture = 0;*/
		list = -1;
	}
	Box(int direction,int type, double x, double y, double length,int texture/*,int objNumber, double addition_x, double addition_y,int addition_texture*/)
	{
		this->direction = direction;
		this->type = type;
		this->x = x;
		this->y = y;
		this->length = length;
		this->texture = texture;
		/*this->objNumber = objNumber;
		this->addition_x = addition_x;
		this->addition_y = addition_y;
		this->addition_texture = addition_texture;*/
		this->list = -1;
	}
	~Box()
	{

	}
};
#endif