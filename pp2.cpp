
#include <iostream>
#include <list>
#include <vector>
#include <stdexcept>

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

        void print_adjacency_list(GraphAl& g) const
        {
           uint n = g.get_num_vertices();
           uint e = g.get_num_edges();
           std:: cout << "num_vertices: " << n << std:: endl;
           std:: cout << "num_edges: " << e << std:: endl;
           for(uint u = 0; u < n; u++)
           {
                std:: list<Vertex> l = g.get_adj(u);
                std:: cout << u << ": ";         
                for (auto item: l)
                {
                    std:: cout << item << ", ";
                  
                }

                std:: cout << std:: endl;

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


int main() {
    GraphAl g(64); // 64 vértices do tabuleiro

    horses_paths(g);

    std::cout << "Numero de vertices: " << g.get_num_vertices() << std::endl;
    std::cout << "Numero de arestas: " << g.get_num_edges() << std::endl;
    std::cout << "---------------------------------------" << std::endl;

    // percorre todas as casas (0 a 63)
    for (Vertex u = 0; u < g.get_num_vertices(); u++) {
        std::cout << u << ": ";
        for (auto vizinho : g.get_adj(u)) {
            std::cout << vizinho << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}