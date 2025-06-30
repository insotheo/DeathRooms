#include "game/map.h"

void gen_map(GEN_MAP_FUNC_ARGS){
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

    //gen room graph
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

    //placing tales
    std::unordered_map<IntPair, sf::Vector2f> room_world_pos;
    std::unordered_map<IntPair, std::vector<sf::FloatRect>> room_floors;

    const float spacing = 2.0f;

    for (const auto& [room, size] : room_size_map) {
        //world offset
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

    //doors and halls
    std::unordered_set<std::pair<int, int>, std::hash<std::pair<int, int>>> placed_corridor_tiles;
    auto round_to_tile = [&](float v, float tile_size) -> float {
        return std::floor(v / tile_size) * tile_size;
    };

    for (const auto& [room, size] : room_size_map) {
        sf::Vector2f centerA = {
            room_world_pos[room].x + tile_w * size.x / 2.0f,
            room_world_pos[room].y + tile_h * size.y / 2.0f
        };

        for (const IntPair& dir : directions) {
            IntPair neighbor = { room.x + dir.x, room.y + dir.y };
            if (!room_size_map.count(neighbor)) continue;

            sf::Vector2f centerB = {
                room_world_pos[neighbor].x + tile_w * room_size_map[neighbor].x / 2.0f,
                room_world_pos[neighbor].y + tile_h * room_size_map[neighbor].y / 2.0f
            };

            sf::Vector2f a = centerA;
            sf::Vector2f b = centerB;

            std::vector<sf::Vector2f> path;
            path.push_back(a);

            path.push_back({b.x, a.y});
            path.push_back(b);

            for (size_t i = 1; i < path.size(); ++i) {
                sf::Vector2f p0 = path[i - 1];
                sf::Vector2f p1 = path[i];

                int steps = 0;
                if (p0.x != p1.x) steps = std::abs((int)((p1.x - p0.x) / tile_w));
                else if (p0.y != p1.y) steps = std::abs((int)((p1.y - p0.y) / tile_h));

                int dx = (p1.x > p0.x) ? 1 : (p1.x < p0.x) ? -1 : 0;
                int dy = (p1.y > p0.y) ? 1 : (p1.y < p0.y) ? -1 : 0;

                for (int s = 0; s <= steps; ++s) {
                    float x = round_to_tile(p0.x + dx * s * tile_w, tile_w);
                    float y = round_to_tile(p0.y + dy * s * tile_h, tile_h);
                    sf::FloatRect floor_tile({x, y}, {tile_w, tile_h});

                    if (std::any_of(out_floors.begin(), out_floors.end(), [&](const sf::FloatRect& f) { return f.findIntersection(floor_tile); }))
                        continue;
                    out_floors.push_back(floor_tile);

                    sf::FloatRect neighbors[4] = {
                        {{x - tile_w, y}, {tile_w, tile_h}},
                        {{x + tile_w, y}, {tile_w, tile_h}},
                        {{x, y - tile_h}, {tile_w, tile_h}},
                        {{x, y + tile_h}, {tile_w, tile_h}}
                    };

                    for (const auto& nb : neighbors) {
                        bool already_wall_or_floor =
                            std::any_of(out_floors.begin(), out_floors.end(), [&](const sf::FloatRect& f) { return f.findIntersection(nb); }) ||
                            std::any_of(out_walls.begin(), out_walls.end(), [&](const sf::FloatRect& w) { return w.findIntersection(nb); });

                        if (!already_wall_or_floor) {
                            out_walls.push_back(nb);
                        }
                    }
                }
            }
        }
    }
}