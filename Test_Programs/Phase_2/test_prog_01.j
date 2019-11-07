// This is a comment
Integer x = 5;

Integer y = x ^ 2;

Integer result = 0;

for(Integer idx = 0; idx < x; idx++)
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
    if(x / 2)
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