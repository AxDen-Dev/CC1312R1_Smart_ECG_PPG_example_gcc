# Sub-G Smart ECG PPG

---

<table>
<tr align="center">
  <td> TOP </td>
</tr>
  <tr align="center">
    <td><img src="./asset/board_top.jpg"></td>
  </tr>
  <tr align="center">
    <td> BOTTOM </td>
  </tr>
    <tr align="center">
      <td><img src="./asset/board_bottom.jpg"></td>
    </tr>
</table>

----

Sub-G Smart ECG PPG 은 AxDen 의 Aggregator Platform 를 이용하여 ECG(심전도), Heart rate(심박수), SPO2(산소포화도), 온도 와 같은 핵심적인 활력 징후(Vital Signs) 정보를 수집합니다.
<br>
1Km 의 이상의 장거리 통신이 필요한 다양한 서비스 시나리오를 빠르게 테스트 할 수 있도록 제공하는 예제입니다.
<br>
<br>
Sub-G Smart ECG PPG collects key vital signs information such as ECG, heart rate, SPO2, and temperature using AxDen's Aggregator Platform.
<br>
This is an example that provides quick testing of various service scenarios that require long-distance communication of 1Km or more.
<br>

### Sub-G Smart ECG PPG 의 주요 특징 및 기능

MCU | 설명
:-------------------------:|:-------------------------:
CC1312R1 | TI ROTS, EasyLink

*TI Sensor Controller 이용한 저전력 기술은 Production version 제품군에서 확인하실 수 있습니다.*

센서 | 설명
:-------------------------:|:-------------------------:
MAX30003 | ECG(심전도), Heart rate(심박수) sensor
MAX30101 | SPO2(산소포화도), Heart rate(심박수) sensor
SI7051 | Temperature sensor

*체온 측정이 필요한 경우 development@axden.io 로 문의 주시기 바랍니다.*

Sub-G Smart ECG PPG 예제는 ECG(심전도), Heart rate(심박수), SPO2(산소포화도), 온도 와 같은 핵심적인 활력 징후(Vital Signs) 정보를 수집하고 Sub-G 장거리 통신을 이용하여 전송합니다.
<br>
<br>
AxDen Aggregator Platform 과 연동하여 서버, DB 와 같은 인프라 구축 없이 Web 과 Mobile 에서 센서 정보를 확인합니다.
<br>
<br>
AxDen Aggregator Platform 에 저장된 센서 정보를 이용하여 Edge AI 를 학습시킵니다.
<br>

#### 터미널을 이용한 확인 방법
어댑터 보드가 있다면 터미널을 통해 통신 확인이 가능합니다.
<br>
<br>
어덥테 보드와 Sub-G Smart ECG PPG 를 PC 에 연결합니다.
<br>
<br>
screen 및 putty 등 시리얼 프로그램을 통해서 확인할 수 있으며, Baudrate 는 9600 입니다.
<table>
  <tr align="center">
    <td>RF RX example terminal</td>
    <td>RF TX example terminal</td>
  </tr>
  <tr align="center">
    <td><img src="./asset/serial_rx.png"></td>
    <td><img src="./asset/serial_tx.png"></td>
  </tr>
  <tr align="center">
    <td>ECG Graph</td>
    <td>PPG Graph</td>
  </tr>
  <tr align="center">
    <td><img src="./asset/ECG_output.png"></td>
    <td><img src="./asset/PPG_output.png"></td>
  </tr>
</table>

#### AxDen Aggregator Platform 을 이용한 확인 방법
AxDen Aggregator 홈페이지에서 회원 가입 후 Sub-G Smart ECG PPG 의 MAC Address 를 등록합니다.
<br>
AxDen Aggregator 홈페이지에서 제공하는 COMPANY ID, DEVICE ID 를 Protocol.h 파일의 COMPANY_ID, DEVICE_ID 에 입력합니다.
<br>
<br>
`#define COMPANY_ID 0`
<br>
`#define DEVICE_TYPE 0`
<br>
<br>
컴파일 후 플래싱을 합니다.
<br>
<br>
터미널을 통해 COMPANY_ID, DEVICE_ID 가 정상적으로 적용되었는지 확인합니다.
<br>
<br>
센서 정보를 Web 또는 Mobile 에서 확인할 수 있습니다.
<br>


### Hardware 핀맵
board_define.h 파일에서 확인 가능합니다.

### [펌웨어 설정 및 컴파일](https://github.com/AxDen-Dev/CC1312R1_Ping_Pong_example_gcc)

### [펌웨어 플래싱](https://github.com/AxDen-Dev/CC1312R1_Ping_Pong_example_gcc)
