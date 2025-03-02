#include <math.h>
#include <glm/glm.hpp>

glm::vec3 hsv2rgb(float ha, float sa, float va) {
	float r, g, b;
	
	float h = ha / 360;
	float s = sa / 100;
	float v = va / 100;
	
	int i = floor(h * 6);
	float f = h * 6 - i;
	float p = v * (1 - s);
	float q = v * (1 - f * s);
	float t = v * (1 - (1 - f) * s);
	
	switch (i % 6) {
		case 0: r = v, g = t, b = p; break;
		case 1: r = q, g = v, b = p; break;
		case 2: r = p, g = v, b = t; break;
		case 3: r = p, g = q, b = v; break;
		case 4: r = t, g = p, b = v; break;
		case 5: r = v, g = p, b = q; break;
	}
	
	glm::vec3 color;
	color.x = r;
	color.y = g;
	color.z = b;
	
	return color;
}