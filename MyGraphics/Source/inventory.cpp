#include "inventory.h"

void Inventory::Init(Vector3 pos)
{
	position = pos;

	for (int i = 0; i < 9; i++)
	{
		slots[i].mesh = MeshBuilder::GenerateXYQuad("Inventory Slot", Color(1, 1, 1), 2.f, 2.f, 1); 
		slots[i].mesh->textureID = LoadTGA("Image//Slot.tga");
		slots[i].position = position; slots[i].position.x += (-4*2 + i*2);
	}

	selector = new Slot;
	selector->mesh =  MeshBuilder::GenerateXYQuad("Selector", Color(1, 1, 1), 2.3f, 2.3f, 1); 
	selector->mesh->textureID = LoadTGA("Image//Selector.tga");

	index = 0;
	selector->position = slots[index].position; selector->position.z++;
};

void Inventory::Update(double dt)
{
	if (Application::mouseScroll >= 1)
	{
		if (index == 0)
			index = 8;
		else
			index--;
	}

	if (Application::mouseScroll <= -1)
	{
		if (index == 8)
			index = 0;
		else
			index++;
	}

	if (Application::IsKeyPressed('1'))
		index = 0;
	if (Application::IsKeyPressed('2'))
		index = 1;
	if (Application::IsKeyPressed('3'))
		index = 2;
	if (Application::IsKeyPressed('4'))
		index = 3;
	if (Application::IsKeyPressed('5'))
		index = 4;
	if (Application::IsKeyPressed('6'))
		index = 5;
	if (Application::IsKeyPressed('7'))
		index = 6;
	if (Application::IsKeyPressed('8'))
		index = 7;
	if (Application::IsKeyPressed('9'))
		index = 8;

	selector->position = slots[index].position;
}

void Inventory::Insert(Object* o)
{
	for (int i = 0; i < 9; i++)
	{
		if(!slots[i].item.empty())
			if(slots[i].item[0]->mesh->name == o->mesh->name)
			{
				slots[i].item.push_back(o);
				return;
			}
	}

	slots[emptySlot()].item.push_back(o);
}

int Inventory::emptySlot()
{
	for (int i = 0; i < 9; i++)
	{
		if (slots[i].item.empty())
			return i;
	}

	return -1;
}