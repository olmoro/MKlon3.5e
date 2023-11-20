# <p align="center"> MKlon3.5e
## <p align="center">Зарядное устройство на модулях ESP32DEVKITCV4, SAMD21 MINI и ILI9486 3.5".
## <p align="center">Проект Arduino, в разработке.
#### <p align="right">2023.11.20
### MKlon3.5e - это компактное ЗУ модульной конструкции с графическим интерфейсом и сенсорным управлением. Разработанное системное ПО позволяет реализовать пользовательские алгоритмы обслуживания аккумуляторных батарей, обладая лишь навыками программирования в среде Arduino, используя приведённые в проекте примеры. 

### Аппаратная часть проектировалась в среде Eagle v7.3.0, программная - в PlatformIO. 
<a name="menu"></a>
1. [Как это работает. Аппаратный аспект.](#hard)
2. [Как это работает. Программный аспект](#soft)
3. [Технические характеристики](#specifications)
4. [Тесты](#tests)
5. [Документы](#docs)
6. [Полезные ссылки](#useful_links)
7. [About Me](#about)

***
![](https://github.com/olmoro/MKlon3.5v7/blob/main/documents/full/img/MKlon3.5full.png)

## <p align="center"> Как это работает.<a name="hard"></a>

Зарядное устройство состоит из двух блоков, каждый на печатной плате размером 100*65 мм. 
Межблочное соединение - высокий 16-контактный разъем.
- Блок управления служит материнской платой для 38-контактного модуля [ESP32_DEVKITC_V4](https://aliexpress.ru/item/1005002443255665.html?spm=a2g2w.cart.cart_split.2.27194aa6uI5WfP&sku_id=12000020676652656&_ga=2.47565607.1828311252.1683559036-197161830.1681374836), 28-контактного  [SAMD21 M0-Mini](https://aliexpress.ru/item/1005002585114846.html?sku_id=12000021271636987&spm=a2g2w.productlist.search_results.0.58ef4aa657Lxkj), а также 3,5-дюймового дисплея [480*320 TFT ILI9486](https://aliexpress.ru/item/1005001999296476.html?spm=a2g2w.orderdetail.0.0.61a14aa6WcFlJS&sku_id=12000018365356569).

- Для силового блока "донором" послужил [XL4016 DC-DC Max 9A 300W](https://aliexpress.ru/item/32661166685.html?_ga=2.264151500.1168692661.1643047564-769654542.1642920280&sku_id=12000020944321893&spm=a2g39.orderlist.0.0.60754aa6RxSv8u), электролитические конденсаторы, шунты и дроссель которого были перенесены на плату прибора. В авторском варианте для подключения к источнику, аккумуляторной батарее и нагрузке использованы 6-амперные ноутбучные разъёмы. Второй вариант (он дешевле) предполагает использование двухконтактных разъёмов "под винт". 

[^](#menu)
***
## <p align="center">2. Как это работает. Программный аспект.<a name="soft"></a>
 Пользовательский интерфейс прибора реализован на ESP32, отлично приспособленном для этой задачи, а вот для чего он не приспособлен, так измерений с заданной погрешностью. Обычно это решается использованием INA226, но это всего лишь измеритель. Здесь же применен микроконтроллер на SAMD21, производящий не только измерения, причем с дифференциальными входами, но и математическую обработку, и генерацию сигнала для драйверов силового блока, сводя до разумного минимума время реакции на аварийные ситуации, возникающие в процессе работы. Связь между модулями производится по специальному протоколу через асинхронный интерфейс UART командами:
- txReadUIS() - Чтение напряжения, тока (мВ и мА) и двух байт состояния. Команда фоновая, запускается автоматически с периодом 0,1 секунды. Полученные данные выводятся на дисплей.
- txPowerAuto(spV, spI) - Запуск ПИД-регулятора в режиме заряда с заданными максимальными уровнями напряжения и тока.
- txPowerStop() - Команда прекратить заряд или разряд.
- txDischargeGo(spI) - Запуск ПИД-регулятора в режиме разряда с заданным максимальным уровнем тока.
- txVoltageAdj(spV) - Команда регулировки напряжения без остановки заряда.
- txCurrentAdj(spI)  - Команда регулировки тока без остановки заряда.
- txDiscurrentAdj(spD) - Команда регулировки тока без остановки разряда.
- txSetPidCoeff(kp, ki, kd) - Команда ввода коэффициентов ПИД-регулятора.
- и ещё полсотни команд для настроек под аппаратную реализацию пользователя. ПО модуля разрабатывается в другом проекте, [здесь](https://github.com/olmoro/MKlon3.5e/blob/main/documents/fwMoroPid) .bin файл прошивки.

[^](#menu)
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

[^](#menu)
***
## <p align="center">4. Тесты.<a name="tests"></a>
[Заряд АКБ Са-Са 60Ач, старт](https://github.com/olmoro/MKlon3.5e/blob/main/documents/tests/ch_60go.png)
[Разряд максимальным током](https://github.com/olmoro/MKlon3.5e/blob/main/documents/tests/dis_type.png)
[Работа системы теплоотвода](https://github.com/olmoro/MKlon3.5e/blob/main/documents/tests/Dis6A_t36.6.png)
[Разряд минимальным током](https://github.com/olmoro/MKlon3.5e/blob/main/documents/tests/dis_min.png)

[^](#menu)
***
## <p align="center">5. Документация.<a name="docs"></a>

- [Схемы](https://github.com/olmoro/MKlon3.5e/tree/main/documents/sch) 


[^](#menu)
***
## <p align="center">6.Полезные ссылки<a name="useful_links"></a>
- [OSS](https://ru.wikipedia.org/wiki/%D0%9E%D1%82%D0%BA%D1%80%D1%8B%D1%82%D0%BE%D0%B5_%D0%BF%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D0%BD%D0%BE%D0%B5_%D0%BE%D0%B1%D0%B5%D1%81%D0%BF%D0%B5%D1%87%D0%B5%D0%BD%D0%B8%D0%B5)
- [Протокол обмена](http://leoniv.diod.club/articles/wake/wake.html)
- [Проекты на ESP32](https://randomnerdtutorials.com/projects-esp32/)
- [Как создать проект](https://dzen.ru/media/kotyara12/sozdanie-platformio--espidf-proekta-i-nastroika-platformioini-6324c68fb3d4c87d20ee80a6)
- [Как строятся графики](https://www.chartjs.org/)
- [Использование памяти в ESP32](https://dzen.ru/media/kotyara12/ispolzovanie-pamiati-v-esp32-63294a246a97da509e279eab)
- [Резистор в цепи затвора или как делать правильно](https://habr.com/en/articles/451152/)

[^](#menu)
***
## <p align="center">7. About Me<a name="about"></a>
🚀 I'm a full stack developer urk2t@yandex.ru

[^](#menu)