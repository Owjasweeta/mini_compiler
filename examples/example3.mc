// example3.mc — while loop: compute sum of 1..10
i = 1;
sum = 0;

while (i < 11) {
    sum = sum + i;
    i = i + 1;
}

print sum;   // prints 55
