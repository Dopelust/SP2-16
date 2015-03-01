#include "inventory.h"

void Inventory::Init(Vector3 pos)
{
	position = pos;

	slotMeshes[0] = MeshBuilder::GenerateXYQuad("Inventory Slot", Color(1, 1, 1), 2.f, 2.f, 1); 
	slotMeshes[0]->textureID = LoadTGA("Image//Slot.tga");

	slotMeshes[1] = MeshBuilder::GenerateXYQuad("Inventory Slot", Color(1, 1, 1), 2.f, 2.f, 1); 
	slotMeshes[1]->textureID = LoadTGA("Image//SlotR.tga");

	slotMeshes[2] = MeshBuilder::GenerateXYQuad("Inventory Slot", Color(1, 1, 1), 2.f, 2.f, 1); 
	slotMeshes[2]->textureID = LoadTGA("Image//SlotG.tga");

	for (int i = 0; i < 9; i++)
	{
		slots[i].position = position; slots[i].position.x += (-4*2 + i*2);
		slots[i].mesh = slotMeshes[0];
	}

	selector.mesh =  MeshBuilder::GenerateXYQuad("Selector", Color(1, 1, 1), 2.3f, 2.3f, 1); 
	selector.mesh->textureID = LoadTGA("Image//Selector.tga");

	index = 0;
	selector.selectedSlot= &slots[index];
};

void Inventory::Update(double dt)
{
	wallet.Update();

	for (int i = 0; i < 9; i++)
	{
		if(!slots[i].item.empty())
		{
			if(slots[i].item[0]->getPaid())
				slots[i].mesh = slotMeshes[2];
			else
				slots[i].mesh = slotMeshes[1];
		}
		else
		{
			slots[i].mesh = slotMeshes[0];
		}
	}

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

	selector.selectedSlot = &slots[index];
}

float Inventory::checkPrice()
{
	float price = 0.f;

	for (int i = 0; i < 9; i++)
	{
		if(!slots[i].item.empty())
		{
			if (!slots[i].item[0]->getPaid())
			{
				for (int j = 0; j < slots[i].item.size(); j++)
				{
					price += slots[i].item[j]->getValue();
				}
			}
		}
	}

	return price;
}

bool Inventory::Checkout()
{
	if (checkPrice() > wallet.trueValue)
		return false;

	wallet.trueValue -= checkPrice();

	for (int i = 0; i < 9; i++)
	{
		if(!slots[i].item.empty())
		{
			for (int j = 0; j < slots[i].item.size(); j++)
			{
				slots[i].item[j]->setPaid(true);
			}
		}
	}

	for (int i = 8; i >= 0; i--)
	{
		if(!slots[i].item.empty())
		{
			for (int j = 0; j < 9; j++)
			{
				if (i != j && !slots[j].item.empty())
				if(slots[i].item[0]->mesh->name == slots[j].item[0]->mesh->name)
				{
					for (int k = slots[i].item.size() - 1; k >= 0; k--)
					{
						slots[j].item.push_back(slots[i].item[k]);
						slots[i].item.pop_back();
					}

					break;
				}
			}
		}
	}

	return true;
}

bool Inventory::Insert(Object* o)
{
	for (int i = 0; i < 9; i++)
	{
		if(!slots[i].item.empty())
			if(slots[i].item[0]->mesh->name == o->mesh->name && slots[i].item[0]->getPaid() == o->getPaid())
			{
				slots[i].item.push_back(o);
				return true;
			}
	}

	if (emptySlot() >= 0)
	{
		slots[emptySlot()].item.push_back(o);
		return true;
	}

	return false;
}

bool Inventory::Remove()
{
	if (!selector.selectedSlot->item.empty())
	{
		selector.selectedSlot->item.pop_back();
		return true;
	}

	return false;
}

bool Inventory::canUse()
{
	if(!selector.selectedSlot->item.empty())
		if(selector.selectedSlot->item[0]->getPaid())
		{
			return true;
		}

	return false;
}

bool Inventory::checkPaid()
{
	for (int i = 0; i < 9; i++)
	{
		if(!slots[i].item.empty())
			if(!slots[i].item[0]->getPaid())
			{
				return false;
			}
	}

	return true;
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

void Wallet::Update()
{
	if (apparentValue < trueValue)
		apparentValue++;
	else if (apparentValue > trueValue)
		apparentValue--;
	else
		apparentValue = trueValue;

}