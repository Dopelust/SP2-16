/******************************************************************************/
/*!
\file	audio.cpp
\author SP Team 16
\par	
\brief
this is for the audio
*/
/******************************************************************************/
#include "PROJECTScene.h"
#include "MENUScene.h"

ISoundEngine * engine;

/******************************************************************************/
/*!
		Class PROJECTScene::soundInit
\brief	the sound initializer for the Project scene
*/
/******************************************************************************/
int PROJECTScene::soundInit()
{
	engine = createIrrKlangDevice();

   if (!engine)
      return 0;

   return 0;
}

/******************************************************************************/
/*!
		Class MENUScene::soundInit
\brief	the sound initializer for the menu scene
*/
/******************************************************************************/
int MENUScene::soundInit()
{
	engine = createIrrKlangDevice();

   if (!engine)
      return 0;

   return 0;
}

/******************************************************************************/
/*!
		Class PROJECTScene::soundUpdate
\brief	This is the sound update for playing diff sounds
*/
/******************************************************************************/
int PROJECTScene::soundUpdate(Player player)
{
	Vector3 direction;
	direction.SphericalToCartesian(player.hOrientation - 180, player.vOrientation);

	engine->setListenerPosition(vec3df(player.position.x,player.position.y,player.position.z), vec3df(direction.x, direction.y, direction.z));

	return 0;
}
