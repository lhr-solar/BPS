EESchema Schematic File Version 4
LIBS:BPSMaster-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 8
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 4700 1400 0    50   ~ 10
This page hods other miscellenious connections that are required:\n- Strobe/Error light indicator\n- Fan connectors
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 5C39DF85
P 8350 2450
F 0 "J?" H 8430 2442 50  0000 L CNN
F 1 "Fan1" H 8430 2351 50  0000 L CNN
F 2 "" H 8350 2450 50  0001 C CNN
F 3 "~" H 8350 2450 50  0001 C CNN
	1    8350 2450
	1    0    0    -1  
$EndComp
Text Notes 8200 2150 0    100  ~ 20
Fans
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 5C39DFFD
P 8350 2750
F 0 "J?" H 8430 2742 50  0000 L CNN
F 1 "Fan2" H 8430 2651 50  0000 L CNN
F 2 "" H 8350 2750 50  0001 C CNN
F 3 "~" H 8350 2750 50  0001 C CNN
	1    8350 2750
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 5C39E02D
P 8350 3050
F 0 "J?" H 8430 3042 50  0000 L CNN
F 1 "Fan3" H 8430 2951 50  0000 L CNN
F 2 "" H 8350 3050 50  0001 C CNN
F 3 "~" H 8350 3050 50  0001 C CNN
	1    8350 3050
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 5C39E047
P 8350 3350
F 0 "J?" H 8430 3342 50  0000 L CNN
F 1 "Fan4" H 8430 3251 50  0000 L CNN
F 2 "" H 8350 3350 50  0001 C CNN
F 3 "~" H 8350 3350 50  0001 C CNN
	1    8350 3350
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR?
U 1 1 5C39E1C8
P 8050 2350
F 0 "#PWR?" H 8050 2200 50  0001 C CNN
F 1 "+12V" H 8065 2523 50  0000 C CNN
F 2 "" H 8050 2350 50  0001 C CNN
F 3 "" H 8050 2350 50  0001 C CNN
	1    8050 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 2350 8050 2450
Wire Wire Line
	8050 2450 8150 2450
Wire Wire Line
	8050 2450 8050 2750
Wire Wire Line
	8050 2750 8150 2750
Connection ~ 8050 2450
Wire Wire Line
	8050 2750 8050 3050
Wire Wire Line
	8050 3050 8150 3050
Connection ~ 8050 2750
Wire Wire Line
	8050 3050 8050 3350
Wire Wire Line
	8050 3350 8150 3350
Connection ~ 8050 3050
$Comp
L power:GND #PWR?
U 1 1 5C39E259
P 7950 3550
F 0 "#PWR?" H 7950 3300 50  0001 C CNN
F 1 "GND" H 7955 3377 50  0000 C CNN
F 2 "" H 7950 3550 50  0001 C CNN
F 3 "" H 7950 3550 50  0001 C CNN
	1    7950 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	7950 3550 7950 3450
Wire Wire Line
	7950 3450 8150 3450
Wire Wire Line
	8150 3150 7950 3150
Wire Wire Line
	7950 3150 7950 3450
Connection ~ 7950 3450
Wire Wire Line
	8150 2850 7950 2850
Wire Wire Line
	7950 2850 7950 3150
Connection ~ 7950 3150
Wire Wire Line
	8150 2550 7950 2550
Wire Wire Line
	7950 2550 7950 2850
Connection ~ 7950 2850
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 5C39EBB1
P 6350 2450
F 0 "J?" H 6430 2442 50  0000 L CNN
F 1 "ErrorLightConnector" H 6430 2351 50  0000 L CNN
F 2 "" H 6350 2450 50  0001 C CNN
F 3 "~" H 6350 2450 50  0001 C CNN
	1    6350 2450
	1    0    0    -1  
$EndComp
Text Notes 6000 2150 0    100  ~ 20
Error Light
$EndSCHEMATC
