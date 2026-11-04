// -*- coding: utf-8 -*-
// 2026-11-04  ECS Troubleshooting 자료 (PPTX)   /  2026-11-05 검토 반영판
//   형식 : 타사 현장 자료(TO40_Troubleshooting_..._WCS_20250113.pptx)와 같은 구성
//          제목 / 왼쪽 = 상황명 + 화면 캡처(강조 박스) / 오른쪽 = 1.현상 · 2.원인 · 3.조치
//   규칙 (2026-11-05 사용자 검토)
//     - 왼쪽 강조 박스 색 = 오른쪽 본문에서 그 요소를 말하는 문장의 글자색 (빨강=현상이 보이는 곳 / 주황=함께 볼 곳 / 파랑=누르는 곳 / 초록=처리 뒤 바뀌는 곳)
//     - 강조 박스는 특정 칸·단추만 좁게 두른다
//     - 모든 캡처는 2026-11-05 최신 적용본(적용_20261015 레이아웃·설정 + 11-10 EcsMain) 운전 화면에서 그 상황을 실제로 만들어 찍은 것(shots/*_1105.png)
//     - 제목은 "3. Troubleshooting - Conveyor (1)" 처럼 장 안의 번호를 붙인다 (본문의 "3-1 참조" 와 맞춤)
//   실행 : NODE_PATH="$(npm root -g)" node gen_troubleshooting_pptx.js
//   출력 : docs/산출물_20261110/05_ECS_Troubleshooting_1105.pptx  (05_ECS_Troubleshooting.pptx 는 사용자 수정본이라 건드리지 않는다)
const pptxgen = require('pptxgenjs');
const fs = require('fs');
const path = require('path');

const HERE = __dirname;
const BASE = path.dirname(HERE);                       // docs/산출물_20261110
const SHOTS = path.join(BASE, 'shots');
const OUT = path.join(BASE, '05_ECS_Troubleshooting_1105.pptx');

const FONT = '맑은 고딕';
const BLUE = '0070C0', NAVY = '1B2A49', GREEN_BG = 'E2EFDA', GRAY = '666666', RED = 'E00000', ORANGE = 'ED7D31', PURPLE = '7030A0', GREEN = '00A050', LINE = '2E75B6', TITLE = '1F5FA8';
const DATE = '2026.11';
const SITE = 'LG화학 1동 자동창고 ECS';

const pres = new pptxgen();
pres.layout = 'LAYOUT_WIDE';          // 13.33 x 7.5
pres.title = 'ECS Troubleshooting';
pres.subject = SITE;
pres.lang = 'ko-KR';

// ───────── 유틸 ─────────
function pngSize(p) {                  // PNG IHDR
  const b = fs.readFileSync(p);
  return { w: b.readUInt32BE(16), h: b.readUInt32BE(20) };
}
function shot(name) { const p = path.join(SHOTS, name); if (!fs.existsSync(p)) throw new Error('없는 캡처: ' + name); return p; }
function has(name) { return fs.existsSync(path.join(SHOTS, name)); }

// 그림을 (x,y,maxW,maxH) 안에 비율 유지로 넣고, 픽셀 좌표 강조 박스를 얹는다. 실제 놓인 크기를 돌려준다.
function img(slide, name, x, y, maxW, maxH, boxes, opts) {
  const p = shot(name); const s = pngSize(p);
  if (y < 1.45) { maxH -= (1.64 - y); y = 1.64; }      // 소제목·캡션(2줄) 아래부터
  let w = maxW, h = maxW * s.h / s.w;
  if (h > maxH) { h = maxH; w = maxH * s.w / s.h; }
  if (opts && opts.center) x = x + (maxW - w) / 2;
  slide.addImage({ path: p, x, y, w, h, objectName: name });
  (boxes || []).forEach(b => {
    const [px, py, pw, ph, col] = b;
    slide.addShape(pres.ShapeType.rect, { x: x + px / s.w * w, y: y + py / s.h * h, w: pw / s.w * w, h: ph / s.h * h,
      fill: { type: 'none' }, line: { color: col || RED, width: 2.25 }, objectName: 'hl' });
  });
  return { x, y, w, h };
}
function caption(slide, text, x, y, w, color) {
  let h = 0.24;
  if (y < 1.3) { y = 1.2; h = 0.42; }
  slide.addText(text, { x, y, w, h, fontFace: FONT, fontSize: 9.5, color: color || TITLE, isTextBox: true, margin: 0, bold: true, valign: 'middle' });
}
// 설명 상자(연한 바탕)
function note(slide, x, y, w, h, title, lines, size) {
  const runs = [{ text: title, options: { bold: true, color: TITLE, breakLine: true, paraSpaceAfter: 4 } }];
  lines.forEach((t, i) => runs.push({ text: t, options: { breakLine: i < lines.length - 1, paraSpaceAfter: 3 } }));
  slide.addText(runs, { x, y, w, h, fontFace: FONT, fontSize: size || 9.5, color: '333333', isTextBox: true, margin: 3, fill: { color: 'F3F6FB' }, valign: 'top' });
}
function header(slide, title) {
  slide.addText(title, { x: 0.53, y: 0.12, w: 12.3, h: 0.51, fontFace: FONT, fontSize: 24, bold: true, color: TITLE, isTextBox: true, margin: 0, valign: 'middle' });
  slide.addShape(pres.ShapeType.line, { x: 0.53, y: 0.78, w: 12.3, h: 0, line: { color: LINE, width: 1.5 } });
}
function footer(slide, n) {
  slide.addText(SITE + '  ·  Troubleshooting', { x: 0.53, y: 7.12, w: 8, h: 0.25, fontFace: FONT, fontSize: 9, color: GRAY, isTextBox: true, margin: 0 });
  slide.addText(String(n), { x: 12.2, y: 7.12, w: 0.6, h: 0.25, fontFace: FONT, fontSize: 9, color: GRAY, isTextBox: true, margin: 0, align: 'right' });
}
// 오른쪽 3단(현상/원인/조치). 각 항목은 문자열(줄) 또는 {t, color, bold, indent, bullet, num} 객체
function rhs(slide, 현상, 원인, 조치, opt) {
  const X = 7.05, W = 5.95; const fs = (opt && opt.size) || 11;
  const blocks = [['1. 현상', 0.95, 현상, 1.5], ['2. 원인', 2.85, 원인, 1.5], ['3. 조치', 4.75, 조치, 2.25]];
  blocks.forEach(([h, y, lines, bh]) => {
    slide.addText(h, { x: X, y, w: W, h: 0.32, fontFace: FONT, fontSize: 14, bold: true, color: NAVY, fill: { color: GREEN_BG }, isTextBox: true, margin: [0, 0, 0, 6], valign: 'middle' });
    const runs = [];
    lines.forEach((ln, i) => {
      const o = (typeof ln === 'string') ? { t: ln } : ln;
      const opt = { fontFace: FONT, fontSize: o.size || fs, color: o.color || '222222', bold: !!o.bold, breakLine: i < lines.length - 1, paraSpaceAfter: 2 };
      if (o.bullet) opt.bullet = o.num ? { type: 'number' } : true;
      if (o.indent) opt.indentLevel = o.indent;
      runs.push({ text: o.t, options: opt });
    });
    slide.addText(runs, { x: X, y: y + 0.36, w: W, h: bh, fontFace: FONT, isTextBox: true, margin: [2, 4, 2, 4], valign: 'top' });
  });
}
// 본문 슬라이드 공통 틀 - 제목에 장 안의 순번 "(n)" 을 붙인다
let pageNo = 0;
const chapterCnt = {};
function body(no, title, sub) {
  const s = pres.addSlide();
  pageNo++;
  chapterCnt[no] = (chapterCnt[no] || 0) + 1;
  header(s, no + '. ' + title + ' (' + chapterCnt[no] + ')');
  s.addText([{ text: '•  ', options: { color: TITLE } }, { text: sub, options: { color: TITLE } }],
    { x: 0.53, y: 0.86, w: 6.15, h: 0.34, fontFace: FONT, fontSize: 12.5, bold: true, isTextBox: true, margin: 0, valign: 'middle' });
  s.addShape(pres.ShapeType.line, { x: 6.85, y: 0.95, w: 0, h: 6.1, line: { color: LINE, width: 0.75 } });
  footer(s, pageNo);
  return s;
}
const B = (t, o) => Object.assign({ t, bullet: true }, o || {});   // 글머리 줄
const N = (t, o) => Object.assign({ t, bullet: true, num: true }, o || {});
const H = (t, color) => ({ t, bold: true, color: color || TITLE });   // 소제목 줄
const W_ = (t) => ({ t: '※ ' + t, bold: true, color: NAVY });        // 주의 줄 (박스 색과 겹치지 않게 남색)

// 자주 쓰는 박스 (픽셀 좌표) - 캡처 종류별
// C/V 상태창 (579x613)
const CV = { lugg: [128, 133, 284, 20], cmd: [430, 100, 118, 162], auto: [430, 262, 118, 20], hseject: [437, 312, 103, 22], err: [128, 435, 420, 22],
  plt: [30, 497, 140, 18], automode: [30, 523, 140, 18], inhs: [205, 523, 145, 18], ouths: [378, 523, 145, 18], rgvdep: [205, 548, 162, 18], rgvarr: [378, 548, 165, 18], stoin: [205, 497, 145, 18], stoout: [378, 497, 145, 18] };
// S/C 상태창 (540x639)
const SC = { resend: [385, 128, 122, 20], force: [385, 150, 122, 20], hmidel: [385, 172, 122, 20], susp: [385, 290, 122, 18], stopbtns: [385, 309, 122, 62],
  ucsta: [128, 290, 235, 18], lugg: [128, 311, 235, 18], job: [30, 415, 487, 130], err: [30, 580, 487, 22], pos: [128, 165, 235, 86] };
// RTV 상태창 (567x572)
const RTV = { resend: [410, 118, 122, 24], force: [410, 144, 122, 24], hmidel: [410, 170, 122, 24], suspbox: [410, 272, 122, 18], suspbtn: [410, 298, 122, 26],
  sta: [296, 283, 96, 18], pos: [296, 305, 96, 18], lugg: [108, 305, 97, 18], job: [30, 345, 512, 137], err: [30, 512, 512, 20], fork: [133, 165, 257, 44] };
// 창고 그림 크롭 (1270x730, 메인 660,230 기준) - 레이아웃 XML(EcsLayout1.xml)의 셀(73x42)을 픽셀로 바꾼다
const CELL = 1270 / 73;
const C = (l, b, r, t) => [l * CELL, 730 - t * CELL, (r - l) * CELL, (t - b) * CELL];
const LY = { t124: C(7,32,10,34), t123: C(10,32,13,34), t126: C(7,30,10,32), t125: C(10,30,13,32), t129: C(7,24,10,26), t130: C(7,22,10,24), t131: C(10,20,13,22),
  cv2: C(25,32,28,34), t103: C(22,32,25,34), cv2dir: C(19,34,25,36), cv11dir: C(25,2,31,4), cv11: C(22,4,28,6), lbl103: C(2,32,7,34), lbl104: C(0,30,7,32),
  sc1: C(25,34,28,36), sc1busy: C(25,34,28,36), sc2: C(25,28,28,30), rail1: C(28,34,73,36), rail2: C(28,28,73,30),
  rgv: C(16,26,19,28), rgvbusy: C(16,22,19,30), t105: C(22,30,25,32), t107: C(22,26,25,28), t109: C(22,20,25,22), t110: C(25,20,28,22), t113: C(22,18,25,20), t127: C(10,26,13,28) };
// 작업정보 판넬 크롭 (624x518, 메인 12,517 기준)
const PN = { row: (n) => [0, 30 + 17 * (n - 1), 624, 18], seq: (n) => [0, 377 + 17.5 * (n - 1), 420, 18], done: [530, 338, 88, 20], sel: [0, 340, 120, 18] };
// 설비 반송 크롭 (624x235, 메인 12,340 기준) - 아래 작업정보 판넬 첫 줄까지
const VH = { sc1: [2, 25, 600, 18], rgv: [2, 112, 600, 18], job1: [2, 214, 620, 18] };
// 범례 칸 (624x185, 메인 12,150 기준) - 왼쪽 범례 칸
const LG = { jobcolor: [8, 10, 610, 38], cvstate: [8, 48, 610, 56], cverr: [310, 62, 72, 20], cvstn: [8, 62, 220, 20], cvpause: [215, 62, 90, 20], scstate: [8, 108, 300, 36], scerr: [215, 120, 70, 20], scmanual: [112, 120, 70, 20] };
// 알람 경고창 (634x362)
const AL = { msg: [3, 30, 318, 42], row1: [5, 228, 628, 20], rows: [5, 228, 628, 40] };

// ───────── 표지 ─────────
{
  const s = pres.addSlide(); pageNo++;
  s.background = { color: 'FFFFFF' };
  s.addShape(pres.ShapeType.rect, { x: 0, y: 5.9, w: 13.33, h: 1.6, fill: { color: TITLE }, line: { color: TITLE } });
  s.addText('Troubleshooting', { x: 0.7, y: 0.9, w: 11, h: 1.2, fontFace: FONT, fontSize: 48, color: TITLE, isTextBox: true, margin: 0 });
  s.addText(SITE, { x: 0.7, y: 2.2, w: 11, h: 0.6, fontFace: FONT, fontSize: 24, color: NAVY, isTextBox: true, margin: 0 });
  s.addText('운전 화면(Ecs)에서 상황별로 무엇을 보고 무엇을 누르는가', { x: 0.7, y: 2.85, w: 11, h: 0.5, fontFace: FONT, fontSize: 16, color: GRAY, isTextBox: true, margin: 0 });
  s.addText([{ text: DATE, options: { breakLine: true } }, { text: 'ECS 개발 · 운영 담당' }],
    { x: 7.3, y: 6.2, w: 5.5, h: 1.0, fontFace: FONT, fontSize: 16, color: 'FFFFFF', isTextBox: true, margin: 0, align: 'right', valign: 'middle' });
  s.addText('본 문서는 현장 운영·인수인계용 자료로, 승인 없는 외부 배포·복사를 금합니다.', { x: 0.7, y: 6.95, w: 8, h: 0.4, fontFace: FONT, fontSize: 9, color: 'DDE6F5', isTextBox: true, margin: 0 });
}

// ───────── 목차 ─────────
{
  const s = pres.addSlide(); pageNo++;
  header(s, '목차');
  const items = ['1. 개정 이력', '2. 전체 레이아웃 - 운전 화면 읽는 법', '3. Troubleshooting - Conveyor (C/V)', '4. Troubleshooting - Stacker Crane (S/C)', '5. Troubleshooting - RGV (RTV)', '6. Troubleshooting - ETC (통신 · 알람 · 화면)'];
  s.addText(items.map((t, i) => ({ text: t, options: { breakLine: i < items.length - 1, paraSpaceAfter: 10 } })),
    { x: 1.2, y: 1.3, w: 7, h: 4.5, fontFace: FONT, fontSize: 20, color: NAVY, isTextBox: true, margin: 0, valign: 'top' });
  s.addText([
    { text: '이 자료의 읽는 법', options: { bold: true, color: TITLE, breakLine: true, paraSpaceAfter: 6 } },
    { text: '각 장면은 왼쪽에 운전 화면 캡처, 오른쪽에 1.현상 2.원인 3.조치 순으로 적었다. 캡처 위의 테두리 색과 오른쪽 글자색이 같은 것끼리 짝이다.', options: { breakLine: true, paraSpaceAfter: 4 } },
    { text: '빨강 = 현상이 보이는 곳', options: { color: RED, breakLine: true, bold: true } },
    { text: '주황 = 함께 확인할 곳', options: { color: ORANGE, breakLine: true, bold: true } },
    { text: '파랑 = 누르는 곳', options: { color: BLUE, breakLine: true, bold: true } },
    { text: '초록 = 처리 뒤 바뀌는 곳', options: { color: GREEN, breakLine: true, bold: true, paraSpaceAfter: 6 } },
    { text: '상세 절차는 02 ECS 사용자 매뉴얼(5.7 완료처리, 6장 이상 대처), 서버 쪽 처치는 01 운영자 매뉴얼 9장을 따른다.', options: { breakLine: true, paraSpaceAfter: 6 } },
    { text: '※ 어떤 경우에도 WCS 는 시간 경과로 작업을 자동 완료하지 않는다. 설비 신호가 없으면 사람이 실물을 확인한 뒤 넘긴다.', options: { color: NAVY, bold: true } },
  ], { x: 8.4, y: 1.3, w: 4.4, h: 4.8, fontFace: FONT, fontSize: 12, color: '333333', isTextBox: true, margin: 6, valign: 'top', fill: { color: 'F3F6FB' } });
  footer(s, pageNo);
}

// ───────── 1. 개정이력 ─────────
{
  const s = pres.addSlide(); pageNo++;
  header(s, '1. 개정이력');
  const hdr = ['버전', '개정일자', '개정사유', '개정내역', '작성자', '승인자'].map(t => ({ text: t, options: { bold: true, fill: { color: 'D9E2F3' }, align: 'center', color: NAVY } }));
  const rows = [hdr,
    ['1.0.0', '2026-11-04', '신규', '최초 작성 (2026-11-10 적용분 기준 : 작업정보 판넬 [완료처리] 경로 탐색, 상태창 [데이터 삭제], 리본 [CV 지우기] 포함)', 'ECS 개발', ''],
    ['1.0.1', '2026-11-05', '검토 반영', '캡처 전부를 최신 적용본 운전 화면에서 상황을 재현해 다시 찍음(최신 레이아웃·범례·상태창). 강조 박스를 좁히고 본문 글자색과 맞춤. 제목에 장 안의 번호. 이중입고·공출고 장 삭제(이 현장에 없음). 3-3/3-4/4-3~4-6/5-3/5-4 문구 수정', 'ECS 개발', '']];
  for (let i = 0; i < 7; i++) rows.push(['', '', '', '', '', '']);
  s.addTable(rows, { x: 0.7, y: 1.0, w: 11.9, colW: [1.1, 1.6, 1.2, 5.6, 1.4, 1.0], fontFace: FONT, fontSize: 11, border: { type: 'solid', color: '8EA9C1', pt: 0.75 }, rowH: 0.38, valign: 'middle', align: 'center', objectName: '개정이력' });
  footer(s, pageNo);
}

// ───────── 2. 전체 레이아웃 ─────────
{
  const s = pres.addSlide(); pageNo++;
  header(s, '2. 전체 레이아웃 - 운전 화면(Ecs) 읽는 법');
  img(s, 'main_run_1105.png', 0.53, 1.0, 8.6, 5.9, [
    [12, 30, 640, 110, ORANGE],       // 리본 (통신 신호등 포함)
    [12, 150, 625, 185, PURPLE],      // 범례
    [12, 340, 625, 172, GREEN],       // 설비 반송
    [12, 517, 625, 518, BLUE],        // 작업정보 판넬
    [660, 230, 1270, 730, RED],       // 창고 그림
  ]);
  const L = (t, color) => ({ text: t, options: { bullet: true, breakLine: true, paraSpaceAfter: 4, color: color || '222222' } });
  s.addText([
    { text: '화면 구역', options: { bold: true, color: TITLE, breakLine: true, paraSpaceAfter: 6 } },
    L('리본 : ECS / MANUAL / LOG 탭, 맨 오른쪽 [통신] 신호등 4개(WMS1 · WMS2 · PLC · SCH). 빨강 = 끊김, 노랑↔초록 교대 = 정상', ORANGE),
    L('범례 : 작업 색상 · C/V 상태 · S/C·RGV 상태 - 색의 뜻. 항목 숨김·색은 리본 [범례] 창에서', PURPLE),
    L('설비 반송 : S/C 5대 · RGV 가 지금 나르는 작업', GREEN),
    L('작업정보 판넬 : 진행 중 작업 목록 → 고르면 아래에 단계표(SEQ). 머리줄의 [완료처리] 가 이 자료의 주된 처치 수단', BLUE),
    L('창고 그림 : 랙 10열 · S/C 5대 · RGV · C/V · 작업대(103 제품 입고대 / 104 원부자재 불출대 / 102 피킹 / 101 외부 입고). 트랙·설비를 더블클릭하면 상태창', RED),
    { text: ' ', options: { fontSize: 6, breakLine: true } },
    { text: '색으로 먼저 읽는다 (범례)', options: { bold: true, color: TITLE, breakLine: true, paraSpaceAfter: 6 } },
    L('작업 색상 : 입고(초록) · 출고(파랑) · 이동(하늘) · 반자동 입고/출고/이동(진한 색) - 트랙·크레인·RGV 칸의 작업번호 바탕색'),
    L('C/V 상태 : 입고대(진분홍) · 출고대(연보라) · 일시정지(빨강) · 에러(빨강) · 수동(회색) · 검색(분홍) · 화물 감지(칸 네 모서리의 점)'),
    L('S/C · RGV 상태 : 정상(연회색) · 수동(회색) · 에러(빨강) - 크레인·RGV 칸의 바탕색'),
    L('S/C 레일 : 입고 정지(주황) · 출고 정지(분홍) · 입출고 정지(진빨강) · 에러(보라) · 작업중(파랑) - 범례에는 없고 레일 끝점 색으로 보인다'),
  ], { x: 9.3, y: 1.0, w: 3.6, h: 5.9, fontFace: FONT, fontSize: 10.5, color: '222222', isTextBox: true, margin: 4, valign: 'top' });
  footer(s, pageNo);
}

// ═════════ 3. Conveyor ═════════
// 3-1 C/V 에러 (모터 과부하)
{
  const s = body('3', 'Troubleshooting - Conveyor', 'C/V 에러 - 모터 과부하 (트랙이 에러색)');
  img(s, 'crop_layout_cverr_1105.png', 0.53, 1.4, 3.0, 2.4, [[...LY.t124, RED]]);
  img(s, 'alarm_cverr_1105.png', 3.65, 1.4, 3.0, 1.75, [[...AL.msg, ORANGE], [...AL.row1, ORANGE]]);
  img(s, 'dlg_17110124_cverr_1105.png', 0.53, 4.05, 2.75, 2.95, [[...CV.err, ORANGE]]);
  img(s, 'legend_1105.png', 3.65, 3.45, 3.0, 0.9, [[...LG.cverr, RED]]);
  note(s, 3.65, 4.45, 3.0, 2.5, '알람 경고창', ['설비 에러가 나면 화면 오른쪽 아래에 스스로 뜬다(10초마다 확인). 해제되면 목록에서 빠진다. [STOP] 은 소리만 끄고 창을 감춘다.'], 8.5);
  caption(s, '창고 그림 - 트랙 124 가 에러색(빨강) / 알람 경고창(오른쪽 아래 자동)', 0.53, 1.1, 6.1);
  caption(s, 'C/V 상태창 - 에러코드 [0124] / 범례 C/V 상태 "에러"', 0.53, 3.82, 2.8);
  rhs(s,
    [B('트랙이 범례의 C/V 상태 "에러" 색(빨강)으로 바뀌고 화물이 다음 트랙으로 넘어가지 않는다.', { color: RED }),
     B('화면 오른쪽 아래에 알람 경고창이 뜬다. 트랙을 더블클릭한 C/V 상태창의 에러코드에 [코드] 문구가 보인다.', { color: ORANGE }),
     B('LOG ▷ 설비에러이력(EQP_ERR_HIS) 에 같은 내용이 남는다(30일).')],
    [B('이 현장 C/V 에서 ECS 로 올라오는 에러는 PLC 알람 리스트의 "C/V 모터 과부하" 한 가지다(트랙 119~131, 코드 = 트랙 번호). 벨트 끼임·화물 걸림으로 모터가 과전류(EOCR 트립)를 받으면 PLC 가 올린다.'),
     B('한 설비가 에러인 동안 스케줄러는 그 설비에만 지시를 내지 않는다. 다른 설비는 계속 돈다.')],
    [N('현장 조작반에서 물리적 원인을 없앤다(화물 정렬, 끼임 제거). 메인 판넬의 EOCR 트립을 리셋한다.'),
     N('조작반에서 알람을 해제하고 자동으로 돌린다. 설비가 하던 반송을 마치면 완료 신호가 올라온다.'),
     N('화면에서 트랙 색이 돌아오고 알람 경고창 목록에서 빠졌는지, 작업정보 판넬에서 작업이 다음 단계로 갔는지 확인한다.'),
     W_('ECS 에는 반송을 취소하거나 목적지를 바꾸는 수단이 없다(설비 인터페이스에 그런 신호가 없다). 설비에서 사람이 푼다.')]);
}
// 3-2 화면 데이터와 현장 화물 위치 불일치
{
  const s = body('3', 'Troubleshooting - Conveyor', 'C/V 데이터 불일치 - 화면의 작업번호 위치와 실제 화물 위치가 다르다');
  img(s, 'dlg_17110104_nolugg_1105.png', 0.53, 1.4, 3.3, 3.55, [[...CV.lugg, RED], [...CV.plt, RED], [...CV.cmd, BLUE], [...CV.auto, GREEN]]);
  img(s, 'crop_layout_nolugg_1105.png', 4.0, 1.4, 2.65, 2.1, [[...LY.cv2, RED]]);
  note(s, 4.0, 3.65, 2.65, 1.3, '창고 그림에서 보이는 모습', ['작업번호가 있는 화물 = 번호 글자가 그 칸에 보인다.', '번호 없는 화물 = 칸에 화물 감지 테두리만 있고 글자가 없다.'], 9);
  note(s, 0.53, 5.05, 6.12, 1.95, '명령 그룹의 뜻 (C/V 상태창 오른쪽)', [
    '쓰기 : 작업번호 칸에 적은 번호를 이 트랙에 기록   /   삭제 : 이 트랙의 번호를 지움',
    '복사 / 잘라내기 : 이 트랙 번호를 담아 둠(잘라내기는 지우면서)   /   붙여넣기 : 담아 둔 번호를 이 트랙에 기록. 통로 트랙(103~120)에 올리면 작업상태 16, 작업대는 15',
    '자동조회 : 켜 두면 값이 주기적으로 다시 읽힌다(아래 상태 신호도 함께)'], 9.5);
  caption(s, 'C/V 상태창 - 팔레트 감지 ON 인데 작업번호 0000 / 창고 그림 - 테두리만 있는 화물(104)', 0.53, 1.1, 6.1);
  rhs(s,
    [B('화물은 다음 트랙으로 갔는데 화면의 작업번호는 앞 트랙에 남아 있거나, 번호 없는 화물(화물 감지 테두리만, 상태창은 팔레트 감지 ON + 작업번호 0000)이 보인다.', { color: RED }),
     B('그 자리에서 작업이 멈추고 뒤따르는 작업이 전부 기다린다.')],
    [B('센서 오동작, 데이터 삭제 없이 화물을 들어냄, 조작반 수동으로 화물을 밀어 보냄 등 - 실물과 트래킹이 어긋난 것.'),
     B('C/V 트래킹은 PLC R 영역의 작업번호다. ECS 는 그 값을 읽어 그리며, 사람이 상태창 명령으로 고쳐 쓸 수 있다.')],
    [H('방법 1 : 화면대로 현장을 맞춘다'), N('화면에 보이는 위치로 화물을 옮긴다. 번호가 사라진 화물은 현장에서 들어낸다.', { indent: 1 }),
     H('방법 2 : 현장대로 화면을 맞춘다'), N('조작반을 수동으로 두고, 번호가 있는 트랙 상태창 [잘라내기] → 실제 화물이 있는 트랙 상태창 [붙여넣기]. 번호가 없으면 작업번호 칸에 적고 [쓰기].', { indent: 1, color: BLUE }),
     N('작업정보 판넬 단계표가 실물 위치의 줄을 진행중으로 가리키는지 확인한 뒤 자동으로 돌린다.', { indent: 1 }),
     W_('반송 중인 트랙에는 쓰지 않는다. 멈춘 뒤에 한다.(OP를 수동으로 전환)')]);
}
// 3-3 작업번호 없는 화물이 출고 H/S 에 있다
{
  const s = body('3', 'Troubleshooting - Conveyor', '작업번호 없는 화물이 출고 H/S(통로) 에 놓여 있다 - [H/S 배출]');
  img(s, 'crop_layout_nolugg_1105.png', 0.53, 1.4, 2.6, 2.1, [[...LY.cv2, RED]]);
  img(s, 'dlg_17110104_nolugg_1105.png', 3.25, 1.4, 2.2, 2.4, [[...CV.lugg, RED], [...CV.plt, RED], [...CV.hseject, BLUE]]);
  note(s, 5.55, 1.48, 1.1, 2.3, '[H/S 배출]', ['크레인 쪽 통로 칸(104)의 번호 없는 화물을 출고대까지 내보내는 반자동 작업을 만든다.'], 8.5);
  img(s, 'ask_8245_nolugg_1105.png', 0.53, 4.05, 2.6, 1.35);
  img(s, 'result_8245_nolugg_1105.png', 3.25, 4.05, 1.1, 0.8);
  img(s, 'crop_veh_nolugg_eject_1105.png', 3.25, 5.0, 3.4, 1.0, [[...VH.rgv, GREEN], [...VH.job1, GREEN]]);
  img(s, 'crop_layout_nolugg_eject_1105.png', 0.53, 5.45, 2.0, 1.55, [[...LY.rgvbusy, GREEN]]);
  caption(s, '창고 그림 - 출고 H/S(104)에 테두리만 있는 화물 / 그 트랙 C/V 상태창 예외 처리 [H/S 배출]', 0.53, 1.1, 6.1);
  caption(s, '[H/S 배출] 확인창 → 결과 / 반자동 출고 9001 이 생겨 RGV 가 나른다', 0.53, 3.82, 6.1);
  rhs(s,
    [B('출고 H/S(통로, 103~120) 트랙에 화물 감지 테두리만 있고 작업번호(글자)가 없다. 크레인·RGV 가 그 화물에는 손대지 않고, 뒷 작업이 멈춘다.', { color: RED }),
     B('C/V 상태창 : 작업번호 0000, 작업상태 N, 팔레트 감지 ON.', { color: RED })],
    [B('크레인을 수동으로 돌려 화물을 H/S 에 내려놓았거나, 트래킹 번호가 소실되었다(3-2 C/V 데이터 불일치). WCS 가 그 화물의 주인을 모른다.')],
    [H('경우 1 : 원래 작업이 아직 작업정보 목록에 있다'), N('판넬에서 그 작업을 고르고 [완료처리] → "대신 PORT:xx 에 화물이 감지됩니다 … 기록할까요?" [예]. 번호가 기록되면 스케줄러가 이어서 반송한다(4-6 참조).', { indent: 1 }),
     H('경우 2 : 작업이 없는 화물이다'), N('그 트랙 상태창의 예외 처리 [H/S 배출] → "트랙 xxx → 출고대 xxx 반자동 출고 작업을 만들어 화물을 내보냅니다" [예].', { indent: 1, color: BLUE }),
     N('배출 작업(9001~, 반자동 출고색)이 생겨 RGV 가 출고대까지 나른다. 상위에는 보고하지 않고 끝나면 지워진다.', { indent: 1, color: GREEN }),
     W_('그 자리에 두면 그 호기의 후속 작업이 전부 대기하므로 즉시 처리한다.')]);
}
// 3-4 출고대 도착했는데 작업 15 로 남음
{
  const s = body('3', 'Troubleshooting - Conveyor', '출고대에 화물이 왔는데 작업이 [15] 로 남는다 - 완료 보고 안 나감');
  img(s, 'crop_panel_st15out_sel_1105.png', 0.53, 1.4, 3.6, 2.25, [[...PN.row(2), RED], [...PN.seq(4), ORANGE], [...PN.done, BLUE]]);
  img(s, 'crop_layout_st15out_1105.png', 4.25, 1.4, 2.4, 1.9, [[...LY.t129, RED]]);
  img(s, 'ask_complete_st15out_0_1105.png', 0.53, 3.95, 2.9, 1.15);
  img(s, 'ask_complete_st15out_1_1105.png', 3.55, 3.95, 3.1, 1.55);
  img(s, 'dlg_17110129_st15out_1105.png', 0.53, 5.2, 1.7, 1.8, [[...CV.lugg, RED], [...CV.plt, RED]]);
  note(s, 2.35, 5.6, 2.3, 1.4, '확인창 2개의 뜻', ['첫 창 = 상태를 15 → 19 로 넘긴다(완료 보고).', '둘째 창 = 출고대 칸에 화물은 감지되는데 번호가 없을 때만 뜬다. [예] 면 그 칸에 번호를 기록한다(C/V 상태창 [쓰기] 와 같다).'], 8.5);
  caption(s, '작업정보 판넬 - 0252 가 [15] 그대로, 단계표 마지막 줄(출고대 C/V) 진행중 / 창고 그림 - 출고대(129)에 테두리만', 0.53, 1.1, 6.1);
  caption(s, '[완료처리] 확인창 [15 → 19] → 이어서 "기록할까요?" / 출고대 C/V 상태창', 0.53, 3.72, 6.1);
  rhs(s,
    [B('출고 화물이 출고대(104 원부자재 불출대 · 102 피킹 · 101)에 실제로 도착했는데 작업정보의 상태가 [15] CV 구동중 그대로이고 목록에서 사라지지 않는다. 출고대 칸에는 번호 없는 화물(테두리만).', { color: RED }),
     B('상위(WMS)에 완료 보고가 나가지 않는다.')],
    [B('출고대의 도착 신호는 3초 펄스라 놓칠 수 있고, 도착 칸의 번호가 지워진 채 오면 "누가 왔는지" 를 모른다.'),
     B('OUT_DONE_ON_ARRIVAL=1 (2026-09-30 부터, 스케줄러 WCS_DB.INI) : 도착 칸에 작업번호가 붙은 화물이 실제로 감지되면 펄스를 기다리지 않고 그 자리에서 완료(19)·상위 보고한다. 그래서 15 로 남는 것은 도착 칸에 번호가 없을 때뿐이다 - 번호 소실, 또는 지게차가 이미 가져가 화물 자체가 없을 때.')],
    [N('작업정보 판넬에서 그 작업을 고른다(단계표 마지막 줄 출고대 C/V 가 진행중).', { color: BLUE }),
     N('[완료처리] → "… 구간을 완료 처리하시겠습니까? [15 -> 19]" [예]. 이어서 "상태는 넘겼습니다. 그런데 PORT:xx 에 화물은 있는데 작업번호가 없습니다 … 기록할까요?" 가 뜨면 [예].'),
     N('상위로 완료 보고가 나가고 목록에서 사라진다. 출고대에서 화물을 반출한다.', { color: GREEN }),
     N('즉, 이때는 화물을 이미 지게차가 가져간 것으로 보이니 로그(LOG ▷ 작업이력 · 설비이력)를 확인하여 화물이 나온 것이 맞다면 작업정보창에서 도착 보고(완료처리)를 한다.'),
     W_('실물이 출고대에 없는데 넘기면 상위 재고와 어긋난다. 실물·로그를 먼저 확인한다.')]);
}
// 3-5 S/C#1 통로 방향
{
  const s = body('3', 'Troubleshooting - Conveyor', 'S/C#1 통로(C/V#2) 방향 불일치 - 입고·출고가 서로 기다린다');
  img(s, 'crop_layout_1105.png', 0.53, 1.4, 3.4, 2.7, [[...LY.cv2dir, RED], [...LY.cv2, ORANGE], [...LY.lbl103, ORANGE], [...LY.lbl104, ORANGE], [...LY.cv11dir, BLUE]]);
  img(s, 'ask_dir_cv2_1105.png', 4.05, 1.4, 2.6, 1.45);
  caption(s, '방향 글자 더블클릭 → 확인창(바꿀 PLC 주소와 값)', 4.05, 2.95, 2.6);
  note(s, 4.05, 3.25, 2.6, 2.0, '103·104 는 C/V#2 한 라인을 함께 쓴다', [
    '103 (TR#24) 제품 입고대 = 입고 방향, 104 (TR#26) 원부자재 불출대 = 출고 방향. C/V#2 칸 위의 "입고 모드"/"출고 모드" 글자가 지금 방향이다.',
    '101 (C/V#11, TR#22) 도 방향 전환형 입출고대라 아래에 같은 글자가 있다.',
    '방향 글자를 더블클릭하면 바꿀 PLC 주소를 보여 주는 확인창이 뜨고 [예] 로 즉시 전환된다.'], 9);
  note(s, 0.53, 4.3, 3.4, 2.7, '스케줄러의 방향 전환 규칙', [
    '입고 화물이 103 쪽으로 오면 입고 모드로, 출고 화물이 104 로 나오면 출고 모드로 스케줄러가 바꾼다.',
    '사람이 수동 전환하면 그 뒤 120초 동안은 스케줄러가 되돌리지 않는다.',
    '통로(103/104)에 화물이 실린 채로는 바꾸지 않는다 - 되끌림 방지.'], 9);
  caption(s, '창고 그림 - C/V#2 방향 글자("입고 모드"/"출고 모드")와 103·104 / 아래 C/V#11(101) 방향 글자', 0.53, 1.1, 6.1);
  rhs(s,
    [B('입고 화물이 103 에서 멈춘 채 S/C#1 이 안 받거나, 출고 화물이 104 쪽으로 못 나오고 통로에서 기다린다.', { color: ORANGE }),
     B('방향 글자가 지금 하려는 작업과 반대("출고 모드" 인데 입고를 보내는 중 등)로 보인다.', { color: RED })],
    [B('S/C#1 은 입고·출고가 C/V#2 한 라인을 함께 쓴다. 스케줄러가 방향을 바꿔 주지만 전환 신호가 어긋나거나, 양쪽 작업이 서로 사용권을 기다리면(교착) 멈춘다.'),
     B('그 라인이 막히면 1호기 작업이 같이 선다.')],
    [N('메인 화면에서 C/V#2 의 방향 글자를 더블클릭한다. 확인창에 바꿀 PLC 주소가 보이면 [예].', { color: BLUE }),
     N('수동 전환은 즉시 적용되고, 그 뒤 120초 동안은 스케줄러가 되돌리지 않는다. 그 사이 멈춘 작업이 흘러가는지 본다.'),
     N('반복되면 운영자에게 : IO_TASK 로그의 "사용권" 줄과 WCS_DB.INI CV2_DIR_MODE 를 본다(01 운영자 매뉴얼 9장).'),
     W_('통로에 화물이 실린 채 방향을 바꾸면 화물이 되끌려 갈 수 있다. 벨트가 멈춘 것을 보고 바꾼다.')]);
}

// ═════════ 4. Stacker Crane ═════════
// 4-1 입고 H/S 준비 안 됨
{
  const s = body('4', 'Troubleshooting - Stacker Crane', 'S/C 입고 이상 - 입고 H/S 가 준비되지 않음 (PLC 신호)');
  img(s, 'dlg_17110104_1105.png', 0.53, 1.4, 2.9, 3.1, [[...CV.inhs, RED], [...CV.automode, RED], [...CV.plt, RED], [...CV.lugg, ORANGE]]);
  img(s, 'dlg_17120901_1105.png', 3.55, 1.4, 2.6, 3.1, [[...SC.ucsta, ORANGE], [...SC.susp, ORANGE], [...SC.err, ORANGE]]);
  img(s, 'crop_layout_1105.png', 0.53, 4.7, 2.9, 2.3, [[...LY.cv2, RED], [...LY.sc1, ORANGE]]);
  note(s, 3.55, 4.7, 3.1, 2.3, '스케줄러가 크레인에 입고를 지시하는 조건', [
    '작업상태 16 (통로 C/V 구동중)',
    '+ 입고 H/S 트랙(크레인 쪽 칸, 1호기는 104)에 팔레트 감지 ON · 같은 작업번호',
    '+ 그 트랙의 입고 HS 신호 ON · 자동모드 ON',
    '+ 크레인 IDLE · 에러 없음 · 입고 정지 아님',
    '하나라도 빠지면 조용히 기다린다(알람 없음).'], 9.5);
  caption(s, '입고 H/S 트랙(104) C/V 상태창 - 아래 "상태" 신호 / S/C 상태창 - IDLE · 입출고상태 · 에러 / 창고 그림 - 통로 칸과 크레인', 0.53, 1.1, 6.1);
  rhs(s,
    [B('S/C 는 IDLE 이고 에러도 없고 입고 정지도 아닌데, 입고 H/S(통로의 크레인 쪽 칸)에 화물·작업번호가 있어도 움직이지 않는다.', { color: ORANGE }),
     B('작업정보의 상태가 [16] 통로 CV 구동중에서 머문다.')],
    [B('PLC 가 그 트랙의 입고 HS 신호(입고 가능)를 올리지 않았다 - 자동모드 OFF, 팔레트 감지 OFF, 또는 입고 HS 신호 OFF.', { color: RED }),
     B('스케줄러는 "상태 16 + 통로 트랙의 화물·같은 작업번호 + HS 신호 + 크레인 IDLE" 이 다 맞아야 지시한다(왼쪽 아래 상자).')],
    [N('그 트랙을 더블클릭해 C/V 상태창 아래 신호를 본다. 입고 HS 신호가 OFF 면 PLC 쪽이다 - 조작반을 자동으로 두고, 그래도 OFF 면 PLC 담당자에게 문의한다.', { color: RED }),
     N('작업상태가 [15] 인데 화물이 이미 통로 H/S 에 있으면(RGV 가 내린 뒤) 단계표의 통로 C/V 줄을 골라 [완료처리] 로 16 으로 넘긴다.'),
     N('작업번호가 그 트랙에 없으면 4-4 (입고 H/S 의 작업 정보 없음) 로 간다.')]);
}
// 4-2 입고/출고 금지
{
  const s = body('4', 'Troubleshooting - Stacker Crane', 'S/C 가 자동 작업을 받지 않음 - 입고 정지 · 출고 정지 · 작업금지');
  img(s, 'dlg_17120901_instop_1105.png', 0.53, 1.4, 3.0, 3.6, [[...SC.susp, RED], [...SC.stopbtns, BLUE]]);
  img(s, 'crop_layout_instop_1105.png', 3.65, 1.4, 3.0, 2.4, [[...LY.rail1, RED]]);
  img(s, 'ask_1087_instop_off_1105.png', 3.65, 3.95, 2.0, 1.3);
  img(s, 'result_1087_instop_off_1105.png', 5.8, 3.95, 0.85, 0.7);
  img(s, 'dlg_17120901_instop_off_1105.png', 0.53, 5.15, 1.55, 1.85, [[...SC.susp, GREEN]]);
  caption(s, 'S/C 상태창 - 입출고상태 "입고정지" 와 [입고 정지] [출고 정지] [작업금지] / 창고 그림 - 1호기 레일 끝점이 입고 정지색(주황)', 0.53, 1.1, 6.1);
  caption(s, '해제 : 같은 단추를 다시 → "S/C 정상처리 하시겠습니까?" [예] → 성공 → 입출고상태 "-"', 2.2, 5.65, 4.4);
  rhs(s,
    [B('S/C 상태창 입출고상태 칸에 "입고정지"/"출고정지"/"입출고정지" 가 보이고, 창고 그림의 그 호기 레일이 입고 정지(주황) · 출고 정지(분홍) · 입출고 정지(진빨강) 색이 된다(현재 레이아웃에서는 레일 끝점에 색이 보인다).', { color: RED }),
     B('그 방향 작업이 그 호기에 지시되지 않는다(작업정보에서 [10]/[16] 으로 기다림).')],
    [B('사람이 상태창에서 [입고 정지] / [출고 정지] / [작업금지] 를 눌러 둔 상태다(점검 · 수동 작업 뒤 해제를 잊음).'),
     B('스케줄러는 정지된 방향의 작업을 그 호기에 내지 않는다.')],
    [N('S/C 상태창을 열어 입출고상태 칸을 본다.'),
     N('켜져 있는 [입고 정지] / [출고 정지] / [작업금지] 를 다시 누른다 → "S/C 정상처리 하시겠습니까?" [예].', { color: BLUE }),
     N('입출고상태가 "-" 로 돌아오고 레일 색이 원래대로 된다. 기다리던 작업이 지시된다.', { color: GREEN }),
     N('누가 언제 눌렀는지는 LOG ▷ 유저사용로그(WCS_CLIENT_LOG) 에 남는다.')]);
}
// 4-3 수동 / 에러
{
  const s = body('4', 'Troubleshooting - Stacker Crane', 'S/C 가 자동 작업을 받지 않음 - 수동 모드 · 에러 (크레인 칸 색)');
  img(s, 'crop_layout_scerr_1105.png', 0.53, 1.4, 2.9, 2.3, [[...LY.sc1, RED]]);
  img(s, 'dlg_17120901_scerr_1105.png', 3.55, 1.4, 2.6, 3.1, [[...SC.ucsta, RED], [...SC.err, RED], [...SC.resend, BLUE]]);
  img(s, 'alarm_scerr_1105.png', 0.53, 3.95, 2.9, 1.7, [[...AL.msg, ORANGE]]);
  img(s, 'legend_1105.png', 0.53, 5.7, 2.9, 0.86, [[...LG.scerr, RED]]);
  note(s, 3.55, 4.85, 3.1, 1.3, '동작상태 칸의 값', ['IDLE = 자동 · 대기 / RUN = 자동 · 작업중 / DOWN = 수동이거나 알람(지시를 받지 않음). 에러 칸은 PLC 알람 리스트(SC_LGLS) 의 [코드] 문구.'], 9);
  caption(s, '창고 그림 - 1호기 칸이 에러색(빨강) / S/C 상태창 - 동작상태 DOWN · 에러 [코드] 문구 / 알람 경고창', 0.53, 1.1, 6.1);
  rhs(s,
    [B('메인 화면의 크레인 칸이 범례 S/C·RGV 상태의 수동(회색) 또는 에러(빨강) 색이고 지시를 받지 않는다.', { color: RED }),
     B('상태창 동작상태가 DOWN 이거나 에러 칸에 [코드] 문구가 있다.', { color: RED }),
     B('에러는 알람 경고창에도 뜬다.', { color: ORANGE })],
    [B('지상반에서 수동으로 돌려 놓았거나(자동 아님), 설비 알람이 걸려 있다.'),
     B('알람 중에는 스케줄러가 그 호기에 지시를 내지 않는다. 지시를 받은 채 멈춘 것이면 지시는 설비에 남아 있다.')],
    [N('지상반에서 알람 원인을 없애고 알람을 해제한 뒤 자동으로 전환한다. 화면 색이 정상(연회색)으로 돌아온다.'),
     N('크레인이 지시를 받은 채 멈췄던 것이면 그 뒤 스스로 이어서 움직인다. 지시가 날아갔으면 상태창 [지시 재전송].', { color: BLUE }),
     W_('포크 위에 화물을 든 채 멈췄으면 4-5 / 4-6 으로 간다.')]);
}
// 4-4 입고 작업이 없음
{
  const s = body('4', 'Troubleshooting - Stacker Crane', 'S/C 입고 이상 - 입고 H/S 의 화물에 작업 정보가 없음');
  img(s, 'dlg_17110104_nolugg_1105.png', 0.53, 1.4, 2.9, 3.1, [[...CV.lugg, RED], [...CV.plt, RED], [...CV.hseject, BLUE]]);
  img(s, 'view_joblist_1105.png', 3.55, 1.4, 3.1, 2.0, [[973, 278, 106, 22, BLUE]]);
  img(s, 'manual_job_1105.png', 3.55, 3.5, 3.1, 1.1, [[125, 160, 180, 23, BLUE], [410, 197, 180, 21, BLUE], [608, 138, 104, 26, BLUE]]);
  img(s, 'crop_layout_nolugg_1105.png', 0.53, 4.65, 2.9, 2.35, [[...LY.cv2, RED]]);
  img(s, 'popup_route_found.png', 3.55, 4.7, 3.1, 1.6);
  caption(s, '입고 H/S(104) C/V 상태창 - 팔레트 감지 ON · 작업번호 0000 / ECS ▷ 뷰 ▷ 작업정보 [복사] / MANUAL ▷ 작업(수동지시) / 창고 그림 / [완료처리] 안내', 0.53, 1.1, 6.1);
  rhs(s,
    [B('S/C 는 받을 수 있는 상태인데 입고 H/S 의 화물에 작업번호가 없어(화물 감지 테두리만, 상태창 작업번호 0000) 크레인이 입고하지 않는다.', { color: RED })],
    [B('작업자 실수 · 수동 하역 · 트래킹 소실(3-2)로 H/S 에 도착한 화물에 작업정보가 없으면 스케줄러는 입고를 지시하지 않는다.')],
    [H('방법 1 : 원래 입고 작업이 목록에 있다'), N('판넬에서 그 작업을 고르고 [완료처리] → "대신 PORT:xx 에 … 기록할까요?" [예] (4-6).', { indent: 1 }),
     H('방법 2 : 작업이 없다 - 입고 위치를 안다'), N('MANUAL ▷ 작업 에서 반자동 입고(출발 = 그 H/S 트랙, 도착 위치 00-000-00)를 [작업 생성].', { indent: 1, color: BLUE }),
     N('ECS ▷ 뷰 ▷ 작업정보 에서 그 작업 [복사] → H/S 트랙 C/V 상태창 [붙여넣기].', { indent: 1, color: BLUE }),
     H('방법 3 : 어디로 갈지 모른다'), N('C/V 상태창 [H/S 배출] 로 출고대로 내보낸다(3-3). (단, 이 경우 크레인 수동 조작으로 입고 H/S 의 화물을 출고 H/S 로 옮겨야 한다.)', { indent: 1, color: BLUE })], { size: 10 });
}
// 4-5 하역 Ack 유실 (25)
{
  const s = body('4', 'Troubleshooting - Stacker Crane', 'S/C 작업이 [25] 에서 오래 머문다 - 하역 완료 신호(Ack) 유실');
  img(s, 'crop_panel_st25ack_sel_1105.png', 0.53, 1.4, 3.6, 2.25, [[...PN.row(2), RED], [...PN.seq(1), ORANGE], [...PN.done, BLUE]]);
  img(s, 'crop_layout_st25ack_1105.png', 4.25, 1.4, 2.4, 1.9, [[...LY.sc1busy, RED], [...LY.cv2, RED]]);
  img(s, 'dlg_17120901_st25ack_1105.png', 0.53, 3.95, 2.5, 3.0, [[...SC.ucsta, ORANGE], [...SC.job, RED], [...SC.resend, BLUE], [...SC.hmidel, BLUE]]);
  img(s, 'ask_complete_st25ack_0_1105.png', 3.15, 3.95, 3.5, 1.3);
  img(s, 'ask_complete_st25ack_1_1105.png', 3.15, 5.3, 2.4, 1.1);
  note(s, 5.6, 5.3, 1.05, 1.1, 'RUN 확인창', ['크레인이 아직 RUN 이면 한 번 더 묻는다.'], 8);
  caption(s, '작업정보 판넬 - 0284 가 [25] 그대로, 단계표 S/C 줄 진행중 / 창고 그림 - 1호기 칸에 번호·작업중색이 남고 통로 칸(104)은 테두리만', 0.53, 1.1, 6.1);
  caption(s, 'S/C 상태창 - 작업상태 칸의 [25] 작업 · 동작상태 RUN / [완료처리] 확인창 [25 → 29]', 0.53, 3.72, 6.1);
  rhs(s,
    [B('작업이 [25] S/C 구동중(출고) 또는 입고의 S/C 단계에 오래 머문다. 메인 화면 크레인 칸에 작업번호·작업중색이 남고, 통로 H/S 에는 화물 감지 테두리만 있고 작업번호가 없다.', { color: RED }),
     B('크레인은 IDLE 로 돌아와 있거나, 하역을 끝냈는데도 RUN 인 채 멈춰 있다(상태창 동작상태).', { color: ORANGE })],
    [B('크레인은 하역을 끝냈는데 하역 완료(Ack) 신호가 ECS 에 오지 않았다. ECS 는 설비 신호로만 완료를 판정하므로 그 상태에 선다.'),
     B('크레인이 지시 자체를 못 받았을 수도 있다(지시는 상태창 SC상태 칸의 출발·도착 위치로 확인).')],
    [N('설비가 지시를 못 받은 것 같으면 S/C 상태창 [지시 재전송] 을 먼저 누른다.', { color: BLUE }),
     N('이미 일을 끝낸 것이면 작업정보 판넬에서 그 작업을 고르고 [완료처리] → [25 → 29] [예]. 크레인이 아직 RUN 이면 "화물을 이미 내려놓은 것이 확실합니까?" 를 한 번 더 묻는다 - 실물을 눈으로 확인한 뒤 [예].'),
     N('크레인 지시가 풀리고, H/S 화물에 번호가 찍혀 통로 벨트가 이어서 옮긴다.', { color: GREEN }),
     N('크레인에 지난 데이터가 남아 다음 지시를 안 받으면 S/C 상태창 [데이터 삭제](4-7) 를 누른 뒤 [완료처리] 한다.', { color: BLUE }),
     W_('[완료처리] 는 DB 만 바꾼다. 포크 위 화물은 센서에 잡히지 않으니 먼저 내려놓는다.')]);
}
// 4-6 수동 조작으로 H/S 에 내려놓음 (실제 사례 : 현장 2161)
{
  const s = body('4', 'Troubleshooting - Stacker Crane', 'S/C 수동 조작 뒤 작업 단계와 실물 위치가 다르다 (실제 사례 : 현장 2161)');
  img(s, 'panel_seq_0758.png', 0.53, 1.4, 3.6, 3.3, [[10, 385, 410, 18, ORANGE], [10, 420, 410, 18, RED], [530, 335, 85, 22, BLUE]]);
  img(s, 'popup_route_found.png', 4.25, 1.4, 2.4, 1.4);
  img(s, 'popup_route_none.png', 4.25, 2.95, 2.4, 1.5);
  img(s, 'dlg_17120901_1105.png', 0.53, 4.85, 1.85, 2.15, [[...SC.pos, ORANGE], [...SC.hmidel, BLUE]]);
  note(s, 2.5, 4.9, 4.15, 2.1, '실제 사례 : 현장 2161 (2026-10-27)', [
    '급한 출고 때문에 크레인을 수동으로 돌려 입고 화물을 H/S 에 두고 H/S 데이터를 지웠다. 작업은 [15] 첫 줄 진행중, 화물은 두 단계 뒤 PORT:12.',
    '종전 판은 "PORT:23 에 화물이 없다" 로 끝났다. 10-28 판부터 경로의 다른 칸을 찾아 넘기자고 묻는다(위 안내창).'], 9.5);
  caption(s, '작업정보 판넬 - 진행중 줄(주황)과 실물이 있는 줄(빨강) / 경로 탐색 안내창 2종 / S/C 상태창 지시 위치', 0.53, 1.1, 6.1);
  rhs(s,
    [B('작업정보의 단계표는 첫 줄(입고대 C/V)이 진행중인데, 화물은 이미 통로 H/S(두 단계 뒤)에 내려와 있다. 크레인에는 지시가 없다.', { color: RED }),
     B('그 자리에서 [완료처리] 를 누르면 "대신 PORT:xx (단계 000n …) 에 화물이 감지됩니다 … 기록할까요?" 가 뜬다.')],
    [B('작업 중 에러 또는 급한 작업으로 크레인을 수동 조작해 화물을 옮기고 설비 데이터를 지웠다. ECS 는 단계가 바뀐 것을 모른다.')],
    [N('작업정보 판넬에서 그 작업을 고르고 [완료처리]. 찾은 칸을 알려 주면 [예] - 상태 이동(15→16)과 그 칸의 작업번호 기록이 한 번에 끝나고 스케줄러가 이어서 입고한다.', { color: BLUE }),
     N('"어느 칸에도 없다" 가 뜨면 - 설비에 실린 채라면 내린 뒤 그 단계 줄을 골라 다시 / 이미 랙·작업대까지 갔다면 마지막 줄을 골라 [완료처리](29).'),
     N('크레인에 지난 데이터가 남아 새 지시를 안 받으면 4-7 [데이터 삭제].'),
     W_('[아니오] 는 아무것도 바꾸지 않는다. 포크 위 화물은 먼저 내린다.')]);
}
// 4-7 데이터 잔존 → [데이터 삭제]
{
  const s = body('4', 'Troubleshooting - Stacker Crane', 'S/C 에 지난 데이터가 남아 새 지시를 안 받음 - [데이터 삭제]');
  img(s, 'dlg_17120901_1105.png', 0.53, 1.4, 3.3, 3.95, [[...SC.hmidel, BLUE], [...SC.pos, RED], [...SC.job, ORANGE]]);
  img(s, 'hmidel_ask_1104.png', 3.95, 1.4, 2.7, 1.3);
  img(s, 'rtv_hmidel_ask_1104.png', 3.95, 2.85, 2.7, 1.3);
  note(s, 3.95, 4.3, 2.7, 2.7, '무엇이 일어나나', [
    '[예] → 설비 통신이 PLC 의 그 호기 삭제 비트에 1 을 쓴다 = 지상반 [삭제] 와 같다. 설비가 자기 작업 데이터를 지우고 비트를 스스로 내린다.',
    '비트 주소가 WCS_DB.INI [HMI_DELETE] 에 없으면 이력만 남고 PLC 에는 쓰지 않는다(운영자가 기입).',
    '요청은 LOG ▷ 유저사용로그에 남는다.'], 9.5);
  caption(s, 'S/C 상태창 - 명령 그룹의 [데이터 삭제] (2026-11-04 추가) · SC상태 칸의 지난 지시 위치 / 확인창 (S/C · RGV)', 0.53, 1.1, 6.1);
  rhs(s,
    [B('크레인이 비정상으로 멈춘 뒤 화물을 내려놓고 알람도 풀었는데, 새 작업을 지시해도 크레인이 받지 않는다.'),
     B('상태창 SC상태 칸에 지난 지시의 출발·도착 위치가 남아 있다. 지상반에는 이전 작업 데이터가 보인다.', { color: RED }),
     B('작업상태 칸(아래)은 ECS 쪽 작업 정보라 [완료처리] 전까지 그대로다.', { color: ORANGE })],
    [B('설비(PLC) 안에 지난 작업 데이터가 남아 있어 새 지시와 충돌한다. 지금까지는 지상반(터치)의 [삭제] 를 눌러야 했다.')],
    [N('설비 위 화물을 먼저 내려놓는다(크레인은 출고 H/S 에).'),
     N('S/C 상태창 [데이터 삭제] → 확인창 [예]. "요청했습니다" 안내가 뜨면 끝.', { color: BLUE }),
     N('지상반에서 데이터가 지워졌는지 본다. 안 지워지면 종전대로 지상반 [삭제].'),
     N('화면의 작업 정보는 지워지지 않는다. 작업까지 정리하려면 [완료처리] 를 이어서 쓴다(4-5 / 4-6).'),
     { t: '버튼이 안 보이면 Ecs.ini [MENU] HMIDEL_BTN=0 이다(운영자).', color: GRAY }]);
}

// ═════════ 5. RGV ═════════
// 5-1 출발지/도착지 준비 안 됨
{
  const s = body('5', 'Troubleshooting - RGV', 'RGV 가 움직이지 않음 - 출발지·도착지 준비 안 됨 (PLC 신호)');
  img(s, 'dlg_17110124_1105.png', 0.53, 1.4, 2.9, 3.1, [[...CV.rgvdep, RED], [...CV.rgvarr, RED], [...CV.automode, ORANGE], [...CV.plt, ORANGE]]);
  img(s, 'dlg_17130801_rtvsusp_1105.png', 3.55, 1.4, 2.6, 2.65, [[...RTV.sta, ORANGE], [...RTV.err, ORANGE]]);
  img(s, 'crop_layout_1105.png', 0.53, 4.7, 2.9, 2.3, [[...LY.t124, RED], [...LY.rgv, ORANGE]]);
  note(s, 3.55, 4.2, 3.1, 2.8, '스케줄러가 RGV 에 지시하는 조건', [
    '작업상태 15 (입고대 C/V 끝) 또는 출고의 16 (통로 RGV 측)',
    '+ 출발 트랙에 팔레트 감지 ON · 같은 작업번호 · RGV 출발 HS 신호 ON',
    '+ 도착 트랙이 비어 있고 RGV 도착 HS 신호 ON',
    '+ RGV IDLE · 에러 없음 · [RTV 금지] 아님',
    '하나라도 빠지면 조용히 기다린다(알람 없음).'], 9.5);
  caption(s, '출발지(입고대 124) C/V 상태창 - RGV 출발/도착 HS 신호 · 자동모드 · 팔레트 감지 / RTV 상태창 - IDLE · 에러 없음 / 창고 그림', 0.53, 1.1, 6.1);
  rhs(s,
    [B('RGV 는 IDLE 이고 에러·금지가 아닌데, 작업이 [15] 입고대 C/V 끝 또는 출고의 [16] 통로 끝에서 머문다.', { color: ORANGE }),
     B('출발지(입고대 끝 트랙 / 통로 RGV 측 트랙)에 화물·번호가 있는데 RGV 가 가지 않는다.', { color: RED })],
    [B('PLC 가 출발지의 RGV 출발 HS 신호 또는 도착지의 RGV 도착 HS 신호를 올리지 않았다 - 자동모드 OFF, 팔레트 감지 상태 불일치, 도착지에 다른 화물.', { color: RED }),
     B('스케줄러는 출발·도착 두 트랙의 신호가 다 맞아야 RGV 에 지시한다(왼쪽 아래 상자).')],
    [N('출발지 트랙과 도착지 트랙을 각각 더블클릭해 C/V 상태창 아래 신호를 본다.', { color: RED }),
     N('도착지에 화물이 남아 있으면 먼저 비운다(그 화물의 작업을 3-3 / 4-4 로 처리).'),
     N('신호가 OFF 인 쪽의 조작반을 자동으로 두고, 그래도 OFF 면 PLC 담당자에게 문의한다.'),
     N('작업번호가 출발지 트랙에 없으면 5-4 (RGV 픽업 칸에 번호 없음).')]);
}
// 5-2 RTV 금지
{
  const s = body('5', 'Troubleshooting - RGV', 'RGV 가 자동 작업을 받지 않음 - [RTV 금지] 상태 · 수동 · 에러');
  img(s, 'dlg_17130801_rtvsusp_1105.png', 0.53, 1.4, 3.1, 3.3, [[...RTV.suspbox, RED], [...RTV.suspbtn, BLUE], [...RTV.sta, ORANGE], [...RTV.err, ORANGE]]);
  img(s, 'ask_1086_rtvsusp_toggle_1105.png', 3.75, 1.4, 1.75, 1.2);
  img(s, 'result_1086_rtvsusp_toggle_1105.png', 5.6, 1.4, 0.9, 1.2);
  img(s, 'crop_panel_rtvsusp_1105.png', 3.75, 2.9, 2.9, 1.85, [[0, 30, 624, 52, RED]]);
  img(s, 'legend_1105.png', 0.53, 5.05, 2.3, 0.7, [[...LG.scstate, ORANGE]]);
  img(s, 'crop_layout_rtvsusp_1105.png', 2.95, 5.05, 3.7, 1.95, [[...LY.rgv, ORANGE], [...LY.t124, RED], [...LY.t130, RED]]);
  caption(s, 'RTV 상태창 - 정지 칸이 비어 있고 [RTV 금지] / 누르면 "Rtv를 사용하시겠습니까?" → 성공 / 작업이 [15]/[16] 에서 기다림', 0.53, 1.1, 6.1);
  caption(s, '범례 S/C·RGV 상태 / 창고 그림 - RGV 는 홈', 0.53, 4.8, 6.1);
  rhs(s,
    [B('RGV 가 홈(IDLE)에 선 채 지시를 받지 않고, 입고대·통로의 작업이 [15]/[16] 에서 줄줄이 기다린다. RTV 상태창의 정지 칸이 비어 있다([RTV 금지] 상태).', { color: RED }),
     B('수동·에러면 RGV 칸이 범례 S/C·RGV 상태의 수동(회색) / 에러(빨강)이고 RTV상태가 IDLE 이 아니거나 에러 칸에 [코드] 문구가 있다.', { color: ORANGE })],
    [B('[RTV 금지] 를 눌러 둔 상태(점검 · 수동 작업 뒤 해제를 잊음), RGV 컨트롤러가 수동, 또는 알람. 금지 상태는 알람이 아니라 화면에 조용하다.'),
     B('RGV 가 홈에서 전혀 안 바뀌어 보이면 화면 조회 설정 문제일 수 있다(RTV_DATA_LGLS.PLC_NO 자릿수, 운영자).')],
    [N('RTV 상태창 [RTV 금지] 를 누른다 → "Rtv를 사용하시겠습니까?" [예] → 성공. 기다리던 작업이 바로 지시된다.', { color: BLUE }),
     N('RGV 컨트롤러(지상반)에서 알람을 풀고 자동으로 전환한다.'),
     N('지시가 날아갔으면 [지시 재전송]. 하역을 끝냈는데 보고가 없으면 5-3.'),
     N('화면만 안 바뀌면 운영자에게(01 운영자 매뉴얼 9장 "RGV 가 홈에 고착").')]);
}
// 5-3 하역 보고 유실 (35)
{
  const s = body('5', 'Troubleshooting - RGV', 'RGV 작업이 [35] 에서 오래 머문다 - 하역 완료 보고 유실');
  img(s, 'crop_panel_st35_sel_1105.png', 0.53, 1.4, 3.6, 2.25, [[...PN.row(1), RED], [...PN.seq(3), ORANGE], [...PN.done, BLUE]]);
  img(s, 'dlg_17130801_st35_1105.png', 4.25, 1.4, 2.4, 2.45, [[...RTV.sta, ORANGE], [...RTV.job, RED], [...RTV.resend, BLUE]]);
  img(s, 'ask_complete_st35_0_1105.png', 0.53, 3.95, 3.0, 1.2);
  img(s, 'crop_veh_st35_1105.png', 0.53, 5.25, 3.0, 0.85, [[...VH.rgv, RED]]);
  img(s, 'crop_layout_st35_1105.png', 3.65, 4.0, 3.0, 2.4, [[...LY.rgvbusy, RED], [...LY.t127, RED]]);
  note(s, 0.53, 6.2, 3.0, 0.8, '처리 뒤', ['[35 → 39] 로 넘기고 도착 칸에 번호를 기록 → 통로 벨트(입고) / 출고대(출고)로 이어진다.'], 8.5);
  caption(s, '작업정보 판넬 - 0298 이 [35] 그대로, 단계표 RGV 줄 진행중 / RTV 상태창 - IDLE 인데 작업상태 칸에 [35] 작업', 0.53, 1.1, 6.1);
  caption(s, '[완료처리] 확인창 [35 → 39] / 설비 반송 RGV 줄 / 창고 그림 - RGV 칸 · 하역 칸(127)', 0.53, 3.72, 6.1);
  rhs(s,
    [B('작업이 [35] RGV 구동중에 머문다. 설비 반송 칸의 RGV 줄과 창고 그림의 RGV 칸에 작업이 남아 있고, 도착 트랙(통로 RGV 측 / 출고대 쪽)에는 화물 감지 테두리만 있고 작업번호가 없다.', { color: RED }),
     B('RGV 는 IDLE 로 돌아와 있다(RTV 상태창).', { color: ORANGE })],
    [B('RGV 가 하역을 끝냈는데 하역 완료 보고가 ECS 에 오지 않았다. ECS 는 설비 신호로만 완료를 판정한다.')],
    [N('RGV 가 지시를 못 받은 것 같으면 RTV 상태창 [지시 재전송].', { color: BLUE }),
     N('이미 내려놓았으면 작업정보 판넬에서 그 작업을 고르고 단계표 RGV 줄을 둔 채 [완료처리] → "RGV 구간을 완료 처리하시겠습니까? [35 -> 39]" [예].', { color: BLUE }),
     N('RGV 지시가 풀리고 도착 칸에 작업번호가 기록되어 통로 벨트(입고) 또는 출고대(출고)로 이어진다.', { color: GREEN }),
     N('RGV 에 지난 데이터가 남아 다음 지시를 안 받으면 5-5 [데이터 삭제].')]);
}
// 5-4 작업 없음 (RGV 픽업 칸)
{
  const s = body('5', 'Troubleshooting - RGV', 'RGV 픽업 칸에 화물은 있는데 작업번호가 없다 - 작업 [16] 머묾');
  img(s, 'crop_panel_st16_sel_1105.png', 0.53, 1.4, 3.6, 2.25, [[...PN.row(2), RED], [...PN.seq(2), ORANGE], [...PN.done, BLUE]]);
  img(s, 'crop_layout_st16_1105.png', 4.25, 1.4, 2.4, 1.9, [[...LY.t113, RED]]);
  img(s, 'ask_complete_st16_0_1105.png', 0.53, 3.95, 2.95, 1.15);
  img(s, 'ask_complete_st16_1_1105.png', 3.6, 3.95, 3.05, 1.55);
  note(s, 0.53, 5.2, 6.12, 1.8, '[완료처리] 의 경로 탐색이 보는 범위', [
    '단계표(경로)에 적힌 도착 칸들만 C/V 데이터에서 찾는다. 이 작업번호가 붙은 칸 → 화물만 있고 번호가 없는 칸 순으로, 뒤 단계를 먼저 본다. 다른 작업번호가 붙은 칸은 제외.',
    '그래서 3·4 트랙 어느 곳에 화물이 있어도 그것이 이 작업의 경로 위라면 찾아서 "대신 PORT:xx 에 … 기록할까요?" 로 묻는다. 경로 밖 트랙은 보지 않는다 - 그때는 3-2 방법 2(잘라내기/붙여넣기).'], 9.5);
  caption(s, '작업정보 판넬 - 0276 이 [16] 그대로, 단계표 통로 C/V 줄 진행중 / 창고 그림 - 통로 RGV 측 칸(113)에 테두리만', 0.53, 1.1, 6.1);
  caption(s, '[완료처리] 확인창 [16 → 15] → 이어서 "PORT:13 에 … 작업번호를 기록할까요?"', 0.53, 3.72, 6.1);
  rhs(s,
    [B('출고 작업이 [16] 통로 CV 구동중에 머문다. 통로 끝(RGV 측) 트랙에 화물은 왔는데 번호가 없어(테두리만) RGV 가 가지 않는다.', { color: RED })],
    [B('통로 벨트가 옮기는 사이 트래킹 번호가 소실되었다(3-2 C/V 데이터 불일치). 스케줄러는 번호가 맞는 화물에만 RGV 를 지시한다.')],
    [N('작업정보 판넬에서 그 작업을 고르고 단계표의 통로 C/V 줄을 클릭한다.', { color: BLUE }),
     N('[완료처리] → [16 → 15] [예] → "작업번호를 기록할까요?" [예]. 그 칸에 번호가 찍히고 RGV 지시가 나간다.', { color: GREEN }),
     N('화물이 경로의 다른 칸에 있어도(3·4 트랙 어느 쪽이든) 경로 위면 찾아서 기록을 제안한다(아래 상자). 경로 밖이면 C/V 상태창 [잘라내기]/[붙여넣기] 로 옮긴다(3-2 방법 2).')]);
}
// 5-5 데이터 잔존 → [데이터 삭제]
{
  const s = body('5', 'Troubleshooting - RGV', 'RGV 에 지난 데이터가 남아 새 지시를 안 받음 - [데이터 삭제]');
  img(s, 'dlg_17130801_rtvsusp_1105.png', 0.53, 1.4, 3.3, 3.4, [[...RTV.hmidel, BLUE], [...RTV.fork, RED], [...RTV.sta, ORANGE]]);
  img(s, 'rtv_hmidel_ask_1104.png', 3.95, 1.4, 2.7, 1.3);
  note(s, 3.95, 2.85, 2.7, 2.0, 'S/C 와 같은 동작', [
    '[예] → 설비 통신이 PLC 의 RGV 삭제 비트에 1 을 쓴다 = RGV 지상반 [삭제]. 끄는 것은 PLC 다.',
    '요청은 LOG ▷ 유저사용로그에, 처리 결과는 ECS 프로그램 로그(VEH_RTV "운전 명령 HMIDEL")에 남는다.'], 9.5);
  note(s, 0.53, 4.95, 6.12, 2.0, '왼쪽 캡처에서 보는 것', [
    'Fork 칸의 출발위치 00-00-09 / 도착위치 00-00-27 = 지난 지시가 RGV 에 그대로 남아 있다(작업번호는 "-", IDLE). 이 상태에서 새 지시를 보내면 RGV 가 받지 않을 수 있다.',
    '[데이터 삭제] 뒤 Fork 칸이 00-00-00 으로 비면 끝.'], 9.5);
  caption(s, 'RTV 상태창 - 명령 그룹의 [데이터 삭제] (2026-11-04 추가) / Fork 칸에 남은 지난 지시 위치 / 확인창', 0.53, 1.1, 6.1);
  rhs(s,
    [B('RGV 가 비정상으로 멈춘 뒤 화물을 내려놓고 알람도 풀었는데 새 지시를 받지 않는다. 상태창 Fork 칸에 지난 지시의 출발·도착 위치가 남아 있다.', { color: RED }),
     B('RTV상태는 IDLE 이다.', { color: ORANGE })],
    [B('RGV 컨트롤러 안에 지난 작업 데이터가 남아 새 지시와 충돌한다.')],
    [N('RGV 위 화물을 먼저 내려놓는다.'),
     N('RTV 상태창 [데이터 삭제] → 확인창 [예]. "요청했습니다" 가 뜨면 끝.', { color: BLUE }),
     N('지상반에서 데이터가 지워졌는지 본다. 안 지워지면 종전대로 지상반 [삭제].'),
     N('작업까지 정리하려면 [완료처리] 를 이어서 쓴다(5-3 / 5-4).')]);
}

// ═════════ 6. ETC ═════════
// 6-1 통신 신호등
{
  const s = body('6', 'Troubleshooting - ETC', '통신 신호등이 빨강 - WMS1 · WMS2 · PLC · SCH');
  img(s, 'crop_ribbon_comm_1105.png', 0.53, 1.4, 2.6, 1.0, [[5, 5, 170, 75, RED]]);
  img(s, 'lamp_plc_1105.png', 3.25, 1.4, 3.4, 1.7);
  caption(s, '신호등을 누르면 뜨는 통신 점검 창(PLC) - 핑 4회 · 포트', 3.25, 3.2, 3.4);
  img(s, 'chk_TASK_LFC10_G1_ECSCOM.png', 0.53, 3.95, 3.3, 2.3);
  note(s, 3.95, 3.6, 2.7, 3.4, '신호등이 보는 것', [
    'WMS1 : 상위가 우리에게 붙는 수신 소켓(8001)',
    'WMS2 : 우리가 상위로 보고하는 송신 소켓(8002)',
    'PLC : 설비 통신이 마스터 PLC 에 붙어 있는가(한 소켓이 C/V·S/C·RGV 를 대표)',
    'SCH : 스케줄러(IO TASK)가 살아 있는가',
    '위 빨강 = 끊김 / 노랑↔초록 1초 교대 = 정상(화면이 살아 있다는 뜻이기도)'], 9);
  caption(s, '리본 [통신] 신호등 (모든 탭에서 보임, 지금은 정상 교대 중) / 서버 ALL_TASK 화면(HOST TASK)', 0.53, 1.1, 6.1);
  rhs(s,
    [B('리본 [통신] 신호등 중 하나 이상이 위(빨강)에 멈춰 있다. PLC 빨강이면 설비 색이 갱신되지 않고, SCH 빨강이면 작업이 전혀 진행되지 않는다.', { color: RED })],
    [B('장비 전원·네트워크 끊김, 상대 프로그램(WMS · PLC)이 안 떠 있음, 우리 서버 TASK 가 죽음, DB 하트비트 끊김.')],
    [N('신호등을 누른다 - 상대에게 ping 4회를 보내는 창이 뜨고 결과가 요약된다.', { color: BLUE }),
     { t: '핑 응답 없음 → 장비 전원·케이블·IP (네트워크 담당)', bullet: true, indent: 1 },
     { t: '핑 응답 있음 / 포트 닫힘 → 상대 프로그램이 안 떠 있다 (WMS·PLC 담당)', bullet: true, indent: 1 },
     { t: '핑·포트 정상인데 빨강 → DB 하트비트만 끊김 → 그 TASK 재기동 (운영자)', bullet: true, indent: 1 },
     { t: '우리가 여는 포트(8001) 닫힘 → HOST TASK 기동 (운영자)', bullet: true, indent: 1 },
     N('모든 신호등이 빨강이고 값이 안 바뀌면 DB(SQL 서비스) 쪽 - 운영자(01 운영자 매뉴얼 9장).')]);
}
// 6-2 알람 경고창 / 설비 에러 이력
{
  const s = body('6', 'Troubleshooting - ETC', '설비 알람 - 알람 경고창 · 상태창 에러 · 설비에러이력');
  img(s, 'dlg_17120901_scerr_1105.png', 0.53, 1.4, 2.5, 3.0, [[...SC.err, RED]]);
  img(s, 'dlg_17110124_cverr_1105.png', 3.15, 1.4, 2.5, 2.7, [[...CV.err, RED]]);
  img(s, 'alarm_scerr_1105.png', 0.53, 4.55, 2.5, 1.45, [[...AL.rows, ORANGE]]);
  img(s, 'log_eqp_his_1105.png', 3.15, 4.2, 3.5, 2.4, [[25, 283, 1055, 50, ORANGE]]);
  note(s, 0.53, 6.05, 2.5, 0.95, '알람 경고창', ['설비 에러가 나면 오른쪽 아래에 스스로 뜬다. 해제되면 빠진다. [STOP] 은 소리만 끈다. 오래 멈춘 화물(체류)은 알람이 아니라 작업정보 판넬에서 본다.'], 8);
  caption(s, 'S/C · C/V 상태창의 에러 칸 ([코드] 문구) / 알람 경고창 목록 / LOG ▷ 설비에러이력 (EQP_ERR_HIS, 30일) - 같은 두 건', 0.53, 1.1, 6.1);
  rhs(s,
    [B('화면 오른쪽 아래에 알람 경고창이 뜨고 소리가 난다. 설비 칸·트랙이 에러색이 된다.', { color: ORANGE }),
     B('상태창 에러 칸에 [코드] 문구(PLC 알람 리스트 기준, 크레인은 SC_LGLS)가 보인다.', { color: RED })],
    [B('설비(PLC) 가 올린 알람. C/V 는 모터 과부하 하나, S/C · RGV 는 알람 리스트의 코드.')],
    [N('경고창의 설비와 코드를 보고 조작반·지상반에서 원인을 없앤 뒤 알람을 해제한다. 목록에서 빠지면 된다.'),
     N('[STOP] 은 소리만 끈다 - 알람이 풀린 것이 아니다.'),
     N('언제 어떤 알람이 났는지는 LOG ▷ 설비에러이력(30일) 에서 설비·코드·시간으로 조회한다. 잦은 알람 설비는 운영자에게.', { color: ORANGE }),
     N('알람이 풀린 뒤에도 작업이 안 흐르면 4-5 / 5-3 ([지시 재전송] · [완료처리]).')]);
}
// 6-3 [CV 지우기]
{
  const s = body('6', 'Troubleshooting - ETC', '모든 C/V 트래킹을 한 번에 지운다 - MANUAL ▷ [CV 지우기]');
  img(s, 'crop_ribbon_manual.png', 0.53, 1.4, 6.1, 1.3, [[55, 25, 55, 75, BLUE]]);
  img(s, 'cvclr_ask_1104.png', 0.53, 2.95, 3.2, 1.45);
  img(s, 'cvtask_main.png', 0.53, 4.55, 6.1, 2.45, [[690, 460, 490, 70, GREEN]]);
  note(s, 3.85, 2.95, 2.8, 1.45, '왜 ECS 가 직접 못 지우나', ['C/V 트래킹(R 영역)은 문자(ASCII)다. ECS 가 "0" 을 써도 0x30 이 남아 PLC 는 "값 있음" 으로 본다. 그래서 PLC 가 지우도록 비트(M) 하나에 1 을 보낸다.'], 9);
  caption(s, '리본 MANUAL ▷ 수동조작 ▷ [CV 지우기] (2026-11-04 추가) / 확인창 / 서버 설비 통신(EQP) 로그', 0.53, 1.1, 6.1);
  rhs(s,
    [B('여러 트랙에 지난 작업번호가 흩어져 남아(3-2 가 여럿) 하나씩 지우기 어렵다. 설비를 비우고 처음부터 정리하려 한다.')],
    [B('수동 운전 · 시운전 · 비상 정지 뒤 PLC R 영역에 작업번호 찌꺼기가 남는다. 트랙마다 상태창 [삭제] 를 누르는 대신 PLC 가 한 번에 지운다.')],
    [N('설비 위에 반송 중인 화물이 없는지 확인한다. 있으면 그 화물의 번호도 같이 지워진다.'),
     N('MANUAL ▷ 수동조작 ▷ [CV 지우기] → 확인창 [예].', { color: BLUE }),
     N('"요청했습니다" 가 뜨면 설비 통신이 PLC 의 CV 비트에 1 을 쓴다(서버 로그에 기록). 트랙 글자가 모두 사라졌는지 본다.', { color: GREEN }),
     N('작업정보에 남은 작업은 [완료처리] 또는 작업정보 창 [삭제] 로 정리한다.'),
     { t: '비트 주소가 WCS_DB.INI [HMI_DELETE] 에 없으면 이력만 남고 PLC 에는 쓰지 않는다(운영자가 기입).', color: GRAY }]);
}
// 6-4 화면 느림 · 안 바뀜
{
  const s = body('6', 'Troubleshooting - ETC', '운전 화면이 느리다 · 값이 안 바뀐다 · 아이콘이 없다');
  img(s, 'crop_ribbon_ecs_1105.png', 0.53, 1.4, 6.1, 1.1, [[455, 22, 190, 85, RED]]);
  img(s, 'main_run_1105.png', 0.53, 2.7, 6.1, 3.3, [[0, 0, 1936, 40, ORANGE]]);
  caption(s, '리본 [통신] 신호등 교대 = 화면이 살아 있다 / 제목줄 = 빌드 시각 · 붙은 DB · 실행 경로', 0.53, 1.1, 6.1);
  note(s, 0.53, 6.1, 6.1, 0.9, '제목줄로 확인할 것', ['Ecs V1.0 [Build:날짜 시각] [DB:LGLS_MCS_IO@서버,포트] [경로] - 현장에 올린 판이 맞는지, DB 가 서버를 가리키는지 여기서 본다.'], 9);
  rhs(s,
    [B('리본 신호등이 교대하지 않고 설비 색·작업 목록이 바뀌지 않는다. 또는 화면 반응이 느리다.', { color: RED }),
     B('아이콘 없이 뜨거나 설치 때 "액세스 거부" 가 난다.')],
    [B('운전 화면 프로그램이 멈춤, DB 연결 끊김, 다른 PC 의 DB 를 가리킴(제목줄). 설비 색은 설비 통신 한 바퀴(약 1초) + 화면 수집 주기(200ms) 만큼 늦는 것이 정상.', { color: ORANGE }),
     B('아이콘 없음 = 재시작 관리자로 떠서 작업 폴더가 바뀐 것(10-15 판에서 보정). 설치 거부 = 폰트 파일 잠금.')],
    [N('운전 화면만 닫고 다시 띄운다(런처 Ecs.exe). 창고는 서버 TASK 가 돌리므로 그대로 돈다.'),
     N('제목줄의 DB 가 서버가 아니면 설정이 잘못된 것 - 운영자에게.', { color: ORANGE }),
     N('2~3초 이상 늦거나 모든 신호등이 빨강이면 6-1 / 운영자(SQL 서비스 · 디스크).'),
     N('아이콘·설치 문제는 01 운영자 매뉴얼 2.3 (작업 디렉터리 · FR_PRIVATE 폰트).')]);
}

// ───────── 저장 ─────────
pres.writeFile({ fileName: OUT }).then(f => console.log('saved', f, 'slides', pageNo));
