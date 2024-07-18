#include "Texture2D.hpp"
#include "SDL2/SDL_image.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <glad/glad.h>


Texture2D::Texture2D() : mTexture(0){

};

Texture2D::~Texture2D(){

};


bool Texture2D::loadTexture(const std::string& fileName, bool generateMipMaps)
{
    // Initialize SDL_image (if not done elsewhere in your code)
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        return false;
    }

    // Use SDL_image library to load our image
    SDL_Surface* imageSurface = IMG_Load(fileName.c_str());
    if (!imageSurface) {
        std::cerr << "Error loading texture '" << fileName << "': " << IMG_GetError() << std::endl;
        return false;
    }

    int width = imageSurface->w;
    int height = imageSurface->h;
    int components = imageSurface->format->BytesPerPixel;

    // Convert SDL_Surface to a format suitable for OpenGL
    SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(imageSurface, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(imageSurface); // Free the original surface
    if (!formattedSurface) {
        std::cerr << "Error converting surface to ABGR8888 format: " << SDL_GetError() << std::endl;
        return false;
    }

    // Invert image (if necessary)
    unsigned char* imageData = static_cast<unsigned char*>(formattedSurface->pixels);
    int widthInBytes = width * components;
    unsigned char* top = NULL;
    unsigned char* bottom = NULL;
    unsigned char temp = 0;
    int halfHeight = height / 2;
    for (int row = 0; row < halfHeight; row++) {
        top = imageData + row * widthInBytes;
        bottom = imageData + (height - row - 1) * widthInBytes;
        for (int col = 0; col < widthInBytes; col++) {
            temp = *top;
            *top = *bottom;
            *bottom = temp;
            top++;
            bottom++;
        }
    }

    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture); // All upcoming GL_TEXTURE_2D operations will affect our texture object (mTexture)

    // Set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    if (generateMipMaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    SDL_FreeSurface(formattedSurface); // Free the formatted surface
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done so we don't accidentally mess up our mTexture

    return true;
}


void Texture2D::bind(GLuint texUnit){
    glBindTexture(GL_TEXTURE_2D, mTexture);
};