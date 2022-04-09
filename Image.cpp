#include "Image.h"

#if _MSC_VER && !__INTEL_COMPILER
 #define STBI_MSC_SECURE_CRT
#endif

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image::Image(std::string filename){
	int nrChannels;
	stbi_set_flip_vertically_on_load(true);
	Color* image = (Color*)stbi_load(filename.c_str(), &_width, &_height, &nrChannels, 3);
	if(image){
		pixels.assign(image, image+_width*_height);
		stbi_image_free(image);
	}
}
	

void Image::savePNG(std::string filename){
	stbi_flip_vertically_on_write(true);
	stbi_write_png(filename.c_str(), _width, _height, 3, pixels.data(), 0);
}

void Image::saveBMP(std::string filename){
	stbi_flip_vertically_on_write(true);
	stbi_write_bmp(filename.c_str(), _width, _height, 3, pixels.data());
}


