#include "ImageRGBA.h"

#include "stb_image_write.h"
#include "stb_image.h"

ImageRGBA::ImageRGBA(std::string filename){
	int nrChannels;
	stbi_set_flip_vertically_on_load(true);
	RGBA* data = (RGBA*)stbi_load(filename.c_str(), &_width, &_height, &nrChannels, 4);
	if(data){
		pixels.assign(data, data+_width*_height);
		stbi_image_free(data);
	}
}
