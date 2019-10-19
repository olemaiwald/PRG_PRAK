#include <QCoreApplication>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class GameOfLife{
public:
    GameOfLife();
    GameOfLife(int n, int m);
    void kill(int x, int y);
    void revive(int x, int y);
    void next();
    void show();
    void importField(string path);
    void exportField(string path);
    void reload();
private:
    int** old_field_;
    int** new_field_;
    int n_;
    int m_;

};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    GameOfLife gol;
    gol.importField("C:\\Users\\Jens\\Downloads\\beispieldatei_cellularautomaton.txt");
    cout << "Game of Life" << endl;
    gol.show();
    string cmd;
    string cmd_split;
    string cmd_array[3];

    cout << "Command: ";
    while (getline(cin, cmd)){

        // Splits the command line by the delimiter space
        cmd_split = cmd;
        int counter = 0;
        while(cmd_split.find(" ")){
            cmd_array[counter] = cmd_split.substr(0, cmd_split.find(" "));
            cmd_split = cmd_split.substr(cmd_split.find(" ")+1, cmd.length());
            if(counter == 2) break;
            counter++;
        }

        if(cmd_array[0] == "next"){
            gol.next();
            gol.show();
        }else if(cmd_array[0] == "exit"){
            return(0);
        }else if(cmd_array[0] == "revive"){           
            try {
                int x = stoi(cmd_array[1]);
                int y = stoi(cmd_array[2]);
                gol.revive(x-1, y-1);
                gol.show();
            } catch(...) {
                cout << "Usage: revive x y" << endl;
            }
        }else if(cmd_array[0] == "kill"){
            try {
                int x = stoi(cmd_array[1]);
                int y = stoi(cmd_array[2]);
                gol.kill(x-1, y-1);
                gol.show();
            } catch (...) {
                cout << "Usage: kill x y" << endl;
            }
        }else {
            cout << "Unknown command" << endl;
        }
        cout << "Command: ";
    }
    return a.exec();
}
GameOfLife::GameOfLife(){
    // Creates a dynamic multidimensional array
    old_field_ = new int* [30];
    new_field_ = new int* [30];
    for(int i = 0; i < 30; ++i){
        old_field_[i] = new int[30];
        new_field_[i] = new int[30];
    }
    n_ = 30;
    m_ = 30;

    for (int x = 0; x < 30; x++) {
        for (int y = 0; y < 30; y++) {
          old_field_[x][y] = 0;
          new_field_[x][y] = 0;
        }
    }
}
GameOfLife::GameOfLife(int n, int m){
    // Creates a dynamic multidimensional array. N is the lenght of the x axis and m the lenght of the y axis
    old_field_ = new int* [n];
    new_field_ = new int* [n];
    for(int i = 0; i < n; ++i){
        old_field_[i] = new int[m];
        new_field_[i] = new int[m];
    }
    n_ = n;
    m_ = m;

    // Fills the field with dead cells
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < m; y++) {
          old_field_[x][y] = 0;
          new_field_[x][y] = 0;
        }
    }
}
void GameOfLife::show(){
    for (int y = 0; y < n_; y++) {
        cout << "|";
        for (int x = 0; x < m_; x++) {
            cout << old_field_[x][y] << "|";
        }
        cout << endl;
    }
}
void GameOfLife::kill(int x, int y){
    // Kills cell x y
    old_field_[x][y] = 0;
}
void GameOfLife::revive(int x, int y){
    // Revives cell x y
    old_field_[x][y] = 1;
}
void GameOfLife::next(){
    for (int x = 0; x < n_; x++) {
        for (int y = 0; y < m_; y++) {
            // Counts the neighbours that are alive from every cell
            int aliveNeighbours = 0;
            for (int i = x-1; i < x+1; i++) {
                for (int j = y-1; j < y+1; j++) {
                    // i and j cant be out of the field
                    if(!(i == x & j == y) & (0 <= i) & (n_ > i) & (0 <= j) & (m_ > j)){
                        if(old_field_[i][j] == 1){
                            aliveNeighbours++;
                        }
                    }

                }
             }
            if(old_field_[x][y] == 0){
                // Dead cells need 3 neighbours to be revived
                if(aliveNeighbours == 3){
                    new_field_[x][y] = 1;
                }else{
                    new_field_[x][y] = 0;
                }

            }else{
                // Alive cells need 3 or 2 neighbours to survive
                if(aliveNeighbours == 3 | aliveNeighbours == 2){
                    new_field_[x][y] = 1;
                }else{
                    new_field_[x][y] = 0;
                }
            }
        }
    }
    // Applies the new field
    for (int x = 0; x < n_; x++) {
        for (int y = 0; y < m_; y++) {
           old_field_[x][y] = new_field_[x][y];
        }
    }
}
void GameOfLife::importField(string path){
    ifstream file(path);
    string line;
    int counter = 1;
    bool error = false;
    try {
        while (getline(file, line)) {
            if (counter == 1){
                n_ = stoi(line);
            }else if(counter == 2){
                m_ = stoi(line);
                reload();
            }else{
                int x = 0;
                for(char& c : line) {
                    if(c == 'o'){
                        old_field_[x][counter-3] = 0;
                        new_field_[x][counter-3] = 0;
                    }else if(c == '*'){
                        old_field_[x][counter-3] = 1;
                        new_field_[x][counter-3] = 1;
                    }else{
                        error = true;
                    }
                    x++;
                }
                // Every line must have n characters
                if(x != n_){
                    error = true;
                }
            }
            counter ++;
        }
        // There must be m lines minus the first 2 and the last blank one
        if(counter-3 != m_){
            error = true;
        }
    } catch (...) {
        reload();
        cout << "Export file is corrupt." << endl;
    }
    if(error){
        reload();
        cout << "Export file is corrupt." << endl;
    }
}

void GameOfLife::reload(){
    // Clears both arrays
    old_field_ = new int* [n_];
    new_field_ = new int* [n_];
    for(int i = 0; i < n_; ++i){
        old_field_[i] = new int[m_];
        new_field_[i] = new int[m_];
    }
}

void GameOfLife::exportField(string path){

    ofstream field;

    // Opens file
    field.open(path, ofstream::out | ofstream::trunc);

    if(field.is_open())
    {
        string str;
        // Write n and m in the first 2 lines
        field << n_ << endl;
        field << m_ << endl;

        //Loops through the nxm matrix and writes every line in the file
        for (int y = 0; y < n_; y++) {
            string line = "";
            for (int x = 0; x < m_; x++) {
                if(old_field_[x][y] == 0){
                    line += "o";
                }else{
                    line += "*";
                }
            }
            cout << line << endl;
            field << line << endl;
        }
        field.close();
    }
    else cerr<<"Unable to open file";
}


