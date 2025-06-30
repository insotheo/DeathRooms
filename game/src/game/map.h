#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_set>
#include <stack>
#include <random>
#include <algorithm>

struct IntPair {
    int x, y;
    bool operator==(const IntPair& other) const {
        return x == other.x && y == other.y;
    }
};

namespace std {
    template<>
    struct hash<IntPair> {
        size_t operator()(const IntPair& v) const {
            return hash<int>()(v.x) ^ (hash<int>()(v.y) << 1);
        }
    };

    template <>
    struct hash<std::pair<int, int>> {
        std::size_t operator()(const std::pair<int, int>& p) const noexcept {
            std::size_t h1 = std::hash<int>{}(p.first);
            std::size_t h2 = std::hash<int>{}(p.second);
            return h1 ^ (h2 << 1);
        }
    };
}


#define GEN_MAP_FUNC_ARGS size_t rooms_count, const sf::Sprite& wall_sprite, std::vector<sf::FloatRect>& out_walls, const sf::Sprite& floor_sprite, std::vector<sf::FloatRect>& out_floors, const std::vector<IntPair>& room_sizes
void gen_map(GEN_MAP_FUNC_ARGS);

#endif