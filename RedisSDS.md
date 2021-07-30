### Redis-String

SDS的定义

```c
struct sdshdr{
    //记录buf数组中已使用的字节数量
    //等于SDS所保存字符串的长度
    int  len;
    
    //记录buf数组中未使用字节的数量
    int free;
    
    //自己数组，用于保存字符串
    char 
}
```

