#include"UNOGAME.h"
int main()
{
	UNOGAME Uno;
	while (Uno.window.isOpen())
	{
		Uno.Run();
	}
	return 0;
}