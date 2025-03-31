def int func(int x, int a)
    if(x == a) then
        x = 10;
    fi;
    return(x + a + 10.2);
fed
def int newFunc(double x)
    double test;
    return(x + 10.3);
fed.
test = func(10.1)