import paho.mqtt.client as mqtt

# MQTT 브로커 정보
BROKER = "test.mosquitto.org"  # 퍼블릭 브로커
PORT = 1883  # 기본 포트
TOPIC = "test/topic"

# 메시지가 도착했을 때 호출되는 콜백 함수
def on_message(client, userdata, msg):
    print(f"Received message: {msg.payload.decode()} on topic {msg.topic}")

# MQTT 클라이언트 생성
client = mqtt.Client()
client.on_message = on_message  # 메시지 수신 콜백 설정

# 브로커 연결
client.connect(BROKER, PORT, 60)

# 토픽 구독
client.subscribe(TOPIC)

# 메시지 게시
client.publish(TOPIC, "Hello MQTT! 1", qos=0)
client.publish(TOPIC, "Hello MQTT! 2", qos=0)
client.publish(TOPIC, "Hello MQTT! 3", qos=0)

# 메시지 처리 루프 실행
client.loop_start()

# 10초 후 루프 정지
import time
time.sleep(10)
client.loop_stop()
client.disconnect()
