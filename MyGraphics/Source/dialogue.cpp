#include "dialogue.h"
#include "NPC.h"

#include <fstream>

Vector3 TextBox::position = Vector3(0,-7.75f,0);

void NPC::InitDialogue(const char* filename)
{
	ifstream inData;
	string data;

	inData.open (filename); 

	while (!inData.eof()) {

		getline (inData, data);

		if (data == "")
			continue;

		greetings.push_back(TextBox(data, bodyParts[HEAD].mesh));
	}

	inData.close (); 
}

void Manager::InitDialogue()
{
	ifstream inData;
	string data;

	inData.open ("Filestream//manager.txt"); 

	while (!inData.eof()) {

		getline (inData, data);

		if (data == "")
			continue;

		greetings.push_back(TextBox(data, bodyParts[HEAD].mesh));
	}

	inData.close (); 
}