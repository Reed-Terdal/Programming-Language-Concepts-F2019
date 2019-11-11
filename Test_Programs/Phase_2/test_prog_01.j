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

while(x > 0)
{

    x = x - 1;
}

print("done");