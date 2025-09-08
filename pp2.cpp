#include <iostream>
#include <list>
#include <vector>
#include <stdexcept>
#include <limits>

typedef unsigned int uint;
typedef unsigned int Vertex;

class GraphAl
{
private:
    uint num_vertices;
    uint num_edges;
    std::list<Vertex> *adj;

public:
    GraphAl(uint num_vertices)
        : num_vertices(num_vertices), num_edges(0)
    {
        adj = new std::list<Vertex>[num_vertices];
    }

    ~GraphAl()
    {
        delete[] adj;
        adj = nullptr;
    }

    void add_edge(const Vertex &u, const Vertex &v)
    {
        if (u < 0 || v < 0 || u > num_vertices || v > num_vertices || u == v)
            throw std::invalid_argument("valores de vertices invalidos");

        adj[u].push_back(v);
        adj[v].push_back(u);
        num_edges++;
    }

    const std::list<Vertex> &get_adj(const Vertex &u) const
    {
        if (u < 0 || u > num_vertices)
            throw std::invalid_argument("Vertice invalido");

        return adj[u];
    }

    uint get_num_vertices() const
    {
        return num_vertices;
    }

    uint get_num_edges() const
    {
        return num_edges;
    }
};

class Queue
{
private:
    std::list<Vertex> list;

public:
    Queue() {}

    void enqueue(Vertex v)
    {
        list.push_back(v);
    }

    void dequeue()
    {
        list.pop_front();
    }

    Vertex front()
    {
        return list.front();
    }

    bool empty()
    {
        return list.empty();
    }
};

class BFS
{
private:
    enum class Color
    {
        white,
        gray,
        black
    };
    GraphAl &g;
    std::vector<Color> color;
    std::vector<uint> dist;
    std::vector<int> pred;

public:
    BFS(GraphAl &graph)
        : g(graph),
          color(graph.get_num_vertices(), Color::white),
          dist(graph.get_num_vertices(), std::numeric_limits<uint>::max()),
          pred(graph.get_num_vertices(), -1) {}

    uint get_distance(Vertex v)
    {
        return dist[v];
    }
    void processBFS(Vertex s)
    {
        uint n = g.get_num_vertices();

        for (Vertex u = 0; u < n; u++)
        {
            color[u] = Color::white;                    // pinta todos os vertices de branco
            dist[u] = std::numeric_limits<uint>::max(); // considera primeiramente distancia infinita
            pred[u] = -1;                               // predecessor "nulo"
        }

        color[s] = Color::gray; // pinta a origem de cinza
        dist[s] = 0;
        pred[s] = -1;

        Queue Q;

        Q.enqueue(s); // cria uma fila e bota o vertice de origem

        while (!Q.empty()) // loop enquanto a fila nao estiver vazia
        {
            Vertex u = Q.front();              // analisa o vertice da frente na fila
            Q.dequeue();                       // tira o primeiro da fila

            std::list<Vertex> list_adj = g.get_adj(u); // pega a lista de adjacentes do vertice u

            // Selection Sort para ordenar a lista em ordem crescente
            for (auto atual = list_adj.begin(); atual != list_adj.end(); ++atual) {
                auto menor = atual;
                auto proximo = atual;
                ++proximo;
                for (; proximo != list_adj.end(); ++proximo) {
                    if (*proximo < *menor) {
                        menor = proximo;
                    }
                }
                if (menor != atual) {
                    int temp = *atual;
                    *atual = *menor;
                    *menor = temp;
                }
            }

            for (const auto &v : list_adj) // loop para pegar os vertices adjacentes do vertice u que estava na frente da fila
            {
                if (color[v] == Color::white) // se for branco (ainda nao encontrado)
                {
                    color[v] = Color::gray; // pinta de cinza, aumenta o numero da distancia e adiciona o predecessor
                    dist[v] = dist[u] + 1;
                    pred[v] = u;
                    Q.enqueue(v); // bote os vertices na fila
                }

                color[u] = Color::black; // depois de analisar todos os adjacentes do vertice, pinte de preto
            }
        }
    }
};

class HorsesAttack
{
private:
    GraphAl g;

    void horses_paths(GraphAl &g)
    {
        int L_moves[8][2] =
            {
                {-1, -2}, {1, -2}, {-2, -1}, {2, -1}, // matriz com todas as possibilidades de movimento em L do cavalo
                {-2, 1}, {2, 1}, {-1, 2}, {1, 2},
            };

        std::string initial_position = "a1";

        while (initial_position <= "h8") // percorrer de 'a1' ate 'h8'
        {
            int initial_row = initial_position[0] - 'a';  // as colunas de 'a' a 'h' sendo 'a'-> 0 e 'h'-> 7
            int initial_line = initial_position[1] - '1'; // as linhas de 1 a 8, diminuido em 1

            Vertex initial_vertice = initial_line * 8 + initial_row; // vertice equivalente, sendo 'a1'-> 0 e 'h8' -> 63

            for (const auto &possible_moves : L_moves) // percorre todos os movimentos que o cavalo pode possivelmente fazer
            {
                int new_line = initial_line + possible_moves[1];                  // novo numero da casa
                int new_row = initial_row + possible_moves[0];                    // nova letra da coluna
                if (new_line >= 0 && new_line < 8 && new_row >= 0 && new_row < 8) // verifica a possibilidade do novo movimento
                {
                    Vertex new_vertice = new_line * 8 + new_row; // novo vertice apos movimento em L
                    if (initial_vertice < new_vertice)           // adiciona aresta sem duplicar
                    {
                        g.add_edge(initial_vertice, new_vertice);
                    }
                }
            }
            if (initial_position[1] < '8')
                initial_position[1]++; // incrementa o numero, de 'a1' para 'a2'
            else
            {
                initial_position[1] = '1';
                initial_position[0]++; // ao chegar no numero 8, aumenta a letra e volta para o 1, de 'a8' para 'b1'
            }
        }
    }

    Vertex position_to_vertice(std::string position)
    {
        int initial_row = position[0] - 'a';
        int initial_line = position[1] - '1';

        return initial_line * 8 + initial_row;
    }

public:
    HorsesAttack() : g(64)
    {
        horses_paths(g);
    }

    std::vector<int> process_tests()
    {
        std::string horse1, horse2, horse3, horse4, king;
        std::cin >> horse1 >> horse2 >> horse3 >> horse4 >> king; // le as entradas das posicoes iniciais dos cavalos e do rei, em string.

        Vertex v_horse1 = position_to_vertice(horse1); // transforma a string em um numero de vertice do grafo
        Vertex v_horse2 = position_to_vertice(horse2);
        Vertex v_horse3 = position_to_vertice(horse3);
        Vertex v_horse4 = position_to_vertice(horse4);
        Vertex v_king = position_to_vertice(king);

        BFS bfs(g); // executa a BFS a partir do rei
        bfs.processBFS(v_king);

        std::vector<int> dist_horses;
        // pega distancias de cada cavalo para ameacar o rei
        dist_horses.push_back(bfs.get_distance(v_horse1) - 1);
        dist_horses.push_back(bfs.get_distance(v_horse2) - 1);
        dist_horses.push_back(bfs.get_distance(v_horse3) - 1);
        dist_horses.push_back(bfs.get_distance(v_horse4) - 1);

        std::vector<int> lowest_dists;
        int min_dist = std::numeric_limits<int>::max();
        for (auto d : dist_horses)
        {
            if (d < min_dist)
                min_dist = d;
        }

        for (auto h : dist_horses)
        {
            if (h == min_dist)
                lowest_dists.push_back(h);
        }

        return lowest_dists;
    }
};

int main()
{
    HorsesAttack ha;

    int tests;
    std::cin >> tests;

    std::vector<std::vector<int>> results; // vetor de vetores para armazenar os resultados de cada teste

    for (int i = 0; i < tests; i++)
    {
        results.push_back(ha.process_tests());
    }

    for (int i = 0; i < tests; i++)
    {
        for (const auto &r : results[i])
        {
            std::cout << r << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}

