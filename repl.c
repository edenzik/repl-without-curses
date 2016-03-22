#include <stdio.h> 
#include <unistd.h> 
#include <termios.h> 
#include <vector>
#include <string>
#include <iostream>

void back_up_buffer(int);
void clear_line_buffer(int);
void clear_line_buffer(int,int);
int getch(void);

int main()
{
    std::vector<std::string> history;
    std::string line;
    char c;
    unsigned int hist_idx;
    unsigned int cursor;
    // For every line
    while (true) {
        printf("> ");
        line.clear();
        hist_idx = history.size();
        cursor = 0;
        // For every char
        while (true) {
            c = getch();
            // Handle backspace key
            if (c == 127 && cursor > 0) {
                clear_line_buffer(line.size(),cursor);
                line.erase(--cursor,1);
                printf("%s",line.c_str());
                back_up_buffer(line.size()-cursor);
                continue;

            }
            // Handle arrow keys
            if (c == 27 && getch() == 91) {
                switch (getch()) {
                    //Down Arrow
                    case 65:
                        if (hist_idx > 0){
                            clear_line_buffer(line.size(),cursor);
                            line = history.at(--hist_idx);
                            printf("%s",line.c_str());
                            cursor = line.size();
                        }
                        break;
                    // Up Arrow
                    case 66:
                        if (hist_idx+1 < history.size() ){
                            clear_line_buffer(line.size(),cursor);
                            line = history.at(++hist_idx);
                            printf("%s",line.c_str());
                            cursor = line.size();
                        } 
                        break;
                    // Right Arrow
                    case 67:
                        if (cursor < line.size()) putchar(line.at(cursor++));
                        break;
                    // Left Arrow
                    case 68:
                        if (cursor > 0) {
                            putchar('\b');
                            cursor--;
                        }
                        break;
                }
                continue;
            }
            // Handle Enter key
            if (c == 10){
                putchar(10);
                history.push_back(line);
                line.clear();
                break;
            } 
            // Everything else
            clear_line_buffer(line.size(),cursor);
            line.insert(cursor++, 1, c);
            printf("%s",line.c_str());
            back_up_buffer(line.size()-cursor);
        }
    }
}

// Backs up n spaces
void back_up_buffer(int n){
    for (int idx = 0; idx < n; idx++) putchar('\b');
}

// Print n spaces
void whitespace(int n){
    for (int idx = 0; idx < n; idx++) putchar(' ');
}

// Clear line of length len
void clear_line_buffer(int len){
    clear_line_buffer(len,len);
}

// Clear line of length len at position pos
void clear_line_buffer(int len, int pos){
    back_up_buffer(pos);
    whitespace(len);
    back_up_buffer(len);
}

// Hack to get getch to work without cursors
int getch(void)
{
    int ch;
    struct termios oldt;
    struct termios newt;
    tcgetattr(STDIN_FILENO, &oldt); /*store old settings */
    newt = oldt; /* copy old settings to new settings */
    newt.c_lflag &= ~(ICANON | ECHO); /* make one change to old settings in new settings */
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); /*apply the new settings immediatly */
    ch = getchar(); /* standard getchar call */
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); /*reapply the old settings */
    return ch; /*return received char */
}
