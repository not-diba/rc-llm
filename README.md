# 🚗 VoiceBot — Voice-Controlled Autonomous RC Car with Local LLM

An experimental robotics project exploring how **small, local Large Language Models** can serve as the "brain" of a physical robot. Instead of cloud-based AI, this project runs a lightweight LLM (via [llama.cpp](https://github.com/ggerganov/llama.cpp) or similar) on a local machine to interpret voice commands, reason about the robot's environment, and translate high-level instructions into low-level motor actions — all through an Arduino-powered RC car.

---

## 🎯 Project Goal

**Primary Research Question:** Can small, purpose-trained local LLMs effectively control physical robots by understanding only what they need to — the robot's capabilities, its purpose, and how to achieve that purpose?

This project investigates a focused approach to LLM-powered robotics:

- **Not** a general-purpose AI assistant
- **Not** a cloud-dependent system with latency issues
- A **small local LLM** that deeply understands:
  - 🤖 **The Robot** — what hardware it has, what it can physically do (move, turn, stop, sense obstacles)
  - 🎯 **Its Purpose** — navigate an environment, respond to voice commands, avoid obstacles
  - 🛠️ **How to Achieve It** — translate natural language into sequences of serial commands for the Arduino

The long-term vision is to explore this pattern for **robotics applications in African markets** — agricultural automation, healthcare delivery, and infrastructure monitoring — where cloud connectivity is unreliable and edge AI is essential.

---

## 🏗️ Architecture Overview

```
┌─────────────┐     ┌──────────────────────┐     ┌─────────────┐
│  Microphone │────▶│   Host Computer      │────▶│   Arduino   │
│  (Voice In) │     │                      │     │   + Motors  │
└─────────────┘     │  ┌─────────────────┐ │     │   + Sensors │
                    │  │ Speech-to-Text  │ │     └──────┬──────┘
                    │  │ (Whisper.cpp)   │ │            │
                    │  └────────┬────────┘ │            │
                    │           ▼          │        RC Car
                    │  ┌─────────────────┐ │      (Physical)
                    │  │ Local LLM       │ │
                    │  │ (llama.cpp)     │ │
                    │  │                 │ │
                    │  │ System Prompt:  │ │
                    │  │ "You are a      │ │
                    │  │  robot brain.   │ │
                    │  │  You control    │ │
                    │  │  motors via     │ │
                    │  │  serial cmds"   │ │
                    │  └────────┬────────┘ │
                    │           ▼          │
                    │  ┌─────────────────┐ │
                    │  │ Serial Comms    │ │
                    │  │ (Python/USB)    │─┼──────▶ Arduino
                    │  └─────────────────┘ │
                    └──────────────────────┘
```

---

## 🧩 Components

### Hardware

| Component          | Purpose                         | Example                                                           |
| ------------------ | ------------------------------- | ----------------------------------------------------------------- |
| RC Car Chassis     | Physical platform with motors   | Any 2WD/4WD kit                                                   |
| Arduino (Uno/Nano) | Motor control, sensor reading   | [Arduino Uno](https://store.arduino.cc/products/arduino-uno-rev3) |
| Motor Driver       | Drive DC motors from Arduino    | L298N or L293D shield                                             |
| Ultrasonic Sensor  | Obstacle detection              | HC-SR04                                                           |
| USB Cable          | Serial communication to host    | USB-B or USB-C                                                    |
| Host Computer      | Runs LLM, STT, and orchestrator | Laptop / Raspberry Pi 4+ / Jetson Nano                            |
| Microphone         | Voice input                     | USB mic or laptop built-in                                        |

### Software

| Component        | Tool                                                    | Purpose                                            |
| ---------------- | ------------------------------------------------------- | -------------------------------------------------- |
| Local LLM        | [llama.cpp](https://github.com/ggerganov/llama.cpp)     | Natural language understanding & command reasoning |
| Speech-to-Text   | [whisper.cpp](https://github.com/ggerganov/whisper.cpp) | Convert voice to text locally                      |
| Orchestrator     | Python script                                           | Glue between STT → LLM → Serial                    |
| Arduino Firmware | C++ (.ino)                                              | Parse serial commands, drive motors, read sensors  |
| Serial Comms     | [pyserial](https://pyserial.readthedocs.io/)            | USB communication between host and Arduino         |

---

## 🔬 The Core Idea: Purpose-Scoped Local LLMs for Robotics

Most LLM-robotics research focuses on large cloud models (GPT-4, Claude) controlling robots via API calls. This project explores the opposite end of the spectrum:

### What if the LLM only needs to know three things?

**1. What am I?**

> "You are the brain of a small wheeled robot. You have 2 DC motors (left, right), one ultrasonic distance sensor (front-facing), and you communicate via serial commands."

**2. What is my purpose?**

> "You navigate a physical space based on voice commands from a human operator. You must avoid obstacles and execute movement instructions safely."

**3. How do I achieve my purpose?**

> "You receive transcribed voice commands. You output one or more serial commands from this set: `FWD <ms>`, `BWD <ms>`, `LEFT <deg>`, `RIGHT <deg>`, `STOP`, `SCAN`. You can chain commands. Always check for obstacles before moving forward."

This is the **system prompt** — the entire "training" the LLM needs. A 1-3B parameter model (like [TinyLlama](https://huggingface.co/TinyLlama), [Phi-2](https://huggingface.co/microsoft/phi-2), or [Gemma 2B](https://huggingface.co/google/gemma-2b)) running via llama.cpp should be sufficient for this constrained reasoning task.

### Why This Matters

- **No internet required** — critical for field robotics in areas with poor connectivity
- **Low latency** — local inference means faster response times
- **Privacy** — no data leaves the device
- **Cost** — no API fees, runs on affordable hardware
- **Customizable** — the system prompt fully defines robot behavior
- **Scalable pattern** — same architecture works for agricultural robots, delivery bots, monitoring drones

### Relevant Research & Resources

- 📄 [Google's SayCan](https://say-can.github.io/) — grounding LLMs in robot affordances
- 📄 [Microsoft's ChatGPT for Robotics](https://www.microsoft.com/en-us/research/group/autonomous-systems-group-robotics/articles/chatgpt-for-robotics/) — using LLMs as robot planners
- 📄 [Code as Policies](https://code-as-policies.github.io/) — LLMs generating robot control code
- 🛠️ [llama.cpp](https://github.com/ggerganov/llama.cpp) — efficient local LLM inference in C/C++
- 🛠️ [whisper.cpp](https://github.com/ggerganov/whisper.cpp) — local speech-to-text
- 🛠️ [Ollama](https://ollama.com/) — easy local LLM management (alternative to raw llama.cpp)
- 📄 [TinyML Foundation](https://www.tinyml.org/) — ML on microcontrollers
- 📄 [Edge Impulse](https://edgeimpulse.com/) — platform for edge AI development

---

## 📁 Project Structure

```
voicebot/
├── README.md
├── arduino/
│   └── voicebot_firmware/
│       └── voicebot_firmware.ino    # Arduino motor/sensor control
├── host/
│   ├── main.py                      # Main orchestrator
│   ├── stt.py                       # Speech-to-text (whisper.cpp wrapper)
│   ├── llm.py                       # LLM interface (llama.cpp wrapper)
│   ├── serial_comms.py              # Arduino serial communication
│   └── prompts/
│       └── robot_brain.txt          # System prompt defining robot identity
├── models/                          # Downloaded GGUF model files (gitignored)
├── docs/
│   ├── wiring_diagram.md            # Hardware wiring guide
│   ├── command_protocol.md          # Serial command specification
│   └── model_benchmarks.md          # LLM performance comparisons
├── tests/
│   ├── test_serial.py               # Serial communication tests
│   └── test_llm_commands.py         # LLM command generation tests
└── requirements.txt
```

---

## 🚀 Getting Started

### Phase 1: Basic Serial Communication

Get the Arduino and host computer talking reliably.

```cpp
// Arduino: voicebot_firmware.ino (simplified)
void setup() {
  Serial.begin(9600);
  // Setup motor pins...
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    if (cmd.startsWith("FWD")) {
      int duration = cmd.substring(4).toInt();
      moveForward(duration);
    }
    else if (cmd == "STOP") {
      stopMotors();
    }
    // ... handle other commands
  }
}
```

```python
# Host: serial_comms.py (simplified)
import serial
import time

arduino = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
time.sleep(2)  # Wait for Arduino reset

def send_command(cmd: str):
    arduino.write(f"{cmd}\n".encode())
    response = arduino.readline().decode().strip()
    return response

# Test
send_command("FWD 1000")  # Move forward 1 second
send_command("STOP")
```

### Phase 2: Add Local LLM Reasoning

```python
# Host: llm.py (simplified, using llama-cpp-python)
from llama_cpp import Llama

llm = Llama(model_path="./models/tinyllama-1.1b.Q4_K_M.gguf")

SYSTEM_PROMPT = """You are the brain of a small wheeled robot.

HARDWARE:
- 2 DC motors (left track, right track)
- 1 ultrasonic sensor (front, range: 2-400cm)
- Commands are sent via serial to an Arduino

AVAILABLE COMMANDS:
- FWD <milliseconds>  : Move forward
- BWD <milliseconds>  : Move backward
- LEFT <degrees>      : Turn left
- RIGHT <degrees>     : Turn right
- STOP                : Emergency stop
- SCAN                : Read ultrasonic sensor (returns distance in cm)

RULES:
1. Always SCAN before FWD to check for obstacles
2. If obstacle < 30cm, do not move forward — suggest alternative
3. Output ONLY commands, one per line, no explanation
4. For unknown requests, output STOP

Convert the human's voice command into a sequence of robot commands."""

def get_robot_commands(voice_text: str) -> list[str]:
    response = llm.create_chat_completion(
        messages=[
            {"role": "system", "content": SYSTEM_PROMPT},
            {"role": "user", "content": voice_text}
        ],
        max_tokens=100,
        temperature=0.1  # Low temperature for deterministic commands
    )
    commands = response['choices'][0]['message']['content'].strip().split('\n')
    return [cmd.strip() for cmd in commands if cmd.strip()]

# Example
cmds = get_robot_commands("go forward and then turn left")
# Expected output: ["SCAN", "FWD 1000", "LEFT 90"]
```

### Phase 3: Add Voice Input

```python
# Host: main.py — Full pipeline
from stt import transcribe_audio
from llm import get_robot_commands
from serial_comms import send_command

print("🤖 VoiceBot ready. Speak a command...")

while True:
    # 1. Listen and transcribe
    text = transcribe_audio()  # Uses whisper.cpp
    print(f"🎤 Heard: {text}")

    # 2. LLM reasoning
    commands = get_robot_commands(text)
    print(f"🧠 Commands: {commands}")

    # 3. Execute on hardware
    for cmd in commands:
        print(f"⚡ Sending: {cmd}")
        response = send_command(cmd)
        print(f"📡 Arduino: {response}")
```

---

## 🧪 Model Recommendations for Local Inference

| Model          | Parameters | RAM Needed | Best For                       | Link                                                                      |
| -------------- | ---------- | ---------- | ------------------------------ | ------------------------------------------------------------------------- |
| TinyLlama 1.1B | 1.1B       | ~1 GB      | Raspberry Pi, minimal hardware | [HuggingFace](https://huggingface.co/TinyLlama/TinyLlama-1.1B-Chat-v1.0)  |
| Phi-2          | 2.7B       | ~2 GB      | Good reasoning, small size     | [HuggingFace](https://huggingface.co/microsoft/phi-2)                     |
| Gemma 2B       | 2B         | ~1.5 GB    | Google's efficient small model | [HuggingFace](https://huggingface.co/google/gemma-2b)                     |
| Qwen2.5 1.5B   | 1.5B       | ~1.2 GB    | Strong instruction following   | [HuggingFace](https://huggingface.co/Qwen/Qwen2.5-1.5B-Instruct)          |
| SmolLM2 1.7B   | 1.7B       | ~1.3 GB    | HuggingFace's efficient model  | [HuggingFace](https://huggingface.co/HuggingFaceTB/SmolLM2-1.7B-Instruct) |

> **Tip:** Use GGUF quantized versions (Q4_K_M or Q5_K_M) for best performance/quality tradeoff on consumer hardware. llama.cpp and Ollama both support GGUF natively.

---

## 🗺️ Roadmap

- [x] Define project architecture and command protocol
- [ ] **Phase 1:** Arduino firmware + serial communication
- [ ] **Phase 2:** LLM integration with llama.cpp
- [ ] **Phase 3:** Voice input with whisper.cpp
- [ ] **Phase 4:** Obstacle avoidance loop (SCAN → reason → act)
- [ ] **Phase 5:** Sensor fusion (add IR sensors, IMU)
- [ ] **Phase 6:** Fine-tune a small model on robot command pairs
- [ ] **Phase 7:** Port to Jetson Nano / Raspberry Pi for fully onboard inference

---

## 🌍 Future Vision: Local LLMs for African Robotics

This project is a stepping stone toward a larger goal: deploying intelligent robots in African contexts where cloud AI isn't practical.

**Potential applications using this same architecture:**

- 🌾 **Agricultural Robots** — Voice-commanded weeding/spraying bots for smallholder farms in Kenya
- 🏥 **Healthcare Delivery** — Autonomous delivery of medical supplies to rural clinics
- 🏙️ **Infrastructure Monitoring** — Robots that inspect roads, bridges, or water infrastructure
- 📦 **Last-Mile Delivery** — Navigating informal settlement layouts

**The pattern stays the same:**

1. Define what the robot _is_ (hardware capabilities)
2. Define what the robot _does_ (its purpose)
3. Let a small local LLM bridge human language → robot actions

---

## 📚 Learning Resources

### LLMs for Robotics

- [Awesome LLM Robotics (GitHub)](https://github.com/GT-RIPL/Awesome-LLM-Robotics) — curated paper list
- [llama.cpp guide](https://github.com/ggerganov/llama.cpp/blob/master/docs/build.md) — building and running local models
- [llama-cpp-python](https://github.com/abetlen/llama-cpp-python) — Python bindings for llama.cpp
- [Ollama Docs](https://github.com/ollama/ollama/blob/main/docs/api.md) — easy local model serving

### Arduino & Robotics

- [Arduino Official Tutorials](https://docs.arduino.cc/built-in-examples/)
- [PySerial Documentation](https://pyserial.readthedocs.io/en/latest/)
- [L298N Motor Driver Guide](https://lastminuteengineers.com/l298n-dc-motor-arduino-tutorial/)
- [HC-SR04 Ultrasonic Sensor Guide](https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/)

### Edge AI & TinyML

- [TinyML Book (O'Reilly)](https://www.oreilly.com/library/view/tinyml/9781492052036/)
- [Edge Impulse Tutorials](https://docs.edgeimpulse.com/docs)
- [NVIDIA Jetson Getting Started](https://developer.nvidia.com/embedded/learn/get-started-jetson-nano-devkit)

---

## 🤝 Contributing

This is a personal learning project, but ideas, suggestions, and discussions are welcome! If you're interested in local LLMs for robotics — especially in African contexts — feel free to open an issue or reach out.

---

## 📄 License

MIT License — use this however you'd like.

---

> _"The best AI for a robot isn't the biggest one — it's the one that understands exactly what the robot needs to do."_
