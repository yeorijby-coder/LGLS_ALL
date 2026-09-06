# -*- coding: utf-8 -*-
"""[LGLS 2026-09-06] EQP_SIM / HOST_SIM 응용 아이콘 생성.

색 언어는 산출물 문서(07/09 흐름도)와 맞춘다.
  배경 NAVY #1E2761 / 설비계 AMBER #E8A33D / 상위계 BLUE #4F9BE8 / 보조 ICE #CADCFC

각 크기를 8배 캔버스에 그린 뒤 LANCZOS 로 축소한다(수퍼샘플링) - 16px 에서도 가장자리가 깨끗하다.
"""
import io, sys, os
sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8')
from PIL import Image, ImageDraw

NAVY  = (0x1E, 0x27, 0x61, 255)
AMBER = (0xE8, 0xA3, 0x3D, 255)
BLUE  = (0x4F, 0x9B, 0xE8, 255)
ICE   = (0xCA, 0xDC, 0xFC, 255)
DARK  = (0x14, 0x1A, 0x42, 255)

SIZES = [256, 128, 64, 48, 32, 16]
SS = 8          # 수퍼샘플 배율


def bg(d, S):
    """네이비 라운드 사각 배경"""
    m = S * 0.02
    d.rounded_rectangle([m, m, S - m, S - m], radius=S * 0.20, fill=NAVY)


def draw_eqp(S):
    """설비 : 컨베이어 위의 파렛트"""
    img = Image.new('RGBA', (S, S), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    bg(d, S)

    # 파렛트(화물) - 앰버 블록
    d.rounded_rectangle([S * 0.22, S * 0.22, S * 0.78, S * 0.52],
                        radius=S * 0.05, fill=AMBER)
    # 파렛트 다리(스트링거) 2개 - 어둡게 파서 '파렛트' 로 읽히게
    d.rectangle([S * 0.35, S * 0.44, S * 0.42, S * 0.52], fill=DARK)
    d.rectangle([S * 0.58, S * 0.44, S * 0.65, S * 0.52], fill=DARK)

    # 컨베이어 벨트
    d.rounded_rectangle([S * 0.13, S * 0.60, S * 0.87, S * 0.70],
                        radius=S * 0.05, fill=ICE)
    # 롤러 3개
    r = S * 0.055
    for cx in (0.28, 0.50, 0.72):
        d.ellipse([S * cx - r, S * 0.775 - r, S * cx + r, S * 0.775 + r], fill=ICE)
    return img


def draw_host(S):
    """상위 : 서버 랙 + 상향 통신"""
    img = Image.new('RGBA', (S, S), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    bg(d, S)

    # 상향 통신 화살표(상위로 보고)
    d.polygon([(S * 0.50, S * 0.13), (S * 0.63, S * 0.27), (S * 0.37, S * 0.27)], fill=ICE)

    # 서버 2단
    for y0 in (0.36, 0.60):
        d.rounded_rectangle([S * 0.18, S * y0, S * 0.82, S * (y0 + 0.16)],
                            radius=S * 0.04, fill=BLUE)
        # LED
        r = S * 0.028
        cy = S * (y0 + 0.08)
        d.ellipse([S * 0.26 - r, cy - r, S * 0.26 + r, cy + r], fill=NAVY)
        # 슬롯 선
        d.rectangle([S * 0.40, cy - S * 0.012, S * 0.74, cy + S * 0.012], fill=NAVY)
    return img


def build(fn, out):
    frames = []
    for s in SIZES:
        big = fn(s * SS)
        frames.append(big.resize((s, s), Image.LANCZOS))
    frames[0].save(out, format='ICO', sizes=[(s, s) for s in SIZES],
                   append_images=frames[1:])
    print('  %s  (%d bytes, %s)' % (out, os.path.getsize(out),
                                    ' '.join('%dx%d' % (s, s) for s in SIZES)))
    # 미리보기 PNG
    prev = fn(256 * 2).resize((256, 256), Image.LANCZOS)
    pv = os.path.splitext(out)[0] + '_preview.png'
    prev.save(pv)
    return pv


SP = r'C:\Users\USER\AppData\Local\Temp\claude\D--project-LGLS-Renewal\ae567b2e-9fb6-418b-b143-f780d085c599\scratchpad'
print('아이콘 생성')
p1 = build(draw_eqp,  os.path.join(SP, 'eqp_sim.ico'))
p2 = build(draw_host, os.path.join(SP, 'host_sim.ico'))

# 16/32/48 실제 크기 나란히 보기
strip = Image.new('RGBA', (16 + 32 + 48 + 64 + 40, 64), (245, 245, 248, 255))
x = 0
for fn in (draw_eqp, draw_host):
    for s in (16, 32, 48):
        im = fn(s * SS).resize((s, s), Image.LANCZOS)
        strip.paste(im, (x, (64 - s) // 2), im)
        x += s + 6
    x += 10
strip = strip.resize((strip.width * 3, strip.height * 3), Image.NEAREST)
sp = os.path.join(SP, 'icons_sizes.png')
strip.save(sp)
print('  크기 비교 : %s' % sp)
