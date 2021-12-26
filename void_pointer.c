#include <stdio.h>
#include <string.h>

enum TYPE {
    TYPE_CHAR,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING
};

void swapValue(void *ptr1, void *ptr2, int len, enum TYPE t)    // 반환값 없음, void 포인터 매개변수 두 개와
{                                                               // 변수의 자료형을 알려줄 열거형을 받음
    switch (t)
    {
        case TYPE_CHAR:    // 문자면 char *로 변환한 뒤 역참조하여 값을 서로 바꿈
        {
            char temp;
            temp = *(char *)ptr1;
            *(char *)ptr1 = *(char *)ptr2;
            *(char *)ptr2 = temp;
            break;
        }
        case TYPE_INT:     // 정수면 int *로 변환한 뒤 역참조하여 값을 서로 바꿈
        {
            int temp;
            temp = *(int *)ptr1;
            *(int *)ptr1 = *(int *)ptr2;
            *(int *)ptr2 = temp;
            break;
        }
        case TYPE_FLOAT:    // 실수면 float *로 변환한 뒤 역참조하여 값을 서로 바꿈
        {
            float temp;
            temp = *(float *)ptr1;
            *(float *)ptr1 = *(float *)ptr2;
            *(float *)ptr2 = temp;
            break;
        }
        case TYPE_STRING:   // character pointer(char *) 로 변환한 뒤 역참조하여 값을 서로 바꿈
        {
            char temp[4096];
            memset(temp, 0x00, sizeof(temp));
            memcpy(temp, (char *)ptr1, len);
            memcpy((char *)ptr1, (char *)ptr2, len);
            memcpy((char *)ptr2, temp, len);
            break;
        }
    }
}

int main()
{
    char c1 = 'a';
    char c2 = 'b';
    swapValue(&c1, &c2, 0,TYPE_CHAR);       // 변수의 메모리 주소와 TYPE_CHAR를 넣음
    printf("%c %c\n", c1, c2);              // b a: swapValue에 의해서 값이 서로 바뀜

    int num1 = 10;
    int num2 = 20;
    swapValue(&num1, &num2, 0,TYPE_INT);   // 변수의 메모리 주소와 TYPE_INT를 넣음
    printf("%d %d\n", num1, num2);         // 20 10: swapValue에 의해서 값이 서로 바뀜

    float num3 = 1.234567f;
    float num4 = 7.654321f;
    swapValue(&num3, &num4, 0,TYPE_FLOAT);  // 변수의 메모리 주소와 TYPE_FLOAT를 넣음
    printf("%f %f\n", num3, num4);          // 7.654321f 1.234567: swapValue에 의해서 값이 서로 바뀜
    
    char str1[] = "123";
    char str2[] = "abc";
    swapValue(str1, str2, sizeof(str1) >= sizeof(str2) ? sizeof(str1) : sizeof(str2),TYPE_FLOAT);
    printf("%s %d %s %d \n", str1, sizeof(str1), str2, sizeof(str2));

    return 0;
}