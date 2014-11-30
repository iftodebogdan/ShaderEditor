// ShaderEditor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <gtkmm/main.h>
#include "HomeWindow.h"
#include "Renderer.h"

using namespace LibRendererDll;


int main(int argc, char* argv[])
{
	Gtk::Main kit(argc, argv);
	HomeWindow window;
	
	Gtk::Main::run(window);

	return 0;
}
