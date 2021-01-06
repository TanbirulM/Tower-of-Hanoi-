#include <stdio.h>
#include <thread>
using namespace std;

const int STACK_SIZE = 5;
const int TIME_SPAN = 100;

int max;
int stacks[3][STACK_SIZE];

void sleep() {
    this_thread::sleep_for(chrono::milliseconds(TIME_SPAN));
}

void recurse(char* str, int &start, int length, char c) {
    for (int i = start; i < start + length; i++) {
        str[i] = c;
    }
    start += length;
}

char* disking(int val) {
    char* x = new char[STACK_SIZE * 2 + 2];
    x[STACK_SIZE * 2 + 1] = 0;
    int next = 0;
    if (val == 0) {
        recurse(x, next, STACK_SIZE * 2 + 1, ' ');
        x[STACK_SIZE] = '|';
    }
    else {
        recurse(x, next, STACK_SIZE - val, ' ');
        x[next++] = '(';
        recurse(x, next, val - 1, '[');
        x[next++] = '|';
        recurse(x, next, val - 1, ']');
        x[next++] = ')';
        recurse(x, next, STACK_SIZE - val, ' ');
    }
    return x;
}

void show() {
    system("clear");
    for (int i = 0; i < 3; i++) {
        printf("%s", disking(0));
    }
    printf("\n");
    for (int y = STACK_SIZE - 1; y >= 0; y--) {
        for (int x = 0; x < 3; x++) {
            printf("%s", disking(stacks[x][y]));
        }
        printf("\n");
    }
    sleep();
}

int tower_top(int towers) {
    for (int i = STACK_SIZE - 1; i >= 0; i--) {
        if (stacks[towers][i] != 0) {
            return i;
        }
    }
    return -1;
}

int top(int towers) {
    for (int i = STACK_SIZE - 1; i >= 0; i--) {
        if (stacks[towers][i] != 0) {
            return stacks[towers][i];
        }
    }
    return 0;
}

int pop(int towers) {
    for (int i = STACK_SIZE - 1; i >= 0; i--) {
        if (stacks[towers][i] != 0) {
            int ret = stacks[towers][i];
            stacks[towers][i] = 0;
            return ret;
        }
    }
    throw underflow_error("Stack is empty, cannot pop any more.\n");
}

void push(int towers, int val) {
    for (int i = 0; i < STACK_SIZE; i++) {
        if (stacks[towers][i] == 0) {
            stacks[towers][i] = val;
            return;
        }
    }
    throw overflow_error("The stack has become full. No further action is possible.\n");
}

bool is_Empty(int disk) {
    return (top(disk) == 0);
}

bool is_Valid(int src, int dst) {
    return (is_Empty(dst) || top(src) < top(dst));
}

void move_disk(int src, int dst) {
    show();
    if (is_Valid(src, dst)) {
        push(dst, pop(src));
    }
    else {
        throw invalid_argument("Bigger disks cannot be placed on smaller disks");
    }
}

bool is_under(int level, int disk) {
    return top(disk) < stacks[disk][level];
}

void reposition(int y, int src, int dst, int tmp) {
    if (is_under(y, src)) {
        int temp = tower_top(tmp);
        reposition(y + 1, src, tmp, dst);
        move_disk(src, dst);
        reposition(temp + 1, tmp, dst, src);
    }
    else {
        move_disk(src, dst);
    }
}

void initial_base() {
    for (int i = STACK_SIZE; i > 0; i--) {
        push(0, i);
    }
}
int main() {
    initial_base();
    reposition(0, 0, 2, 1);
    show();

    return 0;
}