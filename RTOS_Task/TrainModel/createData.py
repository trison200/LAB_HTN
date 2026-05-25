import numpy as np
import pandas as pd

NUM_NORMAL = 800
NUM_ABNORMAL = 200

# Sinh số nguyên (từ min đến max)
normal_temp = np.random.randint(20, 31, NUM_NORMAL) 
normal_hum = np.random.randint(40, 61, NUM_NORMAL) 
normal_labels = np.zeros(NUM_NORMAL, dtype=int)

abnormal_temp = np.random.randint(35, 51, NUM_ABNORMAL) 
abnormal_hum = np.random.randint(70, 91, NUM_ABNORMAL)  
abnormal_labels = np.ones(NUM_ABNORMAL, dtype=int)

temp = np.concatenate([normal_temp, abnormal_temp])
humidity = np.concatenate([normal_hum, abnormal_hum])
labels = np.concatenate([normal_labels, abnormal_labels])

df = pd.DataFrame({
    'temp': temp,
    'humidity': humidity,
    'label': labels
})

df = df.sample(frac=1).reset_index(drop=True)

df.to_csv("sensor_data.csv", index=False, header=False)

print("Đã tạo file csv")