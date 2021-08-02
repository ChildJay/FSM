// Reference:
// FSM: https://stackoverflow.com/questions/50165534/finite-state-machines-in-c
// C Enum: https://www.geeksforgeeks.org/enumeration-enum-c/

/* standard lib */
#include <stdio.h>  // printf #include <stdbool.h> // bool for _Bool and true for 1
#include <conio.h>  // for clrscr, and getch()
#include "stdlib.h" // for rand()
#include "math.h"

/* event array and enum below must be in sync! */
enum state_codes //״̬��
{
    _idle = 0,        //�ȴ�
    _goingUp = 1,     //������
    _goingDown = 2,   //�½���
    _AtTop = 3,       //�ѵ��ﶥ��
    _AtBottom = 4,    //�ѵ���ײ�
    _malfunction = 5, //����ά��
    _unexpected = 6,  //����
    _end = 7          //����
};
enum ret_codes //ָ����
{
    up = 0,
    down = 1,
    halt = 2,
    top = 3,
    bottom = 4,
    fail = 5,
    quit = 6
};

// enum state_codes x = _idle;

int target_floor_number = 8;
int current_floor_number = 1;
int accumulated_floor_number = 0;
#define TOP_FLOOR 9
#define BOTTOM_FLOOR 1

int event_idle(void)
{
    printf("state@:idle!\n");
    printf("hint@:����������ȥ��¥�� [1, 9] : \n");
    target_floor_number = getch() - '0';

    if (current_floor_number < target_floor_number)
        return up;
    else if (current_floor_number > target_floor_number)
        return down;
    else if (current_floor_number == target_floor_number)
        return halt;
}
int event_goingUp(void)
{
    printf("state@:goingUp!\n");
    current_floor_number += 1;
    accumulated_floor_number += 1;
    printf("hint@: ������---��ǰ¥��Ϊ %d\n", current_floor_number);

    if (accumulated_floor_number > 100)
        return fail;
    else if (TOP_FLOOR == current_floor_number)
        return top;
    else if (BOTTOM_FLOOR == current_floor_number)
        return bottom;
    else if (current_floor_number < target_floor_number)
        return up;
    else if (current_floor_number == target_floor_number)
        return halt;
    else
        return quit;
}
int event_goingDown(void)
{
    printf("state@:goingDown!\n");
    current_floor_number -= 1;
    accumulated_floor_number += 1;
    printf("hint@: �½���---��ǰ¥��Ϊ %d\n", current_floor_number);

    if (accumulated_floor_number > 100)
        return fail;
    else if (TOP_FLOOR == current_floor_number)
        return top;
    else if (BOTTOM_FLOOR == current_floor_number)
        return bottom;
    else if (current_floor_number > target_floor_number)
        return down;
    else if (current_floor_number == target_floor_number)
        return halt;
    else
        return quit;
}
int event_atTop(void)
{
    printf("state@:atTop!\n");
    printf("hint@: �ѵ���¥��---��ǰ¥��Ϊ %d\n", current_floor_number);
    printf("hint@:����������ȥ��¥�� [1, 9] : \n");
    target_floor_number = getch() - '0';

    if (current_floor_number > target_floor_number)
        return down;
    else if (current_floor_number == target_floor_number)
        return halt;
}
int event_atBottom(void)
{
    printf("state@:atBottom!\n");
    printf("hint@: �ѵ����¥---��ǰ¥��Ϊ %d\n", current_floor_number);
    printf("hint@:����������ȥ��¥�� [1, 9] : \n");
    target_floor_number = getch() - '0';

    if (current_floor_number < target_floor_number)
        return up;
    else if (current_floor_number == target_floor_number)
        return halt;
}

int event_malfunction(void)
{
    printf("state@:malfunction!\n");
    printf("hint@:���ݵ����˼�����\n");
    printf("hint@:�ܹ�����¥����Ϊ %d\n", accumulated_floor_number);
    return quit;
}
int event_end(void)
{
    printf("state@:end!\n");
    printf("�˳�");
    return 0;
}

int event_unexpected(void)
{
    printf("state@:unexpected!\n");
    printf("������");
    return quit;
}

int (*event[])(void) = {event_idle, event_goingUp, event_goingDown, event_atTop, event_atBottom, event_malfunction, event_unexpected, event_end}; //�¼���

// Nice, thanks for this. The only thing possible flaw here is that the lookup_transitions will likely be linear (O(n)) with this transition table datastructure. Its possible to make it better with multidimentional array - guaranteed O(1). Eg. the table can be represented as a multidimentional array where key is state and the value is an array where the key is the return code and value is the next state: int state_transitions[][3] = { [entry] = { foo, end, foo }, ... } /* ok, fail, repeat */
int lookup_transitions[][7] = {
    // return codes:
    //              up       down       halt          top         bottom         fail          quit
    [_idle] = {_goingUp, _goingDown, _idle, _unexpected, _unexpected, _malfunction, _end},
    [_goingUp] = {_goingUp, _unexpected, _idle, _AtTop, _AtBottom, _malfunction, _end},
    [_goingDown] = {_unexpected, _goingDown, _idle, _AtTop, _AtBottom, _malfunction, _end},
    [_AtTop] = {_unexpected, _goingDown, _AtTop, _unexpected, _unexpected, _malfunction, _end},
    [_AtBottom] = {_goingUp, _goingDown, _AtBottom, _unexpected, _unexpected, _malfunction, _end},
    [_unexpected] = {_end, _end, _end, _end, _end, _end, _end},
    [_malfunction] = {_end, _end, _end, _end, _end, _end, _end},
    /* transitions from end state aren't needed */
};

#define ENTRY_STATE _idle
#define END_STATE _end

int main()
{

    enum state_codes cur_state = ENTRY_STATE;
    enum ret_codes rc;
    int (*state_func)(void);
    printf("��������!\n");

    while (1)
    {
        state_func = event[cur_state];
        rc = state_func();
        if (END_STATE == cur_state)
            break;
        cur_state = lookup_transitions[cur_state][rc];
    }
    printf("END.\n");
    printf("��������˳�\n");
    getch();
    return 0;
}
