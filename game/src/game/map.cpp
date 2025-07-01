#include "game/map.h"

void gen_map(GEN_MAP_FUNC_ARGS) {
    out_walls.clear();
    out_floors.clear();

    const sf::FloatRect wall_bounds = wall_sprite.getGlobalBounds();
    const sf::FloatRect floor_bounds = floor_sprite.getGlobalBounds();
    const float tile_w = wall_bounds.size.x;
    const float tile_h = wall_bounds.size.y;

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dir_dist(0, 3);
    std::uniform_int_distribution<size_t> room_size_dist(0, room_sizes.size() - 1);

    std::vector<IntPair> directions = {{1,0}, {-1,0}, {0,1}, {0,-1}};
    std::unordered_map<IntPair, IntPair> room_size_map;
    std::unordered_set<IntPair> room_positions;
    std::stack<IntPair> room_stack;

    room_stack.push({0, 0});
    room_positions.insert({0, 0});
    room_size_map[{0, 0}] = room_sizes[room_size_dist(rng)];

    while (room_positions.size() < rooms_count && !room_stack.empty()) {
        IntPair current = room_stack.top();
        room_stack.pop();

        std::shuffle(directions.begin(), directions.end(), rng);
        for (const IntPair& dir : directions) {
            IntPair next = { current.x + dir.x, current.y + dir.y };
            if (room_positions.count(next)) continue;

            room_positions.insert(next);
            room_stack.push(next);
            room_size_map[next] = room_sizes[room_size_dist(rng)];
            break;
        }
    }

    std::unordered_map<IntPair, sf::Vector2f> room_world_pos;
    std::unordered_map<IntPair, std::vector<sf::FloatRect>> room_floors;
    const float spacing = 2.0f;

    for (const auto& [room, size] : room_size_map) {
        sf::Vector2f base_pos = {
            room.x * (size.x * tile_w + spacing * tile_w),
            room.y * (size.y * tile_h + spacing * tile_h)
        };
        room_world_pos[room] = base_pos;

        for (int y = 0; y < size.y; ++y) {
            for (int x = 0; x < size.x; ++x) {
                float px = base_pos.x + x * tile_w;
                float py = base_pos.y + y * tile_h;
                sf::FloatRect rect({px, py}, {tile_w, tile_h});

                bool edge = x == 0 || y == 0 || x == size.x - 1 || y == size.y - 1;
                if (edge)
                    out_walls.push_back(rect);
                else {
                    out_floors.push_back(rect);
                    room_floors[room].push_back(rect);
                }
            }
        }
    }

    std::unordered_set<std::pair<int, int>, std::hash<std::pair<int, int>>> placed_corridor_tiles;

    for (const auto& [room, size] : room_size_map) {
        sf::Vector2f centerA = {
            room_world_pos[room].x + (size.x / 2) * tile_w,
            room_world_pos[room].y + (size.y / 2) * tile_h
        };

        for (const IntPair& dir : directions) {
            IntPair neighbor = { room.x + dir.x, room.y + dir.y };
            if (!room_size_map.count(neighbor)) continue;

            const auto& neighbor_size = room_size_map[neighbor];
            sf::Vector2f centerB = {
                room_world_pos[neighbor].x + (neighbor_size.x / 2) * tile_w,
                room_world_pos[neighbor].y + (neighbor_size.y / 2) * tile_h
            };

            std::vector<sf::Vector2f> path = {centerA, {centerB.x, centerA.y}, centerB};

            for (size_t i = 1; i < path.size(); ++i) {
                sf::Vector2f p0 = path[i - 1];
                sf::Vector2f p1 = path[i];

                int dx = (p1.x > p0.x) ? 1 : (p1.x < p0.x) ? -1 : 0;
                int dy = (p1.y > p0.y) ? 1 : (p1.y < p0.y) ? -1 : 0;
                int steps = (dx != 0) ? std::abs((int)((p1.x - p0.x) / tile_w)) :
                            std::abs((int)((p1.y - p0.y) / tile_h));

                for (int s = 0; s <= steps; ++s) {
                    float x = p0.x + dx * s * tile_w;
                    float y = p0.y + dy * s * tile_h;
                    sf::FloatRect tile({x, y}, {tile_w, tile_h});

                    out_walls.erase(
                        std::remove_if(out_walls.begin(), out_walls.end(),
                            [&](const sf::FloatRect& wall) {
                                return wall.findIntersection(tile);
                            }),
                        out_walls.end());

                    auto tile_coord = std::make_pair((int)(x / tile_w), (int)(y / tile_h));
                    if (!placed_corridor_tiles.count(tile_coord)) {
                        out_floors.push_back(tile);
                        placed_corridor_tiles.insert(tile_coord);

                        sf::FloatRect neighbors[4] = {
                            {{x - tile_w, y}, {tile_w, tile_h}},
                            {{x + tile_w, y}, {tile_w, tile_h}},
                            {{x, y - tile_h}, {tile_w, tile_h}},
                            {{x, y + tile_h}, {tile_w, tile_h}}
                        };

                        for (const auto& nb : neighbors) {
                            auto nb_coord = std::make_pair((int)(nb.position.x / tile_w), (int)(nb.position.y / tile_h));
                            bool is_occupied =
                                placed_corridor_tiles.count(nb_coord) ||
                                std::any_of(out_floors.begin(), out_floors.end(),
                                    [&](const sf::FloatRect& f) { return f.findIntersection(nb); }) ||
                                std::any_of(out_walls.begin(), out_walls.end(),
                                    [&](const sf::FloatRect& w) { return w.findIntersection(nb); });

                            if (!is_occupied) {
                                out_walls.push_back(nb);
                            }
                        }
                    }
                }
            }
        }
    }
}
