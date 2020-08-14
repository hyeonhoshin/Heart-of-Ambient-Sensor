# Batterless Ambient Light
## Tools
- [EasyEDA](https://easyeda.com) (PCB design) -> Order [JLCPCB](https://jlcpcb.com)
- [Engergia](http://energia.nu)
    - Tools to program MSP430 launchpad like arduino

## Materials
- Farsens Medusa-RM kit : RFID harvester with antenna + MSP430
- MSP-EXP430FR2433 LaunchPad : MSP430 develop kit + debugger(= uploader)
- OPT3001DNPR : Low power ambient light sensor with Digital converter to I2C

## Code Flow
* Setup : Sensor init -> Write 0xC810 -> Sleep mode
* In loop : Set the sensor as one shot mode(At one shot mode, the sensor reads lux only once. After that, it returns to Sleep mode)

## Tips
- Measured datum is saved in special way.
    - lux = 0.01 * 2^(Exponent) * Frac
    - Exponent : Result register's upper 4bits
    - Frace : Result resgister's lower 8bits
- I2C pins (MSP430 -> sensor or debugger)
    - P1.2 : SDA
    - P1.3 : SCL
    - /RST : SBWTDIO
    - TEST : SBWTCK
    - P1.5 : RXD
    - P1.4 : TXD
    - 3V3 : VREGL or VDD
    - GND : GLOAD
- Inititial design error : If you use v1.0, you have to wire VDD with V_REGL
- OPT3001's I2C ADDRESS : 0x44
- How to read/write resgister
    - Write : begin transmission -> send reg number -> send MSB -> send LSB -> end transmission
    - Read : Request data with address
        - Notice! : I2C sends data of finally selected register. So if you want to see result(0x00) or config(0x01). At first, you need to write it.