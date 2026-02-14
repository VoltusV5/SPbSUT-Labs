import math
import matplotlib.pyplot as plt

A = 1000
V = 360

# --- вычисляем логарифмы членов суммы ---
# ln( A^k / k! ) = k*ln(A) - ln(k!)
log_terms = [k * math.log(A) - math.lgamma(k + 1) for k in range(V + 1)]

# находим максимум для нормализации, чтобы не переполнялось
m = max(log_terms)

# знаменатель: sum( exp(term - m) )
den = sum(math.exp(t - m) for t in log_terms)

# ln(знаменателя) = m + ln(den)
ln_norm = m + math.log(den)

# вероятности P(x) = exp( log_term - ln_norm )
P = [math.exp(t - ln_norm) for t in log_terms]

# проверим
print("Первые 10 вероятностей:")
for x in range(10):
    print(f"P({x}) = {P[x]:.6e}")

print(f"\nP(V={V}) = {P[-1]:.6e}")
print(f"Сумма вероятностей = {sum(P):.6f}")