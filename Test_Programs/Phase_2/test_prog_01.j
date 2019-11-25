// This is a comment

print("Int Test");
Integer x = 20;
print(x);
x = 30;
print(x);
if(x == 30)
{
    print(x);
    x = 40;
    print(x);
    x = 50;
    print(x);
}
print(x);
x = 60;
print(x);
x = 2.0 * 3.0 ^ 2.0 == 36.0 + 3;
print(x);

print("Double Test");

Double z = 20.0;
print(z);
z = 30.0;
print(z);
if(1)
{
    print(z);
    z = 40.0;
    print(z);
    z = 50.0;
    print(z);
}
print(z);
z = 60.0;
print(z);

print("String Test");

String tester = "First";
print(tester);
tester = "Second";
if(1)
{
    print(tester);
    tester = "Third";
    print(tester);
    tester = "Fourth";
    print(tester);
}
print(tester);
tester = "Fifth";
print(tester);

Integer y = x ^ 2;

Integer result = 0;

for(Integer idx = 0; idx < x; idx = idx + 1)
{
    result = result + x;
}

if(y == result)
{
    print("quick maf");
    x = 3;
}
else
{
    print("You don fcked up AARon"); // We shouldn't get here
    x = 7;
}
print(x);

while(x > 0)
{
    if(x / 2 == 0)
    {
        print("x is even ");
        print(x);
    }
    else
    {
        print("x is odd ");
        print(x);
    }
    x = x - 1;
}

print("done");