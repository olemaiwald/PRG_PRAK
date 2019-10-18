#include <QCoreApplication>
#include <iostream>
using namespace std;

class GameOfLife{
public:
    GameOfLife();
    GameOfLife(int n);
    GameOfLife(int field[30][30]);
    GameOfLife(int field[30][30], int n);
    void kill(int x, int y);
    void revive(int x, int y);
    void next();
    void show();

private:
    int** old_field_;
    int** new_field_;
    int n_;

};
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    GameOfLife gol;
    gol.show();

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

    for (int x = 0; x < 30; x++) {
        for (int y = 0; y < 30; y++) {
          old_field_[x][y] = 0;
          new_field_[x][y] = 0;
        }
    }
}
GameOfLife::GameOfLife(int n){
    old_field_ = new int* [n];
    new_field_ = new int* [n];
    for(int i = 0; i < n; ++i){
        old_field_[i] = new int[n];
        new_field_[i] = new int[n];
    }
    n_ = n;

    for (int x = 0; x < 30; x++) {
        for (int y = 0; y < 30; y++) {
          old_field_[x][y] = 0;
          new_field_[x][y] = 0;
        }
    }
}
GameOfLife::GameOfLife(int field[30][30]){
    old_field_ = new int* [30];
    new_field_ = new int* [30];
    for(int i = 0; i < 30; ++i){
        old_field_[i] = new int[30];
        new_field_[i] = new int[30];
    }
    n_ = 30;

    for (int x = 0; x < 30; x++) {
        for (int y = 0; y < 30; y++) {
          old_field_[x][y] = field[x][y];
          new_field_[x][y] = field[x][y];
        }
    }
}
GameOfLife::GameOfLife(int field[30][30], int n){
    old_field_ = new int* [n];
    new_field_ = new int* [n];
    for(int i = 0; i < n; ++i){
        old_field_[i] = new int[n];
        new_field_[i] = new int[n];
    }
    n_ = n;

    for (int x = 0; x < 30; x++) {
        for (int y = 0; y < 30; y++) {
          old_field_[x][y] = field[x][y];
          new_field_[x][y] = field[x][y];
        }
    }
}
void GameOfLife::show(){
    for (int y = 0; y < 30; y++) {
        cout << "|";
        for (int x = 0; x < 30; x++) {
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
    for (int x = 0; x < 30; x++) {
        for (int y = 0; y < 30; y++) {
            int aliveNeighbours = 0;
            for (int i = x-1; i < x+1; i++) {
                for (int j = y-1; j < y+1; j++) {
                    if(!(i == x & j == y) & (0 <= i) & (30 > i) & (0 <= j) & (30 > j)){
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
}

