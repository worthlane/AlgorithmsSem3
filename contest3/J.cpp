#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <deque>
#include <queue>
#include <unordered_set>

#define ll long long

struct Path
{
    std::string condition;
    std::string path;
};

struct Result
{
    ll          len;
    std::string path;
};

const size_t      FIELD_SIZE  = 3;
const size_t      FIELD_SIZE2 = FIELD_SIZE * FIELD_SIZE;
const std::string ANSWER     = "123456780";

const char MOVE[]    = {'L', 'R', 'U', 'D'};
const ll   DELTA_X[] = {-1,   1,   0,   0};
const ll   DELTA_Y[] = { 0,   0,  -1,   1};

Result bfs(std::string& start)
{
    if (start == ANSWER)
        return {0, ""};

    std::unordered_set<std::string> visited;
    std::queue<Path> q;
    q.push({start, ""});

    while (!q.empty())
    {
        Path element = q.front();
        q.pop();

        std::string current_state = element.condition;
        std::string current_path = element.path;

        size_t free_cell = current_state.find('0');
        size_t free_y    = free_cell / FIELD_SIZE;
        size_t free_x    = free_cell % FIELD_SIZE;

        for (size_t delta = 0; delta < 4; delta++)
        {
            size_t new_x = DELTA_X[delta] + free_x;
            size_t new_y = DELTA_Y[delta] + free_y;

            if (new_y < 0 || new_y >= FIELD_SIZE)
                continue;

            if (new_x < 0 || new_x >= FIELD_SIZE)
                continue;

            size_t next_cell = new_y * FIELD_SIZE + new_x;

            std::string next_state = current_state;
            std::swap(next_state[free_cell], next_state[next_cell]);

            Path next = {next_state, current_path + MOVE[delta]};

            if (next_state == ANSWER)
                return {static_cast<ll>(next.path.length()), next.path};

            if (visited.find(next_state) == visited.end())
            {
                visited.insert(next_state);
                q.push(next);
            }
        }
    }

    return {-1, ""};
}

int main()
{
    std::string start;

    for (size_t i = 0; i < FIELD_SIZE2; i++)
    {
        int a = 0;

        std::cin >> a;

        start += '0' + a;
    }

    Result ans = bfs(start);

    std::cout << ans.len << std::endl;
    std::cout << ans.path << std::endl;

    return 0;
}





