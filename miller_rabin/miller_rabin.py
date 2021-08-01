"""
miller-rabin prime test

Test if n is prime with error probability less than 2^(−𝑠).

"""

import random
from exponentiation import exp

Prime = 0
Composite = 1


def miller_rabin(n, s):
    if n == 2:
        return Prime
    elif n % 2 == 0:
        return Composite

    for _ in range(s):
        a = random.randint(1, n-1)
        if test(a, n) == True:
            return Composite

    return Prime

def test(a, n):
    u = 0
    t = 0
    temp = (n-1)
    while(1):
        if (temp % 2 !=1):#아직 짝수일때
            temp = temp // 2
            t += 1

        else:#홀수일때
            u = temp
            break


    x0 = exp(a, u, n)
    x1 = 0
    for i in range(1,t+1):

        x1 = exp(x0,2,n)
        if x1 == 1 and x0 != 1 and x0 != (n-1):
            return True
        x0 = x1

    if x1 != 1:
        return True
    return False



if __name__ == "__main__":

    primes = [7879, 7883, 7901, 7907, 7919, 7927, 7933, 7937, 7949, 7951,
              7963, 7993, 8009, 8011, 8017, 8039, 8053, 8059, 8069, 8081,
              8087, 8089, 8093, 8101, 8111, 8117, 8123, 8147, 8161, 8167]

    for p in primes:
        result = miller_rabin(p, 20)
        if result == Prime:
            print("Prime")
        elif result == Composite:
            print("Composite")
        else:
            print("Undefined")
