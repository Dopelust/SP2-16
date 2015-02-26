#include "dialogue.h"
#include "NPC.h"

#include <fstream>

Vector3 TextBox::position = Vector3(0,-7.75f,0);

void Detective::InitDialogue()
{
	ifstream inData;
	string data;

	inData.open ("Filestream//detective.txt"); 

	while (!inData.eof()) {

		getline (inData, data);

		if (data == "")
			continue;

		greetings.push_back(TextBox(data, bodyParts[HEAD].mesh));
	}

	inData.close (); 
}
void Cashier::InitDialogue()
{
	ifstream inData;
	string data;

	inData.open ("Filestream//cashier.txt"); 

	while (!inData.eof()) {

		getline (inData, data);

		if (data == "")
			continue;

		greetings.push_back(TextBox(data, bodyParts[HEAD].mesh));
	}

	inData.close (); 
}
void Hobo::InitDialogue()
{
	ifstream inData;
	string data;

	inData.open ("Filestream//hobo.txt"); 

	while (!inData.eof()) {

		getline (inData, data);

		if (data == "")
			continue;

		greetings.push_back(TextBox(data, bodyParts[HEAD].mesh));
	}

	inData.close (); 
}
void Thug::InitDialogue()
{
	ifstream inData;
	string data;

	inData.open ("Filestream//thug.txt"); 

	while (!inData.eof()) {

		getline (inData, data);

		if (data == "")
			continue;

		greetings.push_back(TextBox(data, bodyParts[HEAD].mesh));
	}

	inData.close (); 
}
void Blindman::InitDialogue()
{
	ifstream inData;
	string data;

	inData.open ("Filestream//blindman.txt"); 

	while (!inData.eof()) {

		getline (inData, data);

		if (data == "")
			continue;

		greetings.push_back(TextBox(data, bodyParts[HEAD].mesh));
	}

	inData.close (); 
}
void Customer::InitDialogue()
{
	ifstream inData;
	string data;

	inData.open ("Filestream//customer.txt"); 

	while (!inData.eof()) {

		getline (inData, data);

		if (data == "")
			continue;

		greetings.push_back(TextBox(data, bodyParts[HEAD].mesh));
	}

	inData.close (); 
}

void S_Guard::InitDialogue()
{
	ifstream inData;
	string data;

	inData.open ("Filestream//security.txt"); 

	while (!inData.eof()) {

		getline (inData, data);

		if (data == "")
			continue;

		greetings.push_back(TextBox(data, bodyParts[HEAD].mesh));
	}

	inData.close (); 
}