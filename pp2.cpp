
#include <iostream>
#include <list>
#include <vector>
#include <stdexcept>
#include <limits>

typedef unsigned int uint;
typedef unsigned int Vertex;


class GraphAl {
    private:
        uint num_vertices;
        uint num_edges;
        std::list <Vertex> *adj;

    public: 
        GraphAl(uint num_vertices) 
            : num_vertices (num_vertices), num_edges(0)
            {
             adj = new std::list <Vertex>[num_vertices];
            }
        
        ~GraphAl() 
        {
            delete[] adj;
            adj = nullptr;
            
        }

        void add_edge(const Vertex& u, const Vertex& v)
        {
            if (u < 0 || v < 0 || u > num_vertices || v > num_vertices || u == v)
                throw std::invalid_argument("valores de vertices invalidos"); 
            
            adj[u].push_back(v);
            adj[v].push_back(u);
            num_edges++;
        }

        const std::list<Vertex>& get_adj(const Vertex& u) const
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
    Queue(){}

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

enum class Color
{
    white, gray, black
};

class BFS
{
    private:
        GraphAl& g;
        std::vector <Color> color;
        std::vector <uint> dist;
        std::vector <int> pred;
    
    public:
         BFS(GraphAl& graph)
        : g(graph),
          color(graph.get_num_vertices(), Color::white),
          dist(graph.get_num_vertices(), std::numeric_limits<uint>::max()),
          pred(graph.get_num_vertices(), -1) {}
    
    uint get_distance (Vertex v)
    {
        return dist[v];
    }
    void processBFS(Vertex s)
    {
        uint n = g.get_num_vertices();

        for (Vertex u = 0; u < n; u++)
        {
            color[u] = Color:: white; //pinta todos os vertices de branco
            dist[u] = std::numeric_limits<uint>::max(); //considera primeiramente distancia infinita
            pred[u] = -1; //predecessor "nulo"
        }
        
        color[s]= Color::gray; //pinta a origem de cinza
        dist[s] = 0;
        pred[s] = -1;

        Queue Q;

        Q.enqueue(s); //cria uma fila e bota o vertice de origem

        while (!Q.empty()) //loop enquanto a fila nao estiver vazia
        {
            Vertex u = Q.front(); //analisa o vertice da frente na fila
            Q.dequeue(); //tira o primeiro da fila
            for (const auto& v : g.get_adj(u)) //loop para pegar os vertices adjacentes do vertice u que estava na frente da fila //!!!!acho que aqui que precisa botar o algoritmo de ordenacao
            {
            if (color[v] == Color::white) //se for branco (ainda nao encontrado)
            {
                color[v] = Color::gray; //pinta de cinza, aumenta o numero da distancia e adiciona o predecessor
                dist[v] = dist[u] + 1; 
                pred[v] = u;
                Q.enqueue(v); //bote os vertices na fila
            }

            color[u] = Color::black; //depois de analisar todos os adjacentes do vertice, pinte de preto

            }

        }

    }
};


void horses_paths(GraphAl& g) 
{
   int L_moves[8][2] =
   { {-1,-2}, {1, -2}, {-2,-1}, {2, -1},  //matriz com todas as possibilidades de movimento em L do cavalo 
     {-2,1}, {2,1}, {-1,2}, {1, 2}, 
   };

   std::string initial_position = "a1"; 

   while (initial_position <= "h8") //percorrer de 'a1' ate 'h8'
   {
   int initial_row = initial_position[0] - 'a'; //as colunas de 'a' a 'h' sendo 'a'-> 0 e 'h'-> 7
   int initial_line = initial_position[1] - '1'; //as linhas de 1 a 8, diminuido em 1 

   Vertex initial_vertice = initial_line*8 + initial_row; //vertice equivalente, sendo 'a1'-> 0 e 'h8' -> 63

   for (const auto& possible_moves: L_moves) //percorre todos os movimentos que o cavalo pode possivelmente fazer
   {
     int new_line = initial_line + possible_moves[1]; //novo numero da casa 
     int new_row = initial_row + possible_moves[0]; //nova letra da coluna
     if (new_line>=0 && new_line<8 && new_row>=0 && new_row<8) //verifica a possibilidade do novo movimento
     {
         Vertex new_vertice = new_line*8 + new_row; //novo vertice apos movimento em L
         if (initial_vertice < new_vertice) //adiciona aresta sem duplicar 
        {
            g.add_edge(initial_vertice, new_vertice);
        }
     }
   }
      if (initial_position[1] < '8')
        initial_position[1]++; //incrementa o numero, de 'a1' para 'a2'
      else
      {
        initial_position[1] = '1'; 
        initial_position[0]++; //ao chegar no numero 8, aumenta a letra e volta para o 1, de 'a8' para 'b1'
      }
   }
}


Vertex position_to_vertice(std::string position)
{
    int initial_row = position[0] - 'a'; 
    int initial_line = position[1] - '1'; 

    return initial_line*8 + initial_row;
}

void process_tests(GraphAl& g, int tests) 
{
    for (int t = 0; t < tests; t++) 
    {
        std::string horse1, horse2, horse3, horse4, king;
        std::cin >> horse1 >> horse2 >> horse3 >> horse4 >> king; // lê as entradas das posicoes iniciais dos cavalos e do rei, em string.

        Vertex v_horse1 = position_to_vertice(horse1); //transforma a string em um numero de vertice do grafo
        Vertex v_horse2 = position_to_vertice(horse2);
        Vertex v_horse3 = position_to_vertice(horse3);
        Vertex v_horse4 = position_to_vertice(horse4);
        Vertex v_king   = position_to_vertice(king);

        BFS bfs(g);  // executa a BFS a partir do rei
        bfs.processBFS(v_king);

        // pega distâncias de cada cavalo para ameacar o rei
        int dist_h1 = bfs.get_distance(v_horse1) - 1 ;
        int dist_h2 = bfs.get_distance(v_horse2) - 1;
        int dist_h3 = bfs.get_distance(v_horse3) - 1;
        int dist_h4 = bfs.get_distance(v_horse4) - 1;

        // imprime resultado do caso de teste
        std::cout << "Caso " << t+1 << ":\n";
        std::cout << horse1 << " -> " << dist_h1 << "\n";
        std::cout << horse2 << " -> " << dist_h2 << "\n";
        std::cout << horse3 << " -> " << dist_h3 << "\n";
        std::cout << horse4 << " -> " << dist_h4 << "\n";
        std::cout << std::endl;
    }
}


int main() {
    GraphAl g(64); // 64 vértices do tabuleiro

    horses_paths(g);

    int tests;
    std::cin >> tests;
    
    process_tests(g, tests);

    return 0;
}
