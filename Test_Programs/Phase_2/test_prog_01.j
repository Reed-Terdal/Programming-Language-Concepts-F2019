// This is a comment
Integer x = 5;

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

Integer val = "test" == "temp" + 1;
print(val);

String temp = "test";
String letter = "";

while(x > 4)
{
    letter = charAt(temp,x);
    print(letter);
    if(x / 2)
    {
         if(x == 4)
         {
             print("x is 4 now");
         }
        print("x is even ");
        print(x);
    }
    else
    {
        if(x == 5)
        {
             print("x is 5 now");
        }
        print("x is odd ");
        print(x);
    }
    x = x - 1;
}

print("done");