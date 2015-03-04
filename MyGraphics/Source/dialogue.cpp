#include "dialogue.h"
#include "NPC.h"

#include <fstream>

Vector3 TextBox::position = Vector3(0,-9.25f,0);

int frame = 0;
extern Vector3 Select;

void TextBox::Update()
{
	if (frame == 0)
		frame = 1;
	else
		frame = 0;

	if (apparentext.size() < text.size())
	{
		if (frame == 0)
			apparentext += text[apparentext.size()];

		if (apparentext.size() == text.size())
			Select = getAccept().position;
	}	
}

void NPC::InitDialogue(const char* filename)
{
	ifstream inData;
	string data;

	inData.open (filename); 

	while (!inData.eof()) {

		getline (inData, data);

		if (data == "")
			continue;

		greetings.push_back(TextBox(data));
	}

	inData.close (); 
}

void NPC::InitClues(const char* filename)
{
	ifstream inData;
	string data;

	inData.open (filename); 

	while (!inData.eof()) {

		getline (inData, data);

		if (data == "")
			continue;

		clues.push_back(TextBox(data));
	}

	inData.close (); 
}


void NPC::InitQuest(const char* filename)
{
	int count = 0;

	ifstream inData;
	string data;

	inData.open (filename); 

	while (!inData.eof()) {

		getline (inData, data);

		if (data == "")
			continue;

		if (count == 0)
			quest = new Quest(data);
		else if (count == 1)
			quest->Accept.next = new TextBox(data);
		else if (count == 2)
			quest->Decline.next = new TextBox(data);
		else if (count == 3)
			quest->Accept.altNext = new TextBox(data);
		count++;
	}

	inData.close (); 
}

void Hobo::InitQuest(const char* filename)
{
	int count = 0;

	ifstream inData;
	string data;

	inData.open (filename); 

	while (!inData.eof()) {

		getline (inData, data);

		if (data == "")
			continue;

		if (count == 0)
			quest = new Donation(data);
		else if (count == 1)
			quest->Accept.next = new TextBox(data);
		else if (count == 2)
			quest->Decline.next = new TextBox(data);
		else if (count == 3)
			quest->Accept.altNext = new TextBox(data);
		count++;
	}

	inData.close (); 
}

void Cashier::InitQuest(const char* filename)
{
	int count = 0;

	ifstream inData;
	string data;

	inData.open (filename); 

	while (!inData.eof()) {

		getline (inData, data);

		if (data == "")
			continue;

		if (count == 0)
			quest = new Checkout(data);
		else if (count == 1)
			quest->Accept.next = new TextBox(data);
		else if (count == 2)
			quest->Decline.next = new TextBox(data);
		else if (count == 3)
			quest->Accept.altNext = new TextBox(data);
		count++;
	}

	inData.close (); 
}