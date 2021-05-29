#pragma once

namespace env {
	//Resolution X
	constexpr unsigned resX = 1024;
	//Resolution Y
	constexpr unsigned resY = 1024;

	//Chunk size per sound
	constexpr size_t chunk = 2048;
	//Sound clock
	constexpr unsigned clock = 50;

	//Inner circle ring radius
	constexpr float inner = 300;
	//Outer circle ring addition
	constexpr float outer = 200;
	//Minimum radial ring resolution
	constexpr unsigned minResolution = 256;
	//Maximum radial elements
	constexpr unsigned maxResolution = 1024;

}