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
private:
    int** old_field_;
    int** new_field_;
    int n_;
    int m_;

};
int toInt(string x);

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
            int x = toInt(cmd_array[1]);
            int y = toInt(cmd_array[2]);
            gol.revive(x-1, y-1);
            gol.show();
        }else if(cmd_array[0] == "kill"){
            int x = toInt(cmd_array[1]);
            int y = toInt(cmd_array[2]);
            gol.kill(x-1, y-1);
            gol.show();
        }else {
            cout << "Unknown command" << endl;
        }
    }
// gol.show();

    return a.exec();
}
GameOfLife::GameOfLife(){
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
    old_field_ = new int* [n];
    new_field_ = new int* [n];
    for(int i = 0; i < n; ++i){
        old_field_[i] = new int[m];
        new_field_[i] = new int[m];
    }
    n_ = n;
    m_ = m;

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
    old_field_[x][y] = 0;
}
void GameOfLife::revive(int x, int y){
    old_field_[x][y] = 1;
}
void GameOfLife::next(){
    for (int x = 0; x < n_; x++) {
        for (int y = 0; y < m_; y++) {
            int aliveNeighbours = 0;
            for (int i = x-1; i < x+1; i++) {
                for (int j = y-1; j < y+1; j++) {
                    if(!(i == x & j == y) & (0 <= i) & (n_ > i) & (0 <= j) & (m_ > j)){
                        if(old_field_[i][j] == 1){
                            aliveNeighbours++;
                        }
                    }

                }
             }
            if(old_field_[x][y] == 0){
                if(aliveNeighbours == 3){
                    new_field_[x][y] = 1;
                }else{
                    new_field_[x][y] = 0;
                }

            }else{
                if(aliveNeighbours == 3 | aliveNeighbours == 2){
                    new_field_[x][y] = 1;
                }else{
                    new_field_[x][y] = 0;
                }
            }
        }
    }
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
    while (getline(file, line)) {
        if (counter == 1){
            n_ = toInt(line);
        }else if(counter == 2){
            m_ = toInt(line);

            old_field_ = new int* [n_];
            new_field_ = new int* [n_];
            for(int i = 0; i < n_; ++i){
                old_field_[i] = new int[m_];
                new_field_[i] = new int[m_];
            }
        }else{
            int x = 0;
            for(char& c : line) {
                if(c == 'o'){
                    old_field_[x][counter-3] = 0;
                    new_field_[x][counter-3] = 0;
                }else{
                    old_field_[x][counter-3] = 1;
                    new_field_[x][counter-3] = 1;
                }
                x++;
            }
        }
        counter ++;
    }
}
void GameOfLife::exportField(string path){

    ofstream field;
    field.open(path, ofstream::out | ofstream::trunc);

    if(field.is_open())
    {
        string str;
        field << n_ << endl;
        field << m_ << endl;
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
int toInt(string x){
    int i;
    stringstream s(x);
    s >> i;
    return i;
}

