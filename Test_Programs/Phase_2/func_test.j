Integer xa = 5;
Integer za = 2;
String ya = "foobar";
Integer testfunc( Integer xa, Integer gg ){
    String ya = "baz";
    xa = xa - 1;
    za = za - 1;
    print( xa );
    print( ya );
    print( za );
    return xa;
}
print( xa );
print( ya );
print( za );
print("Running function now");
testfunc(7,6);
print("Function run complete");
print( xa );
print( ya );
print( za );
print("done");