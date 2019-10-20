#include <iostream>
#include <conio.h>
#include <vector>
#include <fstream>
#include <ctime>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int matrix = 222222222, userInp, compInp;
int winner = 2;//user if 1 , comp if 0 , draw if -1 , 2 not completed
vector<int> Matrix;
vector<int> moves;
vector<int> optionsAvailable;

int randNum(int limit);
int getNumFromPos(int pos);

void extractData();
void saveData();

void displayMatrix();
void autoplay();
void play(bool isAutomatic);

void refreshOptions();
void takeUserInput(bool isAutomatic);
void takeComputerInput();
void implementMoveInMatrix(int inputPos,int type);
int checkIfWon();

void learn();
void useLearntData();

int main()
{
    extractData();

    for(int i = 0; i < 500; i++)
    {
        play(true);//isAutomatic
        usleep(500000);
        cout<<"\n"<<i<<endl;
    }
    play(false);

    saveData();
    return 0;
}

int randNum(int limit)
{
    //returns a number between 0-limit
    int r = 0;
    srand(time(0));
    r = rand() % limit;
    return r;
}

int getNumFromPos(int pos)
{
    int n;
    n = matrix;
    for(int i = 0; i < pos-1; i++)
    {
        n /= 10;
    }
    return(n%10);//working
}

void displayMatrix()
{
    int dmatrix,out;
    dmatrix = matrix;
    cout<<"\n";
    for(int i = 0; i < 9; i++)
    {
        if(i%3==0)
            cout<<"\n |";
        out = dmatrix % 10;
        cout<<" ";
        if(out == 2)
            cout<<" ";
        else if(out == 1)
            cout<<"X";
        else if(out == 0)
            cout<<"O";
        cout<<" | ";
        dmatrix = dmatrix / 10;
    }
    cout<<"\n\n";
}

void extractData()
{
    int M = 0, m = 0;

    ifstream progressMatrix("DeleteMeToReset1.txt",ios::in);
    ifstream progressMoves("DeleteMeToReset2.txt",ios::in);

    while(progressMatrix>>M)
    {
        Matrix.push_back(M);
    }
    while(progressMoves>>m)
    {
        moves.push_back(m);
    }//working

    progressMatrix.close();
    progressMoves.close();
}

void saveData()
{
    ofstream saveProgressOfMatrix("DeleteMeToReset1.txt",ios::out);
    ofstream saveProgressOfMoves("DeleteMeToReset2.txt",ios::out);

    for(int i = 0; i < int(Matrix.size()); i++)
    {
        saveProgressOfMatrix << Matrix[i] << endl;
        saveProgressOfMoves << moves[i] <<endl;
    }

    saveProgressOfMatrix.close();
    saveProgressOfMoves.close();//working
}

void refreshOptions()
{
    optionsAvailable.clear();
    int dmatrix, element = 0;
    dmatrix = matrix;
    for(int i = 0; i < 9; i++)
    {
        element = dmatrix % 10;
        if(element == 2)
        {
            optionsAvailable.push_back(i+1);
        }
        dmatrix /= 10;
    }//working
}

void takeUserInput(bool isAutomatic)
{
    bool validInput = false;
    while(!validInput)
    {
        if(isAutomatic)
        {
            refreshOptions();
            userInp =  optionsAvailable[randNum(optionsAvailable.size())];
            cout<<"User input : "<<userInp<<endl;
            validInput = true;
        }
        else
        {
            cout<<"\n\nEnter your move(1-9) : ";
            cin>>userInp;
            validInput = true;
        }
        if(userInp > 9 || getNumFromPos(userInp)==1 || getNumFromPos(userInp)==0)
        {
            cout<<"Invalid input"<<endl;
            validInput = false;
        }
    }
}

void takeComputerInput()
{
    cout<<"Computer's move : ";
    refreshOptions();

    compInp = optionsAvailable[randNum(optionsAvailable.size())];

    useLearntData();
    cout<<compInp<<endl;
}

void implementMoveInMatrix(int inputPos,int type)
{
    for(int i = 0; i < inputPos-1; i++)
    {
        type *= 10;
    }
    matrix = matrix - type;
}

int checkIfWon()
{
    //user if 1 , comp if 0 , draw if -1 , 2 not completed
    int a[9],ans = 2, twoCount = 0;
    for(int i = 0; i < 9; i++)
    {
        a[i] = getNumFromPos(i+1);
        if(a[i]==2)
        {
            twoCount += 1;
        }
    }
    if(twoCount == 0)
    {
        ans = -1;
    }
    if( ( (a[0]==a[1]) && (a[0]==a[2]) && (a[0]==1)) ||
        ( (a[3]==a[4]) && (a[3]==a[5]) && (a[3]==1)) ||
        ( (a[6]==a[7]) && (a[6]==a[8]) && (a[6]==1)) ||
        ( (a[0]==a[3]) && (a[0]==a[6]) && (a[0]==1)) ||
        ( (a[1]==a[4]) && (a[1]==a[7]) && (a[1]==1)) ||
        ( (a[2]==a[5]) && (a[2]==a[8]) && (a[2]==1)) ||
        ( (a[0]==a[4]) && (a[0]==a[8]) && (a[0]==1)) ||
        ( (a[6]==a[4]) && (a[6]==a[2]) && (a[6]==1)) )
    {
       ans = 1;
    }
    if( ( (a[0]==a[1]) && (a[0]==a[2]) && (a[0]==0)) ||
        ( (a[3]==a[4]) && (a[3]==a[5]) && (a[3]==0)) ||
        ( (a[6]==a[7]) && (a[6]==a[8]) && (a[6]==0)) ||
        ( (a[0]==a[3]) && (a[0]==a[6]) && (a[0]==0)) ||
        ( (a[1]==a[4]) && (a[1]==a[7]) && (a[1]==0)) ||
        ( (a[2]==a[5]) && (a[2]==a[8]) && (a[2]==0)) ||
        ( (a[0]==a[4]) && (a[0]==a[8]) && (a[0]==0)) ||
        ( (a[6]==a[4]) && (a[6]==a[2]) && (a[6]==0)) )
    {
       ans = 0;
    }
    return ans;
}

void learn()
{
    Matrix.push_back(matrix);
    moves.push_back(compInp);
}

void useLearntData()
{
    for(int i = 0; i < int(Matrix.size()); i++)
    {
        if(Matrix[i] == matrix)
        {
            if((moves[i] ==  compInp) && optionsAvailable.size() != 0)
            {
                cout<<"..........................."<<endl;
                cout<<compInp<<endl;
                compInp = optionsAvailable[abs(randNum(optionsAvailable.size())-1)];
                cout<<compInp<<endl;
            }
            else if(optionsAvailable.size() == 0)
            {
                break;
            }
        }
    }
}

void play(bool isAutomatic)
{
    matrix = 222222222;
    winner = 2;
    optionsAvailable.clear();

    while(true)
    {
        system("cls");

        displayMatrix();
        takeUserInput(isAutomatic);
        implementMoveInMatrix(userInp,1);
        winner = checkIfWon();

        //user if 1 , comp if 0 , draw if -1 , 2 not completed
        if(winner == 1)
        {
            cout<<"You Won ";
            displayMatrix();
            learn();
            break;
        }
        else if(winner == 0)
        {
            cout<<"Comp Wons ";
            displayMatrix();
            break;
        }
        else if(winner == -1)
        {
            cout<<"Draw";
            displayMatrix();
            break;
        }

        takeComputerInput();
        implementMoveInMatrix(compInp,2);
        winner = checkIfWon();

        if(winner == 1)
        {
            cout<<"You Won ";
            displayMatrix();
            learn();
            break;
        }
        else if(winner == 0)
        {
            cout<<"Comp Wons ";
            displayMatrix();
            break;
        }
        else if(winner == -1)
        {
            cout<<"Draw";
            displayMatrix();
            break;
        }
    }
    if(!isAutomatic)
    {
        getch();
    }
}
