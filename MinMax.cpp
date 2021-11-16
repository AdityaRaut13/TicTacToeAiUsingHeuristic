#include <cstdint>
#include <iostream>
#include <vector>
/*
 * Space -> 2
 * X -> 3
 * O -> 5
 * */
#define UPPER_ROW 0
#define MIDDLE_ROW 1
#define LOWER_ROW 2
#define LEFT_COLUMN 3
#define MIDDLE_COLUMN 4
#define RIGHT_COLUMN 5
#define DIAGONAL 6
#define INVERSE_DIAGONAL 7
/*
 * if X winning : 1
 * if O winning : 2
 * if no one wins : 0
 * */
int has_won(int box[9])
{
	int winning_box[8];
	winning_box[UPPER_ROW]=box[0]*box[1]*box[2];
	winning_box[MIDDLE_ROW]=box[3]*box[4]*box[5];
	winning_box[LOWER_ROW]=box[6]*box[7]*box[8];
	winning_box[LEFT_COLUMN]=box[0]*box[3]*box[6];
	winning_box[MIDDLE_COLUMN]=box[1]*box[4]*box[7];
	winning_box[RIGHT_COLUMN]=box[2]*box[5]*box[8];
	winning_box[DIAGONAL]=box[0]*box[4]*box[8];
	winning_box[INVERSE_DIAGONAL]=box[2]*box[4]*box[6];
	
	for(int i=0;i<8;i++)
	{
		if(winning_box[i]==27)
			return 1;
		else if(winning_box[i]==125)
			return 2;
	}
	return 0;
}
std::vector<int> moves_left(int board[9])
{
	std::vector<int> moves;
	for(int i=0;i<9;i++)
		if(board[i]==2)moves.push_back(i);
	return moves;
}
void display(int box[9])
{
	std::cout<<"=============\n";

	for(int i=0;i<3;i++)
	{
		std::cout<<"| ";
		for(int j=0;j<3;j++)
		{
			if(box[3*i+j]==2)       std::cout<<"  | ";
			else if (box[3*i+j]==3) std::cout<<"X | ";
			else                    std::cout<<"O | ";
		}
		std::cout<<"\n";
	}
	std::cout<<"=============\n";
}
int heuristic(int box[9])
{
	if(has_won(box)==1)
		return 10;
	else if (has_won(box)==2)
		return -10;
	int winning_box[8],cnt_max=0,cnt_min=0;
	winning_box[0]=box[0]*box[1]*box[2];
	winning_box[1]=box[3]*box[4]*box[5];
	winning_box[2]=box[6]*box[7]*box[8];
	winning_box[3]=box[0]*box[3]*box[6];
	winning_box[4]=box[1]*box[4]*box[7];
	winning_box[5]=box[2]*box[5]*box[8];
	winning_box[6]=box[0]*box[4]*box[8];
	winning_box[7]=box[2]*box[4]*box[6];
	for(int i=0;i<8;i++)
	{
		if(winning_box[i]==12 or winning_box[i]==18 )
			cnt_max++;
		else if (winning_box[i]==20 or winning_box[i]==50)
			cnt_min++;
	}
	return cnt_max-cnt_min;
	
}
/*
 *
 * depth : 1-INT_MAX_32
 * The maximizer : true
 * The minimizer : false
 * */

int evaluate(int board[9],int depth,bool maximizer)
{
	std::vector<int> moves=moves_left(board);
	int max_value=INT32_MIN,min_value=INT32_MAX;
	if(has_won(board)==1)
	    return 10;
	else if (has_won(board)==2)
	    return -10;
	if(moves.size()==0)return 0;
	if(depth==1)
	{
		for(int i=0;i<moves.size();i++)
		{
			board[moves[i]]=maximizer ? 3 : 5;
			int move=heuristic(board);
			if(maximizer)
				std::cout<<"MAXIMIZER: Move : "<<moves[i]<<" Heuristic : "<<move<<std::endl;
			else
				std::cout<<"MINIMIZER: Move : "<<moves[i]<<" Heuristic : "<<move<<std::endl;
			if(move>max_value)
				max_value=move;
			if(move<min_value)
				min_value=move;
			board[moves[i]]=2;
		}
		return maximizer ? max_value:min_value;
	}
	else
	{
		for(int i=0;i<moves.size();i++)
		{

			int move=evaluate(board,depth-1,maximizer ? false:true);
			board[moves[i]]=maximizer ? 3 : 5;
			//std::cout<<"Move : "<<moves[i]<<" Heuristic : "<<move<<std::endl;
			if(move>max_value)
				max_value=move;
			if(move<min_value)
				min_value=move;
			board[moves[i]]=2;
		}
		return maximizer ? max_value:min_value;
	}
}

int take_input(int box[9])
{
	int key=0;
	do
	{
		std::cout<<"Enter the number : ";
		std::cin>>key;
		if(!(key>=0 and key<=8 and box[key]==2))
			std::cout<<"Enter a valid key\n";
	} while(!(key>=0 and key <=8 and box[key]==2));
	return key;
}
int best_move(int board[9],int depth,bool maximizer)
{
	int best_move_maximizer,best_move_minimizer,max_value=INT32_MIN,min_value=INT32_MAX;
	std::vector<int> moves=moves_left(board);
	for(int i=0;i<moves.size();i++)
	{
		board[moves[i]]=maximizer ? 3 : 5;
		int value=evaluate(board,depth,maximizer ? false : true);
		if(maximizer)
			std::cout<<"IN BEST MOVE : MAXIMIZER: Move : "<<moves[i]<<" Heuristic : "<<value<<std::endl;
		else
			std::cout<<"IN BEST MOVE : MINIMIZER: Move : "<<moves[i]<<" Heuristic : "<<value<<std::endl;
		if(value>max_value)
		{
			max_value=value;
			best_move_maximizer=moves[i];
		}
		if(value<min_value)
		{
			min_value=value;
			best_move_minimizer=moves[i];
		}
		board[moves[i]]=2;
	}
	return maximizer? best_move_maximizer: best_move_minimizer;
}
bool space_left(int board[9])
{
	for(int i=0;i<9;i++)
	{
		if(board[i]==2)
			return true;
	}
	return false;
}
void menu(bool user_takes_x,int depth)
{
	system("clear");
	std::string rules="Enter the number in following format : \n"
	 "=============\n"
	 "| 0 | 1 | 2 |\n"
	 "| 3 | 4 | 5 |\n"
	 "| 6 | 7 | 8 |\n"
	 "=============\n" ;
	int board[9]={2,2,2,2,2,2,2,2,2};
	if(user_takes_x)
	{
		do
		{
			std::cout<<rules;
			int user_move=take_input(board);
			board[user_move]=3;
			if(has_won(board)!=0 or space_left(board)==false)
				break;
			int computer_move=best_move(board,depth,false);
			board[computer_move]=5;
			if(has_won(board)!=0 or space_left(board)==false)
				break;
			//system("clear");
			display(board);
		}while(has_won(board)==0 and space_left(board));
		if (has_won(board)==1)
			std::cout<<"\n YOU WON :=(\n";
		else if (has_won(board)==2)
			std::cout<<"\nI WON :)\nBETTER LUCK NEXT TIME \n";
		else
			std::cout<<"\nIT WAS A DRAW !! \n";
	}
	else
	{
		 
		while(has_won(board) == 0 and space_left(board))
		{
			system("clear");
			std::cout<<rules;
			int computer_position= best_move(board,depth,true);
			board[computer_position]= 3; 
			display(board);
			if(has_won(board)!=0 or space_left(board)==false)
				break;
			int user_move= take_input(board);
			board[user_move]= 5;
			if(has_won(board)!=0 or space_left(board)==false)
				break;
			display(board);
		}
		if (has_won(board)==2)
			std::cout<<"\n YOU WON :=(\n";
		else if (has_won(board)==1)
			std::cout<<"\nI WON :)\nBETTER LUCK NEXT TIME \n";
		else
			std::cout<<"\nIT WAS A DRAW !! \n";
	}
}
bool user_task()
{

	int number;
	std::cout<<"\n0. TAKE O\n";
	std::cout<<"\n1. TAKE X\n";
	std::cout<<"Enter the number ";
	std::cin>>number;
	return number;

}
int take_depth()
{
	int number;
	std::cout<<"Enter a number between 1-6 : ";
	std::cin>>number;
	return number;
}
int main()
{
	bool user_take_x=user_task();
	int depth =take_depth();
	menu(user_take_x,depth);
	return 0;
}
