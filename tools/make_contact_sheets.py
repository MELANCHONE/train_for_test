from pathlib import Path
from PIL import Image, ImageDraw, ImageFont

root = Path(__file__).resolve().parents[1]
files = sorted((root / 'pitctures').glob('*.jpg'))
files = [f for f in files if 'a935a6faaffa' in f.name]
out = root / 'tmp_contact'
out.mkdir(exist_ok=True)
font = ImageFont.truetype('C:/Windows/Fonts/arial.ttf', 24)
for start in range(0, len(files), 4):
    sheet = Image.new('RGB', (1840, 2240), 'white')
    draw = ImageDraw.Draw(sheet)
    for offset, f in enumerate(files[start:start + 4]):
        im = Image.open(f).convert('RGB')
        im = im.crop((0, 180, im.width, min(im.height, 1350)))
        im.thumbnail((900, 1050))
        x = (offset % 2) * 920 + 10
        y = (offset // 2) * 1120 + 40
        draw.text((x, y - 34), f'{start + offset:02d}', fill='black', font=font)
        sheet.paste(im, (x, y))
    sheet.save(out / f'{start:02d}-{min(start + 3, len(files) - 1):02d}.jpg', quality=90)
