//**********************************************
//Singleton Texture Manager class
//Written by Ben English
//benjamin.english@oit.edu
//
//For use with OpenGL and the FreeImage library
//**********************************************

#include "TextureManager.h"
#include <GL\glu.h>
#include "MGDAssert.h"
#include "SDL_surface.h"
#include "SDL_image.h"
#include "..\Graphics\RendererFactory.h"
#include "..\Graphics\Renderer2D.h"

TextureManager* TextureManager::m_inst(0);

TextureManager* TextureManager::Inst()
{
	if(!m_inst)
		m_inst = new TextureManager();

	return m_inst;
}

TextureManager::TextureManager()
{
	// call this ONLY when linking with FreeImage as a static library
	#ifdef FREEIMAGE_LIB
		FreeImage_Initialise();
	#endif
}

//these should never be called
//TextureManager::TextureManager(const TextureManager& tm){}
//TextureManager& TextureManager::operator=(const TextureManager& tm){}
	
TextureManager::~TextureManager()
{
	// call this ONLY when linking with FreeImage as a static library
	#ifdef FREEIMAGE_LIB
		FreeImage_DeInitialise();
	#endif

	UnloadAllTextures();
	m_inst = 0;
}

void TextureManager::GetInfo(const char* filename, real& o_fWidth, real& o_fHeight)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP *dib(0);

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if(fif == FIF_UNKNOWN) 
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if(fif == FIF_UNKNOWN)
		return;

	//check that the plugin has reading capabilities and load the file
	if(FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	MGD_ASSERT(dib);
	if(!dib)
		return;

	o_fWidth = FreeImage_GetWidth(dib);
	o_fHeight = FreeImage_GetHeight(dib);

	FreeImage_Unload(dib);
}


bool TextureManager::SDL_GL_LoadTexture(const char* filename, const unsigned int texID)
{
	SDL_Surface* textureImage;
	GLuint gl_texID;
	textureImage = IMG_Load(filename);
	if(!textureImage)
	{
		printf("%s \n", SDL_GetError());
		printf("Couldn't load %s.\n" , filename);
		return false;
	}

	else
	{
		//Create the texture.
		if(m_texID.find(texID) != m_texID.end())
			glDeleteTextures(1, &(m_texID[texID]));

		//generate an OpenGL texture ID for this texture
		glGenTextures(1, &gl_texID);
		//store the texture ID mapping
		m_texID[texID] = gl_texID;
		//bind to the new texture ID
		glBindTexture(GL_TEXTURE_2D, gl_texID);


		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		GLint nofcolors= textureImage->format->BytesPerPixel;
		GLenum texture_format = NULL;
		//contains an alpha channel
		if(nofcolors==4)
		{
			if(textureImage->format->Rmask==0x000000ff)
				texture_format=GL_RGBA;
			else
				MGD_ASSERT(0);//texture_format=GL_BGRA;
		}
		else if(nofcolors==3) //no alpha channel
		{
			if(textureImage->format->Rmask==0x000000ff)
				texture_format=GL_RGB;
			else
				MGD_ASSERT(0);//texture_format=GL_BGR;
		}
		else
		{
			printf("warning: the image is not truecolor...this will break ");
		}

		//Generate the texture.
		glTexImage2D(GL_TEXTURE_2D , 0 , nofcolors , textureImage->w , 
			textureImage->h , 0 , texture_format , GL_UNSIGNED_BYTE , 
			textureImage->pixels);

		//SDL_SetTextureBlendMode(SDL_CreateTextureFromSurface(static_cast<Renderer2D*>(RendererFactory::GetRendererPtr())->m_pRenderer,textureImage),SDL_BLENDMODE_BLEND);

		//Free up the memory.
		if(textureImage)
			SDL_FreeSurface(textureImage);

		return true;
	}

}

bool TextureManager::LoadTexture(const char* filename, const unsigned int texID, GLenum image_format, GLint internal_format, GLint level, GLint border)
{
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);
	//OpenGL's image ID to map to
	GLuint gl_texID;
	
	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if(fif == FIF_UNKNOWN) 
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if(fif == FIF_UNKNOWN)
		return false;

	//check that the plugin has reading capabilities and load the file
	if(FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	MGD_ASSERT(dib);
	if(!dib)
		return false;

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if((bits == 0) || (width == 0) || (height == 0))
		return false;
	
	//if this texture ID is in use, unload the current texture
	if(m_texID.find(texID) != m_texID.end())
		glDeleteTextures(1, &(m_texID[texID]));

	//generate an OpenGL texture ID for this texture
	glGenTextures(1, &gl_texID);
	//store the texture ID mapping
	m_texID[texID] = gl_texID;
	//bind to the new texture ID
	glBindTexture(GL_TEXTURE_2D, gl_texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//store the texture data for OpenGL use
	glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height,
		border, image_format, GL_UNSIGNED_BYTE, bits);

//	gluBuild2DMipmaps ( GL_TEXTURE_2D, internal_format, width,
//		height, image_format, GL_UNSIGNED_BYTE, bits );

	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);

	//return success
	return true;
}

bool TextureManager::UnloadTexture(const unsigned int texID)
{
	bool result(true);
	//if this texture ID mapped, unload it's texture, and remove it from the map
	if(m_texID.find(texID) != m_texID.end())
	{
		glDeleteTextures(1, &(m_texID[texID]));
	}
	//otherwise, unload failed
	else
	{
		result = false;
	}

	return result;
}

bool TextureManager::BindTexture(const unsigned int texID)
{
	bool result(true);
	//if this texture ID mapped, bind it's texture as current
	if(m_texID.find(texID) != m_texID.end())
		glBindTexture(GL_TEXTURE_2D, m_texID[texID]);
	//otherwise, binding failed
	else
		result = false;

	return result;
}

void TextureManager::UnloadAllTextures()
{
	//start at the begginning of the texture map
	std::map<unsigned int, GLuint>::iterator i = m_texID.begin();

	//Unload the textures untill the end of the texture map is found
	while(i != m_texID.end())
	{
		if(UnloadTexture(i->first))
		{
			i = m_texID.erase(i);
		}
		else
		{
			++i;
		}
	}

	//clear the texture map
	m_texID.clear();
}