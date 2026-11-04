// -*- coding: utf-8 -*-
// 2026-11-04  ECS(WCS) Troubleshooting 자료 (PPTX)
//   형식 : 타사 현장 자료(TO40_Troubleshooting_..._WCS_20250113.pptx)와 같은 구성
//          제목 / 왼쪽 = 상황명 + 화면 캡처(강조 박스) / 오른쪽 = 1.현상 · 2.원인 · 3.조치
//   실행 : node gen_troubleshooting_pptx.js   (출력 docs/산출물_20261110/05_ECS_Troubleshooting.pptx)
const pptxgen = require('pptxgenjs');
const fs = require('fs');
const path = require('path');

const HERE = __dirname;
const BASE = path.dirname(HERE);                       // docs/산출물_20261110
const SHOTS = path.join(BASE, 'shots');
const OUT = path.join(BASE, '05_ECS_Troubleshooting.pptx');

const FONT = '맑은 고딕';
const BLUE = '1F5FA8', NAVY = '1B2A49', GREEN_BG = 'E2EFDA', GRAY = '666666', RED = 'C00000', ORANGE = 'ED7D31', PURPLE = '7030A0', LINE = '2E75B6';
const DATE = '2026.11';
const SITE = 'LG화학 1동 자동창고 ECS(WCS)';

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

// 그림을 (x,y,maxW,maxH) 안에 비율 유지로 넣고, 픽셀 좌표 강조 박스를 얹는다. 실제 놓인 크기를 돌려준다.
function img(slide, name, x, y, maxW, maxH, boxes, opts) {
  const p = shot(name); const s = pngSize(p);
  if (y < 1.45) { maxH -= (1.48 - y); y = 1.48; }      // 소제목·캡션 줄 아래부터
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
  if (y < 1.3) y = 1.22;
  slide.addText(text, { x, y, w, h: 0.24, fontFace: FONT, fontSize: 9.5, color: color || BLUE, isTextBox: true, margin: 0, bold: true, valign: 'middle' });
}
function header(slide, no, title) {
  slide.addText((no ? no + '. ' : '') + title, { x: 0.53, y: 0.12, w: 12.3, h: 0.51, fontFace: FONT, fontSize: 24, bold: true, color: BLUE, isTextBox: true, margin: 0, valign: 'middle' });
  slide.addShape(pres.ShapeType.line, { x: 0.53, y: 0.78, w: 12.3, h: 0, line: { color: LINE, width: 1.5 } });
}
function footer(slide, n) {
  slide.addText(SITE + '  ·  Troubleshooting', { x: 0.53, y: 7.12, w: 8, h: 0.25, fontFace: FONT, fontSize: 9, color: GRAY, isTextBox: true, margin: 0 });
  slide.addText(String(n), { x: 12.2, y: 7.12, w: 0.6, h: 0.25, fontFace: FONT, fontSize: 9, color: GRAY, isTextBox: true, margin: 0, align: 'right' });
}
// 오른쪽 3단(현상/원인/조치). 각 항목은 문자열 배열(줄) 또는 {t, color, bold, indent} 객체
function rhs(slide, 현상, 원인, 조치) {
  const X = 7.05, W = 5.95;
  const blocks = [['1. 현상', 0.95, 현상, 1.5], ['2. 원인', 2.85, 원인, 1.5], ['3. 조치', 4.75, 조치, 2.25]];
  blocks.forEach(([h, y, lines, bh]) => {
    slide.addText(h, { x: X, y, w: W, h: 0.32, fontFace: FONT, fontSize: 14, bold: true, color: NAVY, fill: { color: GREEN_BG }, isTextBox: true, margin: [0, 0, 0, 6], valign: 'middle' });
    const runs = [];
    lines.forEach((ln, i) => {
      const o = (typeof ln === 'string') ? { t: ln } : ln;
      const opt = { fontFace: FONT, fontSize: o.size || 11, color: o.color || '222222', bold: !!o.bold, breakLine: i < lines.length - 1, paraSpaceAfter: 2 };
      if (o.bullet) opt.bullet = o.num ? { type: 'number' } : true;
      if (o.indent) opt.indentLevel = o.indent;
      runs.push({ text: o.t, options: opt });
    });
    slide.addText(runs, { x: X, y: y + 0.36, w: W, h: bh, fontFace: FONT, isTextBox: true, margin: [2, 4, 2, 4], valign: 'top' });
  });
}
// 본문 슬라이드 공통 틀
let pageNo = 0;
function body(no, title, sub) {
  const s = pres.addSlide();
  pageNo++;
  header(s, no, title);
  s.addText([{ text: '•  ', options: { color: BLUE } }, { text: sub, options: { color: BLUE } }],
    { x: 0.53, y: 0.86, w: 6.15, h: 0.34, fontFace: FONT, fontSize: 12.5, bold: true, isTextBox: true, margin: 0, valign: 'middle' });
  s.addShape(pres.ShapeType.line, { x: 6.85, y: 0.95, w: 0, h: 6.1, line: { color: LINE, width: 0.75 } });
  footer(s, pageNo);
  return s;
}
const B = (t, o) => Object.assign({ t, bullet: true }, o || {});   // 글머리 줄
const N = (t, o) => Object.assign({ t, bullet: true, num: true }, o || {});
const H = (t, color) => ({ t, bold: true, color: color || BLUE });   // 강조 줄

// ───────── 표지 ─────────
{
  const s = pres.addSlide(); pageNo++;
  s.background = { color: 'FFFFFF' };
  s.addShape(pres.ShapeType.rect, { x: 0, y: 5.9, w: 13.33, h: 1.6, fill: { color: BLUE }, line: { color: BLUE } });
  s.addText('Troubleshooting', { x: 0.7, y: 0.9, w: 11, h: 1.2, fontFace: FONT, fontSize: 48, color: BLUE, isTextBox: true, margin: 0 });
  s.addText(SITE, { x: 0.7, y: 2.2, w: 11, h: 0.6, fontFace: FONT, fontSize: 24, color: NAVY, isTextBox: true, margin: 0 });
  s.addText('운전 화면(Ecs)에서 상황별로 무엇을 보고 무엇을 누르는가', { x: 0.7, y: 2.85, w: 11, h: 0.5, fontFace: FONT, fontSize: 16, color: GRAY, isTextBox: true, margin: 0 });
  s.addText([{ text: DATE, options: { breakLine: true } }, { text: 'ECS(WCS) 개발 · 운영 담당' }],
    { x: 7.3, y: 6.2, w: 5.5, h: 1.0, fontFace: FONT, fontSize: 16, color: 'FFFFFF', isTextBox: true, margin: 0, align: 'right', valign: 'middle' });
  s.addText('본 문서는 현장 운영·인수인계용 자료로, 승인 없는 외부 배포·복사를 금합니다.', { x: 0.7, y: 6.95, w: 8, h: 0.4, fontFace: FONT, fontSize: 9, color: 'DDE6F5', isTextBox: true, margin: 0 });
}

// ───────── 목차 ─────────
{
  const s = pres.addSlide(); pageNo++;
  header(s, '', '목차'.replace(/^/, '')); // 번호 없이
  const items = ['1. 개정 이력', '2. 전체 레이아웃 - 운전 화면 읽는 법', '3. Troubleshooting - Conveyor (C/V)', '4. Troubleshooting - Stacker Crane (S/C)', '5. Troubleshooting - RGV (RTV)', '6. Troubleshooting - ETC (통신 · 알람 · 상위 · 화면)'];
  s.addText(items.map((t, i) => ({ text: t, options: { breakLine: i < items.length - 1, paraSpaceAfter: 10 } })),
    { x: 1.2, y: 1.3, w: 7, h: 4.5, fontFace: FONT, fontSize: 20, color: NAVY, isTextBox: true, margin: 0, valign: 'top' });
  s.addText([
    { text: '이 자료의 읽는 법', options: { bold: true, color: BLUE, breakLine: true, paraSpaceAfter: 6 } },
    { text: '각 장면은 왼쪽에 운전 화면 캡처(빨강·주황 테두리 = 봐야 할 곳 / 눌러야 할 곳), 오른쪽에 1.현상 2.원인 3.조치 순으로 적었다.', options: { breakLine: true, paraSpaceAfter: 6 } },
    { text: '상세 절차는 02 ECS 사용자 매뉴얼(5.7 완료처리, 6장 이상 대처), 서버 쪽 처치는 01 운영자 매뉴얼 9장을 따른다.', options: { breakLine: true, paraSpaceAfter: 6 } },
    { text: '어떤 경우에도 WCS 는 시간 경과로 작업을 자동 완료하지 않는다. 설비 신호가 없으면 사람이 실물을 확인한 뒤 넘긴다.', options: { color: RED } },
  ], { x: 8.4, y: 1.4, w: 4.4, h: 4.2, fontFace: FONT, fontSize: 12, color: '333333', isTextBox: true, margin: 6, valign: 'top', fill: { color: 'F3F6FB' } });
  footer(s, pageNo);
}

// ───────── 1. 개정이력 ─────────
{
  const s = pres.addSlide(); pageNo++;
  header(s, '1', '개정이력');
  const hdr = ['버전', '개정일자', '개정사유', '개정내역', '작성자', '승인자'].map(t => ({ text: t, options: { bold: true, fill: { color: 'D9E2F3' }, align: 'center', color: NAVY } }));
  const rows = [hdr, ['1.0.0', '2026-11-04', '신규', '최초 작성 (2026-11-10 적용분 기준 : 작업정보 판넬 [완료처리] 경로 탐색, 상태창 [데이터 삭제], 리본 [CV 지우기] 포함)', 'ECS 개발', '']];
  for (let i = 0; i < 8; i++) rows.push(['', '', '', '', '', '']);
  s.addTable(rows, { x: 0.7, y: 1.0, w: 11.9, colW: [1.1, 1.6, 1.2, 5.6, 1.4, 1.0], fontFace: FONT, fontSize: 11, border: { type: 'solid', color: '8EA9C1', pt: 0.75 }, rowH: 0.38, valign: 'middle', align: 'center', objectName: '개정이력' });
  footer(s, pageNo);
}

// ───────── 2. 전체 레이아웃 ─────────
{
  const s = pres.addSlide(); pageNo++;
  header(s, '2', '전체 레이아웃 - 운전 화면(Ecs) 읽는 법');
  const r = img(s, 'main_1029.png', 0.53, 1.0, 8.6, 5.9, [
    [12, 60, 650, 80, ORANGE],        // 리본 (통신 신호등 포함)
    [12, 150, 525, 180, PURPLE],      // 범례
    [12, 340, 525, 170, PURPLE],      // 설비 반송
    [12, 520, 525, 510, BLUE],        // 작업정보 판넬
    [600, 220, 1300, 760, RED],       // 창고 그림
  ]);
  s.addText([
    { text: '화면 구역', options: { bold: true, color: BLUE, breakLine: true, paraSpaceAfter: 6 } },
    B('리본 (주황) : ECS / MANUAL / LOG 탭, 맨 오른쪽 [통신] 신호등 4개(WMS1 · WMS2 · EQP · SCH). 빨강 = 끊김, 노랑↔초록 교대 = 정상'),
    B('범례 (보라) : 색이 무슨 뜻인지. 작업 색상 · C/V 상태 · S/C·RGV 상태. 외울 필요 없이 여기를 본다'),
    B('설비 반송 (보라) : S/C 5대 · RGV 가 지금 나르는 작업'),
    B('작업정보 판넬 (파랑) : 진행 중 작업 목록 → 고르면 아래에 단계표(SEQ). 머리줄의 [완료처리] 가 이 자료의 주된 처치 수단'),
    B('창고 그림 (빨강) : 랙 10열 · S/C 5대 · RGV · C/V · 작업대(103 제품 입고대 / 104 원부자재 불출대 / 102 피킹 / 101 외부 입고). 트랙·설비를 더블클릭하면 상태창'),
    { t: ' ', size: 6 },
    { text: '색으로 먼저 읽는다', options: { bold: true, color: BLUE, breakLine: true, paraSpaceAfter: 6 } },
    B('트랙 빨강 = 에러, 회색 = 수동, 진분홍 = 입고대 · 연보라 = 출고대, 분홍 = 검색', { color: '333333' }),
    B('S/C·RGV 칸 : 정상(연회색) · 수동(회색) · 에러(빨강). 레일 : 입고금지(주황) · 출고금지(갈색) · 작업중(파랑)', { color: '333333' }),
  ].map((o, i, a) => typeof o.t === 'string' ? { text: o.t, options: { bullet: o.bullet ? true : undefined, breakLine: i < a.length - 1, paraSpaceAfter: 4, fontSize: o.size || 11, color: o.color || '222222' } } : o),
    { x: 9.3, y: 1.0, w: 3.6, h: 5.9, fontFace: FONT, fontSize: 11, color: '222222', isTextBox: true, margin: 4, valign: 'top' });
  footer(s, pageNo);
}

// ═════════ 3. Conveyor ═════════
// 3-1 C/V 에러 (모터 과부하)
{
  const s = body('3', 'Troubleshooting - Conveyor', 'C/V 에러 - 모터 과부하');
  img(s, 'cv_0_v20.png', 0.53, 1.4, 4.0, 4.2, [[27, 412, 530, 50, RED], [27, 478, 530, 110, ORANGE]]);
  img(s, 'legend_0.png', 0.53, 5.95, 6.1, 1.1, [[420, 78, 180, 150, RED]]);
  caption(s, 'C/V 상태창 (트랙 더블클릭) - 에러코드 [코드] 문구, 아래 상태 신호', 0.53, 1.1, 6.1);
  caption(s, '범례 : C/V·S/C 상태의 "에러" 색이 그 트랙에 칠해진다', 0.53, 5.7, 6.1);
  s.addText([
    { text: '알람 경고창', options: { bold: true, color: BLUE, breakLine: true, paraSpaceAfter: 4 } },
    { text: '설비 에러가 나면 화면 오른쪽 아래에 스스로 뜬다(10초마다 확인). 해제되면 목록에서 빠진다. [STOP] 은 소리만 끄고 창을 감춘다.', options: {} },
  ], { x: 4.65, y: 1.5, w: 2.1, h: 1.8, fontFace: FONT, fontSize: 9.5, color: '333333', isTextBox: true, margin: 3, fill: { color: 'F3F6FB' }, valign: 'top' });
  rhs(s,
    [B('트랙이 범례의 에러색(빨강)으로 바뀌고 화물이 다음 트랙으로 넘어가지 않는다.'),
     B('화면 오른쪽 아래에 알람 경고창이 뜬다. 트랙을 더블클릭한 C/V 상태창의 에러코드에 [코드] 문구가 보인다.'),
     B('LOG ▷ 설비에러이력(EQP_ERR_HIS) 에 같은 내용이 남는다(30일).')],
    [B('이 현장 C/V 에서 ECS 로 올라오는 에러는 모터 과부하(EOCR 트립) 하나다. 벨트 끼임·화물 걸림으로 모터가 과전류를 받으면 PLC 가 올린다.'),
     B('한 설비가 에러인 동안 스케줄러는 그 설비에만 지시를 내지 않는다. 다른 설비는 계속 돈다.')],
    [N('현장 조작반에서 물리적 원인을 없앤다(화물 정렬, 끼임 제거). 메인 판넬의 EOCR 트립을 리셋한다.'),
     N('조작반에서 알람을 해제하고 자동으로 돌린다. 설비가 하던 반송을 마치면 완료 신호가 올라온다.'),
     N('화면에서 트랙 색이 돌아오고 알람 경고창 목록에서 빠졌는지, 작업정보 판넬에서 작업이 다음 단계로 갔는지 확인한다.'),
     B('ECS 에는 반송을 취소하거나 목적지를 바꾸는 수단이 없다(설비 인터페이스에 그런 신호가 없다). 설비에서 사람이 푼다.', { color: RED })]);
}
// 3-2 화면 데이터와 현장 화물 위치 불일치
{
  const s = body('3', 'Troubleshooting - Conveyor', 'C/V 데이터 불일치 - 화면의 작업번호 위치와 실제 화물 위치가 다르다');
  img(s, 'cv_0_v20.png', 0.53, 1.4, 4.4, 4.9, [[428, 100, 125, 145, RED], [27, 100, 395, 60, ORANGE], [438, 262, 110, 22, BLUE]]);
  caption(s, 'C/V 상태창 - 명령 그룹(쓰기·삭제·복사·잘라내기·붙여넣기)으로 작업번호를 올리고 내린다', 0.53, 1.1, 6.1);
  s.addText([
    { text: '명령 그룹의 뜻', options: { bold: true, color: BLUE, breakLine: true, paraSpaceAfter: 4 } },
    { text: '쓰기 : 작업번호 칸의 번호를 이 트랙에 기록', options: { breakLine: true } },
    { text: '삭제 : 이 트랙의 번호를 지움', options: { breakLine: true } },
    { text: '복사 / 잘라내기 : 이 트랙 번호를 담아 둠(잘라내기는 지우면서)', options: { breakLine: true } },
    { text: '붙여넣기 : 담아 둔 번호를 이 트랙에 기록. 통로 트랙(103~120)에 올리면 작업상태 16, 작업대는 15', options: { breakLine: true } },
    { text: '자동조회 : 켜 두면 값이 주기적으로 다시 읽힌다', options: {} },
  ], { x: 5.05, y: 1.45, w: 1.6, h: 4.8, fontFace: FONT, fontSize: 9.5, color: '333333', isTextBox: true, margin: 3, fill: { color: 'F3F6FB' }, valign: 'top' });
  rhs(s,
    [B('화물은 다음 트랙으로 갔는데 화면의 작업번호는 앞 트랙에 남아 있거나, 번호 없는 화물(화물 감지 테두리만)이 보인다.'),
     B('그 자리에서 작업이 멈추고 뒤따르는 작업이 전부 기다린다.')],
    [B('센서 오동작, 데이터 삭제 없이 화물을 들어냄, 조작반 수동으로 화물을 밀어 보냄 등 - 실물과 트래킹이 어긋난 것.'),
     B('C/V 트래킹은 PLC R 영역의 작업번호다. ECS 는 그 값을 읽어 그리며, 사람이 상태창 명령으로 고쳐 쓸 수 있다.')],
    [H('방법 1 : 화면대로 현장을 맞춘다'), N('화면에 보이는 위치로 화물을 옮긴다. 번호가 사라진 화물은 현장에서 들어낸다.', { indent: 1 }),
     H('방법 2 : 현장대로 화면을 맞춘다'), N('조작반을 수동으로 두고, 번호가 있는 트랙 상태창 [잘라내기] → 실제 화물이 있는 트랙 상태창 [붙여넣기]. 번호가 없으면 작업번호 칸에 적고 [쓰기].', { indent: 1 }),
     N('작업정보 판넬 단계표가 실물 위치의 줄을 진행중으로 가리키는지 확인한 뒤 자동으로 돌린다.', { indent: 1 }),
     B('반송 중인 트랙에는 쓰지 않는다. 멈춘 뒤에 한다.', { color: RED })]);
}
// 3-3 작업번호 없는 화물이 H/S 에 있다
{
  const s = body('3', 'Troubleshooting - Conveyor', '작업번호 없는 화물이 H/S(통로) 에 놓여 있다 - [H/S 배출]');
  img(s, 'cv_0_v20.png', 0.53, 1.4, 3.6, 4.0, [[428, 295, 125, 48, RED], [27, 100, 395, 60, ORANGE], [27, 478, 530, 110, BLUE]]);
  img(s, 'popup_route_found.png', 4.25, 1.4, 2.4, 1.4);
  img(s, 'panel_seq_0758.png', 4.25, 2.95, 2.4, 2.4, [[530, 335, 85, 22, RED]]);
  caption(s, 'C/V 상태창 - 예외 처리 [H/S 배출]', 0.53, 1.1, 3.6);
  caption(s, '원래 작업이 있으면 [완료처리] 가 찾아 준다', 4.25, 1.1, 2.4);
  rhs(s,
    [B('통로 H/S(103~120) 트랙에 화물 감지 테두리만 있고 작업번호(글자)가 없다. 크레인·RGV 가 그 화물에는 손대지 않고, 뒤 작업이 선다.'),
     B('C/V 상태창 : 작업번호 0000, 작업상태 N, 팔레트 감지 ON.')],
    [B('크레인을 수동으로 돌려 화물을 H/S 에 내려놓았거나, 트래킹 번호가 소실되었다(3-2). WCS 가 그 화물의 주인을 모른다.')],
    [H('경우 1 : 원래 작업이 아직 작업정보 목록에 있다'), N('판넬에서 그 작업을 고르고 [완료처리] → "대신 PORT:xx 에 화물이 감지됩니다 … 기록할까요?" [예]. 번호가 기록되면 스케줄러가 이어서 반송한다(4-6 참조).', { indent: 1 }),
     H('경우 2 : 작업이 없는 화물이다'), N('그 트랙 상태창의 예외 처리 [H/S 배출] 을 누른다. 배출용 작업(반자동)이 생겨 출고대까지 나간다. 상위에는 보고하지 않고 끝나면 지워진다.', { indent: 1 }),
     B('그 자리에 두면 그 호기의 후속 작업이 전부 대기하므로 즉시 처리한다.', { color: RED })]);
}
// 3-4 출고대 도착했는데 작업 15 로 남음
{
  const s = body('3', 'Troubleshooting - Conveyor', '출고대에 화물이 왔는데 작업이 [15] 로 남는다 - 완료 보고 안 나감');
  img(s, 'panel_seq_0758.png', 0.53, 1.4, 3.6, 3.3, [[530, 335, 85, 22, RED], [10, 360, 410, 110, ORANGE]]);
  img(s, 'popup_route_none.png', 4.25, 1.4, 2.4, 1.5);
  img(s, 'crop_cv11_dir.png', 4.25, 3.1, 2.4, 0.5);
  img(s, 'crop_layout_stn.png', 0.53, 4.85, 3.0, 2.2, [[190, 50, 100, 48, RED], [190, 180, 100, 85, RED]]);
  caption(s, '작업정보 판넬 - 단계표 마지막 줄(출고대)을 골라 [완료처리] / 안내창 / 출고대(104·102·101)', 0.53, 1.1, 6.1);
  rhs(s,
    [B('출고 화물이 출고대(104 원부자재 불출대 · 102 피킹 · 101) 에 실제로 도착했는데 작업정보의 상태가 [15] CV 구동중 그대로이고 목록에서 사라지지 않는다.'),
     B('상위(WMS)에 완료 보고가 나가지 않아 다음 지시도 오지 않는다.')],
    [B('출고대의 도착 신호는 3초 펄스라 유실될 수 있고, 트랙 번호가 지워진 채 도착하면 "누가 왔는지" 를 모른다.'),
     B('2026-09-30 부터 번호가 붙은 실도착은 자동 완료한다(OUT_DONE_ON_ARRIVAL=1). 남는 경우는 번호가 없을 때다.')],
    [N('작업정보 판넬에서 그 작업을 고른다. 단계표에서 마지막 줄(출고대 C/V)을 클릭한다.'),
     N('[완료처리] → 확인창 [15 → 19] [예]. "작업번호를 기록할까요?" 가 뜨면 [예].'),
     N('상위로 완료 보고가 나가고 목록에서 사라진다. 출고대에서 화물을 반출한다.'),
     B('화물이 아직 출고대에 없으면 "어느 칸에도 없다" 안내만 뜨고 아무것도 바꾸지 않는다. 실물을 먼저 확인한다.', { color: RED })]);
}
// 3-5 S/C#1 통로 방향
{
  const s = body('3', 'Troubleshooting - Conveyor', 'S/C#1 통로(C/V#2) 방향 불일치 - 입고·출고가 서로 기다린다');
  img(s, 'crop_cv2_dir.png', 0.53, 1.4, 6.1, 1.9, [[350, 10, 150, 90, RED], [130, 30, 110, 60, ORANGE], [130, 75, 110, 60, ORANGE]]);
  img(s, 'crop_cv11_dir.png', 0.53, 3.5, 6.1, 1.0, [[445, 60, 110, 45, RED]]);
  img(s, 'crop_layout_stn.png', 0.53, 4.7, 3.0, 2.3);
  caption(s, '메인 화면 - C/V#2 방향 칸("출고 모드"/"입고 모드")과 103·104 / 아래 C/V#11(101) 방향 칸', 0.53, 1.1, 6.1);
  s.addText([
    { text: '103·104 는 C/V#2 한 라인을 함께 쓴다', options: { bold: true, color: BLUE, breakLine: true, paraSpaceAfter: 4 } },
    { text: '103 (TR#24) 제품 입고대 = 입고 방향, 104 (TR#26) 원부자재 불출대 = 출고 방향. 101 (C/V#11, TR#22) 도 방향 전환형 입출고대다.', options: { breakLine: true } },
    { text: '방향 칸을 더블클릭하면 바꿀 PLC 주소를 보여 주는 확인창이 뜨고 [예] 로 즉시 전환된다.', options: {} },
  ], { x: 3.65, y: 4.75, w: 3.0, h: 2.2, fontFace: FONT, fontSize: 9.5, color: '333333', isTextBox: true, margin: 3, fill: { color: 'F3F6FB' }, valign: 'top' });
  rhs(s,
    [B('입고 화물이 103 에서 멈춘 채 S/C#1 이 안 받거나, 출고 화물이 104 쪽으로 못 나오고 통로에서 기다린다.'),
     B('방향 칸이 지금 하려는 작업과 반대("출고 모드" 인데 입고를 보내는 중 등)로 보인다.')],
    [B('S/C#1 은 입고·출고가 C/V#2 한 라인을 함께 쓴다. 스케줄러가 방향을 바꿔 주지만 전환 신호가 어긋나거나, 양쪽 작업이 서로 사용권을 기다리면(교착) 멈춘다.'),
     B('그 라인이 막히면 1호기 작업이 같이 선다.')],
    [N('메인 화면에서 C/V#2 의 방향 칸을 더블클릭한다. 확인창에 바꿀 PLC 주소가 보이면 [예].'),
     N('수동 전환은 즉시 적용되고, 그 뒤 120초 동안은 스케줄러가 되돌리지 않는다. 그 사이 멈춘 작업이 흘러가는지 본다.'),
     N('반복되면 운영자에게 : IO_TASK 로그의 "사용권" 줄과 WCS_DB.INI CV2_DIR_MODE 를 본다(01 운영자 매뉴얼 9장).'),
     B('통로에 화물이 실린 채 방향을 바꾸면 화물이 되끌려 갈 수 있다. 벨트가 멈춘 것을 보고 바꾼다.', { color: RED })]);
}

// ═════════ 4. Stacker Crane ═════════
// 4-1 입고 H/S 준비 안 됨
{
  const s = body('4', 'Troubleshooting - Stacker Crane', 'S/C 입고 이상 - 입고 H/S 가 준비되지 않음 (PLC 신호)');
  img(s, 'cv_0_v20.png', 0.53, 1.4, 3.6, 4.0, [[27, 478, 530, 110, RED], [27, 100, 395, 60, ORANGE], [27, 165, 395, 240, BLUE]]);
  img(s, 'sc_dlg_hmidel_1104.png', 4.25, 1.4, 2.4, 2.9, [[27, 265, 345, 130, ORANGE]]);
  img(s, 'crop_layout_stn.png', 4.25, 4.45, 2.4, 1.9, [[190, 0, 100, 100, RED]]);
  caption(s, '통로 H/S 트랙의 C/V 상태창 - 아래 "상태" 신호(입고 HS·자동모드·팔레트 감지) / S/C 상태창', 0.53, 1.1, 6.1);
  rhs(s,
    [B('S/C 는 IDLE 이고 에러도 없고 입고 정지도 아닌데, 입고 H/S(통로 끝 트랙)에 화물·작업번호가 있어도 움직이지 않는다.'),
     B('작업정보의 상태가 [16] 통로 CV 구동중에서 머문다.')],
    [B('PLC 가 그 트랙의 입고 HS 신호(입고 가능)를 올리지 않았다 - 자동모드 OFF, 팔레트 감지 OFF, 또는 입고 HS 신호 OFF.'),
     B('스케줄러는 "상태 16 + 통로 트랙의 화물·같은 작업번호 + HS 신호" 가 다 맞아야 크레인에 지시한다.')],
    [N('그 트랙을 더블클릭해 C/V 상태창 아래 신호를 본다. 입고 HS 신호가 OFF 면 PLC 쪽이다 - 조작반을 자동으로 두고, 그래도 OFF 면 PLC 담당자에게 문의한다.'),
     N('작업상태가 [15] 인데 화물이 이미 통로 H/S 에 있으면(RGV 가 내린 뒤) 단계표의 통로 C/V 줄을 골라 [완료처리] 로 16 으로 넘긴다.'),
     N('작업번호가 그 트랙에 없으면 3-3 (번호 없는 화물) 로 간다.')]);
}
// 4-2 입고/출고 금지
{
  const s = body('4', 'Troubleshooting - Stacker Crane', 'S/C 가 자동 작업을 받지 않음 - 입고/출고 정지 · 작업금지');
  img(s, 'sc_dlg_hmidel_1104.png', 0.53, 1.4, 3.9, 4.6, [[380, 265, 140, 110, RED], [27, 265, 345, 130, ORANGE]]);
  img(s, 'legend_0.png', 0.53, 6.1, 6.1, 1.0, [[615, 78, 185, 150, RED]]);
  caption(s, 'S/C 상태창 - 입출고상태 그룹 [입고 정지] [출고 정지] [작업금지]', 0.53, 1.1, 6.1);
  caption(s, '범례 RAIL 상태 : 입고금지 · 출고금지 · 입출고금지 색이 레일에 칠해진다', 0.53, 5.85, 6.1);
  rhs(s,
    [B('그 호기의 레일이 입고금지(주황) · 출고금지(갈색) · 입출고금지(진빨강) 색이다.'),
     B('S/C 상태창 입출고상태 칸에 정지 표시가 있고, 해당 방향 작업이 지시되지 않는다.')],
    [B('사람이 상태창에서 [입고 정지] / [출고 정지] / [작업금지] 를 눌러 둔 상태다(점검 · 수동 작업 뒤 해제를 잊음).'),
     B('스케줄러는 정지된 방향의 작업을 그 호기에 내지 않는다.')],
    [N('S/C 상태창을 열어 입출고상태 칸을 본다.'),
     N('켜져 있는 [입고 정지] / [출고 정지] / [작업금지] 를 다시 눌러 해제한다. 레일 색이 돌아온다.'),
     N('누가 언제 눌렀는지는 LOG ▷ 유저사용로그(WCS_CLIENT_LOG) 에 남는다.')]);
}
// 4-3 수동 / 에러
{
  const s = body('4', 'Troubleshooting - Stacker Crane', 'S/C 가 자동 작업을 받지 않음 - 수동 모드 · 에러 (크레인 칸 색)');
  img(s, 'sc_dlg_hmidel_1104.png', 0.53, 1.4, 3.9, 4.6, [[27, 265, 345, 130, RED], [27, 565, 495, 42, RED], [380, 103, 140, 95, ORANGE]]);
  img(s, 'legend_0.png', 0.53, 6.1, 6.1, 1.0, [[420, 78, 180, 150, RED]]);
  caption(s, 'S/C 상태창 - 동작상태(IDLE/RUN/DOWN) · 에러 코드 / 명령 그룹', 0.53, 1.1, 6.1);
  caption(s, '범례 : S/C·RGV 상태 정상 · 수동 · 에러', 0.53, 5.85, 6.1);
  rhs(s,
    [B('메인 화면의 크레인 칸이 수동(회색) 또는 에러(빨강)색이고 지시를 받지 않는다.'),
     B('상태창 동작상태가 DOWN 이거나 에러 칸에 [코드] 문구가 있다. 에러는 알람 경고창에도 뜬다.')],
    [B('지상반에서 수동으로 돌려 놓았거나(자동 아님), 설비 알람이 걸려 있다.'),
     B('3·5호기가 실제 알람 없이 에러색이면 PLC 상태 워드의 하트비트 값을 에러로 오독한 것(운영자 설정 SC_ERR_CODE_BLOCK).')],
    [N('지상반에서 알람 원인을 없애고 알람을 해제한 뒤 자동으로 전환한다. 화면 색이 정상(연회색)으로 돌아온다.'),
     N('크레인이 지시를 받은 채 멈췄던 것이면 그 뒤 스스로 이어서 움직인다. 지시가 날아갔으면 상태창 [지시 재전송].'),
     N('유령 에러(3·5호기)면 운영자에게 - WCS_DB.INI SC_ERR_CODE_BLOCK=3 (01 운영자 매뉴얼 5.2).'),
     B('포크 위에 화물을 든 채 멈췄으면 4-5 / 4-6 으로 간다.', { color: RED })]);
}
// 4-4 입고 작업이 없음
{
  const s = body('4', 'Troubleshooting - Stacker Crane', 'S/C 입고 이상 - 입고 H/S 의 화물에 작업 정보가 없음');
  img(s, 'cv_0_v20.png', 0.53, 1.4, 3.3, 3.6, [[27, 100, 395, 60, RED], [428, 100, 125, 145, ORANGE], [428, 295, 125, 48, BLUE]]);
  img(s, 'joblist_0.png', 3.95, 1.4, 2.7, 1.75, [[975, 278, 100, 22, ORANGE]]);
  img(s, 'manual_job_0.png', 3.95, 3.3, 2.7, 1.0);
  img(s, 'popup_route_found.png', 3.95, 4.45, 2.7, 1.4);
  caption(s, 'C/V 상태창 - 작업번호 0000 · 작업상태 N / 작업정보 [복사] · MANUAL ▷ 작업 · [완료처리] 안내', 0.53, 1.1, 6.1);
  rhs(s,
    [B('S/C 는 받을 수 있는 상태인데 입고 H/S 의 화물에 작업번호가 없어(화물 감지만) 크레인이 입고하지 않는다.')],
    [B('작업자 실수 · 수동 하역 · 트래킹 소실로 H/S 에 도착한 화물에 작업정보가 없으면 스케줄러는 입고를 지시하지 않는다.')],
    [H('방법 1 : 원래 입고 작업이 목록에 있다'), N('판넬에서 그 작업을 고르고 [완료처리] → "대신 PORT:xx 에 … 기록할까요?" [예] (4-6).', { indent: 1 }),
     H('방법 2 : 작업이 없다 - 입고 위치를 안다'), N('MANUAL ▷ 작업 에서 반자동 입고(출발 = 그 H/S 트랙, 도착 위치 00-000-00)를 만든다.', { indent: 1 }),
     N('ECS ▷ 뷰 ▷ 작업정보 에서 그 작업 [복사] → H/S 트랙 C/V 상태창 [붙여넣기].', { indent: 1 }),
     H('방법 3 : 어디로 갈지 모른다 - C/V 상태창 [H/S 배출] 로 출고대로 내보낸다(3-3)')]);
}
// 4-5 하역 Ack 유실 (25)
{
  const s = body('4', 'Troubleshooting - Stacker Crane', 'S/C 작업이 [25] 에서 오래 머문다 - 하역 완료 신호(Ack) 유실');
  img(s, 'cap_s1_before.png', 0.53, 1.4, 6.1, 3.2, [[10, 110, 612, 20, ORANGE], [1092, 190, 55, 32, RED], [530, 385, 92, 20, RED], [10, 432, 410, 20, BLUE]]);
  img(s, 'sc_dlg_hmidel_1104.png', 0.53, 4.75, 2.0, 2.3, [[380, 103, 140, 95, RED]]);
  s.addText([
    { text: '상황 ① (2026-10-14 시험)', options: { bold: true, color: BLUE, breakLine: true, paraSpaceAfter: 4 } },
    { text: '출고 0533 이 [25] S/C 구동중인데 크레인은 IDLE. 1호기 칸에 0533 과 파란색이 남아 있고 통로 H/S(PORT:4)에는 화물 감지만 있다.', options: { breakLine: true, paraSpaceAfter: 4 } },
    { text: '단계표 진행중 줄 = 0001 S/C 1. 이 줄을 둔 채 [완료처리] → [25 → 29].', options: {} },
  ], { x: 2.65, y: 4.8, w: 4.0, h: 2.2, fontFace: FONT, fontSize: 9.5, color: '333333', isTextBox: true, margin: 3, fill: { color: 'F3F6FB' }, valign: 'top' });
  caption(s, '작업정보 판넬 + 메인 화면 - 크레인 칸에 번호·색이 남고 작업은 25 / S/C 상태창 명령', 0.53, 1.1, 6.1);
  rhs(s,
    [B('작업이 [25] S/C 구동중(출고) 또는 입고의 S/C 단계에 오래 머문다. 크레인은 IDLE 로 돌아왔는데 메인 화면 크레인 칸에 작업번호·색이 남아 있다.'),
     B('통로 H/S 에는 화물 감지만 있고 작업번호가 없다.')],
    [B('크레인은 하역을 끝냈는데 하역 완료(Ack) 신호가 ECS 에 오지 않았다. ECS 는 설비 신호로만 완료를 판정하므로 그 상태에 선다.'),
     B('크레인이 지시 자체를 못 받았을 수도 있다(지시는 상태창 OD 로 확인).')],
    [N('설비가 지시를 못 받은 것 같으면 S/C 상태창 [지시 재전송] 을 먼저 누른다.'),
     N('이미 일을 끝낸 것이면 작업정보 판넬에서 그 작업을 고르고 [완료처리] → [25 → 29] [예].'),
     N('크레인이 아직 RUN 이면 "화물을 이미 내려놓은 것이 확실합니까?" 를 한 번 더 묻는다. 실물을 눈으로 확인한 뒤 [예].'),
     N('크레인 지시가 풀리고, H/S 화물에 번호가 찍혀 통로 벨트가 이어서 옮긴다.'),
     B('[완료처리] 는 DB 만 바꾼다. 포크 위 화물은 센서에 잡히지 않으니 먼저 내려놓는다.', { color: RED })]);
}
// 4-6 수동 조작으로 H/S 에 내려놓음 (현장 2161)
{
  const s = body('4', 'Troubleshooting - Stacker Crane', 'S/C 수동 조작 뒤 작업 단계와 실물 위치가 다르다 (현장 2161)');
  img(s, 'panel_seq_0758.png', 0.53, 1.4, 3.6, 3.3, [[10, 385, 410, 18, BLUE], [10, 420, 410, 18, RED], [530, 335, 85, 22, RED]]);
  img(s, 'popup_route_found.png', 4.25, 1.4, 2.4, 1.4);
  img(s, 'popup_route_none.png', 4.25, 2.95, 2.4, 1.5);
  img(s, 'sc_dlg_hmidel_1104.png', 0.53, 4.85, 1.9, 2.2, [[380, 103, 140, 95, ORANGE]]);
  s.addText([
    { text: '현장 2161 (2026-10-27)', options: { bold: true, color: BLUE, breakLine: true, paraSpaceAfter: 4 } },
    { text: '급한 출고 때문에 크레인을 수동으로 돌려 입고 화물을 H/S 에 두고 H/S 데이터를 지웠다. 작업은 [15] 첫 줄 진행중, 화물은 두 단계 뒤 PORT:12.', options: { breakLine: true, paraSpaceAfter: 4 } },
    { text: '종전 판은 "PORT:23 에 화물이 없다" 로 끝났다. 10-28 판부터 경로의 다른 칸을 찾아 넘기자고 묻는다.', options: {} },
  ], { x: 2.55, y: 4.9, w: 4.1, h: 2.1, fontFace: FONT, fontSize: 9.5, color: '333333', isTextBox: true, margin: 3, fill: { color: 'F3F6FB' }, valign: 'top' });
  caption(s, '작업정보 판넬 - 진행중 줄(파랑)과 실물이 있는 줄(빨강) / 경로 탐색 안내창 2종', 0.53, 1.1, 6.1);
  rhs(s,
    [B('작업정보의 단계표는 첫 줄(입고대 C/V)이 진행중인데, 화물은 이미 통로 H/S(두 단계 뒤)에 내려와 있다. 크레인에는 지시가 없다.'),
     B('그 자리에서 [완료처리] 를 누르면 "대신 PORT:xx (단계 000n …) 에 화물이 감지됩니다 … 기록할까요?" 가 뜬다.')],
    [B('작업 중 에러 또는 급한 작업으로 크레인을 수동 조작해 화물을 옮기고 설비 데이터를 지웠다. ECS 는 단계가 바뀐 것을 모른다.')],
    [N('작업정보 판넬에서 그 작업을 고르고 [완료처리]. 찾은 칸을 알려 주면 [예] - 상태 이동(15→16)과 그 칸의 작업번호 기록이 한 번에 끝나고 스케줄러가 이어서 입고한다.'),
     N('"어느 칸에도 없다" 가 뜨면 - 설비에 실린 채라면 내린 뒤 그 단계 줄을 골라 다시 / 이미 랙·작업대까지 갔다면 마지막 줄을 골라 [완료처리](29).'),
     N('크레인에 지난 데이터가 남아 새 지시를 안 받으면 4-7 [데이터 삭제].'),
     B('[아니오] 는 아무것도 바꾸지 않는다. 포크 위 화물은 먼저 내린다.', { color: RED })]);
}
// 4-7 데이터 잔존 → [데이터 삭제]
{
  const s = body('4', 'Troubleshooting - Stacker Crane', 'S/C 에 지난 데이터가 남아 새 지시를 안 받음 - [데이터 삭제]');
  img(s, 'sc_dlg_hmidel_1104.png', 0.53, 1.4, 3.9, 4.6, [[380, 172, 140, 22, RED], [27, 410, 495, 130, ORANGE]]);
  img(s, 'hmidel_ask_1104.png', 4.5, 1.4, 2.15, 1.0);
  img(s, 'rtv_hmidel_ask_1104.png', 4.5, 2.6, 2.15, 1.0);
  caption(s, 'S/C 상태창 - 명령 그룹의 [데이터 삭제] (2026-11-04 추가) / 확인창 (S/C · RGV)', 0.53, 1.1, 6.1);
  s.addText([
    { text: '무엇이 일어나나', options: { bold: true, color: BLUE, breakLine: true, paraSpaceAfter: 4 } },
    { text: '[예] → 설비 통신이 PLC 의 그 호기 삭제 비트에 1 을 쓴다 = 지상반 [삭제] 와 같다. 설비가 자기 작업 데이터를 지우고 비트를 스스로 내린다.', options: { breakLine: true, paraSpaceAfter: 4 } },
    { text: '비트 주소가 WCS_DB.INI [HMI_DELETE] 에 없으면 이력만 남고 PLC 에는 쓰지 않는다(운영자가 기입).', options: {} },
  ], { x: 4.5, y: 3.7, w: 2.15, h: 3.3, fontFace: FONT, fontSize: 9.5, color: '333333', isTextBox: true, margin: 3, fill: { color: 'F3F6FB' }, valign: 'top' });
  rhs(s,
    [B('크레인이 비정상으로 멈춘 뒤 화물을 내려놓고 알람도 풀었는데, 새 작업을 지시해도 크레인이 받지 않는다.'),
     B('상태창 작업상태 칸에 지난 작업의 출발·도착 위치가 남아 있다. 지상반에는 이전 작업 데이터가 보인다.')],
    [B('설비(PLC) 안에 지난 작업 데이터가 남아 있어 새 지시와 충돌한다. 지금까지는 지상반(터치)의 [삭제] 를 눌러야 했다.')],
    [N('설비 위 화물을 먼저 내려놓는다(크레인은 출고 H/S 에).'),
     N('S/C 상태창 [데이터 삭제] → 확인창 [예]. "요청했습니다" 안내가 뜨면 끝.'),
     N('지상반에서 데이터가 지워졌는지 본다. 안 지워지면 종전대로 지상반 [삭제].'),
     N('화면의 작업 정보는 지워지지 않는다. 작업까지 정리하려면 [완료처리] 를 이어서 쓴다(4-5 / 4-6).'),
     B('버튼이 안 보이면 Ecs.ini [MENU] HMIDEL_BTN=0 이다(운영자).', { color: GRAY })]);
}

// ═════════ 5. RGV ═════════
// 5-1 출발지/도착지 준비 안 됨
{
  const s = body('5', 'Troubleshooting - RGV', 'RGV 가 움직이지 않음 - 출발지·도착지 준비 안 됨 (PLC 신호)');
  img(s, 'cv_0_v20.png', 0.53, 1.4, 3.6, 4.0, [[210, 548, 345, 28, RED], [27, 478, 530, 67, ORANGE]]);
  img(s, 'rtv_dlg_hmidel_1104.png', 4.25, 1.4, 2.4, 2.45, [[218, 265, 185, 70, ORANGE], [27, 495, 515, 45, ORANGE]]);
  img(s, 'crop_layout_stn.png', 4.25, 4.05, 2.4, 1.9, [[300, 10, 100, 110, RED]]);
  caption(s, 'C/V 상태창 - RGV 출발/도착 HS 신호 · 자동모드 · 팔레트 감지 / RTV 상태창 - IDLE · 에러 없음', 0.53, 1.1, 6.1);
  rhs(s,
    [B('RGV 는 IDLE 이고 에러·금지가 아닌데, 작업이 [30]/[31] RGV 단계(입고) 또는 출고의 RGV 단계에서 머문다.'),
     B('출발지(입고대 끝 트랙 / 통로 RGV 측 트랙)에 화물·번호가 있는데 RGV 가 가지 않는다.')],
    [B('PLC 가 출발지의 RGV 출발 HS 신호 또는 도착지의 RGV 도착 HS 신호를 올리지 않았다 - 자동모드 OFF, 팔레트 감지 상태 불일치, 도착지에 다른 화물.'),
     B('스케줄러는 출발·도착 두 트랙의 신호가 다 맞아야 RGV 에 지시한다.')],
    [N('출발지 트랙과 도착지 트랙을 각각 더블클릭해 C/V 상태창 아래 신호를 본다.'),
     N('도착지에 화물이 남아 있으면 먼저 비운다(그 화물의 작업을 3-3 / 4-4 로 처리).'),
     N('신호가 OFF 인 쪽의 조작반을 자동으로 두고, 그래도 OFF 면 PLC 담당자에게 문의한다.'),
     N('작업번호가 출발지 트랙에 없으면 3-3 (번호 없는 화물) / 5-4.')]);
}
// 5-2 RTV 금지
{
  const s = body('5', 'Troubleshooting - RGV', 'RGV 가 자동 작업을 받지 않음 - [RTV 금지] 상태 · 수동 · 에러');
  img(s, 'rtv_dlg_hmidel_1104.png', 0.53, 1.4, 3.9, 4.0, [[405, 300, 130, 28, RED], [218, 265, 185, 70, ORANGE], [27, 495, 515, 45, ORANGE]]);
  img(s, 'legend_0.png', 0.53, 5.6, 6.1, 1.0, [[420, 78, 180, 150, RED], [615, 78, 185, 150, RED]]);
  caption(s, 'RTV 상태창 - [RTV 금지] 단추 / RTV상태 · 주행위치 / 에러', 0.53, 1.1, 6.1);
  caption(s, '범례 : S/C·RGV 상태 · RAIL 상태', 0.53, 5.35, 6.1);
  rhs(s,
    [B('RGV 레일이 금지색이거나 RGV 칸이 수동(회색)·에러(빨강)이고 지시를 받지 않는다.'),
     B('RTV 상태창 RTV상태가 IDLE 이 아니거나 에러 칸에 [코드] 문구가 있다.')],
    [B('[RTV 금지] 를 눌러 둔 상태(점검 · 수동 작업 뒤 해제를 잊음), RGV 컨트롤러가 수동, 또는 알람.'),
     B('RGV 가 홈에서 전혀 안 바뀌어 보이면 화면 조회 설정 문제일 수 있다(RTV_DATA_LGLS.PLC_NO 자릿수, 운영자).')],
    [N('RTV 상태창에서 [RTV 금지] 가 켜져 있으면 다시 눌러 해제한다.'),
     N('RGV 컨트롤러(지상반)에서 알람을 풀고 자동으로 전환한다.'),
     N('지시가 날아갔으면 [지시 재전송]. 하역을 끝냈는데 보고가 없으면 5-3.'),
     N('화면만 안 바뀌면 운영자에게(01 운영자 매뉴얼 9장 "RGV 가 홈에 고착").')]);
}
// 5-3 하역 보고 유실 (35)
{
  const s = body('5', 'Troubleshooting - RGV', 'RGV 작업이 [35] 에서 오래 머문다 - 하역 완료 보고 유실');
  img(s, 'panel_seq_0758.png', 0.53, 1.4, 3.6, 3.3, [[10, 403, 410, 18, BLUE], [530, 335, 85, 22, RED]]);
  img(s, 'rtv_dlg_hmidel_1104.png', 4.25, 1.4, 2.4, 2.45, [[405, 102, 130, 95, RED]]);
  img(s, 'left_veh.png', 0.53, 4.9, 3.6, 1.15, [[2, 118, 520, 20, ORANGE]]);
  s.addText([
    { text: '상황 ③ (2026-10-14 시험)', options: { bold: true, color: BLUE, breakLine: true, paraSpaceAfter: 4 } },
    { text: '작업 [35] RGV 구동중, RGV 는 IDLE, 하역 칸(도착 트랙)에는 화물 감지만. 단계표 RGV 줄을 둔 채 [완료처리] → [35 → 39] → 도착 칸에 번호 기록 → 벨트가 이어서 옮김.', options: {} },
  ], { x: 4.25, y: 4.0, w: 2.4, h: 3.0, fontFace: FONT, fontSize: 9.5, color: '333333', isTextBox: true, margin: 3, fill: { color: 'F3F6FB' }, valign: 'top' });
  caption(s, '작업정보 판넬 - RGV 줄 / RTV 상태창 명령 그룹 / 설비 반송 칸의 RGV 줄', 0.53, 1.1, 6.1);
  rhs(s,
    [B('작업이 [35] RGV 구동중에 머문다. RGV 는 IDLE 로 돌아왔고 설비 반송 칸의 RGV 줄에는 작업이 남아 있다.'),
     B('도착 트랙(통로 RGV 측 / 출고대 쪽)에는 화물 감지만 있고 작업번호가 없다.')],
    [B('RGV 가 하역을 끝냈는데 하역 완료 보고가 ECS 에 오지 않았다. ECS 는 설비 신호로만 완료를 판정한다.')],
    [N('RGV 가 지시를 못 받은 것 같으면 RTV 상태창 [지시 재전송].'),
     N('이미 내려놓았으면 작업정보 판넬에서 그 작업을 고르고 단계표 RGV 줄을 둔 채 [완료처리] → [35 → 39] [예].'),
     N('RGV 지시가 풀리고 도착 칸에 작업번호가 기록되어 통로 벨트(입고) 또는 출고대(출고)로 이어진다.'),
     N('RGV 에 지난 데이터가 남아 다음 지시를 안 받으면 5-5 [데이터 삭제].')]);
}
// 5-4 작업 없음 (RGV 픽업 칸)
{
  const s = body('5', 'Troubleshooting - RGV', 'RGV 픽업 칸에 화물은 있는데 작업번호가 없다 - 작업 [16] 머묾');
  img(s, 'panel_seq_0758.png', 0.53, 1.4, 3.6, 3.3, [[10, 420, 410, 18, BLUE], [530, 335, 85, 22, RED]]);
  img(s, 'cv_0_v20.png', 4.25, 1.4, 2.4, 2.55, [[27, 100, 395, 60, RED], [428, 100, 125, 145, ORANGE]]);
  img(s, 'crop_layout_stn.png', 4.25, 4.1, 2.4, 1.9, [[300, 10, 100, 110, RED]]);
  s.addText([
    { text: '상황 ② (2026-10-14 시험)', options: { bold: true, color: BLUE, breakLine: true, paraSpaceAfter: 4 } },
    { text: '출고 작업 [16] 통로 CV 구동중, RGV 픽업 칸에 화물 감지만. 통로 C/V 줄을 골라 [완료처리] → [16 → 15] → "기록할까요?" [예] → RGV 지시.', options: {} },
  ], { x: 0.53, y: 4.85, w: 3.6, h: 2.1, fontFace: FONT, fontSize: 9.5, color: '333333', isTextBox: true, margin: 3, fill: { color: 'F3F6FB' }, valign: 'top' });
  caption(s, '작업정보 판넬 - 통로 C/V 줄 / 그 트랙 C/V 상태창 / 통로 RGV 측 트랙', 0.53, 1.1, 6.1);
  rhs(s,
    [B('출고 작업이 [16] 통로 CV 구동중에 머문다. 통로 끝(RGV 측) 트랙에 화물은 왔는데 번호가 없어 RGV 가 가지 않는다.')],
    [B('통로 벨트가 옮기는 사이 트래킹 번호가 소실되었다(3-2). 스케줄러는 번호가 맞는 화물에만 RGV 를 지시한다.')],
    [N('작업정보 판넬에서 그 작업을 고르고 단계표의 통로 C/V 줄을 클릭한다.'),
     N('[완료처리] → [16 → 15] [예] → "작업번호를 기록할까요?" [예]. 그 칸에 번호가 찍히고 RGV 지시가 나간다.'),
     N('반대로 번호가 앞 트랙에 남아 있으면 C/V 상태창 [잘라내기]/[붙여넣기] 로 옮겨도 된다(3-2 방법 2).')]);
}
// 5-5 데이터 잔존 → [데이터 삭제]
{
  const s = body('5', 'Troubleshooting - RGV', 'RGV 에 지난 데이터가 남아 새 지시를 안 받음 - [데이터 삭제]');
  img(s, 'rtv_dlg_hmidel_1104.png', 0.53, 1.4, 3.9, 4.0, [[405, 170, 130, 25, RED], [27, 345, 515, 150, ORANGE]]);
  img(s, 'rtv_hmidel_ask_1104.png', 4.5, 1.4, 2.15, 1.0);
  caption(s, 'RTV 상태창 - 명령 그룹의 [데이터 삭제] (2026-11-04 추가) / 작업상태 칸에 지난 작업', 0.53, 1.1, 6.1);
  s.addText([
    { text: 'S/C 와 같은 동작', options: { bold: true, color: BLUE, breakLine: true, paraSpaceAfter: 4 } },
    { text: '[예] → 설비 통신이 PLC 의 RGV 삭제 비트에 1 을 쓴다 = RGV 지상반 [삭제]. 끄는 것은 PLC 다.', options: { breakLine: true, paraSpaceAfter: 4 } },
    { text: '요청은 LOG ▷ 유저사용로그에, 처리 결과는 ECS 프로그램 로그(VEH_RTV "운전 명령 HMIDEL")에 남는다.', options: {} },
  ], { x: 4.5, y: 2.55, w: 2.15, h: 3.0, fontFace: FONT, fontSize: 9.5, color: '333333', isTextBox: true, margin: 3, fill: { color: 'F3F6FB' }, valign: 'top' });
  rhs(s,
    [B('RGV 가 비정상으로 멈춘 뒤 화물을 내려놓고 알람도 풀었는데 새 지시를 받지 않는다. 상태창 작업상태 칸에 지난 작업의 출발·도착이 남아 있다.')],
    [B('RGV 컨트롤러 안에 지난 작업 데이터가 남아 새 지시와 충돌한다.')],
    [N('RGV 위 화물을 먼저 내려놓는다.'),
     N('RTV 상태창 [데이터 삭제] → 확인창 [예]. "요청했습니다" 가 뜨면 끝.'),
     N('지상반에서 데이터가 지워졌는지 본다. 안 지워지면 종전대로 지상반 [삭제].'),
     N('작업까지 정리하려면 [완료처리] 를 이어서 쓴다(5-3 / 5-4).')]);
}

// ═════════ 6. ETC ═════════
// 6-1 통신 신호등
{
  const s = body('6', 'Troubleshooting - ETC', '통신 신호등이 빨강 - WMS1 · WMS2 · EQP · SCH');
  img(s, 'ribbon_comm.png', 0.53, 1.4, 2.6, 1.0);
  img(s, 'eqpinfo_0.png', 0.53, 2.95, 6.1, 2.0, [[40, 255, 775, 90, RED]]);
  img(s, 'chk_TASK_LFC10_G1_ECSCOM.png', 0.53, 5.1, 6.1, 1.95);
  caption(s, '리본 [통신] 신호등 (모든 탭에서 보임) / ECS ▷ 통신 연결 정의 / 서버 ALL_TASK 화면', 0.53, 1.1, 6.1);
  s.addText([
    { text: '신호등이 보는 것', options: { bold: true, color: BLUE, breakLine: true, paraSpaceAfter: 4 } },
    { text: 'WMS1 : 상위가 우리에게 붙는 수신 소켓(8001)', options: { breakLine: true } },
    { text: 'WMS2 : 우리가 상위로 보고하는 송신 소켓(8002)', options: { breakLine: true } },
    { text: 'EQP : 설비 통신이 마스터 PLC 에 붙어 있는가(한 소켓이 C/V·S/C·RGV 를 대표)', options: { breakLine: true } },
    { text: 'SCH : 스케줄러(IO TASK)가 살아 있는가', options: { breakLine: true, paraSpaceAfter: 4 } },
    { text: '위 빨강 = 끊김 / 노랑↔초록 1초 교대 = 정상(화면이 살아 있다는 뜻이기도)', options: {} },
  ], { x: 3.25, y: 1.48, w: 3.4, h: 1.4, fontFace: FONT, fontSize: 8.5, color: '333333', isTextBox: true, margin: 3, fill: { color: 'F3F6FB' }, valign: 'top' });
  rhs(s,
    [B('리본 [통신] 신호등 중 하나 이상이 위(빨강)에 멈춰 있다. EQP 빨강이면 설비 색이 갱신되지 않고, SCH 빨강이면 작업이 전혀 진행되지 않는다.')],
    [B('장비 전원·네트워크 끊김, 상대 프로그램(WMS · PLC)이 안 떠 있음, 우리 서버 TASK 가 죽음, DB 하트비트 끊김.')],
    [N('신호등을 누른다 - 상대에게 ping 4회를 보내는 창이 뜨고 결과가 요약된다.'),
     { t: '핑 응답 없음 → 장비 전원·케이블·IP (네트워크 담당)', bullet: true, indent: 1 },
     { t: '핑 응답 있음 / 포트 닫힘 → 상대 프로그램이 안 떠 있다 (WMS·PLC 담당)', bullet: true, indent: 1 },
     { t: '핑·포트 정상인데 빨강 → DB 하트비트만 끊김 → 그 TASK 재기동 (운영자)', bullet: true, indent: 1 },
     { t: '우리가 여는 포트(8001) 닫힘 → HOST TASK 기동 (운영자)', bullet: true, indent: 1 },
     N('모든 신호등이 빨강이고 값이 안 바뀌면 DB(SQL 서비스) 쪽 - 운영자(01 운영자 매뉴얼 9장).')]);
}
// 6-2 알람 경고창 / 설비 에러 이력
{
  const s = body('6', 'Troubleshooting - ETC', '설비 알람 - 알람 경고창 · 상태창 에러 · 설비에러이력');
  img(s, 'sc_dlg_hmidel_1104.png', 0.53, 1.4, 2.6, 3.0, [[27, 565, 495, 42, RED]]);
  img(s, 'cv_0_v20.png', 3.25, 1.4, 2.6, 2.6, [[27, 412, 530, 50, RED]]);
  img(s, 'log_eqp_his_0.png', 0.53, 5.3, 6.1, 1.75, [[40, 55, 1040, 150, ORANGE]]);
  caption(s, 'S/C · C/V 상태창의 에러 칸 ([코드] 문구) / LOG ▷ 설비에러이력 (EQP_ERR_HIS, 30일)', 0.53, 1.1, 6.1);
  s.addText([
    { text: '알람 경고창', options: { bold: true, color: BLUE, breakLine: true, paraSpaceAfter: 3 } },
    { text: '설비 에러가 나면 화면 오른쪽 아래에 스스로 뜬다(10초마다 확인). 해제되면 목록에서 빠진다. [STOP] 은 소리만 끄고 창을 감춘다. 알람은 설비 에러일 때만 뜬다 - 오래 멈춘 화물(체류)은 작업정보 판넬에서 본다.', options: {} },
  ], { x: 3.25, y: 4.2, w: 3.4, h: 0.95, fontFace: FONT, fontSize: 8.5, color: '333333', isTextBox: true, margin: 3, fill: { color: 'F3F6FB' }, valign: 'top' });
  rhs(s,
    [B('화면 오른쪽 아래에 알람 경고창이 뜨고 소리가 난다. 설비 칸·트랙이 에러색이 된다.'),
     B('상태창 에러 칸에 [코드] 문구(PLC 알람 리스트 기준, 크레인은 SC_LGLS)가 보인다.')],
    [B('설비(PLC) 가 올린 알람. C/V 는 모터 과부하 하나, S/C · RGV 는 알람 리스트의 코드.'),
     B('3·5호기 유령 에러(실제 알람 없음)는 4-3.')],
    [N('경고창의 설비와 코드를 보고 조작반·지상반에서 원인을 없앤 뒤 알람을 해제한다. 목록에서 빠지면 된다.'),
     N('[STOP] 은 소리만 끈다 - 알람이 풀린 것이 아니다.'),
     N('언제 어떤 알람이 났는지는 LOG ▷ 설비에러이력(30일) 에서 설비·코드·시간으로 조회한다. 잦은 알람 설비는 운영자에게.'),
     N('알람이 풀린 뒤에도 작업이 안 흐르면 4-5 / 5-3 ([지시 재전송] · [완료처리]).')]);
}
// 6-3 이중입고 / 공출고
{
  const s = body('6', 'Troubleshooting - ETC', '상위(WMS) 지시가 거부된다 - 이중입고(54) · 공출고(58)');
  img(s, 'log_mes_0.png', 0.53, 1.4, 6.1, 3.6, [[40, 265, 995, 200, ORANGE]]);
  img(s, 'joblist_0.png', 0.53, 5.2, 6.1, 1.85, [[40, 320, 1160, 60, RED]]);
  caption(s, 'LOG ▷ HOST 로그 (MES_IF_LOG) - 상위와 주고받은 전문 / 작업정보 창 - 거부된 지시는 목록에 없다', 0.53, 1.1, 6.1);
  rhs(s,
    [B('상위(WMS/IMS)가 보낸 입고·출고 지시가 작업정보 목록에 생기지 않고, HOST 로그에 응답 코드 54 또는 58 로 거부한 전문이 남는다.')],
    [B('54 이중입고 : 지시한 랙 위치에 ECS 재고(화물)가 이미 있다.'),
     B('58 공출고 : 지시한 랙 위치가 ECS 재고에서 비어 있다.'),
     B('ECS 와 상위의 재고가 어긋난 것이다 - 수동 입출고, [완료처리] 로 넘긴 작업의 위치 차이, 작업 삭제 등.')],
    [N('LOG ▷ HOST 로그에서 거부 전문의 위치(LOC)와 코드를 확인한다.'),
     N('현장 실물을 확인하고 상위(WMS) 담당자에게 알려 다른 위치로 재지시받는다. ECS 는 상위 지시를 임의로 고치지 않는다.'),
     N('실물과 ECS 재고가 다르면 운영자에게 - 재고 정정은 DB 에서 한다(01 운영자 매뉴얼).'),
     B('반자동(수동지시) 작업은 상위에 보고하지 않고 끝나면 지워지므로 재고 차이의 원인이 될 수 있다.', { color: RED })]);
}
// 6-4 [CV 지우기]
{
  const s = body('6', 'Troubleshooting - ETC', '모든 C/V 트래킹을 한 번에 지운다 - MANUAL ▷ [CV 지우기]');
  img(s, 'crop_ribbon_manual.png', 0.53, 1.4, 6.1, 1.3, [[55, 25, 55, 75, RED]]);
  img(s, 'cvclr_ask_1104.png', 0.53, 2.95, 3.2, 1.45);
  img(s, 'cvtask_main.png', 0.53, 4.55, 6.1, 2.45, [[690, 460, 490, 70, ORANGE]]);
  caption(s, '리본 MANUAL ▷ 수동조작 ▷ [CV 지우기] (2026-11-04 추가) / 확인창 / 서버 설비 통신(EQP) 로그', 0.53, 1.1, 6.1);
  s.addText([
    { text: '왜 ECS 가 직접 못 지우나', options: { bold: true, color: BLUE, breakLine: true, paraSpaceAfter: 4 } },
    { text: 'C/V 트래킹(R 영역)은 문자(ASCII)다. ECS 가 "0" 을 써도 0x30 이 남아 PLC 는 "값 있음" 으로 본다. 그래서 PLC 가 지우도록 비트(M) 하나에 1 을 보낸다.', options: {} },
  ], { x: 3.85, y: 2.95, w: 2.8, h: 1.45, fontFace: FONT, fontSize: 9, color: '333333', isTextBox: true, margin: 3, fill: { color: 'F3F6FB' }, valign: 'top' });
  rhs(s,
    [B('여러 트랙에 지난 작업번호가 흩어져 남아(3-2 가 여럿) 하나씩 지우기 어렵다. 설비를 비우고 처음부터 정리하려 한다.')],
    [B('수동 운전 · 시운전 · 비상 정지 뒤 PLC R 영역에 작업번호 찌꺼기가 남는다. 트랙마다 상태창 [삭제] 를 누르는 대신 PLC 가 한 번에 지운다.')],
    [N('설비 위에 반송 중인 화물이 없는지 확인한다. 있으면 그 화물의 번호도 같이 지워진다.'),
     N('MANUAL ▷ 수동조작 ▷ [CV 지우기] → 확인창 [예].'),
     N('"요청했습니다" 가 뜨면 설비 통신이 PLC 의 CV 비트에 1 을 쓴다. 트랙 글자가 모두 사라졌는지 본다.'),
     N('작업정보에 남은 작업은 [완료처리] 또는 작업정보 창 [삭제] 로 정리한다.'),
     B('비트 주소가 WCS_DB.INI [HMI_DELETE] 에 없으면 이력만 남고 PLC 에는 쓰지 않는다(운영자가 기입).', { color: GRAY })]);
}
// 6-5 화면 느림 · 안 바뀜
{
  const s = body('6', 'Troubleshooting - ETC', '운전 화면이 느리다 · 값이 안 바뀐다 · 아이콘이 없다');
  img(s, 'crop_ribbon_ecs.png', 0.53, 1.4, 6.1, 1.1, [[480, 25, 150, 80, RED]]);
  img(s, 'main_1029.png', 0.53, 2.7, 6.1, 3.3, [[0, 0, 1936, 40, ORANGE]]);
  caption(s, '리본 [통신] 신호등 교대 = 화면이 살아 있다 / 제목줄 = 빌드 시각 · 붙은 DB · 실행 경로', 0.53, 1.1, 6.1);
  // (subtitle 가 길어 줄바꿈되지 않도록 본문 쪽 제목은 body() 에서 12.5pt)
  s.addText([
    { text: '제목줄로 확인할 것', options: { bold: true, color: BLUE, breakLine: true, paraSpaceAfter: 4 } },
    { text: 'Ecs V1.0 [Build:날짜 시각] [DB:LGLS_MCS_IO@서버,포트] [경로] - 현장에 올린 판이 맞는지, DB 가 서버를 가리키는지 여기서 본다.', options: {} },
  ], { x: 0.53, y: 6.1, w: 6.1, h: 0.9, fontFace: FONT, fontSize: 9, color: '333333', isTextBox: true, margin: 3, fill: { color: 'F3F6FB' }, valign: 'top' });
  rhs(s,
    [B('리본 신호등이 교대하지 않고 설비 색·작업 목록이 바뀌지 않는다. 또는 화면 반응이 느리다.'),
     B('아이콘 없이 뜨거나 설치 때 "액세스 거부" 가 난다.')],
    [B('운전 화면 프로그램이 멈춤, DB 연결 끊김, 다른 PC 의 DB 를 가리킴. 설비 색은 설비 통신 한 바퀴(약 1초) + 화면 수집 주기(200ms) 만큼 늦는 것이 정상.'),
     B('아이콘 없음 = 재시작 관리자로 떠서 작업 폴더가 바뀐 것(10-15 판에서 보정). 설치 거부 = 폰트 파일 잠금.')],
    [N('운전 화면만 닫고 다시 띄운다(런처 Ecs.exe). 창고는 서버 TASK 가 돌리므로 그대로 돈다.'),
     N('제목줄의 DB 가 서버가 아니면 설정이 잘못된 것 - 운영자에게.'),
     N('2~3초 이상 늦거나 모든 신호등이 빨강이면 6-1 / 운영자(SQL 서비스 · 디스크).'),
     N('아이콘·설치 문제는 01 운영자 매뉴얼 2.3 (작업 디렉터리 · FR_PRIVATE 폰트).')]);
}

// ───────── 저장 ─────────
pres.writeFile({ fileName: OUT }).then(f => console.log('saved', f, 'slides', pageNo));
