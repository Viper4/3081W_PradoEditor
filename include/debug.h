#ifndef DEBUG_H
#define DEBUG_H
#include <string>
#include <artwork.h>

class Debug {
public:
	static void initializeConsole();
	static void printArtwork(const Artwork& art, const bool verbose);
	static void printArtworkMap(const std::unordered_map<std::string, Artwork>& map, const std::string& label, const int count);
	static void printArtworkVector(const std::vector<Artwork>& vector, const std::string& label, const int count);
};

#endif // DEBUG_H
