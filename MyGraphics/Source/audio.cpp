#include "PROJECTScene.h"
#include "MENUScene.h"

ISoundEngine * engine;

int PROJECTScene::soundInit()
{
	engine = createIrrKlangDevice();

   if (!engine)
      return 0;

   return 0;
}

int MENUScene::soundInit()
{
	engine = createIrrKlangDevice();

   if (!engine)
      return 0;

   return 0;
}


int PROJECTScene::soundUpdate(Player player)
{
	Vector3 direction;
	direction.SphericalToCartesian(player.hOrientation - 180, player.vOrientation);

	engine->setListenerPosition(vec3df(player.position.x,player.position.y,player.position.z), vec3df(direction.x, direction.y, direction.z));

	return 0;
}
