# sort_r
portable qsort_r/qsort_s alternative

### Why this library?

Did you ever try to use qsort_r/qsort_s in a cross platform project?
There are different definitions for this C function, with a different order of arguments:
```C
void qsort_r(
    void *base, size_t nmemb, size_t size,
    void *thunk,
    int (*compar)(void *, const void *, const void *)
);
```
```C
void qsort_r(
    void *base, size_t nmemb, size_t size,
    int (*compar)(const void *, const void *, void *),
    void *arg
);
```
```C
void qsort_s(
   void *base, size_t num, size_t width,
   int (__cdecl *compare )(void *, const void *, const void *),
   void * context
);
```
```C
errno_t qsort_s(
    void *base, rsize_t nmemb, rsize_t size,
    int (*compar)(const void *x, const void *y, void *context),
    void *context
);
```
The C compiler/linker will happily use this function as long as the name matches, causing a crash at runtime if the order or arguments is different than expected: 
if your C library uses a different order of arguments, a pointer to user data is considered a function pointer and called.
(I spent quite some time searching this crash on a system I had no direct access to, until I finally found (this StackOverflow article](https://stackoverflow.com/questions/39560773/different-declarations-of-qsort-r-on-mac-and-linux/39561369#39561369?newreg=5353a34ab9064dee97d091407128bce3)).

This makes "the standard library function" qsort_r/qsort_s completely unusable in highly portable cross platform code.


### What does it do?

This library provides a sorting function for all platforms without adding any dependencies (only `memcpy` and `alloca` are used, both are usually compiler builtins).
It provides the following interface (see [sort_r.h](../src/sort_r.h)):

```C
void sort_r(
    void *data,
    size_t elemcount,
    size_t elemsize,
    int (*compfunc)(const void *data1, const void *data2, void *userarg),
    void *userarg);
```

You can use it as [sort_r.c](../src/sort_r.c) and [sort_r.h](../src/sort_r.h) or just copy the [implementation](../src/sort_r.c) into your source.

The library implements the "ShellSort" algorithms.
This algorithms does not need to dynamically allocate data (one temporary element is enough, independent from the size of the input data).
In best case the timing complexity is as good as QuickSort.
In my experiments, ShellSort often performed better than qsort from the C library for arrays up to 100 elements, while qsort is faster for arrays with more than 1000 elements.
The implementation of ShellSort only requires ~15 lines of code.

See also
  - https://lamfo-unb.github.io/2019/04/21/Sorting-algorithms/
  - https://medium.com/@byron.skoutaris/sorting-algorithms-fa96ce78006d
  - https://en.wikipedia.org/wiki/Sorting_algorithm#Comparison_sorts

