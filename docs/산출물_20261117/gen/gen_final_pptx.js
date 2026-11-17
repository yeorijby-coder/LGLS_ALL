// -*- coding: utf-8 -*-
// HOST_TASK 조치 결과 요약 (PPTX). 상세는 HOST_TASK_조치결과_종합.docx
//   실행 : NODE_PATH="$(npm root -g)" node gen_final_pptx.js
const pptxgen = require('pptxgenjs');
const path = require('path');

const BASE = path.dirname(__dirname);
const OUT = path.join(BASE, 'HOST_TASK_조치결과_요약.pptx');

const FONT = '맑은 고딕';
const NAVY = '1B2A49', INK = '2B2B2B', GRAY = '666666', LIGHT = 'F3F5F9', LINE = 'C9D0DC';
const RED = 'C0392B', AMBER = 'D9822B', GREEN = '2E7D5B', BLUE = '2F5DA8', REDBG = 'FBE9E7', GRNBG = 'E5F2EC', AMBBG = 'FCEFDC', BLUBG = 'E8EEF8';

const pres = new pptxgen();
pres.layout = 'LAYOUT_WIDE';            // 13.33 x 7.5
pres.title = 'HOST_TASK 조치 결과 요약';
pres.author = 'LGLS ECS Renewal';

function base(title, sub) {
  const s = pres.addSlide();
  s.background = { color: 'FFFFFF' };
  s.addText(title, { x: 0.6, y: 0.35, w: 12.1, h: 0.7, fontFace: FONT, fontSize: 28, bold: true, color: NAVY, margin: 0, isTextBox: true });
  if (sub) s.addText(sub, { x: 0.6, y: 1.02, w: 12.1, h: 0.4, fontFace: FONT, fontSize: 14, color: GRAY, margin: 0, isTextBox: true });
  s.addText('LG화학 1동 자동창고 ECS  |  HOST_TASK 조치 결과 요약', { x: 0.6, y: 7.05, w: 9, h: 0.3, fontFace: FONT, fontSize: 10, color: GRAY, margin: 0, isTextBox: true });
  return s;
}
function card(s, x, y, w, h, fill, head, headColor, body, bodySize) {
  s.addShape(pres.ShapeType.rect, { x, y, w, h, fill: { color: fill }, line: { color: fill, width: 0 } });
  s.addText(head, { x: x + 0.2, y: y + 0.12, w: w - 0.4, h: 0.5, fontFace: FONT, fontSize: 17, bold: true, color: headColor, margin: 0, isTextBox: true });
  s.addText(body, { x: x + 0.2, y: y + 0.65, w: w - 0.4, h: h - 0.75, fontFace: FONT, fontSize: bodySize || 14, color: INK, margin: 0, valign: 'top', paraSpaceAfter: 6, isTextBox: true });
}
const B = (t) => ({ text: t, options: { bullet: true, breakLine: true } });
const BL = (t) => ({ text: t, options: { bullet: true } });

// 열 3개 표(행 막대): [[a, b, c], ...]  열 너비 cw=[w1,w2,w3]
function rows3(s, head, data, y0, rh, cw, sizes) {
  const x0 = 0.6, W = 12.1;
  s.addShape(pres.ShapeType.rect, { x: x0, y: y0, w: W, h: 0.42, fill: { color: NAVY }, line: { color: NAVY, width: 0 } });
  let x = x0 + 0.15;
  head.forEach((h, i) => {
    s.addText(h, { x, y: y0, w: cw[i] - 0.15, h: 0.42, fontFace: FONT, fontSize: 13, bold: true, color: 'FFFFFF', valign: 'middle', margin: 0, isTextBox: true });
    x += cw[i];
  });
  data.forEach((r, k) => {
    const y = y0 + 0.42 + k * rh;
    s.addShape(pres.ShapeType.rect, { x: x0, y, w: W, h: rh, fill: { color: k % 2 === 0 ? LIGHT : 'FFFFFF' }, line: { color: LINE, width: 0.5 } });
    let xx = x0 + 0.15;
    r.forEach((t, i) => {
      s.addText(t, { x: xx, y, w: cw[i] - 0.2, h: rh, fontFace: FONT, fontSize: (sizes && sizes[i]) || 12, bold: i === 0, color: i === 0 ? NAVY : INK, valign: 'middle', margin: 0, isTextBox: true });
      xx += cw[i];
    });
  });
}

// ── 1 표지
{
  const s = pres.addSlide();
  s.background = { color: NAVY };
  s.addText('LG화학 1동 자동창고 ECS', { x: 0.8, y: 2.0, w: 11.7, h: 0.5, fontFace: FONT, fontSize: 18, color: 'B8C4DD', margin: 0, isTextBox: true });
  s.addText('HOST_TASK 조치 결과 요약', { x: 0.8, y: 2.55, w: 11.7, h: 1.0, fontFace: FONT, fontSize: 44, bold: true, color: 'FFFFFF', margin: 0, isTextBox: true });
  s.addText('문제 상황 · 수정 내용 · 대응할 수 있는 상황과 없는 상황', { x: 0.8, y: 3.65, w: 11.7, h: 0.6, fontFace: FONT, fontSize: 22, color: 'FFFFFF', margin: 0, isTextBox: true });
  s.addText('ECS Renewal  |  2026.11.17', { x: 0.8, y: 6.4, w: 8, h: 0.4, fontFace: FONT, fontSize: 14, color: 'B8C4DD', margin: 0, isTextBox: true });
}

// ── 2 한눈에
{
  const s = base('한눈에 보기', '원인은 우리 쪽과 WMS 쪽에 나뉘어 있고, 이번 판은 우리 쪽을 조치했습니다.');
  const w = 3.85, y = 1.7, h = 3.7;
  card(s, 0.6, y, w, h, REDBG, '문제', RED, [B('패치 때마다 WMS 작업 지시를 한동안 못 받음 (11/13 약 3시간 33분)'), B('껐다 켜도 수신 포트(9911)가 안 열리거나 접속이 남음'), BL('11/10 에는 WMS 송신 프로그램이 죽어 있었음')], 14);
  card(s, 0.6 + w + 0.28, y, w, h, GRNBG, '조치', GREEN, [B('수신 · 송신 비동기, 종료 단축'), B('소켓이 자식 프로세스로 넘어가지 않게 함'), B('[포트 정리] 버튼 · 포트정리.bat 으로 현장에서 원인 확인과 정리'), BL('EXE 하나에 포함, 설정은 모두 기본값으로 동작')], 14);
  card(s, 0.6 + (w + 0.28) * 2, y, w, h, AMBBG, '한계', AMBER, [B('WMS 쪽 문제는 우리 프로그램으로 못 고침'), B('LISTENING 은 직접 끊을 수 없음, 일부는 재부팅'), BL('시험은 모두 개발 PC, 실제 WMS 통합 시험은 아직')], 14);
  s.addShape(pres.ShapeType.rect, { x: 0.6, y: 5.7, w: 12.1, h: 0.95, fill: { color: LIGHT }, line: { color: LIGHT, width: 0 } });
  s.addText([{ text: '적용  ', options: { bold: true, color: NAVY } }, { text: 'TASK_LFC10_G1_ECSCOM.exe · HandleHunter.exe · 포트정리.bat 을 HOST_TASK 폴더에 함께 둡니다' }],
    { x: 0.85, y: 5.75, w: 11.6, h: 0.85, fontFace: FONT, fontSize: 16, color: INK, margin: 0, valign: 'middle', isTextBox: true });
}

// ── 3 발생한 문제
{
  const s = base('발생한 문제', '로그와 현장 확인으로 확인된 사실입니다');
  rows3(s, ['일시', '증상', '확인된 사실'], [
    ['11/10 15:30경', 'WMS 가 지시한 작업 4건이 "송신 대기", HOST_TASK 가 못 받음', 'WMS 서버에서 EcsCommA.exe 가 죽어 있었음. 16:55 접속하자 4건이 한꺼번에 들어옴. 우리 로그에는 이상 징후 없음'],
    ['11/13 12:18', '패치 종료 뒤 작업 접수가 3시간 33분 중단', 'WMS 는 99분 뒤(13:58)에야 접속 시도, 신판의 IP 제한이 거절(되돌림). 14:10~14:58 포트 열기 실패(10048) 96회'],
    ['11/13~', '문제 때 netstat 확인', '9911 에 CLOSE_WAIT 3개 + WMS 연결 1개(끄니 CLOSE_WAIT). PID 는 있는데 작업 관리자에 프로세스 없음'],
    ['11/10~13', '접속이 2개로 보인 66시간', '그 기간에도 작업 145건 정상 수신 → 미수신의 원인이 아님'],
  ], 1.65, 1.05, [2.0, 3.6, 6.5], [13, 12.5, 12.5]);
  s.addText('11/10 저녁에는 우리가 WMS 로 보내는 보고가 약 1시간(16:57~18:00) 나가지 않았고, 같은 시간 수신은 정상이었습니다.', { x: 0.6, y: 6.35, w: 12.1, h: 0.45, fontFace: FONT, fontSize: 12, color: GRAY, margin: 0, isTextBox: true });
}

// ── 4 원인 분석
{
  const s = base('원인 분석 결과', '시험으로 확인한 것과 아직 모르는 것을 구분했습니다');
  const w = 3.85, y = 1.7, h = 4.95;
  card(s, 0.6, y, w, h, GRNBG, '확인된 것 (시험)', GREEN, [
    B('[INI 열기] 메모장이 소켓을 물려받아, 종료 뒤에도 포트가 죽은 PID 로 남음 (재현)'),
    B('CLOSE_WAIT 만으로는 새 리스너 열기가 막히지 않음'),
    B('바인드만 한 소켓은 netstat 에 안 보이면서 10048 을 냄'),
    B('송신은 쉬는 중 끊김을 몰라 CLOSE_WAIT 로 남음 (40초 이상)'),
    BL('동시 접속 시 DB 로그인 경합')], 12.5);
  card(s, 0.6 + w + 0.28, y, w, h, AMBBG, '추정', AMBER, [
    B('현장 포트 점유는 메모장 같은 자식 프로세스가 소켓을 쥔 것일 가능성'),
    B('종료 절차가 스레드를 제한 없이 기다려 프로세스가 남을 수 있는 구조'),
    BL('두 가지 모두 현장에서 확정하지는 못함')], 14);
  card(s, 0.6 + (w + 0.28) * 2, y, w, h, REDBG, '모르는 것', RED, [
    B('11/13 14:10~14:58 의 10048 정확한 원인 (netstat 시점 불확실)'),
    B('EcsCommA 가 죽은 이유'),
    BL('WMS 가 종료 뒤 79분 · 98분 만에 붙고 어떤 때는 4초 만에 붙는 이유')], 14);
}

// ── 5 수정 내용
{
  const s = base('수정한 내용', '모두 하나의 EXE 에 들어 있고, 새 설정 키는 없어도 기본값으로 동작합니다');
  const L = [
    ['수신 비동기', '종료 때 스레드 대기 제거 · 종료 0.1~1.9초 (SRV_ASYNC)'],
    ['송신 비동기', 'WMS 끊김 2.2초 만에 감지·재접속 (CLI_ASYNC)'],
    ['종료 개선', '접속 시도 3초 · 종료 대기 10초 한도'],
    ['소켓 상속 방지', '메모장이 소켓을 쥐고 남는 것 예방, [INI 열기]는 셸 실행'],
    ['ReuseAddress 설정화', '기본 끔 (LISTEN_REUSE_ADDR)'],
  ];
  const R = [
    ['DB 로그인 · 로그 · 전문', 'DB 로그인 동시 호출 잠금, 로그에 접속 주소 <IP:포트>, 큰 전문 NAK'],
    ['[포트 정리] 버튼', 'netstat 창 · TASKKILL · 자기 종료 · 핸들 닫기 · 접속 정리 (PORT_CLEAN_BTN, 기본 숨김)'],
    ['포트정리.bat', '버튼과 같은 순서의 명령 파일'],
    ['HandleHunter.exe', '죽은 PID 가 남긴 소켓 핸들 탐색·닫기, 접속 정리'],
    ['문서 · 시험', '사용설명서(메시지 박스 캡처), 조치 결과 종합, 시험 스크립트'],
  ];
  const y0 = 1.65, rh = 0.98;
  const draw = (arr, x, n0) => arr.forEach(([a, b], i) => {
    const y = y0 + i * rh;
    s.addShape(pres.ShapeType.rect, { x, y, w: 5.95, h: rh - 0.1, fill: { color: i % 2 === 0 ? LIGHT : 'FFFFFF' }, line: { color: LINE, width: 0.5 } });
    s.addShape(pres.ShapeType.ellipse, { x: x + 0.12, y: y + 0.22, w: 0.42, h: 0.42, fill: { color: NAVY }, line: { color: NAVY, width: 0 } });
    s.addText(String(n0 + i), { x: x + 0.12, y: y + 0.22, w: 0.42, h: 0.42, fontFace: FONT, fontSize: 13, bold: true, color: 'FFFFFF', align: 'center', valign: 'middle', margin: 0, isTextBox: true });
    s.addText(a, { x: x + 0.65, y: y + 0.05, w: 5.2, h: 0.36, fontFace: FONT, fontSize: 14, bold: true, color: NAVY, margin: 0, isTextBox: true });
    s.addText(b, { x: x + 0.65, y: y + 0.4, w: 5.2, h: rh - 0.55, fontFace: FONT, fontSize: 11.5, color: INK, margin: 0, valign: 'top', isTextBox: true });
  });
  draw(L, 0.6, 1);
  draw(R, 6.75, 6);
}

// ── 6 조치할 수 있는 상황
{
  const s = base('현재 버전이 조치할 수 있는 상황', '개발 PC 시험으로 확인한 범위입니다');
  rows3(s, ['상황', '조치', '근거'], [
    ['패치 때 종료가 늦어 프로세스와 포트가 남음', '종료 절차를 짧게 끝냄', '시험 종료 0.1~1.9초 (현장 효과는 확인 필요)'],
    ['WMS 가 연결을 닫음', '수신·송신 모두 즉시 정리, 송신은 3초 주기 재접속', '송신 2.2초 (종전 40초 이상)'],
    ['메모장 등 자식 프로세스가 소켓을 쥠', '예방(상속 방지), 이미 생겼으면 핸들 닫기', '재현 후 해소 확인'],
    ['다른 프로그램 · 이중 기동이 포트를 쥠', '[포트 정리]에서 TASKKILL', '시험 통과'],
    ['남은 CLOSE_WAIT', '[포트 정리] 접속 정리 (관리자)', '관리자 권한 성공은 미확인'],
    ['동시 · 겹친 · 쪼개진 · 잘못된 전문', '접속별 처리, 전문 조립, NAK', '동시 6접속, 약 2천 건 오류 없음'],
    ['문제 때 원인 자료가 없음', 'netstat 창, PID 별 프로세스 유무, 로그의 접속 주소', '-'],
  ], 1.65, 0.7, [3.9, 5.0, 3.2], [12, 12, 11.5]);
}

// ── 7 대응하지 못하는 상황
{
  const s = base('대응하지 못하는 상황', '우리 프로그램만으로는 해결할 수 없거나 아직 확인하지 못한 것입니다');
  rows3(s, ['상황', '이유', '필요한 조치'], [
    ['WMS 송신 프로그램(EcsCommA)이 죽거나 안 붙음', 'WMS 쪽 프로그램 문제', 'WMS 담당: 이벤트 뷰어 · 자체 로그 확인'],
    ['LISTENING 소켓', '직접 끊을 수 없음', '쥔 프로세스 종료 · 핸들 닫기, 안 되면 재부팅'],
    ['쥔 프로세스를 못 찾는 경우', '핸들 닫기는 죽은 PID 의 자손만 탐색', 'netstat 캡처와 PID 를 남기고 수동 확인'],
    ['netstat 에 안 보이는 소켓이 막는 경우', '소유 PID 를 모르면 대상을 정할 수 없음', '10048 때 netstat · 로그를 같이 남김'],
    ['커널 · 드라이버 · 보안 프로그램이 쥔 경우', '프로세스 도구로 닿지 않음', '서버 재부팅'],
    ['관리자 권한에서 접속 정리 성공 여부', '관리자 아닌 PC 에서만 시험 (코드 317)', '현장 첫 사용 때 결과 확인'],
    ['실제 WMS 와의 통합 동작', '개발 PC 의 가짜 상태로만 시험', '적용 후 로그 관찰'],
    ['11/11 판 기능 · 허용 IP 제한', '이 빌드에 없음 / 앱 안 IP 제한 없음', '필요 시 반영 검토, IP 는 방화벽에서'],
  ], 1.5, 0.64, [4.4, 3.9, 3.8], [11.5, 11.5, 11.5]);
}

// ── 8 적용 · 확인
{
  const s = base('적용과 현장 확인 항목', '현장 설정 값은 바꾸지 않고, 새 키는 필요할 때만 추가합니다');
  card(s, 0.6, 1.7, 6.0, 4.95, LIGHT, '파일과 설정 키', NAVY, [
    B('같은 폴더에: TASK_LFC10_G1_ECSCOM.exe, HandleHunter.exe, 포트정리.bat'),
    B('SRV_ASYNC=1 / CLI_ASYNC=1 : 0 이면 종전 방식으로 복귀'),
    B('LISTEN_REUSE_ADDR=0 : 기본 끔'),
    B('PORT_CLEAN_BTN=0 : 1 로 바꾸면 1초 안에 버튼 표시'),
    BL('새 EXE 교체 전 HOST_TASK 종료, 직전 EXE 는 보관')], 14);
  card(s, 6.8, 1.7, 5.9, 2.85, GRNBG, '적용 직후 확인', GREEN, [
    B('서버 로그의 접속 주소 <IP:포트> 와 접속 로그'),
    B('종료에 걸리는 시간'),
    BL('종료 뒤 WMS 가 다시 접속하는 시각')], 14);
  card(s, 6.8, 4.75, 5.9, 1.9, AMBBG, '문제가 다시 나면 남길 자료', AMBER, [
    B('netstat -ano 전체(LISTENING 유무, PID, 프로세스 유무)'),
    BL('그 시각 서버 로그 마지막 줄, 열려 있던 프로그램')], 13);
}

// ── 9 권장
{
  const s = base('앞으로의 권장', '담당별로 정리했습니다');
  rows3(s, ['권장', '담당', ''], [
    ['11/10 EcsCommA 가 죽은 시각과 예외, 종료 뒤 79분 · 98분 만에 다시 붙는 이유 확인', 'WMS 담당', ''],
    ['서버 방화벽으로 FA망 대역만 수신 포트 허용', '현장 IT 담당', ''],
    ['관리자 권한에서 [포트 정리]의 접속 정리가 성공하는지 시험', '현장 / 개발', ''],
    ['현장 적용 후 종료 시간 · 재접속 시각 · 로그를 관찰하고 공유', '현장', ''],
    ['필요하면 11/11 판의 오래된 접속 자동 정리 · keepalive · 크래시 보고 반영', '개발', ''],
    ['설정 사전 재적재 위험 수정 (설정이 바뀔 때만 다시 읽기)', '개발', ''],
  ], 1.65, 0.78, [8.6, 2.6, 0.9], [14, 13, 10]);
}

pres.writeFile({ fileName: OUT }).then(() => console.log('saved', OUT));
