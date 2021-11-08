#include "Texture.h"

Texture::Texture()
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = ""; 
}

Texture::Texture(const char* fileLoc)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
}

void Texture::LoadTexture()
{
	//1 char = 1 byte. This is essentially an array of bytes, which is used to represent data.
	//Used commonly to represent an image array, or a string as we've been using it for
	//this line loads in the image (texData means Texture Data)
	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);

	if (!texData)
	{
		printf("failed to find %s\n", fileLocation);
		return;
	}

	//generating a texture and applying our ID to it, very similar to how we did VAOs and VBOs
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//set some paramters (options?) for our texture like the clamping/tiling, and the filtering (linear vs nearest) 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//load data for image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D); //generate mipmaps automatically

	glBindTexture(GL_TEXTURE_2D, 0);


	stbi_image_free(texData);
}

//set active texture unit, then bind the textureID to the Texture 2D element 
void Texture::UseTexture()
{
	//GL_TEXTURE0 is a texture unit, there's a lot more, up to TEXTURE32 or more even.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::ClearTexture()
{
	//delete texture from memory 
	glDeleteTextures(1, &textureID);

	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}

Texture::~Texture()
{
	ClearTexture();
}