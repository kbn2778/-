#include "baseheader.h"
#include <process.h>


int update(Obj *p);
int mobUpdate(Obj *head, Obj *player,clock_t *start,clock_t  *end);

//starting page
int start(){
    gotoxy(0, 5);
    int choice;
    //draw logo
    printf("\t\t\t\t=====    =     ======= =       =    =    =       =\n");
    printf("\t\t\t\t=   =    =     =       ==     ==    =    ==      =\n");
    printf("\t\t\t\t=   =  =   =   =       = =   = =  =   =  = =     =\n");
    printf("\t\t\t\t===== =     =  =       =  = =  = =     = =  =    =\n");
    printf("\t\t\t\t=     =======  =       =   =   = ======= =   =   =\n");
    printf("\t\t\t\t=     =     =  =       =       = =     = =    =  =\n");
    printf("\t\t\t\t=     =     =  =       =       = =     = =     = =\n");
    printf("\t\t\t\t=     =     =  ======= =       = =     = =       =\n");

    //make a choice(start game, show rank, exit)
    printf("\n\n\t\t\t\t1. start\n");
    printf("\t\t\t\t2. rank\n");
    printf("\t\t\t\t3. exit\n");
    printf("\n\n\t\t\t\tinput your answer: ");
    scanf("%d",&choice);
    //return choice
    return choice;
}
//draw map
void drawmap(){
    for(int i=0;i<I;i++){
        for(int j=0;j<J;j++){
            switch(lmap[i][j]){
                case empty:
                    printf("  ");
                    break;
                case block:
                    setcolor(15);
                    printf("¡á");
                    break;
                case player:
                    setcolor(14);
                    printf("¡Ü");
                    break;
                case star:
                    setcolor(15);
                    printf("* ");
                    break;
                case monster:
                    setcolor(12);
                    printf("¡ã");
                    break;
                default:;
            }

        }
        printf("\n");
    }
}
//initialize map
void mapinit(){
    cnt_star=0;
    //set outline in map
    for(int i=0;i<I;i++){
        for(int j=0;j<J;j++){
            //init zero
            lmap[i][j]=0;
            //make outline
            if((i==0) || (i==I-1) || (j==0) || (j==J-1)){
                lmap[i][j] = block;
            }
            //make block in even lines
            if(i%2==0){
                lmap[i][j]=block;
            }
            //others are filled by stars
            else {
                if(lmap[i][j]==empty){
                    lmap[i][j] = star;
                    cnt_star++;
                }
            }
        }
    }
    //reduce 1 because player is overlapped on star
    cnt_star-=1;
}
//add monster using linked list
void addMonster(Obj **head, Obj **tail, int x, int y){
    Obj *newmon = (Obj*)malloc(sizeof(Obj));
    newmon->next=NULL;
    newmon->y = y;
    newmon->x = x;
    if(!(*head)){
        *head = newmon;
        *tail = *head;
    }
    else {
        (*tail)->next = newmon;
        *tail = newmon;
    }
}
//set monster's location and make the door between floor.
void setObject(Obj *p, Obj **head, Obj **tail){
    srand(time(NULL));
    int monster_cnt = 3+(int)(level/5);
    int floor[LAYER];
    while(monster_cnt!=0){
        int y = 2*(rand()%(LAYER-1))+3;
        int x = rand()%(J-3)+1;
        if(lmap[y][x]!=monster){
            lmap[y][x]=monster;
            addMonster(head, tail, x, y);
            cnt_star--;
            monster_cnt--;
        }
    }
    (Obj*)malloc(sizeof(Obj));

    for(int i=1;i<I-1;i++){
        for(int j=1;j<J-1;j++){
            if(i==1){
                //set position of player in first layer.
                int rx=rand()%(J-3)+1;
                lmap[1][rx]=2;
                p->x=rx;
                p->y=1;
                p->direction = left;
                break;
            }
            if(i%2==0){
                //randomly make the exit
                int escape_cnt = rand()%5+1;
                while(escape_cnt==1){
                    int door = rand()%(J-3)+1;
                    if(lmap[i][door]==block){
                        lmap[i][door]=empty;
                        escape_cnt--;
                    }
                }
            }
        }
    }
}
//delete all monsters using free
void delObj(Obj **head, Obj **tail){
    Obj *delNext = (*head)->next;
    while (1) {
        free(*head);
        *head = delNext;
        if (!(*head))break;
        delNext = (*head)->next;
    }
    *tail = *head;
}
//make sure it can move the next position
int movable(Obj *p) {
    int px=p->x, py=p->y, direction=p->direction;
    //next position calculated by direction.
    int rx, ry;
    switch(direction){
        //if the next position's state is outline, return 0.
        case left:
            if(px==1)return 0;
            rx = px-1;
            ry = py;
            break;
        case right:
            if(px==J-2)return 0;
            rx = px+1;
            ry = py;
            break;
        case up:
            if(py==1)return 0;
            rx = px;
            ry = py-1;
            break;
        case down:
            if(py==I-2)return 0;
            rx = px;
            ry = py+1;
        default: ;
    }
    //if the next position's state is block, return 0
    if(lmap[ry][rx]==block)return 0;
    //if star, get score and reduce the number of left stars
    else if(lmap[ry][rx]==star){
        score++;
        cnt_star--;
        return 1;
    }
    else return 1;
}
int showrank(){
    system("cls");
    //open the text file which have saving data
    FILE *list = fopen("pacrank.txt", "r");
    //if file pointer is null, recommend the play and make file
    if(!list){
        fclose(list);
        printf("\n\n\n\n\t\t\t\tYou are the first time at this game!\n\n\n");
        printf("\t\t\t\t          Just play!!\n\n\n\n\n");
        printf("\t\t\t\t       1. play    2. exit\n\n");
        setcolor(14);
        printf("\t\t\t\tinput your choice.: ");
        int choice;
        setcolor(15);
        scanf("%d",&choice);
        if(choice==1)return 1;
        else exit(0);
    }
    //else show the rank table
    else {
        printf("------------Rank------------\n");
        printf("rank        user        score\n");
        //variables for fscanf from file
        int _score;
        char _name[10];
        int cnt=0;
        //make struct for sorting the file
        typedef struct Record{
            char name[10];
            int score;
        }Record;
        //make structure pointer array
        Record *rec[100];
        //save the data from file to structures
        while(fscanf(list,"%s %d", _name, &_score)!=EOF){
            rec[cnt] = (Record*)malloc(sizeof(Record));
            rec[cnt]->score = _score;
            strcpy(rec[cnt]->name, _name);
            cnt++;
        }
        //close file pointer
        fclose(list);
        //sorting
        int temp;
        char tempname[10];
        for(int i=0;i<cnt;i++){
            for(int j=i+1;j<cnt; j++){
                if(rec[i]->score < rec[j]->score){
                    temp = rec[i]->score;
                    strcpy(tempname, rec[i]->name);
                    rec[i]->score = rec[j]->score;
                    strcpy(rec[i]->name, rec[j]->name);
                    rec[j]->score = temp;
                    strcpy(rec[j]->name, tempname);

                }
            }
        }
        //showing
        for(int i=0;i<cnt;i++){
            printf("%4d        %4s        %-5d\n",i+1, rec[i]->name,rec[i]->score);
        }
        //make choice for next doing: play or exit
        printf("\n\n\n\n\n\t\t\t\t       1. play    3. exit\n\n");
        setcolor(14);
        printf("\t\t\t\tinput your choice.: ");
        int choice;
        setcolor(15);
        scanf("%d",&choice);
        if(choice==1)return 1;
        else exit(0);
    }


}
//the number of monsters is various. so I use linked list for controlling monster
int save(){
    //show cursor
    CONSOLE_CURSOR_INFO cursorInfo = {0,};
    cursorInfo.bVisible = 1;
    cursorInfo.dwSize=1;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    //print the current state.
    setcolor(15);
    printf("\n\n\n\n\t\t\t\tEnd...");
    Sleep(1000);
    printf("\n\n\t\t\t\tinput your name for save(length<=4): ");
    //input user's name used to save
    char name[5], _name[5];
    scanf("%s", name);
    int cnt=0, _score=0;

    //open saving file
    FILE *list = fopen("pacrank.txt", "a+");
    //if it exits
    if(list){
        //add score and name
        fprintf(list,"%s %d\n", name, score);
    }
    //else make file to save and insert the score and name
    else {
        list = fopen("pacrank.txt", "w+");
        fprintf(list,"%s %d\n", name, score);
    }
    fclose(list);
    //then check the next choice
    printf("\n\n\n\t\t\t\tDo you want play again?\n");
    printf("\n\t\t\t\t       1. play    2. rank    3. exit\n\n");
    setcolor(14);
    printf("\t\t\t\tinput your choice.: ");
    int choice;
    setcolor(15);
    scanf("%d",&choice);
    if(choice==1)return 1;
    else if(choice==2)return 2;
    else exit(0);

}
void play(){
    //initialize the console environments
    init();
    //initialize the map
    mapinit();
    //define the head and tail about linked list for monsters
    Obj *head, *tail;
    head = tail = 0;
    //allocate the player
    Obj *p = (Obj *)malloc(sizeof(Obj));
    //initialize the monster
    setObject(p, &head, &tail);
    //variable to check the direction key
    int c;
    //draw the map
    drawmap();
    //for monster moving delay
    //the reason why using delay is to make motion times between user and monsters different.
	clock_t start=0, end = 0;
	start = clock();
	//start game
    while(1) {
        if(kbhit()){
            //for input from direction keys
            c = getch();
            if(c == 224 || c==0){
                c = getch();
                lmap[p->y][p->x]=0;
                //clear the previous position
                gotoxy(2*p->x,p->y);
                printf("  ");
                switch(c){
                    case 72:
                        //set direction
                        p->direction = up;
                        //check the possibility about next location
                        if(!movable(p))break;
                        p->y-=1;
                        break;
                    case 75:
                        p->direction = left;
                        if(!movable(p))break;
                        p->x-=1;
                        break;
                    case 77:
                        p->direction = right;
                        if(!movable(p))break;
                        p->x+=1;
                        break;
                    case 80:
                        p->direction = down;
                        if(!movable(p))break;
                        p->y+=1;
                        break;
                    default:
                        break;
                }
            }
        }
        //check if the user wins
        if(update(p)){
            lmap[p->y][p->x]=0;
            level++;
            delObj(&head, &tail);
            play();
        }
        //for monster delay
		end = clock();
		//monster update. if monster collide with user, user loses.
        if(!mobUpdate(head, p,&start,&end)){
            //initialize current game sate.
            lmap[p->y][p->x]=0;
            level++;
            level=1;
            cnt_star=0;
            delObj(&head, &tail);
            break;
        }
    }
    //clear the console
    system("cls");
    //save the current game's score
    int result = save();
    free(p);
    score=0;
    //check the next choice
    if(result==1)play();
    else {
        int result2 = showrank();
        if(result2==1)play();
    }
}
int main(){
    //set console size
    system("mode con cols=120 lines=24");
    //get choice at start page
    int start_menu = start();
    //if exit
    if(start_menu==3){
        return 0;
    }
    //if rank
    else if(start_menu==2){
        if(showrank()){
            system("cls");
        }
    }
    //else, play
    play();

    return 0;

}
//estimate the best next position about monster
int nextStep(int mx, int my, int px, int py){
    //calculate the distance from monster to player
    int distance = abs(mx-px)+abs(my-py);
    int result=-1;
    //if distance is less than 5, chase the player
    if(distance<=5){
        //calculate the weight at each position
        //order of array: left right up down
        int heu[4] = {
            abs((mx-1)-px)+abs(my-py),
            abs((mx+1)-px)+abs(my-py),
            abs(mx-px)+abs((my-1)-py),
            abs(mx-px)+abs((my+1)-py)
        };
        int e[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        int min=I*J;
        //find the best next position
        for(int i=0;i<4;i++){
            int rx = mx+e[i][0];
            int ry = my+e[i][1];
            if(lmap[ry][rx]==block){
                continue;
            }
            else if(lmap[ry][rx]==player){
                return -2;
            }
            if(min>heu[i]){
                min = heu[i];
                result = i;
            }
        }
    }
    else {
        // if the distance is more than 5, randomly move
        srand(time(NULL));
        int next;
        int flag=0;
        while(1){
            next = rand()%4;
            //check the moving possibility
            if(next==left){
                if(lmap[my][mx-1]==block)flag=0;
                else {
                    flag=1;
                    result = left;
                }
            }
            else if(next==right){
                if(lmap[my][mx+1]==block)flag=0;
                else {
                    flag=1;
                    result = right;
                }
            }
            else if(next==up){
                if(lmap[my-1][mx]==block)flag=0;
                else {
                    flag=1;
                    result = up;
                }
            }
            else {
                if(lmap[my+1][mx]==block)flag=0;
                else {
                    flag=1;
                    result = down;
                }
            }
            if(flag)break;
        }

    }
    //if possible, return direction. else return -1(result is initialized by -1)
    return result;
}
//the function for monster's next state
int mobUpdate(Obj *head, Obj *player, clock_t *start, clock_t  *end){
	//if the time change is less than delay, return 1
	if (*end - *start < MOB_DELAY) {
		return 1;
	}
	//using linked list
	*start = *end;
    Obj *next = head;
    //check all monsters using while
    while(next!=NULL){
        //delete current monster's location
        gotoxy(2*next->x, next->y);
        if(lmap[next->y][next->x]!=star)printf("  ");
        else {
            setcolor(15);
            printf("* ");
        }
        //check the next direction for moving
        int result = nextStep(next->x, next->y, player->x, player->y);
        switch(result){
            case -2:
                return 0;
            case -1:
                break;
            case left:
                next->x-=1;
                break;
            case right:
                next->x+=1;
                break;
            case up:
                next->y-=1;
                break;
            case down:
                next->y+=1;
                break;
        }
        //set red color and draw monster at next location
        setcolor(12);
        gotoxy(2*next->x, next->y);
        printf("¡ã");
        next = next->next;
    }
    return 1;
}
//check the winning condition
int update(Obj *p){
    int x=p->x, y=p->y, direction=p->direction;
    lmap[y][x]=player;
    //set color yellow and draw user at next position
    //draw different character, following the direction.
    setcolor(14);
    if(direction == left){
        gotoxy(2*x, y);
        printf("¢Å");
        Sleep(50);
    }
    else if(direction == right){
        gotoxy(2*x, y);
        printf("¢Ä");
        Sleep(50);
    }
    else if(direction == down || direction == up){
        gotoxy(2*x, y);
        printf("¡Û");
        Sleep(50);
    }
    gotoxy(2*x, y);
    printf("¡Ü");
    Sleep(50);

    //print current game state such as level, score, stars.
    setcolor(15);
    gotoxy(0, I+4);
    printf("level: %3d, score: %5d, left stars: %3d", level, score, cnt_star);
    //check the condition for win
    if(cnt_star==0)return 1;
    else return 0;
}
