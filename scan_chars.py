#!/usr/bin/env python3
# -*- coding: utf-8 -*-

FILENAME = "testBase4.dat"   # при необходимости поменяй имя файла

def main():
    with open(FILENAME, "rb") as f:
        data = f.read()

    unique_bytes = sorted(set(data))
    print(f"Всего байт в файле: {len(data)}")
    print(f"Уникальных байт: {len(unique_bytes)}\n")

    print(f"{'HEX':>4} {'DEC':>4} {'CP866':>8}  Описание")
    print("-" * 40)

    for b in unique_bytes:
        ch_desc = ""
        try:
            ch = bytes([b]).decode("cp866")
        except UnicodeDecodeError:
            ch = "�"

        # описание символа
        if b == 0x20:
            ch_desc = "пробел"
        elif 32 < b < 127:
            ch_desc = f"печатаемый ASCII '{ch}'"
        elif b < 32:
            ch_desc = "управляющий символ"
        else:
            ch_desc = "расширенный CP866"

        # аккуратный вывод, чтобы не сломать терминал
        if ch == "\n":
            ch_show = "\\n"
        elif ch == "\r":
            ch_show = "\\r"
        elif ch == "\t":
            ch_show = "\\t"
        elif ch == " ":
            ch_show = " "
        else:
            ch_show = ch

        print(f"0x{b:02X} {b:4d} {ch_show!r:>8}  {ch_desc}")

if __name__ == "__main__":
    main()
