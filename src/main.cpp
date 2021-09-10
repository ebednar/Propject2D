#include "sandbox.h"
#include <time.h>

int		main(void)
{
	Engine	engine;
	Sandbox	sandbox;
	
	srand(time(NULL));

	std::vector<int>* t = new std::vector<int>;
	auto p = t->begin();

	engine.init_engine(1280, 720);
	sandbox.place_ent(&engine);
	engine.run_engine();
	return 0;
}