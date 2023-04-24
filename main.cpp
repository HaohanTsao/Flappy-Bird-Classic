#include<iostream>
#include<conio.h>
#include<dos.h>
#include<stdlib.h>
#include<string.h>
#include <windows.h>
#include <time.h>
#include <fstream>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70
#define MENU_WIDTH 20
#define GAP_SIZE 7
#define PIPE_DIF 45

using namespace std;
 
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int delay = 120;
int pipePos[3];
int gapPos[3];
int applePos; //apple position
int appleFlag = 0; // apple flag
int appleHeight;
int ifGenpipe[3];
char bird[2][6] = { '/','=','=','@','\\',' ',
     '|','_','V','_',' ','>' };
int birdPos = 6;
int score = 0;
string username;

class Player
{
 private:
  string name;
  int score;
 public:
  Player();
  Player(string name, int score);
  int getScore() { return score; }
  string getName() { return name; }
  bool rankHigherThan(Player& p);
};

Player::Player()
{
 this->name = "";
 this->score = 0;
}

Player::Player(string name, int score)
{
 this->name = name;
 this->score = score;
}

bool Player::rankHigherThan(Player& p)
{
 if(this->score >= p.score)
  return true;
 else
  return false;
}

void gotoxy(int x, int y)
{
 CursorPosition.X = x;
 CursorPosition.Y = y;
 SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size) 
{
 if(size == 0)
  size = 20; 
 
 CONSOLE_CURSOR_INFO lpCursor; 
 lpCursor.bVisible = visible;
 lpCursor.dwSize = size;
 SetConsoleCursorInfo(console,&lpCursor);
}

void drawBorder(){ 
 
 for(int i=0; i<SCREEN_WIDTH; i++){
  gotoxy(i,0); cout<<"\\";
  gotoxy(i,SCREEN_HEIGHT); cout<<"\\";
 }
 
 for(int i=0; i<SCREEN_HEIGHT; i++){
  gotoxy(0,i); cout<<"\\";
  gotoxy(SCREEN_WIDTH,i); cout<<"\\";
 }
 for(int i=0; i<SCREEN_HEIGHT; i++){
  gotoxy(WIN_WIDTH,i); cout<<"\\";
 }
}

void genPipe(int ind){
 gapPos[ind] = 3 + rand()%14;   
}

void drawPipe(int ind){
 if( ifGenpipe[ind] == true ){
  for(int i=0; i<gapPos[ind]; i++){ 
   gotoxy(WIN_WIDTH-pipePos[ind],i+1); cout<<"###"; 
  }
  for(int i=gapPos[ind]+GAP_SIZE; i<SCREEN_HEIGHT-1; i++){ 
   gotoxy(WIN_WIDTH-pipePos[ind],i+1); cout<<"###"; 
  }
 } 
}
void erasePipe(int ind){
 if( ifGenpipe[ind] == true ){
  for(int i=0; i<gapPos[ind]; i++){ 
   gotoxy(WIN_WIDTH-pipePos[ind],i+1); cout<<"   "; 
  }
  for(int i=gapPos[ind]+GAP_SIZE; i<SCREEN_HEIGHT-1; i++){ 
   gotoxy(WIN_WIDTH-pipePos[ind],i+1); cout<<"   "; 
  }
 }
}

void drawApple(){
 if(appleFlag == 1 && applePos <= 68){
  gotoxy(applePos, appleHeight);
  cout << "$";
 }
}

void eraseApple(){
 if(applePos <= 68){
  gotoxy(applePos, appleHeight);
  cout << " ";
 }
}

void drawBird(){
 for(int i=0; i<2; i++){
  for(int j=0; j<6; j++){
   gotoxy(j+2,i+birdPos); cout<<bird[i][j];
  }
 }
}
void eraseBird(){
 for(int i=0; i<2; i++){
  for(int j=0; j<6; j++){
   gotoxy(j+2,i+birdPos); cout<<" ";
  }
 }
}

int collision(){
 if( pipePos[0] >= 61  ){
  if( birdPos - 1 <gapPos[0] || birdPos >gapPos[0]+GAP_SIZE ){
   return 1;
  }
 }
 return 0;
}

void swap(Player*& ptr1, Player*& ptr2)
{
  Player* temp = ptr1;
  ptr1 = ptr2;
  ptr2 = temp;
  return;
}

void sortPlayer(Player** playerPtrs, int playerCnt)
{
  for(int i = 1; i < playerCnt; i++)
  {
    for(int j = i; j > 0; j--)
    {
      if(playerPtrs[j]->rankHigherThan(*playerPtrs[j - 1]))
        swap(playerPtrs[j], playerPtrs[j - 1]);
    }
  }
  return;
}

void scoreboard()
{
 ifstream inFile("flappyscore.txt");
 
 if(!inFile) //ç¬¬ä?æ¬¡çŽ©ï¼Œé?æ²’æ?æª”æ? 
 {
  ofstream createFile("flappyscore.txt");
  createFile << "0" << endl;
  createFile.close();
 }
 
 //è®?¥æ?æ¡?
 int times = 0;
 inFile >> times;
 Player** playerPtrs = new Player*[times + 1];
  
 string name;
 int histScore = 0;
  
 for(int i = 0; i < times; i++)
 {
  inFile >> name >> histScore;
  playerPtrs[i] = new Player(name, histScore);
 }
 
 inFile.close();
 playerPtrs[times] = new Player(username, score); //?™æ¬¡ç´??
 
 //?†æ•¸?’å?
 sortPlayer(playerPtrs, times + 1);
 
 //è¼¸å‡ºè¨ˆå?è¡?
 cout<<"\t\t--------------------------"<<endl;
 cout<<"\t\t------- Scoreboard -------"<<endl;
 cout<<"\t\t--------------------------"<<endl;
 cout<<"\t\tplayer: "<< username;
 for(int j = 0; j < 9-username.length(); j++)
  cout << " ";
 cout<< "score: " << score <<endl;
 cout<<"\t\t--------------------------"<<endl;
 
 int ranked = min(5, times + 1);
 for(int i = 0; i < ranked; i++)
 {
  cout<<"\t\tNo."<< i+1 << " "<< playerPtrs[i]->getName();
  for(int j = 0; j < 12-(playerPtrs[i]->getName().length()); j++)
   cout << " ";
  cout<<"score: " << playerPtrs[i]->getScore() <<endl;
 }
 cout<<"\t\t--------------------------"<<endl;
 
 //å¯«å…¥æª”æ?
 ofstream outFile("flappyscore.txt");
 outFile << times + 1 << endl;
 for(int i = 0; i < times + 1; i++)
 {
  outFile << playerPtrs[i]->getName() << " " << playerPtrs[i]->getScore() <<endl;
 }
 outFile.close();
 
 for(int i = 0; i < times; i++)
     delete playerPtrs[i];
   delete [] playerPtrs;
 
}

void gameover(){
 system("cls");
 cout<<endl;
 cout<<"\t\t--------------------------"<<endl;
 cout<<"\t\t------- Game Over --------"<<endl;
 cout<<"\t\t--------------------------"<<endl<<endl;
 scoreboard();
 cout<<endl; 
 cout<<"\t\tPress any key to go back to menu.";
 getch();
}
void updateScore(){
 gotoxy(WIN_WIDTH + 7, 5);cout<<"Score: "<<score<<endl;
}

void instructions(){
 
 system("cls");
 cout<<"Instructions";
 cout<<"\n----------------";
 cout<<"\n Press spacebar to make bird fly";
 cout<<"\n\nPress any key to go back to menu";
 getch();
}

int scoreGap = 10;
void play(){
 
 delay = 100;
 if (score == scoreGap)
 {
  delay -= 5;
  scoreGap += 10;
 }
 
 birdPos = 6;
 score = 0;
 ifGenpipe[0] = 1; 
 ifGenpipe[1] = 0;
 pipePos[0] = pipePos[1] = 4;
 
 system("cls");
 gotoxy(10,5); cout<<"Please enter your name: ";
 cin >> username;
 
 system("cls"); 
 drawBorder();
 genPipe(0);
 updateScore();
 
 gotoxy(WIN_WIDTH + 5, 2);cout<<"FLAPPY BIRD";
 gotoxy(WIN_WIDTH + 6, 4);cout<<"----------";
 gotoxy(WIN_WIDTH + 6, 6);cout<<"----------";
 gotoxy(WIN_WIDTH + 7, 12);cout<<"Control ";
 gotoxy(WIN_WIDTH + 7, 13);cout<<"-------- ";
 gotoxy(WIN_WIDTH + 2, 14);cout<<" Spacebar = jump";
 
 gotoxy(10, 5);cout<<"Press any key to start";
 getch();
 gotoxy(10, 5);cout<<"                      ";
 
 int wingRefresh = 0;
 
 while(1){ 
  if(kbhit()){
   char ch = getch();
   if(ch==32){
    if( birdPos > 3 )
     birdPos-=3;
     bird[1][2] = '^';
     wingRefresh = 1;
   } 
   if(ch==27){
    break;
   }
  }
  
  
  drawBird();
  drawApple();
  drawPipe(0);
  drawPipe(1);
  if( collision() == 1 ){
   gameover();
   return;
  }
  Sleep(delay);
  eraseBird();
  eraseApple();
  erasePipe(0);
  erasePipe(1);
  birdPos += 1;
  if( birdPos > SCREEN_HEIGHT - 2 ){
   gameover();
   return;
  }
  
  if (wingRefresh == 0)
  {
   bird[1][2] = 'v';
  }
  if (wingRefresh == 1)
  {
   wingRefresh = 0;
  }
  
  if( ifGenpipe[0] == 1 )
   pipePos[0] += 2;
  
  if( ifGenpipe[1] == 1 )
   pipePos[1] += 2;
  
  if(appleFlag == 1)
 applePos -= 2;    //WHY MINUS when Pipes do addition ???
  
  if( pipePos[0] >= 40 && pipePos[0] < 42 ){
   ifGenpipe[1] = 1;
   pipePos[1] = 4;
   if(appleFlag ==0){
      appleFlag = 1;
      srand(time(nullptr));
      appleHeight = 3 + rand()%14;
      applePos = 77 + rand()%20;
   }
   genPipe(1);
  }
  if( pipePos[0] > 68 ){
   score++;
   updateScore();
   pipePos[0] = pipePos[1];
   gapPos[0] = gapPos[1];
  }
  
  if( applePos <= 8 && applePos >= 3){
   if(appleHeight >= birdPos && appleHeight < birdPos + 2 && appleFlag == 1){
    appleFlag = 0;
    score += 5;
    updateScore();
 }
  }
  else if(applePos <= 2){
   appleFlag = 0;
  }
  
 }
  
}

int main()
{
 setcursor(0,0); 
 srand( (unsigned)time(NULL)); 
 
// play();
 
 do{
  system("cls");
  gotoxy(10,5); cout<<"  N----------------------\\  "; 
  gotoxy(10,6); cout<<" <      Flappy Bird   @   > "; 
  gotoxy(10,7); cout<<" <----------V------------/  ";
  gotoxy(10,9); cout<<"1. Start Game";
  gotoxy(10,10); cout<<"2. Instructions";  
  gotoxy(10,11); cout<<"3. Quit";
  gotoxy(10,13); cout<<"Select option: ";
  char op = getche();
  
  if( op=='1') 
  {
   scoreGap = 10;
   delay = 100;
   play();
  }
  else if( op=='2') instructions();
  else if( op=='3') exit(0);
  
 }while(1);
 
 return 0;
}
