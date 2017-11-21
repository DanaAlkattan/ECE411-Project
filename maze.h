#include<iostream>
#include<ctime>
#include<cstdlib>



using namespace std;


class Maze
{
    public:
        Maze();
        explicit Maze(int set_size);
        Maze(const Maze&);
        ~Maze();
        int entry();
        int exit(int set_entry);
        int * get_matrix() const;
        int get_size() const;
        int * get_edge() const;
        int set_all();
        int get_element(int to_get) const;
        int set_element(int to_set, int which_element);
        int two_d_to_one_d(int to_change_x, int to_change_y);
        int one_d_to_two_d(int to_change, int (&change_to)[2]);
        int display();
        int edge_to_matrix(int from_edge);
        int move(int to_move, int direction);
        int move_cell(int to_move, int direction) const;
        int random_direction();
        int primary(int set_entry, int set_exit);
        //int map_cells();
        int map_walls();
        int map_dead();
        class Cell & move_map(int direction, class Cell & to_move);
        int make();
        int opposite(int direction);
        int display_map();
        int put_map();
        int display_walls();
        int matrix_to_cell(int from_position);
        int cell_to_matrix(int from_cell);
        int display_visited();



    protected:
        int size;
        int * matrix;
        int * edge;
        class Cell * map;
        int make(Cell & to_make);
        

};


class Cell
{
    public:
        Cell();
        ~Cell();
        Cell(const Cell&);
        int set_wall(int direction);
        int remove_wall(int direction);
        int get_wall(int direction) const;
        int get_position() const;
        int set_position(int to_set);
        int set_visited(int to_set);
        int get_visited() const;
        int display();
        int display_walls();
        int display_visited();


    protected:
        int position;
        int visited;
        int * walls;
};




        





