# Aucun n'import ne doit être fait dans ce fichier


def nombre_entier(n: int) -> str:
    return "S" * n + "0"


def S(n: str) -> str:
    return "S" + n


def addition(a: str, b: str) -> str:
    a_count = a.count('S')
    b_count = b.count('S')
    return "S" * (a_count + b_count) + "0"


def multiplication(a: str, b: str) -> str:
    a_count = a.count('S')
    b_count = b.count('S')
    return "S" * (a_count * b_count) + "0"


def facto_ite(n: int) -> int:
    result = 1
    for i in range(1, n + 1):
        result *= i
    return result


def facto_rec(n: int) -> int:
    if n == 0:
        return 1
    return n * facto_rec(n - 1)


def fibo_rec(n: int) -> int:
    if n <= 1:
        return n
    return fibo_rec(n - 1) + fibo_rec(n - 2)

def fibo_ite(n: int) -> int:
    if n == 0:
        return 0
    elif n == 1:
        return 1

    a, b = 0, 1
    for _ in range(2, n + 1):
        a, b = b, a + b
    return b


def golden_phi(n: int) -> float:
    if n == 0:
        return 0
    return fibo_ite(n + 1) / fibo_ite(n)


def sqrt5(n: int) -> float:
    x = 5.0
    for _ in range(n):
        x = (x + 5 / x) / 2
    return x


def pow(x: float, n: int) -> float:
    result = 1.0
    base = x
    while n > 0:
        if n % 2 == 1:
            result *= base
        base *= base
        n //= 2
    return result
