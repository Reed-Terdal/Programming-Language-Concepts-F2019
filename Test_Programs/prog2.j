Integer x = 5;
Double y = 3.1;
String z = "foobar";
String a = concat( z, "123" );

print( x );
print( y );
print( x + 3 );
print( y + 0.21 );
print( z );
print( a );
print( concat( a, "456" ) );
print( concat( "123", concat( "456", "789" ) ) );
print( charAt( "foobar", 4 ) );
print( charAt( "foobar", x - 2 ) );
print( charAt( concat( "foo", "bar" ), 4 ) );