// This is a comment
Integer x = 20;

Integer y = x ^ 2;

Integer result = 0;

for(Integer idx = 0; idx < x; idx = idx + 1)
{
    result = result + x;
}
if(y == result)
{
    print("quick maf");
    x = 15;
}
else
{
    print("You don fcked up AARon"); // We shouldn't get here
    x = 7;
}
print(x);

Integer val = "test" == "test" + 10 > 4;
print(val);

val = "test" == "tesy" + 10;
print(val);

String temp = "test";
String letter = "";

while(x > 0)
{
    if(x < 5)
    {
         if(x == 2)
         {
             print("x is 2 now");
         }
        print("x less than 5 ");
    }
    else
    {
        if(x == 8)
        {
             print("x is 8 now");
        }
        print("x is over 5 ");
    }
    x = x - 1;
}

print("done");