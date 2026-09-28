# Seat‑Navigator  
### ～ μT‑Kernel × NPU推論で空席を探索し自律移動する車いすロボット ～

## Overview
* Seat‑Navigator は、施設内での自動運転車いすの実現を目指して開発した自律移動ロボットです  
* 食堂などの共有スペースにおいて、車いす利用者が安全に移動・着席できるよう、カメラで椅子を検出し、空席を判断して自律的に移動する機能を備えています  
* AI 推論には YOLOX‑Tiny（COCO）モデルを使用し、RUHMI Model Converter により RA8P1 の NPU 向けに最適化することで、軽量かつ高速なリアルタイム推論を実現しています  
* 移動制御はオドメトリと ToF 距離センサを組み合わせ、μT‑Kernel のマルチタスク構成により安定したリアルタイム動作を可能にしています

---

## Features
- YOLOX‑Tiny（COCO）による椅子検出  
- RUHMI Model Converter による NPU 最適化推論  
- オドメトリによる位置推定  
- ToF 距離センサによる安全停止  
- μT‑Kernel のマルチタスク制御  
- micro‑ROS（UART）による ROS 2 ホストとの通信

---

## μT‑Kernel Task Structure
`Application/` ディレクトリに μT‑Kernel の全タスク実装を配置しています。

- **Control Task**：走行制御・状態管理  
- **Sensor Task**：ToF 距離センサの取得  
- **Motor Tasks**：左右タイヤの PWM 制御  
- **Camera Task**：VIN 画像取得・前処理  
- **Display Task**：状態表示  
- **ROS Task**：UART 経由で micro‑ROS 通信

---

## Build Environment
本プロジェクトは以下の環境で動作確認しています。

- **IDE**: e2 studio Version: 2026-04.2 (26.4.2)  
- **FSP**: 6.4.0  
- **Toolchain**: GCC ARM Embedded 13.2.1.arm‑13‑7  
- **Device**: RA8P1  
- **RTOS**: μT‑Kernel  
- **AI Model**: YOLOX‑Tiny (COCO)  
- **Model Optimization**: RUHMI Model Converter  
- **micro‑ROSライブラリ**: RA8P1 用 `libmicroros.a`（UART transport）

---

## Directory Structure
```
SeatNavigator/
├─ src/
│   ├─ microros/
│   │   └─ lib/              # RA8P1 用にビルド済みの micro‑ROS ライブラリ
│   └─ ...                   # FSP API を直接叩く低レベル制御コード
├─ Application/              # μT‑Kernel タスク群（Control, Sensor, Motor, Camera, Display, ROS）
├─ ra/                       # FSP が生成するデバイス設定コード
├─ script/                   # 必要に応じて使用するスクリプト
├─ configuration.xml         # FSP 設定ファイル
└─ README.md
```


---

## License
MIT License

