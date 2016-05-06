#include <stdio.h>
#include <stdlib.h>

#define buffer_size 8
#define Turing_size 47
#define inputarray_size 100
char Turing[Turing_size] = {'B', '1', '0', '1', '0', '1', '0', '1', '1', '0', '1', '1', '0', '0', '1', '0', '1', '1',
    '1', '0', '1', '1', '1', '0', '1', '0', '1', '1', '1', '0', '0', '1', '1', '0', '1', '1',
    '0', '1', '1', '0', '1', '1', '0', '1', '0', '0', '0'};
char buffer[buffer_size] = {'A'};
char inputarray[inputarray_size] = {0, 0};

void Emove();

void ModifyEtoC();

int find(int find_key);

void receive();

void Cleanbuffer();

void ModifyBtoA();

void ModifyEtoA();

void ModifyCtoA();

int Conut1(int conut_key);

void Cmove();

void Change01toarray();

void find5();

//将输入的01转换存入inputarray
void Change01toarray() {
    inputarray[0] = 'C';
    char input;
    int i = 1;
    scanf("%c", &input);
    while (input != '\n') {
        if (input == '0') {
            inputarray[i] = '1';
            i++;
            inputarray[i] = '0';
            i++;
        } else if (input == '1') {
            inputarray[i] = '1';
            i++;
            inputarray[i] = '1';
            i++;
            inputarray[i] = '0';
            i++;
        }
        else
            printf("Input error!\n");
        inputarray[i] = '#';
        scanf("%c", &input);
    }
}

//清空缓冲区
void Cleanbuffer() {
    buffer[0] = 'A';
    for (int i = 1; i < buffer_size; i++) {
        buffer[i] = '0';
    }
}

//将B后面的新符号复制到缓冲区A后面并在后面加一个D
void ModifyBtoA() {
    //计算B或E后面有几个1
    int B_conut = Conut1(1);
    int i = 0;
    for (i = 1; i <= B_conut; i++) {
        buffer[i] = '1';
    }
    buffer[i] = 'D';
}

//将E后面的新符号复制到缓冲区A后面并在后面加一个D
void ModifyEtoA() {
    //计算B或E后面有几个1
    int E_conut = Conut1(2);
    int i = 0;
    for (i = 1; i <= E_conut; i++) {
        buffer[i] = '1';
    }
    buffer[i] = 'D';
}

//将C后面的新符号复制到缓冲区D后面并将D改为0
void ModifyCtoA() {
    int C_conut = Conut1(3);
    int D_pos = find(4);
    for (int i = 1; i <= C_conut; i++) {
        buffer[D_pos + i] = '1';
    }
    buffer[D_pos] = '0';
}

//计算特定符号后有几个1
//conut_key分别对应1 B; 2 E;3 C;
int Conut1(int conut_key) {
    int conut_result = 0;
    int pos = 0;
    switch (conut_key) {
        case 1:
            pos++;
            while (Turing[pos] != '0') {
                conut_result++;
                pos++;
            }
            break;
        case 2:
            pos = find(1);
            pos++;
            while (Turing[pos] != '0') {
                conut_result++;
                pos++;
            }
            break;
        case 3:
            pos = find(2);
            pos++;
            while (inputarray[pos] != '0') {
                conut_result++;
                pos++;
            }
            break;
    }
    return conut_result;
}

//将E后面的新符号覆盖到C后面,
void ModifyEtoC() {
    int C_pos = find(2);
    int E_conut = Conut1(2);
    int C_conut = Conut1(3);
    //判断inputarray增加、缩短、或是不变
    if (E_conut > C_conut) {
        //E后面的1多，inputarray需要增加。
        int arraychange = E_conut - C_conut;    //定义需要改变的长度。
        //C以后的数往后移arraychange位
        int last_pos = find(3);
        for (int i = last_pos; i > C_pos; i--) {
            inputarray[i + arraychange] = inputarray[i];
        }
        //把E后面的数放入C后面
        for (int i = 1; i <= arraychange; i++) {
            inputarray[C_pos + i] = '1';
        }
    } else if (E_conut < C_conut) {
        //C后面的1多，inputarray需要缩短。
        int arraychange = C_conut - E_conut;
        //C以后的数往前移arraychange位
        int stop_mark = 1;
        for (int i = 1; stop_mark == 1; i++) {
            inputarray[C_pos + i] = inputarray[C_pos + arraychange + i];
            if (inputarray[C_pos + arraychange + i] == '#') {
                stop_mark = 0;
            }
        }
    }
}

//E向右移一次，替代通用图灵机编码中的下一个0
//参数：E当前所指的位置
void Emove() {
    int E_pos = find(1);
    Turing[E_pos] = '0';
    E_pos++;
    while (Turing[E_pos] != '0') {
        E_pos++;
    }
    Turing[E_pos] = 'E';
}

//C左移或右移，direction=1 左移，direction=2 右移
void Cmove() {
    int E_conut = Conut1(2);
    int direction = 0;
    if (E_conut == 1) {
        direction = 1;
    } else if (E_conut == 2)
        direction = 2;
    else
        printf("error!\n");
    int C_pos = find(2);
    inputarray[C_pos] = '0';
    switch (direction) {
        case 1:
            inputarray[C_pos] = '0';
            C_pos--;
            while (inputarray[C_pos] != '0') {
                C_pos--;
                if (C_pos < 0) {
                    printf("通用图灵机不接收该串!\n");
                    exit(-1);
                }
            }
            inputarray[C_pos] = 'C';
            break;
        case 2:
            inputarray[C_pos] = '0';
            C_pos++;
            while (inputarray[C_pos] != '0') {
                C_pos++;
            }
            inputarray[C_pos] = 'C';
            break;
    }
    
}


//找到关键字符所在的位置
//参数：需要查找字符的编号 返回关键字符所在的位置
//找E，find_key=1;找C，find_key=2;找#,find_key=3,找D,find_key=4
int find(int find_key) {
    int find_result = 0;
    int pos = 0;
    switch (find_key) {
        case 1:
            while (Turing[pos] != 'E') {
                pos++;
            }
            find_result = pos;
            break;
        case 2:
            while (inputarray[pos] != 'C') {
                pos++;
            }
            find_result = pos;
            break;
        case 3:
            while (inputarray[pos] != '#') {
                pos++;
            }
            find_result = pos;
            break;
        case 4:
            while (buffer[pos] != 'D') {
                pos++;
            }
            find_result = pos;
            break;
        default:
            printf("没有找到该字符！");
            break;
    }
    return find_result;
    
}

//判断是否达到接收态,如果C在最后，返回1，接收。如果C不在最后，返回0，拒绝。
void receive() {
    int C_pos = 0;
    if (find(2) + 1 == find(3)) {
        C_pos = find(2);
        inputarray[C_pos + 1] = '1';
        inputarray[C_pos + 2] = '1';
        inputarray[C_pos + 3] = '1';
        inputarray[C_pos + 4] = '0';
        inputarray[C_pos + 5] = '#';
    }
    if (Conut1(2) == 3) {
        printf("通用图灵机接收该串！\n");
        exit(0);
    }
}

//匹配五元组
void find5() {
    //首先找到五元组的开始   B或者00
    int buffer_pos = 1;
    int Tu_pos = 0;
    int find5_result = 0;
    int find_mark = 0;
    for (Tu_pos = 0; Tu_pos < Turing_size; Tu_pos++) {
        if ((Turing[Tu_pos] == 'B' || (Turing[Tu_pos] == '0' && Turing[Tu_pos + 1] == '0')) && find5_result == 0) {
            //找到了五元组的开头部分
            if (Tu_pos == 0) {
                find_mark = 1;
            } else
                find_mark = 2;
            
            if (find_mark == 1) {
                Tu_pos++;
            } else if (find_mark == 2) {
                Tu_pos++;
                Tu_pos++;
            }
            for (int i = 0; i < buffer_size; i++) {
                if (buffer[buffer_pos + i] == Turing[Tu_pos + i]) {
                    if (buffer[buffer_pos + i] == '0' && buffer[buffer_pos + i + 1] == '0') {
                        int E_pos = Tu_pos + i;
                        E_pos--;
                        while (Turing[E_pos] != '0') {
                            E_pos--;
                            if (E_pos < 0) {
                                printf("通用图灵机不接收该串!\n");
                                exit(-1);
                            }
                        }
                        Turing[E_pos] = 'E';
                        
                        buffer[buffer_pos + i - (Tu_pos + i - E_pos)] = 'D';
                        find5_result = 1;
                        break;
                    }
                    continue;
                } else
                    break;
            }
        }
    }
    if (find5_result == 0) {
        printf("通用图灵机不接收该串!\n");
        exit(-1);
    }
}

int main() {
    printf("请输入要接收的语言:\n");
    Change01toarray();
    Cleanbuffer();
    int i = 0;
    //通用图灵机运行
    do {
        if (i == 0) {
            ModifyBtoA();
            i = 1;
        } else {
            ModifyEtoA();
            int E_pos = find(1);
            Turing[E_pos] = '0';
        }
        ModifyCtoA();
        find5();
        Cleanbuffer();
        Emove();
        ModifyEtoC();
        Emove();
        Cmove();
        Emove();
        receive();
    } while (1);
    return 0;
}

