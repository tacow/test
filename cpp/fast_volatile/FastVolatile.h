#ifndef FAST_VOLATILE_H
#define FAST_VOLATILE_H

// Fast write, normal read
// Only one thread can write the fast volatile object, multiple threads can read it

template <class T>
class FastVolatile {
public:
    FastVolatile() : var_(T()), volatileVar_(T()) {}
    FastVolatile(T var) : var_(var), volatileVar_(var) {}
    ~FastVolatile() {}

    operator T () {
        return volatileVar_;
    }

    void operator = (const T& var) {
        if (var_ == var)
            return;
        var_ = var;
        volatileVar_ = var;
    }

private:
    T var_;
    volatile T volatileVar_;
};

#endif

