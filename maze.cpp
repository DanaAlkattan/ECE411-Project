#include "maze.h"






Maze::Maze(): size(0), matrix(NULL), edge(NULL), map(NULL)
{
}

Maze::Maze(int set_size): size(set_size)
{
    int i;
    int j;

    matrix = new int[size * size];

    edge = new int[4 * size - 4];

    map = new Cell[(size * size / 4)];

    for (i = 0; i < (size * size); ++i)
    {
        matrix[i] = 1;
    }

    for (i = 0; i < size; i += 2)
    {
        for (j = 0; j < size; j += 2)
        {
            matrix[size * i + j] = 8;
        }
    }


    for (i = 0; i < (4 * size - 4); ++i)
    {
        edge[i] = 1;
    }

    for (i = 0; i <= size * size / 4; ++i)
    {
        j = 2 * (i / (size / 2));
        map[i].set_position((2 * (i % (size / 2)) + j * size));
    }

}


Maze::~Maze()
{
    if(matrix)
        delete [] matrix;

    if(edge)
        delete [] edge;

}

Maze::Maze(const Maze& to_copy)
{
    int i;
    int * hold_matrix = to_copy.get_matrix();
    int * hold_edge = to_copy.get_edge();
    size = to_copy.get_size();

    if (hold_matrix)
    {
        matrix = new int[size * size];


        for(i = 0; i < size * size; ++i)
        {
            matrix[i] = hold_matrix[i];
        }
    }

    if(hold_edge)
    {
        edge = new int[4 * size - 4];

        for (i = 0; i < (4 * size - 4); ++i)
        {
            edge[i] = hold_edge[i];
        }
    }

}
            

int Maze::entry()
{
    int edge_size = 4 * size - 4;

    int picker;

    //srand(time(0));

    picker = rand() % edge_size;

    return picker;
}

int Maze::exit(int set_entry)
{
    int set_exit;
    int picker;

    //srand(time(0));

    picker = rand() % (size + 1);

    set_exit = set_entry + size + picker;

    if (set_exit > (4 * size - 5))
    {
        set_exit -= (4 * size - 5);
    }

    return set_exit;
}

int * Maze::get_matrix() const
{
    return matrix;
}

int * Maze::get_edge() const
{
    return edge;
}

int Maze::get_size() const
{
    return size;
}

//sets maze from map array information
int Maze::set_all()
{
    int i;

    put_map();

    map_walls();

    map_dead();

    return 1;
}

int Maze::map_walls()
{
    int i;
    int j;

    for (i = 0; i < size / 2; ++i)
    {
        for (j = 1; j < size; j = j + 2)
        {
            matrix[2 * i * size + j] = map[matrix_to_cell(2 * i * size + j - 1)].get_wall(4);
        }
        for(j = 0; j < size; j = j + 2)
        {
            matrix[((2 * i) + 1) * size + j] = map[matrix_to_cell(((2 * i) + 1) * size + j - size)].get_wall(2);
        }
    }

    return 1;
}


int Maze::map_dead()
{
    int i;
    int j;

    for (i = size + 1; i < size / 2; ++i)
    {
        for(j = 1; j < size; j = j + 2)
        {
           matrix[i + j] = 1;
        }
    } 

    return 1;
}







int Maze::get_element(int to_get) const
{
    if (to_get < size * size)
        return matrix[to_get];

    return -1;
}

int Maze::set_element(int to_set, int which_element)
{

    if(which_element > -1 && which_element < (size * size))
        matrix[which_element] = to_set;

    return matrix[which_element];
}



int Maze::two_d_to_one_d(int to_change_x, int to_change_y)
{
    return (to_change_x * size + to_change_y);
}

int Maze::one_d_to_two_d(int to_change, int (&change_to)[2])
{
    change_to[0] = to_change / size;
    change_to[1] = to_change - change_to[0];

    return 1;
}


int Maze::display()
{
    int i;
    for (i = 0; i < size * size; ++i)
    {
        cout << matrix[i] << ' ';

        if((i % size) == (size - 1))
            cout << endl;

    }

    cout << endl;

    return 1;
}

int Maze::edge_to_matrix(int from_edge)
{
    if (from_edge < (size - 1))
        return from_edge;

    if (from_edge < (2 * size - 2))
        return ((from_edge - size + 2) * size - 1);

    if (from_edge <= (3 * (size - 1)))
        return ((size * size - 1) - (from_edge - (2 * size - 2)));

    return (-(from_edge - (4 * size - 4)) * size);
}



//sends the element number in the direction passed. 1 = up, 2 = down, 
//4 = right, 8 = left.
int Maze::move(int to_move, int direction)
{
    switch(direction)
    {
        case 1:
            {
                if (to_move > (size - 1))
                    return (to_move - size);
                return -1;
            }
        case 2:
            {
                if (to_move < (size * size - size))
                    return (to_move + size);
                return -1;
            }
        case 4:
            {
                if ((to_move % size) != (size - 1))
                    return (to_move + 1);
                return -1;
            }
        case 8:
            {
                if(to_move % size)
                    return (to_move - 1);
                return -1;
            }
        default: return -1;
    }
}

//move by cell array. takes an int corresponding to matrix index and int 
//indicating direction. returns int indicating resulting matrix index.
//1 = up, 2 = down, 4 = right, 8 = left.
int Maze::move_cell(int to_move, int direction) const
{
    switch(direction)
    {
        case 1:
            {
                if (to_move > (size - 1))
                    return (to_move - 2 * size);
                return -1;
            }
        case 2:
            {
                if (to_move < (size * (size - 2)))
                    return (to_move + 2 * size);
                return -1;
            }
        case 4:
            {
                if (to_move % size != (size - 2))
                    return (to_move + 2);
                return -1;
            }
        case 8:
            {
                if(to_move % size)
                    return (to_move - 2);
                return -1;
            }
        default: return -1;
    }
}



//return random direction. up = 1, down = 2, right = 4, left = 8
//returns int. takes no arguments
int Maze::random_direction()
{
    int direction = 1;

    //srand(time(0));

    direction = direction << (rand() % 4);
    //cout << direction << endl;

    return direction;
}

//returns cell array index from corresponding matrix array index.
//takes int to represet matrix index. returns int to represent cell
//index
int Maze::matrix_to_cell(int from_position)
{
    int to_cell;
    to_cell = from_position / (size * 2) * 4 + (from_position % size) / 2;

    return to_cell;
}

//returns matrix array index from corresponding cell array index. 
//takes int to represent cell index. returns int to represent matrix index.
int Maze::cell_to_matrix(int from_cell)
{
    int to_position;
    to_position = from_cell / (size / 2) * 2 * size + (from_cell % (size / 2)) * 2;
    return to_position;
}



//map cell array to corresponding matrix indices. takes void. returns int
/*int Maze::map_cells()
{
    int i;
    int j;

    for (i = 0; i < size; i += 2)
    {
        for (j = 0; j < size; j += 2)
        {
            matrix[size * i + j] = cells[(i + j) / 2];;
        }
    }

    return 1;
}*/

//sets cell array locations to 0.
int Maze::put_map()
{
    int i;

    for (i = 0; i < size * size / 4; ++i)
    {
        matrix[map[i].get_position()] = 0;
    }

    return i;
}


//uses move_cell function move in passed direction from passed Cell object
//takes int and Cell &. returns Cell object by reference.
Cell & Maze::move_map(int direction, Cell & to_move)
{
    int to_position;

        if(move_cell(to_move.get_position(), direction) > -1)
        {
            to_position = move_cell(to_move.get_position(), direction);
            if((to_position < (size * size)) && (to_position > -1))
                return map[matrix_to_cell(to_position)];
        }

    return to_move;
}
            
//wrapper function to call make function to form maze. returns int
//takes no arguments
int Maze::make()
{
    int i;
    int select = (rand() % (size * size / 4));

    make(map[select]);
    /*
    while(make(map[select]))
    { 
        for(i = 0; i < size * size / 4; ++i)
        {
            if(!map[i].get_visited())
                make(map[i]);
        }
        
    }*/

    set_all();

    return 1;
}

//returns opposite direction from given direction. 1 = up, 2 = down, 4 = 
int Maze::opposite(int direction)
{
    switch(direction) {
        case 1:
            {
                direction = 2;
                break;
            }
        case 2:
            {
                direction = 1;
                break;
            }
        case 4:
            {
                direction = 8;
                break;
            }
        case 8:
            {
                direction = 4;
                break;
            }
        default: direction = 0;
    }

    return direction;
}

//private function to recursively generate maze. stores maze information in
//Cell array. takes Cell by reference. returns int.
int Maze::make(Cell & to_make)
{
    int direction = 0;
    Cell *  up;
    Cell *  down;
    Cell *  right;
    Cell *  left;

    Cell * temp;

    //set first point as visited
    to_make.set_visited(1);

    up = &move_map(1, to_make);

    down = &move_map(2, to_make);

    right = &move_map(4, to_make);

    left = &move_map(8, to_make);

    while((!up->get_visited()) || (!down->get_visited()) || !(right->get_visited()) || !(left->get_visited()))
    {
        direction = random_direction();

        temp = &move_map(direction, to_make);
        while(temp == &to_make || temp->get_visited())
        {
            direction = random_direction();

            temp = &move_map(direction, to_make);
        }
        
        to_make.remove_wall(direction);
        
        temp->remove_wall(opposite(direction));

        make(*temp);
    }

    return direction;
}

int Maze::display_map()
{
    int i;

    for (i = 0; i < size * size / 4; ++i)
    {
        map[i].display();
        cout << ' ';
        if(i % (size / 2) == (size / 2 - 1))
            cout << endl;
    }

    return i;
}
        


int Maze::display_walls()
{
    int i;

    for (i = 0; i < size * size / 4; i++)
    {
        map[i].display_walls();
    }

    return 1;
}


int Maze::display_visited()
{
    int i;

    for (i = 0; i < size * size / 4; i++)
    {
        map[i].display_visited();
    }

    cout << endl;

    return 1;
}



Cell::Cell():position(0), visited(0)
{
    walls = new int[4];
    for (int i = 0; i < 4; ++i)
        walls[i] = 1;
}


Cell::~Cell()
{
    if (walls)
        delete [] walls;
}

int Cell::set_position(int to_set)
{
    position = to_set;
    return position;
}

int Cell::get_position() const
{
    return position;
}

int Cell::set_visited(int to_set)
{
    visited = to_set;
    return visited;
}

int Cell::get_visited() const
{
    return visited;
}

int Cell::set_wall(int direction)
{
    switch(direction)
    {
        case 1: {
                    walls[0] = 1;
                    break;
                }

        case 2: { 
                    walls[1] = 1;
                    break;
                }
        case 4:
                {
                    walls[2] = 1;
                    break;
                }
        case 8:
                {
                    walls[3] = 1;
                    break;
                }
        default: direction = 0;
    }
    return direction;
}




int Cell::remove_wall(int direction)
{
    switch(direction)
    {
        case 1: {
                    walls[0] = 0;
                    break;
                }

        case 2: { 
                    walls[1] = 0;
                    break;
                }
        case 4:
                {
                    walls[2] = 0;
                    break;
                }
        case 8:
                {
                    walls[3] = 0;
                    break;
                }
        default: direction = 0;
    }

    return direction;
}



int Cell::get_wall(int direction) const
{
    switch(direction)
    {
        case 1: {
                    direction = walls[0];
                    break;
                }

        case 2: { 
                    direction = walls[1];
                    break;
                }
        case 4:
                {
                    direction = walls[2];
                    break;
                }
        case 8:
                {
                    direction = walls[3];
                    break;
                }
        default: direction = -1;
    }

    return direction;
}


int Cell::display()
{
    cout << position;

    return position;
}

int Cell::display_walls()
{
    int i;

    for (i = 0; i < 4; i++)
    {
        cout << walls[i] << ' ';
    }
    cout << endl;

    return 1;
}

int Cell::display_visited()
{
    cout << visited << ' ';

    return 1;
}

    

int main()
{
    Maze to_make(8);
    Maze to_test(8);
    Maze test_path(8);

    int test;
    int enter = to_make.entry();
    int goal;

    /*for (i = 0; i < 100; ++i)
    {
        goal = to_make.exit(enter);
        cout << enter << ' ' << goal << endl;
    }
    return 0;*/

    srand(time(0));

    /*cout << "enter edge: ";

    cin >> test;

    cin.ignore(100, '\n');

    goal = to_make.exit(test);
    
    cout << to_make.edge_to_matrix(test) << endl;

    cout << enter << ' ' << goal << endl;

    to_make.display();

    cout << endl;

    to_make.set_element(0, to_make.edge_to_matrix(test));

    to_make.set_element(0, to_make.edge_to_matrix(goal));

    to_make.display();

    test = to_make.edge_to_matrix(test);

    if(to_make.move(test, 1) > -1)
        to_make.set_element(0, to_make.move(test, 1));

    to_make.display();

    if(to_make.move(test, 2) > -1)
        to_make.set_element(0, to_make.move(test, 2));

    to_make.display();


    if(to_make.move(test, 4) > -1)
        to_make.set_element(0, to_make.move(test, 4));

    to_make.display();


    if(to_make.move(test, 8) > -1)
        to_make.set_element(0, to_make.move(test, 8));

    to_make.display();
    
    cout << "enter element: ";
    cin >> test;
    //to_test.set_element(0, test);

    cout << "to_test: " << endl;
    to_test.put_map();
    to_test.display();*/
    cout << endl;

    cout << "test path: " << endl;

    test_path.make();

    test_path.display();
    cout << endl;
    //test_path.display_map();
    //cout << endl;

    //test_path.display_walls();
    //cout << endl;

    test_path.display_visited();
    cout << endl;

    cout << "to_test: " << endl;
    to_test.make();
    to_test.display();
    cout << endl;
    to_test.display_visited();
    cout << endl;

    cout << "to_make: " << endl;
    to_make.make();
    to_make.display();
    cout << endl;
    to_make.display_visited();
    cout << endl;


    /*
    for (i = 0; i < 8; ++i)
    {
        to_see = to_test.random_direction();
        cout << to_see << endl;
        if(to_test.move(test, to_see) > -1)
                to_test.set_element(0, to_test.move(test, to_see));
        to_test.display();
        cout << endl;
    }

    enter = (test_path.entry());
    goal = test_path.edge_to_matrix(test_path.exit(enter));
    enter = test_path.edge_to_matrix(enter);

    cout << enter << ' ' << goal << endl;

    test_path.set_element(0, enter);
    test_path.set_element(0, goal);

    test_path.primary(enter, goal);

    test_path.display();*/




    return 0;
}







    



















