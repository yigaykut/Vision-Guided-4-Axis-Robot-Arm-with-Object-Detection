import cv2
import serial
import time
import requests
import base64

s_port = serial.Serial('COM4', 9600, timeout=1)
v_cap = cv2.VideoCapture(0)

time.sleep(2)

mx1, mx2, mx3, mx4 = 90, 90, 90, 20

while True:
    st, fr = v_cap.read()
    if not st:
        continue

    _, b_img = cv2.imencode('.jpg', fr)
    b64_str = base64.b64encode(b_img).decode('utf-8')

    payload = {
        "model": "Qwen2.5-VL-7B-Instruct",
        "messages": [
            {
                "role": "user",
                "content": [
                    {"type": "text", "text": "Locate the target object pack. Return 4 integer values separated by commas representing BaseAngle(0-180), ShoulderAngle(0-180), ElbowAngle(0-180), GripperState(20 for open, 90 for close). Output nothing else but the numbers."},
                    {"type": "image_url", "image_url": {"url": f"data:image/jpeg;base64,{b64_str}"}}
                ]
            }
        ],
        "temperature": 0.1
    }

    try:
        res = requests.post("http://localhost:11434/v1/chat/completions", json=payload, timeout=5)
        res_data = res.json()
        raw_out = res_data['choices'][0]['message']['content'].strip()
        
        if "," in raw_out:
            tokens = raw_out.split(",")
            mx1 = int(float(tokens[0].strip()))
            mx2 = int(float(tokens[1].strip()))
            mx3 = int(float(tokens[2].strip()))
            mx4 = int(float(tokens[3].strip()))
    except:
        pass

    cmd = f"#{mx1} {mx2} {mx3} {mx4}\n"
    s_port.write(cmd.encode())

    cv2.imshow('arm_view', fr)
    if cv2.waitKey(30) & 0xFF == ord('q'):
        break

v_cap.release()
s_port.close()
cv2.destroyAllWindows()