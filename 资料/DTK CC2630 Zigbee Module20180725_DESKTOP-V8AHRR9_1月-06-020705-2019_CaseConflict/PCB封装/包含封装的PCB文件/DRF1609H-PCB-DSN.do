# Template Do File For Protel 99 -> Specctra Autorouter
# Protel International Pty Ltd
# 25-Jun-1999
#
unit mil
bestsave on C:\Users\YIHUA\Desktop\DTK CC2630 Zigbee Module20180220\PCB封装\包含封装的PCB文件\DRF1609H-PCB-DSN.bst
status_file C:\Users\YIHUA\Desktop\DTK CC2630 Zigbee Module20180220\PCB封装\包含封装的PCB文件\DRF1609H-PCB-DSN.sts
grid smart (wire 1) (via 1)
smart_route
critic

#enable the spread and miter features if you have the DFM option
#spread
#miter

# If you have the DFM module use spread and miter instead of the following. 
# Comment these lines out
Center
Recorner Diagonal 2000 2000 2000
Recorner Diagonal 1000 1000 1000
Recorner Diagonal 500 500 500
Recorner Diagonal 250 250 250
Recorner Diagonal 125 125 125
Recorner Diagonal 100 100 100
Recorner Diagonal 50 50 50
Recorner Diagonal 25 25 25
Recorner Diagonal 10 10 10
# Stop commenting here if you have the DFM module


write  routes      C:\Users\YIHUA\Desktop\DTK CC2630 Zigbee Module20180220\PCB封装\包含封装的PCB文件\DRF1609H-PCB-DSN.rte
write  wires       C:\Users\YIHUA\Desktop\DTK CC2630 Zigbee Module20180220\PCB封装\包含封装的PCB文件\DRF1609H-PCB-DSN.w
report conflicts   C:\Users\YIHUA\Desktop\DTK CC2630 Zigbee Module20180220\PCB封装\包含封装的PCB文件\DRF1609H-PCB-DSN.rcf
report corners     C:\Users\YIHUA\Desktop\DTK CC2630 Zigbee Module20180220\PCB封装\包含封装的PCB文件\DRF1609H-PCB-DSN.rcn
report rules       C:\Users\YIHUA\Desktop\DTK CC2630 Zigbee Module20180220\PCB封装\包含封装的PCB文件\DRF1609H-PCB-DSN.rrl
report status      C:\Users\YIHUA\Desktop\DTK CC2630 Zigbee Module20180220\PCB封装\包含封装的PCB文件\DRF1609H-PCB-DSN.rst
report unconnect   C:\Users\YIHUA\Desktop\DTK CC2630 Zigbee Module20180220\PCB封装\包含封装的PCB文件\DRF1609H-PCB-DSN.ruc
report vias        C:\Users\YIHUA\Desktop\DTK CC2630 Zigbee Module20180220\PCB封装\包含封装的PCB文件\DRF1609H-PCB-DSN.rva
quit
