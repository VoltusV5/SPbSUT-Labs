import streamlit as st
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

def p_collisium(a):
    '''Посчёт вероятности коллизии аналитикой'''
    len = 100 * 8
    br = 250_000


    return 1-np.exp(-2*(len/br)*a)

a = np.array([i*0.1 for i in range(1, 21)])
imit1 = np.load("imit.npy")
if 'data' not in st.session_state:
    b = a[:9]
    st.session_state.data = pd.DataFrame({
        "a":b,
        "Имитация":imit1[:9],
        "Аналитика":p_collisium(b)})

edit = st.data_editor(st.session_state.data)



st.write("## График зависимости интенсивности нагрузки, от вероятности коллизий")

fig, ax = plt.subplots()
ax.plot(edit["a"], edit["Аналитика"], label="Аналитика", color="red")
ax.plot(edit["a"], edit["Имитация"], label="Имитация", color="blue")
ax.grid(True)
ax.legend()
ax.set_xlabel("Интенсивность нагрузки")
ax.set_ylabel("Вероятность коллизий")
st.pyplot(fig)

# imit1 = np.array(edit["Имитация"])
# np.save("imit", edit["Имитация"])

k = np.arange(1, 21)
data2 = pd.DataFrame({
        "Количество повторов фрейма":k,
        "Имитация":[0.204, 0.08, 0.026, 0.007, 0.002, 0.0007, 0.0005, 0.0003, 0.00035, 0.0004, 0.00044, 0.0007, 0.0008, 0.001, 0.002, 0.002,0.002, 0.003, 0.004, 0.004]
        })
   
edit2 = st.data_editor(data2)

st.write("## График зависимости интенсивности нагрузки, от вероятности доставки коллизий")

fig, ax = plt.subplots()
ax.plot(edit2["Количество повторов фрейма"], edit2["Имитация"], label="Имитация", color="blue")
ax.grid(True)
ax.legend()
ax.set_xlabel("Интенсивность нагрузки")
ax.set_ylabel("Вероятность доставки коллизий")
st.pyplot(fig)

imit2 = np.array(edit2["Имитация"])
np.save("imit2", edit2["Имитация"])