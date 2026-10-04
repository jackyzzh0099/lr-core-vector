/* vector.c —— 你要实现的地方 */

#include "vector.h"

const size_t max_cap = SIZE_MAX / sizeof(int);
        //定义max_cap参数来存放capacity上限值

int vector_init(vector *v, size_t capacity) {

    v->data = NULL;
    v->end = NULL;
    v->cap = NULL;      //初始化三个指针

    if (capacity == 0)
        return 0;

    if (capacity > max_cap)
        return -1;      //超限结束程序

    int *buf = (int*)malloc(capacity * sizeof(int));
    if (buf == NULL)
        return -1;

    //先拿另一个指针变量暂存地址，然后判断是否申请成功。好习惯！

    v->data = buf;
    v->end = buf;   //空内存，data = end
    v->cap = buf + capacity;    //指针运算
    
    return 0;
}

void vector_destroy(vector *v) {

    if (v->data != NULL)
        free(v->data);  //如果本来就是空指针，就不要free

    v->data = NULL;
    v->end = NULL;
    v->cap = NULL;
    
    return;
        //返回值类型是void,函数直接return;或者不写 return。
}

size_t size(const vector *v) {

    if (v->data == NULL)
        return 0;       //空指针不能做减法！

    return (size_t)(v->end - v->data);
            //指针减法
}

size_t capacity(const vector *v) {

    if (v->data == NULL)
        return 0;       //空指针不能做减法！

    return (size_t)(v->cap - v->data);
            //指针减法
}

int empty(const vector *v) {

    // if((size(const vector *v)+1))
    //     return 1;
    // else
    //     return 0;        //踩坑

    // if (size(v)==0)
    //     return 1;
    // else
    //     return 0;    //正确的尝试，但冗杂。代码优化

    return size(v) == 0;
}

int get(const vector *v, size_t index, int *out) {

    if (index >= size(v))
        return -1;

    // out = v->data + index;
            //踩坑
    *out = *(v->data + index);
    //应该是解引用写入才对。这样才能真正改变里面的值
    return 0;
}

int set(vector *v, size_t index, int value) {

    if (index >= size(v))
        return -1;

    // *v->data[index] = value;  
    //上一个函数中学到的简化表达，等价于*(v->data + index)
    //但是踩坑了！正确的：
    v->data[index] = value; 

    return 0;
}

int front(const vector *v, int *out) {

    if (empty(v))
        return -1;

    *out = *v->data;
    return 0;
}

int back(const vector *v, int *out) {

    if (empty(v))
        return -1;

    *out = *(v->end - 1);
    return 0;
}

int push_back(vector *v, int value) {
    
    // if (capacity(v)==0)
    //     v->cap += 1;    //执行容量+1
    // if (capacity(v)!=0 && v->end==v->cap){
    //     if ((v->cap - v->data)*2 >= max_cap)
    //         return -1;      //判断是否溢出
    //     v->cap += capacity(v);  //执行容量翻倍
    // }
    
    // *v->end = value;    //队尾增添

    //坑全部踩完了（笑），看来对内存管理还是不够熟练

  
  
    // if (capacity(v)==0){
    //     int *tem = malloc(sizeof(int));
    //     capacity = 1;
    // }
        
    // else{
    //     int *tem = realloc(v->data,2*capacity*sizeof(int));
    //     capacity *= 2;
    // }
        
    // if (tem == NULL)
    //     return -1;

    // v->data = tem;
    // v->end = tem + size(v);
    // v->cap = tem + capacity;

    // *v->end = value;
    //第二轮踩坑。首先，caapacity是函数名，我们需要自己定义一个临时变量来存储当前
    //的容量，比如size_t now_cap。其次，两个分支里的 int *tem 是局部变量，
    //只在{}内部有效，出了括号tem直接消失。后面外面判断tem == NULL根本访问不到这个变量（作用域问题）。

   
   
    size_t now_cap,new_cap,now_size;    //定义now_size解决新end指针值的问题
    if (v->data == NULL)
        now_cap = 0;
    else
        now_cap = v->cap - v->data;     //获取当前容量,并避免空指针减法
    now_size = size(v);
    int *tem;           //定义一个大的局部变量（解决上一次尝试的问题）
    
    if(v->end == v->cap){          //判断容量不足
        if(now_cap == 0){
            tem = malloc(sizeof(int));
            new_cap = 1;
        }
        else{
            if(now_cap > max_cap/2)
                return -1;
            else{
                tem = realloc(v->data,2*now_cap*sizeof(int));
                new_cap = 2*now_cap;
            }
        }
                                    //重新分配内存
        if(tem == NULL)
            return -1;              //好习惯

        v->data = tem;
        v->end = tem + now_size;        //改正size(v)的乱用
        v->cap = tem + new_cap;
    }

    *v->end = value;    //赋值

    v->end += 1;        //记得改end指针
    
    return 0;
}

int pop_back(vector *v, int *out) {

    if (empty(v))
        return -1;      //如果空则返回-1

    *out = *(v->end - 1);       //最后一位写入*out
    v->end -= 1;        //“删除”最后一位

    return 0;
}

int reserve(vector *v, size_t capacity) {

    if (capacity>max_cap)
        return -1;          //判断是否超限

    size_t now_cap,now_size;
    if (v->data == NULL){
        now_cap = 0;        //避免空指针减法（未定义行为）
        now_size = 0;
    }
    else{
        now_cap = v->cap - v->data;
        now_size = v->end - v->data;
    }

    int *tem = NULL;
    if (capacity <= now_cap)
        return 0;               //判断是否无需调整
    else{
        tem = realloc(v->data,capacity*sizeof(int));
        
        if(tem == NULL)
            return -1;
        else{
            v->data = tem;
            // v->end = tem + size(v);  有问题！
            v->end = tem + now_size;
            v->cap = tem + capacity;
        }           //realloc
    }
    return 0;
}

int shrink_to_fit(vector *v) {

    if (v->end == v->cap)
        return 0;           //此时capacity==size，无需收缩

    if (v->data == v->end){
        free(v->data);
        v->data = NULL;
        v->end = NULL;
        v->cap = NULL;
        return 0;       //此时size(v)为零，按照要求全部NULL
    }

    //踩坑！忘记free！→内存泄漏

    size_t now_size;
    if(v->data == NULL)
        now_size = 0;
    else
        now_size = v->end - v->data;

    int *tem;
    tem = realloc(v->data,size(v)*sizeof(int));
    if(tem == NULL)
        return -1;
    else{
        v->data = tem;
        v->end = tem + now_size;    //解决size()不合法的问题
        v->cap = v->end;
    }                               //realloc
    
    return 0;
}

void clear(vector *v) {

    v->end = v->data;
    return;
        //返回值类型是void,函数直接return;或者不写 return。
}
