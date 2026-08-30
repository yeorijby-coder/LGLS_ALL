# -*- coding: utf-8 -*-
"""PlcAddressMap.xml 에서 실주소를 계산한다 (추측 금지 - 단일 기준에서 파생)."""
import sys, io, xml.etree.ElementTree as ET
sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8')

T = ET.parse('TASK/WCS_TASK_CV_BIN/7_DeviceMap/PlcAddressMap.xml').getroot()

def grp(t):
    for g in T.findall('EquipGroup'):
        if g.get('type') == t: return g
    raise SystemExit('no group ' + t)

def block(g, name):
    for b in g.findall('Block'):
        if b.get('name') == name: return b
    raise SystemExit('no block ' + name)

def base(g, b, no):
    return int(b.get('origin')) + (no - int(g.get('numberFrom'))) * int(b.get('stride'))

def sig(g, bname, signame, no, slot=None):
    b = block(g, bname)
    bs = base(g, b, no)
    for s in list(b):
        if s.get('name') == signame:
            off = int(s.get('offset'))
            if s.tag == 'SignalArray':
                off += (slot or 0) * int(s.get('perSlot', 1))
            addr = bs + off
            dev = b.get('device')
            if dev == 'M':
                disp = '%%MX%d  (M%03d.%d)' % (addr, addr // 16, addr % 16)
            elif dev == 'D':
                disp = '%%DB%d  (D워드 %d)' % (addr * 2, addr)
            else:
                disp = '%%RB%d  (R워드 %d)' % (addr * 2, addr)
            return addr, dev, disp, b.get('legacy', ''), s.get('tag', '')
    raise SystemExit('no signal ' + signame)

CV, SC, RGV = grp('CV'), grp('SC'), grp('RGV')

def show(title, *r):
    a, dev, disp, leg, tag = r
    print('%-38s %-28s dev=%s  구ECS기준=%s  tag=%s' % (title, disp, dev, leg, tag))

print('── C/V#11 (트랙 21/22) ──')
show('입고대 신호 InReady2',        *sig(CV, 'Event', 'InReady2', 11))
show('출고대 신호 WorkInstruction', *sig(CV, 'Event', 'WorkInstruction', 11))
show('화물감지 P22(slot0)',         *sig(CV, 'Event', 'PalletExist', 11, 0))
show('화물감지 P21(slot1)',         *sig(CV, 'Event', 'PalletExist', 11, 1))
show('방향 IoDirection',            *sig(CV, 'Direction', 'IoDirection', 11))
show('트래킹 JobNo(slot0)',         *sig(CV, 'Tracking', 'JobNo', 11))
print()
print('── C/V#10 (트랙 19/20) ──')
show('입고대 신호 InReady2',        *sig(CV, 'Event', 'InReady2', 10))
show('화물감지 P19(slot0)',         *sig(CV, 'Event', 'PalletExist', 10, 0))
show('화물감지 P20(slot1)',         *sig(CV, 'Event', 'PalletExist', 10, 1))
show('트래킹 JobNo(slot0)',         *sig(CV, 'Tracking', 'JobNo', 10))
print()
print('── S/C#5 ──')
show('반송지시 TransferRequest',    *sig(SC, 'Ack',   'TransferRequest', 5))
show('지시 From',                   *sig(SC, 'Command','From', 5))
show('지시 To',                     *sig(SC, 'Command','To', 5))
show('지시 작업번호 JobNo',         *sig(SC, 'Command','JobNo', 5))
show('적재완료 LoadComplete',       *sig(SC, 'Event', 'LoadComplete', 5))
show('하역완료 UnloadComplete',     *sig(SC, 'Event', 'UnloadComplete', 5))
show('하역완료ACK',                 *sig(SC, 'Ack',   'UnloadCompleteAck', 5))
show('반송응답 TransferAck',        *sig(SC, 'Event', 'TransferAck', 5))
show('상태 SubsystemStatus',        *sig(SC, 'Status','SubsystemStatus', 5))
print()
print('── RGV#1 ──')
show('반송지시 TransferRequest',    *sig(RGV, 'Ack',   'TransferRequest', 1))
show('지시 From',                   *sig(RGV, 'Command','From', 1))
show('지시 To1',                    *sig(RGV, 'Command','To1', 1))
show('지시 작업번호 JobNo',         *sig(RGV, 'Command','JobNo', 1))
show('적재완료 LoadComplete',       *sig(RGV, 'Event', 'LoadComplete', 1))
show('하역완료 UnloadComplete',     *sig(RGV, 'Event', 'UnloadComplete', 1))
show('상태 SubsystemStatus',        *sig(RGV, 'Status','SubsystemStatus', 1))
