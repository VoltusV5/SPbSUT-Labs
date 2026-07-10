import matplotlib.pyplot as plt
import numpy as np

directions = ['С', 'СВ', 'В', 'ЮВ', 'Ю', 'ЮЗ', 'З', 'СЗ']
angles = np.deg2rad([0, 45, 90, 135, 180, 225, 270, 315])

# Данные Иркутск
jan_freq = [6, 9, 14, 34, 3, 1, 7, 26]
jan_speed = [1.3, 1.0, 1.6, 2.9, 2.1, 0.7, 2.0, 2.8]
jul_freq = [4, 2, 5, 32, 9, 6, 18, 24]
jul_speed = [2.0, 1.3, 1.8, 2.2, 1.7, 1.4, 2.3, 3.0]

# ==================== 1. РОЗА ВЕТРОВ — ЯНВАРЬ ====================
fig1 = plt.figure(figsize=(8, 8))
ax1 = fig1.add_subplot(111, projection='polar')
ax1.bar(angles, jan_freq, width=0.4, color='skyblue', alpha=0.85, edgecolor='black')
ax1.set_title('Роза ветров — Январь (Иркутск)', fontsize=15, pad=20)
ax1.set_theta_zero_location('N')
ax1.set_theta_direction(-1)
ax1.set_thetagrids(np.degrees(angles), directions, fontsize=13)
ax1.set_ylim(0, 40)
for a, f, s in zip(angles, jan_freq, jan_speed):
    ax1.text(a, f + 1.5, f'{s}', ha='center', va='bottom', fontsize=11, fontweight='bold')
plt.savefig('роза_ветров_январь.png', dpi=300, bbox_inches='tight')
plt.show()

# ==================== 2. РОЗА ВЕТРОВ — ИЮЛЬ ====================
fig2 = plt.figure(figsize=(8, 8))
ax2 = fig2.add_subplot(111, projection='polar')
ax2.bar(angles, jul_freq, width=0.4, color='lightgreen', alpha=0.85, edgecolor='black')
ax2.set_title('Роза ветров — Июль (Иркутск)', fontsize=15, pad=20)
ax2.set_theta_zero_location('N')
ax2.set_theta_direction(-1)
ax2.set_thetagrids(np.degrees(angles), directions, fontsize=13)
ax2.set_ylim(0, 40)
for a, f, s in zip(angles, jul_freq, jul_speed):
    ax2.text(a, f + 1.5, f'{s}', ha='center', va='bottom', fontsize=11, fontweight='bold')
plt.savefig('роза_ветров_июль.png', dpi=300, bbox_inches='tight')
plt.show()

# ==================== 3. СХЕМА ЗАДЫМЛЕНИЯ ====================
fig3, ax3 = plt.subplots(subplot_kw={'projection': 'polar'}, figsize=(9, 9))
ax3.set_theta_zero_location('N')
ax3.set_theta_direction(-1)
ax3.set_thetagrids(np.degrees(angles), directions, fontsize=13)

theta = np.linspace(np.deg2rad(292.5), np.deg2rad(337.5), 200)   # сектор СЗ
r50, rm, r500 = 50, 279, 500

ax3.fill(theta, np.full_like(theta, r500), color='red', alpha=0.25, label='Январь (сплошная)')
ax3.fill(theta, np.full_like(theta, r500), color='blue', alpha=0.25, label='Июль (пунктирная)')
ax3.plot(theta, np.full_like(theta, r50), 'k--', lw=1.5)
ax3.plot(theta, np.full_like(theta, rm), 'k-', lw=2.5)
ax3.plot(theta, np.full_like(theta, r500), 'k--', lw=1.5)

ax3.text(np.deg2rad(315), 65, 'x₅₀ = 50 м', ha='center', fontsize=12, fontweight='bold')
ax3.text(np.deg2rad(315), 300, 'xₘ = 279 м\n(максимум)', ha='center', fontsize=12, fontweight='bold')
ax3.text(np.deg2rad(315), 520, 'x₅₀₀ = 500 м', ha='center', fontsize=12, fontweight='bold')

ax3.set_title('Схема задымления\n(преобладающий ветер ЮВ → СЗ)\nuₘ = 2,65 м/с', fontsize=14, pad=25)
ax3.legend(loc='upper right', bbox_to_anchor=(1.4, 1.05))
plt.savefig('схема_задымления.png', dpi=300, bbox_inches='tight')
plt.show()

print('✅ Три графика сохранены: роза_ветров_январь.png, роза_ветров_июль.png, схема_задымления.png')