#include <iostream>
#include <stdio.h>
#include <stdlib.h> 
#include <vector>   
#include <string> 
#include <ncurses.h>
#include <cmath>
#include <sstream>

#define WIDTH 40
#define HEIGHT 20 
using namespace std;

WINDOW *list_win;
int startx = 0;
int starty = 0;
const char *choices[10] = { 
			"add by index",
			"add to beggining",
			"add to the end ",
			"insert part of list into it",
			"delete by index",
			"delete part of list",
			"delete list",
			"inverse sort",
			"sort",
			"exit"
};

int n_choices = sizeof(choices) / sizeof(char *);

struct Node {
    string val;
    Node* next;

    Node(string _val) : val(_val), next(nullptr) {}
};
struct l {
    Node* first;
    Node* last;
    int len = 0;
    l() : first(nullptr), last(nullptr) {}

    bool is_empty() {
        return first == nullptr;
    }

    void push_back(string _val) {
        Node* p = new Node(_val);
        if (is_empty()) {
            first = p;
            last = p;
            len++;
            return;
        }
        last->next = p;
        last = p;
        len++;
    }

    void push_ind(string _val, int idx) {
        int i = 1;
        Node* p = new Node(_val);
        len++;
        if (idx > len){
            cout<<"incorrect index"<<'\n';
            return;
        }
        
        if (is_empty()) {
            first = p;
            last = p;
            
            return;
        }

        Node* slow = first;
        Node* fast = first->next;

        if(idx == 0) {
            p->next = first;
            first = p;
            return;
        }

        while (i < idx) {
            fast = fast->next;
            slow = slow->next;
            i++;
        }
        
        slow->next = p;
        p->next = fast;
    }

    void print() {
        if (is_empty()) return;
        Node* p = first;
        while (p) {
            cout << p->val << " ";
            p = p->next;
        }
        cout << endl;
    }
    void remove_first() {
        if (is_empty()) return;
        Node* p = first;
        first = p->next;
        delete p;
        len--;
    }
    void remove_last() {
        if (is_empty()) return;
        if (first == last) {
            remove_first();
            return;
        }
        Node* p = first;
        while (p->next != last) p = p->next;
        p->next = nullptr;
        delete last;
        last = p;
        len--;
    }
    void remove(int idx) {
        if (is_empty()) return;
        if(idx<0){return;}
        
        if(idx == 0){
            remove_first();
            return;
        }

        Node* slow = first;
        Node* fast= first->next;
        int i = 1;
        
        while (i < idx) {
            fast = fast->next;
            slow = slow->next;
            i++;
        }

        if (!fast) {
            cout << "This element does not exist" << endl;
            return;
        }
        slow->next = fast->next;
        delete fast;
        len--;
    }

    void sort(int reverse) {
        for (Node* i = first; i!=NULL;i = i->next){
            for(Node* j = i->next;j!=NULL;j= j->next){
                
                if (reverse == 0) {
                    if(stoi(i->val) > stoi(j->val))
                        swap(i->val,j->val);
                }
                else {
                    if(stoi(i->val) < stoi(j->val))
                        swap(i->val,j->val);
                }
            }   
        }       
    }        
        
    void reverse(){
        Node* slow = first;
        Node* fast = first->next;
        string tmp;

        for (int i = 0; i<len;i++){
        
            for (int j = 0; j<len;j++)
            {
                 if (fast->next == nullptr)
                    break;

                if(slow < fast)
                {
                    fast->next = slow;
                    slow = fast;
                }

                else {
                    slow = slow->next;
                    fast = fast->next;
                }
            }
        }
    }
    string find(int idx) {
        Node* p = first;

        if (idx == 0) {
            string tmp = first->val;
            return tmp;
        }

        int i = 0;

        while (i<idx) { 
            p = p->next;
            i++;
        }
        return p->val;
    }

    void insert_part(int start,int amount,int where) {
        string list[20];

        for (int i = 0; i<amount;++i) {   
            list[i] = find(start+i);
        }
        
        for (int i = amount-1; i>=0; --i) {
           push_ind(list[i],where);
        }
    }
    void delete_part(int start,int amount){
        for (int i = 0; i<amount;++i) {
            remove(start);
        }
    }
};
l l;

void print_win(WINDOW *list_win)
{
	box(list_win,0,0);
	wrefresh(list_win);
}

void print_list(WINDOW *list_win)
{
    int x,y,i;
    x = 2;
	y = 2;

    box(list_win,0,0);
	
    Node* p = l.first;
    while (p) {
        mvwprintw(list_win, y, x, p->val.c_str());
        y+=1;
        p = p->next;
    }
    wrefresh(list_win);
}

void print_menu(WINDOW *menu_win, int highlight)
{
	int x, y, i;	

	x = 2;
	y = 2;
	box(menu_win, 0, 0);
	for(i = 0; i < n_choices; ++i)
	{	if(highlight == i + 1) /* High light the present choice */
		{	wattron(menu_win, A_REVERSE); 
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_REVERSE);
		}
		else
			mvwprintw(menu_win, y, x, "%s", choices[i]);
		++y;
	}
	wrefresh(menu_win);
}

int get_longint() 
{ 
    std::string inputStr = "";
   
    while (true) {
        int ch = getch();
        if (isdigit(ch)) {
            inputStr += ch;
            refresh();
        }

        else if (ch == KEY_ENTER || ch == 10 || ch == 13) {
            break;
        }

        else {
            mvprintw(3,4, "Incorrect input");
            refresh();
            getch();
            return 300;
        }
    }

    bool is_empty = inputStr.empty();
    bool is_whitespace = true;

    for (char c : inputStr) {
        if (!std::isspace(c)) {
            is_whitespace = false;
            break;
        }
    }  

    if (is_empty || is_whitespace) {
        mvprintw(3,4, "Incorrect input");
        refresh();
        getch();
        return 0;
    }  

    if(inputStr.length() > 8) {

        mvprintw(3,4, "Incorrect input");
        refresh();
        getch();
        return 0;
    }

    // Convert the string to an integer using std::stoi()
    return stoi(inputStr);
}

void print_text(int y, int x,string text) {
    text.c_str();// string to char* make
    mvprintw(y,x, text);
	refresh();
	echo();
}

int main (int argc, char *argv[])
{l.push_back("3");
    l.push_back("2");
    l.push_back("1");
    l.push_back("0");
    
    string value;
    int running,highlight = 1;
    int Enter,amount,position,start,f,c,ch,choice= 0;
    char input[80];
    WINDOW *menu_win;
    WINDOW *input_win;
    
    initscr();
    curs_set(0);
    clear();
    noecho();
    cbreak();
    startx = 2;
    starty = 2;
    
    menu_win = newwin(HEIGHT, WIDTH, starty, startx);
    list_win = newwin(HEIGHT, WIDTH, starty, startx+WIDTH);
    keypad(menu_win, TRUE);
    mvprintw(0, 0, "Use arrow keys to go up and down, Press enter to select a choice\n P.s. programm works ONLY with numbers");
    refresh();
    running = 1;
    
while(running)
{
    noecho();
    cbreak();
	keypad(menu_win, TRUE);
	print_menu(menu_win, highlight);
    list_win = newwin(HEIGHT, WIDTH, starty, startx+WIDTH);
	print_list(list_win);
	mvprintw(0, 0, "Use arrows to go up and down, Press enter to select. \n P.s. programm works ONLY with numbers");
	c = wgetch(menu_win);
	switch(c)
	{	case KEY_UP:
			if(highlight == 1)
				highlight = n_choices;
			else
				--highlight;
			break;
		case KEY_DOWN:
			if(highlight == n_choices)
				highlight = 1;
			else 
				++highlight;
			break;
		case 10:
			choice = highlight;
			break;
		default:
			mvprintw(0, 0, " Why you pressed '%c''?", c);
			mvprintw(0, 20, " instead of arrows?");
			refresh();
			break;
	}
	
	if (choice == 1)
	{
		while(1)
		{
			delwin(menu_win);
			input_win = newwin(HEIGHT, WIDTH, starty, startx);
			print_win(input_win);

            print_text(3,4, "Enter number: ");

            string value = to_string(get_longint());
            if (value  == "300")
                goto f;

            if (value.length()>9) {
                mvprintw(3,4, "Too big for int ");
                refresh();
                getch();
                goto f;
            }
            
            mvprintw(5,4, "Enter position: ");
            refresh();
            
            position = get_longint();

            if ((position > l.len) || (position < 0))
            {
                mvprintw(3,4, "list is too short ");
                refresh();
                getch();
                goto f;
            }

            else
            {
                l.push_ind(value,position);
                print_win(input_win);
                goto f;
            }
		}
	}

    if (choice == 2)
	{
		while(1)
		{
			delwin(menu_win);
			input_win = newwin(HEIGHT, WIDTH, starty, startx);
			print_win(input_win);

            mvprintw(3,4, "Enter numeber: ");
			refresh();
			echo();

            string value = to_string(get_longint());
            if (value  == "300")
                goto f;

            if (value.length()>9) 
            {
                mvprintw(3,4, "Too big for int ");
                refresh();
                getch();
                goto f;
            }

            else 
            {
                l.push_ind(value,0);
                print_list(menu_win);
                goto f;
            }
		}
	}

    if (choice == 3)
	{
		while(1)
		{
			delwin(menu_win);
			input_win = newwin(HEIGHT, WIDTH, starty, startx);
			print_win(input_win);

            mvprintw(3,4, "Enter number: ");
			refresh();
			echo();
            
            string value = to_string(get_longint());
            if (value  == "300")
                goto f;

            if (value.length()>9) {
                mvprintw(3,4, "Too big for int ");
                refresh();
                getch();
                goto f;
            }

            else {
                l.push_ind(value,l.len);
                print_list(menu_win);
                goto f;
            }
           
		}
	}

    if (choice == 4)
	{
		while(1)
		{
			delwin(menu_win);
			input_win = newwin(HEIGHT, WIDTH, starty, startx);
			print_win(input_win);

            mvprintw(3,4, "Enter start number: ");
			refresh();
			echo();
         
            start = get_longint();

            mvprintw(5,4, "How many: ");
			refresh();
			echo();

            amount= get_longint();

            mvprintw(7,4, "Where: ");
			refresh();
			echo();

            position= get_longint();
           
            if ((amount < 0) || (amount + l.len > 20) || (start < 0) || (start + amount > l.len + 1) || (position < 0) || (position > l.len)|| (amount>l.len))
            {
                mvprintw(9,4, "Incorrect options!!");
                refresh();
                getch();
                goto f;
            }
            
            else
            {
                l.insert_part(start, amount,position);
                print_list(menu_win);
                goto f;
            }
           
		   
	    }

	}

    if (choice == 5)
	{
		while(1)
		{
			delwin(menu_win);
			input_win = newwin(HEIGHT, WIDTH, starty, startx);
			print_win(input_win);

            mvprintw(3,4, "Enter index: ");
			refresh();
			echo();
            
            position = get_longint();

            if ((position > l.len) || (position < 0) || (position >= l.len))
            {
                mvprintw(3,4, "Incorrect options!!!!!");
                refresh();
                getch();
                goto f;         
            }

            else
            {
                l.remove(position);
                print_list(menu_win);
                goto f;
            }
		}
	}

    if (choice == 6)
	{
		while(1)
		{
			delwin(menu_win);
			input_win = newwin(HEIGHT, WIDTH, starty, startx);
			print_win(input_win);

            mvprintw(3,4, "From which index delete?: ");
			refresh();
			echo();

            position = get_longint();

            mvprintw(5,4, "How many?: ");
			refresh();
			echo();
            amount = get_longint();
        
            if ((position + amount > l.len) || (amount < 0) || (position < 0)){
                mvprintw(7,4, "Incorrect options ");
                refresh();
                getch();
                goto f;
            }

            else {
                l.delete_part(position, amount);
                print_list(menu_win);
                goto f;
            }
		}
	}
     
    if (choice == 7)
	{
		while(1)
		{
			delwin(menu_win);
			input_win = newwin(HEIGHT, WIDTH, starty, startx);
			print_win(input_win);

            amount = l.len;
            
            if (amount == 0) 
            {
                mvprintw(3,4, "ALREADY EMPTY ");
                refresh();
                getch();
                goto f;
            }
          
            else
            {
                l.delete_part(0, amount);
                print_win(input_win);
                refresh();
                goto f;
            }
		}
	}
    
    if (choice == 8)
	{
		while(1)
		{
			delwin(menu_win);
			input_win = newwin(HEIGHT, WIDTH, starty, startx);
			print_win(input_win);

            l.sort(1);
            print_list(menu_win);
            goto f;
        }
	}
         
     if (choice == 9)
	{
		while(1)
		{
			delwin(menu_win);
			input_win = newwin(HEIGHT, WIDTH, starty, startx);
			print_win(input_win);

            l.sort(0);
            print_list(menu_win);  

            f:
            delwin(input_win);
		    menu_win = newwin(HEIGHT, WIDTH, starty, startx);
		    break;      
        }
    }
	

    if (choice == 10)
    {
        l.delete_part(0,l.len);
        running = 0;
    }     
    
    choice = 0; //change choice to 0 so in new cycle it wil not automatically choose smth
	delwin(list_win);
    print_menu(menu_win, highlight);
	clrtoeol();
	refresh();
}
	
endwin();
return 0;
}