#include "sandbox.h"
#include <time.h>

int		main(void)
{
	Engine	engine;
	Sandbox	sandbox;
	
	srand(time(NULL));
	engine.init_engine("Untitled", 1280, 720);
	sandbox.eng = &engine;
	sandbox.place_ent();
	engine.run_engine();
	return 0;
}
