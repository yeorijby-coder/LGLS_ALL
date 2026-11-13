// -*- coding: utf-8 -*-
// 2026-11-13 패치 안정화 보고 요약 (PPTX). 상세는 패치_안정화_보고서_20261113.docx
//   실행 : NODE_PATH="$(npm root -g)" node gen_patch_pptx.js
const pptxgen = require('pptxgenjs');
const path = require('path');

const BASE = path.dirname(__dirname);
const OUT = path.join(BASE, '패치_안정화_보고_요약_20261113.pptx');

const FONT = '맑은 고딕';
const NAVY = '1B2A49', INK = '2B2B2B', GRAY = '666666', LIGHT = 'F3F5F9', LINE = 'C9D0DC';
const RED = 'C0392B', AMBER = 'D9822B', GREEN = '2E7D5B', BLUE = '2F5DA8', REDBG = 'FBE9E7', GRNBG = 'E5F2EC', AMBBG = 'FCEFDC';

const pres = new pptxgen();
pres.layout = 'LAYOUT_WIDE';            // 13.33 x 7.5
pres.title = '패치 안정화 보고 요약';
pres.author = 'LGLS ECS Renewal';

function base(title, sub) {
  const s = pres.addSlide();
  s.background = { color: 'FFFFFF' };
  s.addText(title, { x: 0.6, y: 0.35, w: 12.1, h: 0.7, fontFace: FONT, fontSize: 28, bold: true, color: NAVY, margin: 0, isTextBox: true });
  if (sub) s.addText(sub, { x: 0.6, y: 1.02, w: 12.1, h: 0.4, fontFace: FONT, fontSize: 14, color: GRAY, margin: 0, isTextBox: true });
  s.addText('LG화학 1동 자동창고 ECS  |  패치 안정화 보고 2026.11.13', { x: 0.6, y: 7.05, w: 9, h: 0.3, fontFace: FONT, fontSize: 10, color: GRAY, margin: 0, isTextBox: true });
  return s;
}
function card(s, x, y, w, h, fill, head, headColor, body, bodySize) {
  s.addShape(pres.ShapeType.rect, { x, y, w, h, fill: { color: fill }, line: { color: fill, width: 0 } });
  s.addText(head, { x: x + 0.2, y: y + 0.12, w: w - 0.4, h: 0.5, fontFace: FONT, fontSize: 17, bold: true, color: headColor, margin: 0, isTextBox: true });
  s.addText(body, { x: x + 0.2, y: y + 0.65, w: w - 0.4, h: h - 0.75, fontFace: FONT, fontSize: bodySize || 14, color: INK, margin: 0, valign: 'top', paraSpaceAfter: 6, isTextBox: true });
}
const B = (t) => ({ text: t, options: { bullet: true, breakLine: true } });
const BL = (t) => ({ text: t, options: { bullet: true } });

// ── 1 표지
{
  const s = pres.addSlide();
  s.background = { color: NAVY };
  s.addText('LG화학 1동 자동창고 ECS', { x: 0.8, y: 2.0, w: 11.7, h: 0.5, fontFace: FONT, fontSize: 18, color: 'B8C4DD', margin: 0, isTextBox: true });
  s.addText('패치 안정화 보고', { x: 0.8, y: 2.55, w: 11.7, h: 1.0, fontFace: FONT, fontSize: 44, bold: true, color: 'FFFFFF', margin: 0, isTextBox: true });
  s.addText('11/13 접수 중단의 원인과 앞으로의 진행 방식', { x: 0.8, y: 3.65, w: 11.7, h: 0.6, fontFace: FONT, fontSize: 22, color: 'FFFFFF', margin: 0, isTextBox: true });
  s.addText('ECS Renewal  |  2026.11.13', { x: 0.8, y: 6.4, w: 8, h: 0.4, fontFace: FONT, fontSize: 14, color: 'B8C4DD', margin: 0, isTextBox: true });
}

// ── 2 한눈에
{
  const s = base('한눈에 보기', '접수가 3시간 33분 멈췄고, 작업 유실은 없었습니다. 원인은 세 가지가 겹친 것입니다.');
  const w = 3.85, y = 1.7, h = 3.5;
  card(s, 0.6, y, w, h, REDBG, '① 종료가 늦다', RED, [B('끄는 순간에도 상위(WMS) 접속을 시도하고 있음'), B('접속에 시간 제한이 없고, 종료가 그것을 무제한 기다림'), BL('창만 닫히고 프로세스가 남음')], 14);
  card(s, 0.6 + w + 0.28, y, w, h, AMBBG, '② 끈 뒤에도 포트가 막힌다', AMBER, [B('이전 프로세스의 연결이 남아 9911 포트를 열 수 없음'), B('WMS 는 끊긴 줄 몰라 재접속하지 않음'), BL('서버 환경 영향이 의심됨(미확정)')], 14);
  card(s, 0.6 + (w + 0.28) * 2, y, w, h, 'E8EEF8', '③ 설정이 현장과 안 맞았다', BLUE, [B('신판의 IP 제한이 현장 구성과 맞지 않음'), B('랜카드 2개, WMS 는 두 주소에서 접속, 이중화 가능성'), BL('제한을 제거한 판으로 이미 복구')], 14);
  s.addShape(pres.ShapeType.rect, { x: 0.6, y: 5.5, w: 12.1, h: 1.15, fill: { color: LIGHT }, line: { color: LIGHT, width: 0 } });
  s.addText([{ text: '앞으로  ', options: { bold: true, color: NAVY } }, { text: '같은 절차와 확인표로 패치 · 한 번에 하나만 변경 · 직전 버전으로 즉시 복귀 · 종료 개선판을 시험한 뒤 적용해 WMS 재시작을 없앰' }],
    { x: 0.85, y: 5.55, w: 11.6, h: 1.05, fontFace: FONT, fontSize: 16, color: INK, margin: 0, valign: 'middle', isTextBox: true });
}

// ── 3 경과
{
  const s = base('11/13 경과', '접수가 없던 구간은 12:18 ~ 15:51 (작업번호 2471 → 2472, 번호 연속)');
  const x0 = 0.9, x1 = 12.4, t0 = 11 * 60 + 30, t1 = 16 * 60 + 10;
  const X = (hh, mm) => x0 + (x1 - x0) * ((hh * 60 + mm - t0) / (t1 - t0));
  const yBar = 3.6, hBar = 0.5;
  s.addShape(pres.ShapeType.rect, { x: x0, y: yBar, w: X(12, 18) - x0, h: hBar, fill: { color: GREEN }, line: { color: GREEN, width: 0 } });
  s.addShape(pres.ShapeType.rect, { x: X(12, 18), y: yBar, w: X(15, 51) - X(12, 18), h: hBar, fill: { color: RED }, line: { color: RED, width: 0 } });
  s.addShape(pres.ShapeType.rect, { x: X(15, 51), y: yBar, w: x1 - X(15, 51), h: hBar, fill: { color: GREEN }, line: { color: GREEN, width: 0 } });
  s.addText('접수 중단  3시간 33분', { x: X(12, 18), y: yBar, w: X(15, 51) - X(12, 18), h: hBar, fontFace: FONT, fontSize: 16, bold: true, color: 'FFFFFF', align: 'center', valign: 'middle', margin: 0, isTextBox: true });
  const ev = [
    [12, 18, '12:18', '프로그램 종료\n(상위 접속 시도 중 10004)', 'up', RED],
    [13, 58, '13:58', 'IP 제한으로\nWMS 접속 거절 18회', 'down', AMBER],
    [14, 10, '14:10', '포트 열기 실패\n(10048) 96회', 'up', AMBER],
    [14, 37, '14:37', '제한 없는 판 교체\n(103 연속 작업)', 'down', BLUE],
    [15, 51, '15:51', '서버 재시작\n4초 만에 WMS 접속', 'up', GREEN],
  ];
  ev.forEach(([hh, mm, tm, tx, dir, col]) => {
    const cx = X(hh, mm);
    const up = dir === 'up';
    s.addShape(pres.ShapeType.line, { x: cx, y: up ? yBar - 0.55 : yBar + hBar, w: 0, h: 0.55, line: { color: col, width: 1.5 } });
    s.addText(tm, { x: cx - 1.0, y: up ? yBar - 1.55 : yBar + hBar + 0.58, w: 2.0, h: 0.3, fontFace: FONT, fontSize: 14, bold: true, color: col, align: 'center', margin: 0, isTextBox: true });
    s.addText(tx, { x: cx - 1.2, y: up ? yBar - 1.27 : yBar + hBar + 0.86, w: 2.4, h: 0.6, fontFace: FONT, fontSize: 12, color: INK, align: 'center', valign: 'top', margin: 0, isTextBox: true });
  });
  s.addText('11:27 마지막 작업 완료', { x: x0 - 0.1, y: yBar + hBar + 0.58, w: 2.4, h: 0.3, fontFace: FONT, fontSize: 12, color: GRAY, margin: 0, isTextBox: true });
  s.addText('11:27~12:18 은 접속 문제 이전이라 작업이 없던 시간으로 보임', { x: 0.9, y: 6.5, w: 11.5, h: 0.35, fontFace: FONT, fontSize: 12, color: GRAY, margin: 0, isTextBox: true });
}

// ── 4 종료 지연
{
  const s = base('원인 ① 프로그램이 빨리 꺼지지 않는다', '로그: 종료할 때마다 "리모트 시스템과 연결 실패 !(10004)" 가 기록됨');
  const steps = [
    ['종료 요청', '[종료] 버튼', LIGHT, NAVY],
    ['접속 시도 중', '상위(WMS) 접속을 기다림\n시간 제한 없음, 닿지 않으면 약 21초', REDBG, RED],
    ['무제한 대기', '종료 절차가 그 작업이\n끝나기를 끝까지 기다림', REDBG, RED],
    ['프로세스 잔존', '화면은 사라졌지만\n프로세스와 포트가 남음', AMBBG, AMBER],
  ];
  const w = 2.75, g = 0.37, y = 1.85, h = 2.1;
  steps.forEach(([hd, bd, fill, col], i) => {
    const x = 0.6 + i * (w + g);
    s.addShape(pres.ShapeType.rect, { x, y, w, h, fill: { color: fill }, line: { color: fill, width: 0 } });
    s.addText(hd, { x: x + 0.15, y: y + 0.15, w: w - 0.3, h: 0.45, fontFace: FONT, fontSize: 17, bold: true, color: col, margin: 0, isTextBox: true });
    s.addText(bd, { x: x + 0.15, y: y + 0.7, w: w - 0.3, h: h - 0.8, fontFace: FONT, fontSize: 14, color: INK, margin: 0, valign: 'top', isTextBox: true });
    if (i < steps.length - 1) s.addText('▶', { x: x + w, y: y + h / 2 - 0.2, w: g, h: 0.4, fontFace: FONT, fontSize: 16, color: GRAY, align: 'center', margin: 0, isTextBox: true });
  });
  card(s, 0.6, 4.35, 5.95, 2.4, LIGHT, '구 ECP 는 해당 없음', NAVY, [B('접속과 수신이 모두 비동기(BeginConnect / BeginReceive)'), B('종료 때 기다리는 스레드가 없음'), BL('현행 프로그램에서 달라진 부분')], 14);
  card(s, 6.75, 4.35, 5.95, 2.4, GRNBG, '종료 개선판 (작성 완료, 현장 시험 전)', GREEN, [B('상위 접속 3초 제한, 종료 요청이면 즉시 중단'), B('종료 대기 10초 한도, 넘기면 백그라운드 전환'), B('종료 때 상위 접속 소켓을 정상적으로 닫음'), BL('접속 시간 제한은 별도 시험: 3.3초 / 종료 신호 0.1초')], 14);
}

// ── 5 포트 잔재
{
  const s = base('원인 ② 끈 뒤에도 포트가 막히고 WMS 를 껐다 켜야 했다', '정상 종료라면 윈도우가 연결을 정리해 상대가 바로 알지만, 이번에는 정리되지 않음');
  card(s, 0.6, 1.7, 5.95, 3.55, AMBBG, '현장에서 확인된 상태', AMBER, [B('LISTENING 없이 ESTABLISHED / CLOSE_WAIT 만 남음'), B('해당 PID 가 작업 관리자에 없어 종료도 못 함'), B('새 프로그램은 9911 을 열지 못함 (10048, 96회)'), BL('WMS 는 12:18 부터 1시간 39분 동안 재접속하지 않음')], 16);
  card(s, 6.75, 1.7, 5.95, 3.55, LIGHT, '원인 후보 (확정 못 함)', NAVY, [B('종료되지 못한 입출력이 프로세스 정리를 막음'), B('서버의 보안 프로그램이나 네트워크 드라이버가 연결을 쥐고 있음'), B('개발 PC(Windows 11)에서는 재현되지 않음'), BL('서버 OS 또는 보안 환경 차이일 수 있어 IT 확인 필요')], 16);
  s.addShape(pres.ShapeType.rect, { x: 0.6, y: 5.55, w: 12.1, h: 1.1, fill: { color: GRNBG }, line: { color: GRNBG, width: 0 } });
  s.addText([{ text: 'WMS 재시작은 마지막 수단  ', options: { bold: true, color: GREEN } }, { text: '깨끗한 상태로 열면 WMS 가 4초 만에 접속했음(15:51). 종료 개선판이 확인되면 WMS 를 끄지 않고 패치하는 것이 목표' }],
    { x: 0.85, y: 5.6, w: 11.6, h: 1.0, fontFace: FONT, fontSize: 16, color: INK, margin: 0, valign: 'middle', isTextBox: true });
}

// ── 6 IP 권고
{
  const s = base('원인 ③ 접속 IP 제한 : 권고안', '이중화(가상 IP)가 있으면 접속 주소를 미리 정할 수 없으므로 앱 안의 IP 제한은 맞지 않음');
  const rows = [
    ['1', '앱의 IP 제한은 두지 않는다', '현장 판(14:37)은 이미 제거됨. 코드에도 되돌리지 않음'],
    ['2', '보호는 네트워크에서', '서버 방화벽에서 FA망 대역만 9911 허용 (현장 IT 담당)'],
    ['3', '접속을 여러 개 동시에 받는다', '이중화 전환 때 옛 접속과 새 접속이 겹쳐도 거절하지 않음. 중복 작업은 작업번호로 걸러짐(이미 지시된 작업 응답)'],
    ['4', '접속 주소를 로그에 남긴다', '어느 서버가 붙었는지 추적. 현재 판은 접속 때마다 IP:포트 기록'],
    ['5', '죽은 접속은 자동 정리', '이중화 전환 후 남은 옛 접속을 비활성 시간 기준으로 닫음 (11/11 판의 기능, 현장 판 반영 여부 별도 확인)'],
    ['6', '상위 보고 주소는 서비스(가상) 주소 하나로', 'RemoteIP(10.88.128.20)가 WMS 쪽 서비스 주소가 맞는지 WMS 담당 확인'],
  ];
  const y0 = 1.65, rh = 0.8;
  rows.forEach(([n, a, b], i) => {
    const y = y0 + i * rh;
    s.addShape(pres.ShapeType.rect, { x: 0.6, y, w: 12.1, h: rh - 0.08, fill: { color: i % 2 === 0 ? LIGHT : 'FFFFFF' }, line: { color: LINE, width: 0.5 } });
    s.addShape(pres.ShapeType.ellipse, { x: 0.75, y: y + 0.17, w: 0.42, h: 0.42, fill: { color: NAVY }, line: { color: NAVY, width: 0 } });
    s.addText(n, { x: 0.75, y: y + 0.17, w: 0.42, h: 0.42, fontFace: FONT, fontSize: 14, bold: true, color: 'FFFFFF', align: 'center', valign: 'middle', margin: 0, isTextBox: true });
    s.addText(a, { x: 1.35, y, w: 4.1, h: rh - 0.08, fontFace: FONT, fontSize: 15, bold: true, color: NAVY, valign: 'middle', margin: 0, isTextBox: true });
    s.addText(b, { x: 5.55, y, w: 7.0, h: rh - 0.08, fontFace: FONT, fontSize: 13, color: INK, valign: 'middle', margin: 0, isTextBox: true });
  });
}

// ── 7 11/11 아침
{
  const s = base('11/11 아침 "중단" 조사 결과', '서버 로그와 DB 에서 멈춘 흔적은 찾지 못했고, 11:00 직전에 정상 처리한 기록이 있음');
  card(s, 0.6, 1.7, 6.2, 4.95, LIGHT, '확인된 사실', NAVY, [
    B('10:52 작업 접수(4783), 10:53·10:55 완료 보고, WMS 응답 정상'),
    B('10:58 작업 접수(2407), 11:01 완료 보고'),
    B('11/11 하루 서버 로그에 종료·시작·접속 기록이 없음 → 프로그램을 다시 띄운 적 없음'),
    B('설비 통신 로그는 07:00~12:30 에 1분 이상 끊긴 구간 없음'),
    BL('작업번호 4777~4783, 2404~2407 이 연속')], 16);
  card(s, 7.0, 1.7, 5.7, 4.95, AMBBG, '남는 의문', AMBER, [
    B('09:31 ~ 10:52 약 82분 동안 상위와 주고받은 기록이 없음'),
    B('같은 날 낮에도 3시간, 2시간 17분 공백이 있어 평소 간격과 구분되지 않음'),
    B('상태 보고는 값이 바뀔 때 위주로 나가므로 공백이 곧 정지는 아님'),
    BL('통화하신 11시의 상황(어떤 화면·증상이었는지)을 알면 같은 시각의 로그를 더 좁혀 볼 수 있음')], 16);
}

// ── 8 앞으로
{
  const s = base('앞으로의 진행 방식', '매번 같은 순서로, 한 번에 하나만, 되돌릴 수 있게');
  const steps = [['1 사전', '직전 EXE 보관\nini 는 현장 값 확인'], ['2 정지', '[종료] 버튼\ntasklist · netstat 비어야 함'], ['3 교체', '새 EXE 복사\n크기·시각 확인'], ['4 기동', '"통신이 연결되었습니다"\n수신 포트 열림'], ['5 확인', '5분 안에\nWMS 접속·보고 확인'], ['6 복귀', '접속 안 되면\n직전 EXE 로 복귀']];
  const w = 1.88, g = 0.164, y = 1.75, h = 2.0;
  steps.forEach(([hd, bd], i) => {
    const x = 0.6 + i * (w + g);
    s.addShape(pres.ShapeType.rect, { x, y, w, h, fill: { color: i === 5 ? REDBG : LIGHT }, line: { color: LINE, width: 0.5 } });
    s.addText(hd, { x: x + 0.1, y: y + 0.12, w: w - 0.2, h: 0.4, fontFace: FONT, fontSize: 16, bold: true, color: i === 5 ? RED : NAVY, margin: 0, isTextBox: true });
    s.addText(bd, { x: x + 0.1, y: y + 0.6, w: w - 0.2, h: h - 0.7, fontFace: FONT, fontSize: 12, color: INK, margin: 0, valign: 'top', isTextBox: true });
  });
  card(s, 0.6, 4.05, 5.95, 2.65, GRNBG, '패치 원칙', GREEN, [B('한 번에 하나만 변경'), B('바뀌는 내용과 영향 범위를 사전 공유'), B('접속 주소 등 현장 설정은 패치로 바꾸지 않음'), BL('접수가 적은 시간대에 적용')], 14);
  card(s, 6.75, 4.05, 5.95, 2.65, LIGHT, '개선 계획', NAVY, [B('종료 개선판 현장 시험 (종료 시간·포트 정리·WMS 재접속)'), B('종료 시 소켓 상태를 로그로 남겨 포트 잔재 원인 확정'), BL('일정 시간 접수·연결이 없으면 화면 경고 (협의 후)')], 14);
}

// ── 9 요청
{
  const s = base('현장에 확인 요청드릴 사항', '원인이 코드만으로 끝나지 않을 수 있어 서버 환경과 WMS 구성 확인이 필요합니다');
  card(s, 0.6, 1.7, 6.2, 4.95, LIGHT, '확인 요청', NAVY, [
    B('서버 OS 버전, 설치된 보안 프로그램, 방화벽 종류'),
    B('WMS 접속 주소(192.100.1.147 / .148)와 이중화·가상 IP 구성'),
    B('상위 보고 주소(RemoteIP 10.88.128.20)가 WMS 서비스 주소가 맞는지'),
    B('패치 가능한 시간대와 허용되는 정지 시간'),
    BL('포트가 다시 막히면 netstat 결과와 해당 시각 로그')], 16);
  card(s, 7.0, 1.7, 5.7, 4.95, AMBBG, '아직 확인되지 않은 것', AMBER, [
    B('포트에 연결이 남은 정확한 원인'),
    B('11/11 아침 "중단"의 실체 (흔적 없음)'),
    B('종료 개선판의 현장 효과 (개발 PC 에서는 접속 시간 제한만 시험)'),
    BL('WMS 쪽 재접속 동작 (재시작 후 33초 간격으로 접속한 것만 관측)')], 16);
}

pres.writeFile({ fileName: OUT }).then(() => console.log('saved', OUT));
