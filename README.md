# <p align="center"> MKlon3.5e
## <p align="center">Зарядное устройство на модулях ESP32DEVKITCV4, SAMD21 MINI и ILI9486 3.5".
## <p align="center">Проект Arduino, в разработке.
#### <p align="right">2023.11.19
### MKlon3.5e - это компактное ЗУ модульной конструкции с графическим интерфейсом и сенсорным управлением. Разработанное системное ПО позволяет реализовать пользовательские алгоритмы обслуживания аккумуляторных батарей, обладая лишь навыками программирования в среде Arduino, используя приведённые в проекте примеры. 

### Аппаратная часть проектировалась в среде Eagle v7.3.0, программная - в PlatformIO. 
<a name="menu"></a>
1. [Как это работает. Аппаратный аспект.](#hard)
2. [Как это работает. Программный аспект](#soft)
3. [Технические характеристики](#specifications)
4. [Документы](#docs)
5. [Полезные ссылки](#useful_links)
6. [About Me](#about)

***
![](https://github.com/olmoro/MKlon3.5v7/blob/main/documents/full/img/MKlon3.5full.png)

## <p align="center"> Как это работает.<a name="hard"></a>


***




В начало [^](#menu)
***
## <p align="center">2. Как это работает. Программный аспект.<a name="soft"></a>


#### <p align="center">Формат реализованных команд управления.
##### <p align="center">(Не отредактировано)
Имя | Код | Примечание
---------------------:|------:|-------:|
txReadUIS|0x10|Чтение напряжения,тока (мВ и мА) и состояния
txGetU|0x11|Чтение напряжения (мВ)|
txGetI|0x12|Чтение тока (мА)
txGetUI|0x13|Чтение напряжения (мВ) и тока (мА)
txGetState|0x14|Чтение состояния
txReady|0x15|Готовность
txPowerAuto|0x20|
txPowerStop|0x21|
txPowerMode|0x22|
txPowerVGo|0x22|
txPowerIGo|0x22|
txDischargeGo|0x24|


***
## <p align="center">3. Технические характеристики MKlon3.5<a name="specifications"></a>
 Параметр | Ед | Значение | Погрешность |
------------------------------------:|----:|------------:|----------------------:|
|Зарядный ток Iз|А|0.05 – 6.0|±(0.005 Iз + 0.05)|
|Шаг установки зарядного тока|А| 0.01|
|Зарядное напряжение U|В|1.0 – 18.0|±(0.005 U + 0.05)|
|Шаг установки зарядного напряжения|В|0.01|
|Разрядный ток Iр|А|0.05 – 3.0|±(0.005 Iр + 0.05)|
|Шаг установки разрядного тока|А| 0.01|
|Максимальная рассеиваемая мощность|Вт|не менее 40|

- Источник питания - ноутбучный 19 В (4.74 ... 6.0 А)
- Интерфейс - USB, WiFi, BT
- Индикация - TFT 3.5 RGB, сенсорная панель 
- Диагностика - 2 разъема для логического анализатора
- Micro-SD карта - на плате дисплея
- Защита по выходу - переполюсовка, перегрузка по току, КЗ

***




***
## <p align="center">4. Документация.<a name="docs"></a>





***
## <p align="center">8.Полезные ссылки<a name="useful_links"></a>
- [OSS](https://ru.wikipedia.org/wiki/%D0%9E%D1%82%D0%BA%D1%80%D1%8B%D1%82%D0%BE%D0%B5_%D0%BF%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D0%BD%D0%BE%D0%B5_%D0%BE%D0%B1%D0%B5%D1%81%D0%BF%D0%B5%D1%87%D0%B5%D0%BD%D0%B8%D0%B5)
- [Протокол обмена](http://leoniv.diod.club/articles/wake/wake.html)
- [Проекты на ESP32](https://randomnerdtutorials.com/projects-esp32/)
- [Как создать проект](https://dzen.ru/media/kotyara12/sozdanie-platformio--espidf-proekta-i-nastroika-platformioini-6324c68fb3d4c87d20ee80a6)
- [Как строятся графики](https://www.chartjs.org/)
- [Использование памяти в ESP32](https://dzen.ru/media/kotyara12/ispolzovanie-pamiati-v-esp32-63294a246a97da509e279eab)
- [Резистор в цепи затвора или как делать правильно](https://habr.com/en/articles/451152/)


В начало [^](#menu)
***
## <p align="center">9. About Me<a name="about"></a>
🚀 I'm a full stack developer urk2t@yandex.ru

В начало [^](#menu)