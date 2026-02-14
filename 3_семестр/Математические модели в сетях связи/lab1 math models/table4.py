from math import comb

def bernoulli_prob_le_p(n, n0):
    """
    Вероятность того, что истинная вероятность потерь <= оценке p_hat
    n  - общее число испытаний
    n0 - число неуспешных испытаний
    """
    p_hat = n0 / n
    return sum(comb(n, k) * (p_hat**k) * ((1 - p_hat)**(n - k)) for k in range(n0 + 1))

# Пример использования:
n = [1000, 1000, 1000]  # число испытаний
n0 = [90, 20, 5]        # число неуспешных

for i in range(len(n)):
    p_hat = n0[i] / n[i]
    prob = bernoulli_prob_le_p(n[i], n0[i])
    print(f"Эксперимент {i+1}: n={n[i]}, n0={n0[i]}, p_hat={p_hat:.3f}, P(истинное p ≤ p_hat)={prob:.3f}")