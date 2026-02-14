import random

def random_numbers(count=100, start=0, end=1000, float_nums=False):
    if float_nums:
        for _ in range(count):
            yield round(random.uniform(start, end), 2)
    else:
        for _ in range(count):
            yield random.randint(start, end)

# Примеры использования:
print("Целые числа 0–1000:")
print(list(random_numbers(100, 0, 1000)))

print("\nВещественные числа от -100 до 100:")
print(list(random_numbers(100, -100, 100, float_nums=True)))