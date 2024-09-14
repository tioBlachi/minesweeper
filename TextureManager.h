//
// Created by Blas Antunez on 8/9/24.
//

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
using std::unordered_map;
using std::string;

class TextureManager
{
	// static means that there is one and only one of these in memory, ever
	static unordered_map<string, sf::Texture> textures;
public:
	static sf::Texture& GetTexture(string textureName);
	static void LoadTexture(string textureName); // LoadTexture("space")
	static void Clear(); // call; this once at the end of main()
};
#endif //TEXTUREMANAGER_H
