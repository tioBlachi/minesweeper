//
// Created by Blas Antunez on 8/9/24.
//

#include "TextureManager.h"

// "redeclare" the variable, so this file knows that is exists
unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::LoadTexture(string filename)
{
	string path = "../images/";
	path += filename + ".png";

	// EXAMPLE
	// filename == space
	// path == images/space.png
	textures[filename].loadFromFile(path);
}

sf::Texture& TextureManager::GetTexture(string textureName)
{
	// if the texture does not exist...
	if (textures.find(textureName) == textures.end()) // we did not find it
		// load it first? Then return it?
		LoadTexture(textureName);
	
	return textures[textureName];
}

void TextureManager::Clear()
{
	textures.clear(); // get rid of all stored objects in unordered map
}