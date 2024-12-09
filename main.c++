#include <SFML/Graphics.hpp>
#include <vector>
#include <stack>
#include <string>

// تعريف كلاس Cell
class Cell {
private:
    bool generated = false;
    std::vector<std::string> neighbor;

public:
    Cell() = default;

    bool isGenerated() const {
        return generated;
    }

    void setGenerated() {
        generated = true;
    }

    void setNeighbor(const std::string& direction) {
        neighbor.push_back(direction);
    }

    const std::vector<std::string>& getNeighbors() const {
        return neighbor;
    }
};

// تعريف كلاس Maze
class Maze {
private:
    int mazeSize;
    int cellSize;
    std::vector<Cell> maze;

    Cell& at(int x, int y) {
        return maze[y * mazeSize + x];
    }

public:
    Maze(int mazeSize, int cellSize) : mazeSize(mazeSize), cellSize(cellSize) {
        maze.resize(mazeSize * mazeSize);
    }

    void generate() {
        int visited = 0;
        std::stack<std::pair<int, int>> stack;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, mazeSize - 1);

        int x = dist(gen);
        int y = dist(gen);
        stack.push({x, y});
        at(x, y).setGenerated();
        visited++;

        while (visited < mazeSize * mazeSize) {
            x = stack.top().first;
            y = stack.top().second;

            std::vector<std::string> neighbors;

            if (x > 0 && !at(x - 1, y).isGenerated())
                neighbors.push_back("left");
            if (x < mazeSize - 1 && !at(x + 1, y).isGenerated())
                neighbors.push_back("right");
            if (y > 0 && !at(x, y - 1).isGenerated())
                neighbors.push_back("up");
            if (y < mazeSize - 1 && !at(x, y + 1).isGenerated())
                neighbors.push_back("down");

            if (!neighbors.empty()) {
                std::uniform_int_distribution<> neighborDist(0, neighbors.size() - 1);
                std::string nextMove = neighbors[neighborDist(gen)];

                if (nextMove == "left") {
                    at(x, y).setNeighbor("left");
                    at(x - 1, y).setNeighbor("right");
                    at(x - 1, y).setGenerated();
                    stack.push({x - 1, y});
                } else if (nextMove == "right") {
                    at(x, y).setNeighbor("right");
                    at(x + 1, y).setNeighbor("left");
                    at(x + 1, y).setGenerated();
                    stack.push({x + 1, y});
                } else if (nextMove == "up") {
                    at(x, y).setNeighbor("up");
                    at(x, y - 1).setNeighbor("down");
                    at(x, y - 1).setGenerated();
                    stack.push({x, y - 1});
                } else if (nextMove == "down") {
                    at(x, y).setNeighbor("down");
                    at(x, y + 1).setNeighbor("up");
                    at(x, y + 1).setGenerated();
                    stack.push({x, y + 1});
                }
                visited++;
            } else {
                stack.pop();
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        sf::RectangleShape cellShape(sf::Vector2f(cellSize, cellSize));
        cellShape.setFillColor(sf::Color::White);
        cellShape.setOutlineThickness(1);
        cellShape.setOutlineColor(sf::Color::Black);

        for (int y = 0; y < mazeSize; y++) {
            for (int x = 0; x < mazeSize; x++) {
                cellShape.setPosition(x * cellSize, y * cellSize);
                window.draw(cellShape);

                Cell& cell = at(x, y);
                if (std::find(cell.getNeighbors().begin(), cell.getNeighbors().end(), "left") == cell.getNeighbors().end()) {
                    sf::Vertex line[] = {
                        sf::Vertex(sf::Vector2f(x * cellSize, y * cellSize)),
                        sf::Vertex(sf::Vector2f(x * cellSize, (y + 1) * cellSize))
                    };
                    window.draw(line, 2, sf::Lines);
                }
                if (std::find(cell.getNeighbors().begin(), cell.getNeighbors().end(), "right") == cell.getNeighbors().end()) {
                    sf::Vertex line[] = {
                        sf::Vertex(sf::Vector2f((x + 1) * cellSize, y * cellSize)),
                        sf::Vertex(sf::Vector2f((x + 1) * cellSize, (y + 1) * cellSize))
                    };
                    window.draw(line, 2, sf::Lines);
                }
                if (std::find(cell.getNeighbors().begin(), cell.getNeighbors().end(), "up") == cell.getNeighbors().end()) {
                    sf::Vertex line[] = {
                        sf::Vertex(sf::Vector2f(x * cellSize, y * cellSize)),
                        sf::Vertex(sf::Vector2f((x + 1) * cellSize, y * cellSize))
                    };
                    window.draw(line, 2, sf::Lines);
                }
                if (std::find(cell.getNeighbors().begin(), cell.getNeighbors().end(), "down") == cell.getNeighbors().end()) {
                    sf::Vertex line[] = {
                        sf::Vertex(sf::Vector2f(x * cellSize, (y + 1) * cellSize)),
                        sf::Vertex(sf::Vector2f((x + 1) * cellSize, (y + 1) * cellSize))
                    };
                    window.draw(line, 2, sf::Lines);
                }
            }
        }
    }
};

int main() {
    const int mazeSize = 20;
    const int cellSize = 25;
    const int windowSize = mazeSize * cellSize;

    sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "Maze Generator");

    Maze maze(mazeSize, cellSize);
    maze.generate();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::White);
        maze.draw(window);
        window.display();
    }

    return 0;
}
